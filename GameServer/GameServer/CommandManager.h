// CommandManager.h: interface for the CCommandManager class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "Protocol.h"
#include "User.h"

#define MAX_COMMAND 77

#define COMMAND_YES "/yes"
#define COMMAND_NO "/no"

#define TEST_COMMAND_FIX 1

enum eCommandNumber
{
	COMMAND_MOVE = 1,
	COMMAND_POST = 2,
	COMMAND_ADD_POINT1 = 3,
	COMMAND_ADD_POINT2 = 4,
	COMMAND_ADD_POINT3 = 5,
	COMMAND_ADD_POINT4 = 6,
	COMMAND_ADD_POINT5 = 7,
	COMMAND_PK_CLEAR = 8,
	COMMAND_MONEY = 9,
	COMMAND_CHANGE = 10,
	COMMAND_WARE = 11,
	COMMAND_RESET = 12,
	COMMAND_GM_MOVE = 13,
	COMMAND_GM_POST = 14,
	COMMAND_TRACK = 15,
	COMMAND_TRACE = 16,
	COMMAND_DISCONNECT = 17,
	COMMAND_FIREWORKS = 18,
	COMMAND_MAKE = 19,
	COMMAND_SKIN = 20,
	COMMAND_SET_MONEY = 21,
	COMMAND_NOTICE = 22,
	COMMAND_SET_CREDITS = 23,
	COMMAND_GUILD_WAR = 24,
	COMMAND_BATTLE_SOCCER = 25,
	COMMAND_REQUEST = 26,
	COMMAND_CUSTOM_STORE = 27,
	COMMAND_CUSTOM_STORE_OFFLINE = 28,
	COMMAND_HIDE = 29,
	COMMAND_CUSTOM_ATTACK = 30,
	COMMAND_CUSTOM_ATTACK_OFFLINE = 31,
	COMMAND_CUSTOM_TELEPORTER = 32,
	COMMAND_CLEARINVENTORY = 33,
	COMMAND_DCFRIEND = 34,
	COMMAND_ISMARRY = 35,
	COMMAND_ISMARRYONLINE = 36,
	COMMAND_TRACEMARRY = 37,
	COMMAND_DIVORCE = 38,
	COMMAND_OFFEXP = 39,
	COMMAND_PICKUP = 40,
	COMMAND_QUEST = 41,
	COMMAND_PARTYLEADER = 42,
	COMMAND_DROP = 43,
	COMMAND_CHATBAN = 44,
	COMMAND_CHATUNBAN = 45,
	COMMAND_MARRYPROP = 46,
	COMMAND_MARRYACCEPT = 47,
	COMMAND_MARRYTP = 48,
	COMMAND_MARRYDIVORCE = 49,
	COMMAND_MLRESET = 50,
	//COMMAND_MONSTERCALL = 51,
	COMMAND_MONSTERSPAWN = 51,
	COMMAND_EXCHANGEWCOINP = 52,
	COMMAND_SEND1 = 53,
	COMMAND_SEND2 = 54,
	COMMAND_SEND3 = 55,
	COMMAND_SEND4 = 56,
	COMMAND_SEND5 = 57,
	COMMAND_SEND6 = 58,
	COMMAND_RECV1 = 59,
	COMMAND_RECV2 = 60,
	COMMAND_RECV3 = 61,
	COMMAND_RECV4 = 62,
	COMMAND_RECV5 = 63,
	COMMAND_RECV6 = 64,
	COMMAND_PACKIU = 65,
	COMMAND_UNPACKIU = 66,
	COMMAND_SENDGUARDIAN = 67,
	COMMAND_SENDHARMONY = 68,
	COMMAND_SENDLOWSTONE = 69,
	COMMAND_SENDHIGHSTONE = 70,
	COMMAND_SENDGEMSTONE = 71,
	COMMAND_RECVGUARDIAN = 72,
	COMMAND_RECVHARMONY = 73,
	COMMAND_RECVLOWSTONE = 74,
	COMMAND_RECVHIGHSTONE = 75,
	COMMAND_RECVGEMSTONE = 76,
	COMMAND_ADD_POINT6 = 77,
};

