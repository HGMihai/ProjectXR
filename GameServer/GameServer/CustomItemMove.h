#pragma once

#include "Protocol.h"
#include "ItemManager.h"

struct PMSG_CUSTOM_ITEM_MOVE
{
	PSBMSG_HEAD header;
	BYTE type;
	BYTE slot;
	//BYTE ItemInfo[MAX_ITEM_INFO];
};

enum ItemMoveType
{
	ChaosBox = 0,
};

class CCustomItemMove
{
public:
	void CGItemMove(PMSG_CUSTOM_ITEM_MOVE* lpMsg, int aIndex);
};

extern CCustomItemMove gCustomItemMove;