#include "stdafx.h"
#if (USE_TVTEVENT == TRUE)
#include "TvTEvent.h"
#include "Guild.h"
#include "EffectManager.h"
#include "Notice.h"
#include "ObjectManager.h"
#include "ScheduleManager.h"
#include "MemScript.h"
#include "Util.h"

CTvTEvent gTvTEvent;

CTvTEvent::CTvTEvent()
{
	for(int n=0;n < MAX_TVTEVENT;n++)
	{
		TVT_EVENT_INFO* lpInfo = &this->m_TvTEventInfo[n];

		lpInfo->Index = n;
		lpInfo->State = TVT_EVENT_STATE_BLANK;
		lpInfo->RemainTime = 0;
		lpInfo->TargetTime = 0;
		lpInfo->TickCount = GetTickCount();
		lpInfo->EnterEnabled = 0;
		lpInfo->AlarmMinSave = -1;
		lpInfo->AlarmMinLeft = -1;
		lpInfo->ScoreBlue = 0;
		lpInfo->ScoreRed = 0;
		lpInfo->TotalPlayer = 0;

		this->CleanUser(lpInfo);
	}
}

CTvTEvent::~CTvTEvent()
{
	
}

void CTvTEvent::Init()
{
	for(int n=0;n < MAX_TVTEVENT;n++)
	{
		if(this->m_TvTEventSwitch == 0)
		{
			this->SetState(&this->m_TvTEventInfo[n],TVT_EVENT_STATE_BLANK);
		}
		else
		{
			this->SetState(&this->m_TvTEventInfo[n],TVT_EVENT_STATE_EMPTY);
		}
	}
}

void CTvTEvent::ReadTvTEventInfo(char* section,char* path)
{
	this->m_TvTEventSwitch = GetPrivateProfileInt(section,"TvTEventSwitch",0,path);

	this->m_TvTEventNPCId = GetPrivateProfileInt(section,"TvTEventNPC",0,path);

	this->m_TvTEventNPCMap = GetPrivateProfileInt(section,"TvTEventNPCMap",0,path);

	this->m_TvTEventNPCX = GetPrivateProfileInt(section,"TvTEventNPCX",0,path);

	this->m_TvTEventNPCY = GetPrivateProfileInt(section,"TvTEventNPCY",0,path);

	GetPrivateProfileString(section,"TvTEventText1","",this->m_TvTEventText1,sizeof(this->m_TvTEventText1),path);

	GetPrivateProfileString(section,"TvTEventText2","",this->m_TvTEventText2,sizeof(this->m_TvTEventText2),path);

	GetPrivateProfileString(section,"TvTEventText3","",this->m_TvTEventText3,sizeof(this->m_TvTEventText3),path);

	GetPrivateProfileString(section,"TvTEventText4","",this->m_TvTEventText4,sizeof(this->m_TvTEventText4),path);

	GetPrivateProfileString(section,"TvTEventText5","",this->m_TvTEventText5,sizeof(this->m_TvTEventText5),path);

	GetPrivateProfileString(section,"TvTEventText6","",this->m_TvTEventText6,sizeof(this->m_TvTEventText6),path);

	GetPrivateProfileString(section,"TvTEventText7","",this->m_TvTEventText7,sizeof(this->m_TvTEventText7),path);
}

