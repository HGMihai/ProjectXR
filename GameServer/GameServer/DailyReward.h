#pragma once

#include "Protocol.h"

//Type 0 - Item
//Type 1 - Zen
//Type 2 - Credits
//Type 3 - WCoinC
//Type 4 - WCoinP
//Type 5 - Goblin Points
//Type 6 - Points

struct DAILY_REWARD_DATA
{
	int Index;
	int Type;
	int ItemIndex;
	int ItemLevel;
	int ItemLuck;
	int ItemSkill;
	int ItemOption;
	int ItemExc;
	int ItemAnc;
	int Count;
	int PointsType;
};

struct PMSG_SEND_DAILYREWARD
{
	PSWMSG_HEAD header;
	int Count;
};

struct PMSG_SEND_REWARDINFO
{
	PSBMSG_HEAD	Head;
	int Index;
};

class CDailyReward
{
public:
	CDailyReward();
	virtual ~CDailyReward();
	void InitStruct(DAILY_REWARD_DATA* Data);
	void Load(char* path);
	void SendInfo(int aIndex);
	void SendRewardInfo(int aIndex);
	void MakeReward(int aIndex, int rewardIndex);
	static void Update(int aIndex);
private:
	std::vector<DAILY_REWARD_DATA> m_RewardData;
}; extern CDailyReward gDailyReward;