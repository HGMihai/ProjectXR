// ItemManager.h: interface for the CItemManager class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "DefaultClassInfo.h"
#include "Protocol.h"
#include "User.h"

#define MAX_ITEM_SECTION 16
#define MAX_ITEM_TYPE 512
#define MAX_ITEM (MAX_ITEM_SECTION*MAX_ITEM_TYPE)
#define MAX_ITEM_INFO 12

#define MAX_ITEM_LEVEL	15

#define GET_ITEM(x,y) (((x)*MAX_ITEM_TYPE)+(y))
#define CHECK_ITEM(x) (((x)<0)?-1:((x)>=MAX_ITEM)?-1:x)
#define INVENTORY_RANGE(x) (((x)<0)?0:((x)>=INVENTORY_SIZE)?0:1)
#if(GAMESERVER_UPDATE>=701)
#define INVENTORY_WEAR_RANGE(x) (((x)<0)?0:((x)>=INVENTORY_WEAR_SIZE)?(((x)==236)?1:0):1)
#define INVENTORY_FULL_RANGE(x) (((x)<0)?0:((x)>=INVENTORY_EXT4_SIZE)?(((x)==236)?1:0):1)
#else
#define INVENTORY_WEAR_RANGE(x) (((x)<0)?0:((x)>=INVENTORY_WEAR_SIZE)?0:1)
#define INVENTORY_FULL_RANGE(x) (((x)<0)?0:((x)>=INVENTORY_EXT4_SIZE)?0:1)
#endif
#define INVENTORY_BASE_RANGE(x) (((x)<INVENTORY_WEAR_SIZE)?0:((x)>=INVENTORY_SIZE)?0:1)
#define INVENTORY_MAIN_RANGE(x) (((x)<INVENTORY_WEAR_SIZE)?0:((x)>=INVENTORY_MAIN_SIZE)?0:1)
#define INVENTORY_EXT1_RANGE(x) (((x)<INVENTORY_MAIN_SIZE)?0:((x)>=INVENTORY_EXT1_SIZE)?0:1)
#define INVENTORY_EXT2_RANGE(x) (((x)<INVENTORY_EXT1_SIZE)?0:((x)>=INVENTORY_EXT2_SIZE)?0:1)
#define INVENTORY_EXT3_RANGE(x) (((x)<INVENTORY_EXT2_SIZE)?0:((x)>=INVENTORY_EXT3_SIZE)?0:1)
#define INVENTORY_EXT4_RANGE(x) (((x)<INVENTORY_EXT3_SIZE)?0:((x)>=INVENTORY_EXT4_SIZE)?0:1)
#define INVENTORY_SHOP_RANGE(x) (((x)<INVENTORY_EXT4_SIZE)?0:((x)>=INVENTORY_FULL_SIZE)?0:1)
#define TRADE_RANGE(x) (((x)<0)?0:((x)>=TRADE_SIZE)?0:1)
#define WAREHOUSE_RANGE(x) (((x)<0)?0:((x)>=WAREHOUSE_SIZE)?0:1)
#define WAREHOUSE_MAIN_RANGE(x) (((x)<0)?0:((x)>=WAREHOUSE_EXT1_SIZE)?0:1)
#define WAREHOUSE_EXT1_RANGE(x) (((x)<WAREHOUSE_EXT1_SIZE)?0:((x)>=WAREHOUSE_SIZE)?0:1)
#define CHAOS_BOX_RANGE(x) (((x)<0)?0:((x)>=CHAOS_BOX_SIZE)?0:1)

#define DBI_GET_TYPE(x) ( ((x)&((MAX_ITEM_SECTION-1)<<8))>>4 )
#define DBI_GET_INDEX(x) ( (x) & 0xFF )
#define DBI_GET_LEVEL(x)  ( ((x)>>3)& MAX_ITEM_LEVEL )
#define DBI_GET_SKILL(x)  ( (x) >> 7 )
#define DBI_GET_LUCK(x)  ( ((x) >> 2) & 0x01)
#define DBI_GET_OPTION(x)  ( ( (x) ) & 0x03 )
#define DBI_GET_OPTION16(x) ( (x) & 0x40  )
#define DBI_GET_DINO_OPTION(x)  ( DBI_GET_OPTION16((x)) >> 4 )
#define DBI_GET_NOPTION(x)  ( (x) & 0x3F  )
#define DBI_GET_380OPTION(x) ( ((x) & 0x08) << 4  )

//**********************************************//
//************ Client -> GameServer ************//
//**********************************************//

struct PMSG_ITEM_GET_RECV
{
	PBMSG_HEAD header; // C1:22
	BYTE index[2];
};

struct PMSG_ITEM_DROP_RECV
{
	PBMSG_HEAD header; // C1:23
	BYTE x;
	BYTE y;
	BYTE slot;
};

