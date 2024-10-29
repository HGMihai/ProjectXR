// ServerDisplayer.cpp: implementation of the CServerDisplayer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ServerDisplayer.h"
#include "GameMain.h"
#include "Log.h"
#include "ServerInfo.h"
#include "User.h"
#include "NewLicense.h"
#include "wininet.h"
#include "Protection.h"
#include "../../Util/curl/curl.h"

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

	this->m_brush[0] = CreateSolidBrush(RGB(105,105,105));
	this->m_brush[1] = CreateSolidBrush(BACKCOLOR);
	this->m_brush[2] = CreateSolidBrush(BACKCOLOR);

	strcpy_s(this->m_DisplayerText[0],"STANDBY MODE");
	strcpy_s(this->m_DisplayerText[1],"LTP-TEAM.COM GAMESERVER");
	strcpy_s(this->m_DisplayerText[2], "LTP-TEAM.COM GAMESERVER CASTLE");
	strcpy_s(this->m_DisplayerText[3],"License END! Pay monthly fee!");

	strcpy_s(this->m_DeveloperText[0],"Developed by LTP-Team.com");
	strcpy_s(this->m_DeveloperText[1],"Skype: letstoplay.support");
	strcpy_s(this->m_DeveloperText[2],"Telegram: LTPTEAM");

	/*strcpy_s(this->m_UpdatesText[0],"Update Available");
	strcpy_s(this->m_UpdatesText[1],"No Updates Available!");
	strcpy_s(this->m_UpdatesText[2],"Cant Connect to Server");*/
}

CServerDisplayer::~CServerDisplayer() // OK
{
	DeleteObject(this->m_font);
	DeleteObject(this->m_brush[0]);
	DeleteObject(this->m_brush[1]);
	DeleteObject(this->m_brush[2]);
}

void CServerDisplayer::Init(HWND hWnd, HWND hStatusWindow) // OK
{
	PROTECT_START

	this->m_hwnd = hWnd;
	this->m_hStatusWindow = hStatusWindow;

	PROTECT_FINAL

	this->UpdateInfoBar();

	this->Interface(this->m_hwnd);

	gLog.AddLog(1,"LOG\\GLOBAL_LOG");

	gLog.AddLog(gServerInfo.m_WriteChatLog,"LOG\\CHAT_LOG");

	gLog.AddLog(gServerInfo.m_WriteCommandLog,"LOG\\COMMAND_LOG");

	gLog.AddLog(gServerInfo.m_WriteTradeLog,"LOG\\TRADE_LOG");

	gLog.AddLog(gServerInfo.m_WriteConnectLog,"LOG\\CONNECT_LOG");

	gLog.AddLog(gServerInfo.m_WriteHackLog,"LOG\\HACK_LOG");

	gLog.AddLog(gServerInfo.m_WriteCashShopLog,"LOG\\CASH_SHOP_LOG");

	gLog.AddLog(gServerInfo.m_WriteChaosMixLog,"LOG\\CHAOS_MIX_LOG");

	gLog.AddLog(gServerInfo.m_WriteResetLog, "LOG\\RESET_LOG");

	gLog.AddLog(gServerInfo.m_WriteGrandResetLog, "LOG//GRAND_RESET_LOG");

	gLog.AddLog(1,"LOG//SMITHY_LOG");

	gLog.AddLog(1,"LOG//ITEM_VALUE_SELL_LOG");

	gLog.AddLog(1,"LOG//ITEM_VALUE_BUY_LOG");

	gLog.AddLog(1,"LOG//CUSTOM_CHAOS_BOX_LOG");

	gLog.AddLog(1,"LOG//SUPER_POINTS");

	gLog.AddLog(1,"LOG//VOTE");

	gLog.AddLog(1,"LOG//MONSTER_SPAWN");
}

void CServerDisplayer::Run() // OK
{
	this->Interface(this->m_hwnd);
	this->SetWindowName();
	this->LogTextPaint();
	this->PaintAllInfo();
}

void CServerDisplayer::SetWindowName() // OK
{
	char buff[256];
	/*char playerbuff[256];
	char monsterbuff[256];
	char itembuff[256];
	char offexpbuff[256];
	char offtradebuff[256];*/

	wsprintf(buff,"[%s %s] %s",GAMESERVER_TITLE, GAMESERVER_VERSION,gServerInfo.m_ServerName);
	/*wsprintf(playerbuff, "Players: %d/%d", gObjTotalUser,gServerInfo.m_ServerMaxUserNumber);
	wsprintf(monsterbuff, "Monsters: %d/%d", gObjTotalMonster,MAX_OBJECT_MONSTER);
	wsprintf(itembuff, "Items: %d", gObjTotalItem);
	wsprintf(offexpbuff, "Off-Attacks: %d", gObjTotalOffExp);
	wsprintf(offtradebuff, "Off-Traders: %d", gObjTotalOffTrade);*/

#ifdef IS_NEW_CLIENT
	strcat(buff," - New Client");
#elif GAMESERVER_ROOT
	strcat(buff, " - Normal (Test Server)");
#else
	strcat(buff," - Normal");
#endif

	SetWindowText(this->m_hwnd,buff);
	this->UpdateInfoBar();
	/*SendMessage(this->m_hStatusWindow, SB_SETTEXT, 0, (LONG)playerbuff);
	SendMessage(this->m_hStatusWindow, SB_SETTEXT, 1, (LONG)monsterbuff);
	SendMessage(this->m_hStatusWindow, SB_SETTEXT, 2, (LONG)itembuff);
	SendMessage(this->m_hStatusWindow, SB_SETTEXT, 3, (LONG)offexpbuff);
	SendMessage(this->m_hStatusWindow, SB_SETTEXT, 4, (LONG)offtradebuff);*/
}

