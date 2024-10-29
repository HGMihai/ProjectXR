#pragma once

#include "Protocol.h"
#include "DefaultClassInfo.h"

#define	MAX_GR_GROUP	500
#define MAX_GR_ITEM		5


struct GR_REQ_USERDATA
{
	PSBMSG_HEAD h;
};
// -------------------------------------------------------------------------------

struct GR_ITEM_DATA
{
	WORD	ID;
	BYTE	MinLevel;
	BYTE	MaxLevel;
	BYTE	MinOption;
	BYTE	MaxOption;
	BYTE	IsLuck;
	BYTE	IsSkill;
	bool	IsExcellent;
	bool	IsAncient;
	bool	IsSocket;
};
// -------------------------------------------------------------------------

struct GR_REWARD_DATA
{
	int		LevelPoint;
	int		GensPoint;
	int		WCoinC;
	int		WCoinP;
	int		GoblinPoint;
	int		Credits;
};
// -------------------------------------------------------------------------

struct GR_GROUP_DATA
{
	int		MinGReset;
	int		MaxGReset;
	int		ReqReset[MAX_ACCOUNT_LEVEL];
	int		ReqLevel;
	int		ReqMoney[MAX_ACCOUNT_LEVEL];
	int		ItemCount;
	GR_ITEM_DATA		ItemData[MAX_GR_ITEM];
	GR_REWARD_DATA		RewardData[MAX_CLASS];
};

struct GR_CURRENT_DATA
{
	int MinGReset;
	int MaxGReset;
	int ReqLevel;
	int ReqReset;
	int ItemCount;
	GR_ITEM_DATA		ItemData[MAX_GR_ITEM];
	GR_REWARD_DATA RewardData;
};

// -------------------------------------------------------------------------------

struct GR_ANS_USERDATA
{
	PSBMSG_HEAD h;
	bool		ByDialog;
	int			Reset;
	int			GrandReset;
	int			GResetMoney;
	int			CommandPerGReset;
	int			QuestStat;
	GR_CURRENT_DATA GrData;
	bool		ItemCheck[MAX_GR_ITEM];
};

namespace GRProtocol
{
	enum T
	{
		UserData	= 0x0E,
		ResetFinish	= 0x0F,
		Case		= 0xFB,
	};
};

class GRSystem
{
public:
	GRSystem();
	virtual ~GRSystem();
	// ----
	void Init();
	void Load();
	void ReadGroupFile(char* path);
	void ReadMainFile(char* path);
	// ----
	int GetResetMoney(LPOBJ lpObj);
	int GetGRGroup(LPOBJ lpObj);
	// ----
	bool	SearchQuestItem(LPOBJ lpObj, BYTE QuestItemID);
	bool	DeleteQuestItem(LPOBJ lpObj, BYTE QuestItemID);
	bool	CheckAllReq(LPOBJ lpObj);
	// ----
	bool	Dialog(LPOBJ lpObj, LPOBJ lpNpc);
	// ----
	void	SendResetData(LPOBJ lpObj, bool ByDialog);
	void	FinishResetReq(LPOBJ lpObj);
	int		GetGRPoints(LPOBJ lpObj);
private:
	int		m_GroupLoadedCount;
	int		m_MaxMoney;
	bool	m_ReqCleanInventory;
	bool	m_ResetStats;
	bool	m_ResetPoints;
	WORD	m_NpcID;
	BYTE	m_NpcMap;
	BYTE	m_NpcX;
	BYTE	m_NpcY;
	GR_GROUP_DATA	m_GroupData[MAX_GR_GROUP];
	// ----
	bool	m_ResetSkill;
	bool	m_ResetMasterLevel;
	bool	m_ResetMasterStats;
	bool	m_ResetMasterSKill;
public:
	int		m_BonusCommand;
}; extern GRSystem gGRSystem;