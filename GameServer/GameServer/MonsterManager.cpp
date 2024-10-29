// MonsterManager.cpp: implementation of the CMonsterManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MonsterManager.h"
#include "380ItemType.h"
#include "BloodCastle.h"
#include "BonusManager.h"
#include "CastleDeep.h"
#include "CastleSiege.h"
#include "ChaosCastle.h"
#include "Crywolf.h"
#include "CustomArena.h"
#include "CustomEventDrop.h"
#include "DevilSquare.h"
#include "IllusionTemple.h"
#include "InvasionManager.h"
#include "ItemManager.h"
#include "Map.h"
#include "MemScript.h"
#include "Monster.h"
#include "MonsterSetBase.h"
#include "MossMerchant.h"
#include "ObjectManager.h"
#include "Raklion.h"
#include "ServerInfo.h"
#include "SocketItemType.h"
#include "User.h"
#include "Diablo.h"
#include "Path.h"
#include "TvTEvent.h"
#include "Util.h"
#include "VipLotteryEvent.h"

CMonsterManager gMonsterManager;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMonsterManager::CMonsterManager() // OK
{
	this->Init();
}

CMonsterManager::~CMonsterManager() // OK
{

}

void CMonsterManager::Init() // OK
{
	for(int n=0;n < MAX_MONSTER_INFO;n++)
	{
		this->m_MonsterInfo[n].Index = -1;
	}

	for(int n=0;n < MAX_MONSTER_LEVEL;n++)
	{
		memset(this->m_MonsterItemInfo[n].IndexTable,-1,sizeof(this->m_MonsterItemInfo[n].IndexTable));

		this->m_MonsterItemInfo[n].IndexCount = 0;
	}
}

void CMonsterManager::load_xml(char* path)
{
	if (std::filesystem::exists("Convert\\Monster.txt"))
	{
		std::string dir = "Convert\\Generated";

		if (!std::filesystem::is_directory(dir) || !std::filesystem::exists(dir)) { // Check if src folder exists
			std::filesystem::create_directory(dir); // create src folder
		}

		this->LoadTXT("Convert\\Monster","Convert\\Generated\\MonsterList");
		//return;
	}

	pugi::xml_document file;
	pugi::xml_parse_result res = file.load_file(path);

	if (res.status != pugi::status_ok)
	{
		ErrorMessageBox("%s file load fail (%s)", path, res.description());
	}

	this->Init();

	pugi::xml_node main = file.child("MonsterList");

	for (pugi::xml_node monster = main.child("Monster"); monster; monster = monster.next_sibling())
	{
		MONSTER_INFO info = {};

		info.Index = monster.attribute("Index").as_int();

		info.Rate = monster.attribute("Rate").as_int();

		strcpy_s(info.Name, monster.attribute("Name").as_string());

		info.Level = monster.attribute("Level").as_int();

		info.Life = monster.attribute("Life").as_int();

		info.Mana = monster.attribute("Mana").as_int();

		info.DamageMin = monster.attribute("DamageMin").as_int();

		info.DamageMax = monster.attribute("DamageMax").as_int();

		info.Defense = monster.attribute("Defense").as_int();

		info.MagicDefense = monster.attribute("MagicDefense").as_int();

		info.AttackRate = monster.attribute("AttackRate").as_int();

		info.DefenseRate = monster.attribute("DefenseRate").as_int();

		info.MoveRange = monster.attribute("MoveRange").as_int();

		info.AttackType = monster.attribute("AttackType").as_int();

		info.AttackRange = monster.attribute("AttackRange").as_int();

		info.ViewRange = monster.attribute("ViewRange").as_int();

		info.MoveSpeed = monster.attribute("MoveSpeed").as_int();

		info.AttackSpeed = monster.attribute("AttackSpeed").as_int();

		info.RegenTime = monster.attribute("RegenTime").as_int();

		info.Attribute = monster.attribute("Attribute").as_int();

		info.ItemRate = monster.attribute("ItemRate").as_int();

		info.MoneyRate = monster.attribute("MoneyRate").as_int();

		info.MaxItemLevel = monster.attribute("MaxItemLevel").as_int();

		info.MonsterSkill = monster.attribute("MonsterSkill").as_int();

		info.Resistance[0] = monster.attribute("IceRes").as_int();

		info.Resistance[1] = monster.attribute("PoisonRes").as_int();

		info.Resistance[2] = monster.attribute("LightRes").as_int();

		info.Resistance[3] = monster.attribute("FireRes").as_int();

		this->SetInfo(info);
	}

	this->InitMonsterItem();
}


