#include "stdafx.h"
#include "Rankings.h"
#include "GameMain.h"
#include "Util.h"
#include "ItemManager.h"
#include "Protection.h"

CRankings gRankings;

CRankings::CRankings()
{
	this->lastUpdateTick = 0;
	this->updateTime = 300000;
}

void CRankings::GDSendRequest(int aIndex)
{
	if (!gProtection.GetCustomState(CUSTOM_RANKINGS))
	{
		return;
	}
	
	if (GetTickCount() - this->lastUpdateTick  < updateTime)
	{
		this->GCSendRankings(aIndex);
		return;
	}
	
	PMSG_DB_REQ_SEND pMsg;
	pMsg.header.set(0xFB,0x0D,sizeof(pMsg));
	
	pMsg.aIndex = aIndex;

	gDataServerConnection.DataSend((BYTE*)&pMsg,pMsg.header.size);
}

void CRankings::GDRecvRankings(SDHP_RANKINGS_INFO_SEND* lpMsg)
{
	if (!gObjIsConnectedGP(lpMsg->aIndex))
	{
		return;
	}

	this->m_rankings_data.clear();

	for(int n=0;n < lpMsg->count;n++)
	{
		SDHP_RANKINGS_INFO* lpInfo = (SDHP_RANKINGS_INFO*)(((BYTE*)lpMsg)+sizeof(SDHP_RANKINGS_INFO_SEND)+(sizeof(SDHP_RANKINGS_INFO)*n));

		this->m_rankings_data.emplace_back(*lpInfo);
	}

	this->lastUpdateTick = GetTickCount();

	this->GCSendRankings(lpMsg->aIndex);
}

void CRankings::GCSendRankings(int aIndex)
{
	if (!gObjIsConnectedGP(aIndex))
	{
		return;
	}

	BYTE send[8192];
	
	GC_RANKINGS_INFO_SEND pMsg;

	pMsg.header.set(0xFB,0x95,0);

	int size = sizeof(pMsg);

	pMsg.updateTick = GetTickCount() - this->lastUpdateTick;

	pMsg.count = 0;

	SDHP_RANKINGS_INFO info;
	
	for(auto it=this->m_rankings_data.begin(); it != this->m_rankings_data.end(); ++it)
	{
		memcpy(info.name,it->name,sizeof(info.name));
		info.level = it->level;
		info.classId = it->classId;
		info.map = it->map;
		info.reset = it->reset;
		info.masterReset = it->masterReset;
		info.gensFamily = it->gensFamily;
		info.masterLevel = it->masterLevel;
		info.connectStat = it->connectStat;
		info.accountLevel = it->accountLevel;

		if (info.connectStat == 0)
		{
			if (gObjFind(info.name) != 0)
			{
				info.connectStat = 1;
			}
		}

		memcpy(&send[size],&info,sizeof(info));
		size += sizeof(info);

		pMsg.count++;
	}

	pMsg.header.size[0] = SET_NUMBERHB(size);
	pMsg.header.size[1] = SET_NUMBERLB(size);

	memcpy(send,&pMsg,sizeof(pMsg));

	DataSend(aIndex,send,size);
}

void CRankings::CGRecvCharacterInfoRequest(CG_RANKINGS_INFO_REQUEST_SEND* lpMsg, int aIndex)
{
	if (!gProtection.GetCustomState(CUSTOM_RANKINGS))
	{
		return;
	}
	
	LPOBJ lpObj = gObjFind(lpMsg->name);

	if (lpObj == 0)
	{
		this->GDSendCharacterInfoRequest(lpMsg->name,aIndex);
		return;
	}

	LogAdd(LOG_TEST,"[Rankings] Requested character info from GS");

	GC_RANKINGS_INFO_REQUEST_SEND pMsg;
	pMsg.header.set(0xFB,0x1D,sizeof(pMsg));
	pMsg.result = 1;

	pMsg.Strength = lpObj->Strength;
	pMsg.Dexterity = lpObj->Dexterity;
	pMsg.Vitality = lpObj->Vitality;
	pMsg.Energy = lpObj->Energy;
	pMsg.Leadership = lpObj->Leadership;
	pMsg.PkLevel = lpObj->PKLevel;
	pMsg.PkCount = lpObj->PKCount;
	pMsg.MapPosX = lpObj->X;
	pMsg.MapPosY = lpObj->Y;
	pMsg.QuestIndex = lpObj->iQuestIndex;
	pMsg.QuestStat = lpObj->iQuestStat;
	memcpy(pMsg.GuildName,lpObj->GuildName,sizeof(pMsg.GuildName));

	for(int i=0;i<INVENTORY_WEAR_SIZE;i++)
	{
		if (!lpObj->Inventory[i].IsItem())
		{
			pMsg.isItem[i] = 0;
			continue;
		}
		
		gItemManager.ItemByteConvert(pMsg.ItemInfo[i],lpObj->Inventory[i]);
		pMsg.isItem[i] = 1;
	}


	DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);
}


void CRankings::GDSendCharacterInfoRequest(char* name, int aIndex)
{
	GD_RANKINGS_INFO_REQUEST_SEND pMsg;

	pMsg.header.set(0xFB,0x0E,sizeof(pMsg));

	memcpy(pMsg.name,name,sizeof(pMsg.name));
	
	pMsg.aIndex = aIndex;

	gDataServerConnection.DataSend((BYTE*)&pMsg,pMsg.header.size);
}

void CRankings::DGRecvCharacterInfo(DG_RANKINGS_INFO_REQUEST_RECV* lpMsg)
{
	LogAdd(LOG_TEST,"[Rankings] Requested character info from DB");
	GC_RANKINGS_INFO_REQUEST_SEND pMsg;

	pMsg.header.set(0xFB,0x1D,sizeof(pMsg));

	pMsg.result = lpMsg->result;
	pMsg.Strength = lpMsg->Strength;
	pMsg.Dexterity = lpMsg->Dexterity;
	pMsg.Vitality = lpMsg->Vitality;
	pMsg.Energy = lpMsg->Energy;
	pMsg.Leadership = lpMsg->Leadership;
	pMsg.PkLevel = lpMsg->PkLevel;
	pMsg.PkCount = lpMsg->PkCount;
	pMsg.MapPosX = lpMsg->MapPosX;
	pMsg.MapPosY = lpMsg->MapPosY;
	pMsg.QuestIndex = lpMsg->QuestIndex;
	pMsg.QuestStat = lpMsg->QuestStat;
	memcpy(pMsg.GuildName,lpMsg->GuildName,sizeof(pMsg.GuildName));

	CItem item[INVENTORY_WEAR_SIZE];

	for(int i=0;i<INVENTORY_WEAR_SIZE;i++)
	{
		item[i].Clear();
		if(gItemManager.ConvertItemByte(&item[i],lpMsg->Inventory[i]) == 0)
		{
			pMsg.isItem[i] = 0;
			//LogAdd(LOG_TEST,"Slot = %d empty",i);
			continue;
		}
		//LogAdd(LOG_TEST,"Slot = %d, ItemId = %d",i,item[i].m_Index);
		gItemManager.ItemByteConvert(pMsg.ItemInfo[i],item[i]);
		pMsg.isItem[i] = 1;
	}

	DataSend(lpMsg->aIndex,(BYTE*)&pMsg,pMsg.header.size);
}	