void CTvTEvent::Load(char* path)
{
	CMemScript* lpMemScript = new CMemScript;

	if(lpMemScript == 0)
	{
		ErrorMessageBox(MEM_SCRIPT_ALLOC_ERROR,path);
		return;
	}

	if(lpMemScript->SetBuffer(path) == 0)
	{
		ErrorMessageBox(lpMemScript->GetLastError());
		delete lpMemScript;
		return;
	}

	for(int n=0;n < MAX_TVTEVENT;n++)
	{
		this->m_TvTEventInfo[n].StartTime.clear();
	}

	try
	{
		while(true)
		{
			if(lpMemScript->GetToken() == TOKEN_END)
			{
				break;
			}
		
			int section = lpMemScript->GetNumber();

			while(true)
			{
				if(section == 0)
				{
					if(strcmp("end",lpMemScript->GetAsString()) == 0)
					{
						break;
					}

					TVT_EVENT_START_TIME info;

					int index = lpMemScript->GetNumber();

					info.Year = lpMemScript->GetAsNumber();

					info.Month = lpMemScript->GetAsNumber();

					info.Day = lpMemScript->GetAsNumber();

					info.DayOfWeek = lpMemScript->GetAsNumber();

					info.Hour = lpMemScript->GetAsNumber();

					info.Minute = lpMemScript->GetAsNumber();

					info.Second = lpMemScript->GetAsNumber();

					this->m_TvTEventInfo[index].StartTime.push_back(info);
				}
				else if(section == 1)
				{
					if(strcmp("end",lpMemScript->GetAsString()) == 0)
					{
						break;
					}

					int index = lpMemScript->GetNumber();

					strcpy_s(this->m_TvTEventInfo[index].RuleInfo.Name,lpMemScript->GetAsString());

					this->m_TvTEventInfo[index].RuleInfo.AlarmTime = lpMemScript->GetAsNumber();

					this->m_TvTEventInfo[index].RuleInfo.StandTime = lpMemScript->GetAsNumber();

					this->m_TvTEventInfo[index].RuleInfo.EventTime = lpMemScript->GetAsNumber();

					this->m_TvTEventInfo[index].RuleInfo.CloseTime = lpMemScript->GetAsNumber();

					this->m_TvTEventInfo[index].RuleInfo.WaitingMap = lpMemScript->GetAsNumber();

					this->m_TvTEventInfo[index].RuleInfo.WaitingX = lpMemScript->GetAsNumber();

					this->m_TvTEventInfo[index].RuleInfo.WaitingY = lpMemScript->GetAsNumber();

					this->m_TvTEventInfo[index].RuleInfo.BluTeamX = lpMemScript->GetAsNumber();

					this->m_TvTEventInfo[index].RuleInfo.BluTeamY = lpMemScript->GetAsNumber();

					this->m_TvTEventInfo[index].RuleInfo.RedTeamX = lpMemScript->GetAsNumber();

					this->m_TvTEventInfo[index].RuleInfo.RedTeamY = lpMemScript->GetAsNumber();

					this->m_TvTEventInfo[index].RuleInfo.RewardItem = lpMemScript->GetAsNumber();

					this->m_TvTEventInfo[index].RuleInfo.ItemRewardIndex = lpMemScript->GetAsNumber();

					this->m_TvTEventInfo[index].RuleInfo.ItemRewardLevel = lpMemScript->GetAsNumber();

				}
				else
				{
					break;
				}
			}
		}
	}
	catch(...)
	{
		ErrorMessageBox(lpMemScript->GetLastError());
	}

	delete lpMemScript;
}

void CTvTEvent::MainProc()
{
	for(int n=0;n < MAX_TVTEVENT;n++)
	{
		TVT_EVENT_INFO* lpInfo = &this->m_TvTEventInfo[n];

		if((GetTickCount()-lpInfo->TickCount) >= 1000)
		{
			lpInfo->TickCount = GetTickCount();

			lpInfo->RemainTime = (int)difftime(lpInfo->TargetTime,time(0));

			switch(lpInfo->State)
			{
				case TVT_EVENT_STATE_BLANK:
					this->ProcState_BLANK(lpInfo);
					break;
				case TVT_EVENT_STATE_EMPTY:
					this->ProcState_EMPTY(lpInfo);
					break;
				case TVT_EVENT_STATE_STAND:
					this->ProcState_STAND(lpInfo);
					break;
				case TVT_EVENT_STATE_START:
					this->ProcState_START(lpInfo);
					break;
				case TVT_EVENT_STATE_CLEAN:
					this->ProcState_CLEAN(lpInfo);
					break;
			}
		}
	}
}

void CTvTEvent::ProcState_BLANK(TVT_EVENT_INFO* lpInfo) // OK
{

}

