#include "StdAfx.h"
#include "ResetSystem.h"
#include "user.h"
#include "GameMain.h"
#include "Path.h"
#include "ItemManager.h"
#include "SocketItemType.h"
#include "Notice.h"
#include "Skill.h"
#include "MasterSkillTree.h"
#include "ObjectManager.h"
#include "CashShop.h"
#include "Party.h"
#include "DSProtocol.h"
#include "ServerInfo.h"
#include "Quest.h"
#include "Gate.h"
#include "Message.h"
#include "Util.h"
#include "Log.h"
#include "MemScript.h"
#include "BankEx.h"
#include "GrandResetSystem.h"
#include "MapManager.h"
#include "ChaosBoxManager.h"
//#include "RandomManager.h"
// -------------------------------------------------------------------------

// -------------------------------------------------------------------------

ResetSystem		g_ResetSystem;
// -------------------------------------------------------------------------

ResetSystem::ResetSystem()
{
	this->Init();
}
// -------------------------------------------------------------------------

ResetSystem::~ResetSystem()
{

}
// -------------------------------------------------------------------------

void ResetSystem::Init()
{
	this->m_ItemRewardInfo.clear();
	ZeroMemory(this->m_GroupData, sizeof(this->m_GroupData));
	this->m_GroupLoadedCount	= 0;
	this->m_MaxMoney			= 0;
	this->m_ReqCleanInventory	= true;
	this->m_ShopCleaning		= true;
	this->m_ResetStats			= true;
	this->m_ResetPoints			= true;
	this->m_NpcID				= 0;
	this->m_NpcMap				= 0;
	this->m_NpcX				= 0;
	this->m_NpcY				= 0;
	this->m_MarlonReset			= 0;
	this->m_MarlonStatMinLevel	= 220;
	this->m_MarlonComboMinLevel	= 220;
	this->m_BonusCommand		= 0;

	this->m_ResetSkill			= 0;
	this->m_ResetMasterLevel	= 0;
	this->m_ResetMasterStats	= 0;
	this->m_ResetMasterSKill	= 0;
}
// -------------------------------------------------------------------------

void ResetSystem::Load()
{
	//if (!IsLicenseChecked)
	//{
	//	return;
	//}
	this->Init();
	this->ReadMainData(gPath.GetFullPath("..\\LTPData\\ResetSystem\\ResetMain.ini"));
	this->ReadGroupData(gPath.GetFullPath("..\\LTPData\\ResetSystem\\ResetGroup.txt"));
	this->ReadItemRewardData(gPath.GetFullPath("..\\LTPData\\ResetSystem\\ResetItemReward.xml"));
}
// -------------------------------------------------------------------------

void ResetSystem::ReadGroupData(char * path)
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

			int GroupID = lpMemScript->GetNumber();
			// ----
			this->m_GroupData[GroupID].MinReset		= lpMemScript->GetAsNumber();
			// ----
			this->m_GroupData[GroupID].MaxReset		= lpMemScript->GetAsNumber();
			// ----
			if(this->m_AddGResetToGroup)
			{
				this->m_GroupData[GroupID].MinGReset = lpMemScript->GetAsNumber();
				this->m_GroupData[GroupID].MaxGReset = lpMemScript->GetAsNumber();
			}
			// ----
			this->m_GroupData[GroupID].ReqLevel[0]		= lpMemScript->GetAsNumber();
			// ----
			this->m_GroupData[GroupID].ReqLevel[1]		= lpMemScript->GetAsNumber();
			// ----
			this->m_GroupData[GroupID].ReqLevel[2]		= lpMemScript->GetAsNumber();
			// ----
			this->m_GroupData[GroupID].ReqLevel[3]		= lpMemScript->GetAsNumber();
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
			//this->m_GroupData[GroupID].ItemRewardCount = lpMemScript->GetAsNumber();
			//MsgBox("count = %d",this->m_GroupData[GroupID].ItemRewardCount);
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
				this->m_GroupData[GroupID].ItemData[ItemPerGroup].Count		= lpMemScript->GetAsNumber();
				// ----
				ItemPerGroup++;
			}
			// ----
			//ItemPerGroup = 0;
			// ----
			//MsgBox("Group = %d Count = %d = %d",GroupID,this->m_GroupData[GroupID].ItemRewardCount,this->m_GroupData[GroupID].ItemCount);
			//while(true)
			//{
			//	if( ItemPerGroup >= this->m_GroupData[GroupID].ItemRewardCount )
			//	{
			//		break;
			//	}
			//	// ----
			//	this->m_GroupData[GroupID].ItemRewardData[ItemPerGroup].ID			= lpMemScript->GetAsNumber();
			//	//MsgBox("Id = %d [%d]",this->m_GroupData[GroupID].ItemRewardData[ItemPerGroup].ID,ItemPerGroup);
			//	// ----
			//	this->m_GroupData[GroupID].ItemRewardData[ItemPerGroup].MinLevel	= lpMemScript->GetAsNumber();
			//	// ----
			//	this->m_GroupData[GroupID].ItemRewardData[ItemPerGroup].MaxLevel	= lpMemScript->GetAsNumber();
			//	// ----
			//	this->m_GroupData[GroupID].ItemRewardData[ItemPerGroup].MinOption	= lpMemScript->GetAsNumber();
			//	// ----
			//	this->m_GroupData[GroupID].ItemRewardData[ItemPerGroup].MaxOption	= lpMemScript->GetAsNumber();
			//	// ----
			//	this->m_GroupData[GroupID].ItemRewardData[ItemPerGroup].IsLuck		= lpMemScript->GetAsNumber();
			//	// ----
			//	this->m_GroupData[GroupID].ItemRewardData[ItemPerGroup].IsSkill		= lpMemScript->GetAsNumber();
			//	// ----
			//	this->m_GroupData[GroupID].ItemRewardData[ItemPerGroup].IsExcellent	= lpMemScript->GetAsNumber();
			//	// ----
			//	this->m_GroupData[GroupID].ItemRewardData[ItemPerGroup].IsAncient	= lpMemScript->GetAsNumber();
			//	// ----
			//	this->m_GroupData[GroupID].ItemRewardData[ItemPerGroup].IsSocket	= lpMemScript->GetAsNumber();
			//	// ----
			//	this->m_GroupData[GroupID].ItemRewardData[ItemPerGroup].Count		= lpMemScript->GetAsNumber();

			//	//MsgBox("%d - %d - %d - %d - %d - %d - %d",this->m_GroupData[GroupID].ItemRewardData[ItemPerGroup].ID,this->m_GroupData[GroupID].ItemRewardData[ItemPerGroup].MinLevel,
			//	//	this->m_GroupData[GroupID].ItemRewardData[ItemPerGroup].MaxLevel, this->m_GroupData[GroupID].ItemRewardData[ItemPerGroup].MinOption, this->m_GroupData[GroupID].ItemRewardData[ItemPerGroup].MaxOption,
			//	//	this->m_GroupData[GroupID].ItemRewardData[ItemPerGroup].IsLuck,this->m_GroupData[GroupID].ItemRewardData[ItemPerGroup].IsSkill);
			//	// ----
			//	ItemPerGroup++;
			//}
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
// -------------------------------------------------------------------------

