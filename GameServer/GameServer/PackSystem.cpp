#include "stdafx.h"
#include "PackSystem.h"
#include "MemScript.h"
#include "Util.h"
#include "ItemManager.h"
#include "DSProtocol.h"

CPackSystem gPackSystem;

void CPackSystem::Load(char* path)
{
	pugi::xml_document file;
	pugi::xml_parse_result res = file.load_file(path);

	if(res.status != pugi::status_ok)
	{
		ErrorMessageBox("%s file load fail (%s)", path, res.description());
	}
	
	this->m_Data.clear();
	pugi::xml_node PackSystem = file.child("PackSystem");
	for (pugi::xml_node Jewel = PackSystem.child("Jewel"); Jewel; Jewel = Jewel.next_sibling())
	{
		PackSystemData info;

		int group = Jewel.attribute("Group").as_int();

		int id = Jewel.attribute("ID").as_int();

		info.JewelID = GET_ITEM(group,id);

		group = Jewel.attribute("BundleGroup").as_int();

		id = Jewel.attribute("BundleID").as_int();

		info.JewelBundleID = GET_ITEM(group,id);

		info.Price10 = Jewel.attribute("Price10").as_int();

		info.Price20 = Jewel.attribute("Price20").as_int();

		info.Price30 = Jewel.attribute("Price30").as_int();

		info.PriceUnPack10 = Jewel.attribute("PriceUnPack10").as_int();

		info.PriceUnPack20 = Jewel.attribute("PriceUnPack20").as_int();

		info.PriceUnPack30 = Jewel.attribute("PriceUnPack30").as_int();

		this->m_Data.push_back(info);
	}
}

void CPackSystem::GCSendData(int aIndex)
{
	BYTE send[4096];

	PMSG_SEND_PACKSYSTEM pMsg;
	pMsg.header.set(0xF3, 0xF5, sizeof(pMsg));

	int size = sizeof(pMsg);

	pMsg.Count = 0;

	PackSystemData info;

	for (int n=0; n<this->m_Data.size();n++)
	{
		info = this->m_Data[n];

		memcpy(&send[size],&info,sizeof(info));
		size += sizeof(info);

		pMsg.Count ++;
	}

	pMsg.header.size[0] = SET_NUMBERHB(size);

	pMsg.header.size[1] = SET_NUMBERLB(size);

	memcpy(send,&pMsg,sizeof(pMsg));
	DataSend(aIndex,send,size);
}

void CPackSystem::CGMixRecv(CG_MIX_RECV* lpMsg, int aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];

	if(gObjIsConnectedGP(aIndex) == 0)
	{
		return;
	}

	if(lpObj->ChaosLock != 0)
	{
		this->GCPackSystemResultSend(aIndex,0);
		return;
	}

	if(lpObj->Interface.use == 0 || lpObj->Interface.type != INTERFACE_COMMON)
	{
		return;
	}

	if (lpMsg->level < 0 || lpMsg->level > 2)
	{
		return;
	}

	lpObj->ChaosLock = 1;

	int count = gItemManager.GetInventoryItemCount(lpObj,this->m_Data[lpMsg->index].JewelID,0);

	int JewelCount = (lpMsg->level+1)*10;

	if(count < JewelCount)
	{
		lpObj->ChaosLock = 0;
		//LogAdd(LOG_RED,"[%s][%s] Dont have enougth jewels!",lpObj->Account,lpObj->Name);
		this->GCPackSystemResultSend(aIndex,4);
		return;
	}

	int MixMoney = 0;
	if (lpMsg->level == 0)
	{
		MixMoney = this->m_Data[lpMsg->index].Price10;
	}
	else if (lpMsg->level == 1)
	{
		MixMoney = this->m_Data[lpMsg->index].Price20;
	}
	else if (lpMsg->level == 2)
	{
		MixMoney = this->m_Data[lpMsg->index].Price30;
	}
	
	if(lpObj->Money < ((DWORD)MixMoney))
	{
		lpObj->ChaosLock = 0;
		//LogAdd(LOG_RED,"[%s][%s] Dont have enougth money for MIX!",lpObj->Account,lpObj->Name);
		this->GCPackSystemResultSend(aIndex,5);
		return;
	}

	lpObj->Money -= MixMoney;

	GCMoneySend(aIndex,lpObj->Money);

	gItemManager.DeleteInventoryItemCount(lpObj,this->m_Data[lpMsg->index].JewelID,0,JewelCount);

	GDCreateItemSend(aIndex,0xEB,0,0,this->m_Data[lpMsg->index].JewelBundleID,lpMsg->level,0,0,0,0,-1,0,0,0,0,0,0xFF,0);

	this->GCPackSystemResultSend(aIndex,1);

	lpObj->ChaosLock = 0;
}

