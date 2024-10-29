#include "stdafx.h"
#include "Diablo.h"
#include "Protection.h"
#include "Path.h"
#include "GuildClass.h"
#include "Guild.h"
#include "Notice.h"
#include "ItemManager.h"
#include "Message.h"
#include "Util.h"
#include "LuckyCoin.h"
#include "GameMain.h"
#include "Gate.h"
#include "Map.h"
#include "Monster.h"
#include "MonsterSetBase.h"
#include "ScheduleManager.h"
#include "ItemBagManager.h"
#include "CashShop.h"

cDiablo gDiablo;

cDiablo::cDiablo()
{
	this->m_RegStruct.clear();
	this->m_DiabloInfo.State = DIABLO_STATE_BLANK;
	this->m_DiabloInfo.AlarmMinLeft = 0;
	this->m_DiabloInfo.AlarmMinSave = 0;
	this->m_DiabloInfo.TickCount = GetTickCount();
	this->m_DiabloInfo.UpdateTick = 0;
	this->m_DiabloInfo.RemainTime = 0;
	this->m_DiabloInfo.TargetTime = 0;
	this->m_DBDataLoading = 0;
	this->m_DBDataLoadOK = 0;
	this->m_DiabloObjIndex = -1;
}

cDiablo::~cDiablo()
{
	
}

void cDiablo::Init()
{
	if(this->m_DiabloSwitch == 0)
	{
		this->SetState(&this->m_DiabloInfo,DIABLO_STATE_BLANK);
	}
	else
	{
		this->SetState(&this->m_DiabloInfo,DIABLO_STATE_EMPTY);
	}
}

void cDiablo::Load()
{
	if (gProtection.GetCustomState(CUSTOM_DIABLO) == 0)
	{
		return;
	}

	this->LoadIni(gPath.GetFullPath("..\\LTPData\\DiabloConf.ini"));
}

void cDiablo::LoadIni(char* path)
{
	this->m_DiabloSwitch = GetPrivateProfileInt("Global","Enable",0,path);
	this->m_NpcID = GetPrivateProfileInt("NPC","NPCID",0,path);
	this->m_NpcMap = GetPrivateProfileInt("NPC","NPCMap",0,path);
	this->m_NpcX = GetPrivateProfileInt("NPC","NPCX",0,path);
	this->m_NpcY = GetPrivateProfileInt("NPC","NPCY",0,path);
	this->m_AlarmTime = GetPrivateProfileInt("Global","AlarmTime",0,path);
	this->m_StandTime = GetPrivateProfileInt("Global","StandTime",0,path);
	this->m_EventTime = GetPrivateProfileInt("Global","EventTime",0,path);
	this->m_CloseTime = GetPrivateProfileInt("Global","CloseTime",0,path);
	this->m_PassiveTime = GetPrivateProfileInt("Global","PassiveTime",0,path);
	
	this->m_WCoinCPrize = GetPrivateProfileInt("Prize","WCoinC",0,path);
	this->m_WCoinPPrize = GetPrivateProfileInt("Prize","WCoinP",0,path);
	this->m_PrizeType = GetPrivateProfileInt("Prize","Type",0,path);

	this->TimeInfo.StartHour = GetPrivateProfileInt("Time","StartHour",0,path);
	this->TimeInfo.StartMinutes = GetPrivateProfileInt("Time","StartMinutes",0,path);

	this->m_ChangeStartReg = GetPrivateProfileInt("ChangeDate","ChangeStartReg",0,path);
	this->m_ChangeEndReg = GetPrivateProfileInt("ChangeDate","ChangeEndReg",0,path);
	this->m_ChangeStartEvent = GetPrivateProfileInt("ChangeDate","ChangeStartEvent",0,path);
	//m_DiabloInfo.time
	// ----
}

bool cDiablo::NPCTalk(LPOBJ lpNpc, LPOBJ lpObj)
{
	if (gProtection.GetCustomState(CUSTOM_DIABLO) == 0)
	{
		return false;
	}

	if(		lpNpc->Class		== this->m_NpcID 
		&&	lpNpc->Map			== this->m_NpcMap
		&&	lpNpc->X			== this->m_NpcX
		&&	lpNpc->Y			== this->m_NpcY )
	{
		if (this->m_DiabloSwitch == 0)
		{
			gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, "Diablo disabled");
			return false;
		}
		this->SendDialog(lpObj, DIALOG_SEND);
		return true;
	}
	return false;
}

void cDiablo::SendDialog(LPOBJ lpObj, eDiabloDialog type)
{
	if (gProtection.GetCustomState(CUSTOM_DIABLO) == 0)
	{
		return;
	}

	DIABLO_REQ pRequest;
	pRequest.Head.set(0xFD, sizeof(pRequest));
	// ----
	pRequest.Type = type;
	pRequest.Count = this->GetRegCountByUserIndex(lpObj->Index);
	pRequest.StartYear = this->TimeInfo.StartRegYear;
	pRequest.StartMonth = this->TimeInfo.StartRegMonth;
	pRequest.StartDay = this->TimeInfo.StartRegDay;
	pRequest.EndYear = this->TimeInfo.EndRegYear;
	pRequest.EndMonth = this->TimeInfo.EndRegMonth;
	pRequest.EndDay = this->TimeInfo.EndRegDay;
	pRequest.StartEventYear = this->TimeInfo.StartYear;
	pRequest.StartEventMonth = this->TimeInfo.StartMonth;
	pRequest.StartEventDay = this->TimeInfo.StartDay;
	sprintf(pRequest.StartTime, "%d:%d", this->TimeInfo.StartHour, this->TimeInfo.StartMinutes);
	// ----
	for (int i=0; i<m_RegStruct.size(); i++)
	{
		sprintf(pRequest.GuildName[i], "%s", m_RegStruct[i].GuildName);
		pRequest.GuildRank[i] = m_RegStruct[i].RegisteredRena;
	}
	pRequest.GuildCount = m_RegStruct.size();
	// ----
	DataSend(lpObj->Index, (LPBYTE)&pRequest, sizeof(pRequest));
}


