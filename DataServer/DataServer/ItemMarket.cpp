#include "stdafx.h"
#include "QueryManager.h"
#include "SocketManager.h"
#include "Util.h"
#include "ItemMarket.h"

#if(DEV_PLAYERSHOP)

CItemMarket g_ItemMarket;

CItemMarket::CItemMarket()
= default;

CItemMarket::~CItemMarket()
= default;

//
//bool CItemMarket::Connect()
//{
//	bool Result = gQueryManager.Connect(3, szDbConnectDsn, szDbConnectID, szDbConnectPass);
//
//	if (Result)
//	{
//		this->CreateTable();
//
//		bool Result = this->m_DBItem.Connect(3, szDbConnectDsn, szDbConnectID, szDbConnectPass);
//	}
//
//	return Result;
//}
//
//void CItemMarket::CreateTable()
//{
//	gQueryManager.WriteLog = false;
//
//	/*
//		CREATE TABLE [dbo].[ItemMarketData]( [ID] [int] IDENTITY(1,1) NOT NULL, [Account] [varchar](10) NULL, [Name] [varchar](10) NULL, ) ON [PRIMARY]
//	*/
//
//	gQueryManager.ExecQuery("CREATE TABLE [dbo].[ItemMarketData]( [ID] [int] IDENTITY(1,1) NOT NULL) ON [PRIMARY]");
//	gQueryManager.Fetch();
//	gQueryManager.Close();
//
//	gQueryManager.ExecQuery("ALTER TABLE [ItemMarketData] ADD [Account] [varchar](10) NULL");
//	gQueryManager.Fetch();
//	gQueryManager.Close();
//
//#if(MARKET_NAME_DEV)
//
//	gQueryManager.ExecQuery("ALTER TABLE [ItemMarketData] ADD [Name][varchar](10) NULL");
//	gQueryManager.Fetch();
//	gQueryManager.Close();
//
//#endif
//
//	gQueryManager.ExecQuery("ALTER TABLE [ItemMarketData] ADD [PriceType] INT not null default(0)");
//	gQueryManager.Fetch();
//	gQueryManager.Close();
//
//	gQueryManager.ExecQuery("ALTER TABLE [ItemMarketData] ADD [PriceValue] INT not null default(0)");
//	gQueryManager.Fetch();
//	gQueryManager.Close();
//
//	gQueryManager.ExecQuery("ALTER TABLE [ItemMarketData] ADD [Status] INT not null default(0)");
//	gQueryManager.Fetch();
//	gQueryManager.Close();
//
//#if(MARKET_FILTER_DEV)
//
//	gQueryManager.ExecQuery("ALTER TABLE [ItemMarketData] ADD [FilterType] INT not null default(0)");
//	gQueryManager.Fetch();
//	gQueryManager.Close();
//
//	gQueryManager.ExecQuery("ALTER TABLE [ItemMarketData] ADD [FilterLevel] INT not null default(0)");
//	gQueryManager.Fetch();
//	gQueryManager.Close();
//
//	gQueryManager.ExecQuery("ALTER TABLE [ItemMarketData] ADD [FilterLuck] INT not null default(0)");
//	gQueryManager.Fetch();
//	gQueryManager.Close();
//
//	gQueryManager.ExecQuery("ALTER TABLE [ItemMarketData] ADD [FilterExl] INT not null default(0)");
//	gQueryManager.Fetch();
//	gQueryManager.Close();
//
//	gQueryManager.ExecQuery("ALTER TABLE [ItemMarketData] ADD [FilterAnc] INT not null default(0)");
//	gQueryManager.Fetch();
//	gQueryManager.Close();
//
//#endif
//
//	gQueryManager.ExecQuery("ALTER TABLE [ItemMarketData] ADD [Date] [varchar](20) NULL");
//	gQueryManager.Fetch();
//	gQueryManager.Close();
//
//	gQueryManager.ExecQuery("ALTER TABLE [ItemMarketData] ADD [Item] [varbinary](16) NULL");
//	gQueryManager.Fetch();
//	gQueryManager.Close();
//
//	gQueryManager.WriteLog = true;
//}

