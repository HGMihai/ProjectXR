#include "stdafx.h"
#include "JSProtocol.h"
#include "CustomAttack.h"
#include "CustomStore.h"
#include "DSProtocol.h"
#include "GameMain.h"
#include "Log.h"
#include "MapServerManager.h"
#include "Message.h"
#include "Notice.h"
#include "ObjectManager.h"
#include "Protect.h"
#include "ServerInfo.h"
#include "Util.h"
#include "IpManager.h"
#include "DisableFunctions.h"

void JoinServerProtocolCore(BYTE head,BYTE* lpMsg,int size) // OK
{
	PROTECT_START

	switch(head)
	{
		case 0x00:
			JGServerInfoRecv((SDHP_JOIN_SERVER_INFO_RECV*)lpMsg);
			break;
		case 0x01:
			JGConnectAccountRecv((SDHP_CONNECT_ACCOUNT_RECV*)lpMsg);
			break;
		case 0x02:
			JGDisconnectAccountRecv((SDHP_DISCONNECT_ACCOUNT_RECV*)lpMsg);
			break;
		case 0x03:
			JGMapServerMoveRecv((SDHP_MAP_SERVER_MOVE_RECV*)lpMsg);
			break;
		case 0x04:
			JGMapServerMoveAuthRecv((SDHP_MAP_SERVER_MOVE_AUTH_RECV*)lpMsg);
			break;
		case 0x05:
			JGAccountLevelRecv((SDHP_ACCOUNT_LEVEL_RECV*)lpMsg);
			break;
		case 0x30:
			JGAccountAlreadyConnectedRecv((SDHP_ACCOUNT_ALREADY_CONNECTED_RECV*)lpMsg);
			break;
		case 0x31:
			JGDisconnectAccountOffExpRecv((SDHP_DISCONNECT_ACCOUNT_RECV*)lpMsg);
			break;
		case 0x41:
			JGLastVoteTimeRecv((SDHP_ACCOUNT_VOTE_TIME_RECV*)lpMsg);
			break;
	}

	PROTECT_FINAL
}

void JGServerInfoRecv(SDHP_JOIN_SERVER_INFO_RECV* lpMsg) // OK
{

}

