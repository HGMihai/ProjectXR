#include "stdafx.h"
//#include "..\\include\\prodef.h"
//#include "DBSockMng.h"
//#include "giocp.h"
//#include "..\\common\\zzzitem.h"
#include "user.h"
#include "Protocol.h"
#include "ItemMarket.h"
//#include "logproc.h"
#include "protocol.h"
//#include "ExUser.h"
#include "ItemManager.h"
#include "GameMain.h"
#include "Util.h"
#include "Notice.h"
#include "BankEx.h"
#include "CashShop.h"
#include "SocketItemOption.h"
#include "SocketItemType.h"

#if(DEV_PLAYERSHOP)

CItemMarket g_ItemMarket;

CItemMarket::CItemMarket()
{
	this->Init();
}

CItemMarket::~CItemMarket()
{
}

void CItemMarket::Init()
{

}

void CItemMarket::Load()
{

}

void CItemMarket::Protocol(BYTE protoNum, BYTE* aRecv)
{

}

void CItemMarket::CGReqItemList(PMSG_REQ_MARKET_ITEM* lpMsg, int aIndex)
{
	SDHP_REQ_MARKET_ITEM pMsg;

	pMsg.h.set(0xEC, 0x00, sizeof(pMsg));

	pMsg.Result = 1;

	pMsg.aIndex = aIndex;

	pMsg.PriceType = lpMsg->PriceType;

#if(MARKET_FILTER_DEV)

	pMsg.Filter.FilterType = lpMsg->Filter.FilterType;

	pMsg.Filter.FilterLevel = lpMsg->Filter.FilterLevel;

	pMsg.Filter.FilterLuck = lpMsg->Filter.FilterLuck;

	pMsg.Filter.FilterExl = lpMsg->Filter.FilterExl;

	pMsg.Filter.FilterAnc = lpMsg->Filter.FilterAnc;

#endif

#if(MARKET_PAGE_DEV)

	pMsg.PageNumber = lpMsg->PageNumber;

#endif

	gDataServerConnection.DataSend((BYTE*)&pMsg, pMsg.h.size);

	//LogAddC(2, "[DATA <- SERVER] : CGReqItemList");
}

void CItemMarket::GDReqItemList(int aIndex, int NumPage)
{

}

void CItemMarket::DGAnsItemList(SDHP_ANS_MARKET_ITEM* lpMsg)
{
	int aIndex = lpMsg->aIndex;

	// ----

	memset(this->m_MarketData[aIndex], 0, sizeof(this->m_MarketData[aIndex]));

	memcpy(this->m_MarketData[aIndex], lpMsg->Data, sizeof(this->m_MarketData[aIndex]));

	// ----

	PMSG_ANS_MARKET_ITEM pMsg;

	memset(&pMsg, 0, sizeof(pMsg));

	pMsg.h.set(0xFA, 0xF5, sizeof(pMsg));

	pMsg.Result = 1;

	CItem item;

	for (int i = 0; i < lpMsg->count; i++)
	{
		pMsg.Info[i].ID = lpMsg->Data[i].ID;

		memcpy(pMsg.Info[i].Name, lpMsg->Data[i].Name, 10);

		// ----

		//item = this->GetByteToItem(this->m_MarketData[aIndex][i].Item);
		if(!gItemManager.ConvertItemByte(&item, this->m_MarketData[aIndex][i].Item))
			continue;

		auto u = lpMsg->Data[i].Item;
		
		LogAdd(LOG_GREEN, "name: %s id: %d %x%x%x%x%x%x%x%x%x%x%x%x%x%x%x%x", lpMsg->Data[i].Name, item.m_Index,
			u[0], u[1], u[2], u[3], u[4], u[5], u[6], u[7], u[8], u[9], u[10], u[11]
			, u[12], u[13], u[14], u[15]);
		
		// ----

		pMsg.Info[i].ItemType = item.m_Index;

		pMsg.Info[i].ItemDurability = item.m_Durability;

		pMsg.Info[i].PriceType = lpMsg->Data[i].PriceType;

		pMsg.Info[i].Price = lpMsg->Data[i].Price;

		gItemManager.ItemByteConvert(pMsg.Info[i].ItemData, item);
	}

	DataSend(aIndex, (LPBYTE)&pMsg, sizeof(pMsg));

	//LogAddC(2, "[SERVER -> CLIENT] [SIZE:%d]: DGAnsItemList", sizeof(pMsg));

	//GCServerMsgStringSend("[ItemMarket] Item List Refresh", aIndex, 0);
	gNotice.GCNoticeSend(aIndex, 0, 0, 0, 0, 0, 0, "[ItemMarket] Item List Refresh");
}