void CItemMarket::Protocol(BYTE protoNum, BYTE* aRecv, int uIndex)
{
	switch (protoNum)
	{
	case 0x00:
		this->GDReqItemList((SDHP_REQ_MARKET_ITEM*)aRecv, uIndex);
		break;
	case 0x01:
		this->GDReqItemSell((SDHP_REQ_MARKET_SELL*)aRecv, uIndex);
		break;
	case 0x02:
		this->GDReqBuyItem((SDHP_REQ_MARKET_BUY*)aRecv, uIndex);
		break;
#if(MARKET_FILTER_DEV)
	case 0x03:
		this->GDReqItemStatus((SDHP_REQ_MARKET_STATUS*)aRecv, uIndex);
		break;
#endif
#if(MARKET_ITEM_RECV)
	case 0x04:
		this->GDAnsItemDate((SDHP_ANS_MARKET_ITEMRECV*)aRecv, uIndex);
		break;
#endif
	default:
		break;
	}
}

void CItemMarket::GDReqItemList(SDHP_REQ_MARKET_ITEM* lpMsg, int uIndex)
{
#if(MARKET_PRICE_DEV)

	this->GDReqItemListPage(lpMsg, uIndex);

#else

	char szQuery[256];

	int iReturnCode = 0;

	int iCount = 0;

	SDHP_ANS_MARKET_ITEM pMsg;

	memset(&pMsg, 0, sizeof(pMsg));

	pMsg.h.set((LPBYTE)&pMsg, 0xFE, 0x00, sizeof(pMsg));

	pMsg.Result = 1;

	pMsg.aIndex = lpMsg->aIndex;

	// --

	char szPriceType[256];

	memset(szPriceType, 0, sizeof(szPriceType));

	if (lpMsg->PriceType == eMarketPriceTypeZen)
	{
		sprintf_s(szPriceType, "AND PriceType = %d ", lpMsg->PriceType);
	}

	if (lpMsg->PriceType == eMarketPriceTypeWcoin)
	{
		sprintf_s(szPriceType, "AND PriceType = %d ", lpMsg->PriceType);
	}

	if (lpMsg->PriceType == eMarketPriceTypeCredit)
	{
		sprintf_s(szPriceType, "AND PriceType = %d ", lpMsg->PriceType);
	}

	// --

#if(MARKET_FILTER_DEV)

	char szFilter[256];

	memset(szFilter, 0, sizeof(szFilter));

	if (lpMsg->Filter.FilterType < 16)
	{
		sprintf_s(szFilter, "AND FilterType = %d ", lpMsg->Filter.FilterType);
	}

	if (lpMsg->Filter.FilterLevel > 0 && lpMsg->Filter.FilterLevel <= 15)	//=(
	{
		char szLevel[256];

		memset(szLevel, 0, sizeof(szLevel));

		sprintf_s(szLevel, "AND FilterLevel >= %d AND FilterLevel <= 15 ", lpMsg->Filter.FilterLevel);

		sprintf_s(szFilter, szLevel);
	}

	if (lpMsg->Filter.FilterLuck)
	{
		sprintf_s(szFilter, "AND FilterLuck = 1 ");
	}

	if (lpMsg->Filter.FilterExl)
	{
		sprintf_s(szFilter, "AND FilterExl = 1 ");
	}

	if (lpMsg->Filter.FilterAnc)
	{
		sprintf_s(szFilter, "AND FilterAnc = 1 ");
	}

	sprintf_s(szQuery, "SELECT TOP %d ID, Account, Name, PriceType, PriceValue FROM ItemMarketData WHERE Status = 0 %s %s ORDER BY ID DESC", MARKET_ITEM_MAX, szFilter, szPriceType);

#else

	sprintf_s(szQuery, "SELECT TOP %d ID, Account, PriceType, PriceValue FROM ItemMarketData WHERE Status = 0 ORDER BY ID DESC", MARKET_ITEM_MAX); // <<== Arg

#endif

	if (!gQueryManager.ExecQuery(szQuery))
	{
		gQueryManager.Close();

		pMsg.Result = 0;
	}
	else
	{
		short sqlReturn = gQueryManager.Fetch();

		while (sqlReturn != SQL_NO_DATA && sqlReturn != SQL_NULL_DATA)
		{
			iReturnCode = gQueryManager.GetInt(1);

			if (iReturnCode < 0)
			{
				break;
			}

			char szSeller[MARKET_NAME_LEN];

			//gQueryManager.GetStr("account", szSeller);
			gQueryManager.GetStr("Name", szSeller);

			pMsg.Data[iCount].Name[MARKET_NAME_LEN - 1] = 0;

			memcpy(pMsg.Data[iCount].Name, szSeller, MARKET_NAME_LEN - 1);

			pMsg.Data[iCount].PriceType = gQueryManager.GetInt("PriceType");

			pMsg.Data[iCount].Price = gQueryManager.GetInt("PriceValue");

			// --

			int id = gQueryManager.GetInt("ID");

			pMsg.Data[iCount].ID = id;

			sprintf_s(szQuery, "SELECT Item FROM ItemMarketData WHERE ID = %d", id);

			int	nRet = this->m_DBItem.ReadBlob(szQuery, pMsg.Data[iCount].Item);

			if (nRet < 0)
			{
				memset(pMsg.Data[iCount].Item, -1, sizeof(pMsg.Data[iCount].Item));
			}

			// --

			iCount++;

			if (iCount >= MARKET_ITEM_MAX)
			{
				break;
			}

			sqlReturn = gQueryManager.Fetch();
		}

		gQueryManager.Close();
	}

	wsjServer.DataSend(uIndex, (char*)&pMsg, sizeof(pMsg));

	//LogAddC(2, "[DATA -> SERVER] : GDReqItemList");

#endif
}