void cDiablo::UpdateRena(int aIndex)
{
	if (gProtection.GetCustomState(CUSTOM_DIABLO) == 0)
	{
		return;
	}

	DIABLO_RENARECV pRequest;
	pRequest.Head.set(0xFB,0x02, sizeof(pRequest));
	// ----
	pRequest.Count = this->GetRegCountByUserIndex(aIndex);
	// ----
	DataSend(aIndex, (LPBYTE)&pRequest, sizeof(pRequest));
}

void cDiablo::RegisterRena(DIABLO_RENARECV* lpMsg, int aIndex)
{
	if (gProtection.GetCustomState(CUSTOM_DIABLO) == 0)
	{
		return;
	}

	if (this->m_DiabloSwitch == 0)
	{
		gNotice.GCNoticeSend(aIndex, 1, 0, 0, 0, 0, 0, "Diablo disabled");
		return;
	}

	SYSTEMTIME time;
	GetLocalTime(&time);
	if ((time.wYear < this->TimeInfo.StartRegYear || time.wMonth < this->TimeInfo.StartRegMonth || time.wDay < this->TimeInfo.StartRegDay) ||
		(time.wYear > this->TimeInfo.EndRegYear || time.wMonth > this->TimeInfo.EndRegMonth || time.wDay > this->TimeInfo.EndRegDay))
	{
		gNotice.GCNoticeSend(aIndex, 1,0,0,0,0,0,"Register period is over!");
		return;
	}
	//if( time.wSecond == 0 && time.wMilliseconds <= 10 && time.wMinute == 0)
	//{
		//if (this->m_DiabloInfo.State == DIABLO_STATE_EMPTY)
		//{
		//	this->GDUpdateInfo();
		//}
	//}

	LPOBJ lpObj = &gObj[aIndex];

	if(lpObj->GuildNumber <= 0)
	{
		gNotice.GCNoticeSend(lpObj->Index, 1,0,0,0,0,0,"You must be in a guild, to register Rena!");
		return;
	}

	lpObj->ChaosLock = 1;
	int count = gItemManager.GetInventoryItemCount(lpObj,GET_ITEM(14,21), 0);

	if (count < 1)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,"You don't have \"Rena\"!");
		PMSG_LUCKY_COIN_REGISTER_SEND pMsg;

		pMsg.header.set(0xBF,0x0C,sizeof(pMsg));

		pMsg.result = 0;

		pMsg.LuckyCoin = 0;

		DataSend(lpObj->Index,(BYTE*)&pMsg,pMsg.header.size);

		lpObj->ChaosLock = 0;
		return;
	}

	gItemManager.DeleteInventoryItemCount(lpObj,GET_ITEM(14,21),0,1);

	lpObj->ChaosLock = 0;

	this->AddRena(lpMsg->Count, aIndex);
	// ----
	sort(this->m_RegStruct.begin(), this->m_RegStruct.end());
	// ----
	this->UpdateRena(aIndex);
}

void cDiablo::Teleport(DIABLO_RENARECV* lpMsg, int aIndex)
{
	if (gProtection.GetCustomState(CUSTOM_DIABLO) == 0)
	{
		return;
	}

	if (this->m_DiabloSwitch == 0)
	{
		gNotice.GCNoticeSend(aIndex, 1, 0, 0, 0, 0, 0, "Diablo disabled");
		return;
	}
	DIABLO_INFO* lpInfo = &this->m_DiabloInfo;

	if (lpInfo->EnterEnabled == 0)
	{
		gNotice.GCNoticeSend(aIndex, 1, 0, 0, 0, 0, 0, "Diablo is not currently open");
		return;
	}


	LPOBJ lpObj = &gObj[aIndex];

	if (memcmp(this->m_RegStruct[0].GuildName, lpObj->GuildName, strlen(this->m_RegStruct[0].GuildName)) != 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,"You cannot use teleport!");
		return;
	}

	gObjMoveGate(lpObj->Index, 346);

	//this->m_DiabloInfo.m_Users[this->m_DiabloInfo.UsersCount].Index = lpObj->Index;
	//this->m_DiabloInfo.m_Users[this->m_DiabloInfo.UsersCount].OnMap = 1;
	//this->m_DiabloInfo.m_Users[this->m_DiabloInfo.UsersCount].Time = 0;
	//this->m_DiabloInfo.UsersCount += 1;
	this->AddUser(lpInfo, lpObj->Index);

	gNotice.GCNoticeSend(lpObj->Index, 0, 0, 0, 0, 0, 0, "Welcome to Diablo. UserCount [%d]", this->GetUserCount(lpInfo));
}