void CPackSystem::CGUnMixRecv(CG_UNMIX_RECV* lpMsg, int aIndex)
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
		//this->GCJewelUnMixResultSend(aIndex,0);
		return;
	}

	/*if(lpMsg->type < 0 || lpMsg->type > 9)
	{
		this->GCJewelUnMixResultSend(aIndex,2);
		return;
	}*/

	if(lpMsg->level < 0 || lpMsg->level > 2)
	{
		//this->GCJewelUnMixResultSend(aIndex,3);
		return;
	}

	lpObj->ChaosLock = 1;

	int count = gItemManager.GetInventoryItemCount(lpObj,this->m_Data[lpMsg->index].JewelBundleID,lpMsg->level);
	int jewelCount = (lpMsg->level+1)*10;
	if (count <= 0)
	{
		//this->GCJewelUnMixResultSend(aIndex,5);
		lpObj->ChaosLock = 0;
		return;
	}

	if (gItemManager.GetInventoryEmptySlotCount(lpObj) < jewelCount)
	{
		//this->GCJewelUnMixResultSend(aIndex,5);
		lpObj->ChaosLock = 0;
		return;
	}

	int MixMoney = 0;
	if (lpMsg->level == 0)
	{
		MixMoney = this->m_Data[lpMsg->index].PriceUnPack10;
	}
	else if (lpMsg->level == 1)
	{
		MixMoney = this->m_Data[lpMsg->index].PriceUnPack20;
	}
	else if (lpMsg->level == 2)
	{
		MixMoney = this->m_Data[lpMsg->index].PriceUnPack30;
	}
	/*else
	{
		MixMoney = this->m_Data[lpMsg->index].PriceUnPack10;
	}*/

	if(lpObj->Money < ((DWORD)MixMoney))
	{
		lpObj->ChaosLock = 0;
		//this->GCJewelUnMixResultSend(aIndex,8);
		return;
	}

	lpObj->Money -= MixMoney;

	GCMoneySend(aIndex,lpObj->Money);

	int slot = gItemManager.GetInventoryItemSlot(lpObj,this->m_Data[lpMsg->index].JewelBundleID,lpMsg->level);

	gItemManager.InventoryDelItem(aIndex,slot);

	gItemManager.GCItemDeleteSend(aIndex,slot,1);

	for(int n=0;n < jewelCount;n++)
	{
		GDCreateItemSend(aIndex,0xEB,0,0,this->m_Data[lpMsg->index].JewelID,0,0,0,0,0,-1,0,0,0,0,0,0xFF,0);
	}

	//this->GCJewelUnMixResultSend(aIndex,1);

	lpObj->ChaosLock = 0;
}


void CPackSystem::GCPackSystemResultSend(int aIndex, BYTE result)
{
	CG_MIX_RESULT_SEND pMsg;

	pMsg.h.set(0xFB,0x18,sizeof(pMsg));

	pMsg.result = result;

	DataSend(aIndex,(BYTE*)&pMsg,pMsg.h.size);
}