struct PMSG_ITEM_MOVE_RECV
{
	PBMSG_HEAD header; // C1:24
	BYTE SourceFlag;
	BYTE SourceSlot;
	BYTE ItemInfo[MAX_ITEM_INFO];
	BYTE TargetFlag;
	BYTE TargetSlot;
};

struct PMSG_ITEM_USE_RECV
{
	PBMSG_HEAD header; // C1:26
	BYTE SourceSlot;
	BYTE TargetSlot;
	BYTE type;
};

struct PMSG_ITEM_BUY_RECV
{
	PBMSG_HEAD header; // C1:32
	BYTE slot;
};

struct PMSG_ITEM_SELL_RECV
{
	PBMSG_HEAD header; // C1:33
	BYTE slot;
};

struct PMSG_ITEM_REPAIR_RECV
{
	PBMSG_HEAD header; // C1:34
	BYTE slot;
	BYTE type;
};

//**********************************************//
//************ GameServer -> Client ************//
//**********************************************//

struct PMSG_ITEM_GET_SEND
{
	PBMSG_HEAD header; // C3:22
	BYTE result;
	BYTE ItemInfo[MAX_ITEM_INFO];
};

struct PMSG_ITEM_DROP_SEND
{
	PBMSG_HEAD header; // C1:23
	BYTE result;
	BYTE slot;
};

struct PMSG_ITEM_MOVE_SEND
{
	PBMSG_HEAD header; // C3:24
	BYTE result;
	BYTE slot;
	BYTE ItemInfo[MAX_ITEM_INFO];
};

struct PMSG_ITEM_CHANGE_SEND
{
	PBMSG_HEAD header; // C1:25
	BYTE index[2];
	BYTE ItemInfo[MAX_ITEM_INFO];
	#if(GAMESERVER_UPDATE>=701)
	BYTE attribute;
	#endif
};

struct PMSG_ITEM_DELETE_SEND
{
	PBMSG_HEAD header; // C1:28
	BYTE slot;
	BYTE flag;
};

struct PMSG_ITEM_DUR_SEND
{
	PBMSG_HEAD header; // C1:2A
	BYTE slot;
	BYTE dur;
	BYTE flag;
};

struct PMSG_ITEM_BUY_SEND
{
	PBMSG_HEAD header; // C1:32
	BYTE result;
	BYTE ItemInfo[MAX_ITEM_INFO];
};

struct PMSG_ITEM_SELL_SEND
{
	PBMSG_HEAD header; // C1:33
	BYTE result;
	DWORD money;
};

struct PMSG_ITEM_REPAIR_SEND
{
	PBMSG_HEAD header; // C1:34
	DWORD money;
};

struct PMSG_ITEM_LIST_SEND
{
	PSWMSG_HEAD header; // C4:F3:10
	BYTE count;
};

struct PMSG_ITEM_LIST
{
	BYTE slot;
	BYTE ItemInfo[MAX_ITEM_INFO];
};

struct PMSG_ITEM_EQUIPMENT_SEND
{
	PSBMSG_HEAD header; // C1:F3:13
	BYTE index[2];
	BYTE CharSet[18];
};

struct PMSG_ITEM_MODIFY_SEND
{
	PSBMSG_HEAD header; // C1:F3:14
	BYTE slot;
	BYTE ItemInfo[MAX_ITEM_INFO];
};

struct PMSG_MOVEITEM {
	PSBMSG_HEAD h;
	BYTE sFlag;
	BYTE tFlag;
	BYTE Source;
	BYTE Target;
};

struct PMSG_SET_CHAOSBOX_STATE
{
	PSBMSG_HEAD header; // C1:32
	BYTE state;
};

//**********************************************//
//**********************************************//
//**********************************************//

struct ITEM_INFO
{
	int Index;
	int Slot;
	int Skill;
	int Width;
	int Height;
	int HaveSerial;
	int HaveOption;
	int DropItem;
	chr Name[32];
	int Level;
	int DamageMin;
	int DamageMax;
	int MagicDamageRate;
	int TwoHand;
	int Defense;
	int MagicDefense;
	int DefenseSuccessRate;
	int AttackSpeed;
	int WalkSpeed;
	int Durability;
	int MagicDurability;
	int Value;
	int BuyMoney;
	int Resistance[MAX_RESISTANCE_TYPE];
	int RequireLevel;
	int RequireStrength;
	int RequireDexterity;
	int RequireEnergy;
	int RequireVitality;
	int RequireLeadership;
	int RequireClass[MAX_CLASS];
};