void ResetSystem::ReadItemRewardData(char* path)
{
	//return ;
	pugi::xml_document file;
	pugi::xml_parse_result res = file.load_file(path);

	if (res.status != pugi::status_ok)
	{
		ErrorMessageBox("Failed to load %s file (%s)", path, res.description());
		return;
	}

	this->m_ItemRewardInfo.clear();

	pugi::xml_node main = file.child("ResetItemReward");

	for (auto group = main.child("Group"); group; group = group.next_sibling())
	{
		RESET_ITEM_REWARD_DATA info;
		info.MinGReset = group.attribute("MinGReset").as_int();
		info.MaxGReset = group.attribute("MaxGReset").as_int();
		info.MinReset = group.attribute("MinReset").as_int();
		info.MaxReset = group.attribute("MaxReset").as_int();

		for (auto classGroup = group.child("Class"); classGroup; classGroup = classGroup.next_sibling())
		{
			int classIndex = classGroup.attribute("Index").as_int();
			info.ItemReward[classIndex].Money = classGroup.attribute("Zen").as_int();
			info.ItemReward[classIndex].WCoinC = classGroup.attribute("WCoinC").as_int();
			info.ItemReward[classIndex].WCoinP = classGroup.attribute("WCoinP").as_int();
			info.ItemReward[classIndex].WCoinG = classGroup.attribute("WCoinG").as_int();
			info.ItemReward[classIndex].Credits = classGroup.attribute("Credits").as_int();

			int itemCount = 0;
			for (auto itemRew = classGroup.child("Item"); itemRew; itemRew = itemRew.next_sibling())
			{
				info.ItemReward[classIndex].itemData[itemCount].ID = itemRew.attribute("ID").as_int();
				info.ItemReward[classIndex].itemData[itemCount].MinLevel = itemRew.attribute("MinLevel").as_int();
				info.ItemReward[classIndex].itemData[itemCount].MaxLevel = itemRew.attribute("MaxLevel").as_int();
				info.ItemReward[classIndex].itemData[itemCount].MinOption = itemRew.attribute("MinOption").as_int();
				info.ItemReward[classIndex].itemData[itemCount].MaxOption = itemRew.attribute("MaxOption").as_int();
				info.ItemReward[classIndex].itemData[itemCount].IsLuck = itemRew.attribute("IsLuck").as_int();
				info.ItemReward[classIndex].itemData[itemCount].IsSkill = itemRew.attribute("IsSkill").as_int();
				info.ItemReward[classIndex].itemData[itemCount].ExcType = itemRew.attribute("ExcType").as_int();
				info.ItemReward[classIndex].itemData[itemCount].MinExcOption = itemRew.attribute("MinExcellentCount").as_int();
				info.ItemReward[classIndex].itemData[itemCount].MaxExcOption = itemRew.attribute("MaxExcellentCount").as_int();
				info.ItemReward[classIndex].itemData[itemCount].ExcCode = itemRew.attribute("ExcCode").as_int();
				info.ItemReward[classIndex].itemData[itemCount].IsAncient = itemRew.attribute("IsAncient").as_int();
				info.ItemReward[classIndex].itemData[itemCount].IsSocket = itemRew.attribute("MaxSocket").as_int();
				info.ItemReward[classIndex].itemData[itemCount].Count = itemRew.attribute("Count").as_int();
				//MsgBox("Class = %d, ItemId = %d",classIndex,info.ItemReward[classIndex].itemData[itemCount].ID);
				itemCount++;
			}
			info.ItemReward[classIndex].ItemCount = itemCount;
		}


		//int itemCount = 0;
		/*for (auto itemRew = group.child("Item"); itemRew; itemRew = itemRew.next_sibling())
		{
			if (itemCount>=MAX_CLASS)
			{
				ErrorMessageBox("Failed to load %s file (Incorrect class count)", path);
				return;
			}

			info.ItemReward[itemCount].ID = itemRew.attribute("ID").as_int();
			info.ItemReward[itemCount].MinLevel = itemRew.attribute("MinLevel").as_int();
			info.ItemReward[itemCount].MaxLevel = itemRew.attribute("MaxLevel").as_int();
			info.ItemReward[itemCount].MinOption = itemRew.attribute("MinOption").as_int();
			info.ItemReward[itemCount].MaxOption = itemRew.attribute("MaxOption").as_int();
			info.ItemReward[itemCount].IsLuck = itemRew.attribute("IsLuck").as_int();
			info.ItemReward[itemCount].IsSkill = itemRew.attribute("IsSkill").as_int();
			info.ItemReward[itemCount].IsExcellent = itemRew.attribute("MaxExcellentCount").as_int();
			info.ItemReward[itemCount].IsAncient = itemRew.attribute("IsAncient").as_int();
			info.ItemReward[itemCount].IsSocket = itemRew.attribute("MaxSocket").as_int();
			info.ItemReward[itemCount].Count = itemRew.attribute("Count").as_int();
			info.ItemReward[itemCount].WCoinC = itemRew.attribute("WCoinC").as_int();
			info.ItemReward[itemCount].WCoinP = itemRew.attribute("WCoinP").as_int();
			info.ItemReward[itemCount].WCoinG = itemRew.attribute("WCoinG").as_int();
			info.ItemReward[itemCount].Credits = itemRew.attribute("Credits").as_int();
			itemCount++;
		}*/

		this->m_ItemRewardInfo.push_back(info);
	}
}

