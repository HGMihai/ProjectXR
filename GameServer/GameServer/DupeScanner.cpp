#include "stdafx.h"
#include "DupeScanner.h"
#include "User.h"
#include "ItemManager.h"
#include "Notice.h"
#include "DSProtocol.h"
#include "JSProtocol.h"
#include "Util.h"

CDupeScanner gDupeScanner;

void CDupeScanner::load_settings(char* path, char* section)
{
	this->settings.is_active = GetPrivateProfileInt(section,"DupeScannerActive",0,path);
	this->settings.use_disconnect= GetPrivateProfileInt(section,"DupeScannerDisconnect",0,path);
	this->settings.use_clean = GetPrivateProfileInt(section,"DupeScannerClean",0,path);
	this->settings.use_ban_character = GetPrivateProfileInt(section,"DupeScannerBanCharacter",0,path);
	this->settings.use_ban_account = GetPrivateProfileInt(section,"DupeScannerBanAccount",0,path);
	this->settings.check_delay = GetPrivateProfileInt(section,"DupeScannerCheckDelay",0,path);
}

void CDupeScanner::scan()
{
	if(!this->settings.is_active)
		return;

	for(int user_index=OBJECT_START_USER; user_index < MAX_OBJECT; user_index++)
	{
		if(!gObjIsConnected(user_index))
			continue;

		LPOBJ lpObj = &gObj[user_index];

		for(int n=0; n < INVENTORY_SIZE; n++)
		{
			if(!lpObj->Inventory[n].IsItem())
				continue;

			CItem* lpItem = &lpObj->Inventory[n];

			if(!gItemManager.GetItemHaveSerial(lpItem->m_Index))
				continue;

			int item_serial = lpItem->m_Serial;

			if(item_serial == 0)
				continue;

			if(this->check_inventory(user_index,item_serial,n))
			{
				gNotice.GCNoticeSend(user_index,0,0,0,0,0,0,"..:: Anti-Cheat ::..");
				gNotice.GCNoticeSend(user_index,0,0,0,0,0,0,"... Inventory Dupe Founded ...");

				if(this->settings.use_clean)
				{
					gItemManager.InventoryDelItem(user_index,n);
					gItemManager.GCItemDeleteSend(user_index,n,1);
					gItemManager.GCItemModifySend(user_index,n);
				}

				if(this->settings.use_ban_character)
				{
					gNotice.GCNoticeSend(lpObj->Index, 0, 0, 0, 0, 0, 0, "Your character banned. Goodbye!");	

					GDSetCharacterBlockSend(lpObj->Index,1);

					if(!this->settings.use_disconnect)
						GCCloseClientSend(user_index,0);
				}

				if(this->settings.use_ban_account)
				{
					gNotice.GCNoticeSend(lpObj->Index, 0, 0, 0, 0, 0, 0, "Your account banned. Goodbye!");	

					GJSetBlockCodeSend(lpObj->Index,1);

					if(!this->settings.use_disconnect)
						GCCloseClientSend(user_index,0);
				}

				if(this->settings.use_disconnect)
					GCCloseClientSend(user_index,0);
			}
		}

		for(int n = 0; n < WAREHOUSE_SIZE; n++)
		{
			if(!lpObj->Warehouse[n].IsItem())
				continue;

			CItem* lpItem = &lpObj->Warehouse[n];

			if(!gItemManager.GetItemHaveSerial(lpItem->m_Index))
				continue;

			int item_serial = lpItem->m_Serial;

			if(item_serial == 0)
				continue;

			if(this->check_warehouse(user_index,item_serial,n))
			{
				gNotice.GCNoticeSend(user_index,0,0,0,0,0,0,"..:: Anti-Cheat ::..");
				gNotice.GCNoticeSend(user_index,0,0,0,0,0,0,"... Warehouse Dupe Founded ...");

				if(this->settings.use_clean)
				{
					//gItemManager.InventoryDelItem(user_index,n);
					//gItemManager.GCItemDeleteSend(user_index,n,1);
					//gItemManager.GCItemModifySend(user_index,n);
					gItemManager.WarehouseDelItem(user_index,n);
				}

				if(this->settings.use_ban_character)
				{
					gNotice.GCNoticeSend(lpObj->Index, 0, 0, 0, 0, 0, 0, "Your character banned. Goodbye!");	

					GDSetCharacterBlockSend(lpObj->Index,1);

					if(!this->settings.use_disconnect)
						GCCloseClientSend(user_index,0);
				}

				if(this->settings.use_ban_account)
				{
					gNotice.GCNoticeSend(lpObj->Index, 0, 0, 0, 0, 0, 0, "Your account banned. Goodbye!");	

					GJSetBlockCodeSend(lpObj->Index,1);

					if(!this->settings.use_disconnect)
						GCCloseClientSend(user_index,0);
				}

				if(this->settings.use_disconnect)
					GCCloseClientSend(user_index,0);
			}
		}
	}
	
}

bool CDupeScanner::check_inventory(int aIndex, int item_serial, int item_pos)
{
	LPOBJ lpObj = &gObj[aIndex];
	
	for(int n = 0; n < INVENTORY_SIZE; n++)
	{
		if(n == item_pos)
			continue;

		if(!lpObj->Inventory[n].IsItem())
			continue;

		CItem* lpItem = &lpObj->Inventory[n];

		if(!gItemManager.GetItemHaveSerial(lpItem->m_Index))
			continue;

		int current_serial = lpItem->m_Serial;

		if(current_serial == item_serial)
			return true;
	}

	for(int n = 0; n < WAREHOUSE_SIZE; n++)
	{
		if(!lpObj->Warehouse[n].IsItem())
			continue;

		CItem* lpItem = &lpObj->Warehouse[n];

		if(!gItemManager.GetItemHaveSerial(lpItem->m_Index))
			continue;

		int current_serial = lpItem->m_Serial;

		if(current_serial == item_serial)
			return true;
	}

	return false;
}

bool CDupeScanner::check_warehouse(int aIndex, int item_serial, int item_pos)
{
	LPOBJ lpObj = &gObj[aIndex];
	
	for(int n = 0; n < INVENTORY_SIZE; n++)
	{
		if(!lpObj->Inventory[n].IsItem())
			continue;

		CItem* lpItem = &lpObj->Inventory[n];

		if(!gItemManager.GetItemHaveSerial(lpItem->m_Index))
			continue;

		int current_serial = lpItem->m_Serial;

		if(current_serial == item_serial)
			return true;
	}

	for(int n = 0; n < WAREHOUSE_SIZE; n++)
	{
		if(n == item_pos)
			continue;
		
		if(!lpObj->Warehouse[n].IsItem())
			continue;

		CItem* lpItem = &lpObj->Warehouse[n];

		if(!gItemManager.GetItemHaveSerial(lpItem->m_Index))
			continue;

		int current_serial = lpItem->m_Serial;

		if(current_serial == item_serial)
			return true;
	}

	return false;
}