void CMonsterManager::LoadTXT(const std::string& txt_path, const std::string& xml_path) // OK
{
	CMemScript* lpMemScript = new CMemScript;

	if(lpMemScript == 0)
	{
		ErrorMessageBox(MEM_SCRIPT_ALLOC_ERROR, (txt_path + ".txt").c_str());
		return;
	}

	if(lpMemScript->SetBuffer(const_cast<char*>((txt_path + ".txt").c_str())) == 0)
	{
		ErrorMessageBox(lpMemScript->GetLastError());
		delete lpMemScript;
		return;
	}

	this->Init();

	pugi::xml_document doc;
	pugi::xml_node declarationNode = doc.append_child(pugi::node_declaration);
	declarationNode.append_attribute("version") = "1.0";
	declarationNode.append_attribute("encoding") = "utf-8";

	doc.append_child(pugi::node_pcdata).set_value(mm_info_text.c_str());
	doc.append_child(pugi::node_pcdata).set_value("");

	pugi::xml_node root = doc.append_child("MonsterList");

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

			MONSTER_INFO info;

			memset(&info,0,sizeof(info));
			auto monster = root.append_child("Monster");

			info.Index = lpMemScript->GetNumber();
			monster.append_attribute("Index") = info.Index;

			info.Rate = lpMemScript->GetAsNumber();
			//monster.append_attribute("Rate") = info.Rate;

			strcpy_s(info.Name,lpMemScript->GetAsString());
			// moved below
			
			info.Level = lpMemScript->GetAsNumber();
			monster.append_attribute("Level") = info.Level;

			info.Life = lpMemScript->GetAsNumber();
			monster.append_attribute("Life") = info.Life;

			info.Mana = lpMemScript->GetAsNumber();
			monster.append_attribute("Mana") = info.Mana;

			info.DamageMin = lpMemScript->GetAsNumber();
			monster.append_attribute("DamageMin") = info.DamageMin;

			info.DamageMax = lpMemScript->GetAsNumber();
			monster.append_attribute("DamageMax") = info.DamageMax;

			info.Defense = lpMemScript->GetAsNumber();
			monster.append_attribute("Defense") = info.Defense;

			info.MagicDefense = lpMemScript->GetAsNumber();
			monster.append_attribute("MagicDefense") = info.MagicDefense;

			info.AttackRate = lpMemScript->GetAsNumber();
			monster.append_attribute("AttackRate") = info.AttackRate;

			info.DefenseRate = lpMemScript->GetAsNumber();
			monster.append_attribute("DefenseRate") = info.DefenseRate;

			info.MoveRange = lpMemScript->GetAsNumber();
			monster.append_attribute("MoveRange") = info.MoveRange;

			info.AttackType = lpMemScript->GetAsNumber();
			monster.append_attribute("AttackType") = info.AttackType;

			info.AttackRange = lpMemScript->GetAsNumber();
			monster.append_attribute("AttackRange") = info.AttackRange;

			info.ViewRange = lpMemScript->GetAsNumber();
			monster.append_attribute("ViewRange") = info.ViewRange;

			info.MoveSpeed = lpMemScript->GetAsNumber();
			monster.append_attribute("MoveSpeed") = info.MoveSpeed;

			info.AttackSpeed = lpMemScript->GetAsNumber();
			monster.append_attribute("AttackSpeed") = info.AttackSpeed;

			info.RegenTime = lpMemScript->GetAsNumber();
			monster.append_attribute("RegenTime") = info.RegenTime;

			info.Attribute = lpMemScript->GetAsNumber();
			monster.append_attribute("Attribute") = info.Attribute;

			info.ItemRate = lpMemScript->GetAsNumber();
			monster.append_attribute("ItemRate") = info.ItemRate;

			info.MoneyRate = lpMemScript->GetAsNumber();
			monster.append_attribute("MoneyRate") = info.MoneyRate;

			info.MaxItemLevel = lpMemScript->GetAsNumber();
			monster.append_attribute("MaxItemLevel") = info.MaxItemLevel;

			info.MonsterSkill = lpMemScript->GetAsNumber();
			monster.append_attribute("MonsterSkill") = info.MonsterSkill;

			info.Resistance[0] = lpMemScript->GetAsNumber();
			monster.append_attribute("IceRes") = info.Resistance[0];

			info.Resistance[1] = lpMemScript->GetAsNumber();
			monster.append_attribute("PoisonRes") = info.Resistance[1];

			info.Resistance[2] = lpMemScript->GetAsNumber();
			monster.append_attribute("LightRes") = info.Resistance[2];

			info.Resistance[3] = lpMemScript->GetAsNumber();
			monster.append_attribute("FireRes") = info.Resistance[3];

			//
			monster.append_attribute("Name") = info.Name;

			#if(GAMESERVER_UPDATE>=701)

			info.ElementalAttribute = lpMemScript->GetAsNumber();

			info.ElementalPattern = lpMemScript->GetAsNumber();

			info.ElementalDefense = lpMemScript->GetAsNumber();

			info.ElementalDamageMin = lpMemScript->GetAsNumber();

			info.ElementalDamageMax = lpMemScript->GetAsNumber();

			info.ElementalAttackRate = lpMemScript->GetAsNumber();

			info.ElementalDefenseRate = lpMemScript->GetAsNumber();

			#endif

			//this->SetInfo(info);
		}
	}
	catch(...)
	{
		ErrorMessageBox(lpMemScript->GetLastError());
	}


	if (std::filesystem::exists(xml_path + ".xml"))
		std::filesystem::rename(xml_path + ".xml", xml_path + ".bak");

	doc.save_file((xml_path + ".xml").c_str());

	std::filesystem::rename(txt_path + ".txt", txt_path + ".bak");

	MsgBox("[%s] convert complete.\nCheck new file in \"Convert\\Generated\" folder.\nPlease, reboot gameserver", (txt_path + ".txt").c_str());

	//doc.save_file(xml_path);

	//this->InitMonsterItem();

	delete lpMemScript;
}