void CTvTEvent::ProcState_EMPTY(TVT_EVENT_INFO* lpInfo) // OK
{
	if(lpInfo->RemainTime > 0 && lpInfo->RemainTime <= (lpInfo->RuleInfo.AlarmTime*60))
	{
		this->CheckUser(lpInfo);

		lpInfo->EnterEnabled = 1;

		if((lpInfo->AlarmMinSave=(((lpInfo->RemainTime%60)==0)?((lpInfo->RemainTime/60)-1):(lpInfo->RemainTime/60))) != lpInfo->AlarmMinLeft)
		{
			lpInfo->AlarmMinLeft = lpInfo->AlarmMinSave;

			gNotice.GCNoticeSendToAll(0,0,0,0,0,0,this->m_TvTEventText1,lpInfo->RuleInfo.Name,(lpInfo->AlarmMinLeft+1));
		}
	}

	if(lpInfo->RemainTime <= 0)
	{
		gNotice.GCNoticeSendToAll(0,0,0,0,0,0,this->m_TvTEventText2,lpInfo->RuleInfo.Name);
		this->NoticeSendToAll(lpInfo,1,this->m_TvTEventText3,lpInfo->RuleInfo.Name,lpInfo->RuleInfo.StandTime);
		this->SetState(lpInfo,TVT_EVENT_STATE_STAND);
	}
}

void CTvTEvent::ProcState_STAND(TVT_EVENT_INFO* lpInfo) // OK
{
	this->CheckUser(lpInfo);

	if(this->GetUserCount(lpInfo) < 2)
	{
		this->SetState(lpInfo,TVT_EVENT_STATE_EMPTY);
		return;
	}

	if(lpInfo->RemainTime <= 0)
	{
		if (lpInfo->TotalPlayer >=2)
		{
			this->DivisionTeam(lpInfo);
			this->NoticeSendToAll(lpInfo,0,this->m_TvTEventText4,lpInfo->RuleInfo.Name);
			this->SetState(lpInfo,TVT_EVENT_STATE_START);
		}
		else
		{
			this->NoticeSendToAll(lpInfo,0,this->m_TvTEventText2,lpInfo->RuleInfo.Name);
			this->SetState(lpInfo,TVT_EVENT_STATE_EMPTY);
		}
	}
}

void CTvTEvent::ProcState_START(TVT_EVENT_INFO* lpInfo) // OK
{
	this->CheckUser(lpInfo);

	if(this->GetUserCount(lpInfo) == 0)
	{
		this->SetState(lpInfo,TVT_EVENT_STATE_EMPTY);
		return;
	}

	if(this->GetUserCount(lpInfo) == 1)
	{
		this->SetState(lpInfo,TVT_EVENT_STATE_CLEAN);
		return;
	}

	if(lpInfo->RemainTime <= 0)
	{
		this->NoticeSendToAll(lpInfo,0,this->m_TvTEventText5,lpInfo->RuleInfo.Name);
		this->SetState(lpInfo,TVT_EVENT_STATE_CLEAN);
	}
}

void CTvTEvent::ProcState_CLEAN(TVT_EVENT_INFO* lpInfo) // OK
{
	this->CheckUser(lpInfo);

	if(lpInfo->RemainTime <= 0)
	{
		this->SetState(lpInfo,TVT_EVENT_STATE_EMPTY);
	}
}

void CTvTEvent::SetState(TVT_EVENT_INFO* lpInfo,int state) // OK
{
	switch((lpInfo->State=state))
	{
		case TVT_EVENT_STATE_BLANK:
			this->SetState_BLANK(lpInfo);
			break;
		case TVT_EVENT_STATE_EMPTY:
			this->SetState_EMPTY(lpInfo);
			break;
		case TVT_EVENT_STATE_STAND:
			this->SetState_STAND(lpInfo);
			break;
		case TVT_EVENT_STATE_START:
			this->SetState_START(lpInfo);
			break;
		case TVT_EVENT_STATE_CLEAN:
			this->SetState_CLEAN(lpInfo);
			break;
	}
}

void CTvTEvent::SetState_BLANK(TVT_EVENT_INFO* lpInfo) // OK
{

}

void CTvTEvent::SetState_EMPTY(TVT_EVENT_INFO* lpInfo) // OK
{
	lpInfo->EnterEnabled = 0;
	lpInfo->AlarmMinSave = -1;
	lpInfo->AlarmMinLeft = -1;

	this->ClearUser(lpInfo);

	this->CheckSync(lpInfo);
}

