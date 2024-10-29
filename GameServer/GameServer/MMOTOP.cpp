#include "stdafx.h"
#include "MMOTOP.h"
#include "Protection.h"
#include "Util.h"
#include "Notice.h"
#include "CashShop.h"
#include "DSProtocol.h"
#include "ItemManager.h"
#include "BankEx.h"
#include "../../Util/curl/curl.h"
#include "JSProtocol.h"
#include "Log.h"

cMMOTOP gMMOTOP;

cMMOTOP::cMMOTOP()
{
	this->Init();
}

cMMOTOP::~cMMOTOP()
{
	
}

void cMMOTOP::Init()
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
	//this->m_File_Loaded = 0;
	//CloseHandle(this->VoterFile);
	this->m_ParcerStruct.clear();
}

void cMMOTOP::Load(char* path)
{
	if (gProtection.GetCustomState(CUSTOM_VOTEPARCER) == 0)
	{
		return;
	}

	this->Init();

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

	/*if (this->m_MmoTopUse == 1)
	{
		this->LoadFile(this->m_MmmoTopLink, ".\\VoteList.txt");
	}*/
	//this->DownloadFile(this->m_MmmoTopLink);
}

size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

void cMMOTOP::Tick()
{
	if (gProtection.GetCustomState(CUSTOM_VOTEPARCER) == 0)
	{
		return;
	}

	this->DownloadFile(this->m_MmmoTopLink);
}

#include <fstream>
#include <sstream>
void cMMOTOP::DownloadFile(char* url)
{
	if (!this->m_MmoTopUse)
	{
		return;
	}
	//MsgBox(url);
	CURL *curl;
	CURLcode res;

	std::string readBuffer;

	curl = curl_easy_init();
	if(curl) {
		curl_easy_setopt(curl, CURLOPT_URL, url);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
		res = curl_easy_perform(curl);
		curl_easy_cleanup(curl);
	}
	else
	{
		MsgBox("Curl error!");
		return;
	}

	this->m_ParcerStruct.clear();

	std::vector<std::string> arr;

	std::ofstream out("test.txt");

	std::stringstream ss_buffer(readBuffer);
	
	std::string temp_str;
	while(std::getline(ss_buffer, temp_str))
	{
        out << temp_str << std::endl;
		arr.push_back( temp_str );
    }
	
	out.close();
	for	(std::vector<std::string>::iterator it=arr.begin();it != arr.end();it++)
	{
		MMOTOP_DATA info;
				
		sscanf(it->c_str(), "%d\t%s\t%s\t%s\t%s\t%d", &info.Id, &info.Date, &info.Time, &info.IP, &info.Account, &info.Type);

		this->m_ParcerStruct.push_back(info);
	}

	for(auto it=this->m_ParcerStruct.begin();it != this->m_ParcerStruct.end();it++)
	{
		LogAdd(LOG_TEST,"Account = %s",it->Account);

		char DataTime[100];
		sprintf(DataTime, "%s %s",it->Date, it->Time);

		int nYear, nMonth, nDay, nHour, nMin, nSec;
		sscanf(DataTime, "%d.%d.%d %d:%d:%d", &nDay, &nMonth, &nYear, &nHour, &nMin, &nSec);

		if( ((nYear >= 1900)  && (nMonth >= 1 && nMonth <= 12) && (nDay >= 1 && nDay <= 31) && (nHour >= 0 && nHour <= 23) && (nMin >= 0 && nMin <= 59) && (nSec >= 0 && nSec <= 59)) )
		{

		}
		else
		{
			continue;
		}

		int aIndex = -1;

		for(int n=OBJECT_START_USER;n < MAX_OBJECT;n++)
		{
			if (gObj[n].Connected == OBJECT_ONLINE)
			{
				if (gObj[n].Account[0] == it->Account[0])
				{
					if (!strcmp(gObj[n].Account, it->Account))
					{
						aIndex = n;
						break;
					}
				}
			}
		}

		if(aIndex == -1) continue;

		CTime ct = CTime(nYear, nMonth, nDay, nHour, nMin, nSec);
		DWORD tData = ct.GetTime();

		LPOBJ lpObj = &gObj[aIndex];

		LogAdd(LOG_TEST,"Year = %d Mon = %d Day = %d H = %d Min = %d sec = %d",nYear, nMonth, nDay, nHour, nMin, nSec);

		LogAdd(LOG_TEST,"tData = %d last = %d",tData,lpObj->last_vote_time);

		if (tData > lpObj->last_vote_time)
		{
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

			lpObj->last_vote_time = tData;

			GJLastVoteTimeSave(aIndex);

			gAchievements.MootopVoteUp(lpObj->Index);
			LogAdd(LOG_BLACK, "[VoteParcer] [%s | %s] successfully get prize. VoteType %d", lpObj->Account, lpObj->Name, it->Type);

			gLog.Output(LOG_VOTE,"[VoteParcer] [%s | %s] successfully get prize. VoteType %d", lpObj->Account, lpObj->Name, it->Type);


		}
	}

	
}
