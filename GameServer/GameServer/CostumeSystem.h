#pragma once

#include "Protocol.h"

struct PMSG_COSTUME_INFO_SEND
{
	PSWMSG_HEAD header;
	BYTE count;
};

struct COSTUME_INFO
{
	short costumeItemIndex;
	short itemIndex;
	BYTE costumeLevel;
};

struct COSTUME_OPTIONS
{
	WORD Option1;
	WORD Option2;
	WORD Option3;
	WORD Option4;
	WORD Option5;
	WORD Option6;
};

struct COSTUME_LIST
{
	COSTUME_INFO costumeInfo;
	COSTUME_OPTIONS costumeOptions;
	BYTE costumeOptionColor;
};

struct PMSG_COSTUME_SEND
{
	PSBMSG_HEAD header;
	COSTUME_INFO info;
	int aIndex;
};

class CCostumeSystem
{
public:
	CCostumeSystem();
	virtual ~CCostumeSystem();
	// ----
	void load(char* path);
	void GCCostumeSend(LPOBJ lpObj, int aIndex);
	void GCCostumeListSend(int aIndex);
	bool checkIsCostumeItem(int itemId);
	int GetCostumeSystemNewOptionValue(int itemId, int optionNumber);

	// ----
	std::map<int,COSTUME_LIST> m_CostumeInfo;
};

extern CCostumeSystem gCostumeSystem;