void CItemMarket::CGReqItemSell(PMSG_REQ_MARKET_SELL* lpMsg, int aIndex)
{
	this->GDReqItemSell(aIndex, lpMsg->ItemPos, lpMsg->ItemPriceType, lpMsg->ItemPrice);
}

void CItemMarket::GDReqItemSell(int aIndex, int ItemPos, int ItemPriceType, int ItemPrice)
{
	if (!OBJMAX_RANGE(aIndex))
	{
		return;
	}

	LPOBJ lpUser = &gObj[aIndex];

	SDHP_REQ_MARKET_SELL pMsg;

	pMsg.h.set(0xEC, 0x01, sizeof(pMsg));

	pMsg.Result = 1;

	pMsg.aIndex = aIndex;

	pMsg.PriceType = ItemPriceType;

	pMsg.Price = ItemPrice;

	//pMsg.Account[10] = 0;

	memcpy(pMsg.Account, lpUser->Account, sizeof(pMsg.Account) - 1);

#if(MARKET_NAME_DEV)

	//pMsg.Name[10] = 0;

	memcpy(pMsg.Name, lpUser->Name, sizeof(pMsg.Name) - 1);

#endif

	pMsg.ItemPos = ItemPos;

	//ItemByteConvert16((LPBYTE)pMsg.ItemData, &lpUser->Inventory[ItemPos], 1);
	gItemManager.DBItemByteConvert(pMsg.ItemData, &lpUser->Inventory[ItemPos]);

#if(MARKET_FILTER_DEV)

	BYTE btLuck = lpUser->Inventory[ItemPos].m_LuckOption;

	BYTE btOption = lpUser->Inventory[ItemPos].m_Option;

	BYTE btNewOption = lpUser->Inventory[ItemPos].m_NewOption;

	BYTE btSetOption = lpUser->Inventory[ItemPos].m_SetOption;

	// --

	pMsg.Filter.FilterLuck = 0;

	pMsg.Filter.FilterExl = 0;

	pMsg.Filter.FilterAnc = 0;

	pMsg.Filter.FilterType = lpUser->Inventory[ItemPos].m_Index / 512;

	pMsg.Filter.FilterLevel = lpUser->Inventory[ItemPos].m_Level;

	if (btLuck)
	{
		pMsg.Filter.FilterLuck = 1;
	}

	if (btNewOption)
	{
		pMsg.Filter.FilterExl = 1;
	}

	if (btSetOption)
	{
		pMsg.Filter.FilterAnc = 1;
	}

#endif

	gDataServerConnection.DataSend((BYTE*)&pMsg, pMsg.h.size);

	//LogAddC(2, "[SERVER -> CLIENT] [SIZE:%d]: GDReqItemSell", sizeof(pMsg));
}

void CItemMarket::DGAnsItemSell(SDHP_ANS_MARKET_SELL* lpMsg)
{
	int aIndex = lpMsg->aIndex;

	if (lpMsg->Result)
	{
		//GCServerMsgStringSend("[ItemMarket] SELL Result : 1", aIndex, 0);
		gNotice.GCNoticeSend(aIndex, 0, 0, 0, 0, 0, 0, "[ItemMarket] SELL Result : 1");

		LPOBJ lpUser = &gObj[aIndex];

		int ItemPos = lpMsg->ItemPos;

		gItemManager.InventoryItemSet(lpUser->Index, ItemPos, -1);
		//gObjInventoryItemSet(lpUser->Index, ItemPos, -1);

		lpUser->Inventory[ItemPos].Clear();

		//GCInventoryItemDeleteSend(lpUser->Index, ItemPos, 1);
		gItemManager.GCItemDeleteSend(lpUser->Index, ItemPos, 1);
	}
	else
	{
		//GCServerMsgStringSend("[ItemMarket] SELL Result : 0", aIndex, 0);
		gNotice.GCNoticeSend(aIndex, 0, 0, 0, 0, 0, 0, "[ItemMarket] SELL Result : 0");
	}
}