void ResetSystem::ReadMainData(char * File)
{
	this->m_MaxMoney			= GetPrivateProfileInt("Common", "MaxMoney", 0, File);
	this->m_ShopCleaning		= GetPrivateProfileInt("Common", "PShopCleaning", 0, File);
	this->m_MultiplyToReset		= GetPrivateProfileInt("Common", "MultiplyToReset", 1, File);
	this->m_AddGResetToGroup	= GetPrivateProfileInt("Common", "AddGResetToGroup", 0, File);
	this->m_ReqCleanInventory	= GetPrivateProfileInt("Common", "InventoryCheck", 0, File);
	this->m_ResetStats			= GetPrivateProfileInt("Common", "ResetStats", 1, File);
	this->m_ResetPoints			= GetPrivateProfileInt("Common", "ResetPoints", 1, File);
	this->m_MarlonReset			= GetPrivateProfileInt("Common", "MarlonReset", 0, File);
	this->m_MarlonStatMinLevel	= GetPrivateProfileInt("Common", "MarlonStatMinLevel", 220, File);
	this->m_MarlonComboMinLevel	= GetPrivateProfileInt("Common", "MarlonComboMinLevel", 220, File);
	this->m_BonusCommand		= GetPrivateProfileInt("Common", "BonusCommand", 220, File);
	this->m_NpcMap				= GetPrivateProfileInt("NPC", "Map", 0, File);
	this->m_NpcID				= GetPrivateProfileInt("NPC", "ID", 0, File);
	this->m_NpcMap				= GetPrivateProfileInt("NPC", "Map", 0, File);
	this->m_NpcX				= GetPrivateProfileInt("NPC", "X", 0, File);
	this->m_NpcY				= GetPrivateProfileInt("NPC", "Y", 0, File);
	this->m_ResetSkill			= GetPrivateProfileInt("Common", "ResetSkill", 0, File);
	this->m_ResetMasterLevel	= GetPrivateProfileInt("Common", "ResetMasterLevel", 0, File);
	this->m_ResetMasterStats	= GetPrivateProfileInt("Common", "ResetMasterStats", 0, File);
	this->m_ResetMasterSKill	= GetPrivateProfileInt("Common", "ResetMasterSKill", 0, File);
}
// -------------------------------------------------------------------------

int ResetSystem::GetResetMoney(LPOBJ lpUser)
{
	int Group = this->GetResetGroup(lpUser);
	// ----
	if( Group == -1 )
	{
		return -1;
	}
	// ----

	long long Money;

	if (this->m_MultiplyToReset)
		Money = long long(this->m_GroupData[this->GetResetGroup(lpUser)].ReqMoney[lpUser->AccountLevel]) * long long(lpUser->Reset + 1);
	else
		Money = long long(this->m_GroupData[this->GetResetGroup(lpUser)].ReqMoney[lpUser->AccountLevel]);

	// ----
	if( Money > (long long)this->m_MaxMoney )
	{
		Money = (long long)this->m_MaxMoney;
	}
	// ----
	return Money;
}
// -------------------------------------------------------------------------

int ResetSystem::GetResetGroup(LPOBJ lpUser)
{
	for( int Group = 0; Group < this->m_GroupLoadedCount; Group++ )
	{
		if(this->m_AddGResetToGroup)
		{
			if ((lpUser->Reset >= this->m_GroupData[Group].MinReset && lpUser->Reset < this->m_GroupData[Group].MaxReset) &&
				(lpUser->MasterReset >= this->m_GroupData[Group].MinGReset && lpUser->MasterReset < this->m_GroupData[Group].MaxGReset))
			{
				return Group;
			}
		}
		else
		{
			if (lpUser->Reset >= this->m_GroupData[Group].MinReset && lpUser->Reset < this->m_GroupData[Group].MaxReset)
			{
				return Group;
			}
		}
		
	}
	// ----
	return -1;
}
// -------------------------------------------------------------------------

RESET_ITEM_REWARD* ResetSystem::GetResetReward(LPOBJ lpObj)
{
	for(std::vector<RESET_ITEM_REWARD_DATA>::iterator it=this->m_ItemRewardInfo.begin();it != this->m_ItemRewardInfo.end();it++)
	{
		//LogAdd(LOG_TEST,"minres = %d max = %d mingres = %d max = %d",it->MinReset,it->MaxReset,it->MinGReset,it->MaxGReset);
		if ((lpObj->MasterReset >= it->MinGReset && lpObj->MasterReset < it->MaxGReset) && (lpObj->Reset >= it->MinReset && lpObj->Reset < it->MaxReset))
		{
			//LogAdd(LOG_TEST,"Found!");
			return &(it->ItemReward[lpObj->Class]);
		}
	}

	return 0;

}
// -------------------------------------------------------------------------

bool ResetSystem::SearchQuestItem(LPOBJ lpUser, BYTE QuestItemID)
{
	if( this->GetResetGroup(lpUser) == (int)-1 )
	{
		LogAdd(LOG_RED, "[ResetSystem] [%s] [%s] Invalid group", lpUser->Account, lpUser->Name);
		return false;
	}
	// ----
	WORD ItemID = this->m_GroupData[this->GetResetGroup(lpUser)].ItemData[QuestItemID].ID;
	// ----
	int MaxValue = gItemManager.GetInventoryMaxValue(lpUser);

	int count = 0;

	for (int i = INVENTORY_WEAR_SIZE; i < MaxValue; i++)
	{
		if( !lpUser->Inventory[i].IsItem() || lpUser->Inventory[i].m_Index != ItemID )
		{
			continue;
		}
		// ----
		if( lpUser->Inventory[i].m_Level < this->m_GroupData[this->GetResetGroup(lpUser)].ItemData[QuestItemID].MinLevel
			|| lpUser->Inventory[i].m_Level > this->m_GroupData[this->GetResetGroup(lpUser)].ItemData[QuestItemID].MaxLevel )
		{
			continue;
		}
		// ----
		if( lpUser->Inventory[i].m_Option < this->m_GroupData[this->GetResetGroup(lpUser)].ItemData[QuestItemID].MinOption
			|| lpUser->Inventory[i].m_Option > this->m_GroupData[this->GetResetGroup(lpUser)].ItemData[QuestItemID].MaxOption )
		{
			continue;
		}
		// ----
		if( this->m_GroupData[this->GetResetGroup(lpUser)].ItemData[QuestItemID].IsLuck != 2 )
		{
			if( lpUser->Inventory[i].m_LuckOption != this->m_GroupData[this->GetResetGroup(lpUser)].ItemData[QuestItemID].IsLuck )
			{
				continue;
			}
		}
		// ----
		if( this->m_GroupData[this->GetResetGroup(lpUser)].ItemData[QuestItemID].IsSkill != 2 )
		{
			if( lpUser->Inventory[i].m_SkillOption != this->m_GroupData[this->GetResetGroup(lpUser)].ItemData[QuestItemID].IsSkill )
			{
				continue;
			}
		}
		// ----
		if( this->m_GroupData[this->GetResetGroup(lpUser)].ItemData[QuestItemID].IsExcellent )
		{
			if( !lpUser->Inventory[i].m_NewOption )
			{
				continue;
			}
		}
		// ----
		if( this->m_GroupData[this->GetResetGroup(lpUser)].ItemData[QuestItemID].IsAncient )
		{
			if( !lpUser->Inventory[i].m_SetOption )
			{
				continue;
			}
		}
		// ----
		if( this->m_GroupData[this->GetResetGroup(lpUser)].ItemData[QuestItemID].IsSocket )
		{
			if( !lpUser->Inventory[i].IsSocketItem()/*gSocketItemType.CheckSocketItemType(ItemID)*/ )
			{
				continue;
			}
		}
		// ----
		count++;
	}
	// ----
	if (count>=this->m_GroupData[this->GetResetGroup(lpUser)].ItemData[QuestItemID].Count)
	{
		return true;
	}

	return false;
}
// -------------------------------------------------------------------------------