void CTvTEvent::SetState_STAND(TVT_EVENT_INFO* lpInfo) // OK
{
	lpInfo->EnterEnabled = 0;
	lpInfo->AlarmMinSave = -1;
	lpInfo->AlarmMinLeft = -1;

	lpInfo->RemainTime = lpInfo->RuleInfo.StandTime*60;

	lpInfo->TargetTime = (int)(time(0)+lpInfo->RemainTime);
}

void CTvTEvent::SetState_START(TVT_EVENT_INFO* lpInfo) // OK
{
	lpInfo->EnterEnabled = 0;
	lpInfo->AlarmMinSave = -1;
	lpInfo->AlarmMinLeft = -1;

	lpInfo->RemainTime = lpInfo->RuleInfo.EventTime*60;

	lpInfo->TargetTime = (int)(time(0)+lpInfo->RemainTime);
}

void CTvTEvent::SetState_CLEAN(TVT_EVENT_INFO* lpInfo) // OK
{
	lpInfo->EnterEnabled = 0;
	lpInfo->AlarmMinSave = -1;
	lpInfo->AlarmMinLeft = -1;



	lpInfo->RemainTime = lpInfo->RuleInfo.CloseTime*60;

	lpInfo->TargetTime = (int)(time(0)+lpInfo->RemainTime);
}

void CTvTEvent::CheckSync(TVT_EVENT_INFO* lpInfo) // OK
{
	if(lpInfo->StartTime.empty() != 0)
	{
		this->SetState(lpInfo,TVT_EVENT_STATE_BLANK);
		return;
	}

	CTime ScheduleTime;

	CScheduleManager ScheduleManager;

	for(std::vector<TVT_EVENT_START_TIME>::iterator it=lpInfo->StartTime.begin();it != lpInfo->StartTime.end();it++)
	{
		ScheduleManager.AddSchedule(it->Year,it->Month,it->Day,it->Hour,it->Minute,it->Second,it->DayOfWeek);
	}

	if(ScheduleManager.GetSchedule(&ScheduleTime) == 0)
	{
		this->SetState(lpInfo,TVT_EVENT_STATE_BLANK);
		return;
	}

	lpInfo->RemainTime = (int)difftime(ScheduleTime.GetTime(),time(0));

	lpInfo->TargetTime = (int)ScheduleTime.GetTime();
}

int CTvTEvent::GetState(int index) // OK
{
	if(index < 0 || index >= MAX_TVTEVENT)
	{
		return TVT_EVENT_STATE_BLANK;
	}

	return this->m_TvTEventInfo[index].State;
}

bool CTvTEvent::Dialog(LPOBJ lpObj, LPOBJ lpNpc)
{
	LogAdd(LOG_TEST, "Enabled = %d", this->m_TvTEventSwitch);
	if (!this->m_TvTEventSwitch) return false;

	if (lpNpc->Class == this->m_TvTEventNPCId &&
		lpNpc->Map == this->m_TvTEventNPCMap &&
		lpNpc->X == this->m_TvTEventNPCX &&
		lpNpc->Y == this->m_TvTEventNPCY)
	{
		LogAdd(LOG_TEST, "NPC Done");
		return this->CheckEnterEnabled(lpObj);
	}

	return false;
}

bool CTvTEvent::CheckEnterEnabled(LPOBJ lpObj)
{
	if(OBJECT_RANGE(lpObj->PartyNumber) != 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,this->m_TvTEventText6);
		return 0;
	}

	if(lpObj->Guild != 0 && lpObj->Guild->WarState == 1)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,this->m_TvTEventText6);
		return 0;
	}

	if (lpObj->Guild != 0 && lpObj->Guild)

	if(OBJECT_RANGE(lpObj->DuelUser) != 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,this->m_TvTEventText6);
		return 0;
	}

	if(lpObj->PShopOpen != 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,this->m_TvTEventText6);
		return 0;
	}

	for(int n=0;n < MAX_TVTEVENT;n++)
	{
		TVT_EVENT_INFO* lpInfo = &this->m_TvTEventInfo[n];

		if(this->GetEnterEnabled(lpInfo->Index) == 0)
		{
			gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,this->m_TvTEventText7);
			return 0;
		}

		gEffectManager.ClearAllEffect(lpObj);

		return this->AddUser(lpInfo,lpObj->Index);
	}
}