// ----

void CItemMarket::CGReqItemBuy(PMSG_REQ_MARKET_BUY* lpMsg, int aIndex)
{
	this->GDReqItemBuy(aIndex, lpMsg->ID);
}

void CItemMarket::GDReqItemBuy(int aIndex, int ID)
{
	if (!OBJMAX_RANGE(aIndex))
	{
		return;
	}

	LPOBJ lpUser = &gObj[aIndex];

	SDHP_REQ_MARKET_BUY pMsg;

	pMsg.h.set(0xEC, 0x02, sizeof(pMsg));
	
	pMsg.aIndex = aIndex;

	pMsg.ID = ID;

	pMsg.Result = 0;

	for (int i = 0; i < MARKET_ITEM_MAX; i++)
	{
		if (ID == this->m_MarketData[aIndex][i].ID)
		{
#if(MARKET_PLAYER_BUY)
			if (!strcmp(lpUser->Name, this->m_MarketData[aIndex][i].Name))
			{
				pMsg.Result = 1;
			}
			else
			{
				pMsg.Result = this->GetCheckMoney(aIndex, this->m_MarketData[aIndex][i].PriceType, this->m_MarketData[aIndex][i].Price);
			}
#else
			pMsg.Result = this->GetCheckMoney(aIndex, this->m_MarketData[aIndex][i].PriceType, this->m_MarketData[aIndex][i].Price);
#endif
			break;
		}
	}

	gDataServerConnection.DataSend((BYTE*)&pMsg, pMsg.h.size);

	//LogAddC(2, "[SERVER -> CLIENT] [SIZE:%d]: GDReqItemBuy", sizeof(pMsg));
}

void CItemMarket::DGAnsItemBuy(SDHP_ANS_MARKET_BUY* lpMsg)
{
	int aIndex = lpMsg->aIndex;

	if (!OBJMAX_RANGE(aIndex))
	{
		return;
	}

	if (lpMsg->Result)
	{
		//GCServerMsgStringSend("[ItemMarket] BUY Result : 1", aIndex, 0);
		gNotice.GCNoticeSend(aIndex, 0, 0, 0, 0, 0, 0, "[ItemMarket] BUY Result : 1");

		// ----

		CItem item;

		for (int i = 0; i < MARKET_ITEM_MAX; i++)
		{
			if (lpMsg->ID == this->m_MarketData[aIndex][i].ID)
			{
#if(MARKET_PLAYER_BUY)

				//char szbuffname[64] = { 0 };
				//sprintf(szbuffname, "%s = %s", gObj[aIndex].Name, this->m_MarketData[aIndex][i].Name); 
				//GCServerMsgStringSend(szbuffname, aIndex, 0);

				if (!strcmp(gObj[aIndex].Name, this->m_MarketData[aIndex][i].Name))
				{
					//item = this->GetByteToItem(this->m_MarketData[aIndex][i].Item);
					if (!gItemManager.ConvertItemByte(&item, this->m_MarketData[aIndex][i].Item))
						continue;

					BYTE btItemPos = gItemManager.InventoryInsertItem(lpMsg->aIndex, item);

					//GCInventoryItemOneSend(lpMsg->aIndex, btItemPos);
					gItemManager.GCItemModifySend(lpMsg->aIndex, btItemPos);
				}
				else if (this->PlayerMoney(aIndex, this->m_MarketData[aIndex][i].PriceType, this->m_MarketData[aIndex][i].Price))
				{
					//item = this->GetByteToItem(this->m_MarketData[aIndex][i].Item);
					if (!gItemManager.ConvertItemByte(&item, this->m_MarketData[aIndex][i].Item))
						continue;

					BYTE btItemPos = gItemManager.InventoryInsertItem(lpMsg->aIndex, item);

					gItemManager.GCItemModifySend(lpMsg->aIndex, btItemPos);
				}
#else
				if (this->PlayerMoney(aIndex, this->m_MarketData[aIndex][i].PriceType, this->m_MarketData[aIndex][i].Price))
				{
					item = this->GetByteToItem(this->m_MarketData[aIndex][i].Item);

					BYTE btItemPos = gObjInventoryInsertItem(lpMsg->aIndex, item);

					GCInventoryItemOneSend(lpMsg->aIndex, btItemPos);
				}
#endif
				break;
			}
		}

		// ----

	}
	else
	{
		//GCServerMsgStringSend("[ItemMarket] BUY Result : 0", aIndex, 0);
		gNotice.GCNoticeSend(aIndex, 0, 0, 0, 0, 0, 0, "[ItemMarket] BUY Result : 0");
	}

	PMSG_ANS_MARKET_BUY pMsg;

	pMsg.h.set(0xEC, 0x01, sizeof(pMsg));

	pMsg.Result = 1;

	DataSend(aIndex, (LPBYTE)&pMsg, sizeof(pMsg));
}