//**********************************************//
//********** DataServer -> GameServer **********//
//**********************************************//

struct SDHP_COMMAND_RESET_RECV
{
	PSBMSG_HEAD header; // C1:0F:00
	WORD index;
	char account[11];
	char name[11];
	UINT ResetDay;
	UINT ResetWek;
	UINT ResetMon;
};

struct SDHP_COMMAND_MASTER_RESET_RECV
{
	PSBMSG_HEAD header; // C1:0F:01
	WORD index;
	char account[11];
	char name[11];
	UINT MasterResetDay;
	UINT MasterResetWek;
	UINT MasterResetMon;
};

//**********************************************//
//********** GameServer -> DataServer **********//
//**********************************************//

struct SDHP_COMMAND_RESET_SEND
{
	PSBMSG_HEAD header; // C1:0F:00
	WORD index;
	char account[11];
	char name[11];
};

struct SDHP_COMMAND_MASTER_RESET_SEND
{
	PSBMSG_HEAD header; // C1:0F:01
	WORD index;
	char account[11];
	char name[11];
};

//**********************************************//
//**********************************************//
//**********************************************//

struct COMMAND_INFO
{
	int code;
	char label[32];
	int Active;
	int GMCode;
	int MinLevel;
	int MinReset;
	int PriceType;
	int Price[MAX_ACCOUNT_LEVEL];
	int AccountLevel;
};

class CCommandManager
{
public:
	CCommandManager();
	virtual ~CCommandManager();
	void Init();
	void Load(char* path);
	void MainProc();
	void Add(COMMAND_INFO info);
	void Add(char* label, int code);
	long GetNumber(char* arg,int pos);
	void GetString(char* arg,char* out,int size,int pos);
	COMMAND_INFO* GetCommandInfo(int index);
	long GetCommandCode(char* label);
	char* GetLabelByCode(int Code);
	//int CheckCommand(LPOBJ lpObj, int CommandCode);
#if (TEST_COMMAND_FIX != 1)
	bool ManagementCore(LPOBJ lpObj,char* message);
#else
	void ManagementCore(LPOBJ lpObj,char* message);
#endif
	static void ManagementCodeCallback(LPOBJ lpObj,char message[MAX_CHAT_MESSAGE_SIZE],DWORD slot,DWORD WCoinC,DWORD WCoinP,DWORD GoblinPoint);
	void CommandMove(LPOBJ lpObj,char* arg);
	void CommandPost(LPOBJ lpObj,char* arg);
	void CommandAddPoint(LPOBJ lpObj,char* arg,int type);
	void CommandAddPointAuto(LPOBJ lpObj,char* arg,int type);
	void CommandPKClear(LPOBJ lpObj,char* arg);
	void CommandMoney(LPOBJ lpObj,char* arg);
	void CommandChange(LPOBJ lpObj,char* arg);
	void CommandWare(LPOBJ lpObj,char* arg);
	void CommandReset(LPOBJ lpObj,char* arg);
	void CommandResetAuto(LPOBJ lpObj,char* arg);
	void CommandGMMove(LPOBJ lpObj,char* arg);
	void CommandGMPost(LPOBJ lpObj,char* arg);
	void CommandTrack(LPOBJ lpObj,char* arg);
	void CommandTrace(LPOBJ lpObj,char* arg);
	void CommandDisconnect(LPOBJ lpObj,char* arg);
	void CommandFireworks(LPOBJ lpObj,char* arg);
	void CommandMake(LPOBJ lpObj,char* arg);
	void CommandDrop(LPOBJ lpObj, char* arg);
	void CommandSkin(LPOBJ lpObj,char* arg);
	void CommandSetMoney(LPOBJ lpObj,char* arg);
	void CommandNotice(LPOBJ lpObj,char* arg);
	void CommandSetCretits(LPOBJ lpObj,char* arg);
	//void CommandMasterReset(LPOBJ lpObj,char* arg);
	void CommandGuildWar(LPOBJ lpObj,char* arg);
	void CommandBattleSoccer(LPOBJ lpObj,char* arg);
	void CommandRequest(LPOBJ lpObj,char* arg);
	void CommandHide(LPOBJ lpObj,char* arg);
	void CommandAddPointAutoProc(LPOBJ lpObj);
	void CommandResetAutoProc(LPOBJ lpObj);
	void CommandClearInventory(LPOBJ lpObj);
	void CommandDCFriend(LPOBJ lpObj,char* arg);
	void DGCommandResetRecv(SDHP_COMMAND_RESET_RECV* lpMsg);
	//void DGCommandMasterResetRecv(SDHP_COMMAND_MASTER_RESET_RECV* lpMsg);
	void CommandQuest(LPOBJ lpObj);
	void CommandPartyLeader(LPOBJ lpObj, char* arg);
	void CommandChatBan(LPOBJ lpObj, char* arg);
	void CommandChatUnBan(LPOBJ lpObj, char* arg);
	void CommandMarryProp(LPOBJ lpObj, char* arg);
	void CommandMarryAccept(LPOBJ lpObj, char* arg);
	void CommandMarryTP(LPOBJ lpObj, char* arg);
	void CommandMarryDivorce(LPOBJ lpObj, char* arg);
	void CommandMonsterCall(LPOBJ lpObj, char* arg);
	void CommandExchangeWcoinP(LPOBJ lpObj, char* arg);
	static void CommandExchangeWcoinPCallback(LPOBJ lpObj, int value, int kek, DWORD WCoinC, DWORD WCoinP, DWORD GoblinPoint);
	BOOL CheckTraceMarryCondition(LPOBJ lpObj, LPOBJ lpTargetObj);
	static void CommandMLResetCallback(LPOBJ lpObj,DWORD null,DWORD null1,DWORD WCoinC,DWORD WCoinP,DWORD GoblinPoint);