void CItemMarket::GDReqItemSell(SDHP_REQ_MARKET_SELL* lpMsg, int uIndex)
{
	SDHP_ANS_MARKET_SELL pMsg;

	memset(&pMsg, 0, sizeof(pMsg));

	pMsg.h.set(0xFE, 0x01, sizeof(pMsg));

	pMsg.Result = 1;

	pMsg.aIndex = lpMsg->aIndex;

	pMsg.ItemPos = lpMsg->ItemPos;

#if(MARKET_FILTER_DEV)

	char szAccount[11];
	memset(szAccount, 0, sizeof(szAccount));
	memcpy(szAccount, lpMsg->Account, sizeof(szAccount) - 1);

	char szName[11];
	memset(szName, 0, sizeof(szName));
	memcpy(szName, lpMsg->Name, sizeof(szName) - 1);

#if(MARKET_ITEM_RECV)

	char szDate[20] = { 0 };

	SYSTEMTIME st;

	GetLocalTime(&st);

	sprintf_s(szDate, "%02d.%02d.%04d-%02d:%02d", st.wDay, st.wMonth, st.wYear, st.wHour, st.wMinute);
	
	/*sprintf_s(szQuery,
		"INSERT INTO ItemMarketData (Account, PriceType, PriceValue, Date, Status, FilterLuck, FilterExl, FilterAnc, FilterType, FilterLevel, Name) VALUES ('%s', %d, %d, '%s', 0, %d, %d, %d, %d, %d, '%s')",
		lpMsg->Account,
		lpMsg->PriceType,
		lpMsg->Price,
		szDate,
		lpMsg->Filter.FilterLuck,
		lpMsg->Filter.FilterExl,
		lpMsg->Filter.FilterAnc,
		lpMsg->Filter.FilterType,
		lpMsg->Filter.FilterLevel,
		lpMsg->Name
	);*/

#else

	sprintf_s(szQuery,
		"INSERT INTO ItemMarketData (Account, PriceType, PriceValue, Date, Status, FilterLuck, FilterExl, FilterAnc, FilterType, FilterLevel, Name) VALUES ('%s', %d, %d, '%s', 0, %d, %d, %d, %d, %d, '%s')",
		lpMsg->Account,
		lpMsg->PriceType,
		lpMsg->Price,
		"19.12.2020",
		lpMsg->Filter.FilterLuck,
		lpMsg->Filter.FilterExl,
		lpMsg->Filter.FilterAnc,
		lpMsg->Filter.FilterType,
		lpMsg->Filter.FilterLevel,
		lpMsg->Name
	);

#endif

#else

	sprintf_s(szQuery, "INSERT INTO ItemMarketData (Account, PriceType, PriceValue, Date, Status) VALUES ('%s', %d, %d, '%s', 0)",
		lpMsg->Account, lpMsg->PriceType, lpMsg->Price, "19.12.2020");

#endif

	gQueryManager.BindParameterAsBinary(1, lpMsg->ItemData, sizeof(lpMsg->ItemData));
	
	pMsg.Result = gQueryManager.ExecQuery("INSERT INTO ItemMarketData (Account, PriceType, PriceValue, Date, Status, FilterLuck, FilterExl, FilterAnc, FilterType, FilterLevel, Name, Item) VALUES ('%s', %d, %d, '%s', 0, %d, %d, %d, %d, %d, '%s', ?)",
		lpMsg->Account,
		lpMsg->PriceType,
		lpMsg->Price,
		szDate,
		lpMsg->Filter.FilterLuck,
		lpMsg->Filter.FilterExl,
		lpMsg->Filter.FilterAnc,
		lpMsg->Filter.FilterType,
		lpMsg->Filter.FilterLevel,
		lpMsg->Name
	);

	gQueryManager.Fetch();

	gQueryManager.Close();

	// --

	//sprintf_s(szQuery, "SELECT TOP 1 ID FROM ItemMarketData ORDER BY ID DESC");	//need recode

	//if (!gQueryManager.ExecQuery(szQuery) || gQueryManager.Fetch() == SQL_NO_DATA)
	//{
	//	pMsg.Result = 0;
	//}

	//int id = gQueryManager.GetAsInteger("ID");

	//gQueryManager.Close();

	// --

	/*gQueryManager.BindParameterAsBinary(1, lpMsg->ItemData, sizeof(lpMsg->ItemData));

	sprintf_s(szQuery, "UPDATE ItemMarketData SET Item = ? WHERE ID = %d", id);
	gQueryManager.Close();*/
	// --

	gSocketManager.DataSend(uIndex, (BYTE*)&pMsg, sizeof(pMsg));

	//LogAddC(2, "[DATA -> SERVER] : GDReqItemSell");
}

