#include "stdafx.h"
#include "GrandResetSystem.h"
#include "user.h"
#include "ItemManager.h"
#include "Path.h"
#include "MemScript.h"
#include "Log.h"
#include "Util.h"
#include "Notice.h"
#include "SkillManager.h"
#include "MasterSkillTree.h"
#include "ServerInfo.h"
#include "ObjectManager.h"
#include "CashShop.h"
#include "BankEx.h"
#include "DSProtocol.h"
#include "Message.h"
#include "MapManager.h"

GRSystem gGRSystem;

GRSystem::GRSystem()
{
	this->Init();
}

GRSystem::~GRSystem()
{
	
}

void GRSystem::Init()
{
	ZeroMemory(this->m_GroupData, sizeof(this->m_GroupData));
	this->m_GroupLoadedCount	= 0;
	this->m_MaxMoney			= 0;
	this->m_ReqCleanInventory	= true;
	this->m_ResetStats			= true;
	this->m_ResetPoints			= true;
	this->m_NpcID				= 0;
	this->m_NpcMap				= 0;
	this->m_NpcX				= 0;
	this->m_NpcY				= 0;
	this->m_BonusCommand		= 0;

	this->m_ResetSkill			= 0;
	this->m_ResetMasterLevel	= 0;
	this->m_ResetMasterStats	= 0;
	this->m_ResetMasterSKill	= 0;
}

void GRSystem::Load()
{
	this->Init();
	this->ReadGroupFile(gPath.GetFullPath("..\\LTPData\\GrandResetSystem\\GrandResetGroup.txt"));
	this->ReadMainFile(gPath.GetFullPath("..\\LTPData\\GrandResetSystem\\GrandResetMain.ini"));
}

void GRSystem::ReadGroupFile(char* path)
{
	CMemScript* lpMemScript = new CMemScript;

	if(lpMemScript == 0)
	{
		ErrorMessageBox(MEM_SCRIPT_ALLOC_ERROR,path);
		return;
	}

	if(lpMemScript->SetBuffer(path) == 0)
	{
		ErrorMessageBox(lpMemScript->GetLastError());
		delete lpMemScript;
		return;
	}

	try
	{
		while(true)
		{
			if(lpMemScript->GetToken() == TOKEN_END)
			{
				break;
			}

			if(strcmp("end",lpMemScript->GetString()) == 0)
			{
				break;
			}

			int GroupID = lpMemScript->GetNumber();;
			// ----
			this->m_GroupData[GroupID].MinGReset		= lpMemScript->GetAsNumber();
			// ----
			this->m_GroupData[GroupID].MaxGReset		= lpMemScript->GetAsNumber();
			// ----
			this->m_GroupData[GroupID].ReqReset[0]		= lpMemScript->GetAsNumber();
			// ----
			this->m_GroupData[GroupID].ReqReset[1]		= lpMemScript->GetAsNumber();
			// ----
			this->m_GroupData[GroupID].ReqReset[2]		= lpMemScript->GetAsNumber();
			// ----
			this->m_GroupData[GroupID].ReqReset[3]		= lpMemScript->GetAsNumber();
			// ----
			this->m_GroupData[GroupID].ReqLevel			= lpMemScript->GetAsNumber();
			// ----
			this->m_GroupData[GroupID].ReqMoney[0]		= lpMemScript->GetAsNumber();
			// ----
			this->m_GroupData[GroupID].ReqMoney[1]		= lpMemScript->GetAsNumber();
			// ----
			this->m_GroupData[GroupID].ReqMoney[2]		= lpMemScript->GetAsNumber();
			// ----
			this->m_GroupData[GroupID].ReqMoney[3]		= lpMemScript->GetAsNumber();
			// ----
			this->m_GroupData[GroupID].ItemCount	= lpMemScript->GetAsNumber();
			// ----
			int RewardPerGroup	= 0;
			// ----
			while(true)
			{
				if( RewardPerGroup >= 7 )
				{
					break;
				}
				// ----
				this->m_GroupData[GroupID].RewardData[RewardPerGroup].LevelPoint = lpMemScript->GetAsNumber();
				// ----
				this->m_GroupData[GroupID].RewardData[RewardPerGroup].GensPoint = lpMemScript->GetAsNumber();
				// ----
				this->m_GroupData[GroupID].RewardData[RewardPerGroup].WCoinC = lpMemScript->GetAsNumber();
				// ----
				this->m_GroupData[GroupID].RewardData[RewardPerGroup].WCoinP = lpMemScript->GetAsNumber();
				// ----
				this->m_GroupData[GroupID].RewardData[RewardPerGroup].GoblinPoint = lpMemScript->GetAsNumber();
				// ----
				this->m_GroupData[GroupID].RewardData[RewardPerGroup].Credits = lpMemScript->GetAsNumber();
				// ----
				RewardPerGroup++;
			}
			// ----
			int ItemPerGroup = 0;
			// ----
			while(true)
			{
				if( ItemPerGroup >= this->m_GroupData[GroupID].ItemCount )
				{
					break;
				}
				// ----
				this->m_GroupData[GroupID].ItemData[ItemPerGroup].ID			= lpMemScript->GetAsNumber();
				// ----
				this->m_GroupData[GroupID].ItemData[ItemPerGroup].MinLevel		= lpMemScript->GetAsNumber();
				// ----
				this->m_GroupData[GroupID].ItemData[ItemPerGroup].MaxLevel		= lpMemScript->GetAsNumber();
				// ----
				this->m_GroupData[GroupID].ItemData[ItemPerGroup].MinOption		= lpMemScript->GetAsNumber();
				// ----
				this->m_GroupData[GroupID].ItemData[ItemPerGroup].MaxOption		= lpMemScript->GetAsNumber();
				// ----
				this->m_GroupData[GroupID].ItemData[ItemPerGroup].IsLuck		= lpMemScript->GetAsNumber();
				// ----
				this->m_GroupData[GroupID].ItemData[ItemPerGroup].IsSkill		= lpMemScript->GetAsNumber();
				// ----
				this->m_GroupData[GroupID].ItemData[ItemPerGroup].IsExcellent	= lpMemScript->GetAsNumber();
				// ----
				this->m_GroupData[GroupID].ItemData[ItemPerGroup].IsAncient		= lpMemScript->GetAsNumber();
				// ----
				this->m_GroupData[GroupID].ItemData[ItemPerGroup].IsSocket		= lpMemScript->GetAsNumber();
				// ----
				ItemPerGroup++;
			}
			// ----
			this->m_GroupLoadedCount++;
		}
	}
	catch(...)
	{
		ErrorMessageBox(lpMemScript->GetLastError());
	}
	delete lpMemScript;
}

