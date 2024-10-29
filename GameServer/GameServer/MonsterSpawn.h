#pragma once
#include "User.h"

struct BOTS_DATA
{
	int NpcID;
	int Level;
	int Reset;
	int Money;
	int WCoinC;
	int WCoinP;
	int GPoints;
	int Credits;
	char Name[30];
};

class CMonsterSpawn
{
public:
	CMonsterSpawn();
	virtual ~CMonsterSpawn();
	void Load(char* path);
	void CreateMonster(LPOBJ lpObj, char* name);
	static void CreateMonsterCallback(LPOBJ lpObj,char* arg,DWORD slot,DWORD WCoinC,DWORD WCoinP,DWORD GoblinPoint);
	bool CheckReq(LPOBJ lpObj, int index, DWORD WCoinC, DWORD WCoinP, DWORD GoblinPoint);
private:
	std::vector<BOTS_DATA> m_Data;
}; extern CMonsterSpawn gMonsterSpawn;