void CItemMarket::GDReqBuyItem(SDHP_REQ_MARKET_BUY* lpMsg, int uIndex)
{
	char szQuery[256];

	int iResult = lpMsg->Result;

	if (iResult)
	{
		sprintf_s(szQuery, "UPDATE ItemMarketData SET Status = 1 WHERE ID = %d", lpMsg->ID);

		iResult = gQueryManager.ExecQuery(szQuery);

		gQueryManager.Close();
	}

	SDHP_ANS_MARKET_BUY pMsg;

	memset(&pMsg, 0, sizeof(pMsg));

	pMsg.h.set(0xFE, 0x02, sizeof(pMsg));

	pMsg.Result = iResult;

	pMsg.aIndex = lpMsg->aIndex;

	pMsg.ID = lpMsg->ID;

	gSocketManager.DataSend(uIndex, (BYTE*)&pMsg, sizeof(pMsg));
}

#if(MARKET_PRICE_DEV)

void CItemMarket::GDReqItemStatus(SDHP_REQ_MARKET_STATUS* lpMsg, int uIndex)
{
	char szQuery[256];

	SDHP_ANS_MARKET_STATUS pMsg;

	memset(&pMsg, 0, sizeof(pMsg));

	pMsg.h.set(0xFE, 0x03, sizeof(pMsg));

	pMsg.Result = 1;

	pMsg.aIndex = lpMsg->aIndex;

#if(MARKET_DEBUG)

	while (true)
	{
		pMsg.Result = 1;

		sprintf_s(szQuery, "SELECT * FROM ItemMarketData WHERE Account = '%s' AND Status = 1", lpMsg->Account);

		if (!gQueryManager.ExecQuery(szQuery) || gQueryManager.Fetch() == SQL_NO_DATA)
		{
			pMsg.Result = 0;

			gQueryManager.Close();

			break;
		}
		else
		{
			int iID = gQueryManager.GetAsInteger("ID");

			pMsg.PriceType = gQueryManager.GetAsInteger("PriceType");

			pMsg.PriceValue = gQueryManager.GetAsInteger("PriceValue");

			gQueryManager.GetAsString("Account", pMsg.Account, sizeof(pMsg.Account));

			gQueryManager.Close();

			// --

			if (pMsg.Result)
			{
				sprintf_s(szQuery, "DELETE FROM ItemMarketData WHERE ID = %d", iID);

				if (!gQueryManager.ExecQuery(szQuery) || gQueryManager.Fetch() == SQL_NO_DATA)
				{
					pMsg.Result = 0;
				}

				gQueryManager.Close();
			}
		}

		gSocketManager.DataSend(uIndex, (BYTE*)&pMsg, sizeof(pMsg));
	}

#if(MARKET_ITEM_RECV)

	this->GDReqItemDate(uIndex, lpMsg->Account, lpMsg->aIndex);

#endif

#else

	sprintf_s(szQuery, "SELECT * FROM ItemMarketData WHERE Account = '%s' AND Status = 1", lpMsg->Account);

	if (!gQueryManager.ExecQuery(szQuery) || gQueryManager.Fetch() == SQL_NO_DATA)
	{
		pMsg.Result = 0;
	}
	else
	{
		int iID = gQueryManager.GetInt("ID");

		pMsg.PriceType = gQueryManager.GetInt("PriceType");

		pMsg.PriceValue = gQueryManager.GetInt("PriceValue");

		gQueryManager.GetStr("Account", pMsg.Account);

		gQueryManager.Close();

		// --

		if (pMsg.Result)
		{
			sprintf_s(szQuery, "DELETE FROM ItemMarketData WHERE ID = %d", iID);

			if (!gQueryManager.ExecQuery(szQuery) || gQueryManager.Fetch() == SQL_NO_DATA)
			{
				pMsg.Result = 0;
			}

			gQueryManager.Close();
		}
	}

	wsjServer.DataSend(uIndex, (char*)&pMsg, sizeof(pMsg));

#endif
}

