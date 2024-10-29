#pragma once

#include "User.h"

struct TELEPORTS_COMMAND_DATA
{
	char Command[32];
	int TargetGate;
	int ReqWCC;
	int ReqWCP;
	int ReqWGP;
	int ReqZen;
	int ReqItem;
	int ReqItemLevel;
	int ReqItemCount;
};

struct TELEPORTS_NPC_DATA
{
	int NPC;
	int Map;
	int X;
	int Y;
	char Name[32];
	int TargetGate;
	int ReqWCC;
	int ReqWCP;
	int ReqWGP;
	int ReqZen;
	int ReqItem;
	int ReqItemLevel;
	int ReqItemCount;
};

struct TELEPORT_DATA
{
	char Command[32];
	int TargetGate;
	int ReqWCC;
	int ReqWCP;
	int ReqWGP;
	int ReqZen;
	int ReqItem;
	int ReqItemLevel;
	int ReqItemCount;
};

struct WCOINS_STRUCT
{
	DWORD WCoinC;
	DWORD WCoinP;
	DWORD GoblinPoint;
};

class NPCTeleport
{
public:
	NPCTeleport();
	virtual ~NPCTeleport();
	void Load(char* path);
	void CommandCustomTeleport(LPOBJ gObj, char* args);
	void Teleport(LPOBJ gObj, TELEPORT_DATA data, WCOINS_STRUCT wCoins);
	static void TeleportCallback(LPOBJ lpObj,char command[32],DWORD slot,DWORD WCoinC,DWORD WCoinP,DWORD GoblinPoint);

	bool Dialog(LPOBJ lpObj, LPOBJ lpNpc);
	static void TeleportNpcCallback(LPOBJ lpObj,LPOBJ lpNpc,DWORD null,DWORD WCoinC,DWORD WCoinP,DWORD GoblinPoint);

	bool CheckItem(LPOBJ lpObj, TELEPORT_DATA data);
private:
	//TELEPORTS m_Teleports[MAX_TELEPORTS];
	std::vector<TELEPORTS_COMMAND_DATA> m_Teleports_Command;
	std::vector<TELEPORTS_NPC_DATA> m_Teleports_NPC;
}; extern NPCTeleport gTeleporter;