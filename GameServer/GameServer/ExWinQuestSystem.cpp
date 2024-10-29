#include "StdAfx.h"
#include "ExWinQuestSystem.h"
#include "user.h"
#include "GameMain.h"
#include "DSProtocol.h"
#include "Path.h"
#include "Log.h"
#include "MemScript.h"
#include "Util.h"
#include "ItemManager.h"
#include "Item.h"
#include "Map.h"
#include "Monster.h"
#include "Notice.h"
#include "CashShop.h"
#include "ObjectManager.h"
#include "BankEx.h"

BOOL gObjIsConnectedEx(int aIndex)
{
	if (aIndex < 0 || aIndex > MAX_OBJECT - 1) 
	{
		return FALSE;
	}
	
	OBJECTSTRUCT * lpObj = static_cast<OBJECTSTRUCT *>(&gObj[aIndex]);
	
	if (lpObj->Type != OBJECT_USER)	
	{
		return FALSE;
	}
	
	if (lpObj->Connected < OBJECT_ONLINE)
	{
		return FALSE;
	}
	
	if (lpObj->CloseCount >= 0)
	{
		return FALSE;
	}
	
	return TRUE;
}

bool CheckOptionItem(int Item1, int Item2, int iType)
{
		if (iType)
		{
			if (Item1 == Item2)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		else
		{
			if (Item1 && Item2)
			{
				return true;
			}
			else if (!Item1 && !Item2)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
}
// ----------------------------------------------------------------------------------------------

ExWinQuestSystem g_ExWinQuestSystem;
// ----------------------------------------------------------------------------------------------

ExWinQuestSystem::ExWinQuestSystem()
{
	this->Init();
}
// ----------------------------------------------------------------------------------------------

ExWinQuestSystem::~ExWinQuestSystem()
{
	// ----
}
// ----------------------------------------------------------------------------------------------

void ExWinQuestSystem::Init()
{
	ZeroMemory(&this->ewdata, sizeof(this->ewdata));

	this->Enable = false;
	this->NPC_Class = 0;
	this->NPC_Map = 0;
	this->NPC_X = 0;
	this->NPC_Y = 0;
	this->iDropLoaded = 0;
}
// ----------------------------------------------------------------------------------------------

void ExWinQuestSystem::Load()
{
	this->Init();

	this->Read(gPath.GetFullPath("..\\LTPData\\WinQuest\\QuestSystem.ini"));
	this->ReadQuest(0, gPath.GetFullPath("..\\LTPData\\WinQuest\\QuestData_1.ini"));
	this->ReadQuest(1, gPath.GetFullPath("..\\LTPData\\WinQuest\\QuestData_2.ini"));
	this->ReadQuest(2, gPath.GetFullPath("..\\LTPData\\WinQuest\\QuestData_3.ini"));
	this->ReadDrop(gPath.GetFullPath("..\\LTPData\\WinQuest\\QuestDrop.ini"));
}
// ----------------------------------------------------------------------------------------------

void ExWinQuestSystem::Read(char* File)
{
	this->Enable = GetPrivateProfileInt("Common", "Enable", 0, File);
	this->NPC_Class = GetPrivateProfileInt("Common", "NPC_Class", 0, File);
	this->NPC_Map = GetPrivateProfileInt("Common", "NPC_Map", 0, File);
	this->NPC_X = GetPrivateProfileInt("Common", "NPC_X", 0, File);
	this->NPC_Y = GetPrivateProfileInt("Common", "NPC_Y", 0, File);
}
// ----------------------------------------------------------------------------------------------

void ExWinQuestSystem::ReadQuest(int a, char* File)
{
	CMemScript* lpMemScript = new CMemScript;

	if (lpMemScript == nullptr)
	{
		printf(MEM_SCRIPT_ALLOC_ERROR, File);
		return;
	}

	if (lpMemScript->SetBuffer(File) == 0)
	{
		printf(lpMemScript->GetLastError());
		delete lpMemScript;
		return;
	}

	try
	{
		int n = 0;

		while (true)
		{
			if (lpMemScript->GetToken() == TOKEN_END)
			{
				break;
			}

			if (strcmp("end", lpMemScript->GetString()) == 0)
			{
				break;
			}

			this->ewdata[a][n].NeedLevel = lpMemScript->GetNumber();
			this->ewdata[a][n].NeedReset = lpMemScript->GetAsNumber();
			this->ewdata[a][n].NeedGrand = lpMemScript->GetAsNumber();
			strncpy(this->ewdata[a][n].NameQuest, lpMemScript->GetAsString(), 25);

			//g_Console.AddMessage(5, this->ewdata[a][n].NameQuest);

			for (int m = 0; m < EW_MAX_MISSION; m++)
			{
				this->ewdata[a][n].MissionType[m] = lpMemScript->GetAsNumber();
				this->ewdata[a][n].MissionID[m] = lpMemScript->GetAsNumber();
				this->ewdata[a][n].MissionIndex[m] = lpMemScript->GetAsNumber();
				this->ewdata[a][n].MissionLevel[m] = lpMemScript->GetAsNumber();
				this->ewdata[a][n].MissionLuck[m] = lpMemScript->GetAsNumber();
				this->ewdata[a][n].MissionOpt[m] = lpMemScript->GetAsNumber();
				this->ewdata[a][n].MissionExl[m] = lpMemScript->GetAsNumber();
				this->ewdata[a][n].MissionPercent[m] = lpMemScript->GetAsNumber();
				this->ewdata[a][n].MissionCount[m] = lpMemScript->GetAsNumber();
			}

			for (int r = 0; r < EW_MAX_REWARD; r++)
			{
				this->ewdata[a][n].RewardType[r] = lpMemScript->GetAsNumber();
				this->ewdata[a][n].RewardItemType[r] = lpMemScript->GetAsNumber();
				this->ewdata[a][n].RewardItemIndex[r] = lpMemScript->GetAsNumber();
				this->ewdata[a][n].RewardItemLevel[r] = lpMemScript->GetAsNumber();
				this->ewdata[a][n].RewardCount[r] = lpMemScript->GetAsNumber();
			}

			n++;
		}

		this->iLoaded[a] = n;
	}
	catch (...)
	{
		printf(lpMemScript->GetLastError());
	}

	gLog.AddLog(1, "[ExWinQuestSystem][ReadQuestEx] Item loaded from list");

	delete lpMemScript;
}
// ----------------------------------------------------------------------------------------------

void ExWinQuestSystem::ReadDrop(char* File)
{
		CMemScript* lpMemScript = new CMemScript;

		if (lpMemScript == nullptr)
		{
			printf(MEM_SCRIPT_ALLOC_ERROR, File);
			return;
		}

		if (lpMemScript->SetBuffer(File) == 0)
		{
			printf(lpMemScript->GetLastError());
			delete lpMemScript;
			return;
		}

		try
		{
			while (true)
			{
				if (lpMemScript->GetToken() == TOKEN_END)
				{
					break;
				}

				if (strcmp("end", lpMemScript->GetString()) == 0)
				{
					break;
				}

				this->m_Drop[this->iDropLoaded].iAct = lpMemScript->GetNumber();
				this->m_Drop[this->iDropLoaded].iNeedQuest = lpMemScript->GetAsNumber();
				this->m_Drop[this->iDropLoaded].ItemType = lpMemScript->GetAsNumber();
				this->m_Drop[this->iDropLoaded].ItemIndex = lpMemScript->GetAsNumber();
				this->m_Drop[this->iDropLoaded].ItemMinLevel = lpMemScript->GetAsNumber();
				this->m_Drop[this->iDropLoaded].ItemMaxLevel = lpMemScript->GetAsNumber();
				this->m_Drop[this->iDropLoaded].ItemMaxOption = lpMemScript->GetAsNumber();
				this->m_Drop[this->iDropLoaded].ItemLuck = lpMemScript->GetAsNumber();
				this->m_Drop[this->iDropLoaded].ItemSkill = lpMemScript->GetAsNumber();
				this->m_Drop[this->iDropLoaded].ItemExcellent = lpMemScript->GetAsNumber();
				this->m_Drop[this->iDropLoaded].ItemDur = lpMemScript->GetAsNumber();
				this->m_Drop[this->iDropLoaded].ItemDropRate = lpMemScript->GetAsNumber();
				this->m_Drop[this->iDropLoaded].MonsterMinLevel = lpMemScript->GetAsNumber();
				this->m_Drop[this->iDropLoaded].MonsterMaxLevel = lpMemScript->GetAsNumber();
				this->m_Drop[this->iDropLoaded].MonsterMap = lpMemScript->GetAsNumber();

				//g_Console.AddMessage(5, "%d", this->m_Drop[this->iDropLoaded].iAct);
			}
		}
		catch (...)
		{
			printf(lpMemScript->GetLastError());
		}

		gLog.AddLog(1, "[ExWinQuestSystem][ReadDrop]Item loaded from list");

		delete lpMemScript;
}
// ----------------------------------------------------------------------------------------------

bool ExWinQuestSystem::Dialog(int aIndex, int aNpcIndex)
{
	//LogAdd(LOG_RED,"Dialog quest");
	if (!this->Enable)
	{
		//LogAdd(LOG_RED,"!Enable");
		return false;
	}

	if (!gObjIsConnectedEx(aIndex))
	{
		//LogAdd(LOG_RED,"!gObjIsConnectedEx");
		return false;
	}

		//LPOBJ lpUser = &gObj[aIndex];
	const LPOBJ lpNpc = &gObj[aNpcIndex];

	if (lpNpc->Class == this->NPC_Class
		&&	lpNpc->Map			== this->NPC_Map
		&&	lpNpc->X			== this->NPC_X
		&&	lpNpc->Y			== this->NPC_Y )
	{

	//	LogAdd(LOG_RED,"WindowOpen");
		this->GC_OpenWindow(aIndex);
		return true;
	}
		// ----
	//LogAdd(LOG_RED,"!return false");
	return false;
}
//// ----------------------------------------------------------------------------------------------
void ExWinQuestSystem::GC_MainInfo(int aIndex, bool isOpenWindow)
{
		LPOBJ lpUser = &gObj[aIndex];

		GC_MainPacket pReq;
		pReq.h.set(0xFB, 0x96, sizeof(pReq));

		pReq.isOpenWindow = isOpenWindow;

		for (int a = 0; a < EW_MAX_ACT; a++)
		{
			bool sb_accept = true;
			bool sb_done = true;

			bool sb_level = true;
			bool sb_reset = true;
			bool sb_grand = true;

			bool b_mission[EW_MAX_MISSION];

			for (int i = 0; i < EW_MAX_MISSION; i++)
			{
				b_mission[i] = true;
			}

			const int n = lpUser->ExWQuestNum[a];

			if (n >= this->iLoaded[a])
			{
				pReq.exQuest[a].b_End = true;
			}
			else
			{
				pReq.exQuest[a].b_End = false;
			}
			// -> user send
			strncpy(pReq.exQuest[a].NameQuest, this->ewdata[a][n].NameQuest, 25);
			pReq.exQuest[a].Start = lpUser->ExWQuestStart[a];
			pReq.exQuest[a].Number = lpUser->ExWQuestNum[a];

			for (int m = 0; m < EW_MAX_MISSION; m++)
			{
				if (this->ewdata[a][n].MissionType[m] == Searching)
				{
					pReq.exQuest[a].Count[m] = this->SearchItem(aIndex, a, m);
				}
				else
				{
					if (lpUser->ExWQuestCount[a][m] > this->ewdata[a][n].MissionCount[m])
					{
						lpUser->ExWQuestCount[a][m] = this->ewdata[a][n].MissionCount[m];
					}

					pReq.exQuest[a].Count[m] = lpUser->ExWQuestCount[a][m];
				}
			}
			// -> need send
			if (this->ewdata[a][n].NeedLevel > lpUser->Level)
			{
				sb_level = false;
			}

			pReq.exQuest[a].NeedLevel = this->ewdata[a][n].NeedLevel;

			if (this->ewdata[a][n].NeedReset > lpUser->Reset)
			{
				sb_reset = false;
			}

			pReq.exQuest[a].NeedReset = this->ewdata[a][n].NeedReset;

			if (this->ewdata[a][n].NeedGrand > lpUser->MasterReset)
			{
				sb_grand = false;
			}
			pReq.exQuest[a].NeedGrand = this->ewdata[a][n].NeedGrand;
			// -> mission send
			for (int m = 0; m < EW_MAX_MISSION; m++)
			{
				if (this->ewdata[a][n].MissionType[m] == Hunting || this->ewdata[a][n].MissionType[m] == Droping)
				{
					if (this->ewdata[a][n].MissionCount[m] > lpUser->ExWQuestCount[a][m])
					{
						b_mission[m] = false;
						sb_done = false;
					}
				}
				else if (this->ewdata[a][n].MissionType[m] == Searching)
				{
					if (!this->SearchItem(aIndex, a, m))
					{
						b_mission[m] = false;
						sb_done = false;
					}
				}
				pReq.exQuest[a].b_Mission[m] = b_mission[m];

				pReq.exQuest[a].MissionType[m] = this->ewdata[a][n].MissionType[m];
				pReq.exQuest[a].MissionID[m] = this->ewdata[a][n].MissionID[m];
				pReq.exQuest[a].MissionIndex[m] = this->ewdata[a][n].MissionIndex[m];
				pReq.exQuest[a].MissionLevel[m] = this->ewdata[a][n].MissionLevel[m];
				pReq.exQuest[a].MissionLuck[m] = this->ewdata[a][n].MissionLuck[m];
				pReq.exQuest[a].MissionOpt[m] = this->ewdata[a][n].MissionOpt[m];
				pReq.exQuest[a].MissionExl[m] = this->ewdata[a][n].MissionExl[m];
				pReq.exQuest[a].MissionCount[m] = this->ewdata[a][n].MissionCount[m];
				pReq.exQuest[a].MissionPercent[m] = this->ewdata[a][n].MissionPercent[m];
			}
			// -> reward send
			for (int r = 0; r < EW_MAX_REWARD; r++)
			{
				pReq.exQuest[a].RewardType[r] = this->ewdata[a][n].RewardType[r];
				pReq.exQuest[a].RewardCount[r] = this->ewdata[a][n].RewardCount[r];
				pReq.exQuest[a].RewardItemType[r] = this->ewdata[a][n].RewardItemType[r];
				pReq.exQuest[a].RewardItemIndex[r] = this->ewdata[a][n].RewardItemIndex[r];
				pReq.exQuest[a].RewardItemLevel[r] = this->ewdata[a][n].RewardItemLevel[r];
			}

			if (lpUser->ExWQuestStart[a] == FALSE)
			{
				if (sb_level == false || sb_reset == false || sb_grand == false)
				{
					sb_accept = false;
				}
			}

			pReq.exQuest[a].b_done = sb_done;
			pReq.exQuest[a].b_accept = sb_accept;

			pReq.exQuest[a].b_Level = sb_level;
			pReq.exQuest[a].b_Reset = sb_reset;
			pReq.exQuest[a].b_Grand = sb_grand;
		}

		DataSend(aIndex, reinterpret_cast<LPBYTE>(&pReq), sizeof(pReq));
}
//// ----------------------------------------------------------------------------------------------
//
void ExWinQuestSystem::GC_OpenWindow(int aIndex)
{
		if (!gObjIsConnectedEx(aIndex))
		{
			return;
		}

		this->GC_MainInfo(aIndex,true);
}
//// ----------------------------------------------------------------------------------------------
//
void ExWinQuestSystem::GC_MiniInfo(int aIndex)
{
		LPOBJ lpUser = &gObj[aIndex];

		GC_ExWinMiniInfo pReq = { 0 };

		pReq.h.set(0xFB, 0x97, sizeof(pReq));

		for (int a = 0; a < EW_MAX_ACT; a++)
		{
			const int n = lpUser->ExWQuestNum[a];
			strncpy(pReq.exQuest[a].NameQuest, this->ewdata[a][n].NameQuest, 25);
			pReq.exQuest[a].Start = lpUser->ExWQuestStart[a];

			for (int m = 0; m < EW_MAX_MISSION; m++)
			{
				if (this->ewdata[a][n].MissionType[m] == Hunting || this->ewdata[a][n].MissionType[m] == Droping)
				{
					pReq.exQuest[a].ExWQuestCount[m] = lpUser->ExWQuestCount[a][m];
				}
				else if (this->ewdata[a][n].MissionType[m] == Searching)
				{
					pReq.exQuest[a].ExWQuestCount[m] = this->SearchItem(aIndex, a, m);
				}

				pReq.exQuest[a].MissionType[m] = this->ewdata[a][n].MissionType[m];
				pReq.exQuest[a].MissionID[m] = this->ewdata[a][n].MissionID[m];
				pReq.exQuest[a].MissionIndex[m] = this->ewdata[a][n].MissionIndex[m];
				pReq.exQuest[a].MissionLevel[m] = this->ewdata[a][n].MissionLevel[m];
				pReq.exQuest[a].MissionLuck[m] = this->ewdata[a][n].MissionLuck[m];
				pReq.exQuest[a].MissionOpt[m] = this->ewdata[a][n].MissionOpt[m];
				pReq.exQuest[a].MissionExl[m] = this->ewdata[a][n].MissionExl[m];
				pReq.exQuest[a].MissionCount[m] = this->ewdata[a][n].MissionCount[m];
			}
		}

		DataSend(aIndex, reinterpret_cast<LPBYTE>(&pReq), sizeof(pReq));
}
//// ----------------------------------------------------------------------------------------------
//
void ExWinQuestSystem::GC_MiniInfoEx(int aIndex, int a, int m)
{
		if (!gObjIsConnectedEx(aIndex))
		{
			return;
		}

		GC_ExWinMiniInfoEx exSend = { 0 };
		exSend.h.set(0xFB, 0x95, sizeof(exSend));

		exSend.a = a;
		exSend.m = m;

		DataSend(aIndex, reinterpret_cast<LPBYTE>(&exSend), exSend.h.size);
}
//// ----------------------------------------------------------------------------------------------
//
void ExWinQuestSystem::CG_AcceptQuest(int aIndex, CG_Accept_Done* aRecv)
{
		if (!gObjIsConnectedEx(aIndex))
		{
			return;
		}

		if (aRecv->Result != 0 && aRecv->Result != 1 && aRecv->Result != 2)
		{
			return;
		}

		LPOBJ lpUser = &gObj[aIndex];

		const int a = aRecv->Result;
		const int n = lpUser->ExWQuestNum[a];

		LogAdd(LOG_RED,"val = %d",lpUser->ExWQuestStart[a]);
		if (!lpUser->ExWQuestStart[a])
		{
			if (this->ewdata[a][n].NeedLevel > lpUser->Level ||
				this->ewdata[a][n].NeedReset > lpUser->Reset ||
				this->ewdata[a][n].NeedGrand > lpUser->MasterReset)
			{
				gLog.AddLog(2, "[ExWinQuestSystem] Error CG_AcceptQuest Result");
				return;
			}

			lpUser->ExWQuestStart[a] = true;

			this->GC_Start(aIndex, a, true);
		}
		else
		{
			this->RewardQuest(aIndex, a);
		}
}
//// ----------------------------------------------------------------------------------------------
//
void ExWinQuestSystem::GC_Start(int aIndex, int a, int s)
{
		GC_ExWinStart pReq = { 0 };
		pReq.h.set(0xFB, 0x94, sizeof(pReq));
		pReq.Act = a;
		pReq.Start = s;
		DataSend(aIndex, reinterpret_cast<LPBYTE>(&pReq), pReq.h.size);

		/*if(g_BuffManagerEx.CheckExBuffData(aIndex, AT_QUEST_BUFF) == FALSE)
		{
		g_BuffManagerEx.AddExBuffData(aIndex, AT_QUEST_BUFF, -10);
		}*/

		//g_BuffManagerEx.GC_QuestBuffInfo(aIndex);
}
//// ----------------------------------------------------------------------------------------------
//
bool ExWinQuestSystem::MonsterKiller(int aIndexMonster, int aIndexUser)
{
		bool Result = false;

		if (!this->Enable)
		{
			return Result;
		}

		LPOBJ lpUser = &gObj[aIndexUser];
		const LPOBJ lpMonster = &gObj[aIndexMonster];

		for (int a = 0; a < EW_MAX_ACT; a++)
		{
			const int n = lpUser->ExWQuestNum[a];

			if (lpUser->ExWQuestStart[a] == TRUE)
			{
				for (int m = 0; m < EW_MAX_MISSION; m++)
				{
					// -> Monsterr Mission
					if (this->ewdata[a][n].MissionType[m] == Hunting)
					{
						if (this->ewdata[a][n].MissionCount[m] > lpUser->ExWQuestCount[a][m])
						{
							if (this->ewdata[a][n].MissionLuck[m] == lpMonster->Map || this->ewdata[a][n].MissionLuck[m] == -1)
							{
								if (this->ewdata[a][n].MissionID[m] == lpMonster->Class)	// ����� �� ��������� �� ������
								{
									lpUser->ExWQuestCount[a][m]++;

									this->GC_MiniInfoEx(aIndexUser, a, m);
								}
								else if (this->ewdata[a][n].MissionID[m] == -1)	// ����� �� ��������� �� ������
								{
									if (lpMonster->Level >= this->ewdata[a][n].MissionOpt[m] && lpMonster->Level <= this->ewdata[a][n].MissionExl[m])
									{
										lpUser->ExWQuestCount[a][m]++;
										this->GC_MiniInfoEx(aIndexUser, a, m);
									}
								}
							}
						}
					}
					// -> Drop Mission
					if (this->ewdata[a][n].MissionType[m] == Droping)
					{
						if (this->ewdata[a][n].MissionCount[m] > lpUser->ExWQuestCount[a][m])
						{
							if (this->ewdata[a][n].MissionLuck[m] == lpMonster->Map || this->ewdata[a][n].MissionLuck[m] == -1)
							{
								if (lpMonster->Level >= this->ewdata[a][n].MissionOpt[m] && lpMonster->Level <= this->ewdata[a][n].MissionExl[m])
								{
									if ((rand() % 100) < this->ewdata[a][n].MissionPercent[m])
									{
										const int ItemType = GET_ITEM(this->ewdata[a][n].MissionID[m], this->ewdata[a][n].MissionIndex[m]);
										//ItemSerialCreateSend(aIndexUser,lpMonster->Map,lpMonster->X,lpMonster->Y,ItemType,0,0,0,0,0,aIndexUser,0,0);
										GDCreateItemSend(aIndexUser, lpMonster->Map, static_cast<BYTE>(lpMonster->X), static_cast<BYTE>(lpMonster->Y), ItemType, 0, 0, 0, 0, 0, aIndexUser, 0, 0, 0, 0, nullptr, 0xFF, 0);
										Result = true;
									}
								}
							}
						}
					}
				}
			}
		}

#if(EW_NEW_DROP==TRUE)
		if (this->NewDrop(aIndexUser, aIndexMonster))
		{
			Result = true;
		}
#endif

		return Result;
}
//// ----------------------------------------------------------------------------------------------
//
int ExWinQuestSystem::PickUpItem(int aIndex, int ItemType)
{
		if (!this->Enable)
		{
			return 0;
		}

		LPOBJ lpUser = &gObj[aIndex];

		for (int a = 0; a < EW_MAX_ACT; a++)
		{
			const int n = lpUser->ExWQuestNum[a];

			if (lpUser->ExWQuestStart[a] == TRUE)
			{
				for (int m = 0; m < EW_MAX_MISSION; m++)
				{
					if (this->ewdata[a][n].MissionType[m] == Droping &&
						GET_ITEM(this->ewdata[a][n].MissionID[m], this->ewdata[a][n].MissionIndex[m]) == ItemType)
					{
						if (this->ewdata[a][n].MissionCount[m] > lpUser->ExWQuestCount[a][m])
						{
							lpUser->ExWQuestCount[a][m]++;
							this->GC_MiniInfoEx(aIndex, a, m);
							return 1;
						}
						else if (lpUser->ExWQuestCount[a][m] >= this->ewdata[a][n].MissionCount[m])
						{
							return 2;
						}
					}
				}
			}
		}

		return 0;
}
//// ----------------------------------------------------------------------------------------------
//
bool ExWinQuestSystem::SearchItem(int aIndex, int a, int m)
{
		LPOBJ lpUser = &gObj[aIndex];

		const int n = lpUser->ExWQuestNum[a];

		const short iType = GET_ITEM(this->ewdata[a][n].MissionID[m], this->ewdata[a][n].MissionIndex[m]);
		const short iLevel = this->ewdata[a][n].MissionLevel[m];
		const short iLuck = this->ewdata[a][n].MissionLuck[m];
		const short iOpt = this->ewdata[a][n].MissionOpt[m];
		const short iExl = this->ewdata[a][n].MissionExl[m];

		for (int i = INVENTORY_WEAR_SIZE; i < INVENTORY_SIZE; i++)
		{
			if (lpUser->Inventory[i].m_Index == iType &&
				lpUser->Inventory[i].m_Level >= iLevel &&
				CheckOptionItem(lpUser->Inventory[i].m_LuckOption, iLuck, FALSE) &&
				CheckOptionItem(lpUser->Inventory[i].m_Option, iOpt, FALSE) &&
				CheckOptionItem(lpUser->Inventory[i].m_NewOption, iExl, FALSE))
			{
				return true;
			}
		}
		return false;
}
//// ----------------------------------------------------------------------------------------------
//
void ExWinQuestSystem::DeleteItem(int aIndex, int a, int m)
{
		LPOBJ lpUser = &gObj[aIndex];

		const int n = lpUser->ExWQuestNum[a];

		const short iType = GET_ITEM(this->ewdata[a][n].MissionID[m], this->ewdata[a][n].MissionIndex[m]);
		const short iLevel = this->ewdata[a][n].MissionLevel[m];
		const short iLuck = this->ewdata[a][n].MissionLuck[m];
		const short iOpt = this->ewdata[a][n].MissionOpt[m];
		const short iExl = this->ewdata[a][n].MissionExl[m];

		for (int i = INVENTORY_WEAR_SIZE; i < INVENTORY_SIZE; i++)
		{
			if (lpUser->Inventory[i].m_Index == iType &&
				lpUser->Inventory[i].m_Level >= iLevel &&
				CheckOptionItem(lpUser->Inventory[i].m_LuckOption, iLuck, FALSE) &&
				CheckOptionItem(lpUser->Inventory[i].m_Option, iOpt, FALSE) &&
				CheckOptionItem(lpUser->Inventory[i].m_NewOption, iExl, FALSE))
			{
				gItemManager.InventoryDelItem(aIndex, i);
				gItemManager.GCItemDeleteSend(aIndex, i, 1);
			}
		}
}
//// ----------------------------------------------------------------------------------------------
//
bool ExWinQuestSystem::CheckMission(int aIndex, int a)
{
		LPOBJ lpUser = &gObj[aIndex];

		const int n = lpUser->ExWQuestNum[a];

		for (int m = 0; m < EW_MAX_MISSION; m++)
		{
			if (this->ewdata[a][n].MissionType[m] == Hunting || this->ewdata[a][n].MissionType[m] == Droping)
			{
				if (this->ewdata[a][n].MissionCount[m] > lpUser->ExWQuestCount[a][m])
				{
					return false;
				}
			}
			else if (this->ewdata[a][n].MissionType[m] == Searching)
			{
				if (!this->SearchItem(aIndex, a, m))
				{
					return false;
				}
			}
		}

		return true;
}
//// ----------------------------------------------------------------------------------------------
//
void ExWinQuestSystem::UserConnect(int aIndex)
{
		LPOBJ lpUser = &gObj[aIndex];

		this->GC_MiniInfo(aIndex);

		for (int a = 0; a < EW_MAX_ACT; a++)
		{
			if (lpUser->ExWQuestNum[a] >= 0 && lpUser->ExWQuestStart[a] == TRUE)
			{
				//#if(CUSTOM_QUEST_BUFF==TRUE)
				//			//gObjAddBuffEffect(lpUser,AT_QUEST_BUFF,0,0,0,0,-10);
				//			g_BuffManagerEx.AddExBuffData(aIndex, AT_QUEST_BUFF, -10);
				//			g_BuffManagerEx.GC_QuestBuffInfo(aIndex);
				//#endif
				return;
			}
		}
}
//// ----------------------------------------------------------------------------------------------
//
void ExWinQuestSystem::RewardQuest(int aIndex, int Act)
{
	//LogAdd(LOG_RED,"Start reward");
		if (!gObjIsConnectedEx(aIndex))
		{
			//LogAdd(LOG_RED,"reward not connected");
			return;
		}

		LPOBJ lpUser = &gObj[aIndex];

		int a = Act;
		int n = lpUser->ExWQuestNum[a];

		if (!this->CheckMission(aIndex, a))
		{
			gLog.AddLog(2, "[ExWinQuestSystem] RewardQuest ERROR");
			LogAdd(LOG_RED,"reward error");
			return;
		}

		int X = 0;
		int Y = 0;

		for (int i = 0; i < EW_MAX_REWARD; i++)
		{
			if (this->ewdata[a][n].RewardType[i] == ewItem)
			{
				int iItemIndex = this->ewdata[a][n].RewardItemIndex[i];
				int iItemType = this->ewdata[a][n].RewardItemType[i];

				ITEM_INFO ItemInfo;

				gItemManager.GetInfo(GET_ITEM(iItemType, iItemIndex), &ItemInfo);

				X += ItemInfo.Width;

				if (Y < ItemInfo.Height)
				{
					Y = ItemInfo.Height;
				}
			}
		}

		if (!gItemManager.CheckItemInventorySpace(&gObj[aIndex], X, Y))
		{
			gNotice.GCNoticeSend(aIndex, 1, 0, 0, 0, 0, 0, "Need %dx%d cells in inventory", X, Y);
			return;
		}

		int UserMap = lpUser->Map;

		int rType = 0;
		int rItemType = 0;
		int rItemIndex = 0;
		int rItemLevel = 0;
		int rCount = 0;

		for (int m = 0; m < EW_MAX_MISSION; m++)
		{
			lpUser->ExWQuestCount[a][m] = 0;

			if (this->ewdata[a][n].MissionType[m] == Searching)
			{
				this->DeleteItem(aIndex, a, m);
			}
		}

		//g_Console.AddMessage(3, "%d %d", X, Y);

		//GET_ITEM(rItemType, rItemIndex)

		for (int r = 0; r < EW_MAX_REWARD; r++)
		{
			rType = this->ewdata[a][n].RewardType[r];
			rItemType = this->ewdata[a][n].RewardItemType[r];
			rItemIndex = this->ewdata[a][n].RewardItemIndex[r];
			rItemLevel = this->ewdata[a][n].RewardItemLevel[r];
			rCount = this->ewdata[a][n].RewardCount[r];

			if (rType == ewZen)
			{
				if (!MAP_RANGE(UserMap))
				{
					return;
				}

				//gMap[UserMap].MoneyItemDrop(rCount, UserX, UserY);
	
				if (lpUser->Money + rCount > MAX_MONEY)
				{
					lpUser->Money = MAX_MONEY;
				}
				else
				{
					lpUser->Money += rCount;
				}

				GCMoneySend(lpUser->Index, lpUser->Money);
			}
			if (rType == ewFreePoint)
			{
				lpUser->iQuestStat += rCount;
				lpUser->LevelUpPoint += rCount;

				gObjectManager.CharacterCalcAttribute(lpUser->Index);

				GCNewCharacterInfoSend(lpUser);

				GDCharacterInfoSaveSend(lpUser->Index);
				//#if(CUSTOM_EX_FREE_POINT==TRUE)
				//lpUser->ExFreePoints += rCount;
				//#endif
				//lpUser->LevelUpPoint += rCount;

				//g_ExUser.UpdateCharInfo(aIndex);
			}

			if (rType == ewItem)
			{
				int level, skill, luck, option, exc, set, socket;

				BYTE ItemSocketOption[MAX_SOCKET_OPTION] = { 0xFF,0xFF,0xFF,0xFF,0xFF };

				ITEM_INFO TargetItem;
				gItemManager.GetInfo(GET_ITEM(rItemType, rItemIndex), &TargetItem);

				level = rItemLevel;
				skill = TargetItem.Skill;
				luck = 0;
				option = 0;
				exc = 0;
				set = 0;
				socket = 0;

				for (int d = 0; d < socket && socket <= MAX_SOCKET_OPTION; d++)
				{
					ItemSocketOption[d] = 0xFF;
				}

				for (int d = 0; d < rCount; d++)
				{
					GDCreateItemSend(aIndex, 0xEB, 0, 0, GET_ITEM(rItemType, rItemIndex), level, 0, skill, luck, option, -1, exc, set, 0, 0, ItemSocketOption, 0xFF, 0);
				}
				
				/*for (int d = 0; d < rCount; d++)
				{*/
				//ITEM_INFO TargetItem;
				//gItemManager.GetInfo(GET_ITEM(rItemType, rItemIndex), &TargetItem);

				////----

				//CItem Item;

				//Item.m_Index = GET_ITEM(rItemType, rItemIndex);
				//Item.m_Level = rItemLevel;
				//Item.m_Option1 = rand() % 2;
				//Item.m_Option2 = TargetItem.Skill;
				//Item.m_Option3 = rand() % 8;

				//BYTE ItemOption3 = 0;
				//BYTE ItemNewOption = 0;
				//BYTE ItemSetOption = 0;
				//BYTE ItemSocketOption[MAX_SOCKET_OPTION] = { 0xFF,0xFF,0xFF,0xFF,0xFF };

				//gItemOptionRate.GetItemOption3(Item.m_Index, &ItemOption3);

				//gItemOptionRate.MakeNewOption(Item.m_Index, 2, &ItemNewOption);

				//gItemOptionRate.MakeSetOption(Item.m_Index, ItemSetOption, &ItemSetOption);

				//gItemOptionRate.MakeSocketOption(Item.m_Index, rand()%5+1, &ItemSocketOption[0]);

				//Item.m_NewOption = ItemNewOption;
				//Item.m_SetOption = ItemSetOption;

				//memcpy(Item.m_SocketOption, ItemSocketOption, sizeof(ItemSocketOption));

				//Item.m_Durability = gItemManager.GetItemDurability(Item.m_Index, Item.m_Level, Item.m_NewOption, Item.m_SetOption);

				//gItemManager.InventoryInsertItem(aIndex, Item);

				//GDInventoryItemSaveSend(aIndex);

			//	GDCreateItemSend(aIndex, UserMap, UserX, UserY, GET_ITEM(rItemType, rItemIndex), rItemLevel, 0, 0, 0, 0, lpUser->Index, 0, 0, 0, 0, 0, 0xFF, 0);
			/*}*/
			}
			if (rType == ewCredits)
			{
				lpUser->BankEx.Credits += rCount;
				gBankEx.GCUpdateBankEx(lpUser->Index);
				gBankEx.GDSavePoint(lpUser->Index);
				//gCashShop.GDCashShopAddPointSaveSend(aIndex, nullptr, rCount, 0, 0);
				//lpUser->Bonus += rCount;
				//g_ExUser.UpdateCharInfo(aIndex);
				//������
			}
			if (rType == ewWCoinC)
			{
				gCashShop.GDCashShopAddPointSaveSend(aIndex, nullptr, rCount, 0, 0);
			}
			if (rType == ewWCoinP)
			{
				gCashShop.GDCashShopAddPointSaveSend(aIndex, nullptr, 0, rCount, 0);
			}
			if (rType == ewWCoinG)
			{
				gCashShop.GDCashShopAddPointSaveSend(aIndex, nullptr, 0, 0, rCount);
			}
			if (rType == ewBuff)
			{
				//g_BuffManagerEx.AddedBuff(aIndex, rItemType, rCount);
				//������� �������
			}
		}

		lpUser->ExWQuestNum[a]++;
		lpUser->ExWQuestStart[a] = false;
		this->GC_MainInfo(aIndex,true);

		//g_BuffManagerEx.GC_QuestBuffInfo(aIndex);
}
//// ----------------------------------------------------------------------------------------------
//
bool ExWinQuestSystem::NewDrop(int aIndexMob, int aIndexUser) const
{
		LPOBJ lpUser = &gObj[aIndexUser];
		const LPOBJ lpMonster = &gObj[aIndexMob];

		switch (lpMonster->Class)
		{
			case 78:
			case 53:
			case 79:
			case 80:
			case 82:
			case 502:
			case 493:
			case 494:
			case 495:
			case 496:
			case 497:
			case 498:
			case 499:
			case 500:
			case 501:
			{
				return false;
			}
		}

		for (int i = 0; i < this->iDropLoaded; i++)
		{
			const int a = this->m_Drop[i].iAct;

			if (a < 0 || a >= EW_MAX_ACT)
			{
				continue;
			}

			if (lpUser->ExWQuestNum[a] >= this->m_Drop[i].iNeedQuest)
			{
				if (lpMonster->Level >= this->m_Drop[i].MonsterMinLevel && lpMonster->Level <= this->m_Drop[i].MonsterMaxLevel)
				{
					if (lpMonster->Map == this->m_Drop[i].MonsterMap || this->m_Drop[i].MonsterMap == -1)
					{
						if (rand() % 10000 < this->m_Drop[i].ItemDropRate)
						{
							int iLuck = 0;
							int iSkill = 0;
							int iOption = 0;
							int iLevel;

							// ----
							if (this->m_Drop[i].ItemMaxLevel == this->m_Drop[i].ItemMinLevel)
							{
								iLevel = this->m_Drop[i].ItemMinLevel;
							}
							else
							{
								iLevel = this->m_Drop[i].ItemMinLevel + (rand() % (this->m_Drop[i].ItemMaxLevel - this->m_Drop[i].ItemMinLevel + 1));
							}

							// ----
							if (this->m_Drop[i].ItemLuck == 1)
							{
								if (rand() % 100 < 50)
								{
									iLuck = 1;
								}
							}
							// ----
							if (this->m_Drop[i].ItemSkill == 1)
							{
								if (rand() % 100 < 50)
								{
									iSkill = 1;
								}
							}
							// ----
							if (this->m_Drop[i].ItemMaxOption != 0)
							{
								if (rand() % 100 < 50)
								{
									iOption = rand() % this->m_Drop[i].ItemMaxOption + 1;
								}
							}

							//ItemSerialCreateSend(lpUser->m_Index, lpMonster->MapNumber, lpMonster->X, lpMonster->Y, 
							//ITEMGET(this->m_Drop[i].ItemType, this->m_Drop[i].ItemIndex), iLevel, this->m_Drop[i].ItemDur, iOption, iLuck, iSkill, gObjMonsterGetTopHitDamageUser(lpMonster), this->m_Drop[i].ItemExcellent, 0);
							GDCreateItemSend(lpUser->Index, lpMonster->Map, static_cast<BYTE>(lpMonster->X), static_cast<BYTE>(lpMonster->Y),
								GET_ITEM(this->m_Drop[i].ItemType, this->m_Drop[i].ItemIndex), iLevel, this->m_Drop[i].ItemDur, iOption, iLuck, iSkill, gObjMonsterGetTopHitDamageUser(lpMonster), this->m_Drop[i].ItemExcellent, 0, 0, 0, nullptr, 0xFF, 0);

							return true;
						}
					}
				}
			}
		}

		return false;
}
//// ----------------------------------------------------------------------------------------------
//
void ExWinQuestSystem::SaveQuest(int aIndex, LPBYTE buf)
{
	return;
	LPOBJ lpUser = &gObj[aIndex];

	int n = 0;
	BYTE Start = 0;
	WORD Number = 0;
	WORD Count = 0;

	for (int a = 0; a < EW_MAX_ACT; a++)
	{
		Start = lpUser->ExWQuestStart[a];
		buf[n] = Start;
		n++;
		WORD Number = lpUser->ExWQuestNum[a];
		buf[n] = SET_NUMBERHB(Number);
		n++;
		buf[n] = SET_NUMBERLB(Number);
		n++;
		for (int m = 0; m < EW_MAX_MISSION; m++)
		{
			Count = lpUser->ExWQuestCount[a][m];
			buf[n] = SET_NUMBERHB(Count);
			n++;
			buf[n] = SET_NUMBERLB(Count);
			n++;
		}
	}

#if(EW_DEBUG==TRUE)
	LogAddC(2, "SaveQuest: %d", n);
#endif
}
//// ----------------------------------------------------------------------------------------------
//
void ExWinQuestSystem::LoadQuest(int aIndex, LPBYTE buf)
{
	return;
	LPOBJ lpUser = &gObj[aIndex];
	int n = 0;

	for (int a = 0; a < EW_MAX_ACT; a++)
	{
		lpUser->ExWQuestStart[a] = buf[n];
		n++;
		lpUser->ExWQuestNum[a] = MAKE_NUMBERW(buf[n], buf[n + 1]);
		n += 2;
		for (int m = 0; m < EW_MAX_MISSION; m++)
		{
			lpUser->ExWQuestCount[a][m] = MAKE_NUMBERW(buf[n], buf[n + 1]);
			n += 2;
		}
	}

#if(EW_DEBUG==TRUE)
	LogAddC(2, "LoadQuest: %d", n);
#endif
}
//// ----------------------------------------------------------------------------------------------