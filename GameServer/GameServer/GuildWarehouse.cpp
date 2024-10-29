#include "stdafx.h"
#include "GuildWarehouse.h"
#include "GameMain.h"
#include "Guild.h"
#include "ItemManager.h"
#include "NpcTalk.h"
#include "Util.h"
#include "DSProtocol.h"
#include "Shop.h"
#include "Warehouse.h"
#include "Notice.h"
#include "GuildClass.h"
#include "Protection.h"


cGuildWarehouse gGuildWarehouse;

cGuildWarehouse::cGuildWarehouse()
{
	this->m_NpcMap = -1;
	this->m_NpcId = 0;
	this->m_NpcX = -1;
	this->m_NpcY = -1;
}

cGuildWarehouse::~cGuildWarehouse()
{
	
}

void cGuildWarehouse::Load(char* path)
{
	if (!gProtection.GetCustomState(CUSTOM_GUILD_WAREHOUSE))
	{
		return;
	}

	this->m_NpcMap = GetPrivateProfileInt("NPC", "Map", 0, path);
	this->m_NpcId = GetPrivateProfileInt("NPC", "NpcIndex", 0, path);
	this->m_NpcX = GetPrivateProfileInt("NPC", "X", 0, path);
	this->m_NpcY = GetPrivateProfileInt("NPC", "Y", 0, path);
}

bool cGuildWarehouse::Dialog(LPOBJ lpObj, LPOBJ lpNpc)
{
	if (!gProtection.GetCustomState(CUSTOM_GUILD_WAREHOUSE))
	{
		return false;
	}

	if(		lpNpc->Class		== this->m_NpcId 
		&&	lpNpc->Map			== this->m_NpcMap
		&&	lpNpc->X			== this->m_NpcX
		&&	lpNpc->Y			== this->m_NpcY )
	{
		if (lpObj->GuildNumber == 0)
		{
			return true;
		}

		if (lpObj->Guild->WarehouseInUse)
		{
			gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,"Guild Warehouse already in use by %s!",gObj[lpObj->Guild->WarehouseUserIndex].Name);
			return true;
		}

		this->GDCheckAllowWarehouse(lpObj->Index);

		return true;
	}
	// ----
	return false;
}

void cGuildWarehouse::GDGuildWarehouseItemSend(int aIndex,char* guild) // OK
{
	if(gObjIsAccountValid(aIndex,gObj[aIndex].Account) == 0)
	{
		return;
	}

	if(gObj[aIndex].LoadGuildWarehouse != 0)
	{
		return;
	}

	gObj[aIndex].Interface.use = 1;
	gObj[aIndex].Interface.type = INTERFACE_WAREHOUSE;
	gObj[aIndex].Interface.state = 0;
	//gObj[aIndex].WarehouseNumber = 0;

	SDHP_GUILDWAREHOUSE_ITEM_SEND pMsg;

	pMsg.header.set(0x05,0x10,sizeof(pMsg));

	pMsg.index = aIndex;

	memcpy(pMsg.guild,guild,sizeof(pMsg.guild));

	gDataServerConnection.DataSend((BYTE*)&pMsg,pMsg.header.size);
}

void cGuildWarehouse::GCGuildWarehouseListSend(LPOBJ lpObj) // OK
{
	BYTE send[4096];

	PMSG_SHOP_ITEM_LIST_SEND pMsg;

	pMsg.header.set(0x31,0);

	int size = sizeof(pMsg);

	pMsg.type = 0;

	pMsg.count = 0;

	PMSG_SHOP_ITEM_LIST info;

	for(int n=0;n < WAREHOUSE_SIZE;n++)
	{
		if(lpObj->GuildWarehouse[n].IsItem() == 0)
		{
			continue;
		}

		info.slot = n;

		gItemManager.ItemByteConvert(info.ItemInfo,lpObj->GuildWarehouse[n]);

		memcpy(&send[size],&info,sizeof(info));
		size += sizeof(info);

		pMsg.count++;
	}

	pMsg.header.size[0] = SET_NUMBERHB(size);
	pMsg.header.size[1] = SET_NUMBERLB(size);

	memcpy(send,&pMsg,sizeof(pMsg));

	DataSend(lpObj->Index,send,size);

	gWarehouse.GCWarehouseMoneySend(lpObj->Index,1,lpObj->Money,0);
}

