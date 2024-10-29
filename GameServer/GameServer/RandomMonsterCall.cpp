#include "stdafx.h"
#include "RandomMonsterCall.h"
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

CRandomMonsterCall gRandomMonsterCall;

void CRandomMonsterCall::Load(char* path)
{
	pugi::xml_document file;
	pugi::xml_parse_result res = file.load_file(path);

	if(res.status != pugi::status_ok)
	{
		ErrorMessageBox("%s file load fail (%s)", path, res.description());
	}

	this->m_MonsterCallInfo.clear();

	auto RandomMonsterCall = file.child("RandomMonsterCall");

	for (auto item = RandomMonsterCall.child("Item"); item; item = item.next_sibling())
	{
		RAND_MONSTERCALL_DATA data;

		data.ItemID = item.attribute("Index").as_int();

		int counter=0;

		for(auto monster = item.child("Monster"); monster; monster=monster.next_sibling())
		{
			RAND_MONSTERCALL_INFO info;

			info.MonsterID = monster.attribute("Index").as_int();

			info.MinCount = monster.attribute("MinCount").as_int();

			info.MaxCount = monster.attribute("MaxCount").as_int();

			info.Time = monster.attribute("Time").as_int();

			info.Renewal = monster.attribute("Renewal").as_int();

			info.AllowParty = monster.attribute("AllowParty").as_int();

			info.AllowGuild = monster.attribute("AllowGuild").as_int();

			data.MonsterInfo.emplace_back(info);
		}

		this->m_MonsterCallInfo.emplace_back(data);
	}
}

bool CRandomMonsterCall::CheckMonsterCallItem(int ItemIndex)
{
	for(std::vector<RAND_MONSTERCALL_DATA>::iterator it=this->m_MonsterCallInfo.begin();it != this->m_MonsterCallInfo.end();it++)
	{
		if(it->ItemID == ItemIndex)
		{
			return 1;
		}
	}

	return 0;
}


bool CRandomMonsterCall::CreateMonster(LPOBJ lpObj, int ItemIndex)
{
	for(auto it = this->m_MonsterCallInfo.begin(); it != this->m_MonsterCallInfo.end(); it++)
	{
		if (it->ItemID == ItemIndex)
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

			
			int index = GetLargeRand() % it->MonsterInfo.size();

			int count = it->MonsterInfo.at(index).MinCount;

			if (it->MonsterInfo.at(index).MinCount != it->MonsterInfo.at(index).MaxCount)
			{
				count = it->MonsterInfo.at(index).MinCount + GetLargeRand() % (it->MonsterInfo.at(index).MaxCount - it->MonsterInfo.at(index).MinCount);
			}

			gLog.Output(LOG_MONSTER_CALL,"[MonsterCallRandom] User: (%s - %s) spawned %d monsters with id - %d",lpObj->Account,lpObj->Name,count,it->MonsterInfo.at(index).MonsterID);

			for (int i=0;i<count;i++)
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

				gObjSetMonster(result,it->MonsterInfo.at(index).MonsterID);
				gObj[result].RegenTime = GetTickCount();
				gObj[result].MaxRegenTime = 1000;

				gObj[result].CreatorIndex = lpObj->Index;
				memcpy(gObj[result].CreatorName,lpObj->Name,sizeof(gObj[result].CreatorName));
				gObj[result].SpotTime = time(NULL) + it->MonsterInfo.at(index).Time;
				gObj[result].SpotRenewal = (bool)it->MonsterInfo.at(index).Renewal;
				gObj[result].SpotAllowParty = (bool)it->MonsterInfo.at(index).AllowParty;
				gObj[result].SpotAllowGuild = (bool)it->MonsterInfo.at(index).AllowGuild;
			}

			return TRUE;
		}
	}

	return false;
}

int CRandomMonsterCall::RespawnMonster(LPOBJ lpObj)
{
	return false;
}
