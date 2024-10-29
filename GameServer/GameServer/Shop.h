// Shop.h: interface for the CShop class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "Item.h"
#include "ItemManager.h"
#include "Protocol.h"

#define SHOP_SIZE 120

#define SHOP_INVENTORY_RANGE(x) (((x)<0)?0:((x)>=SHOP_SIZE)?0:1)

//**********************************************//
//************ GameServer -> Client ************//
//**********************************************//

struct PMSG_SHOP_ITEM_LIST_SEND
{
	PWMSG_HEAD header; // C2:31
	BYTE type; // 4
	BYTE count; // 5
};

struct PMSG_SHOP_ITEM_LIST
{
	BYTE slot;
	BYTE ItemInfo[MAX_ITEM_INFO];
};

//**********************************************//
//**********************************************//
//**********************************************//

struct DONATE_SHOP_ITEMS
{
	int itemIndex;
	int itemLevel;
	int itemDurability;
	int itemOption1;
	int itemOption2;
	int itemOption3;
	int itemNewOption;
	int itemAncient;
	int itemOptionEx;
	int itemSocketCount;
	int ItemPriceType;
	int itemBuyValue;
	int itemSellValue;
	
};

class CShop
{
public:
	CShop();
	virtual ~CShop();
	void Init();
	void Load(char* path);
	void LoadDonateShop(char* path, int DonateShopIndex);
	void ShopItemSet(int slot,BYTE type);
	BYTE ShopRectCheck(int x,int y,int width,int height);
	void InsertItem(int ItemIndex,int ItemLevel,int ItemDurability,int ItemOption1,int ItemOption2,int ItemOption3,int ItemNewOption,int AncOption,int ItemOptionEx,int ItemValue);
	void InsertDonateItem(DONATE_SHOP_ITEMS* lpInfo/*int ItemIndex,int ItemLevel,int ItemDurability,int ItemOption1,int ItemOption2,int ItemOption3,int ItemNewOption,int AncOption,int ItemOptionEx,int PriceType,int BuyValue,int SellValue*/);
	bool GetItem(CItem* lpItem,int slot);
	int GetItemPrice(CItem* lpItem, int* type);
	long GetItemCount();
	bool GCShopItemListSend(int aIndex);
private:
	CItem m_Item[SHOP_SIZE];
	BYTE m_InventoryMap[SHOP_SIZE];
	BYTE ShopIndex;
};
