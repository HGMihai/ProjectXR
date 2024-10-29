#pragma once

#include "CustomJewel.h"

struct JEWEL_MIX_DATA
{
	int JewelID;
	int JewelBoundleID;
	char Text[30];
};

class CCustomJewelMix
{
public:
	CCustomJewelMix();
	virtual ~CCustomJewelMix();
	void Load(char* path);
private:
	//JEWEL_MIX_DATA m_JewelMixInfo[MAX_CUSTOM_JEWEL];
	std::vector<JEWEL_MIX_DATA> m_JewelMixInfo;
}; extern CCustomJewelMix gCustomJewelMix;