#include "stdafx.h"
#include "DiabloDBSet.h"
#include "QueryManager.h"
#include "Util.h"

CDiabloDBSet gDiabloDBSet;

CDiabloDBSet::CDiabloDBSet()
{

}

CDiabloDBSet::~CDiabloDBSet()
{
	
}

BOOL CDiabloDBSet::DSDB_GetGuildList(int aIndex)
{
	int COUNT = 0;

	if (gQueryManager.ExecQuery("EXEC WZ_Diablo_GetGuildList") == FALSE)
	{
		gQueryManager.Close();
		return TRUE;
	}

	DG_DIABLO_GUILDLIST pMsg;

	pMsg.header.set(0xFB,0x00,sizeof(pMsg));

	for (int i = 0; i < MAX_GUILDCOUNT; i++)
	{
		memset(pMsg.GUILD_NAME[i],0,sizeof(pMsg.GUILD_NAME[i]));
		pMsg.RENA_COUNT[i] = 0;
	}

	short sqlRet = gQueryManager.Fetch();

	while(sqlRet != SQL_NO_DATA && sqlRet != SQL_NULL_DATA)
	{
		if (COUNT > MAX_GUILDCOUNT)
		{
			break;
		}
		
		char szGuildName[8+8] = {'\0'};
		
		gQueryManager.GetAsString("GUILD_NAME", szGuildName,sizeof(szGuildName));
		
		memcpy(pMsg.GUILD_NAME[COUNT],szGuildName,sizeof(pMsg.GUILD_NAME[COUNT]));
		pMsg.RENA_COUNT[COUNT] = gQueryManager.GetAsInteger("RENA_COUNT");

		COUNT++;
		
		sqlRet = gQueryManager.Fetch();
	}

	if (sqlRet == SQL_NULL_DATA)
	{
		gQueryManager.Close();
		return TRUE;
	}
	
	gQueryManager.Close();

	if(gQueryManager.ExecQuery("SELECT * FROM MuDiablo_DATA") == 0 || gQueryManager.Fetch() == SQL_NO_DATA)
	{
		gQueryManager.Close();

		pMsg.Time.StartRegYear = 0;
		pMsg.Time.StartRegMonth = 0;
		pMsg.Time.StartRegDay = 0;

		pMsg.Time.EndRegYear = 0;
		pMsg.Time.EndRegMonth = 0;
		pMsg.Time.EndRegDay = 0;

		pMsg.Time.StartYear = 0;
		pMsg.Time.StartMonth = 0;
		pMsg.Time.StartDay = 0;

		pMsg.Time.Result = 1;
	}
	else
	{
		//pMsg.ItemCount = gQueryManager.GetAsInteger("ItemCount");
		pMsg.Time.StartRegYear = gQueryManager.GetAsInteger("StartRegYear");
		pMsg.Time.StartRegMonth = gQueryManager.GetAsInteger("StartRegMonth");
		pMsg.Time.StartRegDay = gQueryManager.GetAsInteger("StartRegDay");

		pMsg.Time.EndRegYear = gQueryManager.GetAsInteger("EndRegYear");
		pMsg.Time.EndRegMonth = gQueryManager.GetAsInteger("EndRegMonth");
		pMsg.Time.EndRegDay = gQueryManager.GetAsInteger("EndRegDay");

		pMsg.Time.StartYear = gQueryManager.GetAsInteger("StartYear");
		pMsg.Time.StartMonth = gQueryManager.GetAsInteger("StartMonth");
		pMsg.Time.StartDay = gQueryManager.GetAsInteger("StartDay");

		pMsg.Time.Result = 0;

		gQueryManager.Close();
	}

	pMsg.Count = COUNT;



	gSocketManager.DataSend(aIndex,(BYTE*)&pMsg,sizeof(pMsg));

	return FALSE;
}

BOOL CDiabloDBSet::DSDB_SaveGuildList(GD_DIABLO_GUILDLIST* aRecv)
{
	if (aRecv->Count <= 0)
	{
		return FALSE;
	}
	if (gQueryManager.ExecQuery("DELETE FROM MuDiablo_GUILDLIST") == FALSE)
	{
		gQueryManager.Close();
		return TRUE;
	}

	for (int i = 0; i < aRecv->Count; i++)
	{
		if (gQueryManager.ExecQuery("INSERT INTO MuDiablo_GUILDLIST (GUILD_NAME,RENA_COUNT) VALUES ('%s',%d)", aRecv->GUILD_NAME[i], aRecv->RENA_COUNT[i]) == FALSE)
		{
			gQueryManager.Close();
			return TRUE;
		}
	}

	gQueryManager.Close();
	return FALSE;

}

BOOL CDiabloDBSet::DSDB_ChangeDate(GD_DIABLO_CHANGEDATE* aRecv)
{
	if (gQueryManager.ExecQuery("EXEC LTP_Diablo_ChangeDate %d, %d, %d, %d, %d, %d, %d, %d, %d", aRecv->DayStartReg, aRecv->MonthStartReg, aRecv->YearStartReg, aRecv->DayEndReg, aRecv->MonthEndReg, aRecv->YearEndReg, aRecv->DayStartEvent, aRecv->MonthStartEvent, aRecv->YearStartEvent) == FALSE)
	{
		gQueryManager.Close();
		return TRUE;
	}
}