void cDiablo::AddRena(int Count, int aIndex)
{
	if (gProtection.GetCustomState(CUSTOM_DIABLO) == 0)
	{
		return;
	}

	if (this->m_DiabloSwitch == 0)
	{
		gNotice.GCNoticeSend(aIndex, 1, 0, 0, 0, 0, 0, "Diablo disabled");
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];

	for(std::vector<REGISTERED_GUILD_INFO>::iterator it=this->m_RegStruct.begin();it != this->m_RegStruct.end();it++)
	{
		if (!memcmp(it->GuildName, lpObj->GuildName, strlen(it->GuildName)))
		{
			it->RegisteredRena += Count;
			//LogAdd(LOG_RED, "Guild [%d] Registered - %d Rena", lpObj->GuildName, it->RegisteredRena);
			return;
		}
	}
	
	REGISTERED_GUILD_INFO info;

	memcpy(info.GuildName,lpObj->GuildName,sizeof(info.GuildName));

	info.RegisteredRena = 0;
	info.RegisteredRena += Count;

	this->m_RegStruct.push_back(info);
}

int cDiablo::GetRegCountByUserIndex(int aIndex)
{
	if (gProtection.GetCustomState(CUSTOM_DIABLO) == 0)
	{
		return 0;
	}

	LPOBJ lpObj = &gObj[aIndex];

	for(std::vector<REGISTERED_GUILD_INFO>::iterator it=this->m_RegStruct.begin();it != this->m_RegStruct.end();it++)
	{
		if (!memcmp(it->GuildName, lpObj->GuildName, strlen(it->GuildName)))
		{
			return it->RegisteredRena;
		}
	}
	return 0;
}

void cDiablo::SendGetGuildInfo()
{
	if (gProtection.GetCustomState(CUSTOM_DIABLO) == 0)
	{
		return;
	}

	this->m_DBDataLoading = 1;
	DIABLO_RENARECV pMsg;
	pMsg.Head.set(0xFB,0x00,sizeof(pMsg));
	pMsg.Count = 0;

	gDataServerConnection.DataSend((BYTE*)&pMsg,pMsg.Head.size);
}

void cDiablo::DGGetGuildList(DG_DIABLO_GUILDLIST* aRecv)
{
	if (gProtection.GetCustomState(CUSTOM_DIABLO) == 0)
	{
		return;
	}

	this->m_RegStruct.clear();
	for (int i = 0; i < aRecv->Count; i++)
	{
		REGISTERED_GUILD_INFO info;

		memcpy(info.GuildName,aRecv->GUILD_NAME[i],sizeof(aRecv->GUILD_NAME[i]));

		info.RegisteredRena = aRecv->RENA_COUNT[i];

		this->m_RegStruct.push_back(info);
	}

	if (aRecv->Time.Result == 1)
	{
		this->m_DiabloSwitch = 0;
	}


	this->TimeInfo.StartRegYear = aRecv->Time.StartRegYear;
	this->TimeInfo.StartRegMonth = aRecv->Time.StartRegMonth;
	this->TimeInfo.StartRegDay = aRecv->Time.StartRegDay;

	this->TimeInfo.EndRegYear = aRecv->Time.EndRegYear;
	this->TimeInfo.EndRegMonth = aRecv->Time.EndRegMonth;
	this->TimeInfo.EndRegDay = aRecv->Time.EndRegDay;

	this->TimeInfo.StartYear = aRecv->Time.StartYear;
	this->TimeInfo.StartMonth = aRecv->Time.StartMonth;
	this->TimeInfo.StartDay = aRecv->Time.StartDay;

	//LogAdd(LOG_RED, "%d %d %d | %d %d %d | %d %d %d", aRecv->Time.StartRegYear, aRecv->Time.StartRegMonth, aRecv->Time.StartRegDay, aRecv->Time.EndRegYear, aRecv->Time.EndRegMonth, aRecv->Time.EndRegDay, aRecv->Time.StartYear, aRecv->Time.StartMonth, aRecv->Time.StartDay);

	
	this->m_DBDataLoadOK = 1;
	this->Init();

	//this->m_DiabloInfo.State = DIABLO_STATE_EMPTY;
}

void cDiablo::MainProc()
{
	if (gProtection.GetCustomState(CUSTOM_DIABLO) == 0)
	{
		return;
	}

	if (this->m_DiabloSwitch == 0)
	{
		return;
	}

	if(gDiablo.m_DBDataLoadOK == 0 && gDiablo.m_DBDataLoading == 0)
	{
		gDiablo.SendGetGuildInfo();
	}

	DIABLO_INFO* lpInfo = &this->m_DiabloInfo;

	lpInfo->RemainTime = (int)difftime(lpInfo->TargetTime,time(0));

	switch(lpInfo->State)
			{
			case DIABLO_STATE_BLANK:
				this->ProcState_BLANK(lpInfo);
				break;
			case DIABLO_STATE_EMPTY:
				this->ProcState_EMPTY(lpInfo);
				break;
			case DIABLO_STATE_STAND:
				this->ProcState_STAND(lpInfo);
				break;
			case DIABLO_STATE_START:
				this->ProcState_START(lpInfo);
				break;
			case DIABLO_STATE_CLEAN:
				this->ProcState_CLEAN(lpInfo);
				break;
		}

	if((GetTickCount()-lpInfo->TickCount) >= 1000)
	{
			lpInfo->TickCount = GetTickCount();
			lpInfo->UpdateTick++;

			if (lpInfo->UpdateTick == 3600)
			{
				this->GDUpdateInfo();
				lpInfo->UpdateTick = 0;
			}
	}

	//LogAdd(LOG_RED, "Time");
	//SYSTEMTIME time;
	//GetLocalTime(&time);
	//if( time.wSecond == 0 && time.wMilliseconds <= 10 && time.wMinute == 0)
	//{
		//if (this->m_DiabloInfo.State == DIABLO_STATE_EMPTY)
		//{
		//	this->GDUpdateInfo();
		//}
	//}
}