void cGuildWarehouse::DGGuildWarehouseItemRecv(SDHP_GUILDWAREHOUSE_ITEM_RECV* lpMsg)
{
	/*if(gObjIsAccountValid(lpMsg->index,lpMsg->account) == 0)
	{
		LogAdd(LOG_RED,"[DGWarehouseItemRecv] Invalid Account [%d](%s)",lpMsg->index,lpMsg->account);
		CloseClient(lpMsg->index);
		return;
	}*/

	LPOBJ lpObj = &gObj[lpMsg->index];

	if (lpObj->GuildNumber == 0)
	{
		return;
	}

	if(lpObj->Interface.use == 0 || lpObj->Interface.type != INTERFACE_WAREHOUSE || lpObj->Interface.state != 0)
	{
		return;
	}

	if(lpObj->LoadGuildWarehouse != 0)
	{
		return;
	}

	memset(lpObj->GuildWarehouseMap,0xFF,WAREHOUSE_SIZE);

	for(int n=0;n < WAREHOUSE_SIZE;n++)
	{
		CItem item;

		lpObj->GuildWarehouse[n].Clear();

		if(gItemManager.ConvertItemByte(&item,lpMsg->WarehouseItem[n]) != 0){gItemManager.GuildWarehouseAddItem(lpObj->Index,item,n);}
	}

	lpObj->GuildWarehouseMoney = lpMsg->Money;

	//lpObj->WarehouseMoney = 0;

	//lpObj->WarehousePW = 0;

	PMSG_NPC_TALK_SEND pMsg;

	pMsg.header.setE(0x30,sizeof(pMsg));

	pMsg.result = 2;

	DataSend(lpObj->Index,(BYTE*)&pMsg,pMsg.header.size);

	this->GCExtWarehouseStateSend(lpObj->Index,1,true);

	this->GCGuildWarehouseListSend(lpObj);

	gWarehouse.GCWarehouseStateSend(lpObj->Index,/*((lpObj->WarehousePW>0)?((lpObj->WarehouseLock==0)?12:1):0)*/0);

	lpObj->Interface.use = 1;

	lpObj->Interface.type = INTERFACE_WAREHOUSE;

	lpObj->Interface.state = 1;

	lpObj->LoadGuildWarehouse = 1;

	lpObj->Guild->WarehouseInUse = 1;

	lpObj->Guild->WarehouseUserIndex = lpObj->Index;

	GDPetItemInfoSend(lpObj->Index,1);
}

void cGuildWarehouse::GDGuildWarehouseItemSaveSend(int aIndex) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	if(lpObj->LoadGuildWarehouse == 0)
	{
		return;
	}

	this->GCExtWarehouseStateSend(lpObj->Index,lpObj->ExtWarehouse,false);

	SDHP_GUILDWAREHOUSE_ITEM_SAVE_SEND pMsg;

	pMsg.header.set(0x05,0x15,sizeof(pMsg));

	pMsg.index = aIndex;

	memcpy(pMsg.guild,lpObj->GuildName,sizeof(pMsg.guild));

	for(int n=0;n < WAREHOUSE_SIZE;n++){gItemManager.DBItemByteConvert(pMsg.WarehouseItem[n],&lpObj->GuildWarehouse[n]);}

	pMsg.Money = lpObj->GuildWarehouseMoney;

	gDataServerConnection.DataSend((BYTE*)&pMsg,sizeof(pMsg));

	GDPetItemInfoSaveSend(aIndex,1);
}

void cGuildWarehouse::CGGuildWarehouseMoneyRecv(PMSG_WAREHOUSE_MONEY_RECV* lpMsg, int aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];

	if(lpMsg->type == 0) //From Inventory to Guild Warehouse
	{
		if(lpMsg->money <= 0 || lpMsg->money > MAX_GUILD_WAREHOUSE_MONEY)
		{
			gWarehouse.GCWarehouseMoneySend(aIndex,0,0,0);
			return;
		}

		if((QWORD)(lpObj->GuildWarehouseMoney+lpMsg->money) > MAX_GUILD_WAREHOUSE_MONEY)
		{
			gWarehouse.GCWarehouseMoneySend(aIndex,0,0,0);
			return;
		}

		if(lpMsg->money > lpObj->Money)
		{
			gWarehouse.GCWarehouseMoneySend(aIndex,0,0,0);
			return;
		}

		lpObj->Money -= lpMsg->money;
		lpObj->GuildWarehouseMoney += lpMsg->money;
	}
	else if(lpMsg->type == 1)
	{
		if(lpMsg->money <= 0 || lpMsg->money > MAX_GUILD_WAREHOUSE_MONEY)
		{
			gWarehouse.GCWarehouseMoneySend(aIndex,0,0,0);
			return;
		}
		
		if(lpMsg->money > lpObj->GuildWarehouseMoney)
		{
			gWarehouse.GCWarehouseMoneySend(aIndex,0,0,0);
			return;
		}

		lpObj->GuildWarehouseMoney -= lpMsg->money;
		lpObj->Money += lpMsg->money;

		//int tax = this->GetWarehouseTaxMoney(lpObj->Level,lpObj->WarehousePW);

		//if(lpObj->Money >= ((DWORD)tax))
		//{
		//	lpObj->Money -= tax;
		//}
		//else if(lpObj->WarehouseMoney >= tax)
		//{
		//	lpObj->WarehouseMoney -= tax;
		//}
		//else
		//{
		//	lpObj->WarehouseMoney += lpMsg->money;
		//	lpObj->Money -= lpMsg->money;
		//	this->GCWarehouseMoneySend(aIndex,0,0,0);
		//	return;
		//}
	}

	lpObj->WarehouseCount++;

	gWarehouse.GCWarehouseMoneySend(aIndex,1,lpObj->Money,0);
	this->GCExtWarehouseStateSend(aIndex,1,true);
}


