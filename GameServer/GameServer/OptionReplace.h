#pragma once

#include "Item.h"

struct OPTION_REPLACE_INFO
{
	int ItemIndex;
	int OptionIndex;
	int ClientOptionIndex;
	int NewOptionIndex;
	int NewOptionValue;
};

class COptionReplace{
public:
	COptionReplace();
	// ----
	void Load(char* path);
	bool IsItemOptionReplace(int itemIndex, int optionIndex);
	void DoItemOptionReplace(int optionIndex, int index, CItem* lpItem);
	// ----
	std::vector<OPTION_REPLACE_INFO> m_OptionReplaceData;
}; extern COptionReplace gOptionReplace;