#pragma once

#include "User.h"
#include "Protocol.h"
#include "Warehouse.h"

#define MAX_GUILD_WAREHOUSE_MONEY 10000000000000

struct SDHP_GUILDWAREHOUSE_ITEM_SEND
{
	PSBMSG_HEAD header; // C1:05:10
	WORD index;
	char guild[11];
	//UINT WarehouseNumber;
};

struct SDHP_GUILDWAREHOUSE_ITEM_RECV
{
	PSWMSG_HEAD header; // C2:05:10
	WORD index;
	char guild[11];
	BYTE WarehouseItem[WAREHOUSE_SIZE][ITEM_BYTES_SIZE];
	QWORD Money;
};

struct SDHP_GUILDWAREHOUSE_ITEM_SAVE_SEND
{
	PSWMSG_HEAD header; // C2:05:15
	WORD index;
	char guild[11];
	BYTE WarehouseItem[WAREHOUSE_SIZE][ITEM_BYTES_SIZE];
	QWORD Money;
};

struct SDHP_EXTWAREHOUSE_STATE_SEND
{
	PSBMSG_HEAD header;
	int state;
	QWORD money;
	bool warehouseState;
};

struct GD_GET_WARE_SATUS_SEND
{
	PSBMSG_HEAD header;
	int aIndex;
	char GuildName[9];
};

struct PMSG_WARE_SATUS_RECV
{
	PSWMSG_HEAD header;
	int aIndex;
	BYTE count;
};

struct PMSG_WARE_SATUS
{
	char Name[11];
	BYTE GuildStatus;
	BYTE Status;
};

struct PMSG_STATUS_CHECK_SEND
{
	PSBMSG_HEAD header;
	int aIndex;
	char Name[11];
	char Account[11];
};

struct PMSG_STATUS_CHECK_ANS
{
	PSBMSG_HEAD header;
	int aIndex;
	BYTE Status;
	char Account[11];
};

class cGuildWarehouse
{
public:
	cGuildWarehouse();
	virtual ~cGuildWarehouse();
	void Load(char *path);
	bool Dialog(LPOBJ lpObj, LPOBJ lpNpc);
	void GDGuildWarehouseItemSend(int aIndex,char* guild);
	void GCGuildWarehouseListSend(LPOBJ lpObj);
	void DGGuildWarehouseItemRecv(SDHP_GUILDWAREHOUSE_ITEM_RECV* lpMsg);
	void GDGuildWarehouseItemSaveSend(int aIndex);
	void GCExtWarehouseStateSend(int aIndex,int state, bool warehouseState);
	void CGGuildWarehouseMoneyRecv(PMSG_WAREHOUSE_MONEY_RECV* lpMsg,int aIndex);
	void CGGetUsersWareStatus(int aIndex);
	void DGUsersWareStatusRecv(PMSG_WARE_SATUS_RECV* lpMsg, int size);
	void CGUsersWareStatusSave(PMSG_WARE_SATUS_RECV* lpMsg,int aIndex, int size);
	void GDCheckAllowWarehouse(int aIndex);
	void DGCheckAllowWarehouseRecv(PMSG_STATUS_CHECK_ANS* lpMsg);
private:
	int m_NpcMap;
	int m_NpcId;
	int m_NpcX;
	int m_NpcY;
	//----
}; extern cGuildWarehouse gGuildWarehouse;