void cDiablo::ProcState_BLANK(DIABLO_INFO* lpInfo) // OK
{
	//LogAdd(LOG_RED, __FUNCTION__);
}

void cDiablo::ProcState_EMPTY(DIABLO_INFO* lpInfo) // OK
{
	//LogAdd(LOG_RED, __FUNCTION__);
	if(lpInfo->RemainTime > 0 && lpInfo->RemainTime <= (this->m_AlarmTime*60))
	{
		this->CheckUser(lpInfo);

		if((lpInfo->AlarmMinSave=(((lpInfo->RemainTime%60)==0)?((lpInfo->RemainTime/60)-1):(lpInfo->RemainTime/60))) != lpInfo->AlarmMinLeft)
		{
			lpInfo->AlarmMinLeft = lpInfo->AlarmMinSave;

			gNotice.GCNoticeSendToAll(0,0,0,0,0,0,"[Diablo] will open in %d minute(s)",(lpInfo->AlarmMinLeft+1));
		}
	}

	if(lpInfo->RemainTime <= 0)
	{
		lpInfo->EnterEnabled = 1;
		//gNotice.GCNoticeSendToAll(0,0,0,0,0,0,"[Diablo] closed");
		//this->NoticeSendToAll(lpInfo,1,"[Diablo] started",this->m_StandTime);
		gNotice.GCNoticeSendToAll(0, 0, 0, 0, 0, 0, "[Diablo] started");
		this->SetState(lpInfo,DIABLO_STATE_STAND);
	}
}

void cDiablo::ProcState_STAND(DIABLO_INFO* lpInfo) // OK
{
	//LogAdd(LOG_RED, __FUNCTION__);
	this->CheckUser(lpInfo);

	/*if(this->GetUserCount(lpInfo) < ((lpInfo->RuleInfo.MinUser==-1)?1:lpInfo->RuleInfo.MinUser))
	{
		this->SetState(lpInfo,CUSTOM_ARENA_STATE_EMPTY);
		return;
	}*/

	if(lpInfo->RemainTime <= 0)
	{
		this->NoticeSendToAll(lpInfo,0,"[Diablo] attacks");
		this->SetState(lpInfo,DIABLO_STATE_START);
	}
}

void cDiablo::ProcState_START(DIABLO_INFO* lpInfo) // OK
{
	//LogAdd(LOG_RED, __FUNCTION__);
	this->CheckUser(lpInfo);
	lpInfo->EventTime++;
	/*if(this->GetUserCount(lpInfo) == 0)
	{
		this->SetState(lpInfo,CUSTOM_ARENA_STATE_EMPTY);
		return;
	}*/

	/*if(this->GetUserCount(lpInfo) == 1)
	{
		this->SetState(lpInfo,CUSTOM_ARENA_STATE_CLEAN);
		return;
	}*/

	if(lpInfo->RemainTime <= 0 || this->m_DiabloObjIndex == -1)
	{
		this->NoticeSendToAll(lpInfo,0,"[Diablo] finished");
		this->SetState(lpInfo,DIABLO_STATE_CLEAN);
	}
}

void cDiablo::ProcState_CLEAN(DIABLO_INFO* lpInfo) // OK
{
	//LogAdd(LOG_RED, __FUNCTION__);
	this->CheckUser(lpInfo);

	
	//if (lpInfo->RemainTime <= 30)
	//{
	this->NoticeSendToAll(lpInfo, 0, "Diablo will close after %d seconds", lpInfo->RemainTime);
	//}

	if(lpInfo->RemainTime <= 0)
	{
		this->SetState(lpInfo,DIABLO_STATE_EMPTY);
	}
}

void cDiablo::SetState(DIABLO_INFO* lpInfo,int state) // OK
{
	if (gProtection.GetCustomState(CUSTOM_DIABLO) == 0)
	{
		return;
	}

	switch((lpInfo->State=state))
	{
		case DIABLO_STATE_BLANK:
			this->SetState_BLANK(lpInfo);
			break;
		case DIABLO_STATE_EMPTY:
			this->SetState_EMPTY(lpInfo);
			break;
		case DIABLO_STATE_STAND:
			this->SetState_STAND(lpInfo);
			break;
		case DIABLO_STATE_START:
			this->SetState_START(lpInfo);
			break;
		case DIABLO_STATE_CLEAN:
			this->SetState_CLEAN(lpInfo);
			break;
	}
}

void cDiablo::SetState_BLANK(DIABLO_INFO* lpInfo) // OK
{
	//LogAdd(LOG_RED, __FUNCTION__);
}

