#pragma once

#include "Protocol.h"

//Type 0 - Item
//Type 1 - Zen
//Type 2 - Credits
//Type 3 - WCoinC
//Type 4 - WCoinP
//Type 5 - Goblin Points
//Type 6 - Points
//Type 7 - EventBag

struct PMSG_SEND_ROULETTE
{
	PSWMSG_HEAD header;
	int Count;
};

struct ROULETTE_INFO
{
	int index;
	BYTE type;
	int item_index;
	BYTE item_level;
	BYTE item_luck;
	BYTE item_skill;
	BYTE item_option;
	BYTE item_excellent;
	BYTE item_ancient;
	int count;
	BYTE points_type;
	int rate;
};

struct PMSG_SEND_ROULETTE_INDEX
{
	PSBMSG_HEAD header;
	int index;
};

class CRoulette
{
public:
	CRoulette();
	void load(char* path);
	void send_roulette_data(int aIndex);
	void recv_roulette_start(int aIndex);
	// ----
	std::vector<ROULETTE_INFO> m_roulette_data;
	bool is_enabled;
}; extern CRoulette gRoulette;