void GRSystem::ReadMainFile(char* path)
{
	this->m_MaxMoney			= GetPrivateProfileInt("Common", "MaxMoney", 0, path);
	this->m_ReqCleanInventory	= GetPrivateProfileInt("Common", "InventoryCheck", 0, path);
	this->m_ResetStats			= GetPrivateProfileInt("Common", "ResetStats", 1, path);
	this->m_ResetPoints			= GetPrivateProfileInt("Common", "ResetPoints", 1, path);
	this->m_BonusCommand		= GetPrivateProfileInt("Common", "BonusCommand", 220, path);
	this->m_NpcMap				= GetPrivateProfileInt("NPC", "Map", 0, path);
	this->m_NpcID				= GetPrivateProfileInt("NPC", "ID", 0, path);
	this->m_NpcMap				= GetPrivateProfileInt("NPC", "Map", 0, path);
	this->m_NpcX				= GetPrivateProfileInt("NPC", "X", 0, path);
	this->m_NpcY				= GetPrivateProfileInt("NPC", "Y", 0, path);
	this->m_ResetSkill			= GetPrivateProfileInt("Common", "ResetSkill", 0, path);
	this->m_ResetMasterLevel	= GetPrivateProfileInt("Common", "ResetMasterLevel", 0, path);
	this->m_ResetMasterStats	= GetPrivateProfileInt("Common", "ResetMasterStats", 0, path);
	this->m_ResetMasterSKill	= GetPrivateProfileInt("Common", "ResetMasterSKill", 0, path);
}

int GRSystem::GetResetMoney(LPOBJ lpObj)
{
	int Group = this->GetGRGroup(lpObj);
	// ----
	if( Group == -1 )
	{
		return -1;
	}
	// ----
	QWORD Money = (QWORD)this->m_GroupData[Group].ReqMoney[lpObj->AccountLevel] * (lpObj->MasterReset + 1);
	// ----
	if( Money > this->m_MaxMoney )
	{
		Money = this->m_MaxMoney;
	}
	// ----
	return (int)Money;
}