void cDiablo::SetState_EMPTY(DIABLO_INFO* lpInfo) // OK
{
	//LogAdd(LOG_RED, __FUNCTION__);
	//gDiablo.SendGetGuildInfo();
	lpInfo->EnterEnabled = 0;
	lpInfo->AlarmMinSave = -1;
	lpInfo->AlarmMinLeft = -1;

	this->ClearUser(lpInfo);

	/*if (this->m_DBDataLoading == 0 || this->m_DBDataLoadOK == 0)
	{
		this->SetState(lpInfo,DIABLO_STATE_EMPTY);
		return;
	}*/
	this->CheckSync(lpInfo);
}

void cDiablo::SetState_STAND(DIABLO_INFO* lpInfo) // OK
{
	//LogAdd(LOG_RED, __FUNCTION__);
	//lpInfo->EnterEnabled = 0;
	lpInfo->AlarmMinSave = -1;
	lpInfo->AlarmMinLeft = -1;

	lpInfo->RemainTime = this->m_StandTime*60;

	lpInfo->TargetTime = (int)(time(0)+lpInfo->RemainTime);
}

void cDiablo::SetState_START(DIABLO_INFO* lpInfo) // OK
{
	//LogAdd(LOG_RED, __FUNCTION__);
	//lpInfo->EnterEnabled = 0;
	lpInfo->AlarmMinSave = -1;
	lpInfo->AlarmMinLeft = -1;

	this->CreateDiablo();

	lpInfo->RemainTime = this->m_EventTime*60;

	lpInfo->TargetTime = (int)(time(0)+lpInfo->RemainTime);
}

void cDiablo::SetState_CLEAN(DIABLO_INFO* lpInfo) // OK
{
	//LogAdd(LOG_RED, __FUNCTION__);
	lpInfo->EnterEnabled = 0;
	lpInfo->AlarmMinSave = -1;
	lpInfo->AlarmMinLeft = -1;

	//this->CalcUserRank(lpInfo);

	/*for(int n=0;n < MAX_DIABLO_USERS;n++)
	{
		if(OBJECT_RANGE(lpInfo->m_Users[n].Index) != 0)
		{
			if(lpInfo->m_Users[n].Rank == 1)
			{
				GDRankingCustomArenaSaveSend(lpInfo->User[n].Index,lpInfo->Index,lpInfo->User[n].Score,lpInfo->User[n].Rank);

				gNotice.GCNoticeSendToAll(0,0,0,0,0,0,this->m_CustomArenaText6,lpInfo->RuleInfo.Name,gObj[lpInfo->User[n].Index].Name);

				gNotice.GCNoticeSend(lpInfo->User[n].Index,1,0,0,0,0,0,this->m_CustomArenaText7,lpInfo->RuleInfo.Name,lpInfo->User[n].Rank,lpInfo->User[n].Score);
			}
			else
			{
				GDRankingCustomArenaSaveSend(lpInfo->User[n].Index,lpInfo->Index,lpInfo->User[n].Score,lpInfo->User[n].Rank);

				gNotice.GCNoticeSend(lpInfo->User[n].Index,1,0,0,0,0,0,this->m_CustomArenaText7,lpInfo->RuleInfo.Name,lpInfo->User[n].Rank,lpInfo->User[n].Score);
			}
		}
	}*/

	int TimePerc = lpInfo->EventTime / 100 * 50;
	int WCPrize = 0;
	int WPPrize = 0;

	if (this->m_PrizeType == 0)
	{
		if (this->m_WCoinCPrize <= 0)
		{
			WCPrize = 0;
		}
		else
		{
			WCPrize = this->m_WCoinCPrize / this->GetUserCount(lpInfo);
		}

		if (this->m_WCoinPPrize <= 0)
		{
			WPPrize = 0;
		}
		else
		{
			WPPrize = this->m_WCoinPPrize / this->GetUserCount(lpInfo);
		}
	}
	else
	{
		WCPrize = this->m_WCoinCPrize;
		WPPrize = this->m_WCoinPPrize;
	}

	for (int n=0; n < MAX_DIABLO_USERS; n++)
	{
		if(OBJECT_RANGE(lpInfo->m_Users[n].Index) != 0)
		{
			if (lpInfo->m_Users[n].Time >= TimePerc)
			{
				#if(GAMESERVER_UPDATE>=501)

				gCashShop.GDCashShopAddPointSaveSend(lpInfo->m_Users[n].Index,0,WCPrize,WPPrize,0);

				#else

				gPcPoint.GDPcPointAddPointSaveSend(lpInfo->m_Users[n].Index,Prize);

				gPcPoint.GDPcPointPointSend(lpInfo->m_Users[n].Index);

				#endif

				gNotice.GCNoticeSend(lpInfo->m_Users[n].Index, 1, 0, 0, 0, 0, 0, "You got %d %s and %d %s", WCPrize, gMessage.GetMessage(524), WPPrize, gMessage.GetMessage(525));
			}
		}
	}

	this->GDChangeDate();
	this->m_RegStruct.clear();

	lpInfo->RemainTime = this->m_CloseTime*60;

	lpInfo->TargetTime = (int)(time(0)+lpInfo->RemainTime);
}