class CItemManager
{
public:
	CItemManager();
	virtual ~CItemManager();
	void Load(char* path);
	bool IsItemExist(int index);
	bool GetInfo(int index,ITEM_INFO* lpInfo);
	ITEM_INFO* GetInfo(int index);
	int GetItemSkill(int index,int isExcl);
	int GetItemTwoHand(int index);
	int GetItemHaveSerial(int index);
	int GetItemDurability(int index,int level,int NewOption,int SetOption);
	int GetItemRepairMoney(CItem* lpItem,int type);
	int GetInventoryItemSlot(LPOBJ lpObj,int index,int level);
	int GetInventoryItemCount(LPOBJ lpObj,int index,int level);
	int GetInventoryEmptySlotCount(LPOBJ lpObj);
	int GetInventoryMaxValue(LPOBJ lpObj);
	bool CheckItemRequireLevel(LPOBJ lpObj,CItem* lpItem);
	bool CheckItemRequireStrength(LPOBJ lpObj,CItem* lpItem);
	bool CheckItemRequireDexterity(LPOBJ lpObj,CItem* lpItem);
	bool CheckItemRequireVitality(LPOBJ lpObj,CItem* lpItem);
	bool CheckItemRequireEnergy(LPOBJ lpObj,CItem* lpItem);
	bool CheckItemRequireLeadership(LPOBJ lpObj,CItem* lpItem);
	bool CheckItemRequireClass(LPOBJ lpObj,int index);
	bool CheckItemMoveToInventory(LPOBJ lpObj,CItem* lpItem,int slot);
	bool CheckItemMoveToTrade(LPOBJ lpObj,CItem* lpItem,BYTE TargetFlag);
	bool CheckItemMoveToVault(LPOBJ lpObj,CItem* lpItem,BYTE TargetFlag);
	bool CheckItemMoveToChaos(LPOBJ lpObj,CItem* lpItem,BYTE TargetFlag);
	bool CheckItemMoveToBlock(LPOBJ lpObj,CItem* lpItem);
	bool CheckItemInventorySpace(LPOBJ lpObj,int index);
	bool CheckItemInventorySpace(LPOBJ lpObj,int width,int height);
	void InventoryItemSet(int aIndex,int slot,BYTE type);
	BYTE InventoryRectCheck(int aIndex,int x,int y,int width,int height);
	BYTE InventoryInsertItem(int aIndex,CItem item);
	BYTE InventoryAddItem(int aIndex,CItem item,int slot);
	void InventoryDelItem(int aIndex,int slot);
	bool InventoryInsertItemStack(LPOBJ lpObj,CItem* lpItem);
	bool InventoryAddItemStack(LPOBJ lpObj,int SourceSlot,int TargetSlot);
	BYTE InventoryCheckInsertItem(int aIndex, WORD item);
	void TradeItemSet(int aIndex,int slot,BYTE type) ;
	BYTE TradeRectCheck(int aIndex,int x,int y,int width,int height);
	BYTE TradeInsertItem(int aIndex,CItem item);
	BYTE TradeAddItem(int aIndex,CItem item,int slot);
	void TradeDelItem(int aIndex,int slot);
	void WarehouseItemSet(int aIndex,int slot,BYTE type);
	BYTE WarehouseRectCheck(int aIndex,int x,int y,int width,int height);
	BYTE WarehouseInsertItem(int aIndex,CItem item);
	BYTE WarehouseAddItem(int aIndex,CItem item,int slot);
	void WarehouseDelItem(int aIndex,int slot);
	void GuildWarehouseItemSet(int aIndex,int slot,BYTE type);
	BYTE GuildWarehouseRectCheck(int aIndex,int x,int y,int width,int height);
	BYTE GuildWarehouseAddItem(int aIndex,CItem item,int slot);
	void GuildWarehouseDelItem(int aIndex,int slot);
	void ChaosBoxItemSet(int aIndex,int slot,BYTE type);
	BYTE ChaosBoxRectCheck(int aIndex,int x,int y,int width,int height);
	BYTE ChaosBoxInsertItem(int aIndex,CItem item);
	BYTE ChaosBoxFindPlace(int aIndex,CItem item);
	BYTE ChaosBoxAddItem(int aIndex,CItem item,int slot);
	void ChaosBoxDelItem(int aIndex,int slot);
	void ItemByteConvert(BYTE* lpMsg,CItem item);
	void DBItemByteConvert(BYTE* lpMsg,CItem* lpItem);
	bool ConvertItemByte(CItem* lpItem,BYTE* lpMsg);
	bool IsValidItem(LPOBJ lpObj,CItem* lpItem);
	void UpdateInventoryViewport(int aIndex,int slot);
	void DeleteInventoryItemCount(LPOBJ lpObj,int index,int level,int count);
	void DecreaseItemDur(LPOBJ lpObj,int slot,int dur);
	int RepairItem(LPOBJ lpObj,CItem* lpItem,int slot,int type);
	BYTE MoveItemToInventoryFromInventory(LPOBJ lpObj,BYTE SourceSlot,BYTE TargetSlot,BYTE TargetFlag);
	BYTE MoveItemToInventoryFromTrade(LPOBJ lpObj,BYTE SourceSlot,BYTE TargetSlot,BYTE TargetFlag);
	BYTE MoveItemToInventoryFromWarehouse(LPOBJ lpObj,BYTE SourceSlot,BYTE TargetSlot,BYTE TargetFlag);
	BYTE MoveItemToInventoryFromChaosBox(LPOBJ lpObj,BYTE SourceSlot,BYTE TargetSlot,BYTE TargetFlag);
	BYTE MoveItemToInventoryFromPersonalShop(LPOBJ lpObj,BYTE SourceSlot,BYTE TargetSlot,BYTE TargetFlag);
	BYTE MoveItemToTradeFromInventory(LPOBJ lpObj,BYTE SourceSlot,BYTE TargetSlot,BYTE TargetFlag);
	BYTE MoveItemToTradeFromTrade(LPOBJ lpObj,BYTE SourceSlot,BYTE TargetSlot,BYTE TargetFlag);
	BYTE MoveItemToTradeFromEventInventory(LPOBJ lpObj,BYTE SourceSlot,BYTE TargetSlot,BYTE TargetFlag);
	BYTE MoveItemToWarehouseFromInventory(LPOBJ lpObj,BYTE SourceSlot,BYTE TargetSlot,BYTE TargetFlag);
	BYTE MoveItemToWarehouseFromWarehouse(LPOBJ lpObj,BYTE SourceSlot,BYTE TargetSlot,BYTE TargetFlag);
	BYTE MoveItemToChaosBoxFromInventory(LPOBJ lpObj,BYTE SourceSlot,BYTE TargetSlot,BYTE TargetFlag);
	BYTE MoveItemToChaosBoxFromChaosBox(LPOBJ lpObj,BYTE SourceSlot,BYTE TargetSlot,BYTE TargetFlag);
	BYTE MoveItemToPersonalShopFromInventory(LPOBJ lpObj,BYTE SourceSlot,BYTE TargetSlot,BYTE TargetFlag);
	BYTE MoveItemToPersonalShopFromPersonalShop(LPOBJ lpObj,BYTE SourceSlot,BYTE TargetSlot,BYTE TargetFlag);
	BYTE MoveItemToEventInventoryFromTrade(LPOBJ lpObj,BYTE SourceSlot,BYTE TargetSlot,BYTE TargetFlag);
	BYTE MoveItemToEventInventoryFromEventInventory(LPOBJ lpObj,BYTE SourceSlot,BYTE TargetSlot,BYTE TargetFlag);
	BYTE MoveItemToMuunInventoryFromMuunInventory(LPOBJ lpObj,BYTE SourceSlot,BYTE TargetSlot,BYTE TargetFlag);
	void CGItemGetRecv(PMSG_ITEM_GET_RECV* lpMsg,int aIndex);
	void CGItemDropRecv(PMSG_ITEM_DROP_RECV* lpMsg,int aIndex);
	void CGItemMoveRecv(PMSG_ITEM_MOVE_RECV* lpMsg,int aIndex);
	void CGItemUseRecv(PMSG_ITEM_USE_RECV* lpMsg,int aIndex);
	void CGItemBuyRecv(PMSG_ITEM_BUY_RECV* lpMsg,int aIndex);
	static void CGItemBuyRecvCallback(LPOBJ lpObj,BYTE slot,DWORD null,DWORD WCoinC,DWORD WCoinP,DWORD GoblinPoint);
	void CGItemSellRecv(PMSG_ITEM_SELL_RECV* lpMsg,int aIndex);
	void CGItemRepairRecv(PMSG_ITEM_REPAIR_RECV* lpMsg,int aIndex);
	void GCItemMoveSend(int aIndex,BYTE result,BYTE slot,BYTE* ItemInfo);
	void GCItemChangeSend(int aIndex,BYTE slot);
	void GCItemDeleteSend(int aIndex,BYTE slot,BYTE flag);
	void GCItemDurSend(int aIndex,BYTE slot,BYTE dur,BYTE flag);
	void GCItemListSend(int aIndex);
	void GCItemEquipmentSend(int aIndex);
	void GCItemModifySend(int aIndex,BYTE slot);
	void CGMoveItemProc(PMSG_MOVEITEM* aRecv, short aIndex);
	// ----
	bool ChaosBoxHasItem(LPOBJ lpObj);
	bool TradeHasItem(LPOBJ lpObj);
private:
	std::map<int,ITEM_INFO> m_ItemInfo;
};

extern CItemManager gItemManager;
