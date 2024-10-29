#include "stdafx.h"
#include "DailyReward.h"
#include "MemScript.h"
#include "Util.h"
#include "DSProtocol.h"
#include "ItemManager.h"
#include "BankEx.h"
#include "CashShop.h"
#include "ObjectManager.h"

#define USE_DAILY_REWARD 1

CDailyReward gDailyReward;

CDailyReward::CDailyReward()
{
	this->m_RewardData.clear();
}

CDailyReward::~CDailyReward()
{
	
}

void CDailyReward::InitStruct(DAILY_REWARD_DATA* Data)
{
	Data->Type		   = -1;
	Data->ItemIndex	   = 0;
	Data->ItemLevel	   = 0;
	Data->ItemLuck	   = 0;
	Data->ItemSkill	   = 0;
	Data->ItemOption   = 0;
	Data->ItemExc	   = 0;
	Data->ItemAnc	   = 0;
	//Data->ZenCount	   = 0;
	//Data->CreditsCount = 0;
	//Data->WCoinCCount  = 0;
	//Data->WCoinPCount  = 0;
	//Data->GCoinCount   = 0;
	//Data->PointsCount  = 0;
	Data->Count = 0;
	Data->PointsType   = 0;
}


void CDailyReward::Load(char* path)
{
#if (USE_DAILY_REWARD == 0)
	return;
#endif
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

	this->m_RewardData.clear();

	int Index = 0;
	
	try
	{
		while(true)
		{
			if(lpMemScript->GetToken() == TOKEN_END)
			{
				break;
			}

			if(strcmp("end",lpMemScript->GetString()) == 0)
			{
				break;
			}

			DAILY_REWARD_DATA info;

			this->InitStruct(&info);

			info.Type = lpMemScript->GetNumber();
	
			info.Index = Index++;

			//Type 0 - Item
			//Type 1 - Zen
			//Type 2 - Credits
			//Type 3 - WCoinC
			//Type 4 - WCoinP
			//Type 5 - Goblin Points
			//Type 6 - Points

			if (info.Type == 0)
			{
				int type = lpMemScript->GetAsNumber();
				int index = lpMemScript->GetAsNumber();
				info.ItemIndex = type*512+index;
				info.ItemLevel = lpMemScript->GetAsNumber();
				info.ItemLuck = lpMemScript->GetAsNumber();
				info.ItemSkill = lpMemScript->GetAsNumber();
				info.ItemOption = lpMemScript->GetAsNumber();
				info.ItemExc = lpMemScript->GetAsNumber();
				info.ItemAnc = lpMemScript->GetAsNumber();
			}
			else if (info.Type == 1)
			{
				info.Count = lpMemScript->GetAsNumber();
			}
			else if (info.Type == 2)
			{
				info.Count = lpMemScript->GetAsNumber();
			}
			else if (info.Type == 3)
			{
				info.Count = lpMemScript->GetAsNumber();
			}
			else if (info.Type == 4)
			{
				info.Count = lpMemScript->GetAsNumber();
			}
			else if (info.Type == 5)
			{
				info.Count = lpMemScript->GetAsNumber();
			}
			else if (info.Type == 6)
			{
				info.Count = lpMemScript->GetAsNumber();
				info.PointsType = lpMemScript->GetAsNumber();
			}

			this->m_RewardData.push_back(info);
		}
	}
	catch(...)
	{
		ErrorMessageBox(lpMemScript->GetLastError());
	}

	delete lpMemScript;
}

void CDailyReward::SendInfo(int aIndex)
{
#if (USE_DAILY_REWARD == 0)
	return;
#endif
	BYTE send[8192];

	PMSG_SEND_DAILYREWARD pMsg;
	pMsg.header.set(0xF3, 0xF9, sizeof(pMsg));

	int size = sizeof(pMsg);

	pMsg.Count = 0;

	for (int i=0;i<this->m_RewardData.size();i++)
	{
		if (i>=100)
		{
			break;
		}

		memcpy(&send[size],&this->m_RewardData[i],sizeof(this->m_RewardData[i]));
		size += sizeof(this->m_RewardData[i]);

		pMsg.Count ++;
	}

	pMsg.header.size[0] = SET_NUMBERHB(size);

	pMsg.header.size[1] = SET_NUMBERLB(size);

	memcpy(send,&pMsg,sizeof(pMsg));
	DataSend(aIndex,send,size);
}