void cDiablo::CheckSync(DIABLO_INFO* lpInfo) // OK
{
	if (gProtection.GetCustomState(CUSTOM_DIABLO) == 0)
	{
		return;
	}

	if (this->TimeInfo.StartRegYear == 0 || this->TimeInfo.EndRegYear == 0 || this->TimeInfo.StartYear == 0)
	{
		//LogAdd(LOG_RED, "Return Blank 0");
		this->SetState(lpInfo,DIABLO_STATE_BLANK);
		return;
	}
	//}

	CTime ScheduleTime;

	CScheduleManager ScheduleManager;

	ScheduleManager.AddSchedule(this->TimeInfo.StartYear,this->TimeInfo.StartMonth,this->TimeInfo.StartDay,this->TimeInfo.StartHour,this->TimeInfo.StartMinutes,0,-1);

	if(ScheduleManager.GetSchedule(&ScheduleTime) == 0)
	{
		this->SetState(lpInfo,DIABLO_STATE_BLANK);
		return;
	}

	lpInfo->RemainTime = (int)difftime(ScheduleTime.GetTime(),time(0));

	lpInfo->TargetTime = (int)ScheduleTime.GetTime();
}

void cDiablo::GDUpdateInfo()
{
	if (gProtection.GetCustomState(CUSTOM_DIABLO) == 0)
	{
		return;
	}

	GD_DIABLO_GUILDLIST pMsg;

	pMsg.header.set(0xE2,sizeof(pMsg));

	int Count = 0;

	for (int i = 0; i < MAX_DIABLOGUILD; i++)
	{
		memset(pMsg.GUILD_NAME[i],0,sizeof(pMsg.GUILD_NAME[i]));
		pMsg.RENA_COUNT[i] = 0;
	}

	for(std::vector<REGISTERED_GUILD_INFO>::iterator it=this->m_RegStruct.begin();it != this->m_RegStruct.end();it++)
	{
		memcpy(pMsg.GUILD_NAME[Count], it->GuildName, sizeof(pMsg.GUILD_NAME[Count]));
		pMsg.RENA_COUNT[Count] = it->RegisteredRena;
		Count++;
	}
	pMsg.Count = Count;
	gDataServerConnection.DataSend((BYTE*)&pMsg,sizeof(pMsg));
}

void cDiablo::GDChangeDate()
{
	if (gProtection.GetCustomState(CUSTOM_DIABLO) == 0)
	{
		return;
	}

	GD_DIABLO_CHANGEDATE pMsg;
	pMsg.Head.set(0xFB,0x01,sizeof(pMsg));
	// ----
	struct tm StartRegT = { 0, 0, 12 } ;
	struct tm EndRegT = { 0, 0, 12 } ;
	struct tm StartEventT = { 0, 0, 12 } ;
	// ----
	StartRegT.tm_year = this->TimeInfo.StartRegYear - 1900;
	StartRegT.tm_mon = this->TimeInfo.StartRegMonth - 1;
	StartRegT.tm_mday = this->TimeInfo.StartRegDay;
	// ----
	EndRegT.tm_year = this->TimeInfo.EndRegYear - 1900;
	EndRegT.tm_mon = this->TimeInfo.EndRegMonth - 1;
	EndRegT.tm_mday = this->TimeInfo.EndRegDay;
	// ----
	StartEventT.tm_year = this->TimeInfo.StartYear - 1900;
	StartEventT.tm_mon = this->TimeInfo.StartMonth - 1;
	StartEventT.tm_mday = this->TimeInfo.StartDay;
	// ----
	DatePlusDays( &StartRegT, this->m_ChangeStartReg);
	DatePlusDays( &EndRegT, this->m_ChangeEndReg);
	DatePlusDays( &StartEventT, this->m_ChangeStartEvent);
	//LogAdd(LOG_RED, "%d %d %d", this->m_ChangeStartReg, this->m_ChangeEndReg, this->m_ChangeStartEvent);
	//LogAdd(LOG_RED, "StartReg %d %d %d EndReg %d %d %d StartEvent %d %d %d", this->TimeInfo.StartRegYear, this->TimeInfo.StartRegMonth, this->TimeInfo.StartRegDay,  this->TimeInfo.EndRegYear, this->TimeInfo.EndRegMonth, this->TimeInfo.EndRegDay, this->TimeInfo.StartYear, this->TimeInfo.StartMonth, this->TimeInfo.StartDay);
	//LogAdd(LOG_RED, "StartReg %d %d %d EndReg %d %d %d StartEvent %d %d %d", StartRegT.tm_mday, StartRegT.tm_mon+1, StartRegT.tm_year+1900, EndRegT.tm_mday, EndRegT.tm_mon+1, EndRegT.tm_year+1900, StartEventT.tm_mday, StartEventT.tm_mon+1, StartEventT.tm_year+1900);
	//DatePlusDays( StartRegT, 30 );
	pMsg.DayStartReg = StartRegT.tm_mday;
	pMsg.MonthStartReg = StartRegT.tm_mon+1;
	pMsg.YearStartReg = StartRegT.tm_year+1900;
	// ----
	pMsg.DayEndReg = EndRegT.tm_mday;
	pMsg.MonthEndReg = EndRegT.tm_mon+1;
	pMsg.YearEndReg = EndRegT.tm_year+1900;
	// ----
	pMsg.DayStartEvent = StartEventT.tm_mday;
	pMsg.MonthStartEvent = StartEventT.tm_mon+1;
	pMsg.YearStartEvent = StartEventT.tm_year+1900;
	gDataServerConnection.DataSend((BYTE*)&pMsg,sizeof(pMsg));
}

