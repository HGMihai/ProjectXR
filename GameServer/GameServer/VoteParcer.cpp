#include "StdAfx.h"
#include "VoteParcer.h"
#include "Protection.h"
#include "Util.h"
#include "MemScript.h"
#include "Notice.h"
#include "Message.h"
#include "Path.h"
#include "CashShop.h"
#include "DSProtocol.h"
#include "ItemManager.h"
#include "BankEx.h"

CVoteParcer gVoteParcer;

CVoteParcer::CVoteParcer()
{
	this->Init();
}

CVoteParcer::~CVoteParcer()
{
	
}

void CVoteParcer::Init()
{
	if (gProtection.GetCustomState(CUSTOM_VOTEPARCER) == 0)
	{
		return;
	}

	this->m_MmoTopUse = 0;
	this->m_MmmoTopLink[0] = 0;
	for (int i=0;i<MAX_ACCOUNT_LEVEL;i++)
	{
		this->m_Credits[i] = 0;
		this->m_WCoinC[i] = 0;
		this->m_WCoinP[i] = 0;
		this->m_WCoinG[i] = 0;
		this->m_CreditsSms[i] = 0;
		this->m_WCoinCSms[i] = 0;
		this->m_WCoinPSms[i] = 0;
		this->m_WCoinGSms[i] = 0;
		this->m_ItemActive[i] = 0;
		this->m_ItemType[i] = 0;
		this->m_ItemIndex[i] = 0;
		this->m_ItemLevel[i] = 0;
	}
	// ----
	this->m_File_Loaded = 0;
	CloseHandle(this->VoterFile);
}

