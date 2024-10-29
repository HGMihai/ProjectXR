#pragma once

#include "User.h"
#include "Protocol.h"

struct StartItemData
{
	BYTE Class;
	BYTE Section;
	BYTE Type;
	BYTE Level;
	BYTE Dur;
	BYTE Skill;
	BYTE Luck;
	BYTE Option;
	BYTE ExcOption;
	BYTE AncOption;
	BYTE JoHOption;
	BYTE ExOption;
	BYTE SocketOpt[5];
	int  Time;
};

struct StartBuffData
{
	BYTE Class;
	int Effect;
	int value[4];
	int Time;
};

struct PMSG_GD_STARTITEM_STATE_SEND
{
	PSBMSG_HEAD	h;

	char Account[11];
	char Name[11];
	BYTE state;
};

class CCustomStartItem
{
public:
	void Load(char* path);
	void MakeStartItem(LPOBJ lpObj);
	void SendState(LPOBJ lpObj);
private:
	bool StartItemEnable;
	bool StartBuffEnable;
	std::vector<StartItemData> m_ItemData;
	std::vector<StartBuffData> m_BuffData;
}; extern CCustomStartItem gCustomStartItem;