bool ResetSystem::DeleteQuestItem(LPOBJ lpUser, BYTE QuestItemID)
{
	if( this->GetResetGroup(lpUser) == (int)-1 )
	{
		LogAdd(LOG_RED, "[ResetSystem] [%s] [%s] Invalid group", lpUser->Account, lpUser->Name);
		return false;
	}
	// ----
	WORD ItemID = this->m_GroupData[this->GetResetGroup(lpUser)].ItemData[QuestItemID].ID;
	// ----
	int MaxValue = gItemManager.GetInventoryMaxValue(lpUser);
	int count = 0;

	for (int i = INVENTORY_WEAR_SIZE; i < MaxValue; i++)
	{
		if( !lpUser->Inventory[i].IsItem() || lpUser->Inventory[i].m_Index != ItemID )
		{
			continue;
		}
		// ----
		if( lpUser->Inventory[i].m_Level < this->m_GroupData[this->GetResetGroup(lpUser)].ItemData[QuestItemID].MinLevel
			|| lpUser->Inventory[i].m_Level > this->m_GroupData[this->GetResetGroup(lpUser)].ItemData[QuestItemID].MaxLevel )
		{
			continue;
		}
		// ----
		if( lpUser->Inventory[i].m_Option < this->m_GroupData[this->GetResetGroup(lpUser)].ItemData[QuestItemID].MinOption
			|| lpUser->Inventory[i].m_Option > this->m_GroupData[this->GetResetGroup(lpUser)].ItemData[QuestItemID].MaxOption )
		{
			continue;
		}
		// ----
		if( this->m_GroupData[this->GetResetGroup(lpUser)].ItemData[QuestItemID].IsLuck != 2 )
		{
			if( lpUser->Inventory[i].m_LuckOption != this->m_GroupData[this->GetResetGroup(lpUser)].ItemData[QuestItemID].IsLuck )
			{
				continue;
			}
		}
		// ----
		if( this->m_GroupData[this->GetResetGroup(lpUser)].ItemData[QuestItemID].IsSkill != 2 )
		{
			if( lpUser->Inventory[i].m_SkillOption != this->m_GroupData[this->GetResetGroup(lpUser)].ItemData[QuestItemID].IsSkill )
			{
				continue;
			}
		}
		// ----
		if( this->m_GroupData[this->GetResetGroup(lpUser)].ItemData[QuestItemID].IsExcellent )
		{
			if( !lpUser->Inventory[i].m_NewOption )
			{
				continue;
			}
		}
		// ----
		if( this->m_GroupData[this->GetResetGroup(lpUser)].ItemData[QuestItemID].IsAncient )
		{
			if( !lpUser->Inventory[i].m_SetOption )
			{
				continue;
			}
		}
		// ----
		if( this->m_GroupData[this->GetResetGroup(lpUser)].ItemData[QuestItemID].IsSocket )
		{
			if( !gSocketItemType.CheckSocketItemType(ItemID) )
			{
				continue;
			}
		}
		// ----
		gItemManager.InventoryDelItem(lpUser->Index,i);
		gItemManager.GCItemDeleteSend(lpUser->Index,i,1);
		// ----
		count++;
		if (count>=this->m_GroupData[this->GetResetGroup(lpUser)].ItemData[QuestItemID].Count)
		return true;
	}
	// ----
	return false;
}
// -------------------------------------------------------------------------------

bool ResetSystem::Dialog(LPOBJ lpUser, LPOBJ lpNpc)
{
	if(		lpNpc->Class		== this->m_NpcID 
		&&	lpNpc->Map			== this->m_NpcMap
		&&	lpNpc->X			== this->m_NpcX
		&&	lpNpc->Y			== this->m_NpcY )
	{
		this->SendResetData(lpUser, true);
		return true;
	}
	// ----
	return false;
}
// -------------------------------------------------------------------------

