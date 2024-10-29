#include "stdafx.h"
#include "DonateShop.h"
#include "ShopManager.h"
#include "NpcTalk.h"
#include "CastleSiegeSync.h"
#include "Shop.h"
#include "Util.h"

cDonateShop gDonateShop;
#if (GAMESERVER_UPDATE == 603)
void cDonateShop::GCDonateShopInfo(int aIndex)
{
	PMSG_DONATESHOP_INFO pMsg;
	pMsg.Head.set(0xFB,0x03, sizeof(pMsg));
	// ----
	//pMsg.PageCount = gShopManager.GetDonateShopCount();
	// ----
	DataSend(aIndex, (LPBYTE)&pMsg, sizeof(pMsg));
}

void cDonateShop::GCSendState(int State, int aIndex)
{
	PMSG_DONATESHOP_STATE pMsg;
	pMsg.Head.set(0xFB, 0x04, sizeof(pMsg));
	// ----
	pMsg.State = State;
	pMsg.Page = gObj[aIndex].DonateShopPage+1;
	pMsg.MaxPage = gShopManager.GetDonateShopCount(gShopManager.GetClassByShopNumber(gObj[aIndex].TargetShopNumber));
	pMsg.Credits = gObj[aIndex].BankEx.Credits;
	// ----
	DataSend(aIndex, (LPBYTE)&pMsg, sizeof(pMsg));
}

void cDonateShop::GCSendCurrentPage(int Page, int aIndex)
{
	PMSG_DONATESHOP_SETPAGE_SEND pMsg;
	pMsg.header.set(0xFB, 0x05, sizeof(pMsg));
	// ----
	pMsg.CurrentPage = Page;
	// ----
	DataSend(aIndex, (LPBYTE)&pMsg, sizeof(pMsg));
}

void cDonateShop::CGPageRecv(PMSG_DONATESHOP_PAGE_RECV* lpMsg, int aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];
   
    if(gObjIsConnectedGP(aIndex) == 0)
    {
        return;
    }
 
    if(lpObj->Interface.type == INTERFACE_SHOP)
    {
		int CurrentPage = lpObj->DonateShopPage;
		int MaxShop = gShopManager.GetDonateShopCount(gShopManager.GetClassByShopNumber(lpObj->TargetShopNumber))-1;
		if (lpMsg->Type == 0)
		{
			CurrentPage--;

			if (CurrentPage < 0)
			{
				CurrentPage = 0;
			}

		}
		else
		{
			CurrentPage++;

			if (CurrentPage > MaxShop)
			{
				CurrentPage = MaxShop;
			}
		}

		int ShopNumber = gShopManager.GetShopNumberByDonateShopIndex(gShopManager.GetClassByShopNumber(lpObj->TargetShopNumber),CurrentPage);

		lpObj->Interface.use = 1;
		lpObj->Interface.type = INTERFACE_SHOP;
		lpObj->Interface.state = 1;
 
		PMSG_NPC_TALK_SEND pMsg;
		pMsg.header.setE(0x30,sizeof(pMsg));
		pMsg.result = 0;
		DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);
		lpObj->DonateShopPage = CurrentPage;
		lpObj->TargetShopNumber = ShopNumber;
		this->GCSendCurrentPage(CurrentPage+1, aIndex);
		gShopManager.GCShopItemListSendByIndex(ShopNumber,aIndex);
		this->GCSendDonateItems(aIndex);
		//GCTaxInfoSend(aIndex,2,gCastleSiegeSync.GetTaxRateStore(aIndex));
	}
}

void cDonateShop::GCSendDonateItems(int aIndex)
{
	//LPOBJ lpObj = &gObj[aIndex];
	BYTE send[4096];

	PMSG_SEND_DONATEITEMS pMsg;
	pMsg.header.set(0xF3, 0xF1, sizeof(pMsg));

	int size = sizeof(pMsg);

	pMsg.Count = 0;

	PMSG_DONATEITEM info;

	for (int n=0; n<SHOP_SIZE;n++)
	{
		CItem item;

		if (gShopManager.GetItemByIndex(gObj[aIndex].TargetShopNumber,&item,n) == 0)
		{
			continue;
		}

		info.ItemIndex = item.m_Index;

		info.Level = item.m_Level;

		info.Grade = item.m_NewOption;

		info.AncOption = item.m_SetOption;

		info.OptionEx = item.m_ItemOptionEx;

		info.Type = item.m_PriceType;

		if (info.Type == 0) // Zen
		{
			info.BuyPrice = item.m_BuyMoney;

			info.SellPrice = item.m_SellMoney;
		}
		else if (info.Type == 1) // Credits
		{
			info.BuyPrice = item.m_BuyCredits;

			info.SellPrice = item.m_SellCredits;
		}
		else if (info.Type == 2) // WCoinC
		{
			info.BuyPrice = item.m_BuyWCoinC;

			info.SellPrice = item.m_SellWCoinC;
		}
		else if (info.Type == 3) // WCoinP
		{
			info.BuyPrice = item.m_BuyWCoinP;

			info.SellPrice = item.m_SellWCoinP;
		}
		else if (info.Type == 4) // WCoinG
		{
			info.BuyPrice = item.m_BuyWCoinG;

			info.SellPrice = item.m_SellWCoinG;
		}
		else
		{
			info.BuyPrice = item.m_BuyMoney;

			info.SellPrice = item.m_SellMoney;
		}
		

		/*info.BuyPrice = item.m_BuyCredits;

		info.SellPrice = item.m_SellCredits;*/

		memcpy(&send[size],&info,sizeof(info));
		size += sizeof(info);

		pMsg.Count ++;
	}

	pMsg.header.size[0] = SET_NUMBERHB(size);

	pMsg.header.size[1] = SET_NUMBERLB(size);

	memcpy(send,&pMsg,sizeof(pMsg));
	DataSend(aIndex,send,size);
}

#endif