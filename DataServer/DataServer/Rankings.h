#pragma once

#include "DataServerProtocol.h"

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

struct GD_RANKINGS_INFO_REQUEST_RECV
{
	PSBMSG_HEAD header;
	char name[11];
	int aIndex;
};

struct DG_RANKINGS_INFO_REQUEST_SEND
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

class CRankings
{
public:
	void GDRecvRequest(PMSG_DB_REQ_SEND* lpMsg, int Index);
	void GDCharacterInfoRequest(GD_RANKINGS_INFO_REQUEST_RECV* lpMsg, int Index);
}; extern CRankings gRankings;