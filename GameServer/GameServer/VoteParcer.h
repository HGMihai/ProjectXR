#pragma once

#include "User.h"

struct PARCER_DATA
{
	int Id;
	char Date[12];
	char Time[12];
	char IP[16];
	char Account[32];
	int Type;
};

class CVoteParcer
{
public:
	CVoteParcer();
	virtual ~CVoteParcer();
	void Init();
	void Load(char* path);
	void LoadFile(char* link, char* path);
	void LoadVotersFile(char* path);
	void CheckVote();
	bool CheckVoter(std::vector<PARCER_DATA>::iterator it);
	void AddVoteToFile(std::vector<PARCER_DATA>::iterator it);
public:
	int m_MmoTopUse;
	char m_MmmoTopLink[100];
	int m_Credits[MAX_ACCOUNT_LEVEL];
	int m_WCoinC[MAX_ACCOUNT_LEVEL];
	int m_WCoinP[MAX_ACCOUNT_LEVEL];
	int m_WCoinG[MAX_ACCOUNT_LEVEL];
	int m_ItemActive[MAX_ACCOUNT_LEVEL];
	int m_ItemType[MAX_ACCOUNT_LEVEL];
	int m_ItemIndex[MAX_ACCOUNT_LEVEL];
	int m_ItemLevel[MAX_ACCOUNT_LEVEL];

	int m_CreditsSms[MAX_ACCOUNT_LEVEL];
	int m_WCoinCSms[MAX_ACCOUNT_LEVEL];
	int m_WCoinPSms[MAX_ACCOUNT_LEVEL];
	int m_WCoinGSms[MAX_ACCOUNT_LEVEL];
	int m_ItemActiveSms[MAX_ACCOUNT_LEVEL];
	int m_ItemTypeSms[MAX_ACCOUNT_LEVEL];
	int m_ItemIndexSms[MAX_ACCOUNT_LEVEL];
	int m_ItemLevelSms[MAX_ACCOUNT_LEVEL];
	// ----
	int m_File_Loaded;
	HANDLE VoterFile;
	std::vector<PARCER_DATA> m_ParcerStruct;
	std::vector<PARCER_DATA> m_VoterStruct;
}; extern CVoteParcer gVoteParcer;