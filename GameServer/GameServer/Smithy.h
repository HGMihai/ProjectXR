#pragma once

#include "StdAfx.h"
#include "Protocol.h"
#include "SetItemType.h"

enum smithy_time_values
{
	smithy_1day = 0x02,
	smithy_3day = 0x04,
	smithy_7day = 0x08,
	smithy_30day = 0x10,
	smithy_90day = 0x20,
	smithy_forever = 0x40,
};

struct GC_SmithySettings
{
	PSBMSG_HEAD h;
	int AllowAncExce;
	unsigned char MaxLevel;
	unsigned char MaxExcOptions;
	unsigned short LevelPrice;
	unsigned short LuckPrice;
	unsigned short AddPrice;
	unsigned short Opt1Price;
	unsigned short Opt2Price;
	unsigned short Opt3Price;
	unsigned short Opt4Price;
	unsigned short Opt5Price;
	unsigned short Opt6Price;
	unsigned short AncientPrice;
	unsigned short SocketPrice;
	BYTE time_forever_price;
	BYTE time_1day_price;
	BYTE time_3days_price;
	BYTE time_7days_price;
	BYTE time_30days_price;
	BYTE time_90days_price;
};

struct CG_SmithyCreateItem
{
	PSBMSG_HEAD h;
	unsigned short Item;
	unsigned char ItemLevel;
	unsigned char ItemAdd;
	bool ItemLuck;
	bool ItemOpt1;
	bool ItemOpt2;
	bool ItemOpt3;
	bool ItemOpt4;
	bool ItemOpt5;
	bool ItemOpt6;
	BYTE Ancient;
	bool ItemSocket;
	BYTE ItemTime;
};

struct GC_SmithyInit
{
	PSBMSG_HEAD h;
	bool result;
};

struct GC_SmithyDATA
{
	PSBMSG_HEAD h;
	unsigned char Class;
	unsigned char Type;
	unsigned char Index;
	unsigned int  Price;
};

struct SmithyDATA
{
	unsigned char Class;
	unsigned char Type;
	unsigned char Index;
	unsigned int  Price;
};

struct PMSG_SMITHY_ANCIENT_SEND
{
	PSWMSG_HEAD header;
	BYTE count;
};

struct PMSG_SMITHY_ANCIENT
{
	int index;
	BYTE options[MAX_SET_ITEM_OPTION_INDEX];
};

struct PMSG_SMITHY_SOCKET_SEND
{
	PSWMSG_HEAD header;
	BYTE count;
};

struct PMSG_SMITHY_SOCKET
{
	int index;
};

class CSmithy
{
public:
	CSmithy();
	~CSmithy();
	void Init();
	void Load();
	void Read(char* filename);
	static void GCInitItem(int aIndex);
	void GCItemSend(int aIndex);
	void GCSettingsSend(int aIndex);
	static void GCSendAncientList(int aIndex);
	static void GCSendSocketList(int aIndex);
	void CGCreateItem(CG_SmithyCreateItem* aRecv, int aIndex);
	int AllowAncExce;
	unsigned char MaxLevel;
	unsigned char MaxExcOptions;
	unsigned short LevelPrice;
	unsigned short LuckPrice;
	unsigned short AddPrice;
	unsigned short Opt1Price;
	unsigned short Opt2Price;
	unsigned short Opt3Price;
	unsigned short Opt4Price;
	unsigned short Opt5Price;
	unsigned short Opt6Price;
	unsigned short AncientPrice;
	unsigned short SocketPrice;
	BYTE time_forever_price;
	BYTE time_1day_price;
	BYTE time_3days_price;
	BYTE time_7days_price;
	BYTE time_30days_price;
	BYTE time_90days_price;
	std::vector<SmithyDATA> itemdata;
};
extern CSmithy gSmithy;