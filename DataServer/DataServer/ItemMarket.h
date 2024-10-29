#pragma once

#include "DataServerProtocol.h"

#define DEV_PLAYERSHOP			1

/*
Фильтр:
	- имя
	- опции
	- левел
	- анц
	- валюта

рынок должен отображать картинку предмета при наведении и ее точные опции:
рынок должен показывать продавца предмета
рынок должен позволять продавать в нескольких валютах
*/

/* => NEED DEV:
	+ катогории предметов
	+ заточку
	+ продавца предмета (NICK)
*/

/* => MY NEXT DEV:
	+ BUY Wcoin
	+ Buy Credit
	> Recode filter price item
	+ client check buy price
	+ all item status  one cmd
	+ scroll limit
	- sell item online add money
	- buy item active button buy
	- scroll interface
	- date in db
	+ rename db colmn
*/

#if(DEV_PLAYERSHOP)

#define MARKET_ITEM_BUFFER		16
#define MARKET_ITEM_MAX			100
#define MARKET_NAME_LEN			11

#define MARKET_DEBUG			1
#define MARKET_PRICE_DEV		1
#define MARKET_FILTER_DEV		1
#define MARKET_NAME_DEV			1
#define MARKET_PAGE_DEV			1
#define MARKET_ITEM_RECV		1

enum
{
	eMarketPriceTypeZen = 0,
	eMarketPriceTypeWcoin = 1,
	eMarketPriceTypeCredit = 2,
};

struct MARKET_DATA
{
	int ID;
	char Name[MARKET_NAME_LEN];
	BYTE Item[MARKET_ITEM_BUFFER];
	int PriceType;
	int Price;
};

#if(MARKET_FILTER_DEV)
struct MARKET_FILTER
{
	int FilterType;
	int FilterLevel;
	int FilterLuck;
	int FilterExl;
	int FilterAnc;
};
#endif

// -> DATA SERVER PROTOCOL

struct SDHP_REQ_MARKET_ITEM
{
	PSBMSG_HEAD h;
	int Result;
	int aIndex;
	int PriceType;
#if(MARKET_FILTER_DEV)
	MARKET_FILTER Filter;
#endif
#if(MARKET_PAGE_DEV)
	int PageNumber;
#endif
};

struct SDHP_ANS_MARKET_ITEM
{
	PSWMSG_HEAD h;
	int Result;
	WORD count;
	int aIndex;
	MARKET_DATA Data[MARKET_ITEM_MAX];
};

struct SDHP_REQ_MARKET_SELL
{
	PSBMSG_HEAD h;
	int Result;
	int aIndex;
	char Account[11];
	char Name[11];
	int PriceType;
	int Price;
	int ItemPos;
	BYTE ItemData[MARKET_ITEM_BUFFER];
#if(MARKET_FILTER_DEV)
	MARKET_FILTER Filter;
#endif
};

struct SDHP_ANS_MARKET_SELL
{
	PSBMSG_HEAD h;
	int Result;
	int aIndex;
	int ItemPos;
};

struct SDHP_REQ_MARKET_BUY
{
	PSBMSG_HEAD h;
	int Result;
	int aIndex;
	int ID;
};

struct SDHP_ANS_MARKET_BUY
{
	PSBMSG_HEAD h;
	int Result;
	int aIndex;
	int ID;
};

#if(MARKET_PRICE_DEV)
struct SDHP_REQ_MARKET_STATUS
{
	PSBMSG_HEAD h;
	int Result;
	int aIndex;
	char Account[11];
};

struct SDHP_ANS_MARKET_STATUS
{
	PSBMSG_HEAD h;
	int Result;
	int aIndex;
	char Account[11];
	int PriceType;
	int PriceValue;
};
#endif

#if(MARKET_ITEM_RECV)
struct SDHP_REQ_MARKET_ITEMRECV
{
	PSBMSG_HEAD h;
	int Result;
	int aIndex;
	int ID;
	BYTE Item[MARKET_ITEM_BUFFER];
};

struct SDHP_ANS_MARKET_ITEMRECV
{
	PSBMSG_HEAD h;
	int Result;
	int ID;
};
#endif

class CItemMarket
{
public:
	CItemMarket();
	~CItemMarket();

	//bool Connect();
	//void CreateTable();
	void Protocol(BYTE protoNum, BYTE* aRecv, int uIndex);

	void GDReqItemList(SDHP_REQ_MARKET_ITEM* lpMsg, int uIndex);
	void GDReqItemSell(SDHP_REQ_MARKET_SELL* lpMsg, int uIndex);
	void GDReqBuyItem(SDHP_REQ_MARKET_BUY* lpMsg, int uIndex);
#if(MARKET_PRICE_DEV)
	void GDReqItemStatus(SDHP_REQ_MARKET_STATUS* lpMsg, int uIndex);
#endif
#if(MARKET_PRICE_DEV)
	void GDReqItemListPage(SDHP_REQ_MARKET_ITEM* lpMsg, int uIndex);
#endif
#if(MARKET_ITEM_RECV)
	static CTime StrToCTime(const char* str);
	void RunItemDate();
	void GDReqItemDate(int uIndex, char* Account, int aIndex);
	void GDAnsItemDate(SDHP_ANS_MARKET_ITEMRECV* lpMsg, int uIndex);
#endif
};

extern CItemMarket g_ItemMarket;

#endif
