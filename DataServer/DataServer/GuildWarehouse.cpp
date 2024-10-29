// GuildWarehouse.cpp: implementation of the cGuildWarehouse class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GuildWarehouse.h"
#include "QueryManager.h"
#include "SocketManager.h"
#include "Util.h"

cGuildWarehouse gGuildWarehouse;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

cGuildWarehouse::cGuildWarehouse() // OK
{

}

cGuildWarehouse::~cGuildWarehouse() // OK
{

}

void cGuildWarehouse::GDWarehouseItemRecv(SDHP_GUILDWAREHOUSE_ITEM_RECV* lpMsg,int index) // OK
{
	SDHP_GUILDWAREHOUSE_ITEM_SEND pMsg;

	pMsg.header.set(0x05,0x10,sizeof(pMsg));

	pMsg.index = lpMsg->index;

	memcpy(pMsg.guild,lpMsg->guild,sizeof(pMsg.guild));

	if (gQueryManager.ExecQuery("SELECT G_Name from Guild WHERE G_Name = '%s'",lpMsg->guild) == 0 || gQueryManager.Fetch() == SQL_NO_DATA)
	{
		gQueryManager.Close();
	}
	else
	{
		gQueryManager.Close();

		if (gQueryManager.ExecQuery("SELECT Warehouse, Money from Guild WHERE G_Name = '%s'",lpMsg->guild) == 0 || gQueryManager.Fetch() == SQL_NO_DATA)
		{
			gQueryManager.Close();

			memset(pMsg.WarehouseItem,0xFF,sizeof(pMsg.WarehouseItem));

			pMsg.Money = 0;
		}
		else
		{
			pMsg.Money = gQueryManager.GetAsInteger64("Money");

			gQueryManager.GetAsBinary("Warehouse",pMsg.WarehouseItem[0],sizeof(pMsg.WarehouseItem));

			gQueryManager.Close();
		}
	}

	gSocketManager.DataSend(index,(BYTE*)&pMsg,sizeof(pMsg));
}

void cGuildWarehouse::GDWarehouseItemSaveRecv(SDHP_GUILDWAREHOUSE_ITEM_SAVE_RECV* lpMsg) // OK
{
	gQueryManager.BindParameterAsBinary(1,lpMsg->WarehouseItem[0],sizeof(lpMsg->WarehouseItem));

	gQueryManager.ExecQuery("UPDATE Guild SET Warehouse=?, Money=%I64d WHERE G_Name='%s'",lpMsg->Money,lpMsg->guild);
	gQueryManager.Close();
}

void cGuildWarehouse::GDWareStatusRecv(GD_GET_WARE_SATUS_RECV* lpMsg, int index)
{
	if(gQueryManager.ExecQuery("SELECT * FROM GuildMember where G_Name = '%s'",lpMsg->GuildName) != 0)
	{
		BYTE send[8192];

		PMSG_WARE_SATUS_SEND pMsg;

		pMsg.header.set(0xFB,0x02,0);

		int size = sizeof(pMsg);

		pMsg.aIndex = lpMsg->aIndex;

		pMsg.count = 0;

		PMSG_WARE_SATUS info;

		while (gQueryManager.Fetch() != SQL_NO_DATA)
		{
			gQueryManager.GetAsString("Name",info.Name,sizeof(info.Name));

			info.GuildStatus = gQueryManager.GetAsInteger("G_Status");

			info.Status = gQueryManager.GetAsInteger("G_WareStatus");

			memcpy(&send[size],&info,sizeof(info));
			size += sizeof(info);

			pMsg.count++;
		}

		pMsg.header.size[0] = SET_NUMBERHB(size);

		pMsg.header.size[1] = SET_NUMBERLB(size);

		memcpy(send,&pMsg,sizeof(pMsg));

		gSocketManager.DataSend(index,send,size);

		//DataSend(lpObj->Index,send,size);
	}
	gQueryManager.Close();
}

void cGuildWarehouse::GDUsersWareStatusSave(PMSG_WARE_SATUS_SEND* lpMsg)
{
	for(int n=0;n < lpMsg->count;n++)
	{
		PMSG_WARE_SATUS* lpInfo = (PMSG_WARE_SATUS*)(((BYTE*)lpMsg)+sizeof(PMSG_WARE_SATUS_SEND)+(sizeof(PMSG_WARE_SATUS)*n));

		gQueryManager.ExecQuery("UPDATE GuildMember SET G_WareStatus=%d WHERE Name='%s'",lpInfo->Status,lpInfo->Name);
		gQueryManager.Close();
	}
}

void cGuildWarehouse::GDCheckAllowWarehouse(PMSG_STATUS_CHECK_SEND* lpMsg, int index)
{
	//gQueryManager.ExecQuery("SELECT * from GuildMember where Name = '%s'",lpMsg->Name);

	PMSG_STATUS_CHECK_ANS pMsg;

	pMsg.header.set(0xFB,0x03,sizeof(pMsg));

	if (gQueryManager.ExecQuery("SELECT * from GuildMember where Name = '%s'",lpMsg->Name) == 0 || gQueryManager.Fetch() == SQL_NO_DATA)
	{
		gQueryManager.Close();

		pMsg.Status = 0;
	}
	else
	{
		pMsg.Status = gQueryManager.GetAsInteger64("G_WareStatus");

		gQueryManager.Close();
	}

	pMsg.aIndex = lpMsg->aIndex;
	memcpy(pMsg.Account,lpMsg->Account,sizeof(pMsg.Account));

	gSocketManager.DataSend(index,(BYTE*)&pMsg,sizeof(pMsg));
}