void CServerDisplayer::UpdateInfoBar()
{
	char buff[256];

	wsprintf(buff, "Players: %d/%d", gObjTotalUser,gServerInfo.m_ServerMaxUserNumber);
	SendMessage(this->m_hStatusWindow, SB_SETTEXT, 0, (LONG)buff);

	wsprintf(buff, "Monsters: %d/%d", gObjTotalMonster,MAX_OBJECT_MONSTER);
	SendMessage(this->m_hStatusWindow, SB_SETTEXT, 1, (LONG)buff);

	wsprintf(buff, "Items: %d", gObjTotalItem);
	SendMessage(this->m_hStatusWindow, SB_SETTEXT, 2, (LONG)buff);

	wsprintf(buff, "Off-Attacks: %d", gObjTotalOffExp);
	SendMessage(this->m_hStatusWindow, SB_SETTEXT, 3, (LONG)buff);

	wsprintf(buff, "Off-Traders: %d", gObjTotalOffTrade);
	SendMessage(this->m_hStatusWindow, SB_SETTEXT, 4, (LONG)buff);

	wsprintf(buff, "Licensed to: %s", gProtection.CustomerName);
	SendMessage(this->m_hStatusWindow, SB_SETTEXT, 5, (LONG)buff);

	wsprintf(buff, "End license date: %s", gProtection.end_license_date_text);
	SendMessage(this->m_hStatusWindow, SB_SETTEXT, 6, (LONG)buff);
}

void CServerDisplayer::PaintAllInfo() // OK
{
	RECT rect;

	GetClientRect(this->m_hwnd,&rect);

	rect.top = 0;
	rect.bottom = 80;
	//rect.bottom = 50;

	HDC hdc = GetDC(this->m_hwnd);

	int OldBkMode = SetBkMode(hdc,TRANSPARENT);

	FillRect(hdc,&rect,this->m_brush[0]);

	/*if (this->m_Version == 1)
	{
		SetTextColor(hdc, RGB(60, 255, 0));
		TextOut(hdc, 12, 5, this->m_UpdatesText[0], strlen(this->m_UpdatesText[0]));
	}
	else if (this->m_Version == 0)
	{
		SetTextColor(hdc, RGB(0, 255, 255));
		TextOut(hdc, 12, 5, this->m_UpdatesText[1], strlen(this->m_UpdatesText[1]));
	}
	else
	{
		SetTextColor(hdc, RGB(255, 0, 0));
		TextOut(hdc, 12, 5, this->m_UpdatesText[2], strlen(this->m_UpdatesText[2]));
	}*/

	HPEN hPen = CreatePen(PS_SOLID, 1, RGB(0,255,255));
    HPEN hOldPen = (HPEN)SelectObject(hdc, hPen); 

	MoveToEx(hdc, GAMESERVER_WIDTH / 2 + 260, 0, nullptr);
	
	LineTo(hdc, GAMESERVER_WIDTH / 2 + 260, 80);

	SelectObject(hdc, hOldPen);
    DeleteObject(hPen);

	SetTextColor(hdc,RGB(0, 255, 255));

	for(int i = 0; i < 3; i++)
	{

		SIZE size;
        GetTextExtentPoint32(hdc, this->m_DeveloperText[i], strlen(this->m_DeveloperText[i]), &size );

		int x = GAMESERVER_WIDTH - 110 - size.cx/2;
		
		TextOut(hdc, x, ((i + 1) * 15), this->m_DeveloperText[i], strlen(this->m_DeveloperText[i]));
	}
	
	HFONT OldFont = (HFONT)SelectObject(hdc,this->m_font);

	if(gJoinServerConnection.CheckState() == 0 || gDataServerConnection.CheckState() == 0)
	{
		SetTextColor(hdc,RGB(0, 255, 255));
		SIZE size;
        GetTextExtentPoint32(hdc, this->m_DisplayerText[0], strlen(this->m_DisplayerText[0]), &size );
		int x = (GAMESERVER_WIDTH / 2 + 280) / 2 - size.cx / 2;

		TextOut(hdc,x,18,this->m_DisplayerText[0],strlen(this->m_DisplayerText[0]));
	}
	else
	{
#if (GAMESERVER_TYPE == 0)
		{
			gGameServerDisconnect = 0;

			SIZE size;
			GetTextExtentPoint32(hdc, this->m_DisplayerText[1], strlen(this->m_DisplayerText[1]), &size );
			int x = (GAMESERVER_WIDTH / 2 + 280) / 2 - size.cx / 2;

			if (gProtection.is_license_end)
			{
				SetTextColor(hdc, RGB(255, 0, 0));
				TextOut(hdc, x, 18, this->m_DisplayerText[3], strlen(this->m_DisplayerText[3]));
			}
			else
			{
				SetTextColor(hdc, RGB(0, 255, 255));
				TextOut(hdc, x, 18, this->m_DisplayerText[1], strlen(this->m_DisplayerText[1]));
			}
		}
#else
		{
			gGameServerDisconnect = 0;
			SetTextColor(hdc, RGB(0, 255, 255));

			SIZE size;
			GetTextExtentPoint32(hdc, this->m_DisplayerText[2], strlen(this->m_DisplayerText[2]), &size);
			int x = (GAMESERVER_WIDTH / 2 + 280) / 2 - size.cx / 2;

			if (gProtection.is_license_end)
				TextOut(hdc, x, 18, this->m_DisplayerText[3], strlen(this->m_DisplayerText[3]));
			else
				TextOut(hdc, x, 18,this->m_DisplayerText[2],strlen(this->m_DisplayerText[2]));
		}
#endif

	}

	SelectObject(hdc,OldFont);
	SetBkMode(hdc,OldBkMode);
	ReleaseDC(this->m_hwnd,hdc);
}

