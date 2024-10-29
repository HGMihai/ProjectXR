#pragma once
// -------------------------------------------------------------------------

// -------------------------------------------------------------------------

#include "ResetSystemDefine.h"
// -------------------------------------------------------------------------

class ResetSystem
{
public:
			ResetSystem();
	virtual ~ResetSystem();
	// ----
	void	Init();
	void	Load();
	void	ReadGroupData(char * File);
	void	ReadMainData(char * File);
	void	ReadItemRewardData(char* path);

	// ----
	int		GetResetMoney(LPOBJ lpUser);
	int		GetResetGroup(LPOBJ lpUser);
	RESET_ITEM_REWARD*	GetResetReward(LPOBJ lpObj);

	bool	SearchQuestItem(LPOBJ lpUser, BYTE QuestItemID);
	bool	DeleteQuestItem(LPOBJ lpUser, BYTE QuestItemID);
	bool	CheckAllReq(LPOBJ lpUser);
	// ----
	bool	Dialog(LPOBJ lpUser, LPOBJ lpNpc);
	// ----
	void	SendResetData(LPOBJ lpUser, bool ByDialog);
	void	FinishResetReq(LPOBJ lpUser);
	int		GetResetPoints(LPOBJ lpObj);
	// ----
	bool	m_ResetStats;
private:
	int		m_GroupLoadedCount;
	int		m_MaxMoney;
	bool	m_MultiplyToReset;
	bool	m_AddGResetToGroup;
	bool	m_ReqCleanInventory;
	bool	m_ResetPoints;
	WORD	m_NpcID;
	BYTE	m_NpcMap;
	BYTE	m_NpcX;
	BYTE	m_NpcY;
	RESET_GROUP_DATA	m_GroupData[MAX_RESET_GROUP];
	std::vector<RESET_ITEM_REWARD_DATA> m_ItemRewardInfo;
	// ----
	bool	m_ResetSkill;
	bool	m_ResetMasterLevel;
	bool	m_ResetMasterStats;
	bool	m_ResetMasterSKill;
public:
	bool	m_ShopCleaning;
	bool	m_MarlonReset;
	WORD	m_MarlonStatMinLevel;
	WORD	m_MarlonComboMinLevel;
	int		m_BonusCommand;
	// ----
}; extern ResetSystem g_ResetSystem;
// -------------------------------------------------------------------------

// -------------------------------------------------------------------------