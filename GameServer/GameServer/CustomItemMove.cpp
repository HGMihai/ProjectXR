#include "stdafx.h"
#include "CustomItemMove.h"
#include "Util.h"


//ChaosBoxInsertItem

CCustomItemMove gCustomItemMove;

void CCustomItemMove::CGItemMove(PMSG_CUSTOM_ITEM_MOVE* lpMsg, int aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];

	if(gObjIsConnectedGP(aIndex) == 0)
	{
		return;
	}

	if(lpObj->DieRegen != 0)
	{
		return;
	}

	if(INVENTORY_FULL_RANGE(lpMsg->slot) == 0)
	{
		return;
	}

	/*for(int i=INVENTORY_WEAR_SIZE;i<INVENTORY_MAIN_SIZE;i++)
	{
		if (lpObj->Inventory[i].IsItem())
		{
			LogAdd(LOG_TEST,"Is item! slot = %d",i);
		}
	}*/
	//LogAdd(LOG_TEST,"Slot %d state %d",lpMsg->slot,lpObj->Inventory[12].IsItem());
	
	if (lpMsg->type == ItemMoveType::ChaosBox) // chaosbox
	{
		if (lpObj->Interface.type != INTERFACE_CHAOS_BOX)
		{
			return;
		}

		if (!lpObj->Inventory[lpMsg->slot].IsItem())
		{
			return;
		}

		PMSG_ITEM_MOVE_SEND pMsg;

		pMsg.header.setE(0x24,sizeof(pMsg));

		pMsg.result = 0xFF;

		memset(pMsg.ItemInfo,0xFF,sizeof(pMsg.ItemInfo));

		int slot = gItemManager.ChaosBoxFindPlace(aIndex,lpObj->Inventory[lpMsg->slot]);

		if (slot == 0xFF)
		{
			return;
		}
		pMsg.slot = slot;
		BYTE res = gItemManager.MoveItemToChaosBoxFromInventory(lpObj,lpMsg->slot,slot,3);

		if((pMsg.result=res) != 0xFF)
		{
			gItemManager.ItemByteConvert(pMsg.ItemInfo,lpObj->ChaosBox[slot]);
		}

		//gItemManager.UpdateInventoryViewport(aIndex,lpMsg->slot);
		gItemManager.GCItemDeleteSend(aIndex,lpMsg->slot,1);
		
		DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);
	}
}