int CTvTEvent::GetEnterEnabled(int index) // OK
{
	if(index < 0 || index >= MAX_TVTEVENT)
	{
		return TVT_EVENT_STATE_BLANK;
	}

	return this->m_TvTEventInfo[index].EnterEnabled;
}

int CTvTEvent::GetEnteredUserCount(int index) // OK
{
	if(index < 0 || index >= MAX_TVTEVENT)
	{
		return TVT_EVENT_STATE_BLANK;
	}

	return 0;//this->GetUserCount(&this->m_TvTEventInfo[index]);
}

bool CTvTEvent::CheckEnteredUser(int index,int aIndex) // OK
{
	if(index < 0 || index >= MAX_TVTEVENT)
	{
		return TVT_EVENT_STATE_BLANK;
	}

	return 0;//((this->GetUser(&this->m_TvTEventInfo[index],aIndex)==0)?0:1);
}

bool CTvTEvent::AddUser(TVT_EVENT_INFO* lpInfo,int aIndex) // OK
{
	LPOBJ lpObj = &gObj[aIndex];
	if(OBJECT_RANGE(aIndex) == 0)
	{
		return 0;
	}

	if(this->GetUser(lpInfo,aIndex) != 0)
	{
		return 0;
	}

	for(int n=0;n < MAX_TVTEVENT_USER;n++)
	{
		if(OBJECT_RANGE(lpInfo->User[n].Index) != 0)
		{
			continue;
		}

		lpInfo->User[n].Index = aIndex;
		lpInfo->User[n].Available = 1;
		lpInfo->TotalPlayer++;
		gObjTeleport(lpObj->Index,lpInfo->RuleInfo.WaitingMap,lpInfo->RuleInfo.WaitingX,lpInfo->RuleInfo.WaitingY);
		return 1;
	}

	return 0;
}

bool CTvTEvent::DelUser(TVT_EVENT_INFO* lpInfo,int aIndex) // OK
{
	if(OBJECT_RANGE(aIndex) == 0)
	{
		return 0;
	}

	TVT_EVENT_USER* lpUser = this->GetUser(lpInfo,aIndex);

	if(lpUser == 0)
	{
		return 0;
	}

	lpUser->Reset();
	lpInfo->TotalPlayer--;
	return 1;
}

TVT_EVENT_USER* CTvTEvent::GetUser(TVT_EVENT_INFO* lpInfo,int aIndex) // OK
{
	if(OBJECT_RANGE(aIndex) == 0)
	{
		return 0;
	}

	for(int n=0;n < MAX_TVTEVENT_USER;n++)
	{
		if(lpInfo->User[n].Index == aIndex)
		{
			return &lpInfo->User[n];
		}
	}

	return 0;
}

void CTvTEvent::CleanUser(TVT_EVENT_INFO* lpInfo) // OK
{
	for(int n=0;n < MAX_TVTEVENT_USER;n++)
	{
		lpInfo->User[n].Reset();
		lpInfo->TotalPlayer = 0;
	}
}

void CTvTEvent::ClearUser(TVT_EVENT_INFO* lpInfo) // OK
{
	for(int n=0;n < MAX_TVTEVENT_USER;n++)
	{
		if(OBJECT_RANGE(lpInfo->User[n].Index) == 0)
		{
			continue;
		}

		gObjMoveGate(lpInfo->User[n].Index,17);

		gObj[lpInfo->User[n].Index].Change = 404;
		gObjViewportListProtocolCreate(&gObj[lpInfo->User[n].Index]);
		gObjectManager.CharacterUpdateMapEffect(&gObj[lpInfo->User[n].Index]);

		lpInfo->User[n].Reset();

		lpInfo->TotalPlayer--;
	}
}

