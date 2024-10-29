#pragma once

#include "User.h"

struct CALL_INFO
{
	//int Index;
	int ItemID;
	int MonsterID;
	int Count;
	int Time;
	int Renewal;
	int AllowParty;
	int AllowGuild;
};

//struct RANDOM_CALL_INFO
//{
//	int Index;
//	int ItemID;
//	int SpotIndex;
//
//};

class cMonsterCall
{
public:
	void Load(char* path);
	int SetMonster(LPOBJ lpObj,CItem* lpItem);
	int RespawnMonster(LPOBJ lpObj);
	bool gObjMonsterRegen(LPOBJ lpObj, LPOBJ Player);
	bool CheckAttack(LPOBJ lpObj,LPOBJ lpMonster);
	std::vector<CALL_INFO> m_MonsterCallInfo;
}; extern cMonsterCall gMonsterCall;