void JGConnectAccountRecv(SDHP_CONNECT_ACCOUNT_RECV* lpMsg) // OK
{
	if(OBJECT_RANGE(lpMsg->index) == 0)
	{
		return;
	}

	if((gObj[lpMsg->index].LoginMessageSend--) == 0)
	{
		CloseClient(lpMsg->index);
		return;
	}

	if(gObj[lpMsg->index].Connected != OBJECT_CONNECTED)
	{
		GJDisconnectAccountSend(lpMsg->index,lpMsg->account,gObj[lpMsg->index].IpAddr);
		return;
	}

	if(lpMsg->result != 1)
	{
		GCConnectAccountSend(lpMsg->index,((gObj[lpMsg->index].LoginMessageCount>=3)?8:lpMsg->result));
		return;
	}

	if(lpMsg->BlockCode != 0)
	{
		GJDisconnectAccountSend(lpMsg->index,lpMsg->account,gObj[lpMsg->index].IpAddr);
		GCConnectAccountSend(lpMsg->index,5);
		return;
	}

	if(gServerInfo.m_ServerLock > lpMsg->AccountLevel)
	{
		GJDisconnectAccountSend(lpMsg->index,lpMsg->account,gObj[lpMsg->index].IpAddr);
		GCConnectAccountSend(lpMsg->index,9);
		return;
	}

	//if(gIpManager.CheckIpAddress(gObj[lpMsg->index].IpAddr) == 0)
	//{
	//	GJDisconnectAccountSend(lpMsg->index,lpMsg->account,gObj[lpMsg->index].IpAddr);
	//	GCConnectAccountSend(lpMsg->index,6);
	//	return;
	//}

	gObj[lpMsg->index].Connected = OBJECT_LOGGED;

	memcpy(gObj[lpMsg->index].Account,lpMsg->account,sizeof(gObj[lpMsg->index].Account));

	memcpy(gObj[lpMsg->index].Password, lpMsg->password, sizeof(gObj[lpMsg->index].Password));

	memcpy(gObj[lpMsg->index].PersonalCode,lpMsg->PersonalCode,sizeof(gObj[lpMsg->index].PersonalCode));

	gObj[lpMsg->index].AccountLevel = lpMsg->AccountLevel;

	memcpy(gObj[lpMsg->index].AccountExpireDate,lpMsg->AccountExpireDate,sizeof(gObj[lpMsg->index].AccountExpireDate));

	memcpy(gObj[lpMsg->index].region,lpMsg->region,sizeof(gObj[lpMsg->index].region));

	//LogAdd(LOG_TEST,"[Region] Account: (%s), Region: (%s)",lpMsg->account,lpMsg->region);

	gDisableFunctions.GC_SendDisable(lpMsg->index);
	
	GCConnectAccountSend(lpMsg->index,1);
#if USE_OFFEXP_RELOAD == TRUE
	OFFEXP_DATA* info = gCustomAttack.GetOffExpInfoByAccount(&gObj[lpMsg->index]);

	if (info != 0 && gObj[lpMsg->index].Socket == INVALID_SOCKET)
	{
		//memcpy(gObj[lpMsg->index].Account,it->second.Account,sizeof(gObj[lpMsg->index].Account));
		GDCharacterInfoSend(lpMsg->index,info->Name);

		gObj[lpMsg->index].IsOffExpReconnect = true;
	}
#endif
	LogAdd(LOG_BLACK,"[ObjectManager][%d] AddAccountInfo (%s)",lpMsg->index,gObj[lpMsg->index].Account);

	GJGetLastVoteInfo(lpMsg->index);
}

void JGDisconnectAccountRecv(SDHP_DISCONNECT_ACCOUNT_RECV* lpMsg) // OK
{
	if(gObjIsAccountValid(lpMsg->index,lpMsg->account) != 0)
	{
		if (gObj[lpMsg->index].AttackCustomOffline != 0 || gObj[lpMsg->index].PShopCustomOffline != 0)
		{
			//gObj[lpMsg->index].AttackCustomOffline = 0;
			//gObj[lpMsg->index].AttackCustomOfflineTime = 0;
			//gObj[lpMsg->index].AttackCustomOfflineMoneyDelay = 0;
			gObjDelOffExp(lpMsg->index);
		}
		CloseClient(lpMsg->index);
	}
}

void JGDisconnectAccountOffExpRecv(SDHP_DISCONNECT_ACCOUNT_RECV* lpMsg) // OK
{
	if(gObjIsAccountValid(lpMsg->index,lpMsg->account) != 0)
	{
		gObjDelOffExp(lpMsg->index);
	}
}

void JGMapServerMoveRecv(SDHP_MAP_SERVER_MOVE_RECV* lpMsg) // OK
{
	if(gObjIsAccountValid(lpMsg->index,lpMsg->account) == 0)
	{
		LogAdd(LOG_RED,"[JGMapServerMoveRecv] Invalid Account [%d](%s)",lpMsg->index,lpMsg->account);
		CloseClient(lpMsg->index);
		return;
	}

	PMSG_MAP_SERVER_MOVE_SEND pMsg = {0};

	pMsg.header.setE(0xB1,0x00,sizeof(pMsg));

	if(lpMsg->result == 0)
	{
		gObj[lpMsg->index].State = OBJECT_PLAYING;
		DataSend(lpMsg->index,(BYTE*)&pMsg,pMsg.header.size);
		gNotice.GCNoticeSend(lpMsg->index,0,0,0,0,0,0,gMessage.GetMessage(259));
		return;
	}

	if(gMapServerManager.GetMapServerData(lpMsg->NextServerCode,pMsg.IpAddress,&pMsg.ServerPort) == 0)
	{
		gObj[lpMsg->index].State = OBJECT_PLAYING;
		GJMapServerMoveCancelSend(lpMsg->index);
		DataSend(lpMsg->index,(BYTE*)&pMsg,pMsg.header.size);
		gNotice.GCNoticeSend(lpMsg->index,0,0,0,0,0,0,gMessage.GetMessage(259));
		return;
	}

	pMsg.ServerCode = gMapServerManager.GetMapServerGroup();

	pMsg.AuthCode1 = lpMsg->AuthCode1;

	pMsg.AuthCode2 = lpMsg->AuthCode2;

	pMsg.AuthCode3 = lpMsg->AuthCode3;

	pMsg.AuthCode4 = lpMsg->AuthCode4;

	DataSend(lpMsg->index,(BYTE*)&pMsg,pMsg.header.size);

	gObjectManager.CharacterGameClose(lpMsg->index);

	gObj[lpMsg->index].MapServerMoveQuit = 1;

	gObj[lpMsg->index].MapServerMoveQuitTickCount = GetTickCount();
}

