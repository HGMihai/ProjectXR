// ShopManager.h: interface for the CShopManager class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "Shop.h"

#define MAX_SHOP 150

#define SHOP_RANGE(x) (((x)<0)?0:((x)>=MAX_SHOP)?0:1)

struct SHOP_MANAGER_INFO
{
	int Index;
	int MonsterClass;
	#if (GAMESERVER_UPDATE == 603 || GAMESERVER_UPDATE == 601)
	int DonateShopIndex;
	#endif
	CShop Shop;
};

class CShopManager
{
public:
	CShopManager();
	virtual ~CShopManager();
	void Load(char* path);
	void LoadShop();
	void LoadDonateShop();
	void ReloadShopInterface();
	long GetShopNumber(int MonsterClass);
	long GetClassByShopNumber(int ShopNumber);
	#if (GAMESERVER_UPDATE == 603)
	long GetShopNumberByDonateShopIndex(int MonsterClass, int DonateShopIndex);
	long IsDonateShop(int MonsterClass);
	long GetDonateShopCount(int MonsterClass);
	#endif
	bool GetItemByIndex(int index,CItem* lpItem,int slot);
	int GetPriceByItem(CItem* lpItem, int *type);
	bool GetItemByMonsterClass(int MonsterClass,CItem* lpItem,int slot);
	long GetItemCountByIndex(int index);
	long GetItemCountByMonsterClass(int MonsterClass);
	bool GCShopItemListSendByIndex(int index,int aIndex);
	bool GCShopItemListSendByMonsterClass(int MonsterClass,int aIndex);
private:
	std::map<int,SHOP_MANAGER_INFO> m_ShopManagerInfo;
};

extern CShopManager gShopManager;