int GRSystem::GetGRGroup(LPOBJ lpObj)
{
	for( int Group = 0; Group < this->m_GroupLoadedCount; Group++ )
	{
		if( lpObj->MasterReset >= this->m_GroupData[Group].MinGReset && lpObj->MasterReset < this->m_GroupData[Group].MaxGReset )
		{
			return Group;
		}
	}
	// ----
	return -1;
}

bool GRSystem::SearchQuestItem(LPOBJ lpObj, BYTE QuestItemID)
{
	int GrandResetGroup = this->GetGRGroup(lpObj);

	if( GrandResetGroup == -1 )
	{
		LogAdd(LOG_RED, "[GrandResetSystem] [%s] [%s] Invalid group", lpObj->Account, lpObj->Name);
		return false;
	}
	// ----
	WORD ItemID = this->m_GroupData[GrandResetGroup].ItemData[QuestItemID].ID;
	// ----
	int MaxValue = gItemManager.GetInventoryMaxValue(lpObj);

	for (int i = INVENTORY_WEAR_SIZE; i < MaxValue; i++)
	{
		if( !lpObj->Inventory[i].IsItem() || lpObj->Inventory[i].m_Index != ItemID )
		{
			continue;
		}
		// ----
		if( lpObj->Inventory[i].m_Level < this->m_GroupData[GrandResetGroup].ItemData[QuestItemID].MinLevel
			|| lpObj->Inventory[i].m_Level > this->m_GroupData[GrandResetGroup].ItemData[QuestItemID].MaxLevel )
		{
			continue;
		}
		// ----
		if( lpObj->Inventory[i].m_Option < this->m_GroupData[GrandResetGroup].ItemData[QuestItemID].MinOption
			|| lpObj->Inventory[i].m_Option > this->m_GroupData[GrandResetGroup].ItemData[QuestItemID].MaxOption )
		{
			continue;
		}
		// ----
		if( this->m_GroupData[GrandResetGroup].ItemData[QuestItemID].IsLuck != 2 )
		{
			if( lpObj->Inventory[i].m_LuckOption != this->m_GroupData[GrandResetGroup].ItemData[QuestItemID].IsLuck )
			{
				continue;
			}
		}
		// ----
		if( this->m_GroupData[GrandResetGroup].ItemData[QuestItemID].IsSkill != 2 )
		{
			if( lpObj->Inventory[i].m_SkillOption != this->m_GroupData[GrandResetGroup].ItemData[QuestItemID].IsSkill )
			{
				continue;
			}
		}
		// ----
		if( this->m_GroupData[GrandResetGroup].ItemData[QuestItemID].IsExcellent )
		{
			if( !lpObj->Inventory[i].m_NewOption )
			{
				continue;
			}
		}
		// ----
		if( this->m_GroupData[GrandResetGroup].ItemData[QuestItemID].IsAncient )
		{
			if( !lpObj->Inventory[i].m_SetOption )
			{
				continue;
			}
		}
		// ----
		if( this->m_GroupData[GrandResetGroup].ItemData[QuestItemID].IsSocket )
		{
			if (!lpObj->Inventory[i].IsSocketItem())
			{
				continue;
			}
			//if( !gSocketItemType.CheckSocketItemType(ItemID) )
		}
		// ----
		return true;
	}
	// ----
	return false;
}

