#include "stdafx.h"
#include "MonsterSpawn.h"
#include "MemScript.h"
#include "Util.h"
#include "Map.h"
#include "Monster.h"
#include "MasterSkillTree.h"
#include "CommandManager.h"
#include "CashShop.h"
#include "Message.h"
#include "Notice.h"


CMonsterSpawn gMonsterSpawn;

CMonsterSpawn::CMonsterSpawn()
{
	//this->m_Data.clear();
}

CMonsterSpawn::~CMonsterSpawn()
{
	
}

void CMonsterSpawn::Load(char* path)
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

	this->m_Data.clear();

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

			BOTS_DATA info;

			info.NpcID = lpMemScript->GetNumber();
			info.Level = lpMemScript->GetAsNumber();
			info.Reset = lpMemScript->GetAsNumber();
			info.Money = lpMemScript->GetAsNumber();
			info.WCoinC = lpMemScript->GetAsNumber();
			info.WCoinP = lpMemScript->GetAsNumber();
			info.GPoints = lpMemScript->GetAsNumber();
			info.Credits = lpMemScript->GetAsNumber();
			strncpy(info.Name, lpMemScript->GetAsString(), sizeof(info.Name));
			this->m_Data.push_back(info);
		}
	}
	catch(...)
	{
		ErrorMessageBox(lpMemScript->GetLastError());
	}

	delete lpMemScript;
}

void CMonsterSpawn::CreateMonster(LPOBJ lpObj, char* arg)
{
	gCashShop.GDCashShopRecievePointSend(lpObj->Index,(DWORD)&CMonsterSpawn::CreateMonsterCallback,(DWORD)arg,0);
}

void CMonsterSpawn::CreateMonsterCallback(LPOBJ lpObj, char *arg, DWORD slot, DWORD WCoinC, DWORD WCoinP, DWORD GoblinPoint)
{
	char name[32] = {0};

	gCommandManager.GetString(arg,name,sizeof(name),0);

	if (strcmp(name,"remove") == 0 || atoi(name) == -1)
	{
		if(OBJECT_RANGE(lpObj->SummonIndex) != 0)
		{
			gObjSummonKill(lpObj->Index);
			GCSummonLifeSend(lpObj->Index,0,1);
		}
	}

	for	(int i=0;i<gMonsterSpawn.m_Data.size();i++)
	{
		if (strcmp(name,gMonsterSpawn.m_Data[i].Name) == 0)
		{
			if (!gMonsterSpawn.CheckReq(lpObj,i,WCoinC,WCoinP,GoblinPoint))
			{
				return;
			}

			if(lpObj->Map == MAP_ICARUS || CC_MAP_RANGE(lpObj->Map) != 0)
			{
				return;
			}

			if(OBJECT_RANGE(lpObj->SummonIndex) != 0)
			{
				gObjSummonKill(lpObj->Index);
				GCSummonLifeSend(lpObj->Index,0,1);
				return;
			}

			int index = gObjAddSummon();

			if(OBJECT_RANGE(index) == 0)
			{
				return;
			}

			lpObj->Money -= gMonsterSpawn.m_Data[i].Money;

			gCashShop.GDCashShopSubPointSaveSend(lpObj->Index,0,gMonsterSpawn.m_Data[i].WCoinC,gMonsterSpawn.m_Data[i].WCoinP,gMonsterSpawn.m_Data[i].GPoints);

			lpObj->SummonIndex = index;

			LPOBJ lpSummon = &gObj[index];

			lpSummon->X = lpObj->X-1;
			lpSummon->Y = lpObj->Y+1;
			lpSummon->MTX = lpSummon->X;
			lpSummon->MTY = lpSummon->Y;
			lpSummon->Dir = 2;
			lpSummon->Map = lpObj->Map;

			gObjSetMonster(index,gMonsterSpawn.m_Data[i].NpcID);

#if(GAMESERVER_UPDATE>=602)
			lpSummon->Life += (float)((__int64)lpSummon->Life*gMasterSkillTree.GetMasterSkillValue(lpObj,MASTER_SKILL_ADD_SUMMON_LIFE))/100;
			lpSummon->MaxLife += (float)((__int64)lpSummon->MaxLife*gMasterSkillTree.GetMasterSkillValue(lpObj,MASTER_SKILL_ADD_SUMMON_LIFE))/100;
			lpSummon->ScriptMaxLife += (float)((__int64)lpSummon->ScriptMaxLife*gMasterSkillTree.GetMasterSkillValue(lpObj,MASTER_SKILL_ADD_SUMMON_LIFE))/100;
			lpSummon->Defense += ((__int64)lpSummon->Defense*gMasterSkillTree.GetMasterSkillValue(lpObj,MASTER_SKILL_ADD_SUMMON_DEFENSE))/100;
			lpSummon->PhysiDamageMin += ((__int64)lpSummon->PhysiDamageMin*gMasterSkillTree.GetMasterSkillValue(lpObj,MASTER_SKILL_ADD_SUMMON_DAMAGE))/100;
			lpSummon->PhysiDamageMax += ((__int64)lpSummon->PhysiDamageMax*gMasterSkillTree.GetMasterSkillValue(lpObj,MASTER_SKILL_ADD_SUMMON_DAMAGE))/100;
#endif
			lpSummon->SummonIndex = lpObj->Index;
			lpSummon->Attribute = 100;
			lpSummon->TargetNumber = -1;
			lpSummon->ActionState.Attack = 0;
			lpSummon->ActionState.Emotion = 0;
			lpSummon->ActionState.EmotionCount = 0;
			lpSummon->PathCount = 0;
			lpSummon->MoveRange = 15;

			GCSummonLifeSend(lpSummon->SummonIndex,(int)lpSummon->Life,(int)lpSummon->MaxLife);
		}
	}
}


bool CMonsterSpawn::CheckReq(LPOBJ lpObj, int index, DWORD WCoinC, DWORD WCoinP, DWORD GoblinPoint)
{
	if (lpObj->Level < this->m_Data[index].Level)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(521), this->m_Data[index].Level);
		return false;
	}

	if (lpObj->Reset < this->m_Data[index].Reset)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(528), this->m_Data[index].Reset);
		return false;
	}

	if (lpObj->Money < this->m_Data[index].Money)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(498), this->m_Data[index].Money);
		return false;
	}

	if (WCoinC < this->m_Data[index].WCoinC)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(495), this->m_Data[index].WCoinC);
		return false;
	}

	if (WCoinP < this->m_Data[index].WCoinP)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(496), this->m_Data[index].WCoinP);
		return false;
	}

	if (GoblinPoint < this->m_Data[index].GPoints)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(497), this->m_Data[index].GPoints);
		return false;
	}

	if (lpObj->BankEx.Credits < this->m_Data[index].Credits)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(523), this->m_Data[index].Credits);
		return false;
	}

	return true;
}