void CMonsterManager::SetInfo(MONSTER_INFO info) // OK
{
	if(info.Index < 0 || info.Index >= MAX_MONSTER_INFO)
	{
		return;
	}

	info.Life = ((__int64)info.Life*gServerInfo.m_MonsterLifeRate)/100;

	info.ScriptLife = info.Life;

	info.DamageMin = ((__int64)info.DamageMin*gServerInfo.m_MonsterDamageRate)/100;

	info.DamageMax = ((__int64)info.DamageMax*gServerInfo.m_MonsterDamageRate)/100;

	info.Defense = ((__int64)info.Defense*gServerInfo.m_MonsterDefenseRate)/100;

	this->m_MonsterInfo[info.Index] = info;
}

MONSTER_INFO* CMonsterManager::GetInfo(int index) // OK
{
	if(index < 0 || index >= MAX_MONSTER_INFO)
	{
		return 0;
	}

	if(this->m_MonsterInfo[index].Index != index)
	{
		return 0;
	}

	return &this->m_MonsterInfo[index];
}

char* CMonsterManager::get_name(int index)
{
	auto info = this->GetInfo(index);
	if (!info)
		return "Unknown index";

	return info->Name;
}


void CMonsterManager::InitMonsterItem() // OK
{
	for(int n=0;n < MAX_MONSTER_LEVEL;n++)
	{
		this->InsertMonsterItem(n);
	}
}