bool cDiablo::GetUserRespawnLocation(LPOBJ lpObj,int* gate,int* map,int* x,int* y,int* dir,int* level)
{
	if (gProtection.GetCustomState(CUSTOM_DIABLO) == 0)
	{
		return 0;
	}

	for (int i = 0; i < this->GetUserCount(&this->m_DiabloInfo); i++)
	{
		if (lpObj->Index == this->m_DiabloInfo.m_Users[i].Index)
		{
			this->m_DiabloInfo.m_Users[i].OnMap = 0;
		}
	}

	if(gGate.GetGate(17,gate,map,x,y,dir,level) != 0)
	{
		return 1;
	}
	return 0;
}

void cDiablo::CheckUser(DIABLO_INFO* lpInfo) // OK
{
	if (gProtection.GetCustomState(CUSTOM_DIABLO) == 0)
	{
		return;
	}

	//LogAdd(LOG_RED, __FUNCTION__);
	for(int n=0;n < MAX_DIABLO_USERS;n++)
	{
		if(OBJECT_RANGE(lpInfo->m_Users[n].Index) == 0)
		{
			continue;
		}

		if(gObjIsConnected(lpInfo->m_Users[n].Index) == 0)
		{
			this->DelUser(lpInfo,lpInfo->m_Users[n].Index);
			continue;
		}

		if(DE_MAP_RANGE(gObj[lpInfo->m_Users[n].Index].Map) == 0)
		{
			lpInfo->m_Users[n].OnMap = 0;
			continue;
		}
		else
		{
			lpInfo->m_Users[n].OnMap = 1;
		}

		//LogAdd(LOG_RED, "[%s] %d - %d", &gObj[lpInfo->m_Users[n].Index].Name, lpInfo->m_Users[n].HitTick, lpInfo->m_Users[n].DontMoveTick);
		if (lpInfo->m_Users[n].HitTick >= (this->m_PassiveTime*60) && lpInfo->m_Users[n].DontMoveTick >= (this->m_PassiveTime*60))
		{
			gObjMoveGate(lpInfo->m_Users[n].Index,17);
			lpInfo->m_Users[n].HitTick = 0;
			lpInfo->m_Users[n].DontMoveTick = 0;
			gNotice.GCNoticeSend(lpInfo->m_Users[n].Index, 1, 0, 0, 0, 0, 0, "You teleported because of inactivity");
		}

		if (lpInfo->State == DIABLO_STATE_STAND)
		{
			gNotice.GCNoticeSend(lpInfo->m_Users[n].Index, 1, 0, 0, 0, 0, 0, "Diablo will start after %d seconds", lpInfo->RemainTime);
		}

		if (lpInfo->State == DIABLO_STATE_START)
		{
			if (lpInfo->m_Users[n].OnMap == 1)
			{
				lpInfo->m_Users[n].HitTick++;
				lpInfo->m_Users[n].DontMoveTick++;
				lpInfo->m_Users[n].Time++;
				//LogAdd(LOG_RED, "Time = %d Name = %s EventTime = %d", lpInfo->m_Users[n].Time, &gObj[lpInfo->m_Users[n].Index].Name, lpInfo->EventTime);
			}
		}
	}
}

bool cDiablo::AddUser(DIABLO_INFO* lpInfo,int aIndex) // OK
{
	if (gProtection.GetCustomState(CUSTOM_DIABLO) == 0)
	{
		return 0;
	}

	if(OBJECT_RANGE(aIndex) == 0)
	{
		return 0;
	}

	if(this->GetUser(lpInfo,aIndex) != 0)
	{
		return 0;
	}

	for(int n=0;n < MAX_DIABLO_USERS;n++)
	{
		if(OBJECT_RANGE(lpInfo->m_Users[n].Index) != 0)
		{
			continue;
		}
		lpInfo->m_Users[n].Index = aIndex;
		lpInfo->m_Users[n].OnMap = 1;
		lpInfo->m_Users[n].Time = 0;
		lpInfo->m_Users[n].HitTick = 0;
		lpInfo->m_Users[n].DontMoveTick = 0;
		return 1;
	}

	return 0;
}

bool cDiablo::DelUser(DIABLO_INFO* lpInfo,int aIndex) // OK
{
	if (gProtection.GetCustomState(CUSTOM_DIABLO) == 0)
	{
		return 0;
	}

	if(OBJECT_RANGE(aIndex) == 0)
	{
		return 0;
	}

	DIABLO_USER* lpUser = this->GetUser(lpInfo,aIndex);

	if(lpUser == 0)
	{
		return 0;
	}

	lpUser->Reset();
	return 1;
}

DIABLO_USER* cDiablo::GetUser(DIABLO_INFO* lpInfo,int aIndex) // OK
{
	if (gProtection.GetCustomState(CUSTOM_DIABLO) == 0)
	{
		return 0;
	}

	if(OBJECT_RANGE(aIndex) == 0)
	{
		return 0;
	}

	for(int n=0;n < MAX_DIABLO_USERS;n++)
	{
		if(lpInfo->m_Users[n].Index == aIndex)
		{
			return &lpInfo->m_Users[n];
		}
	}

	return 0;
}

int cDiablo::GetUserCount(DIABLO_INFO* lpInfo) // OK
{
	if (gProtection.GetCustomState(CUSTOM_DIABLO) == 0)
	{
		return 0;
	}

	int count = 0;

	for(int n=0;n < MAX_DIABLO_USERS;n++)
	{
		if(OBJECT_RANGE(lpInfo->m_Users[n].Index) != 0)
		{
			count++;
		}
	}

	return count;
}