void JGMapServerMoveAuthRecv(SDHP_MAP_SERVER_MOVE_AUTH_RECV* lpMsg) // OK
{
	if(OBJECT_RANGE(lpMsg->index) == 0)
	{
		return;
	}

	if((gObj[lpMsg->index].LoginMessageSend--) == 0)
	{
		CloseClient(lpMsg->index);
		return;
	}

	if(gObj[lpMsg->index].MapServerMoveRequest == 0)
	{
		CloseClient(lpMsg->index);
		return;
	}

	if(gObj[lpMsg->index].Connected != OBJECT_CONNECTED)
	{
		GJDisconnectAccountSend(lpMsg->index,lpMsg->account,gObj[lpMsg->index].IpAddr);
		return;
	}

	if(lpMsg->result != 1)
	{
		GCMapServerMoveAuthSend(lpMsg->index,((gObj[lpMsg->index].LoginMessageCount>=3)?8:lpMsg->result));
		return;
	}

	if(lpMsg->BlockCode != 0)
	{
		GJDisconnectAccountSend(lpMsg->index,lpMsg->account,gObj[lpMsg->index].IpAddr);
		GCMapServerMoveAuthSend(lpMsg->index,5);
		return;
	}

	if(gServerInfo.m_ServerLock > lpMsg->AccountLevel)
	{
		GJDisconnectAccountSend(lpMsg->index,lpMsg->account,gObj[lpMsg->index].IpAddr);
		GCMapServerMoveAuthSend(lpMsg->index,9);
		return;
	}

	gObj[lpMsg->index].Connected = OBJECT_LOGGED;

	memcpy(gObj[lpMsg->index].Account,lpMsg->account,sizeof(gObj[lpMsg->index].Account));
	
	memcpy(gObj[lpMsg->index].region,lpMsg->region,sizeof(gObj[lpMsg->index].region));

	memcpy(gObj[lpMsg->index].PersonalCode,lpMsg->PersonalCode,sizeof(gObj[lpMsg->index].PersonalCode));

	gObj[lpMsg->index].AccountLevel = lpMsg->AccountLevel;

	memcpy(gObj[lpMsg->index].AccountExpireDate,lpMsg->AccountExpireDate,sizeof(gObj[lpMsg->index].AccountExpireDate));

	GCMapServerMoveAuthSend(lpMsg->index,1);

	gObj[lpMsg->index].EnableDelCharacter = 0;

	gObj[lpMsg->index].LastServerCode = lpMsg->LastServerCode;

	gObj[lpMsg->index].DestMap  = lpMsg->map;

	gObj[lpMsg->index].DestX = lpMsg->x;

	gObj[lpMsg->index].DestY = lpMsg->y;

	GDCharacterInfoSend(lpMsg->index,lpMsg->name);

	LogAdd(LOG_BLACK,"[ObjectManager][%d] AddAccountInfo (%s)",lpMsg->index,gObj[lpMsg->index].Account);
}