bool GRSystem::DeleteQuestItem(LPOBJ lpObj, BYTE QuestItemID)
{
	int GrandResetGroup = this->GetGRGroup(lpObj);

	if( GrandResetGroup == -1 )
	{
		LogAdd(LOG_RED, "[GrandResetSystem] [%s] [%s] Invalid group", lpObj->Account, lpObj->Name);
		return false;
	}
	// ----
	WORD ItemID = this->m_GroupData[GrandResetGroup].ItemData[QuestItemID].ID;
	// ----
	int MaxValue = gItemManager.GetInventoryMaxValue(lpObj);

	for (int i = INVENTORY_WEAR_SIZE; i < MaxValue; i++)
	{
		if( !lpObj->Inventory[i].IsItem() || lpObj->Inventory[i].m_Index != ItemID )
		{
			continue;
		}
		// ----
		if( lpObj->Inventory[i].m_Level < this->m_GroupData[GrandResetGroup].ItemData[QuestItemID].MinLevel
			|| lpObj->Inventory[i].m_Level > this->m_GroupData[GrandResetGroup].ItemData[QuestItemID].MaxLevel )
		{
			continue;
		}
		// ----
		if( lpObj->Inventory[i].m_Option < this->m_GroupData[GrandResetGroup].ItemData[QuestItemID].MinOption
			|| lpObj->Inventory[i].m_Option > this->m_GroupData[GrandResetGroup].ItemData[QuestItemID].MaxOption )
		{
			continue;
		}
		// ----
		if( this->m_GroupData[GrandResetGroup].ItemData[QuestItemID].IsLuck != 2 )
		{
			if( lpObj->Inventory[i].m_LuckOption != this->m_GroupData[GrandResetGroup].ItemData[QuestItemID].IsLuck )
			{
				continue;
			}
		}
		// ----
		if( this->m_GroupData[GrandResetGroup].ItemData[QuestItemID].IsSkill != 2 )
		{
			if( lpObj->Inventory[i].m_SkillOption != this->m_GroupData[GrandResetGroup].ItemData[QuestItemID].IsSkill )
			{
				continue;
			}
		}
		// ----
		if( this->m_GroupData[GrandResetGroup].ItemData[QuestItemID].IsExcellent )
		{
			if( !lpObj->Inventory[i].m_NewOption )
			{
				continue;
			}
		}
		// ----
		if( this->m_GroupData[GrandResetGroup].ItemData[QuestItemID].IsAncient )
		{
			if( !lpObj->Inventory[i].m_SetOption )
			{
				continue;
			}
		}
		// ----
		if( this->m_GroupData[GrandResetGroup].ItemData[QuestItemID].IsSocket )
		{
			if (!lpObj->Inventory[i].IsSocketItem())
			{
				continue;
			}
			/*if( !gSocketItemType.CheckSocketItemType(ItemID) )
			{
				continue;
			}*/
		}
		// ----
		gItemManager.InventoryDelItem(lpObj->Index,i);
		gItemManager.GCItemDeleteSend(lpObj->Index,i,1);
		// ----
		return true;
	}
	// ----
	return false;
}

bool GRSystem::CheckAllReq(LPOBJ lpObj)
{
	int GrandResetGroup = this->GetGRGroup(lpObj);

	if( GrandResetGroup == -1 )
	{
		gLog.Output(LOG_GRANDRESET,"[%s] [%s] [Fail] Invalid group (GrandReset: %d, Group: %d)", 
			lpObj->Account, lpObj->Name, lpObj->MasterReset, GrandResetGroup);
		return false;
	}
	// ----
	if (lpObj->Reset < this->m_GroupData[GrandResetGroup].ReqReset[lpObj->AccountLevel])
	{
		gLog.Output(LOG_GRANDRESET,"[%s] [%s] [Fail] Reset is small to Grand reset (%d / %d)", 
			lpObj->Account, lpObj->Name, lpObj->Reset, this->m_GroupData[GrandResetGroup].ReqReset[lpObj->AccountLevel]); 
	}
	// ----
	if( lpObj->Level < this->m_GroupData[GrandResetGroup].ReqLevel )
	{
		gLog.Output(LOG_GRANDRESET,"[%s] [%s] [Fail] Level is small to Grand reset (%d / %d)", 
			lpObj->Account, lpObj->Name, lpObj->Level, this->m_GroupData[GrandResetGroup].ReqLevel); 
		return false;
	}
	// ----
	int Money = this->GetResetMoney(lpObj);
	// ----
	if( Money == -1 )
	{
		gLog.Output(LOG_GRANDRESET,"[%s] [%s] [Fail] Money return == -1", 
			lpObj->Account, lpObj->Name);
		return false;
	}
	// ----
	if( lpObj->Money < Money )
	{
		gLog.Output(LOG_GRANDRESET,"[%s] [%s] [Fail] Not have money for reset (%d / %d)", 
			lpObj->Account, lpObj->Name, lpObj->Money, Money);
		return false;
	}
	// ----
	if( this->m_ReqCleanInventory )
	{
		for( int i = 0; i < 12; i++ )
		{
			if( lpObj->Inventory[i].IsItem() )
			{
				gLog.Output(LOG_GRANDRESET,"[%s] [%s] [Fail] User have equiped items (ItemSlot: %d)", 
					lpObj->Account, lpObj->Name, i); 
				return false;
			}
		}
	}
	// ----
	if( this->m_GroupData[GrandResetGroup].ItemCount > 0 )
	{
		for( int i = 0; i < this->m_GroupData[GrandResetGroup].ItemCount; i++ )
		{
			if( !this->SearchQuestItem(lpObj, i) )
			{
				gLog.Output(LOG_GRANDRESET,"[%s] [%s] [Fail] User not have req. items ([%d] ItemID: %d)",
					lpObj->Account, lpObj->Name, i, this->m_GroupData[GrandResetGroup].ItemData[i].ID);
				return false;
			}
		}
	}
	// ----
	return true;
}