void CVoteParcer::Load(char* path)
{
	if (gProtection.GetCustomState(CUSTOM_VOTEPARCER) == 0)
	{
		return;
	}

	this->m_MmoTopUse = GetPrivateProfileInt("VoteParcer","UseMMOTOP",0,path);

	GetPrivateProfileString("MMOTOP","Link","",this->m_MmmoTopLink,sizeof(this->m_MmmoTopLink),path);

	this->m_Credits[0] = GetPrivateProfileInt("MMOTOP","Credits_AL0",0,path);
	this->m_Credits[1] = GetPrivateProfileInt("MMOTOP","Credits_AL1",0,path);
	this->m_Credits[2] = GetPrivateProfileInt("MMOTOP","Credits_AL2",0,path);
	this->m_Credits[3] = GetPrivateProfileInt("MMOTOP","Credits_AL3",0,path);

	this->m_WCoinC[0] = GetPrivateProfileInt("MMOTOP","WCoinC_AL0",0,path);
	this->m_WCoinC[1] = GetPrivateProfileInt("MMOTOP","WCoinC_AL1",0,path);
	this->m_WCoinC[2] = GetPrivateProfileInt("MMOTOP","WCoinC_AL2",0,path);
	this->m_WCoinC[3] = GetPrivateProfileInt("MMOTOP","WCoinC_AL3",0,path);

	this->m_WCoinP[0] = GetPrivateProfileInt("MMOTOP","WCoinP_AL0",0,path);
	this->m_WCoinP[1] = GetPrivateProfileInt("MMOTOP","WCoinP_AL1",0,path);
	this->m_WCoinP[2] = GetPrivateProfileInt("MMOTOP","WCoinP_AL2",0,path);
	this->m_WCoinP[3] = GetPrivateProfileInt("MMOTOP","WCoinP_AL3",0,path);

	this->m_WCoinG[0] = GetPrivateProfileInt("MMOTOP","WCoinG_AL0",0,path);
	this->m_WCoinG[1] = GetPrivateProfileInt("MMOTOP","WCoinG_AL1",0,path);
	this->m_WCoinG[2] = GetPrivateProfileInt("MMOTOP","WCoinG_AL2",0,path);
	this->m_WCoinG[3] = GetPrivateProfileInt("MMOTOP","WCoinG_AL3",0,path);

	this->m_ItemActive[0] = GetPrivateProfileInt("MMOTOP","ItemActive_AL0",0,path);
	this->m_ItemActive[1] = GetPrivateProfileInt("MMOTOP","ItemActive_AL1",0,path);
	this->m_ItemActive[2] = GetPrivateProfileInt("MMOTOP","ItemActive_AL2",0,path);
	this->m_ItemActive[3] = GetPrivateProfileInt("MMOTOP","ItemActive_AL3",0,path);

	this->m_ItemType[0] = GetPrivateProfileInt("MMOTOP","ItemType_AL0",0,path);
	this->m_ItemType[1] = GetPrivateProfileInt("MMOTOP","ItemType_AL1",0,path);
	this->m_ItemType[2] = GetPrivateProfileInt("MMOTOP","ItemType_AL2",0,path);
	this->m_ItemType[3] = GetPrivateProfileInt("MMOTOP","ItemType_AL3",0,path);

	this->m_ItemIndex[0] = GetPrivateProfileInt("MMOTOP","ItemIndex_AL0",0,path);
	this->m_ItemIndex[1] = GetPrivateProfileInt("MMOTOP","ItemIndex_AL1",0,path);
	this->m_ItemIndex[2] = GetPrivateProfileInt("MMOTOP","ItemIndex_AL2",0,path);
	this->m_ItemIndex[3] = GetPrivateProfileInt("MMOTOP","ItemIndex_AL3",0,path);

	this->m_ItemLevel[0] = GetPrivateProfileInt("MMOTOP","ItemLevel_AL0",0,path);
	this->m_ItemLevel[1] = GetPrivateProfileInt("MMOTOP","ItemLevel_AL1",0,path);
	this->m_ItemLevel[2] = GetPrivateProfileInt("MMOTOP","ItemLevel_AL2",0,path);
	this->m_ItemLevel[3] = GetPrivateProfileInt("MMOTOP","ItemLevel_AL3",0,path);

	this->m_CreditsSms[0] = GetPrivateProfileInt("MMOTOP","SmsCredits_AL0",0,path);
	this->m_CreditsSms[1] = GetPrivateProfileInt("MMOTOP","SmsCredits_AL1",0,path);
	this->m_CreditsSms[2] = GetPrivateProfileInt("MMOTOP","SmsCredits_AL2",0,path);
	this->m_CreditsSms[3] = GetPrivateProfileInt("MMOTOP","SmsCredits_AL3",0,path);

	this->m_WCoinCSms[0] = GetPrivateProfileInt("MMOTOP","SmsWCoinC_AL0",0,path);
	this->m_WCoinCSms[1] = GetPrivateProfileInt("MMOTOP","SmsWCoinC_AL1",0,path);
	this->m_WCoinCSms[2] = GetPrivateProfileInt("MMOTOP","SmsWCoinC_AL2",0,path);
	this->m_WCoinCSms[3] = GetPrivateProfileInt("MMOTOP","SmsWCoinC_AL3",0,path);

	this->m_WCoinPSms[0] = GetPrivateProfileInt("MMOTOP","SmsWCoinP_AL0",0,path);
	this->m_WCoinPSms[1] = GetPrivateProfileInt("MMOTOP","SmsWCoinP_AL1",0,path);
	this->m_WCoinPSms[2] = GetPrivateProfileInt("MMOTOP","SmsWCoinP_AL2",0,path);
	this->m_WCoinPSms[3] = GetPrivateProfileInt("MMOTOP","SmsWCoinP_AL3",0,path);

	this->m_WCoinGSms[0] = GetPrivateProfileInt("MMOTOP","SmsWCoinG_AL0",0,path);
	this->m_WCoinGSms[1] = GetPrivateProfileInt("MMOTOP","SmsWCoinG_AL1",0,path);
	this->m_WCoinGSms[2] = GetPrivateProfileInt("MMOTOP","SmsWCoinG_AL2",0,path);
	this->m_WCoinGSms[3] = GetPrivateProfileInt("MMOTOP","SmsWCoinG_AL3",0,path);
	
	this->m_ItemActiveSms[0] = GetPrivateProfileInt("MMOTOP","SmsItemActive_AL0",0,path);
	this->m_ItemActiveSms[1] = GetPrivateProfileInt("MMOTOP","SmsItemActive_AL1",0,path);
	this->m_ItemActiveSms[2] = GetPrivateProfileInt("MMOTOP","SmsItemActive_AL2",0,path);
	this->m_ItemActiveSms[3] = GetPrivateProfileInt("MMOTOP","SmsItemActive_AL3",0,path);

	this->m_ItemTypeSms[0] = GetPrivateProfileInt("MMOTOP","SmsItemType_AL0",0,path);
	this->m_ItemTypeSms[1] = GetPrivateProfileInt("MMOTOP","SmsItemType_AL1",0,path);
	this->m_ItemTypeSms[2] = GetPrivateProfileInt("MMOTOP","SmsItemType_AL2",0,path);
	this->m_ItemTypeSms[3] = GetPrivateProfileInt("MMOTOP","SmsItemType_AL3",0,path);

	this->m_ItemIndexSms[0] = GetPrivateProfileInt("MMOTOP","SmsItemIndex_AL0",0,path);
	this->m_ItemIndexSms[1] = GetPrivateProfileInt("MMOTOP","SmsItemIndex_AL1",0,path);
	this->m_ItemIndexSms[2] = GetPrivateProfileInt("MMOTOP","SmsItemIndex_AL2",0,path);
	this->m_ItemIndexSms[3] = GetPrivateProfileInt("MMOTOP","SmsItemIndex_AL3",0,path);

	this->m_ItemLevelSms[0] = GetPrivateProfileInt("MMOTOP","SmsItemLevel_AL0",0,path);
	this->m_ItemLevelSms[1] = GetPrivateProfileInt("MMOTOP","SmsItemLevel_AL1",0,path);
	this->m_ItemLevelSms[2] = GetPrivateProfileInt("MMOTOP","SmsItemLevel_AL2",0,path);
	this->m_ItemLevelSms[3] = GetPrivateProfileInt("MMOTOP","SmsItemLevel_AL3",0,path);

	if (this->m_MmoTopUse == 1)
	{
		this->LoadFile(this->m_MmmoTopLink, ".\\VoteList.txt");
	}
}