bool ResetSystem::CheckAllReq(LPOBJ lpUser)
{
	if( this->GetResetGroup(lpUser) == (int)-1 )
	{
		gLog.Output(LOG_RESET,"[%s] [%s] [Fail] Invalid group (Reset: %d, Group: %d)", 
			lpUser->Account, lpUser->Name, lpUser->Reset, this->GetResetGroup(lpUser));
		return false;
	}
	// ----
	int	ResetGroup = this->GetResetGroup(lpUser);
	// ----
	if( lpUser->Level < this->m_GroupData[ResetGroup].ReqLevel[lpUser->AccountLevel] )
	{
		gLog.Output(LOG_RESET,"[%s] [%s] [Fail] Level is small to reset (%d / %d)", 
			lpUser->Account, lpUser->Name, lpUser->Level, this->m_GroupData[ResetGroup].ReqLevel[lpUser->AccountLevel]); 
		return false;
	}
	// ----
	int Money = this->GetResetMoney(lpUser);
	// ----
	if( Money == -1 )
	{
		gLog.Output(LOG_RESET,"[%s] [%s] [Fail] Money return == -1", 
			lpUser->Account, lpUser->Name);
		return false;
	}
	// ----
	if( lpUser->Money < Money )
	{
		gLog.Output(LOG_RESET,"[%s] [%s] [Fail] Not have money for reset (%d / %d)", 
			lpUser->Account, lpUser->Name, lpUser->Money, Money);
		return false;
	}
	// ----
	if( this->m_ReqCleanInventory )
	{
		for( int i = 0; i < 12; i++ )
		{
			if( lpUser->Inventory[i].IsItem() )
			{
				gLog.Output(LOG_RESET,"[%s] [%s] [Fail] User have equiped items (ItemSlot: %d)", 
					lpUser->Account, lpUser->Name, i); 
				return false;
			}
		}
	}
	// ----
	if( this->m_GroupData[ResetGroup].ItemCount > 0 )
	{
		for( int i = 0; i < this->m_GroupData[ResetGroup].ItemCount; i++ )
		{
			if( !this->SearchQuestItem(lpUser, i) )
			{
				gLog.Output(LOG_RESET,"[%s] [%s] [Fail] User not have req. items ([%d] ItemID: %d)",
					lpUser->Account, lpUser->Name, i, this->m_GroupData[ResetGroup].ItemData[i].ID);
				return false;
			}
		}
	}
	// ----
	return true;
}
// -------------------------------------------------------------------------
//CRandomManager RandomManager;
void ResetSystem::FinishResetReq(LPOBJ lpUser)
{
	if( this->GetResetGroup(lpUser) == (int)-1 )
	{
		LogAdd(LOG_RED, "[ResetSystem] [%s] [%s] Invalid group", lpUser->Account, lpUser->Name);
		return;
	}
	// ----
	RESET_REQ_USERDATA pSend;
	pSend.h.set(ResetProtocol::Case, ResetProtocol::ResetFinish, sizeof(pSend));
	// ----
	int ResetGroup = this->GetResetGroup(lpUser);
	// ----
	if( !this->CheckAllReq(lpUser) )
	{
		gNotice.GCNoticeSend(lpUser->Index,1,0,0,0,0,0,"You must finish all the requirements for make Reset");
		return;
	}
	// ----
	if(lpUser->Interface.use != 0 || lpUser->State == OBJECT_DELCMD || lpUser->DieRegen != 0 || lpUser->Teleport != 0 || lpUser->PShopOpen != 0 || lpUser->SkillSummonPartyTime != 0)
	{
		gNotice.GCNoticeSend(lpUser->Index,1,0,0,0,0,0,gMessage.GetMessage(90));
		return;
	}
	// ----
	if (lpUser->CloseType != -1)
	{
		gNotice.GCNoticeSend(lpUser->Index,1,0,0,0,0,0,"You can't make reset, if you try exit the game");
		return;
	}
	// ----
	if (gMapManager.GetMapResetEnable(lpUser->Map) == 0)
	{
		gNotice.GCNoticeSend(lpUser->Index,1,0,0,0,0,0,"You can not make a reset on this map");
		return;
	}
	// ----
	if( this->m_GroupData[ResetGroup].ItemCount > 0 )
	{
		for( int i = 0; i < this->m_GroupData[ResetGroup].ItemCount; i++ )
		{
			if( !this->DeleteQuestItem(lpUser, i) )
			{
				gNotice.GCNoticeSend(lpUser->Index,1,0,0,0,0,0,"Please check quest items positions");
				return;
			}
		}
	}
	// ----
	lpUser->Level		= 1;
	lpUser->Experience	= 0;
	// ----
	if( this->m_ResetStats )
	{
		lpUser->Strength	= gDefaultClassInfo.m_DefaultClassInfo[lpUser->Class].Strength;
		lpUser->Dexterity	= gDefaultClassInfo.m_DefaultClassInfo[lpUser->Class].Dexterity;
		lpUser->Energy		= gDefaultClassInfo.m_DefaultClassInfo[lpUser->Class].Energy;
		lpUser->Vitality	= gDefaultClassInfo.m_DefaultClassInfo[lpUser->Class].Vitality;
		lpUser->Leadership	= gDefaultClassInfo.m_DefaultClassInfo[lpUser->Class].Leadership;
		lpUser->NewStat		= gDefaultClassInfo.m_DefaultClassInfo[lpUser->Class].NewStat;

		if (lpUser->Class == CLASS_DL)
		{
			lpUser->Leadership += this->m_BonusCommand * (lpUser->Reset + 1);
		}
	}
	// ----
	if( this->m_MarlonReset )
	{
		//g_QuestInfo.ReSetQuestState(lpUser, 2);
		//g_QuestInfo.ReSetQuestState(lpUser, 3);
	}

	/*if( m_ResetPoints )
	{
		lpUser->LevelUpPoint = ((this->m_GroupData[ResetGroup].RewardData[lpUser->Class].LevelPoint * (lpUser->Reset + 1)) + (gServerInfo.m_CommandMasterResetPoint[lpUser->AccountLevel]*lpUser->MasterReset)) + lpUser->iQuestStat;
	}
	else
	{
		lpUser->LevelUpPoint += ((this->m_GroupData[ResetGroup].RewardData[lpUser->Class].LevelPoint * (lpUser->Reset + 1)) + (gServerInfo.m_CommandMasterResetPoint[lpUser->AccountLevel]*lpUser->MasterReset)) + lpUser->iQuestStat;
	}*/

	if (this->m_ResetPoints)
	{
		lpUser->LevelUpPoint = ((this->m_GroupData[ResetGroup].RewardData[lpUser->Class].LevelPoint * (lpUser->Reset + 1)) + gGRSystem.GetGRPoints(lpUser) + lpUser->iQuestStat);
	}
	else
	{
		lpUser->LevelUpPoint += this->m_GroupData[ResetGroup].RewardData[lpUser->Class].LevelPoint * (lpUser->Reset + 1);
	}

	if( this->m_ResetSkill && this->m_ResetMasterSKill )
	{
		lpUser->MasterLevel = 0;

		lpUser->MasterPoint = 0;

		for(int n=0;n < MAX_SKILL_LIST;n++)
		{
			lpUser->Skill[n].Clear();
		}

		for(int n=0;n < MAX_MASTER_SKILL_LIST;n++)
		{
			lpUser->MasterSkill[n].Clear();
		}

		gMasterSkillTree.GCMasterSkillListSend(lpUser->Index);

		gSkillManager.GCSkillListSend(lpUser,0);

		gObjectManager.CharacterCalcAttribute(lpUser->Index);

		gMasterSkillTree.GCMasterInfoSend(lpUser);
	}
	else if( this->m_ResetSkill )
	{
		for(int n=0;n < MAX_SKILL_LIST;n++)
		{
			lpUser->Skill[n].Clear();
		}

		gSkillManager.GCSkillListSend(lpUser,0);
		gObjectManager.CharacterCalcAttribute(lpUser->Index);
	}

	if( this->m_ResetMasterLevel )
	{
		lpUser->MasterLevel = 0;
		lpUser->MasterPoint = 0;
		gMasterSkillTree.GCMasterInfoSend(lpUser);
		gObjectManager.CharacterCalcAttribute(lpUser->Index);
	}

	if( this->m_ResetMasterStats )
	{
		lpUser->MasterPoint = 0;
	}

	// ----
	lpUser->GensContribution 		+= this->m_GroupData[ResetGroup].RewardData[lpUser->Class].GensPoint;
	// ----
	lpUser->Money					-= this->GetResetMoney(lpUser);
	GCMoneySend(lpUser->Index, lpUser->Money);
	// ----
	/*if( lpUser->PartyNumber >= 0 )
	{
		gParty.DelMember(lpUser->PartyNumber, lpUser->Index);
		lpUser->PartyTargetUser	= -1;
	}*/
	// ----
	// ----
	// ----

	RESET_ITEM_REWARD* item = this->GetResetReward(lpUser);

	lpUser->Reset++;
	gAchievements.ResetUp(lpUser->Index);
	// ----
	if (!gObjMoveGate(lpUser->Index, gDefaultClassInfo.m_DefaultClassInfo[lpUser->Class].StartGate))
	{
		return;
	}

	if (item != 0)
	{
		for	(int k=0;k<item->ItemCount;k++)
		{
			for (int i=0;i<item->itemData[k].Count;i++)
			{
				int itemLevel = 0;
				if (item->itemData[k].MaxLevel == item->itemData[k].MinLevel)
				{
					itemLevel = item->itemData[k].MaxLevel;
				}
				else if (item->itemData[k].MinLevel < item->itemData[k].MaxLevel)
				{
					int Sub = (item->itemData[k].MaxLevel - item->itemData[k].MinLevel) + 1;
					itemLevel = item->itemData[k].MinLevel + (GetLargeRand() % Sub);
				}
				else
				{
					itemLevel = item->itemData[k].MinLevel;
				}

				int itemOpt = 0;
				if (item->itemData[k].MaxOption == item->itemData[k].MinOption )
				{
					itemOpt = item->itemData[k].MaxOption;
				}
				else if (item->itemData[k].MinOption < item->itemData[k].MaxOption )
				{
					int Sub = (item->itemData[k].MaxOption - item->itemData[k].MinOption) + 1;
					itemOpt = item->itemData[k].MinOption + (GetLargeRand() % Sub);
				}
				else
				{
					itemOpt = item->itemData[k].MinOption;
				}


				int ExcOption=0;
				if (item->itemData[k].ExcType == 1)
				{
					ExcOption = item->itemData[k].ExcCode;
				}
				else
				{
					if (item->itemData[k].MaxExcOption > 0)
					{
						int ExcOptionCount = item->itemData[k].MinExcOption;
						// ----
						if( ExcOptionCount != item->itemData[k].MaxExcOption  )
						{
							ExcOptionCount += GetLargeRand() % (1 + (item->itemData[k].MaxExcOption  - ExcOptionCount));
						}
						// ----
						while(ExcOptionCount != 0)
						{

							BYTE ExcValue;
							RandomManager.GetRandomElement(&ExcValue);
							// ----
							if( (ExcOption & ExcValue) == 0 )
							{
								ExcOption |= ExcValue;
								ExcOptionCount--;
							}
						}
					}
				}
				

				BYTE SocketOption[5] = {0xFF,0xFF,0xFF,0xFF,0xFF};
				if( gSocketItemType.CheckSocketItemType(item->itemData[k].ID) )
				{
					int SocketMin = 1;
					int SocketMax = item->itemData[k].IsSocket;
					int SocketCount = 0;
					// ----
					if( SocketMax < SocketMin )
					{
						SocketMax = SocketMin;
					}
					// ----
					SocketCount = SocketMin;
					// ----
					if( SocketMin != SocketMax )
					{
						SocketCount += GetLargeRand() % (1 + (SocketMax - SocketMin));
					}
					// ----
					for( int i = 0; i < MAX_SOCKET_OPTION; i++ )
					{
						if( i < SocketCount )
						{
							SocketOption[i] = (BYTE)-2;
						}
						else
						{
							SocketOption[i] = (BYTE)-1;
						}
					}
				}

				GDCreateItemSend(lpUser->Index,lpUser->Map,lpUser->X,lpUser->Y,item->itemData[k].ID,itemLevel,0,item->itemData[k].IsSkill,item->itemData[k].IsLuck,itemOpt,-1,ExcOption,0,0,0,SocketOption,0xFF,0);
			}
		}

		gCashShop.GDCashShopAddPointSaveSend(lpUser->Index, 0, item->WCoinC, item->WCoinP, item->WCoinG);
		// ----
		lpUser->BankEx.Credits += item->Credits;
		gBankEx.GCUpdateBankEx(lpUser->Index);
		gBankEx.GDSavePoint(lpUser->Index);

		lpUser->Money += item->Money;
		GCMoneySend(lpUser->Index, lpUser->Money);
	}
	else
	{
		// ----
		gCashShop.GDCashShopAddPointSaveSend(lpUser->Index, 0, this->m_GroupData[ResetGroup].RewardData[lpUser->Class].WCoinC, this->m_GroupData[ResetGroup].RewardData[lpUser->Class].WCoinP,
			this->m_GroupData[ResetGroup].RewardData[lpUser->Class].GoblinPoint);
		// ----
		lpUser->BankEx.Credits += this->m_GroupData[ResetGroup].RewardData[lpUser->Class].Credits;
		gBankEx.GCUpdateBankEx(lpUser->Index);
		gBankEx.GDSavePoint(lpUser->Index);
	}

	gEffectManager.DelEffect(lpUser,EFFECT_NOOB_BUFF);

	if (lpUser->Reset < gServerInfo.m_CustomNoobBuffMaxReset[lpUser->AccountLevel] && lpUser->MasterReset == 0)
	{
		gEffectManager.AddEffect(lpUser,0,EFFECT_NOOB_BUFF,0,0,0,0,0);
	}
	// ----
	gLog.Output(LOG_RESET,"[%s] [%s] [Success] Reset: %d -> %d | Points: %d",
		lpUser->Account, lpUser->Name, lpUser->Reset - 1, lpUser->Reset, lpUser->LevelUpPoint);

	gNotice.GCNoticeSend(lpUser->Index,1,0,0,0,0,0,gMessage.GetMessage(95),lpUser->Reset);

	gObjectManager.CharacterCalcAttribute(lpUser->Index);

	GCNewCharacterInfoSend(lpUser);

	GDCharacterInfoSaveSend(lpUser->Index);

	GDResetInfoSaveSend(lpUser->Index,0,0,0);

	//for	(int i=0;i<this->m_GroupData[ResetGroup].ItemRewardCount;i++)
	//{
	//	int itemLevel = 0;
	//	if (this->m_GroupData[ResetGroup].ItemRewardData[i].MaxLevel == this->m_GroupData[ResetGroup].ItemRewardData[i].MinLevel)
	//	{
	//		itemLevel = this->m_GroupData[ResetGroup].ItemRewardData[i].MaxLevel;
	//	}
	//	else if (this->m_GroupData[ResetGroup].ItemRewardData[i].MinLevel < this->m_GroupData[ResetGroup].ItemRewardData[i].MaxLevel)
	//	{
	//		int Sub = (this->m_GroupData[ResetGroup].ItemRewardData[i].MaxLevel - this->m_GroupData[ResetGroup].ItemRewardData[i].MinLevel) + 1;
	//		itemLevel = this->m_GroupData[ResetGroup].ItemRewardData[i].MinLevel + (GetLargeRand() % Sub);
	//	}
	//	else
	//	{
	//		itemLevel = this->m_GroupData[ResetGroup].ItemRewardData[i].MinLevel;
	//	}

	//	int itemOpt = 0;
	//	if (this->m_GroupData[ResetGroup].ItemRewardData[i].MaxOption == this->m_GroupData[ResetGroup].ItemRewardData[i].MinOption )
	//	{
	//		itemOpt = this->m_GroupData[ResetGroup].ItemRewardData[i].MaxOption;
	//	}
	//	else if (this->m_GroupData[ResetGroup].ItemRewardData[i].MinOption < this->m_GroupData[ResetGroup].ItemRewardData[i].MaxOption )
	//	{
	//		int Sub = (this->m_GroupData[ResetGroup].ItemRewardData[i].MaxOption - this->m_GroupData[ResetGroup].ItemRewardData[i].MinOption) + 1;
	//		itemOpt = this->m_GroupData[ResetGroup].ItemRewardData[i].MinOption + (GetLargeRand() % Sub);
	//	}
	//	else
	//	{
	//		itemOpt = this->m_GroupData[ResetGroup].ItemRewardData[i].MinOption;
	//	}


	//	int ExcOption=0;
	//	if (this->m_GroupData[ResetGroup].ItemRewardData[i].IsExcellent > 0)
	//	{
	//		int ExcOptionCount = 1;
	//		// ----
	//		if( ExcOptionCount != this->m_GroupData[ResetGroup].ItemRewardData[i].IsExcellent  )
	//		{
	//			ExcOptionCount += GetLargeRand() % (1 + (this->m_GroupData[ResetGroup].ItemRewardData[i].IsExcellent  - ExcOptionCount));
	//		}
	//		// ----
	//		while(ExcOptionCount != 0)
	//		{

	//			BYTE ExcValue;
	//			RandomManager.GetRandomElement(&ExcValue);
	//			// ----
	//			if( (ExcOption & ExcValue) == 0 )
	//			{
	//				ExcOption |= ExcValue;
	//				ExcOptionCount--;
	//			}
	//		}
	//	}

	//	BYTE SocketOption[5] = {0xFF,0xFF,0xFF,0xFF,0xFF};
	//	if( gSocketItemType.CheckSocketItemType(this->m_GroupData[ResetGroup].ItemRewardData[i].ID) )
	//	{
	//		int SocketMin = 1;
	//		int SocketMax = this->m_GroupData[ResetGroup].ItemRewardData[i].IsSocket;
	//		int SocketCount = 0;
	//		// ----
	//		if( SocketMax < SocketMin )
	//		{
	//			SocketMax = SocketMin;
	//		}
	//		// ----
	//		SocketCount = SocketMin;
	//		// ----
	//		if( SocketMin != SocketMax )
	//		{
	//			SocketCount += GetLargeRand() % (1 + (SocketMax - SocketMin));
	//		}
	//		// ----
	//		for( int i = 0; i < MAX_SOCKET_OPTION; i++ )
	//		{
	//			if( i < SocketCount )
	//			{
	//				SocketOption[i] = (BYTE)-2;
	//			}
	//			else
	//			{
	//				SocketOption[i] = (BYTE)-1;
	//			}
	//		}
	//	}

	//	GDCreateItemSend(lpUser->Index,lpUser->Map,lpUser->X,lpUser->Y,this->m_GroupData[ResetGroup].ItemRewardData[i].ID,itemLevel,0,this->m_GroupData[ResetGroup].ItemRewardData[i].IsSkill,this->m_GroupData[ResetGroup].ItemRewardData[i].IsLuck,itemOpt,-1,ExcOption,0,0,0,SocketOption,0xFF,0);
	//}

	// ----

	lpUser->AutoAddPointCount = 0;

	lpUser->AutoAddPointStats[0] = 0;

	lpUser->AutoAddPointStats[1] = 0;

	lpUser->AutoAddPointStats[2] = 0;

	lpUser->AutoAddPointStats[3] = 0;

	lpUser->AutoAddPointStats[4] = 0;

	lpUser->AutoAddPointStats[5] = 0;

	/*switch(lpUser->Class)
	{
		case CLASS_DW:
			gObjMoveGate(lpUser->Index,17);
			break;
		case CLASS_DK:
			gObjMoveGate(lpUser->Index,17);
			break;
		case CLASS_FE:
			gObjMoveGate(lpUser->Index,27);
			break;
		case CLASS_MG:
			gObjMoveGate(lpUser->Index,17);
			break;
		case CLASS_DL:
			gObjMoveGate(lpUser->Index,17);
			break;
		case CLASS_SU:
			gObjMoveGate(lpUser->Index,267);
			break;
		case CLASS_RF:
			gObjMoveGate(lpUser->Index,17);
			break;
	}*/
	// ----
	//gGateC.GetGate(Gate, (short&)X, (short&)Y, (BYTE&)MapNumber, (BYTE&)Dir, (short&)Level);
	// ----
	//lpUser->MapNumber	= MapNumber;
	//lpUser->X			= X;
	//lpUser->Y			= Y;
	//lpUser->Dir			= Dir;
	// ----
	DataSend(lpUser->Index, (LPBYTE)&pSend, pSend.h.size);
	g_ResetSystem.SendResetData(lpUser, false);
	//gObjCloseSet(lpUser->Index, 1);
}
// -------------------------------------------------------------------------

