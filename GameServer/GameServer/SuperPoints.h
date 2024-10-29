#pragma once

#include "User.h"

struct SUPERPOINTS_INFO
{
	int ItemID;
	int ItemLevel;
	int Points;
};

struct SUPERPOINTS_NPC_DATA
{
	int NPC_Id;
	int NPC_Map;
	int NPC_X;
	int NPC_Y;
};

class cSuperPoints
{
public:
	cSuperPoints();
	virtual ~cSuperPoints();
	void Load(char* path);
	bool Dialog(LPOBJ lpObj, LPOBJ lpNpc);
	void CheckItem(LPOBJ lpObj);
private:
	std::vector<SUPERPOINTS_INFO> m_SuperPointsInfo;
	SUPERPOINTS_NPC_DATA m_NpcData;
}; extern cSuperPoints gSuperPoints;