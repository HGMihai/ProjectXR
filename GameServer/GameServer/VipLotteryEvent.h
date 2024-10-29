#pragma once

#define MAX_VIP_LOTTERY_DROP 10

enum eCustomVipLotteryState
{
	CUSTOM_VIP_LOTTERY_STATE_BLANK = 0,
	CUSTOM_VIP_LOTTERY_STATE_EMPTY = 1,
	CUSTOM_VIP_LOTTERY_STATE_START = 2,
};

struct CUSTOM_VIP_LOTTERY_START_TIME
{
	int Year;
	int Month;
	int Day;
	int DayOfWeek;
	int Hour;
	int Minute;
	int Second;
};

struct CUSTOM_VIP_LOTTERY_ITEM_INFO
{
	int AccountLevel;
	int Count;
	int Rate;
};

struct CUSTOM_VIP_LOTTERY_RULE_INFO
{
	chr Name[32];
	int Map;
	int MinLevel;
	int MaxLevel;
	int MinReset;
	int MaxReset;
	int MinGReset;
	int MaxGReset;
	int AlarmTime;
	std::vector<CUSTOM_VIP_LOTTERY_ITEM_INFO> DropItem;
};

struct CUSTOM_VIP_LOTTERY_INFO
{
	int Index;
	int State;
	int RemainTime;
	int TargetTime;
	int TickCount;
	int AlarmMinSave;
	int AlarmMinLeft;
	CUSTOM_VIP_LOTTERY_RULE_INFO RuleInfo;
	std::vector<CUSTOM_VIP_LOTTERY_START_TIME> StartTime;
};

class cVipLottery
{
public:
	cVipLottery();
	virtual ~cVipLottery();
	void Init();
	void Load(char* path);
	void MainProc();
	void ProcState_BLANK(CUSTOM_VIP_LOTTERY_INFO* lpInfo);
	void ProcState_EMPTY(CUSTOM_VIP_LOTTERY_INFO* lpInfo);
	void ProcState_START(CUSTOM_VIP_LOTTERY_INFO* lpInfo);
	void SetState(CUSTOM_VIP_LOTTERY_INFO* lpInfo,int state);
	void SetState_BLANK(CUSTOM_VIP_LOTTERY_INFO* lpInfo);
	void SetState_EMPTY(CUSTOM_VIP_LOTTERY_INFO* lpInfo);
	void SetState_START(CUSTOM_VIP_LOTTERY_INFO* lpInfo);
	void CheckSync(CUSTOM_VIP_LOTTERY_INFO* lpInfo);
private:
	int m_Enable;
	CUSTOM_VIP_LOTTERY_INFO m_CustomVipLotteryInfo[MAX_VIP_LOTTERY_DROP];
}; extern cVipLottery gVipLottery;