#include "StdAfx.h"
//#include "..\common\winutil.h"
//#include "..\include\readscript.h"
#if (GAMESERVER_UPDATE == 603 || IS_NEW_CLIENT)
#include "NewsBoard.h"
//#include "GameMain.h"
//#include "logproc.h"
//#include "NewLicense.h"
#include "Path.h"
#include "MemScript.h"
#include "Util.h"

// -------------------------------------------------------------------------------

NewsBoard gNewsBoard;
// -------------------------------------------------------------------------------

NewsBoard::NewsBoard()
{
	this->Init();
}
// -------------------------------------------------------------------------------

NewsBoard::~NewsBoard()
{

}
// -------------------------------------------------------------------------------

void NewsBoard::Init()
{
	this->m_LoadedCount = 0;
	this->m_ShowOnLogin	= false;
	// ----
	for( int i = 0; i < MAX_NEWS_LIST; i++ )
	{
		this->m_Data[i].Title.Date[0]	= 0;
		this->m_Data[i].Title.Time[0]	= 0;
		this->m_Data[i].Title.Text[0]	= 0;
		this->m_Data[i].Text[0]			= 0;
	}
}
// -------------------------------------------------------------------------------

void NewsBoard::ReadListData(char * path)
{
	CMemScript* lpMemScript = new CMemScript;

	if(lpMemScript == 0)
	{
		ErrorMessageBox(MEM_SCRIPT_ALLOC_ERROR,path);
		return;
	}

	if(lpMemScript->SetBuffer(path) == 0)
	{
		ErrorMessageBox(lpMemScript->GetLastError());
		delete lpMemScript;
		return;
	}

	//this->m_Teleports.clear();

	try
	{
		while(true)
		{
			if(lpMemScript->GetToken() == TOKEN_END)
			{
				break;
			}

			if(strcmp("end",lpMemScript->GetString()) == 0)
			{
				break;
			}

			memcpy(this->m_Data[this->m_LoadedCount].Title.Date, lpMemScript->GetString(), sizeof(this->m_Data[this->m_LoadedCount].Title.Date));
			memcpy(this->m_Data[this->m_LoadedCount].Title.Time, lpMemScript->GetAsString(), sizeof(this->m_Data[this->m_LoadedCount].Title.Time));
			memcpy(this->m_Data[this->m_LoadedCount].Title.Text, lpMemScript->GetAsString(), sizeof(this->m_Data[this->m_LoadedCount].Title.Text));
			memcpy(this->m_Data[this->m_LoadedCount].Text, lpMemScript->GetAsString(), sizeof(this->m_Data[this->m_LoadedCount].Text));
			// ----
			this->m_LoadedCount++;
		}
	}
	catch(...)
	{
		ErrorMessageBox(lpMemScript->GetLastError());
	}

	delete lpMemScript;
	LogAdd(LOG_BLACK, "[NewsBoard] [%d] Item loaded from list", this->m_LoadedCount);
}
// -------------------------------------------------------------------------------

void NewsBoard::ReadMainData(char * File)
{
	this->m_ShowOnLogin	= GetPrivateProfileIntA("Common", "ShowOnLogin", 0, File);
}
// -------------------------------------------------------------------------------

void NewsBoard::Load()
{
	//if (!IsLicenseChecked)
	//{
	//	return;
	//}
	this->Init();
	this->ReadListData(gPath.GetFullPath("..\\LTPData\\NewsList.txt"));
	this->ReadMainData(gPath.GetFullPath("..\\LTPData\\NewsMain.ini"));
}
// -------------------------------------------------------------------------------

void NewsBoard::OpenMain(LPOBJ lpUser)
{
	if( lpUser->Connected != OBJECT_ONLINE )
	{
		return;
	}
	// ----
	NEWS_ANS_TITLES pAnswer = { 0 };
	//PHeadSetW((LPBYTE)&pAnswer, 0xFD, sizeof(NEWS_ANS_TITLES));
	pAnswer.h.set(0xFE, sizeof(NEWS_ANS_TITLES));
	// ----
	pAnswer.RealCount = this->m_LoadedCount;
	// ----
	for( int i = 0; i < this->m_LoadedCount; i++ )
	{

		strcpy(pAnswer.Titles[i].Date, this->m_Data[i].Title.Date);
		strcpy(pAnswer.Titles[i].Time, this->m_Data[i].Title.Time);
		strcpy(pAnswer.Titles[i].Text, this->m_Data[i].Title.Text);
	}
	// ----
	DataSend(lpUser->Index, (LPBYTE)&pAnswer, sizeof(NEWS_ANS_TITLES));
}
// -------------------------------------------------------------------------------

void NewsBoard::OpenItem(LPOBJ lpUser, NEWS_REQ_NEWS * Request)
{
	if( lpUser->Connected != OBJECT_ONLINE )
	{
		return;
	}
	// ----
	if( Request->ID < 0 || Request->ID >= MAX_NEWS_LIST )
	{
		return;
	}
	// ----
	NEWS_ANS_NEWS pAnswer = { 0 };
	//PHeadSetW((LPBYTE)&pAnswer, 0xFE, sizeof(NEWS_ANS_TITLES));
	pAnswer.h.set(0xFF, sizeof(NEWS_ANS_NEWS));
	// ----
	pAnswer.ID = Request->ID;
	// ----
	strcpy(pAnswer.News.Title.Date, this->m_Data[Request->ID].Title.Date);
	strcpy(pAnswer.News.Title.Time, this->m_Data[Request->ID].Title.Time);
	strcpy(pAnswer.News.Title.Text, this->m_Data[Request->ID].Title.Text);
	strcpy(pAnswer.News.Text, this->m_Data[Request->ID].Text);
	// ----
	DataSend(lpUser->Index, (LPBYTE)&pAnswer, sizeof(NEWS_ANS_NEWS));
}
// -------------------------------------------------------------------------------
#endif