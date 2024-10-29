#pragma once

#include "User.h"
#include "Protocol.h"

struct PMSG_TELEPORTPANEL_REQ
{
	PSBMSG_HEAD	Head;
	// ----
};

struct PMSG_TELEPORTPANEL_RECV
{
	PSBMSG_HEAD header;
	int Number;
};

struct PMSG_TELEPORT_PANEL_INFO_SEND
{
	PSWMSG_HEAD header;
	BYTE count;
	unsigned int pkclear_price;
	BYTE pkclear_price_type;
};

#pragma pack(push, 1)
struct TELEPORT_PANEL_INFO
{
	unsigned short index;
	unsigned char gate;
	unsigned int min_greset;
	unsigned int price;
	unsigned char price_type;
	char text[32];
};
#pragma pack(pop)

class CTeleportPanel
{
public:
	CTeleportPanel();
	virtual ~CTeleportPanel();
	void load(char* path);
	void send_open_window(int aIndex);
	bool Dialog(LPOBJ lpObj, LPOBJ lpNpc);
	static void TeleportCallback(LPOBJ lpObj,int Class,DWORD null,DWORD WCoinC,DWORD WCoinP,DWORD GoblinPoint);
	//static void ChangeClassCallback(LPOBJ lpObj,int Class,DWORD null,DWORD WCoinC,DWORD WCoinP,DWORD GoblinPoint);
	void RecvTeleportPanel(PMSG_TELEPORTPANEL_RECV* Data, int aIndex);
	bool check_req(LPOBJ lpObj, int index, DWORD WCoinC, DWORD WCoinP, DWORD GoblinPoint);
	void clear_pk(LPOBJ lpObj, DWORD WCoinC, DWORD WCoinP, DWORD GoblinPoint);
	// ----
	int m_NPCId;
	int m_NPCMap;
	int m_X;
	int m_Y;
	unsigned int pkclear_price;
	unsigned char pkclear_price_type;
	std::map<unsigned short,TELEPORT_PANEL_INFO> m_TeleportPanelInfo;
private:
}; extern CTeleportPanel gTeleportPanel;
