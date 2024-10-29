#pragma once
#if GAMESERVER_UPDATE == 603
#include "User.h"
// -------------------------------------------------------------------------------

#define ITEMRANK_MAX_OPTION		6
// -------------------------------------------------------------------------------

struct ITEMRANK_LIST
{
	BYTE	RankID;
	WORD	ItemType;
};
// -------------------------------------------------------------------------------

struct ItemRank_Data
{
	BYTE	RankID;
	short	OptionValue[ITEMRANK_MAX_OPTION];
	std::vector<ITEMRANK_LIST> ItemList;
	// ----
	ItemRank_Data()
	{
		ZeroMemory(this, sizeof(*this));
	}
};
// -------------------------------------------------------------------------------

class ItemRank
{
public:
			ItemRank();
			~ItemRank();
	// ----
	void Load(char* path);
	// ----
	bool IsRankItem(WORD ItemType);
	short GetValue(WORD ItemType, BYTE OptionType);
	short GetDataIndex(BYTE RankID);
	void SetItemRank(LPOBJ lpObj,CItem* lpItem);
	void CalcItemRank(LPOBJ lpObj, bool flag);
	// ----
private:
	std::vector<ItemRank_Data> m_Data;
	// ----
}; extern ItemRank gItemRank;
// -------------------------------------------------------------------------------
#endif