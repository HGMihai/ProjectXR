// JewelMix.cpp: implementation of the CJewelMix class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "JewelMix.h"
#include "DSProtocol.h"
#include "ItemManager.h"
#include "Util.h"

CJewelMix gJewelMix;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CJewelMix::CJewelMix() // OK
{

}

CJewelMix::~CJewelMix() // OK
{

}

int CJewelMix::GetJewelSimpleIndex(int type) // OK
{
	switch(type)
	{
		case 0:
			return GET_ITEM(14,13);
		case 1:
			return GET_ITEM(14,14);
		case 2:
			return GET_ITEM(14,16);
		case 3:
			return GET_ITEM(14,22);
		case 4:
			return GET_ITEM(14,31);
		case 5:
			return GET_ITEM(14,41);
		case 6:
			return GET_ITEM(14,42);
		case 7:
			return GET_ITEM(12,15);
		case 8:
			return GET_ITEM(14,43);
		case 9:
			return GET_ITEM(14,44);
		case 10:
			return GET_ITEM(14, 200);
		case 11:
			return GET_ITEM(14, 201);
		case 12:
			return GET_ITEM(14, 202);
		case 13:
			return GET_ITEM(14, 203);
		case 14:
			return GET_ITEM(14, 205);
	}

	return -1;
}

int CJewelMix::GetJewelBundleIndex(int type) // OK
{
	switch(type)
	{
		case 0:
			return GET_ITEM(12,30);
		case 1:
			return GET_ITEM(12,31);
		case 2:
			return GET_ITEM(12,136);
		case 3:
			return GET_ITEM(12,137);
		case 4:
			return GET_ITEM(12,138);
		case 5:
			return GET_ITEM(12,139);
		case 6:
			return GET_ITEM(12,140);
		case 7:
			return GET_ITEM(12,141);
		case 8:
			return GET_ITEM(12,142);
		case 9:
			return GET_ITEM(12,143);
		case 10:
			return GET_ITEM(14, 220);
		case 11:
			return GET_ITEM(14, 221);
		case 12:
			return GET_ITEM(14, 222);
		case 13:
			return GET_ITEM(14, 223);
		case 14:
			return GET_ITEM(14, 224);
	}

	return -1;
}

int CJewelMix::GetJewelBundleType(int index)
{
	switch(index)
	{
		case GET_ITEM(12,30):
			return 0;
		case GET_ITEM(12,31):
			return 1;
		case GET_ITEM(12,136):
			return 2;
		case GET_ITEM(12,137):
			return 3;
		case GET_ITEM(12,138):
			return 4;
		case GET_ITEM(12,139):
			return 5;
		case GET_ITEM(12,140):
			return 6;
		case GET_ITEM(12,141):
			return 7;
		case GET_ITEM(12,142):
			return 8;
		case GET_ITEM(12,143):
			return 9;
		case GET_ITEM(14,220):
			return 10;
		case GET_ITEM(14,221):
			return 11;
		case GET_ITEM(14,222):
			return 12;
		case GET_ITEM(14,223):
			return 13;
		case GET_ITEM(14,224):
			return 14;
	}
	return -1;
}

void CJewelMix::CGJewelMixRecv(PMSG_JEWEL_MIX_RECV* lpMsg,int aIndex) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	if(gObjIsConnectedGP(aIndex) == 0)
	{
		return;
	}
	
	if(lpObj->Interface.use == 0 || lpObj->Interface.type != INTERFACE_COMMON)
	{
		return;
	}

	if(lpObj->ChaosLock != 0)
	{
		this->GCJewelMixResultSend(aIndex,0);
		return;
	}

	if(lpMsg->type < 0 || lpMsg->type > 14)
	{
		this->GCJewelMixResultSend(aIndex,2);
		return;
	}

	if(lpMsg->level < 0 || lpMsg->level > 2)
	{
		this->GCJewelMixResultSend(aIndex,3);
		return;
	}

	lpObj->ChaosLock = 1;

	int count = gItemManager.GetInventoryItemCount(lpObj,this->GetJewelSimpleIndex(lpMsg->type),0);
	
	int JewelCount = (lpMsg->level+1)*10;

	if(count < JewelCount)
	{
		lpObj->ChaosLock = 0;
		this->GCJewelMixResultSend(aIndex,4);
		return;
	}

	int MixMoney = (lpMsg->level+1)*500000;

	if(lpObj->Money < ((DWORD)MixMoney))
	{
		lpObj->ChaosLock = 0;
		this->GCJewelMixResultSend(aIndex,5);
		return;
	}

	lpObj->Money -= MixMoney;

	GCMoneySend(aIndex,lpObj->Money);

	gItemManager.DeleteInventoryItemCount(lpObj,this->GetJewelSimpleIndex(lpMsg->type),0,JewelCount);

	GDCreateItemSend(aIndex,0xEB,0,0,this->GetJewelBundleIndex(lpMsg->type),lpMsg->level,0,0,0,0,-1,0,0,0,0,0,0xFF,0);

	this->GCJewelMixResultSend(aIndex,1);

	lpObj->ChaosLock = 0;
}