#if(MARKET_PRICE_DEV)

void CItemMarket::GDReqItemStatus(int aIndex)
{
	if (!OBJMAX_RANGE(aIndex))
	{
		return;
	}

	LPOBJ lpUser = &gObj[aIndex];

	SDHP_REQ_MARKET_STATUS pMsg;

	pMsg.h.set(0xEC, 0x03, sizeof(pMsg));

	pMsg.Result = 1;

	pMsg.aIndex = aIndex;

	memcpy(pMsg.Account, lpUser->Account, sizeof(pMsg.Account) - 1);

	gDataServerConnection.DataSend((BYTE*)&pMsg, pMsg.h.size);

	//LogAddC(2, "[SERVER -> DATA] [SIZE:%d]: GDReqItemBuy", sizeof(pMsg));
}

void CItemMarket::DGAnsItemStatus(SDHP_ANS_MARKET_STATUS* lpMsg)
{
	int aIndex = lpMsg->aIndex;

	if (!OBJMAX_RANGE(aIndex))
	{
		return;
	}

	if (!lpMsg->Result)
	{
		return;
	}

	LPOBJ lpUser = &gObj[aIndex];

	if (lpMsg->PriceType == eMarketPriceTypeZen)
	{
		lpUser->Money += lpMsg->PriceValue;

		GCMoneySend(aIndex, lpUser->Money);

		//MsgOutput(aIndex, "[MARKET][SELL] %s +%d Zen", lpMsg->Account, lpMsg->PriceValue);
		gNotice.GCNoticeSend(aIndex, 1, 0, 0, 0, 0, 0, "[MARKET][SELL] %s +%d Zen", lpMsg->Account, lpMsg->PriceValue);
	}

	if (lpMsg->PriceType == eMarketPriceTypeWcoin)
	{
		//lpUser->GameShop.WCoinC += lpMsg->PriceValue;

		//gGameShop.GDSaveUserInfo(lpUser->m_Index);
		gCashShop.GDCashShopAddPointSaveSend(aIndex, nullptr, lpMsg->PriceValue,0,0);

		gNotice.GCNoticeSend(aIndex, 1, 0, 0, 0, 0, 0, "[MARKET][SELL] %s +%d Wcoin", lpMsg->Account, lpMsg->PriceValue);
	}

	if (lpMsg->PriceType == eMarketPriceTypeCredit)
	{
		lpUser->BankEx.Credits += lpMsg->PriceValue;

		gBankEx.GCUpdateBankEx(lpUser->Index);
		gBankEx.GDSavePoint(lpUser->Index);
		
		//ExUserDataSend(lpUser->m_Index);

		gNotice.GCNoticeSend(aIndex, 1, 0, 0, 0, 0, 0, "[MARKET][SELL] %s +%d Credit", lpMsg->Account, lpMsg->PriceValue);
	}
}

