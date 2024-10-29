#pragma once

#include "DataServerProtocol.h"

#define MAX_GUILDCOUNT 100

struct DG_DIABLO_TIMESETTING
{
	int StartRegYear;
	int StartRegMonth;
	int StartRegDay;
	int EndRegYear;
	int EndRegMonth;
	int EndRegDay;
	int StartYear;
	int StartMonth;
	int StartDay;
	int Result;
};

struct DG_DIABLO_GUILDLIST
{
	PSBMSG_HEAD header;
	char GUILD_NAME[MAX_GUILDCOUNT][8];
	int RENA_COUNT[MAX_GUILDCOUNT];
	int Count;
	DG_DIABLO_TIMESETTING Time;
};

struct GD_DIABLO_GUILDLIST
{
	PWMSG_HEAD header;
	char GUILD_NAME[MAX_GUILDCOUNT][8];
	int RENA_COUNT[MAX_GUILDCOUNT];
	int Count;
};

struct GD_DIABLO_CHANGEDATE
{
	PSBMSG_HEAD	Head;
	int DayStartReg;
	int MonthStartReg;
	int YearStartReg;
	int DayEndReg;
	int MonthEndReg;
	int YearEndReg;
	int DayStartEvent;
	int MonthStartEvent;
	int YearStartEvent;
};

class CDiabloDBSet
{
public:
	CDiabloDBSet();
	virtual ~CDiabloDBSet();
	// ----
	BOOL DSDB_GetGuildList(int aIndex);
	BOOL DSDB_SaveGuildList(GD_DIABLO_GUILDLIST* aRecv);
	BOOL DSDB_ChangeDate(GD_DIABLO_CHANGEDATE* aRecv);

}; extern CDiabloDBSet gDiabloDBSet;