#pragma once

#include "User.h"

struct RAND_MONSTERCALL_INFO
{
	int MonsterID;
	int MinCount;
	int MaxCount;
	int Time;
	int Renewal;
	int AllowParty;
	int AllowGuild;
};

struct RAND_MONSTERCALL_DATA
{
	int ItemID;
	//RAND_MONSTERCALL_INFO MonsterInfo[100];
	std::vector<RAND_MONSTERCALL_INFO> MonsterInfo;
};

class CRandomMonsterCall
{
public:
	void Load(char* path);
	bool CheckMonsterCallItem(int ItemIndex);
	bool CreateMonster(LPOBJ lpObj,int ItemIndex);
	int RespawnMonster(LPOBJ lpObj);
private:
	std::vector<RAND_MONSTERCALL_DATA> m_MonsterCallInfo;

}; extern CRandomMonsterCall gRandomMonsterCall;