#pragma once
// ----------------------------------------------------------------------------------------------

#include "StdAfx.h"
#include "Protocol.h"
// ----------------------------------------------------------------------------------------------

#if(CUSTOM_PERSONAL_SHOP==TRUE)

struct GC_PersonalShopInfo
{
	WORD aIndex;
	char Name[11];
	char PersonalText[36];
	BYTE Value;
};
// ----------------------------------------------------------------------------------------------

struct CG_PersonalPage
{
	PSBMSG_HEAD h;
	BYTE Page;
};
// ----------------------------------------------------------------------------------------------

struct GC_PSInventoryValue
{
	WORD ItemPos;
	DWORD Zen;
	DWORD Bonus;
	WORD Bless;
	WORD Soul;
	WORD Chaos;
	WORD Life;
};
// ----------------------------------------------------------------------------------------------

class PersonalShopEx
{
public:
	void Init();
	void Load();
	void Read(char* File);

	bool NPC_Dialog(int aIndex, int aIndexNPC);
	
	void Search(int aIndex, CG_PersonalPage* aRecv);
	void UpdatePSInventoryUser(int aIndex);
	void UpdatePSInventoryTarget(int aIndex, int aTargetIndex);
	int GetMoney(int aIndex, int Pos);
	bool CheckPrice(int Zen, int Bonus, int Bless, int Soul, int Chaos, int Life);
	bool PShop_CheckInventoryPrice(short aIndex);

	bool Enable;
	int NPC_CLASS;
	int NPC_MAP;
	int NPC_X;
	int NPC_Y;
	int NPC_MAP2;
	int NPC_X2;
	int NPC_Y2;
	int NPC_MAP3;
	int NPC_X3;
	int NPC_Y3;
	int NPC_MAP4;
	int NPC_X4;
	int NPC_Y4;

	int BonusPrice;
};
extern PersonalShopEx g_PersonalShopEx;
// ----------------------------------------------------------------------------------------------
#endif