void ResetSystem::SendResetData(LPOBJ lpUser, bool ByDialog)
{
	if( ByDialog && this->GetResetGroup(lpUser) == (int)-1 )
	{
		LogAdd(LOG_RED, "[ResetSystem] [%s] [%s] Invalid group", lpUser->Account, lpUser->Name);
		return;
	}
	// ----
	//LogAdd(LOG_TEST,"sizeof(%d)",sizeof(RESET_ANS_USERDATA));
	RESET_ANS_USERDATA pSend;
	pSend.h.set(ResetProtocol::Case, ResetProtocol::UserData, sizeof(pSend));
	// ----
	pSend.ByDialog	= ByDialog;
	pSend.Reset		= lpUser->Reset;
	pSend.GrandReset = lpUser->MasterReset;
	pSend.ResetMoney = this->GetResetMoney(lpUser);
	pSend.CommandPerReset = this->m_BonusCommand;
	pSend.QuestStat = lpUser->iQuestStat;

	pSend.ResetData.MinReset =  this->m_GroupData[this->GetResetGroup(lpUser)].MinReset;
	pSend.ResetData.MaxReset =  this->m_GroupData[this->GetResetGroup(lpUser)].MaxReset;
	pSend.ResetData.ReqLevel =  this->m_GroupData[this->GetResetGroup(lpUser)].ReqLevel[lpUser->AccountLevel];
	pSend.ResetData.ItemCount = this->m_GroupData[this->GetResetGroup(lpUser)].ItemCount;

	pSend.ResetData.RewardData = this->m_GroupData[this->GetResetGroup(lpUser)].RewardData[lpUser->Class];
	
	if (this->m_ResetPoints)
	{
		//LogAdd(LOG_TEST,"Stage 1: %d",pSend.ResetData.RewardData.LevelPoint);
		//lpUser->LevelUpPoint = ((this->m_GroupData[ResetGroup].RewardData[lpUser->Class].LevelPoint * (lpUser->Reset + 1)) + gGRSystem.GetGRPoints(lpUser) + lpUser->iQuestStat);
		pSend.ResetData.RewardData.LevelPoint *= lpUser->Reset+1;
		//pSend.ResetData.RewardData.LevelPoint += lpUser->iQuestStat;
	}

	//LogAdd(LOG_TEST,"ItemCount = %d",this->m_GroupData[this->GetResetGroup(lpUser)].ItemCount);
	// ----
	if( this->m_GroupData[this->GetResetGroup(lpUser)].ItemCount > 0 )
	{
		for( int i = 0; i < this->m_GroupData[this->GetResetGroup(lpUser)].ItemCount; i++ )
		{
			pSend.ResetData.ItemData[i] = this->m_GroupData[this->GetResetGroup(lpUser)].ItemData[i];

			pSend.ItemCheck[i] = true;
			// ----
			if( !this->SearchQuestItem(lpUser, i) )
			{
				pSend.ItemCheck[i] = false;
			}
		}
	}

	RESET_ITEM_REWARD* item = (this->GetResetReward(lpUser));
	if (item != 0)
	{
		pSend.ResetData.ItemReward = *item;
		pSend.ResetData.UseItemReward = true;
	}
	else
	{
		memset(&pSend.ResetData.ItemReward,0,sizeof(pSend.ResetData.ItemReward));
		pSend.ResetData.UseItemReward = false;
	}

	//LogAdd(LOG_TEST,"ItemId = %d use = %d",pSend.ResetData.ItemReward.ID,pSend.ResetData.UseItemReward);

	
	// ----
	//LogAdd(LOG_TEST,"ItemRewardCount = %d",this->m_GroupData[this->GetResetGroup(lpUser)].ItemRewardCount);
	//if (this->m_GroupData[this->GetResetGroup(lpUser)].ItemRewardCount > 0)
	//{
	//	for	(int i=0;i<this->m_GroupData[this->GetResetGroup(lpUser)].ItemRewardCount;i++)
	//	{
	//		//LogAdd(LOG_TEST,"ItemIndex = %d",this->m_GroupData[this->GetResetGroup(lpUser)].ItemRewardData[i].ID);
	//		pSend.ResetData.ItemRewardData[i] = this->m_GroupData[this->GetResetGroup(lpUser)].ItemRewardData[i];
	//	}
	//}
	// ----
	//LogAdd(LOG_TEST,"%d itenid = %d",pSend.ByDialog,pSend.ResetData.ItemReward.itemData[0].ID);
	//LogAdd(LOG_TEST,"Stage 4: %d",pSend.ResetData.RewardData.LevelPoint);
	DataSend(lpUser->Index, (LPBYTE)&pSend, sizeof(pSend));
}
// -------------------------------------------------------------------------

int ResetSystem::GetResetPoints(LPOBJ lpObj)
{
	int ResetGroup = this->GetResetGroup(lpObj);

	if (lpObj->Reset >= 1)
	{
		if (this->m_ResetPoints)
		{
			return ((this->m_GroupData[ResetGroup].RewardData[lpObj->Class].LevelPoint * lpObj->Reset) + gGRSystem.GetGRPoints(lpObj) + lpObj->iQuestStat);
		}
		else
		{
			return lpObj->LevelUpPoint + (this->m_GroupData[ResetGroup].RewardData[lpObj->Class].LevelPoint * lpObj->Reset);
		}
	}

	return 0;
}