void JGAccountLevelRecv(SDHP_ACCOUNT_LEVEL_RECV* lpMsg) // OK
{
	if(gObjIsAccountValid(lpMsg->index,lpMsg->account) == 0)
	{
		LogAdd(LOG_RED,"[JGAccountLevelRecv] Invalid Account [%d](%s)",lpMsg->index,lpMsg->account);
		CloseClient(lpMsg->index);
		return;
	}

	if(gServerInfo.m_ServerLock > lpMsg->AccountLevel)
	{
		gObjUserKill(lpMsg->index);
		return;
	}

	if(gObj[lpMsg->index].AccountLevel == lpMsg->AccountLevel && strcmp(gObj[lpMsg->index].AccountExpireDate,lpMsg->AccountExpireDate) == 0)
	{
		gObj[lpMsg->index].AccountLevel = lpMsg->AccountLevel;
		memcpy(gObj[lpMsg->index].AccountExpireDate,lpMsg->AccountExpireDate,sizeof(gObj[lpMsg->index].AccountExpireDate));
	}
	else
	{
		gObj[lpMsg->index].AccountLevel = lpMsg->AccountLevel;
		memcpy(gObj[lpMsg->index].AccountExpireDate,lpMsg->AccountExpireDate,sizeof(gObj[lpMsg->index].AccountExpireDate));
		gNotice.GCNoticeSend(lpMsg->index,1,0,0,0,0,0,gMessage.GetMessage((248+gObj[lpMsg->index].AccountLevel)),gObj[lpMsg->index].AccountExpireDate);
	}

	if (gEffectManager.CheckEffectByGroup(&gObj[lpMsg->index],117))
	{
		gEffectManager.DelEffectByGroup(&gObj[lpMsg->index],117);
	}

	if (gObj[lpMsg->index].AccountLevel != 0)
	{
		gEffectManager.AddEffect(&gObj[lpMsg->index],0,EFFECT_PREMIUM_BUFF1+(gObj[lpMsg->index].AccountLevel-1),0,0,0,0,0);
	}
}

void JGAccountAlreadyConnectedRecv(SDHP_ACCOUNT_ALREADY_CONNECTED_RECV* lpMsg) // OK
{
	/*if(gObjIsAccountValidPassword(lpMsg->index,lpMsg->account,lpMsg->password) == 0)
	{
		LogAdd(LOG_RED,"[JGAccountAlreadyConnectedRecv] Invalid Account or wrong password [%d](%s)",lpMsg->index,lpMsg->account);
		CloseClient(lpMsg->index);
		return;
	}
	else
	{*/
	if (gObjIsConnectedGP(lpMsg->index) && strcmp(lpMsg->password,gObj[lpMsg->index].Password) == 0 && gServerInfo.m_DisconnectOnConenct[gObj[lpMsg->index].AccountLevel] == 1)
	{
		gObjUserKill(lpMsg->index);
	}

	if (gObj[lpMsg->index].AttackCustomOffline != 0 || gObj[lpMsg->index].PShopCustomOffline != 0)
	{
		if(gObjIsAccountValidPassword(lpMsg->index,lpMsg->account,lpMsg->password) != 0)
		{
			#if USE_TESTOFFEXP == TRUE
			gOffLevel.OnAttackAlreadyConnected(&gObj[lpMsg->index]);
			#endif
			gCustomAttack.OnAttackAlreadyConnected(&gObj[lpMsg->index]);
			gCustomStore.OnPShopAlreadyConnected(&gObj[lpMsg->index]);
		}
	}
	//}
	
}

void GJServerInfoSend() // OK
{
	SDHP_JOIN_SERVER_INFO_SEND pMsg;

	pMsg.header.set(0x00,sizeof(pMsg));

	pMsg.type = 1;

	pMsg.ServerPort = (WORD)gServerInfo.m_ServerPort;

	strcpy_s(pMsg.ServerName,gServerInfo.m_ServerName);

	pMsg.ServerCode = (WORD)gServerInfo.m_ServerCode;

	gJoinServerConnection.DataSend((BYTE*)&pMsg,pMsg.header.size);
}

