#pragma once

#include "User.h"
#include "Protocol.h"

#define MAX_DIABLOGUILD	100
#define MAX_DIABLO_USERS 100

enum eDiabloDialog
{
	DIALOG_UPDATE = 0,
	DIALOG_SEND = 1,
};

enum eDiabloState
{
	DIABLO_STATE_BLANK = 0,
	DIABLO_STATE_EMPTY = 1,
	DIABLO_STATE_STAND = 2,
	DIABLO_STATE_START = 3,
	DIABLO_STATE_CLEAN = 4,
};

struct DIABLO_USER
{
	void Reset() // OK
	{
		this->Index = -1;
		this->OnMap = 0;
		this->Time = 0;
		this->HitTick = 0;
		this->DontMoveTick = 0;
	}
	int Index;
	int OnMap;
	int Time;
	int HitTick;
	int DontMoveTick;
};

struct DIABLO_INFO
{	
	int UpdateTick;
	int State;
	int RemainTime;
	int TargetTime;
	int AlarmMinSave;
	int AlarmMinLeft;
	int EnterEnabled;
	int TickCount;
	int EventTime;
	//int UsersCount;
	DIABLO_USER m_Users[MAX_DIABLO_USERS];
};

struct DIABLO_TIMEINFO
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
	int StartHour;
	int StartMinutes;
};

struct DIABLO_REQ
{
	PWMSG_HEAD	Head;
	// ----
	bool Type;
	int Count;
	int StartYear;
	int StartMonth;
	int StartDay;
	int EndYear;
	int EndMonth;
	int EndDay;
	int StartEventYear;
	int StartEventMonth;
	int StartEventDay;
	char StartTime[20];
	char GuildName[MAX_DIABLOGUILD][11];
	int GuildRank[MAX_DIABLOGUILD];
	int GuildCount;
};

struct DIABLO_RENARECV
{
	PSBMSG_HEAD	Head;
	// ----
	int Count;
};


struct REGISTERED_GUILD_INFO
{
	char GuildName[11];
	int RegisteredRena;

	bool operator<(const REGISTERED_GUILD_INFO& a) const
    {
        return RegisteredRena > a.RegisteredRena;
    }
};

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
	char GUILD_NAME[MAX_DIABLOGUILD][8];
	int RENA_COUNT[MAX_DIABLOGUILD];
	int Count;
	DG_DIABLO_TIMESETTING Time;
};

struct GD_DIABLO_GUILDLIST
{
	PWMSG_HEAD header;
	char GUILD_NAME[MAX_DIABLOGUILD][8];
	int RENA_COUNT[MAX_DIABLOGUILD];
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

class cDiablo
{
public:
	cDiablo();
	virtual ~cDiablo();
	void Init();
	void Load();
	void LoadIni(char* path);
	bool NPCTalk(LPOBJ lpNpc, LPOBJ lpObj);
	void SendDialog(LPOBJ lpObj, eDiabloDialog type);
	void UpdateRena(int aIndex);
	void RegisterRena(DIABLO_RENARECV* lpMsg, int aIndex);
	void Teleport(DIABLO_RENARECV* lpMsg, int aIndex);
	void AddRena(int Count, int aIndex);
	int GetRegCountByUserIndex(int index);
	bool CreateDiablo();
	void DeleteDiablo();
	void DiabloDie(int index);
	void DiabloMonsterDieProc(LPOBJ lpObj, LPOBJ lpTarget);
	bool GetUserRespawnLocation(LPOBJ lpObj,int* gate,int* map,int* x,int* y,int* dir,int* level);
	void GetHit(LPOBJ lpObj, LPOBJ lpTarget);
	void GetMove(LPOBJ lpObj);
	void MainProc();
	void ProcState_BLANK(DIABLO_INFO* lpInfo);
	void ProcState_EMPTY(DIABLO_INFO* lpInfo);
	void ProcState_STAND(DIABLO_INFO* lpInfo);
	void ProcState_START(DIABLO_INFO* lpInfo);
	void ProcState_CLEAN(DIABLO_INFO* lpInfo);
	void SetState(DIABLO_INFO* lpInfo,int state);
	void SetState_BLANK(DIABLO_INFO* lpInfo);
	void SetState_EMPTY(DIABLO_INFO* lpInfo);
	void SetState_STAND(DIABLO_INFO* lpInfo);
	void SetState_START(DIABLO_INFO* lpInfo);
	void SetState_CLEAN(DIABLO_INFO* lpInfo);
	void CheckSync(DIABLO_INFO* lpInfo);
	void GDUpdateInfo();
	void GDChangeDate();
	// ----
	void SendGetGuildInfo();
	void CheckUser(DIABLO_INFO* lpInfo);
	bool DelUser(DIABLO_INFO* lpInfo,int aIndex);
	DIABLO_USER* GetUser(DIABLO_INFO* lpInfo,int aIndex);
	int GetUserCount(DIABLO_INFO* lpInfo);
	bool AddUser(DIABLO_INFO* lpInfo,int aIndex);
	void ClearUser(DIABLO_INFO* lpInfo);
	void DGGetGuildList(DG_DIABLO_GUILDLIST* aRecv);
	// ----
	void NoticeSendToAll(DIABLO_INFO* lpInfo,int type,char* message,...);
	// ----
	int m_DiabloSwitch;
	int m_NpcID;
	int m_NpcMap;
	int m_NpcX;
	int m_NpcY;
	int m_AlarmTime;
	int m_StandTime;
	int m_EventTime;
	int m_CloseTime;
	int m_PassiveTime;
	int m_WCoinCPrize;
	int m_WCoinPPrize;
	int m_PrizeType;
	char m_InfoText[1000];
	int m_ChangeStartReg;
	int m_ChangeEndReg;
	int m_ChangeStartEvent;
	// ----
	int m_DBDataLoadOK;
	int m_DBDataLoading;
	// ----
	int m_DiabloObjIndex;
	// ----
	//int DiabloState;
	std::vector<REGISTERED_GUILD_INFO> m_RegStruct;
	//std::vector<DIABLO_INFO> m_Info;
	DIABLO_INFO m_DiabloInfo;
	DIABLO_TIMEINFO TimeInfo;
}; extern cDiablo gDiablo;