void CServerDisplayer::LogTextPaint() // OK
{
    RECT rect;
 
    rect.top = 80;
	rect.bottom = 50;
 
    HDC hdc = GetDC(this->m_hwnd);

	this->Interface(this->m_hwnd);
 
    int line = MAX_LOG_TEXT_LINE;
 
    int count = (((this->m_count-1)>=0)?(this->m_count-1):(MAX_LOG_TEXT_LINE-1));

	SetBkColor(hdc, BACKCOLOR);
 
    for(int n=0;n < MAX_LOG_TEXT_LINE;n++)
    {
        switch(this->m_log[count].color)
        {
            case LOG_BLACK:
                //SetTextColor(hdc, RGB(0, 0, 0));
                SetTextColor(hdc, RGB(127, 255, 212));
                SetBkColor(hdc, BACKCOLOR);
                break;
            case LOG_RED:
                SetTextColor(hdc, RGB(255, 0, 0));
                SetBkColor(hdc, BACKCOLOR);
                break;
            case LOG_GREEN:
                SetTextColor(hdc, RGB(0, 255, 0));
                SetBkColor(hdc, BACKCOLOR);
                break;
            case LOG_BLUE:
                SetTextColor(hdc, RGB(0, 100, 255));
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

void CServerDisplayer::Interface(HWND hWnd)
{
    HDC hDC = GetDC(this->m_hwnd);
    RECT rect;
    // ----
    GetClientRect(this->m_hwnd, &rect);
	rect.bottom -= 20;
    FillRect(hDC, &rect, this->m_brush[2]);
    SetBkMode(hDC, TRANSPARENT);
    ReleaseDC(this->m_hwnd, hDC);
}

void CServerDisplayer::CheckVersion()
{
	//std::string URL;
	//URL.resize(_MAX_PATH);
	//sprintf(&URL[0], gProtection.GetEncryptedString(gProtectString7, sizeof(gProtectString7)));
	//strcat(&URL[0],gProtection.m_ComputerHardwareId);

	//std::string readBuffer;
	//CURL *curl;
	//CURLcode res1;

	//// Открываем и читаем в буффер XML файл.
	//curl = curl_easy_init();
	//if(curl) {
	//	curl_easy_setopt(curl, CURLOPT_URL, URL.c_str());
	//	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, gProtection.WriteCallback);
	//	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
	//	res1 = curl_easy_perform(curl);
	//	curl_easy_cleanup(curl);
	//}

	//// Загружаем файл из буффера в pugixml.
	//pugi::xml_document file;
	//pugi::xml_parse_result res = file.load(readBuffer.c_str());

	//if(res.status != pugi::status_ok)
	//{
	//	this->m_Version = 3;
	//	gProtection.ConnectionErrorBox();
	//}
	//
	//pugi::xml_node LicenseInfo = file.child(gProtection.GetEncryptedString(gProtectAttr1, sizeof(gProtectAttr1))); // Открываем главный узел.
	//pugi::xml_node Node;
	//Node = LicenseInfo.child(gProtection.GetEncryptedString(gProtectAttr11, sizeof(gProtectAttr11))); // Открываем узел с основной информацией о лицензиях.

	//if (Node.attribute(gProtection.GetEncryptedString(gProtectAttr12,sizeof(gProtectAttr12))).empty())
	//{
	//	this->m_Version = 3;
	//	return;
	//}

	//char Version[10];
	//strcpy_s(Version, Node.attribute(gProtection.GetEncryptedString(gProtectAttr12,sizeof(gProtectAttr12))).as_string());

	//if( strcmp(Version, GAMESERVER_VERSION) )
	//{
	//	this->m_Version = 1;
	//	return;
	//}
	//this->m_Version = 0;
}