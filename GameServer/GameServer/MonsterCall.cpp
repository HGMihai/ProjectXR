#include "stdafx.h"
#include "MonsterCall.h"
#include "Monster.h"
#include "EffectManager.h"
#include "Crywolf.h"
#include "CrywolfSync.h"
#include "MemScript.h"
#include "Util.h"
#include "Notice.h"
#include "Message.h"
#include "MapManager.h"
#include "Log.h"

cMonsterCall gMonsterCall;

/*void cMonsterCall::Load(char* path)
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

	this->m_MonsterCallInfo.clear();

	try
	{
		while(true)
		{
			if(lpMemScript->GetToken() == TOKEN_END)
			{
				break;
			}

			int section = lpMemScript->GetNumber();

			if (section == 0)
			{
				if(strcmp("end",lpMemScript->GetString()) == 0)
				{
					break;
				}

				CALL_INFO info;

				info.ItemID = lpMemScript->GetNumber();

				info.MonsterID = lpMemScript->GetAsNumber();

				info.Count = lpMemScript->GetAsNumber();

				info.Time = lpMemScript->GetAsNumber();

				info.Renewal = lpMemScript->GetAsNumber();

				info.AllowParty = lpMemScript->GetAsNumber();

				info.AllowGuild = lpMemScript->GetAsNumber();

				this->m_MonsterCallInfo.push_back(info);
			}
			else if (section == 1)
			{
				
			}
		}
	}
	catch(...)
	{
		ErrorMessageBox(lpMemScript->GetLastError());
	}

	delete lpMemScript;
}*/

void cMonsterCall::Load(char* path)
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

	this->m_MonsterCallInfo.clear();

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

			CALL_INFO info;

			info.ItemID = lpMemScript->GetNumber();

			info.MonsterID = lpMemScript->GetAsNumber();

			info.Count = lpMemScript->GetAsNumber();

			info.Time = lpMemScript->GetAsNumber();

			info.Renewal = lpMemScript->GetAsNumber();

			info.AllowParty = lpMemScript->GetAsNumber();

			info.AllowGuild = lpMemScript->GetAsNumber();

			//LogAdd(LOG_TEST,"[MonsterCall] Item: %d Monster: %d Count: %d Time: %d Renew: %d Party: %d Guild: %d", info.ItemID,info.MonsterID,info.Count,
			//	info.Time,info.Renewal,info.AllowParty,info.AllowGuild);

			this->m_MonsterCallInfo.push_back(info);
		}
	}
	catch(...)
	{
		ErrorMessageBox(lpMemScript->GetLastError());
	}

	delete lpMemScript;
}

int cMonsterCall::SetMonster(LPOBJ lpObj,CItem* lpItem)
{
	std::vector<CALL_INFO>::iterator it = this->m_MonsterCallInfo.begin();

	for(; it != this->m_MonsterCallInfo.end(); it++)
	{
		if (it->ItemID == lpItem->m_Index)
		{
			if (gMapManager.GetMapMonsterCallEnable(lpObj->Map) == 0)
			{
				gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(529));
				return FALSE;
			}

			if(gMap[lpObj->Map].CheckAttr(lpObj->X,lpObj->Y,1) != 0)
			{
				gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(529));
				return FALSE;
			}

			lpObj->SpotX = lpObj->X;
			lpObj->SpotY = lpObj->Y;

			gLog.Output(LOG_MONSTER_CALL,"[MonsterCall] Item:(%d) User: (%s - %s) spawned %d monsters with id - %d",it->ItemID,lpObj->Account,lpObj->Name,it->Count,it->MonsterID);

			for (int i=0;i<it->Count;i++)
			{
				int result = gObjAddMonster(lpObj->Map);

				gObj[result].PosNum = (WORD)-1;
				gObj[result].Map = lpObj->Map;
				gObj[result].X = lpObj->X;
				gObj[result].Y = lpObj->Y;
				gObj[result].TX = lpObj->X;
				gObj[result].TY = lpObj->Y;
				gObj[result].OldX = lpObj->X;
				gObj[result].OldY = lpObj->Y;
				gObj[result].StartX = lpObj->X;
				gObj[result].StartY = lpObj->Y;
				gObj[result].Dir = rand()%8;

				gObj[result].SpotX = lpObj->X;
				gObj[result].SpotY = lpObj->Y;

				gObj[result].ActionState.Emotion       = 1;
				gObj[result].ActionState.EmotionCount  = 15;

				gObjSetMonster(result,it->MonsterID);
				gObj[result].RegenTime = GetTickCount();
				gObj[result].MaxRegenTime = 1000;

				gObj[result].CreatorIndex = lpObj->Index;
				memcpy(gObj[result].CreatorName,lpObj->Name,sizeof(gObj[result].CreatorName));
				gObj[result].SpotTime = time(NULL) + it->Time;
				gObj[result].SpotRenewal = (bool)it->Renewal;
				gObj[result].SpotAllowParty = (bool)it->AllowParty;
				gObj[result].SpotAllowGuild = (bool)it->AllowGuild;
			}

			return TRUE;
		}
	}

	return FALSE;
}

