#pragma once

#include "DataServerProtocol.h"

#if(DATASERVER_UPDATE>=602)
#define WAREHOUSE_SIZE 240
#else
#define WAREHOUSE_SIZE 120
#endif


//**********************************************//
//********** GameServer -> DataServer **********//
//**********************************************//

struct SDHP_GUILDWAREHOUSE_ITEM_RECV
{
	PSBMSG_HEAD header; // C1:05:00
	WORD index;
	char guild[11];
};

struct SDHP_GUILDWAREHOUSE_ITEM_SAVE_RECV
{
	PSWMSG_HEAD header; // C2:05:15
	WORD index;
	char guild[11];
	BYTE WarehouseItem[WAREHOUSE_SIZE][ITEM_BYTES_SIZE];
	QWORD Money;
};

struct GD_GET_WARE_SATUS_RECV
{
	PSBMSG_HEAD header;
	int aIndex;
	char GuildName[9];
};

//**********************************************//
//********** DataServer -> GameServer **********//
//**********************************************//

struct SDHP_GUILDWAREHOUSE_ITEM_SEND
{
	PSWMSG_HEAD header; // C2:05:00
	WORD index;
	char guild[11];
	BYTE WarehouseItem[WAREHOUSE_SIZE][ITEM_BYTES_SIZE];
	QWORD Money;
};

struct SDHP_GUILDWAREHOUSE_FREE_SEND
{
	PSBMSG_HEAD header; // C1:05:01
	WORD index;
	char account[11];
};

struct PMSG_WARE_SATUS_SEND
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

//**********************************************//
//**********************************************//
//**********************************************//

class cGuildWarehouse
{
public:
	cGuildWarehouse();
	virtual ~cGuildWarehouse();
	void GDWarehouseItemRecv(SDHP_GUILDWAREHOUSE_ITEM_RECV* lpMsg,int index);
	void GDWarehouseItemSaveRecv(SDHP_GUILDWAREHOUSE_ITEM_SAVE_RECV* lpMsg);
	void GDWareStatusRecv(GD_GET_WARE_SATUS_RECV* lpMsg, int index);
	void GDUsersWareStatusSave(PMSG_WARE_SATUS_SEND* lpMsg);
	void GDCheckAllowWarehouse(PMSG_STATUS_CHECK_SEND* lpMsg, int index);
};

extern cGuildWarehouse gGuildWarehouse;
