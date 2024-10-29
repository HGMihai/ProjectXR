// CustomAttack.h: interface for the CCustomAttack class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "User.h"
#include <fstream>

struct OFFEXP_ITEMS
{
	int Group;
	int Index;
};
#if USE_OFFEXP_RELOAD == TRUE
struct OFFEXP_DATA
{
	char Account[11];
	char Password[11];
	char Name[11];
	char IP[16];
	int SkillID;
	int UseBuffs;
	int Pick;
};
#endif
class CCustomAttack
{
public:
	CCustomAttack();
	virtual ~CCustomAttack();
	void Load(char* Path);
	void ReadCustomAttackInfo(char* section,char* path);
	void CommandCustomAttack(LPOBJ lpObj,char* arg);
	void CommandCustomAttackOffline(LPOBJ lpObj,char* arg);
	bool GetAttackSkill(LPOBJ lpObj,int* SkillNumber);
	bool GetTargetMonster(LPOBJ lpObj,int SkillNumber,int* MonsterIndex);
	bool CheckRequireMoney(LPOBJ lpObj);
	void OnAttackClose(LPOBJ lpObj);
	void OnAttackSecondProc(LPOBJ lpObj);
	void OnAttackAlreadyConnected(LPOBJ lpObj);
	void OnAttackMonsterAndMsgProc(LPOBJ lpObj);
	void SendSkillAttack(LPOBJ lpObj,int aIndex,int SkillNumber);
	void SendMultiSkillAttack(LPOBJ lpObj,int aIndex,int SkillNumber);
	void SendDurationSkillAttack(LPOBJ lpObj,int aIndex,int SkillNumber);
	void SendRFSkillAttack(LPOBJ lpObj,int aIndex,int SkillNumber);
	void GetItemOnFloor(LPOBJ lpObj);
	void ElfBuffs(LPOBJ lpObj);
#if USE_OFFEXP_RELOAD == TRUE
	void RestoreOffline();
	OFFEXP_DATA* GetOffExpInfo(LPOBJ lpObj);
	OFFEXP_DATA* GetOffExpInfoByAccount(LPOBJ lpObj);
	void LoadOffExp(char* path);
	void AddToList(LPOBJ lpObj);
	void DelFromList(char* account);
	void WriteToFile();
#endif
public:
	int m_CustomAttackSwitch;
	int m_CustomAttackEnable[MAX_ACCOUNT_LEVEL];
	int m_CustomAttackRequireLevel[MAX_ACCOUNT_LEVEL];
	int m_CustomAttackRequireReset[MAX_ACCOUNT_LEVEL];
	char m_CustomAttackText1[128];
	char m_CustomAttackText2[128];
	char m_CustomAttackText3[128];
	char m_CustomAttackText4[128];
	char m_CustomAttackText5[128];
	char m_CustomAttackText6[128];
	int m_CustomAttackOfflineSwitch;
	int m_CustomAttackOfflineGPGain;
	int m_CustomAttackOfflineEnable[MAX_ACCOUNT_LEVEL];
	int m_CustomAttackOfflineRequireLevel[MAX_ACCOUNT_LEVEL];
	int m_CustomAttackOfflineRequireReset[MAX_ACCOUNT_LEVEL];
	int m_CustomAttackOfflineRequireMoney[MAX_ACCOUNT_LEVEL];
	int m_CustomAttackOfflineMoneyFreeTime[MAX_ACCOUNT_LEVEL];
	int m_CustomAttackOfflineOpenPSShop[MAX_ACCOUNT_LEVEL];
	char m_CustomAttackOfflineText1[128];
	char m_CustomAttackOfflineText2[128];
	char m_CustomAttackOfflineText3[128];
	char m_CustomAttackOfflineText4[128];
	char m_CustomAttackOfflineText5[128];
	char m_CustomAttackOfflineText6[128];
	char m_CustomAttackOfflineText7[36];
	int m_CustomAttackGetItems[MAX_ACCOUNT_LEVEL];
	int m_CustomAttackGetExc[MAX_ACCOUNT_LEVEL];
	int m_CustomAttackGetMoney[MAX_ACCOUNT_LEVEL];
	int m_CustomAttackGetAnc[MAX_ACCOUNT_LEVEL];
	int m_CustomAttackGetSocket[MAX_ACCOUNT_LEVEL];
	int m_CustomAttackGetSelectedItems[MAX_ACCOUNT_LEVEL];
	int m_CustomAttackSpeed1[MAX_ACCOUNT_LEVEL];
	int m_CustomAttackSpeed2[MAX_ACCOUNT_LEVEL];
	int m_CustomAttackSpeed3[MAX_ACCOUNT_LEVEL];
	int m_CustomAttackSpeed4[MAX_ACCOUNT_LEVEL];
	int m_CustomAttackSpeed5[MAX_ACCOUNT_LEVEL];
	int m_CustomAttackSpeed6[MAX_ACCOUNT_LEVEL];
	int m_CustomAttackSpeed7[MAX_ACCOUNT_LEVEL];
	int m_CustomAttackSpeed8[MAX_ACCOUNT_LEVEL];
	// ----
	std::vector<OFFEXP_ITEMS> m_OffExpItems;
	//std::vector<OFFEXP_DATA> m_Data;
#if USE_OFFEXP_RELOAD == TRUE
	std::map<std::string,OFFEXP_DATA> m_Data;
	int AccountsRestored;
#endif
	//HANDLE File;
	//bool FileLoaded;
	//std::fstream File;
};

extern CCustomAttack gCustomAttack;
