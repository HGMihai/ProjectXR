// ServerDisplayer.cpp: implementation of the CServerDisplayer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ServerDisplayer.h"
#include "GameMain.h"
#include "Log.h"
#include "Protect.h"
#include "ServerInfo.h"
#include "SocketManager.h"
#include "User.h"

CServerDisplayer gServerDisplayer;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CServerDisplayer::CServerDisplayer() // OK
{
	for(int n=0;n < MAX_LOG_TEXT_LINE;n++)
	{
		memset(&this->m_log[n],0,sizeof(this->m_log[n]));
	}

	this->m_font = CreateFontA(40,0,0,0,FW_REGULAR,0,0,0,ANSI_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH | FF_DONTCARE,"Times");

	this->m_brush[0] = CreateSolidBrush(BACKCOLOR);
	this->m_brush[1] = CreateSolidBrush(BACKCOLOR);

	strcpy_s(this->m_DisplayerText[0],"[ STANDBY MODE ]");
	strcpy_s(this->m_DisplayerText[1],"[ LTPTeam GameServer ]");
}

CServerDisplayer::~CServerDisplayer() // OK
{
	DeleteObject(this->m_font);
	DeleteObject(this->m_brush[0]);
	DeleteObject(this->m_brush[1]);
}

void CServerDisplayer::Init(HWND hWnd) // OK
{
	PROTECT_START

	this->m_hwnd = hWnd;

	PROTECT_FINAL

	gLog.AddLog(1,"LOG");

	gLog.AddLog(gServerInfo.m_WriteChatLog,"CHAT_LOG");

	gLog.AddLog(gServerInfo.m_WriteCommandLog,"COMMAND_LOG");

	gLog.AddLog(gServerInfo.m_WriteTradeLog,"TRADE_LOG");

	gLog.AddLog(gServerInfo.m_WriteConnectLog,"CONNECT_LOG");

	gLog.AddLog(gServerInfo.m_WriteHackLog,"HACK_LOG");

	gLog.AddLog(gServerInfo.m_WriteCashShopLog,"CASH_SHOP_LOG");

	gLog.AddLog(gServerInfo.m_WriteChaosMixLog,"CHAOS_MIX_LOG");
}

void CServerDisplayer::Run() // OK
{
	this->SetWindowName();
	this->PaintAllInfo();
	this->LogTextPaint();
}

void CServerDisplayer::SetWindowName() // OK
{
	char buff[256];

	wsprintf(buff,"[%s] %s (PlayerCount : %d/%d) (MonsterCount : %d/%d)",GAMESERVER_VERSION,gServerInfo.m_ServerName,gObjTotalUser,gServerInfo.m_ServerMaxUserNumber,gObjTotalMonster,MAX_OBJECT_MONSTER);

	SetWindowText(this->m_hwnd,buff);
}

void CServerDisplayer::PaintAllInfo() // OK
{
	RECT rect;

	GetClientRect(this->m_hwnd,&rect);

	rect.top = 0;
	rect.bottom = 80;

	HDC hdc = GetDC(this->m_hwnd);

	int OldBkMode = SetBkMode(hdc,TRANSPARENT);

	HFONT OldFont = (HFONT)SelectObject(hdc,this->m_font);

	if(gJoinServerConnection.CheckState() == 0 || gDataServerConnection.CheckState() == 0)
	{
		SetTextColor(hdc,RGB(0, 255, 255));
		FillRect(hdc,&rect,this->m_brush[0]);
		TextOut(hdc,GAMESERVER_WIDTH / 2 - 240,18,this->m_DisplayerText[0],strlen(this->m_DisplayerText[0]));
	}
	else
	{
		gGameServerDisconnect = 0;
		SetTextColor(hdc,RGB(0, 255, 255));
		FillRect(hdc,&rect,this->m_brush[1]);
		TextOut(hdc,GAMESERVER_WIDTH / 2 - 240,18,this->m_DisplayerText[1],strlen(this->m_DisplayerText[1]));
	}

	SelectObject(hdc,OldFont);
	SetBkMode(hdc,OldBkMode);
	ReleaseDC(this->m_hwnd,hdc);
}

void CServerDisplayer::Interface(HWND hWnd)
{
	HBRUSH BackgroundSolidColor = CreateSolidBrush(BACKCOLOR);

	HDC hDC = GetDC(hWnd);
	RECT rect;
	GetClientRect(hWnd, &rect);
	FillRect(hDC, &rect, BackgroundSolidColor);
	//---
	SetBkMode(hDC, TRANSPARENT);
	ReleaseDC(hWnd, hDC);
}

void CServerDisplayer::LogTextPaint() // OK
{
	RECT rect;

	GetClientRect(this->m_hwnd,&rect);

	rect.top = 80;

	HDC hdc = GetDC(this->m_hwnd);

	//this->Interface(this->m_hwnd);
	FillRect(hdc, &rect, CreateSolidBrush(BACKCOLOR));
	// ----
	
	//FillRect(hdc,&rect,(HBRUSH)GetStockObject(0));


	int line = MAX_LOG_TEXT_LINE;

	int count = (((this->m_count-1)>=0)?(this->m_count-1):(MAX_LOG_TEXT_LINE-1));

	for(int n=0;n < MAX_LOG_TEXT_LINE;n++)
	{
		switch(this->m_log[count].color)
		{
			case LOG_BLACK:
				SetTextColor(hdc,RGB(127, 255, 212));
				SetBkColor(hdc, BACKCOLOR);
				break;
			case LOG_RED:
				SetTextColor(hdc,RGB(255,0,0));
				SetBkColor(hdc, BACKCOLOR);
				break;
			case LOG_GREEN:
				SetTextColor(hdc,RGB(0,255,0));
				SetBkColor(hdc, BACKCOLOR);
				break;
			case LOG_BLUE:
				SetTextColor(hdc,RGB(0,0,255));
				SetBkColor(hdc, BACKCOLOR);
				break;
		}

		int size = strlen(this->m_log[count].text);

		if(size > 1)
		{
			TextOut(hdc,0,(65+(line*15)),this->m_log[count].text,size);
			line--;
		}

		count = (((--count)>=0)?count:(MAX_LOG_TEXT_LINE-1));
	}
	//this->Interface(this->m_hwnd);
	SetBkMode(hdc, TRANSPARENT);
	ReleaseDC(this->m_hwnd,hdc);
}

void CServerDisplayer::LogAddText(eLogColor color,char* text,int size) // OK
{
	PROTECT_START

	size = ((size>=MAX_LOG_TEXT_SIZE)?(MAX_LOG_TEXT_SIZE-1):size);

	memset(&this->m_log[this->m_count].text,0,sizeof(this->m_log[this->m_count].text));

	memcpy(&this->m_log[this->m_count].text,text,size);

	this->m_log[this->m_count].color = color;

	this->m_count = (((++this->m_count)>=MAX_LOG_TEXT_LINE)?0:this->m_count);

	PROTECT_FINAL

	gLog.Output(LOG_GENERAL,"%s",&text[9]);
}