void CMonsterManager::InsertMonsterItem(int level) // OK
{
	for(int n=0;n < MAX_ITEM;n++)
	{
		ITEM_INFO ItemInfo;

		if(gItemManager.GetInfo(n,&ItemInfo) == 0)
		{
			continue;
		}

		if(ItemInfo.Level < 0 || ItemInfo.DropItem == 0)
		{
			continue;
		}

		if((ItemInfo.Level+4) >= level && (ItemInfo.Level-2) <= level)
		{
			LogAdd(LOG_TEST, "MonsterLvl: %d, item: %d", level, ItemInfo.Level);
			MONSTER_ITEM_INFO* lpItem = &this->m_MonsterItemInfo[level];

			if(lpItem->IndexCount >= 0 && lpItem->IndexCount < MAX_MONSTER_ITEM)
			{
				lpItem->IndexTable[lpItem->IndexCount++] = n;
			}
		}
	}
}

long CMonsterManager::GetMonsterItem(int level,int excellent,int socket) // OK
{
	if(level < 0 || level >= MAX_MONSTER_LEVEL)
	{
		return -1;
	}

	if(this->m_MonsterItemInfo[level].IndexCount == 0)
	{
		return -1;
	}

	int IndexTable[MAX_MONSTER_ITEM];
	int IndexCount = 0;

	for(int n=0;n < this->m_MonsterItemInfo[level].IndexCount;n++)
	{
		if(excellent != 0 && this->m_MonsterItemInfo[level].IndexTable[n] >= GET_ITEM(12,0))
		{
			continue;
		}

		if(excellent != 0 && g380ItemType.Check380ItemType(this->m_MonsterItemInfo[level].IndexTable[n]) != 0)
		{
			continue;
		}

		if((excellent != 0 || socket == 0) && gSocketItemType.CheckSocketItemType(this->m_MonsterItemInfo[level].IndexTable[n]) != 0)
		{
			continue;
		}

		IndexTable[IndexCount++] = this->m_MonsterItemInfo[level].IndexTable[n];
	}

	return ((IndexCount==0)?-1:IndexTable[GetLargeRand()%IndexCount]);
}

void CMonsterManager::SetMonsterData() // OK
{
	for(int n=0;n < gMonsterSetBase.m_count;n++)
	{
		MONSTER_SET_BASE_INFO* lpInfo = &gMonsterSetBase.m_MonsterSetBaseInfo[n];

		if(lpInfo->Type == 3 || lpInfo->Type == 4)
		{
			continue;
		}

		int index = gObjAddMonster(lpInfo->Map);

		if(OBJECT_RANGE(index) == 0)
		{
			continue;
		}

		if(gObjSetPosMonster(index,n) == 0)
		{
			gObjDel(index);
			continue;
		}

		if(gObjSetMonster(index,lpInfo->MonsterClass) == 0)
		{
			gObjDel(index);
			continue;
		}

		#if(GAMESERVER_TYPE==1)

		LPOBJ lpObj = &gObj[index];

		if(lpObj->Map == MAP_CASTLE_SIEGE)
		{
			if(lpObj->Class == 216)
			{
				gCastleSiege.SetCrownIndex(index);
			}
		}

		if(lpObj->Map == MAP_CRYWOLF)
		{
			if(lpObj->Type == OBJECT_MONSTER)
			{
				if(gCrywolf.m_ObjCommonMonster.AddObj(index) == 0)
				{
					gObjDel(index);
					continue;
				}
			}

			if(lpObj->Type == OBJECT_NPC)
			{
				if(lpObj->Class >= 204 && lpObj->Class <= 209)
				{
					if(gCrywolf.m_ObjSpecialNPC.AddObj(index) == 0)
					{
						gObjDel(index);
						continue;
					}
				}
				else
				{
					if(lpObj->Class != 406 && lpObj->Class != 407)
					{
						if(gCrywolf.m_ObjCommonNPC.AddObj(index) == 0)
						{
							gObjDel(index);
							continue;
						}
					}
				}
			}
		}

		#endif
	}

	gBloodCastle.Init();

	gBonusManager.Init();

	gCastleDeep.Init();

	gChaosCastle.Init();

	gCustomArena.Init();

	gCustomEventDrop.Init();

	gVipLottery.Init();

	gTvTEvent.Init();

	gDevilSquare.Init();

	gIllusionTemple.Init();

	gInvasionManager.Init();

	gMossMerchant.Init();

	gRaklion.SetState(RAKLION_STATE_END);
}