void CVoteParcer::LoadFile(char* link, char* path)
{
	if (this->m_File_Loaded == 0)
	{
		this->VoterFile = CreateFile(".\\VoterList.txt",GENERIC_WRITE,FILE_SHARE_READ,0,OPEN_ALWAYS,FILE_ATTRIBUTE_NORMAL,0);
	}

	this->m_File_Loaded = 1;

	HRESULT res = URLDownloadToFile(0, link, path, 0, 0);
	if (res == S_OK)
	{
		this->m_ParcerStruct.clear();

		FILE *file;

		file = fopen(path,"r");

		if (file == NULL)
		{ 
			LogAdd(LOG_RED, "[ERROR] Cant find file!");
			return;
		}

		char Buff[256];

		while (true)
		{
			fgets(Buff,1025,file);

			if (feof(file) != 0) 
			{
				break;
			}

			PARCER_DATA info;
				
			sscanf(Buff, "%d\t%s\t%s\t%s\t%s\t%d", &info.Id, &info.Date, &info.Time, &info.IP, &info.Account, &info.Type);

			this->m_ParcerStruct.push_back(info);
		}


		fclose(file);
		this->LoadVotersFile(".\\VoterList.txt");
	}
	else
	{
		LogAdd(LOG_RED, "[ERROR] Cant load file!");
	}
}

void CVoteParcer::LoadVotersFile(char* path)
{
	this->m_VoterStruct.clear();

	FILE *file;

	file = fopen(path,"r");

	if (file == NULL)
	{ 
		LogAdd(LOG_RED, "[ERROR] Cant find file!");
		return;
	}

	char Buff[256];

	while (true)
	{
		fgets(Buff,1025,file);

		if (feof(file) != 0) 
		{
			break;
		}

		PARCER_DATA info;
				
		sscanf(Buff, "%d\t%s\t%s\t%s\t%s\t%d", &info.Id, &info.Date, &info.Time, &info.IP, &info.Account, &info.Type);

		this->m_VoterStruct.push_back(info);
	}

	this->CheckVote();
	fclose(file);
}