#endif

#if(MARKET_PRICE_DEV)

void CItemMarket::GDReqItemListPage(SDHP_REQ_MARKET_ITEM* lpMsg, int uIndex)
{
	WORD iCount = 0;

	int iPageNumber = lpMsg->PageNumber;

	SDHP_ANS_MARKET_ITEM pMsg;

	memset(&pMsg, 0, sizeof(pMsg));

	pMsg.h.set(0xFE, 0x00, sizeof(pMsg));

	pMsg.Result = 1;

	pMsg.aIndex = lpMsg->aIndex;

	// --

	char szPriceType[256];

	memset(szPriceType, 0, sizeof(szPriceType));

	if (lpMsg->PriceType == eMarketPriceTypeZen)
	{
		sprintf_s(szPriceType, "AND PriceType = %d ", lpMsg->PriceType);
	}

	if (lpMsg->PriceType == eMarketPriceTypeWcoin)
	{
		sprintf_s(szPriceType, "AND PriceType = %d ", lpMsg->PriceType);
	}

	if (lpMsg->PriceType == eMarketPriceTypeCredit)
	{
		sprintf_s(szPriceType, "AND PriceType = %d ", lpMsg->PriceType);
	}

	// --

//#if(MARKET_FILTER_DEV)

	char szFilter[256];

	memset(szFilter, 0, sizeof(szFilter));

	if (lpMsg->Filter.FilterType < 16)
	{
		sprintf_s(szFilter, "AND FilterType = %d ", lpMsg->Filter.FilterType);
	}

	if (lpMsg->Filter.FilterLevel > 0 && lpMsg->Filter.FilterLevel <= 15)	//=(
	{
		char szLevel[256];

		memset(szLevel, 0, sizeof(szLevel));

		sprintf_s(szLevel, "AND FilterLevel >= %d AND FilterLevel <= 15 ", lpMsg->Filter.FilterLevel);

		strcat_s(szFilter, szLevel);
	}

	if (lpMsg->Filter.FilterLuck)
	{
		sprintf_s(szFilter, "AND FilterLuck = 1 ");
	}

	if (lpMsg->Filter.FilterExl)
	{
		sprintf_s(szFilter, "AND FilterExl = 1 ");
	}

	if (lpMsg->Filter.FilterAnc)
	{
		sprintf_s(szFilter, "AND FilterAnc = 1 ");
	}


	if (gQueryManager.ExecQuery("SELECT TOP %d ID, Account, Name, PriceType, PriceValue, Item FROM ItemMarketData WHERE Status = 0 %s %s ORDER BY ID DESC", MARKET_ITEM_MAX * 10, szFilter, szPriceType) != 0)
	{
		int iQueryCount = 0;
		
		while (gQueryManager.Fetch() != SQL_NO_DATA)
		{
			if (iQueryCount >= iPageNumber * MARKET_ITEM_MAX)
			{
				char szSeller[MARKET_NAME_LEN];

				//gQueryManager.GetStr("account", szSeller);
				gQueryManager.GetAsString("Name", szSeller, sizeof(szSeller));

				pMsg.Data[iCount].Name[MARKET_NAME_LEN - 1] = 0;

				memcpy(pMsg.Data[iCount].Name, szSeller, MARKET_NAME_LEN - 1);

				pMsg.Data[iCount].PriceType = gQueryManager.GetAsInteger("PriceType");

				pMsg.Data[iCount].Price = gQueryManager.GetAsInteger("PriceValue");

				// --

				//int id = gQueryManager.GetAsInteger("ID");

				gQueryManager.GetAsBinary("Item", pMsg.Data[iCount].Item, sizeof(pMsg.Data[iCount].Item));

				pMsg.Data[iCount].ID = gQueryManager.GetAsInteger("ID");

				/*if (this->gQueryItem.ExecQuery("SELECT Item FROM ItemMarketData WHERE ID = %d", id) == 0 || gQueryManager.Fetch() == SQL_NO_DATA)
				{
					this->gQueryItem.Close();

					memset(pMsg.Data[iCount].Item, 0xFF, sizeof(pMsg.Data[iCount].Item));
				}
				else
				{
					this->gQueryItem.GetAsBinary("Item", pMsg.Data[iCount].Item, sizeof(pMsg.Data[iCount].Item));

					this->gQueryItem.Close();
				}*/

				// --

				iCount++;
				
				if (iCount >= MARKET_ITEM_MAX)
				{
					break;
				}
			}

			iQueryCount++;
		}
	}

	gQueryManager.Close();

	pMsg.count = iCount;
//
//	sprintf_s(szQuery, "SELECT TOP %d ID, Account, Name, PriceType, PriceValue FROM ItemMarketData WHERE Status = 0 %s %s ORDER BY ID DESC", MARKET_ITEM_MAX * 10, szFilter, szPriceType);
//
//#else
//
//	sprintf_s(szQuery, "SELECT TOP %d ID, Account, PriceType, PriceValue FROM ItemMarketData WHERE Status = 0 ORDER BY ID DESC", MARKET_ITEM_MAX); // <<== Arg
//
//#endif
//
//	if (!gQueryManager.ExecQuery(szQuery))
//	{
//		gQueryManager.Close();
//
//		pMsg.Result = 0;
//	}
//	else
//	{
//		int iQueryCount = 0;
//
//		short sqlReturn = gQueryManager.Fetch();
//
//		while (sqlReturn != SQL_NO_DATA && sqlReturn != SQL_NULL_DATA)
//		{
//			LogAdd(LOG_BLUE, "WHILE");
//			iReturnCode = gQueryManager.GetResult(1);
//
//			if (iReturnCode < 0)
//			{
//				LogAdd(LOG_BLUE, "return 1");
//				break;
//			}
//
//			if (iQueryCount >= iPageNumber * MARKET_ITEM_MAX)
//			{
//				char szSeller[MARKET_NAME_LEN];
//
//				//gQueryManager.GetStr("account", szSeller);
//				gQueryManager.GetAsString("Name", szSeller, sizeof(szSeller));
//
//				pMsg.Data[iCount].Name[MARKET_NAME_LEN - 1] = 0;
//
//				memcpy(pMsg.Data[iCount].Name, szSeller, MARKET_NAME_LEN - 1);
//
//				pMsg.Data[iCount].PriceType = gQueryManager.GetAsInteger("PriceType");
//
//				pMsg.Data[iCount].Price = gQueryManager.GetAsInteger("PriceValue");
//
//				// --
//
//				int id = gQueryManager.GetAsInteger("ID");
//
//				pMsg.Data[iCount].ID = id;
//
//				//sprintf_s(szQuery, "SELECT Item FROM ItemMarketData WHERE ID = %d", id);
//
//				/*int	nRet = this->m_DBItem.ReadBlob(szQuery, pMsg.Data[iCount].Item);
//
//				if (nRet < 0)
//				{
//					memset(pMsg.Data[iCount].Item, -1, sizeof(pMsg.Data[iCount].Item));
//				}*/
//
//				if (this->gQueryItem.ExecQuery("SELECT Item FROM ItemMarketData WHERE ID = %d", id) == 0 || gQueryManager.Fetch() == SQL_NO_DATA)
//				{
//					this->gQueryItem.Close();
//
//					memset(pMsg.Data[iCount].Item, 0xFF, sizeof(pMsg.Data[iCount].Item));
//				}
//				else
//				{
//					this->gQueryItem.GetAsBinary("Item", pMsg.Data[iCount].Item, sizeof(pMsg.Data[iCount].Item));
//
//					this->gQueryItem.Close();
//				}
//
//				// --
//
//				iCount++;
//
//				if (iCount >= MARKET_ITEM_MAX)
//				{
//					break;
//				}
//			}
//
//			sqlReturn = gQueryManager.Fetch();
//
//			iQueryCount++;
//		}
//
//		gQueryManager.Close();
//	}

	gSocketManager.DataSend(uIndex, (BYTE*)&pMsg, sizeof(pMsg));

	//LogAddC(2, "[DATA -> SERVER] : GDReqItemList");
}

