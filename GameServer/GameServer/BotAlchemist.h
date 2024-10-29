#pragma once

#define MAX_BOTALCHEMIST	30

struct BotAlchemistBodyItems
{
	int num;
	int level;
	int opt;
	bool Enabled;
};

struct botAlchemistStruct
{
	int index;
	BYTE Class;
	int MinAccountLevel;
	int Zen;
	int Credits;
	BYTE Rate;
	bool only_same_type;
	bool only_same_index;
	bool only_lower_index;
	bool accept_anc;
	BYTE MaxLevel;
	BYTE MaxExc;
	char Name[11];
	BYTE Map;
	BYTE X;
	BYTE Y;
	BYTE Dir;
	bool Enabled;
	BotAlchemistBodyItems body[9];
	//-
	BOOL AllowLevel;
	BOOL AllowOpt;
	BOOL AllowLuck;
	BOOL AllowSkill;
	BOOL AllowExc;
};

class CBotAlchemist
{
public:
	void Read(char * path);
	void MakeBot();
	int GetBotIndex(int aIndex);
	BOOL IsInTrade(int aIndex);
	BOOL TradeOpen(int aIndex, int bot_index);
	void TradeOk(int aIndex);
	static void TradeCancel(int aIndex);
private:
	bool Enabled;
	bool AllowExc(BYTE BotNum, BYTE ExcOpt);
	BYTE Alchemy(int aIndex,int BotNum);
	botAlchemistStruct bot[MAX_BOTALCHEMIST];
};
extern CBotAlchemist gBotAlchemist;