void cDiablo::NoticeSendToAll(DIABLO_INFO* lpInfo,int type,char* message,...)
{
	if (gProtection.GetCustomState(CUSTOM_DIABLO) == 0)
	{
		return;
	}

	char buff[256];

	va_list arg;
	va_start(arg,message);
	vsprintf_s(buff,message,arg);
	va_end(arg);

	for(int n=0;n < MAX_DIABLO_USERS;n++)
	{
		if(OBJECT_RANGE(lpInfo->m_Users[n].Index) != 0)
		{
			gNotice.GCNoticeSend(lpInfo->m_Users[n].Index,type,0,0,0,0,0,buff);
		}
	}
}

void cDiablo::ClearUser(DIABLO_INFO* lpInfo) // OK
{
	if (gProtection.GetCustomState(CUSTOM_DIABLO) == 0)
	{
		return;
	}

	for(int n=0;n < MAX_DIABLO_USERS;n++)
	{
		if(OBJECT_RANGE(lpInfo->m_Users[n].Index) == 0)
		{
			continue;
		}

		gObjMoveGate(lpInfo->m_Users[n].Index,17);

		lpInfo->m_Users[n].Reset();
	}
}

bool cDiablo::CreateDiablo()
{
	if (gProtection.GetCustomState(CUSTOM_DIABLO) == 0)
	{
		return 0;
	}

	for(int n=0;n < gMonsterSetBase.m_count;n++)
	{
		MONSTER_SET_BASE_INFO* lpInfo = &gMonsterSetBase.m_MonsterSetBaseInfo[n];

		if(lpInfo->Type != 4 || lpInfo->MonsterClass != 12 || lpInfo->Map != MAP_DIABLO_ARENA)
		{
			continue;
		}

		int index = gObjAddMonster(lpInfo->Map);

		if(OBJECT_RANGE(index) == 0)
		{
			continue;
		}

		if(gObjSetPosMonster(index,n) == 0)
		{
			gObjDel(index);
			continue;
		}

		if(gObjSetMonster(index,lpInfo->MonsterClass) == 0)
		{
			gObjDel(index);
			continue;
		}

		this->m_DiabloObjIndex = index;
		//this->SetSelupanObjIndex(index);

		return 1;
	}

	return 0;
}

void cDiablo::DeleteDiablo()
{
	if (gProtection.GetCustomState(CUSTOM_DIABLO) == 0)
	{
		return;
	}

	if(OBJECT_RANGE(this->m_DiabloObjIndex) != 0)
	{
		gObjDel(this->m_DiabloObjIndex);
		this->m_DiabloObjIndex = -1;
	}
}

void cDiablo::DiabloMonsterDieProc(LPOBJ lpObj, LPOBJ lpTarget)
{
	if (gProtection.GetCustomState(CUSTOM_DIABLO) == 0)
	{
		return;
	}

	int aIndex = gObjMonsterGetTopHitDamageUser(lpObj);

	if(OBJECT_RANGE(aIndex) != 0)
	{
		lpTarget = &gObj[aIndex];
	}

	if(lpObj->Class == 12)
	{
		gItemBagManager.DropItemByMonsterClass(lpObj->Class, lpTarget, lpObj->Map, lpObj->X, lpObj->Y);
		//gItemBagManager.DropItemBySpecialValue(ITEM_BAG_DIABLO,lpTarget,lpTarget->Map,lpTarget->X,lpTarget->Y);
		this->DiabloDie(lpObj->Index);

		gNotice.GCNoticeSendToAll(0,0,0,0,0,0,"Guild %s defeated Diablo",lpTarget->GuildName);
	}
}

void cDiablo::GetHit(LPOBJ lpObj, LPOBJ lpTarget)
{
	if (gProtection.GetCustomState(CUSTOM_DIABLO) == 0)
	{
		return;
	}

	//this->m_DiabloInfo.m_Users[i].OnMap = 0;
	for(int n=0;n < MAX_DIABLO_USERS;n++)
	{
		if(OBJECT_RANGE(this->m_DiabloInfo.m_Users[n].Index) == 0)
		{
			continue;
		}
		
		if (this->m_DiabloInfo.m_Users[n].Index == lpObj->Index)
		{
			this->m_DiabloInfo.m_Users[n].HitTick = 0;
		}
	}
}

void cDiablo::GetMove(LPOBJ lpObj)
{
	if (gProtection.GetCustomState(CUSTOM_DIABLO) == 0)
	{
		return;
	}

	for(int n=0;n < MAX_DIABLO_USERS;n++)
	{
		if(OBJECT_RANGE(this->m_DiabloInfo.m_Users[n].Index) == 0)
		{
			continue;
		}
		
		if (this->m_DiabloInfo.m_Users[n].Index == lpObj->Index)
		{
			this->m_DiabloInfo.m_Users[n].DontMoveTick = 0;
		}
	}
}

void cDiablo::DiabloDie(int index)
{
	if (gProtection.GetCustomState(CUSTOM_DIABLO) == 0)
	{
		return;
	}

	if(OBJECT_RANGE(this->m_DiabloObjIndex) == 0)
	{
		return;
	}

	if(this->m_DiabloObjIndex != index)
	{
		return;
	}

	gObjDel(this->m_DiabloObjIndex);
	this->m_DiabloObjIndex = -1;
}