#endif

#if(MARKET_ITEM_RECV)

CTime CItemMarket::StrToCTime(const char* str)
{
	int year, month, day, hh, mm, ss = 0;

	sscanf_s(str, "%02d.%02d.%04d-%02d:%02d", &day, &month, &year, &hh, &mm);

	return {year, month, day, hh, mm, ss};
}

void CItemMarket::RunItemDate()
{
	char szQuery[256] = { 0 };

	int iItemMaxTime = 60;	//86400

	while (true)
	{
		bool bResult = false;

		sprintf_s(szQuery, "SELECT * FROM ItemMarketData WHERE Status = 0");

		if (!gQueryManager.ExecQuery(szQuery) || gQueryManager.Fetch() == SQL_NO_DATA)
		{
			gQueryManager.Close();

			break;
		}
		else
		{
			int iID = gQueryManager.GetAsInteger("ID");

			// --
			char szDate1[20] = { 0 };

			char szDate2[20] = { 0 };

			SYSTEMTIME st;

			GetLocalTime(&st);

			sprintf_s(szDate1, "%02d.%02d.%04d-%02d:%02d", st.wDay, st.wMonth, st.wYear, st.wHour, st.wMinute);

			gQueryManager.GetAsString("Date", szDate2, sizeof(szDate2));

			CTime ct1 = this->StrToCTime(szDate1);

			CTime ct2 = this->StrToCTime(szDate2);

			auto dwDate1 = ct1.GetTime();

			auto dwDate2 = ct2.GetTime();

			if (dwDate1 + iItemMaxTime > dwDate2)
			{
				bResult = true;
			}

			// --

			gQueryManager.Close();

			if (bResult)
			{
				sprintf_s(szQuery, "UPDATE ItemMarketData SET Status = 2 WHERE ID = %d", iID);

				gQueryManager.ExecQuery(szQuery);

				gQueryManager.Close();
			}
		}
	}
}

