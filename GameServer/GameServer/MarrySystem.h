#pragma once

#include "User.h"

#define MAXMARRYITEM 20

enum eGender
{
	MAN,
	WOMAN
};

struct WCOINS_STRUCT_MAARRY
{
	DWORD WCoinC;
	DWORD WCoinP;
	DWORD GoblinPoint;
};

class CMarrySystem
{
public:
	CMarrySystem();
	virtual ~CMarrySystem();
	void Load(char* section,char* path);
	void ResetConfigs();
	bool NPCTalk(LPOBJ lpNpc, LPOBJ lpObj);
	void StartMarriage(LPOBJ lpNpc, LPOBJ lpObj);
	void EndMarriageFalse(LPOBJ lpObj);
	void EndMarriageTrue();
	void FireworkOnTheScreen(LPOBJ gObj);
	bool YesCommand(LPOBJ lpObj);
	bool NoCommand(LPOBJ lpObj);

	void IsMarry(LPOBJ lpObj, char* arg);
	void IsMarryOnline(LPOBJ lpObj);
	void TraceMarry(LPOBJ lpObj);
	void MarryDivorce(LPOBJ lpObj);
	static void MarryDivorceCallback(LPOBJ lpObj,int Type,LPOBJ lpNpc,DWORD WCoinC,DWORD WCoinP,DWORD GoblinPoint);

	LPOBJ NpcObj;
	LPOBJ gObj1, gObj2;
	bool NpcUse;

	struct SItems
		{
			unsigned long Serial;
			int Group;
			short Index;
			int Type;
			int Lvl;
			int Skill;
			int Luck;
			int Opt;
			int Exc;
			int Num;
		};

		struct sMarry
		{
			int MarrySystemEnabled;
			int MarryHomoSexual[4];
			int MarryCostZen[4];
			int MarryCostPCPoint[4];
			int MarryCostWCoinP[4];
			int MarryCostWCoinC[4];
			int MarryMinLvl[4];
			int MarryNpcNum;
			int MarryTrace[4];
			int MarryTraceLvl[4];
			int IsMarry[4];
			int MarryOnline[4];
			int MarryExpBonus[4];
			int NumWitnesses;

			int IsDivorce[4];
			int DivorceNpcNum;
			int DivorceCostZen[4];
			int DivorceCostPCPoint[4];
			int DivorceCostWCoinC[4];
			int DivorceCostWCoinP[4];
			int DivorceMinLvl[4];

			int TakeItemsForMarry;
			//	int TakeItemsForDivorce;

			SItems MarryItems[MAXMARRYITEM];
			//	SItems DivorceItems[MAXMARRYITEM];

			bool MarrySerial[76];
			//	bool DivorceSerial[76];

		} Config;

		void CancelMarry(LPOBJ lpObj);
		bool CheckGender(LPOBJ lpObj, LPOBJ lpTarget);
		void CheckMarryCost(LPOBJ lpObj, LPOBJ lpNpc, int Type);
		static void CheckMarryCostCallback(LPOBJ lpObj,int Type,LPOBJ lpNpc,DWORD WCoinC,DWORD WCoinP,DWORD GoblinPoint);
		bool CheckCost(LPOBJ lpObj, WCOINS_STRUCT_MAARRY wcstruct, int type);

}; extern CMarrySystem gMarrySystem;