void CJewelMix::CGJewelUnMixRecv(PMSG_JEWEL_UNMIX_RECV* lpMsg,int aIndex) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	if(gObjIsConnectedGP(aIndex) == 0)
	{
		return;
	}
	
	if(lpObj->Interface.use == 0 || lpObj->Interface.type != INTERFACE_COMMON)
	{
		return;
	}

	if(lpObj->ChaosLock != 0)
	{
		this->GCJewelUnMixResultSend(aIndex,0);
		return;
	}

	if(lpMsg->type < 0 || lpMsg->type > 9)
	{
		this->GCJewelUnMixResultSend(aIndex,2);
		return;
	}

	if(lpMsg->level < 0 || lpMsg->level > 2)
	{
		this->GCJewelUnMixResultSend(aIndex,3);
		return;
	}

	if(INVENTORY_FULL_RANGE(lpMsg->slot) == 0)
	{
		this->GCJewelUnMixResultSend(aIndex,5);
		return;
	}

	lpObj->ChaosLock = 1;
	
	if(lpObj->Inventory[lpMsg->slot].IsItem() == 0)
	{
		lpObj->ChaosLock = 0;
		this->GCJewelUnMixResultSend(aIndex,4);
		return;
	}

	if(lpObj->Inventory[lpMsg->slot].m_Level != lpMsg->level)
	{
		lpObj->ChaosLock = 0;
		this->GCJewelUnMixResultSend(aIndex,3);
		return;
	}

	//int iInventoryItemType	= gObj[iIndex].pInventory[iInventoryPos].m_Type;
	//int iInventoryItemLevel	= gObj[iIndex].pInventory[iInventoryPos].m_Level;
	int iInventoryItemType = lpObj->Inventory[lpMsg->slot].m_Index;
	//int iInventoryItemLevel = lpObj->Inventory[lpMsg->slot].m_Level;

	if(lpObj->Inventory[lpMsg->slot].m_Index != this->GetJewelBundleIndex(this->GetJewelBundleType(iInventoryItemType)))
	{
		lpObj->ChaosLock = 0;
		this->GCJewelUnMixResultSend(aIndex,6);
		return;
	}

	int count = gItemManager.GetInventoryEmptySlotCount(lpObj);

	int JewelCount = (lpMsg->level+1)*10;

	if(count < JewelCount)
	{
		lpObj->ChaosLock = 0;
		this->GCJewelUnMixResultSend(aIndex,7);
		return;
	}

	int MixMoney = 1000000;

	if(lpObj->Money < ((DWORD)MixMoney))
	{
		lpObj->ChaosLock = 0;
		this->GCJewelUnMixResultSend(aIndex,8);
		return;
	}

	lpObj->Money -= MixMoney;

	GCMoneySend(aIndex,lpObj->Money);

	gItemManager.InventoryDelItem(aIndex,lpMsg->slot);

	gItemManager.GCItemDeleteSend(aIndex,lpMsg->slot,1);

	for(int n=0;n < JewelCount;n++)
	{
		GDCreateItemSend(aIndex,0xEB,0,0,this->GetJewelSimpleIndex(this->GetJewelBundleType(iInventoryItemType)),0,0,0,0,0,-1,0,0,0,0,0,0xFF,0);
	}

	this->GCJewelUnMixResultSend(aIndex,1);

	lpObj->ChaosLock = 0;
}

void CJewelMix::GCJewelMixResultSend(int aIndex,BYTE result) // OK
{
	PMSG_JEWEL_MIX_RESULT_SEND pMsg;

	pMsg.header.set(0xBC,0x00,sizeof(pMsg));

	pMsg.result = result;

	DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);
}

void CJewelMix::GCJewelUnMixResultSend(int aIndex,BYTE result) // OK
{
	PMSG_JEWEL_MIX_RESULT_SEND pMsg;

	pMsg.header.set(0xBC,0x01,sizeof(pMsg));

	pMsg.result = result;

	DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);
}
