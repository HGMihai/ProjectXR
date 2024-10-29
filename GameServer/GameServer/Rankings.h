#pragma once

#include "Protocol.h"

struct PMSG_DB_REQ_SEND
{
	PSBMSG_HEAD header;
	int aIndex;
};

struct SDHP_RANKINGS_INFO_SEND
{
	PSWMSG_HEAD header;
	WORD aIndex;
	BYTE count;
};

struct SDHP_RANKINGS_INFO
{
	char name[11];
	WORD level;
	BYTE classId;
	BYTE map;
	int reset;
	int masterReset;
	BYTE gensFamily;
	WORD masterLevel;
	BYTE connectStat;
	BYTE accountLevel;
};

struct GC_RANKINGS_INFO_SEND
{
	PSWMSG_HEAD header;
	BYTE count;
	DWORD updateTick;
};

struct CG_RANKINGS_INFO_REQUEST_SEND
{
	PSBMSG_HEAD header;
	char name[11];
};

struct GD_RANKINGS_INFO_REQUEST_SEND
{
	PSBMSG_HEAD header;
	char name[11];
	int aIndex;
};

struct DG_RANKINGS_INFO_REQUEST_RECV
{
	PSWMSG_HEAD header;
	BYTE result;
	int aIndex;
	DWORD Strength;
	DWORD Dexterity;
	DWORD Vitality;
	DWORD Energy;
	DWORD Leadership;
	DWORD PkLevel;
	DWORD PkCount;
	DWORD MapPosX;
	DWORD MapPosY;
	DWORD QuestIndex;
	DWORD QuestStat;
	char GuildName[9];
	BYTE Inventory[INVENTORY_SIZE][ITEM_BYTES_SIZE];
};

struct GC_RANKINGS_INFO_REQUEST_SEND
{
	PSBMSG_HEAD header;
	BYTE result;
	DWORD Strength;
	DWORD Dexterity;
	DWORD Vitality;
	DWORD Energy;
	DWORD Leadership;
	DWORD PkLevel;
	DWORD PkCount;
	DWORD MapPosX;
	DWORD MapPosY;
	DWORD QuestIndex;
	DWORD QuestStat;
	char GuildName[9];
	BYTE ItemInfo[INVENTORY_WEAR_SIZE][12];
	BYTE isItem[INVENTORY_WEAR_SIZE];
	//BYTE Inventory[INVENTORY_SIZE][ITEM_BYTES_SIZE];
};

class CRankings
{
public:
	CRankings();
	void GDSendRequest(int aIndex);
	void GDRecvRankings(SDHP_RANKINGS_INFO_SEND* lpMsg);
	void GCSendRankings(int aIndex);
	void CGRecvCharacterInfoRequest(CG_RANKINGS_INFO_REQUEST_SEND* lpMsg, int aIndex);
	void GDSendCharacterInfoRequest(char* name, int aIndex);
	void DGRecvCharacterInfo(DG_RANKINGS_INFO_REQUEST_RECV* lpMsg);
	// ----
	std::vector<SDHP_RANKINGS_INFO> m_rankings_data;
	DWORD lastUpdateTick;
	int updateTime;
}; extern CRankings gRankings;