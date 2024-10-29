#pragma once
// -------------------------------------------------------------------------------

//#include "..\include\prodef.h"
#include "Protocol.h"
#include "DefaultClassInfo.h"
// -------------------------------------------------------------------------------

#define	MAX_RESET_GROUP		500
#define MAX_RESET_ITEM		5
// -------------------------------------------------------------------------

struct RESET_REQ_USERDATA
{
	PSBMSG_HEAD h;
};
// -------------------------------------------------------------------------------

struct RESET_ITEM_DATA
{
	WORD	ID;
	BYTE	MinLevel;
	BYTE	MaxLevel;
	BYTE	MinOption;
	BYTE	MaxOption;
	BYTE	IsLuck;
	BYTE	IsSkill;
	BYTE	IsExcellent;
	BYTE	IsAncient;
	BYTE	IsSocket;
	int		Count;
};

struct RESET_ITEM_REWARD_ITEM
{
	WORD	ID;
	BYTE	MinLevel;
	BYTE	MaxLevel;
	BYTE	MinOption;
	BYTE	MaxOption;
	BYTE	IsLuck;
	BYTE	IsSkill;
	BYTE	ExcType;
	BYTE	MinExcOption;
	BYTE	MaxExcOption;
	BYTE	ExcCode;
	BYTE	IsAncient;
	BYTE	IsSocket;
	int		Count;
};

struct RESET_ITEM_REWARD
{
	int		Money;
	int		WCoinC;
	int		WCoinP;
	int		WCoinG;
	int		Credits;
	int		ItemCount;
	RESET_ITEM_REWARD_ITEM itemData[5];
};
// -------------------------------------------------------------------------

// -------------------------------------------------------------------------------

struct RESET_ITEM_REWARD_DATA
{
	int MinGReset;
	int MaxGReset;
	int MinReset;
	int MaxReset;
	RESET_ITEM_REWARD ItemReward[MAX_CLASS];
};

struct RESET_REWARD_DATA
{
	int		LevelPoint;
	int		GensPoint;
	int		WCoinC;
	int		WCoinP;
	int		GoblinPoint;
	int		Credits;
};
// -------------------------------------------------------------------------

struct RESET_GROUP_DATA
{
	int		MinReset;
	int		MaxReset;
	int		MinGReset;
	int		MaxGReset;
	int		ReqLevel[MAX_ACCOUNT_LEVEL];
	int		ReqMoney[MAX_ACCOUNT_LEVEL];
	int		ItemCount;
	//int		ItemRewardCount;
	RESET_ITEM_DATA		ItemData[MAX_RESET_ITEM];
	//RESET_ITEM_DATA		ItemRewardData[MAX_RESET_ITEM];
	RESET_REWARD_DATA	RewardData[MAX_CLASS];
};

struct RESET_CURRENT_DATA
{
	int MinReset;
	int MaxReset;
	int ReqLevel;
	int ItemCount;
	bool UseItemReward;
	RESET_ITEM_DATA		ItemData[MAX_RESET_ITEM];
	RESET_ITEM_REWARD	ItemReward;
	RESET_REWARD_DATA RewardData;
};

struct RESET_ANS_USERDATA
{
	PSWMSG_HEAD h;
	bool		ByDialog;
	int			Reset;
	int			GrandReset;
	int			ResetMoney;
	int			CommandPerReset;
	int			QuestStat;
	RESET_CURRENT_DATA ResetData;
	bool		ItemCheck[MAX_RESET_ITEM];
};
// -------------------------------------------------------------------------

namespace ResetProtocol
{
	enum T
	{
		UserData	= 0x0A,
		ResetFinish	= 0x0B,
		Case		= 0xFB,
	};
};
// -------------------------------------------------------------------------------