bool GRSystem::Dialog(LPOBJ lpObj, LPOBJ lpNpc)
{
	if(		lpNpc->Class		== this->m_NpcID 
		&&	lpNpc->Map			== this->m_NpcMap
		&&	lpNpc->X			== this->m_NpcX
		&&	lpNpc->Y			== this->m_NpcY )
	{
		this->SendResetData(lpObj, true);
		return true;
	}
	// ----
	return false;
}

void GRSystem::SendResetData(LPOBJ lpObj, bool ByDialog)
{
	int Group = this->GetGRGroup(lpObj);
	if( ByDialog && Group == -1 )
	{
		LogAdd(LOG_RED, "[GrandResetSystem] [%s] [%s] Invalid group", lpObj->Account, lpObj->Name);
		return;
	}
	if (Group == -1)
	{
		return;
	}
	// ----
	GR_ANS_USERDATA pSend;
	pSend.h.set(GRProtocol::Case, GRProtocol::UserData, sizeof(pSend));
	// ----
	pSend.ByDialog	= ByDialog;
	pSend.Reset		= lpObj->Reset;
	pSend.GrandReset = lpObj->MasterReset;
	pSend.GResetMoney = this->GetResetMoney(lpObj);
	pSend.CommandPerGReset = this->m_BonusCommand;
	pSend.QuestStat = lpObj->iQuestStat;
	// ----
	pSend.GrData.MinGReset =  this->m_GroupData[this->GetGRGroup(lpObj)].MinGReset;
	pSend.GrData.MaxGReset =  this->m_GroupData[this->GetGRGroup(lpObj)].MaxGReset;
	pSend.GrData.ReqLevel =  this->m_GroupData[this->GetGRGroup(lpObj)].ReqLevel;
	pSend.GrData.ReqReset =  this->m_GroupData[this->GetGRGroup(lpObj)].ReqReset[lpObj->AccountLevel];
	pSend.GrData.ItemCount = this->m_GroupData[this->GetGRGroup(lpObj)].ItemCount;

	pSend.GrData.RewardData = this->m_GroupData[this->GetGRGroup(lpObj)].RewardData[lpObj->Class];
	
	/*if (this->m_ResetPoints)
	{
		pSend.GrData.RewardData.LevelPoint += lpObj->iQuestStat;
	}*/

	// ----
	if( this->m_GroupData[Group].ItemCount > 0 )
	{
		for( int i = 0; i < this->m_GroupData[Group].ItemCount; i++ )
		{
			pSend.GrData.ItemData[i] = this->m_GroupData[this->GetGRGroup(lpObj)].ItemData[i];

			pSend.ItemCheck[i] = true;
			// ----
			if( !this->SearchQuestItem(lpObj, i) )
			{
				pSend.ItemCheck[i] = false;
			}
		}
	}
	// ----
	DataSend(lpObj->Index, (LPBYTE)&pSend, pSend.h.size);
}

