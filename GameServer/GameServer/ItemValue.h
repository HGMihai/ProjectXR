// ItemValue.h: interface for the CItemValue class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "Item.h"
#include "Protocol.h"

struct ITEM_VALUE_INFO
{
	int Index;
	int Level;
	int Grade;
	int AncOption;
	int OptionEx;
	int Type;
	int BuyPrice;
	int SellPrice;
};

enum ItemPriceType
{
	ZEN = 0,
	CREDITS = 1,
	WCOINC = 2,
	WCOINP = 3,
	WCOING = 4,
};

struct PMSG_SEND_ITEM_VALUE
{
	PSWMSG_HEAD header;
	BYTE count;
	BYTE is_first;
};

class CItemValue
{
public:
	CItemValue();
	virtual ~CItemValue();
	void Load(char* path);
	bool GetItemValue(CItem* lpItem, int* type, int* BuyPrice, int* SellPrice);
	void GCItemValueSend(int aIndex, std::vector<ITEM_VALUE_INFO>::iterator);
	std::vector<ITEM_VALUE_INFO>::iterator GetBeginIterator();
private:
	//std::map<int, ITEM_VALUE_INFO> m_ItemValueInfo;
	std::vector<ITEM_VALUE_INFO> m_ItemValueInfo;
};

extern CItemValue gItemValue;