#endif
//
//CItem CItemMarket::GetByteToItem(LPBYTE lpByte)
//{
//	CItem item;
//
//	item.Clear();
//
//	int n = 0;
//
//	int ItemDbByte = 16;
//
//	int itype = lpByte[n * ItemDbByte];
//
//	/*
//	if(lpMsg->dbInventory[n*ItemDbByte] == 0xFF
//		&& (lpMsg->dbInventory[n*ItemDbByte+7] & 0x80) == 0x80
//		&& (lpMsg->dbInventory[n*ItemDbByte+9] & 0xF0) == 0xF0)
//	{
//		continue;
//	}
//	*/
//
//	itype |= (lpByte[n * ItemDbByte + 9] & 0xF0) * 32;
//
//	itype |= (lpByte[n * ItemDbByte + 7] & 0x80) * 2;
//
//	int _type = itype;
//
//	item.m_Level = DBI_GET_LEVEL(lpByte[n * ItemDbByte + 1]);
//
//	BYTE OptionData = lpByte[n * ItemDbByte + 1];
//
//	item.m_SkillOption = (((OptionData) >> 7) & 0x01);
//
//	item.m_LuckOption = DBI_GET_LUCK(OptionData);
//
//	item.m_Option = DBI_GET_OPTION(OptionData);
//
//	item.m_Option |= DBI_GET_OPTION16(lpByte[n * ItemDbByte + 7]) >> 4;
//
//	item.m_Durability = lpByte[n * ItemDbByte + 2];
//
//	item.m_JewelOfHarmonyOption = lpByte[n * ItemDbByte + 10];
//
//	item.m_ItemOptionEx = DBI_GET_380OPTION(lpByte[n * ItemDbByte + 9]);
//
//	if (item.m_ItemOptionEx)
//	{
//		item.m_Index = itype;
//
//		//if (g_kItemSystemFor380.Is380Item(&item) == false)
//		if(!item.Is380Item())
//		{
//			item.m_ItemOptionEx = 0;
//		}
//	}
//
//	BYTE SocketOption[5];
//
//	BYTE SocketOptionIndex = 0xFF;
//
//	memset(&SocketOption, 0xFF, 5);
//
//	//if (g_SocketOption.CheckItemType(_type) == 1)
//	if(gSocketItemType.CheckSocketItemType(_type))
//	{
//		for (int i = 0; i < 5; i++)
//		{
//			SocketOption[i] = lpByte[((n * ItemDbByte) + 11) + i];
//		}
//
//		SocketOptionIndex = lpByte[n * ItemDbByte + 10];
//	}
//	else
//	{
//		for (int i = 0; i < 5; i++)
//		{
//			SocketOption[i] = 0xFF;
//		}
//
//		SocketOptionIndex = 0;
//	}
//
////#ifdef PERIOD
//
//	item.m_IsPeriodicItem = (lpByte[n * ItemDbByte + 9] & 6) >> 1;
//
//	//item.Convert(itype, item.m_SkillOption, item.m_LuckOption, item.m_Option, DBI_GET_NOPTION(lpByte[n * ItemDbByte + 7]), lpByte[n * ItemDbByte + 8], item.m_ItemOptionEx, SocketOption, SocketOptionIndex, item.m_PeriodItemOption, 3); //season4 changed
//	item.Convert(item.m_Index, item.m_SkillOption, item.m_LuckOption, item.m_Option, item.m_NewOption, item.m_SetOption, item.m_JewelOfHarmonyOption, item.m_ItemOptionEx, item.m_SocketOption, item.m_SocketOptionBonus);
//
//	
////#else
////
////	item.Convert(itype, item.m_Option1, item.m_Option2, item.m_Option3, DBI_GET_NOPTION(lpByte[n * ItemDbByte + 7]), lpByte[n * ItemDbByte + 8], item.m_ItemOptionEx, SocketOption, SocketOptionIndex, 0, 3); //season4 changed
////
////#endif
//
//	WORD hiWord = ((lpByte[n * ItemDbByte + 4]) & 0xFF) & 0xFF | (((lpByte[n * ItemDbByte + 3]) & 0xFF) & 0xFF) * 256;
//
//	WORD loWord = ((lpByte[n * ItemDbByte + 6]) & 0xFF) & 0xFF | (((lpByte[n * ItemDbByte + 5]) & 0xFF) & 0xFF) * 256;
//
//	item.m_Serial = ((loWord & 0xFFFF) & 0xFFFF) | ((hiWord & 0xFFFF) & 0xFFFF) << 16;
//
//	return item;
//}

