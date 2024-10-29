#pragma once

#include "Protocol.h"
#include "User.h"

struct PMSG_DONATESHOP_INFO
{
	PSBMSG_HEAD	Head;
	int PageCount;
};

struct PMSG_DONATESHOP_STATE
{
	PSBMSG_HEAD Head;
	int State;
	int Page;
	int MaxPage;
	int Credits;
};

struct PMSG_DONATESHOP_PAGE_RECV
{
	PSBMSG_HEAD header;
	int Type;
};

struct PMSG_DONATESHOP_SETPAGE_SEND
{
	PSBMSG_HEAD header;
	int CurrentPage;
};

enum DonateShopState
{
	STATE_DONTSHOW,
	STATE_SHOW,
};

struct PMSG_SEND_DONATEITEMS
{
	PSWMSG_HEAD header;
	int Count;
};

struct PMSG_DONATEITEM
{
	int ItemIndex;
	int Level;
	int Grade;
	int AncOption;
	int OptionEx;
	int Type;
	int BuyPrice;
	int SellPrice;
};

class cDonateShop
{
public:
	void GCDonateShopInfo(int aIndex);
	void GCSendState(int State, int aIndex);
	void CGPageRecv(PMSG_DONATESHOP_PAGE_RECV* lpMsg, int aIndex);
	void GCSendCurrentPage(int Page, int aIndex);
	void GCSendDonateItems(int aIndex);
	void GCSendClearItems(int aIndex);
}; extern cDonateShop gDonateShop;