void GRSystem::FinishResetReq(LPOBJ lpObj)
{
	if( this->GetGRGroup(lpObj) == -1 )
	{
		LogAdd(LOG_RED, "[GrandResetSystem] [%s] [%s] Invalid group", lpObj->Account, lpObj->Name);
		return;
	}
	// ----
	GR_REQ_USERDATA pSend;
	pSend.h.set(GRProtocol::Case, GRProtocol::ResetFinish, sizeof(pSend));
	// ----
	int ResetGroup = this->GetGRGroup(lpObj);
	// ----
	if( !this->CheckAllReq(lpObj) )
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,"You must finish all the requirements for make Reset");
		return;
	}
	// ----
	if(lpObj->Interface.use != 0 || lpObj->State == OBJECT_DELCMD || lpObj->DieRegen != 0 || lpObj->Teleport != 0 || lpObj->PShopOpen != 0 || lpObj->SkillSummonPartyTime != 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(90));
		return;
	}
	// ----
	if (lpObj->CloseType != -1)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,"You can't make reset, if you try exit the game");
		return;
	}
	// ----
	if (gMapManager.GetMapGResetEnable(lpObj->Map) == 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,"You can not make a grand reset on this map");
		return;
	}
	// ----
	if( this->m_GroupData[ResetGroup].ItemCount > 0 )
	{
		for( int i = 0; i < this->m_GroupData[ResetGroup].ItemCount; i++ )
		{
			if( !this->DeleteQuestItem(lpObj, i) )
			{
				gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,"Please check quest items positions");
				return;
			}
		}
	}
	// ----
	lpObj->Level		= 1;
	lpObj->Reset		= 0;
	lpObj->Experience	= 0;
	// ----
	if( this->m_ResetStats )
	{
		lpObj->Strength	= gDefaultClassInfo.m_DefaultClassInfo[lpObj->Class].Strength;
		lpObj->Dexterity	= gDefaultClassInfo.m_DefaultClassInfo[lpObj->Class].Dexterity;
		lpObj->Energy		= gDefaultClassInfo.m_DefaultClassInfo[lpObj->Class].Energy;
		lpObj->Vitality	= gDefaultClassInfo.m_DefaultClassInfo[lpObj->Class].Vitality;
		lpObj->Leadership	= gDefaultClassInfo.m_DefaultClassInfo[lpObj->Class].Leadership;
		lpObj->NewStat = gDefaultClassInfo.m_DefaultClassInfo[lpObj->Class].NewStat;

		if (lpObj->Class == CLASS_DL)
		{
			lpObj->Leadership += this->m_BonusCommand * (lpObj->MasterReset + 1);
		}
	}
	// ----

	//if (gCustomsManager.GetCustomState(CUSTOM_QUESTSYSTEM) == 1)
	//{
		if( this->m_ResetPoints )
		{
			lpObj->LevelUpPoint = this->m_GroupData[ResetGroup].RewardData[lpObj->Class].LevelPoint * (lpObj->MasterReset + 1) + lpObj->iQuestStat;
			//lpObj->LevelUpPoint = ((this->m_GroupData[ResetGroup].RewardData[lpObj->Class].LevelPoint * (lpObj->Reset + 1)) + (gServerInfo.m_CommandMasterResetPoint[lpObj->AccountLevel]*lpObj->MasterReset)) + lpObj->iQuestStat;
		}
		else
		{
			lpObj->LevelUpPoint += this->m_GroupData[ResetGroup].RewardData[lpObj->Class].LevelPoint * (lpObj->MasterReset + 1);
			//lpObj->LevelUpPoint += ((this->m_GroupData[ResetGroup].RewardData[lpObj->Class].LevelPoint * (lpObj->Reset + 1)) + (gServerInfo.m_CommandMasterResetPoint[lpObj->AccountLevel]*lpObj->MasterReset)) + lpObj->iQuestStat;
		}
	//}
	/*else
	{
		if( m_ResetPoints )
		{
			lpObj->LevelUpPoint = (this->m_GroupData[ResetGroup].RewardData[lpObj->Class].LevelPoint * (lpObj->Reset + 1)) + (gServerInfo.m_CommandMasterResetPoint[lpObj->AccountLevel]*lpObj->MasterReset);
		}
		else
		{
			lpObj->LevelUpPoint += (this->m_GroupData[ResetGroup].RewardData[lpObj->Class].LevelPoint * (lpObj->Reset + 1)) + (gServerInfo.m_CommandMasterResetPoint[lpObj->AccountLevel]*lpObj->MasterReset);
		}
	}*/

	if( this->m_ResetSkill && this->m_ResetMasterSKill )
	{
		lpObj->MasterLevel = 0;

		lpObj->MasterPoint = 0;

		for(int n=0;n < MAX_SKILL_LIST;n++)
		{
			lpObj->Skill[n].Clear();
		}

		for(int n=0;n < MAX_MASTER_SKILL_LIST;n++)
		{
			lpObj->MasterSkill[n].Clear();
		}

		gMasterSkillTree.GCMasterSkillListSend(lpObj->Index);

		gSkillManager.GCSkillListSend(lpObj,0);

		gObjectManager.CharacterCalcAttribute(lpObj->Index);

		gMasterSkillTree.GCMasterInfoSend(lpObj);
	}
	else if( this->m_ResetSkill )
	{
		for(int n=0;n < MAX_SKILL_LIST;n++)
		{
			lpObj->Skill[n].Clear();
		}

		gSkillManager.GCSkillListSend(lpObj,0);
		gObjectManager.CharacterCalcAttribute(lpObj->Index);
	}

	if( this->m_ResetMasterLevel )
	{
		lpObj->MasterLevel = 0;
		lpObj->MasterPoint = 0;
		gMasterSkillTree.GCMasterInfoSend(lpObj);
		gObjectManager.CharacterCalcAttribute(lpObj->Index);
	}

	if( this->m_ResetMasterStats )
	{
		lpObj->MasterPoint = 0;
	}

	// ----
	lpObj->GensContribution 		+= this->m_GroupData[ResetGroup].RewardData[lpObj->Class].GensPoint;
	// ----
	gCashShop.GDCashShopAddPointSaveSend(lpObj->Index, 0, this->m_GroupData[ResetGroup].RewardData[lpObj->Class].WCoinC, this->m_GroupData[ResetGroup].RewardData[lpObj->Class].WCoinP,
		this->m_GroupData[ResetGroup].RewardData[lpObj->Class].GoblinPoint);
	// ----
	lpObj->BankEx.Credits += this->m_GroupData[ResetGroup].RewardData[lpObj->Class].Credits;
	gBankEx.GCUpdateBankEx(lpObj->Index);
	gBankEx.GDSavePoint(lpObj->Index);
	// ----
	lpObj->Money					-= this->GetResetMoney(lpObj);
	GCMoneySend(lpObj->Index, lpObj->Money);
	//lpObj->Reset++;
	lpObj->MasterReset++;
	gAchievements.GrandResetUp(lpObj->Index);
	// ----
	gLog.Output(LOG_GRANDRESET,"[%s] [%s] [Success] GrandReset: %d -> %d | Points: %d",
		lpObj->Account, lpObj->Name, lpObj->MasterReset - 1, lpObj->MasterReset, lpObj->LevelUpPoint);
	// ----
	/*if( lpObj->PartyNumber >= 0 )
	{
		gParty.DelMember(lpObj->PartyNumber, lpObj->Index);
		lpObj->PartyTargetUser	= -1;
	}*/
	// ----
	gObjectManager.CharacterCalcAttribute(lpObj->Index);

	GCNewCharacterInfoSend(lpObj);

	GDCharacterInfoSaveSend(lpObj->Index);

	GDMasterResetInfoSaveSend(lpObj->Index,0,0,0);

	gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(95),lpObj->Reset);
	// ----
	gObjMoveGate(lpObj->Index, gDefaultClassInfo.m_DefaultClassInfo[lpObj->Class].StartGate);
	/*switch(lpObj->Class)
	{
		case CLASS_DW:
			gObjMoveGate(lpObj->Index,17);
			break;
		case CLASS_DK:
			gObjMoveGate(lpObj->Index,17);
			break;
		case CLASS_FE:
			gObjMoveGate(lpObj->Index,27);
			break;
		case CLASS_MG:
			gObjMoveGate(lpObj->Index,17);
			break;
		case CLASS_DL:
			gObjMoveGate(lpObj->Index,17);
			break;
		case CLASS_SU:
			gObjMoveGate(lpObj->Index,267);
			break;
		case CLASS_RF:
			gObjMoveGate(lpObj->Index,17);
			break;
	}*/
	// ----
	//gGateC.GetGate(Gate, (short&)X, (short&)Y, (BYTE&)MapNumber, (BYTE&)Dir, (short&)Level);
	// ----
	//lpObj->MapNumber	= MapNumber;
	//lpObj->X			= X;
	//lpObj->Y			= Y;
	//lpObj->Dir			= Dir;
	// ----
	DataSend(lpObj->Index, (LPBYTE)&pSend, pSend.h.size);
	this->SendResetData(lpObj, false);
	//gObjCloseSet(lpObj->Index, 1);
}

int GRSystem::GetGRPoints(LPOBJ lpObj)
{
	int ResetGroup = this->GetGRGroup(lpObj);
	if (lpObj->MasterReset > 0)
	{
		return this->m_GroupData[ResetGroup].RewardData[lpObj->Class].LevelPoint * lpObj->MasterReset;
	}

	return 0;
}