	void SendZenCustomVault(LPOBJ lpObj,char* arg);
	void RecvZenCustomVault(LPOBJ lpObj,char* arg);
	void SendBlessCustomVault(LPOBJ lpObj,char* arg);
	void RecvBlessCustomVault(LPOBJ lpObj,char* arg);
	void SendSoulCustomVault(LPOBJ lpObj,char* arg);
	void RecvSoulCustomVault(LPOBJ lpObj,char* arg);
	void SendChaosCustomVault(LPOBJ lpObj,char* arg);
	void RecvChaosCustomVault(LPOBJ lpObj,char* arg);
	void SendLifeCustomVault(LPOBJ lpObj,char* arg);
	void RecvLifeCustomVault(LPOBJ lpObj,char* arg);
	void SendCreationCustomVault(LPOBJ lpObj,char* arg);
	void RecvCreationCustomVault(LPOBJ lpObj,char* arg);
	void SendGuardianCustomVault(LPOBJ lpObj,char* arg);
	void RecvGuardianCustomVault(LPOBJ lpObj,char* arg);
	void SendHarmonyCustomVault(LPOBJ lpObj,char* arg);
	void RecvHarmonyCustomVault(LPOBJ lpObj,char* arg);
	void SendLowStoneCustomVault(LPOBJ lpObj,char* arg);
	void RecvLowStoneCustomVault(LPOBJ lpObj,char* arg);
	void SendHighStoneCustomVault(LPOBJ lpObj,char* arg);
	void RecvHighStoneCustomVault(LPOBJ lpObj,char* arg);
	void SendGemStoneCustomVault(LPOBJ lpObj,char* arg);
	void RecvGemStoneCustomVault(LPOBJ lpObj,char* arg);

	void PackIudexItem(LPOBJ lpObj,char* arg);
	void UnPackIudexItem(LPOBJ lpObj,char* arg);
private:
	COMMAND_INFO m_CommandInfo[MAX_COMMAND];
};

extern CCommandManager gCommandManager;