void CVoteParcer::CheckVote()
{
	for(std::vector<PARCER_DATA>::iterator it=this->m_ParcerStruct.begin();it != this->m_ParcerStruct.end();it++)
	{
		if (this->CheckVoter(it) == 0)
		{
			continue;
		}

		LPOBJ lpObj = gObjFindByAcc(it->Account);

		if(lpObj == 0)
		{
			continue;
		}

		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,"You are voted! Get your prize! [%s] %s", it->IP, it->Date);

		if (it->Type == 1)
		{
			lpObj->BankEx.Credits += this->m_Credits[lpObj->AccountLevel];

			gBankEx.GDSavePoint(lpObj->Index);

			gCashShop.GDCashShopAddPointSaveSend(lpObj->Index,0, this->m_WCoinC[lpObj->AccountLevel], this->m_WCoinP[lpObj->AccountLevel], this->m_WCoinG[lpObj->AccountLevel]);

			if (this->m_ItemActive[lpObj->AccountLevel] == 1)
			{
				GDCreateItemSend(lpObj->Index,0xEB,0,0,GET_ITEM(this->m_ItemType[lpObj->AccountLevel],this->m_ItemIndex[lpObj->AccountLevel]),this->m_ItemLevel[lpObj->AccountLevel],0,0,0,0,-1,0,0,0,0,0,0xFF,0);
			}
		}
		else if (it->Type == 2)
		{
			lpObj->BankEx.Credits += this->m_CreditsSms[lpObj->AccountLevel];

			gBankEx.GDSavePoint(lpObj->Index);

			gCashShop.GDCashShopAddPointSaveSend(lpObj->Index,0, this->m_WCoinCSms[lpObj->AccountLevel], this->m_WCoinPSms[lpObj->AccountLevel], this->m_WCoinGSms[lpObj->AccountLevel]);

			if (this->m_ItemActiveSms[lpObj->AccountLevel] == 1)
			{
				GDCreateItemSend(lpObj->Index,0xEB,0,0,GET_ITEM(this->m_ItemTypeSms[lpObj->AccountLevel],this->m_ItemIndexSms[lpObj->AccountLevel]),this->m_ItemLevelSms[lpObj->AccountLevel],0,0,0,0,-1,0,0,0,0,0,0xFF,0);
			}
		}
		else
		{
			//LogAdd(LOG_RED, "[BankEx] Wrong Type! Please, contact support! Type [%d]", it->Type);
			continue;
		}

		gAchievements.MootopVoteUp(lpObj->Index);
		LogAdd(LOG_BLACK, "[VoteParcer] [%s | %s] successfully get prize. VoteType %d", lpObj->Account, lpObj->Name, it->Type);
		this->AddVoteToFile(it);
	}
}

bool CVoteParcer::CheckVoter(std::vector<PARCER_DATA>::iterator it)
{
	for(std::vector<PARCER_DATA>::iterator voter=this->m_VoterStruct.begin();voter != this->m_VoterStruct.end();voter++)
	{
		if(strcmp(it->Account,voter->Account) == 0 && strcmp(it->Date,voter->Date) == 0 && strcmp(it->Time,voter->Time) == 0)
		{
			return 0;
		}
	}
	return 1;
}

void CVoteParcer::AddVoteToFile(std::vector<PARCER_DATA>::iterator it)
{
	if(this->VoterFile == INVALID_HANDLE_VALUE)
	{
		//lpInfo->Active = 0;
		LogAdd(LOG_RED, "%s Error File not found!", __FUNCTION__);
		return;
	}

	if(SetFilePointer(this->VoterFile,0,0,FILE_END) == INVALID_SET_FILE_POINTER)
	{
		//lpInfo->Active = 0;
		CloseHandle(this->VoterFile);
		LogAdd(LOG_RED, "%s Error SetFilePointer!", __FUNCTION__);
		return;
	}

	char buff[1024] = {0};

	//wsprintf(buff,"%02d:%02d:%02d %s\r\n",time.wHour,time.wMinute,time.wSecond,temp);
	wsprintf(buff, "%d\t%s\t%s\t%s\t%s\t%d\r\n", it->Id, it->Date, it->Time, it->IP, it->Account, it->Type);

	DWORD OutSize;

	WriteFile(this->VoterFile,buff,strlen(buff),&OutSize,0);

	PARCER_DATA info;

	info.Id	= it->Id;
	memcpy(info.Date, it->Date, sizeof(info.Date));
	memcpy(info.Time, it->Time, sizeof(info.Time));
	memcpy(info.IP, it->IP, sizeof(info.IP));
	memcpy(info.Account, it->Account, sizeof(info.Account));
	info.Type = it->Type;

	this->m_VoterStruct.push_back(info);
}