void CTvTEvent::CheckUser(TVT_EVENT_INFO* lpInfo) // OK
{
	for(int n=0;n < MAX_TVTEVENT_USER;n++)
	{
		if(OBJECT_RANGE(lpInfo->User[n].Index) == 0)
		{
			continue;
		}

		if(gObjIsConnected(lpInfo->User[n].Index) == 0)
		{
			this->DelUser(lpInfo,lpInfo->User[n].Index);
			continue;
		}
	}
}

int CTvTEvent::GetUserCount(TVT_EVENT_INFO* lpInfo) // OK
{
	int count = 0;

	for(int n=0;n < MAX_TVTEVENT_USER;n++)
	{
		if(OBJECT_RANGE(lpInfo->User[n].Index) != 0)
		{
			count++;
		}
	}

	return count;
}

void CTvTEvent::DivisionTeam(TVT_EVENT_INFO* lpInfo)
{
	int iTeam = 1;
	int Index = 0;
	//char szText[150];

	for(int i=0;i<lpInfo->TotalPlayer;i++)
	{
		Index = this->ReturnMaxReset(lpInfo);
		lpInfo->User[i].Team = iTeam;
		if(iTeam == 1) iTeam = 2;
		else iTeam = 1;
	}

	for(int n=0;n<lpInfo->TotalPlayer;n++)
	{
		if (lpInfo->User[n].Team == 1)
		{
			gObjTeleport(lpInfo->User[n].Index,lpInfo->RuleInfo.WaitingMap,lpInfo->RuleInfo.BluTeamX,lpInfo->RuleInfo.BluTeamY);

			gObj[lpInfo->User[n].Index].Change = 405;
			gObjViewportListProtocolCreate(&gObj[lpInfo->User[n].Index]);
			gObjectManager.CharacterUpdateMapEffect(&gObj[lpInfo->User[n].Index]);

			gNotice.GCNoticeSend(lpInfo->User[n].Index,0,0,0,0,0,0,"[BlueTeam]: %d Vs [RedTeam]: %d",lpInfo->ScoreBlue,lpInfo->ScoreRed);
		}
		else if (lpInfo->User[n].Team == 2)
		{
			gObjTeleport(lpInfo->User[n].Index,lpInfo->RuleInfo.WaitingMap,lpInfo->RuleInfo.RedTeamX,lpInfo->RuleInfo.RedTeamY);

			gObj[lpInfo->User[n].Index].Change = 404;
			gObjViewportListProtocolCreate(&gObj[lpInfo->User[n].Index]);
			gObjectManager.CharacterUpdateMapEffect(&gObj[lpInfo->User[n].Index]);

			gNotice.GCNoticeSend(lpInfo->User[n].Index,0,0,0,0,0,0,"[RedTeam]: %d Vs [BlueTeam]: %d",lpInfo->ScoreRed,lpInfo->ScoreBlue);
		}
	}
}

int CTvTEvent::ReturnMaxReset(TVT_EVENT_INFO* lpInfo)
{
	int MaxRes = 0;
	int Index = 0;
	int UserIndex = 0;

	for(int n=0;n < lpInfo->TotalPlayer;n++)
	{
		LPOBJ lpObj = &gObj[lpInfo->User[n].Index];
		if(OBJECT_RANGE(lpObj->Index) == 0)
		{
			continue;
		}

		if (!lpInfo->User->Available)
		{
			continue;
		}

		if(lpObj->Reset >= MaxRes)
		{
			MaxRes = lpObj->Reset;
			Index = lpObj->Index;
			UserIndex = n;
		}
	}
	lpInfo->User[UserIndex].Available = 0;
	return Index;
}

void CTvTEvent::NoticeSendToAll(TVT_EVENT_INFO* lpInfo,int type,char* message,...) // OK
{
	char buff[256];

	va_list arg;
	va_start(arg,message);
	vsprintf_s(buff,message,arg);
	va_end(arg);

	for(int n=0;n < MAX_TVTEVENT_USER;n++)
	{
		if(OBJECT_RANGE(lpInfo->User[n].Index) != 0)
		{
			gNotice.GCNoticeSend(lpInfo->User[n].Index,type,0,0,0,0,0,buff);
		}
	}
}

#endif