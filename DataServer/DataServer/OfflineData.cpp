#include "stdafx.h"
#include "OfflineData.h"
#include "QueryManager.h"
#include "SocketManager.h"

cOfflineData gOfflineData;

cOfflineData::cOfflineData()
{

}

cOfflineData::~cOfflineData()
{
	
}

void cOfflineData::OfflineDataRecv(GD_OFFLINE_DATA_RECV* lpMsg, int aIndex)
{
	if (gQueryManager.ExecQuery("SELECT * from OfflineData where AccountID = '%s'",lpMsg->AccountID) == 0 || gQueryManager.Fetch() == SQL_NO_DATA)
	{
		gQueryManager.Close();
		gQueryManager.ExecQuery("INSERT INTO OfflineData (AccountID,Name,Type) VALUES ('%s','%s',%d)",lpMsg->AccountID,lpMsg->Name,lpMsg->Type);
		gQueryManager.Close();
	}
}

void cOfflineData::OfflineDelRecv(GD_OFFLINE_DATA_DEL_RECV* lpMsg, int aIndex)
{
	gQueryManager.ExecQuery("Delete from OfflineData where AccountID = '%s'",lpMsg->AccountID);
	gQueryManager.Close();
}
