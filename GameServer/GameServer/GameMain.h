#pragma once

#include "Connection.h"
#include "Protocol.h"
#include "SocketManagerUdp.h"

struct SDHP_GAME_SERVER_LIVE_SEND
{
	PBMSG_HEAD header; // C1:01
//#if (SEASON9DOT3 == TRUE)
//	WORD ServerCode;
//	BYTE UserTotal;
//	BYTE PlayType;
//	WORD UserCount;
//#else
	WORD ServerCode;
	BYTE UserTotal;
	WORD UserCount;
	WORD AccountCount;
	WORD PCPointCount;
	WORD MaxUserCount;
//#endif
};

void GameMainInit(HWND hwnd);
void ConnectServerInfoSend();
bool JoinServerConnect(DWORD wMsg);
bool DataServerConnect(DWORD wMsg);
bool WebServerConnect(DWORD wMsg);
bool JoinServerReconnect(HWND hwnd,DWORD wMsg);
bool DataServerReconnect(HWND hwnd,DWORD wMsg);
void JoinServerMsgProc(WPARAM wParam,LPARAM lParam);
void DataServerMsgProc(WPARAM wParam,LPARAM lParam);
void CALLBACK QueueTimerCallback(PVOID lpParameter,BOOLEAN TimerOrWaitFired);

extern CConnection gJoinServerConnection;
extern CConnection gDataServerConnection;