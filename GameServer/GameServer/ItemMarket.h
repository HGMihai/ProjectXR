#pragma once

#define DEV_PLAYERSHOP	1

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
#define MARKET_PLAYER_BUY		1

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

struct MARKET_INFO
{
	int ID;
	char Name[MARKET_NAME_LEN];
	short ItemType;
	float ItemDurability;
	BYTE ItemData[MARKET_ITEM_BUFFER];
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

// -> GAME SERVER PROTOCOL

struct PMSG_REQ_MARKET_ITEM
{
	PSBMSG_HEAD h;
	int Result;
	int PriceType;
#if(MARKET_FILTER_DEV)
	MARKET_FILTER Filter;
#endif
#if(MARKET_PAGE_DEV)
	int PageNumber;
#endif
};

struct PMSG_ANS_MARKET_ITEM
{
	PSWMSG_HEAD h;
	int Result;
	MARKET_INFO Info[MARKET_ITEM_MAX];
};

struct PMSG_REQ_MARKET_SELL
{
	PSBMSG_HEAD h;
	int Result;
	int ItemPos;
	int ItemPriceType;
	int ItemPrice;
};

struct PMSG_ANS_MARKET_SELL
{
	PSBMSG_HEAD h;
	int Result;
};

struct PMSG_REQ_MARKET_BUY
{
	PSBMSG_HEAD h;
	int Result;
	int ID;
};

struct PMSG_ANS_MARKET_BUY
{
	PSBMSG_HEAD h;
	int Result;
};

// ----

class CItemMarket
{
public:
	CItemMarket();
	~CItemMarket();

	void Init();
	void Load();

	void Protocol(BYTE protoNum, BYTE* aRecv);

	void CGReqItemList(PMSG_REQ_MARKET_ITEM* lpMsg, int aIndex);
	void GDReqItemList(int aIndex, int NumPage);
	void DGAnsItemList(SDHP_ANS_MARKET_ITEM* lpMsg);

	void CGReqItemSell(PMSG_REQ_MARKET_SELL* lpMsg, int aIndex);
	void GDReqItemSell(int aIndex, int ItemPos, int ItemPriceType, int ItemPrice);
	void DGAnsItemSell(SDHP_ANS_MARKET_SELL* lpMsg);

	void CGReqItemBuy(PMSG_REQ_MARKET_BUY* lpMsg, int aIndex);
	void GDReqItemBuy(int aIndex, int ID);
	void DGAnsItemBuy(SDHP_ANS_MARKET_BUY* lpMsg);

#if(MARKET_PRICE_DEV)
	void GDReqItemStatus(int aIndex);
	void DGAnsItemStatus(SDHP_ANS_MARKET_STATUS* lpMsg);
#endif

#if(MARKET_ITEM_RECV)
	void GDReqItemDate(SDHP_REQ_MARKET_ITEMRECV* lpMsg);
#endif

	//CItem GetByteToItem(LPBYTE lpByte);
	void ConnectPlayer(int aIndex);
	bool GetCheckMoney(int aIndex, int PriceType, int PriceValue);
	bool PlayerMoney(int aIndex, int PriceType, int PriceValue);	//rename

	MARKET_DATA m_MarketData[MAX_OBJECT][MARKET_ITEM_MAX];
};

extern CItemMarket g_ItemMarket;

#endif
