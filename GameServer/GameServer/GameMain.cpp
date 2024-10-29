#include "stdafx.h"
#include "GameMain.h"
#include "BattleSoccerManager.h"
//#include "CommandManager.h"
#include "DSProtocol.h"
#include "HackCheck.h"
#include "JSProtocol.h"
#include "MasterSkillTree.h"
#include "MonsterAI.h"
#include "MonsterManager.h"
#include "ObjectManager.h"
#include "Protect.h"
#include "QueueTimer.h"
#include "ServerInfo.h"
#include "SocketManagerUdp.h"
#include "User.h"
#include "Util.h"
#include "MMOTOP.h"
#include "DupeScanner.h"

CConnection gJoinServerConnection;
CConnection gDataServerConnection;

void GameMainInit(HWND hwnd) // OK
{
	if(CreateMutex(nullptr,0,gServerInfo.m_ServerMutex) == nullptr || GetLastError() == ERROR_ALREADY_EXISTS)
	{
		ErrorMessageBox("Could not open GameServer");
		return;
	}

	PROTECT_START

	gObjInit();

//#if (SEASON9DOT3 == FALSE)
#ifndef IS_NEW_CLIENT
	InitHackCheck();
#endif
//#endif

	InitBattleSoccer();

	gGameServerLogOut = 0;

	gGameServerDisconnect = 0;

	gServerInfo.ReadInit();

	gObjSetExperienceTable();

	gMonsterManager.SetMonsterData();

	#if(GAMESERVER_UPDATE>=401)

	gMasterSkillTree.SetMasterLevelExperienceTable();

	#endif

	gJoinServerConnection.Init(hwnd,JoinServerProtocolCore);

	gDataServerConnection.Init(hwnd,DataServerProtocolCore);

	#if USE_TESTOFFEXP == TRUE
	if (gOffLevel.b_Enabled == true)
	{
		DWORD ThreadId;
		CreateThread(nullptr, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(OffLevelThread), nullptr, 0, &ThreadId);
	}
	#endif

	PROTECT_FINAL
}

void ConnectServerInfoSend() // OK
{
	PROTECT_START

	SDHP_GAME_SERVER_LIVE_SEND pMsg;

	pMsg.header.set(0x01,sizeof(pMsg));

	pMsg.ServerCode = (WORD)gServerInfo.m_ServerCode;

	pMsg.UserTotal = (BYTE)((gObjTotalUser==0)?0:((gObjTotalUser*100)/gServerInfo.m_ServerMaxUserNumber));

	pMsg.UserCount = (WORD)gObjTotalUser;

	pMsg.AccountCount = 0;

	pMsg.PCPointCount = 0;

	pMsg.MaxUserCount = (WORD)gServerInfo.m_ServerMaxUserNumber;

	gSocketManagerUdp.DataSend((BYTE*)&pMsg,pMsg.header.size);

	PROTECT_FINAL
}

bool JoinServerConnect(DWORD wMsg) // OK
{
	if(gJoinServerConnection.Connect(gServerInfo.m_JoinServerAddress,(WORD)gServerInfo.m_JoinServerPort,wMsg) == 0)
	{
		return false;
	}

	GJServerInfoSend();
	return true;
}

bool DataServerConnect(DWORD wMsg) // OK
{
	if (gDataServerConnection.Connect(gServerInfo.m_DataServerAddress, (WORD)gServerInfo.m_DataServerPort, wMsg) == 0)
	{
		return false;
	}

	GDServerInfoSend();
	return true;
}

bool JoinServerReconnect(HWND hwnd,DWORD wMsg) // OK
{
	if(gJoinServerConnection.CheckState() == 0)
	{
		gJoinServerConnection.Init(hwnd,JoinServerProtocolCore);
		return JoinServerConnect(wMsg);
	}

	return true;
}

bool WebServerConnect(DWORD wMsg)
{
	
}


bool DataServerReconnect(HWND hwnd,DWORD wMsg) // OK
{
	if(gDataServerConnection.CheckState() == 0)
	{
		gDataServerConnection.Init(hwnd,DataServerProtocolCore);
		return DataServerConnect(wMsg);
	}

	return true;
}

void JoinServerMsgProc(WPARAM wParam,LPARAM lParam) // OK
{
	switch(LOWORD(lParam))
	{
		case FD_READ:
			gJoinServerConnection.DataRecv();
			break;
		case FD_WRITE:
			gJoinServerConnection.DataSendEx();
			break;
		case FD_CLOSE:
			gJoinServerConnection.Disconnect();
			gObjAllDisconnect();
			break;
	}
}

void DataServerMsgProc(WPARAM wParam,LPARAM lParam) // OK
{
	switch(LOWORD(lParam))
	{
		case FD_READ:
			gDataServerConnection.DataRecv();
			break;
		case FD_WRITE:
			gDataServerConnection.DataSendEx();
			break;
		case FD_CLOSE:
			gDataServerConnection.Disconnect();
			gObjAllDisconnect();
			break;
	}
}

void CALLBACK QueueTimerCallback(PVOID lpParameter,BOOLEAN TimerOrWaitFired) // OK
{
	PROTECT_START

	static CCriticalSection critical;

	critical.lock();

	switch(((int)lpParameter))
	{
		case QUEUE_TIMER_MONSTER:
			gObjectManager.ObjectMonsterAndMsgProc();
			//gWebServer.listen();
			break;
		case QUEUE_TIMER_MONSTER_MOVE:
			gObjectManager.ObjectMoveProc();
			break;
		case QUEUE_TIMER_MONSTER_AI:
			CMonsterAI::MonsterAIProc();
			break;
		case QUEUE_TIMER_MONSTER_AI_MOVE:
			CMonsterAI::MonsterMoveProc();
			break;
		case QUEUE_TIMER_EVENT:
			gObjEventRunProc();
			break;
		case QUEUE_TIMER_VIEWPORT:
			gObjViewportProc();
			break;
		case QUEUE_TIMER_FIRST:
			gObjFirstProc();
			break;
		case QUEUE_TIMER_CLOSE:
			gObjCloseProc();
			break;
		case QUEUE_TIMER_MATH_AUTHENTICATOR:
			gObjMathAuthenticatorProc();
			break;
		case QUEUE_TIMER_ACCOUNT_LEVEL:
			gObjAccountLevelProc();
			break;
		case QUEUE_TIMER_VOTE_PARCER:
#if(GAMESERVER_TYPE==0)
			//gVoteParcer.Load(gPath.GetFullPath("..\\LTPData\\Vote.ini"));
#endif
			gMMOTOP.Tick();
			break;
		case QUEUE_TIMER_CHECK_VERSION:
			//gServerDisplayer.CheckVersion();
			break;
		case QUEUE_TIMER_DUPE_SCANNER:
			gDupeScanner.scan();
			break;
	}

	critical.unlock();

	PROTECT_FINAL
}