void CDailyReward::SendRewardInfo(int aIndex)
{
#if (USE_DAILY_REWARD == 0)
	return;
#endif

	LPOBJ lpObj = &gObj[aIndex];
	
	if(lpObj->Interface.use != 0 || lpObj->State == OBJECT_DELCMD || lpObj->DieRegen != 0 || lpObj->Teleport != 0 || lpObj->PShopOpen != 0 || lpObj->SkillSummonPartyTime != 0)
	{
		return;
	}

	int randValue = GetLargeRand() % this->m_RewardData.size();

	DAILY_REWARD_DATA* Data = &this->m_RewardData[randValue];

	//LogAdd(LOG_TEST,"index = %d ItemId = %d Level = %d Type = %d Value = %d",randValue,Data->ItemIndex,Data->ItemLevel,Data->Type,Data->Count);

	PMSG_SEND_REWARDINFO pRequest;
	pRequest.Head.set(0xFB,0x1A, sizeof(pRequest));

	pRequest.Index = randValue;

	DataSend(aIndex, (LPBYTE)&pRequest, sizeof(pRequest));

	this->MakeReward(aIndex,randValue);
}

void CDailyReward::MakeReward(int aIndex, int rewardIndex)
{
	LPOBJ lpObj = &gObj[aIndex];
	
	if(lpObj->Interface.use != 0 || lpObj->State == OBJECT_DELCMD || lpObj->DieRegen != 0 || lpObj->Teleport != 0 || lpObj->PShopOpen != 0 || lpObj->SkillSummonPartyTime != 0)
	{
		return;
	}

	if(rewardIndex >= this->m_RewardData.size())
	{
		return;
	}

	DAILY_REWARD_DATA* info = &this->m_RewardData[rewardIndex];

	if(info->Type == 0)
	{
		int dur = gItemManager.GetItemDurability(info->ItemIndex,info->ItemLevel,info->ItemExc,info->ItemAnc);
		GDCreateItemSend(lpObj->Index,0xEB,0,0,info->ItemIndex,info->ItemLevel,dur,info->ItemSkill,info->ItemLuck,info->ItemOption,-1,info->ItemExc,info->ItemAnc,0,0,0,0xFF,0);
	}
	else if(info->Type == 1)
	{
		lpObj->Money += info->Count;
		GCMoneySend(lpObj->Index, lpObj->Money);
	}
	else if(info->Type == 2)
	{
		lpObj->BankEx.Credits += info->Count;
		gBankEx.GCUpdateBankEx(lpObj->Index);
		gBankEx.GDSavePoint(lpObj->Index);
	}
	else if(info->Type == 3)
	{
		gCashShop.GDCashShopAddPointSaveSend(lpObj->Index, 0, info->Count, 0, 0);
	}
	else if(info->Type == 4)
	{
		gCashShop.GDCashShopAddPointSaveSend(lpObj->Index, 0, 0, info->Count, 0);
	}
	else if(info->Type == 5)
	{
		gCashShop.GDCashShopAddPointSaveSend(lpObj->Index, 0, 0, 0, info->Count);
	}
	else if (info->Type == 6)
	{
		if(info->PointsType == 0)
		{
			lpObj->LevelUpPoint += info->Count;
			CDailyReward::Update(aIndex);
		}
		else
		{
			lpObj->iQuestStat += info->Count;
			lpObj->LevelUpPoint += info->Count;
			CDailyReward::Update(aIndex);
		}
	}
}

void CDailyReward::Update(int aIndex)
{
	gObjectManager.CharacterCalcAttribute(aIndex);

	GCNewCharacterInfoSend(&gObj[aIndex]);

	GDCharacterInfoSaveSend(aIndex);
}

