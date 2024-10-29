#pragma once

#include "User.h"

struct ItemToPointsData
{
	int ItemID;
	int ItemLevel;
	int Points;
	int Time;
};

class CItemToPoints
{
public:
	CItemToPoints();
	void Load(char* path);
	bool Dialog(LPOBJ lpObj, LPOBJ lpNpc);
	void CheckItem(LPOBJ lpObj);
	void Tick(LPOBJ lpObj);
	void Update(LPOBJ lpObj);
	void ResetStats(LPOBJ lpObj);
private:
	std::vector<ItemToPointsData> m_Data;
	bool m_Enable;
	int m_NpcID;
	int m_NpcMap;
	int m_NpcX;
	int m_NpcY;
}; extern CItemToPoints gItemToPoints;