void CItemMarket::ConnectPlayer(int aIndex)
{
	this->GDReqItemStatus(aIndex);
}

bool CItemMarket::GetCheckMoney(int aIndex, int PriceType, int PriceValue)
{
	bool Result = true;

	LPOBJ lpUser = &gObj[aIndex];

	if (PriceType == eMarketPriceTypeZen)
	{
		if (PriceValue > lpUser->Money)
		{
			Result = false;
		}
	}

	if (PriceType == eMarketPriceTypeWcoin)
	{
		//if (PriceValue > lpUser->GameShop.WCoinC)
		//{
			Result = false;
		//}
	}

	if (PriceType == eMarketPriceTypeCredit)
	{
		if (PriceValue > lpUser->BankEx.Credits)
		{
			Result = false;
		}
	}

	return Result;
}

bool CItemMarket::PlayerMoney(int aIndex, int PriceType, int PriceValue)
{
	bool Result = this->GetCheckMoney(aIndex, PriceType, PriceValue);

	LPOBJ lpUser = &gObj[aIndex];

	if (Result)
	{
		if (PriceType == eMarketPriceTypeZen)
		{
			lpUser->Money -= PriceValue;

			GCMoneySend(aIndex, lpUser->Money);

			//MsgOutput(aIndex, "[MARKET][BUY] %s +%d Zen", lpUser->AccountID, PriceValue);
			gNotice.GCNoticeSend(aIndex,1,0,0,0,0,0, "[MARKET][BUY] %s +%d Zen", lpUser->Account, PriceValue);
		}

		if (PriceType == eMarketPriceTypeWcoin)
		{
			/*pUser->GameShop.WCoinC -= PriceValue;

			gGameShop.GDSaveUserInfo(lpUser->m_Index);

			MsgOutput(aIndex, "[MARKET][BUY] %s +%d Wcoin", lpUser->AccountID, PriceValue);*/
		}

		if (PriceType == eMarketPriceTypeCredit)
		{
			lpUser->BankEx.Credits += PriceValue;

			//ExUserDataSend(lpUser->m_Index);
			gBankEx.GCUpdateBankEx(lpUser->Index);
			gBankEx.GDSavePoint(lpUser->Index);

			//MsgOutput(aIndex, "[MARKET][BUY] %s +%d Credit", lpUser->AccountID, PriceValue);
			gNotice.GCNoticeSend(aIndex, 1, 0, 0, 0, 0, 0, "[MARKET][BUY] %s +%d Credits", lpUser->Account, PriceValue);

		}
	}

	return Result;
}

#if(MARKET_ITEM_RECV)

void CItemMarket::GDReqItemDate(SDHP_REQ_MARKET_ITEMRECV* lpMsg)
{
	int aIndex = lpMsg->aIndex;

	if (!OBJMAX_RANGE(aIndex))
	{
		return;
	}

	SDHP_ANS_MARKET_ITEMRECV pMsg;

	pMsg.h.set(0xEC, 0x04, sizeof(pMsg));

	pMsg.Result = 1;

	pMsg.ID = lpMsg->ID;

	//GCServerMsgStringSend("[ItemMarket] Item Recv : 1", aIndex, 0);
	gNotice.GCNoticeSend(aIndex, 0, 0, 0, 0, 0, 0, "[ItemMarket] Item Recv : 1");

	CItem item;

	//item = this->GetByteToItem(lpMsg->Item);
	if (!gItemManager.ConvertItemByte(&item, lpMsg->Item))
		return;

	BYTE btItemPos = gItemManager.InventoryInsertItem(aIndex, item);

	if (btItemPos == 0xFF)
	{
		return;
	}

	gItemManager.GCItemModifySend(lpMsg->aIndex, btItemPos);

	gDataServerConnection.DataSend((BYTE*)&pMsg, pMsg.h.size);
}

#endif

#endif