void GJConnectAccountSend(int aIndex,char* account,char* password,char* IpAddress) // OK
{
	SDHP_CONNECT_ACCOUNT_SEND pMsg;

	pMsg.header.set(0x01,sizeof(pMsg));

	pMsg.index = aIndex;

	memcpy(pMsg.account,account,sizeof(pMsg.account));

	memcpy(pMsg.password,password,sizeof(pMsg.password));

	memcpy(pMsg.IpAddress,IpAddress,sizeof(pMsg.IpAddress));

	gJoinServerConnection.DataSend((BYTE*)&pMsg,pMsg.header.size);
}

void GJDisconnectAccountSend(int aIndex,char* account,char* IpAddress) // OK
{
	SDHP_DISCONNECT_ACCOUNT_SEND pMsg;

	pMsg.header.set(0x02,sizeof(pMsg));

	pMsg.index = aIndex;

	memcpy(pMsg.account,account,sizeof(pMsg.account));

	memcpy(pMsg.IpAddress,IpAddress,sizeof(pMsg.IpAddress));

	gJoinServerConnection.DataSend((BYTE*)&pMsg,pMsg.header.size);
}

void GJMapServerMoveSend(int aIndex,WORD NextServerCode,WORD map,BYTE x,BYTE y) // OK
{
	SDHP_MAP_SERVER_MOVE_SEND pMsg;

	pMsg.header.set(0x03,sizeof(pMsg));

	pMsg.index = aIndex;

	memcpy(pMsg.account,gObj[aIndex].Account,sizeof(pMsg.account));

	memcpy(pMsg.name,gObj[aIndex].Name,sizeof(pMsg.name));

	pMsg.GameServerCode = (WORD)gServerInfo.m_ServerCode;

	pMsg.NextServerCode = NextServerCode;

	pMsg.map = map;

	pMsg.x = x;

	pMsg.y = y;

	gJoinServerConnection.DataSend((BYTE*)&pMsg,pMsg.header.size);
}

void GJSetOffExpSend(LPOBJ lpObj, bool isOffExp)
{
	SDHP_SET_OFFEXP_SEND pMsg;

	pMsg.header.set(0x40,sizeof(pMsg));

	pMsg.index = lpObj->Index;

	memcpy(pMsg.account,lpObj->Account,sizeof(pMsg.account));

	memcpy(pMsg.name,lpObj->Name,sizeof(pMsg.name));

	pMsg.map = lpObj->Map;

	pMsg.x = lpObj->X;

	pMsg.y = lpObj->Y;

	pMsg.isOffExp = isOffExp;

	gJoinServerConnection.DataSend((BYTE*)&pMsg,pMsg.header.size);
}

void GJMapServerMoveAuthSend(int aIndex,char* account,char* name,DWORD AuthCode1,DWORD AuthCode2,DWORD AuthCode3,DWORD AuthCode4) // OK
{
	SDHP_MAP_SERVER_MOVE_AUTH_SEND pMsg;

	pMsg.header.set(0x04,sizeof(pMsg));

	pMsg.index = aIndex;

	memcpy(pMsg.account,account,sizeof(pMsg.account));

	memcpy(pMsg.name,name,sizeof(pMsg.name));

	memcpy(pMsg.region,gObj[aIndex].region,sizeof(pMsg.region));

	pMsg.LastServerCode = (WORD)gServerInfo.m_ServerCode;

	pMsg.AuthCode1 = AuthCode1;

	pMsg.AuthCode2 = AuthCode2;

	pMsg.AuthCode3 = AuthCode3;

	pMsg.AuthCode4 = AuthCode4;

	gJoinServerConnection.DataSend((BYTE*)&pMsg,pMsg.header.size);
}