void cGuildWarehouse::GCExtWarehouseStateSend(int aIndex, int state, bool warehouseState)
{
	SDHP_EXTWAREHOUSE_STATE_SEND pMsg;
	pMsg.header.set(0xFB,0x1B,sizeof(pMsg));
	pMsg.state = state;
	pMsg.money = gObj[aIndex].GuildWarehouseMoney;
	//LogAdd(LOG_TEST,"Money = %I64d - %llu",gObj[aIndex].GuildWarehouseMoney,gObj[aIndex].GuildWarehouseMoney);
	pMsg.warehouseState = warehouseState;
	DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);
}

void cGuildWarehouse::CGGetUsersWareStatus(int aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];

	if(lpObj->GuildStatus != G_MASTER)
	{
		return;
	}

	GD_GET_WARE_SATUS_SEND pMsg;
	pMsg.header.set(0xFB,0x07,sizeof(pMsg));
	pMsg.aIndex = aIndex;
	memcpy(pMsg.GuildName,lpObj->Guild->Name,sizeof(pMsg.GuildName));
	
	gDataServerConnection.DataSend((BYTE*)&pMsg,pMsg.header.size);
}

void cGuildWarehouse::DGUsersWareStatusRecv(PMSG_WARE_SATUS_RECV* lpMsg, int size)
{
	lpMsg->header.head = 0xF3;
	lpMsg->header.subh = 0xFA;
	DataSend(lpMsg->aIndex,(BYTE*)lpMsg,size);
}

void cGuildWarehouse::CGUsersWareStatusSave(PMSG_WARE_SATUS_RECV* lpMsg,int aIndex, int size)
{
	LPOBJ lpObj = &gObj[aIndex];

	if(lpObj->GuildStatus != G_MASTER)
	{
		return;
	}

	for(int n=0;n < lpMsg->count;n++)
	{
		PMSG_WARE_SATUS* lpInfo = (PMSG_WARE_SATUS*)(((BYTE*)lpMsg)+sizeof(PMSG_WARE_SATUS_RECV)+(sizeof(PMSG_WARE_SATUS)*n));
		
		bool state = false;

		for ( int i=0;i<MAX_GUILD_USER;i++)
		{
			if ( lpObj->Guild->Use[i] == TRUE )
			{
				if (strcmp(lpObj->Guild->Names[i],lpInfo->Name) != 0)
				{
					state = true;
				}
			}
		}

		if (!state)
		{
			LogAdd(LOG_RED,"[GuildWarehouse] Can't save users state. ErrorCode: 0x1");
			return;
		}

		//lpObj->Guild->Names
	}

	lpMsg->header.head = 0xFB;
	lpMsg->header.subh = 0x08;
	gDataServerConnection.DataSend((BYTE*)lpMsg,size);
}

void cGuildWarehouse::GDCheckAllowWarehouse(int aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];

	if (!gObjIsConnectedGP(aIndex))
	{
		return;
	}

	PMSG_STATUS_CHECK_SEND pMsg;

	pMsg.header.set(0xFB,0x09,sizeof(pMsg));

	pMsg.aIndex = aIndex;

	memcpy(pMsg.Name,lpObj->Name,sizeof(pMsg.Name));
	memcpy(pMsg.Account,lpObj->Account,sizeof(pMsg.Account));

	gDataServerConnection.DataSend((BYTE*)&pMsg,pMsg.header.size);
}

void cGuildWarehouse::DGCheckAllowWarehouseRecv(PMSG_STATUS_CHECK_ANS* lpMsg)
{
	if(gObjIsAccountValid(lpMsg->aIndex,lpMsg->Account) == 0)
	{
		LogAdd(LOG_RED,"[DGCheckAllowWarehouseRecv] Invalid Account [%d](%s)",lpMsg->aIndex,lpMsg->Account);
		CloseClient(lpMsg->aIndex);
		return;
	}

	LPOBJ lpObj = &gObj[lpMsg->aIndex];

	if (!lpMsg->Status)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,"You can't use Guild Warehouse!");
		return;
	}

	this->GDGuildWarehouseItemSend(lpObj->Index,lpObj->GuildName);
}