void CItemMarket::GDReqItemDate(int uIndex, char* Account, int aIndex)
{
	char szQuery[256] = { 0 };

	SDHP_REQ_MARKET_ITEMRECV pMsg;

	memset(&pMsg, 0, sizeof(pMsg));

	pMsg.h.set(0xFE, 0x04, sizeof(pMsg));

	pMsg.Result = 0;

	pMsg.aIndex = aIndex;

	sprintf_s(szQuery, "SELECT * FROM ItemMarketData WHERE Account = '%s' AND Status = 2", Account);

	if (!gQueryManager.ExecQuery(szQuery))
	{
		gQueryManager.Close();
	}
	else
	{
		short sqlReturn = gQueryManager.Fetch();

		while (sqlReturn != SQL_NO_DATA && sqlReturn != SQL_NULL_DATA)
		{
			int iReturnCode = gQueryManager.GetResult(1);

			if (iReturnCode < 0)
			{
				break;
			}

			pMsg.Result = 1;

			int iID = gQueryManager.GetAsInteger("ID");

			gQueryManager.GetAsBinary("Item", pMsg.Item, sizeof(pMsg.Item));

			pMsg.ID = iID;

			/*sprintf_s(szQuery, "SELECT Item FROM ItemMarketData WHERE ID = %d", iID);

			int	nRet = this->m_DBItem.ReadBlob(szQuery, pMsg.Item);

			if (nRet < 0)
			{
				pMsg.Result = 0;
			}*/

			if (pMsg.Result)
			{
				gSocketManager.DataSend(uIndex, (BYTE*)&pMsg, sizeof(pMsg));
			}

			sqlReturn = gQueryManager.Fetch();
		}

		gQueryManager.Close();
	}

	//while(true)
	//{
	//	

	//	if(!gQueryManager.ExecQuery(szQuery) || gQueryManager.Fetch() == SQL_NO_DATA)
	//	{
	//		gQueryManager.Close();

	//		break;
	//	}
	//	else
	//	{
	//		pMsg.Result = 1;

	//		int iID = gQueryManager.GetInt("ID");	

	//		pMsg.ID = iID;

	//		sprintf_s(szQuery, "SELECT Item FROM ItemMarketData WHERE ID = %d", iID);

	//		int	nRet = this->m_DBItem.ReadBlob(szQuery, pMsg.Item);

	//		if(nRet < 0)
	//		{
	//			pMsg.Result = 0;
	//		}

	//		gQueryManager.Close();
	//
	//		if(pMsg.Result)
	//		{
	//			wsjServer.DataSend(uIndex, (char*)&pMsg, sizeof(pMsg));
	//		}
	//	}		
	//}
}

void CItemMarket::GDAnsItemDate(SDHP_ANS_MARKET_ITEMRECV* lpMsg, int uIndex)
{
	char szQuery[256];

	sprintf_s(szQuery, "DELETE FROM ItemMarketData WHERE ID = %d", lpMsg->ID);

	gQueryManager.ExecQuery(szQuery);

	gQueryManager.Fetch();

	gQueryManager.Close();
}

#endif

#endif