void GJAccountLevelSend(int aIndex) // OK
{
	SDHP_ACCOUNT_LEVEL_SEND pMsg;

	pMsg.header.set(0x05,sizeof(pMsg));

	pMsg.index = aIndex;

	memcpy(pMsg.account,gObj[aIndex].Account,sizeof(pMsg.account));

	gJoinServerConnection.DataSend((BYTE*)&pMsg,pMsg.header.size);
}

void GJMapServerMoveCancelSend(int aIndex) // OK
{
	SDHP_MAP_SERVER_MOVE_CANCEL_SEND pMsg;

	pMsg.header.set(0x10,sizeof(pMsg));

	pMsg.index = aIndex;

	memcpy(pMsg.account,gObj[aIndex].Account,sizeof(pMsg.account));

	gJoinServerConnection.DataSend((BYTE*)&pMsg,pMsg.header.size);
}

void GJAccountLevelSaveSend(int aIndex,int AccountLevel,int AccountExpireTime) // OK
{
	SDHP_ACCOUNT_LEVEL_SAVE_SEND pMsg;

	pMsg.header.set(0x11,sizeof(pMsg));

	pMsg.index = aIndex;

	memcpy(pMsg.account,gObj[aIndex].Account,sizeof(pMsg.account));

	pMsg.AccountLevel = AccountLevel;

	pMsg.AccountExpireTime = AccountExpireTime;

	gJoinServerConnection.DataSend((BYTE*)&pMsg,pMsg.header.size);
}

void GJServerUserInfoSend() // OK
{
	PROTECT_START

	SDHP_SERVER_USER_INFO_SEND pMsg;

	pMsg.header.set(0x20,sizeof(pMsg));

	pMsg.CurUserCount = (WORD)gObjTotalUser;

	pMsg.MaxUserCount = (WORD)gServerInfo.m_ServerMaxUserNumber;

	gJoinServerConnection.DataSend((BYTE*)&pMsg,pMsg.header.size);

	PROTECT_FINAL
}

void GJLastVoteTimeSave(int aIndex)
{
	SDHP_ACCOUNT_VOTE_TIME_SAVE pMsg;

	pMsg.header.set(0x42,sizeof(pMsg));

	memcpy(pMsg.account,gObj[aIndex].Account,sizeof(pMsg.account));

	pMsg.last_vote_time = gObj[aIndex].last_vote_time;

	//LogAdd(LOG_TEST,"[VoteTime Save] LastVoteTime: %d",pMsg.last_vote_time);

	gJoinServerConnection.DataSend((BYTE*)&pMsg,pMsg.header.size);
}

void GJGetLastVoteInfo(int aIndex)
{
	SDHP_ACCOUNT_VOTE_TIME_RECV pMsg;

	pMsg.header.set(0x41,sizeof(pMsg));

	memcpy(pMsg.account,gObj[aIndex].Account,sizeof(pMsg.account));

	pMsg.index = aIndex;

	pMsg.last_vote_time = 0;

	gJoinServerConnection.DataSend((BYTE*)&pMsg,pMsg.header.size);
}

void JGLastVoteTimeRecv(SDHP_ACCOUNT_VOTE_TIME_RECV* lpMsg)
{
	if(gObjIsAccountValid(lpMsg->index,lpMsg->account) == 0)
	{
		return;
	}

	gObj[lpMsg->index].last_vote_time = lpMsg->last_vote_time;

	//LogAdd(LOG_TEST,"[VoteTime Recv] LastVoteTime: %d",lpMsg->last_vote_time);
}

void GJSetBlockCodeSend(int aIndex, BYTE BlockCode)
{
	SDHP_SET_BLOCK_CODE_SEND pMsg;

	pMsg.header.set(0x43,sizeof(pMsg));

	memcpy(pMsg.account,gObj[aIndex].Account,sizeof(pMsg.account));

	pMsg.BlockCode = BlockCode;

	gJoinServerConnection.DataSend((BYTE*)&pMsg,pMsg.header.size);
}