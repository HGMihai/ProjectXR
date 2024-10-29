#pragma once
#include "User.h"

struct MMOTOP_DATA
{
	int Id;
	char Date[12];
	char Time[12];
	char IP[16];
	char Account[32];
	int Type;
};

class cMMOTOP
{
public:
	cMMOTOP();
	virtual ~cMMOTOP();
	void Init();
	void Load(char* path);
	void DownloadFile(char* url);
	void Tick();
private:
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
	HANDLE VoterFile;
	std::vector<MMOTOP_DATA> m_ParcerStruct;
}; extern cMMOTOP gMMOTOP;