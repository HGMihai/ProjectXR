#pragma once

#include "Protocol.h"

struct PMSG_CUSTOM_MOVE_INFO_SEND
{
	PSWMSG_HEAD header;
	BYTE count;
};

#pragma pack(push, 1)
struct CUSTOM_MOVE_INFO
{
	unsigned short index;
	unsigned char map_number;
	unsigned short min_level;
	unsigned int min_reset;
	unsigned int min_greset;
	unsigned char min_account_level;
	unsigned int price;
	unsigned char price_type;
	char text[32];
};
#pragma pack(pop)

class CCustomMove
{
public:
	CCustomMove();
	void load(char* path);
	void move_data_send(int aIndex);
	// ----
	std::map<unsigned short,CUSTOM_MOVE_INFO> m_CustomMoveInfo;
}; extern CCustomMove gCustomMove;