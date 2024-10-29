#pragma once
#include "User.h"

struct PICKUP
{
	int ItemIndex;
	int ItemID;
};

enum ePickupType
{
	PICKUP_NONE = 0,
	PICKUP_EXCELLENT = 1,
	PICKUP_JEWELS = 2,
};

class Pickup
{
public:
	Pickup();
	~Pickup();
	void Load(char* path);
	void ReadCustomPickInfo(char* section,char* path);
	void PickMsgProc(LPOBJ lpObj);
	void PickCommand(LPOBJ lpObj, char* arg);
	std::vector<PICKUP> m_PickupStruct;
private:
	int IsEnabled[4];
	DWORD PriceZen[4];
	char Command1[20];
	char Command2[20];
	char Command3[20];
}; extern Pickup gPickup;