int cMonsterCall::RespawnMonster(LPOBJ lpObj)
{
	if ( lpObj->CreatorIndex != -1 )
	{
		if ( time(NULL) >= lpObj->SpotTime/* ||  (lpObj->SpotRenewal == false && lpObj->SpotKillCount > 0)*/)
		{
			gObjDel(lpObj->Index);
			return 0;
		}

		if (lpObj->SpotRenewal == false && lpObj->SpotKillCount > 0)
		{
			gObjMonsterDieGiveItem(lpObj,&gObj[lpObj->CreatorIndex]);
			gObjDel(lpObj->Index);
			return 0;
		}
		//gObjMonsterDieGiveItem(lpObj,lpTarget);

		lpObj->Teleport = ((lpObj->Teleport==3)?0:lpObj->Teleport);

		if(lpObj->State == OBJECT_CREATE)
		{
			lpObj->State = OBJECT_PLAYING;
			lpObj->RegenOk = ((lpObj->RegenOk==3)?0:lpObj->RegenOk);
		}

		if(lpObj->DieRegen != 2)
		{
			return 0;
		}

		lpObj->AttackerKilled = 0;

		if(lpObj->CurrentAI != 0 && lpObj->RegenType != 0)
		{
			return 0;
		}

		lpObj->Live = 1;
		lpObj->ViewState = 0;
		lpObj->Teleport = 0;
		lpObj->Life = lpObj->MaxLife+lpObj->AddLife;
		lpObj->Mana = lpObj->MaxMana+lpObj->AddMana;

		gEffectManager.ClearAllEffect(lpObj);

		if(this->gObjMonsterRegen(lpObj, &gObj[lpObj->CreatorIndex]) == 0)
		{
			return 0;
		}

		lpObj->DieRegen = 0;
		lpObj->State = OBJECT_CREATE;

		gObjViewportListProtocolCreate(lpObj);
		return TRUE;
	}

	return 0;
}

bool cMonsterCall::gObjMonsterRegen(LPOBJ lpObj, LPOBJ Player) // OK
{
	gObjClearViewport(lpObj);

	lpObj->TargetNumber = -1;
	lpObj->LastAttackerID = -1;
	lpObj->NextActionTime = 5000;
	lpObj->ActionState.Rest = 0;
	lpObj->ActionState.Attack = 0;
	lpObj->ActionState.Move = 0;
	lpObj->ActionState.Escape = 0;
	lpObj->ActionState.Emotion = 0;
	lpObj->ActionState.EmotionCount = 0;

	int px = lpObj->SpotX;
	int py = lpObj->SpotY;

	gObjGetRandomFreeLocation(lpObj->Map,&px,&py,1,1,30);

	lpObj->X = px;
	lpObj->Y = py;

	lpObj->MTX = px;
	lpObj->MTY = py;
	lpObj->TX = lpObj->X;
	lpObj->TY = lpObj->Y;
	lpObj->StartX = (BYTE)lpObj->X;
	lpObj->StartY = (BYTE)lpObj->Y;
	lpObj->PathCur = 0;
	lpObj->PathCount = 0;
	lpObj->PathStartEnd = 0;

	gObjMonsterInitHitDamage(lpObj);

	if(lpObj->Map != MAP_CRYWOLF || (gCrywolfSync.GetCrywolfState() != CRYWOLF_STATE_NOTIFY2 && gCrywolfSync.GetCrywolfState() != CRYWOLF_STATE_READY && gCrywolfSync.GetCrywolfState() != CRYWOLF_STATE_START && gCrywolfSync.GetCrywolfState() != CRYWOLF_STATE_END))
	{
		if(gCrywolfSync.CheckApplyBenefit() != 0 && gCrywolfSync.GetOccupationState() == 0)
		{
			lpObj->Life = (lpObj->ScriptMaxLife*gCrywolfSync.GetMonHPBenefitRate())/100;
			lpObj->MaxLife = (lpObj->ScriptMaxLife*gCrywolfSync.GetMonHPBenefitRate())/100;
		}
	}

	return 1;
}

bool cMonsterCall::CheckAttack(LPOBJ lpObj,LPOBJ lpMonster)
{
	if (lpMonster->CreatorIndex == -1)
	{
		return 1;
	}
	
	if (lpMonster->CreatorIndex != -1)
	{
		LPOBJ lpCreator = gObjFind(lpMonster->CreatorName);
		
		if (lpCreator == nullptr)
		{
			return false;
		}

		if (lpMonster->CreatorIndex != lpCreator->Index)
		{
			lpMonster->CreatorIndex = lpCreator->Index;
		}
	}

	if ( lpMonster->CreatorIndex != -1 && lpMonster->CreatorIndex == lpObj->Index)
	{
		return 1;
	}

	bool party_value = false;
	bool guild_value = false;
	
	if(lpMonster->SpotAllowGuild)
	{
		if(gObj[lpMonster->CreatorIndex].GuildNumber == -1)
			guild_value = false;

		if(gObj[lpMonster->CreatorIndex].GuildNumber != -1 && 
			gObj[lpMonster->CreatorIndex].GuildNumber == lpObj->GuildNumber)
			guild_value = true;
	}

	if(lpMonster->SpotAllowParty)
	{
		if(gObj[lpMonster->CreatorIndex].PartyNumber == -1)
			party_value = false;

		if(gObj[lpMonster->CreatorIndex].PartyNumber != -1 && 
			gObj[lpMonster->CreatorIndex].PartyNumber == lpObj->PartyNumber)
			party_value = true;
	}

	if(lpMonster->SpotAllowGuild == true && lpMonster->SpotAllowParty == false)
		return guild_value;

	if(lpMonster->SpotAllowParty == true && lpMonster->SpotAllowGuild == false)
		return party_value;

	return party_value || guild_value;
}