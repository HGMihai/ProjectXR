#include "stdafx.h"
#include "QuestSystemEx.h"
#include "Protection.h"
#include "GameMain.h"
#include "CashShop.h"
#include "DSProtocol.h"
#include "MonsterManager.h"
#include "ItemManager.h"
#include "Path.h"
#include "Util.h"
#include "Message.h"
#include "Log.h"
#include "BankEx.h"
#include "MemScript.h"
// -------------------------------------------------------------------------

CQuestSystem gCustomQuest;
// -------------------------------------------------------------------------

CQuestSystem::CQuestSystem()
{
	this->Init();
}

CQuestSystem::~CQuestSystem()
= default;

void CQuestSystem::Init()
{
	if (gProtection.GetCustomState(CUSTOM_QUEST_SYSTEM) == 0)
	{
		return;
	}

	this->quest_list.clear();
	/*QUEST_DATA info;
	this->quest_list.push_back(info);*/
}
// -------------------------------------------------------------------------


void CQuestSystem::Load()
{
	if (gProtection.GetCustomState(CUSTOM_QUEST_SYSTEM) == 0)
	{
		return;
	}

	/*convert_data.enable = GetPrivateProfileInt("Common", "Enable", 0, gPath.GetFullPath("..\\LTPData\\QuestSystem\\QuestSystem.ini"));
	convert_data.AddForEachPartyMember = GetPrivateProfileInt("Common", "AddForEachPartyMember", 0, gPath.GetFullPath("..\\LTPData\\QuestSystem\\QuestSystem.ini"));
	convert_data.npc_id = GetPrivateProfileInt("NPC", "ID", -1, gPath.GetFullPath("..\\LTPData\\QuestSystem\\QuestSystem.ini"));
	convert_data.npc_map = GetPrivateProfileInt("NPC", "Map", -1, gPath.GetFullPath("..\\LTPData\\QuestSystem\\QuestSystem.ini"));
	convert_data.npc_x = GetPrivateProfileInt("NPC", "X", -1, gPath.GetFullPath("..\\LTPData\\QuestSystem\\QuestSystem.ini"));
	convert_data.npc_y = GetPrivateProfileInt("NPC", "Y", -1, gPath.GetFullPath("..\\LTPData\\QuestSystem\\QuestSystem.ini"));

	this->load_txt(gPath.GetFullPath("..\\LTPData\\QuestSystem\\QuestList.txt"));*/

	this->LoadFile(gPath.GetFullPath("..\\LTPData\\QuestSystem\\QuestList.xml"));
}

void CQuestSystem::load_txt(char* path)
{
	CMemScript* lpMemScript = new CMemScript;

	if (lpMemScript == nullptr)
	{
		ErrorMessageBox(MEM_SCRIPT_ALLOC_ERROR, path);
		return;
	}

	if (lpMemScript->SetBuffer(path) == 0)
	{
		ErrorMessageBox(lpMemScript->GetLastError());
		delete lpMemScript;
		return;
	}

	int q_Index = 0;
	int q_ItemCat;
	int q_ItemID;
	int q_ItemLvl;
	int	q_ItemEx;
	int	q_ItemAnc;
	int q_ItemOpt;
	int q_ItemCount;
	int q_wZen;
	int q_wStat;
	int q_wcoinc;
	int q_wcoinp;
	int q_goblin;
	char szHint[255];

	this->Init();
	this->convert_data.quest_list.clear();

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

			int q_Type = lpMemScript->GetNumber();
			int q_MobID = lpMemScript->GetAsNumber();
			int q_MobCnt = lpMemScript->GetAsNumber();

			if (q_Type == 0)
			{
				q_ItemCat = lpMemScript->GetAsNumber();
				// ----
				q_ItemID = lpMemScript->GetAsNumber();
				// ----
				q_ItemLvl = lpMemScript->GetAsNumber();
				// ----
				q_ItemEx = lpMemScript->GetAsNumber();
				// ----
				q_ItemAnc = lpMemScript->GetAsNumber();
				// ----
				q_ItemOpt = lpMemScript->GetAsNumber();
				// ----
				q_ItemCount = lpMemScript->GetAsNumber();
				// ----
				q_wStat = -1;
				q_wZen = -1;
				q_wcoinc = -1;
				q_wcoinp = -1;
				q_goblin = -1;
			}
			else
			{
				q_wStat = lpMemScript->GetAsNumber();
				// ----
				q_wZen = lpMemScript->GetAsNumber();
				// ----
				q_wcoinc = lpMemScript->GetAsNumber();
				// ----
				q_wcoinp = lpMemScript->GetAsNumber();
				// ----
				q_goblin = lpMemScript->GetAsNumber();
				// ----
				q_ItemCat = -1;
				q_ItemID = -1;
				q_ItemLvl = -1;
				q_ItemEx = -1;
				q_ItemAnc = -1;
				q_ItemOpt = -1;
				q_ItemCount = -1;
			}

			memcpy(szHint, lpMemScript->GetAsString(), sizeof(szHint));
			// ----
			//this->Insert(q_Index, q_Type, q_MobID, q_MobCnt, q_ItemCat, q_ItemID, q_ItemLvl, q_wZen, q_wStat, q_wcoinc, q_wcoinp, q_goblin);
			q_Index++;

			QUEST_DATA info{};

			//this->m_Quest[q_Index].q_Index = q_Index;
			info.type = q_Type;
			info.monster_id = q_MobID;
			info.monster_count = q_MobCnt;
			info.item_type = q_ItemCat;
			info.item_index = q_ItemID;
			info.item_level = q_ItemLvl;
			info.item_excl = q_ItemEx;
			info.item_anc = q_ItemAnc;
			info.item_opt = q_ItemOpt;
			info.item_count = q_ItemCount;
			info.zen = q_wZen * 1000000;
			info.level_up_points = q_wStat;
			info.wcoin_c = q_wcoinc;
			info.wcoin_p = q_wcoinp;
			info.wcoin_g = q_goblin;
			info.credits = 0;
			info.min_level = 0;
			info.min_reset = 0;
			info.min_greset = 0;
			memcpy(info.text, szHint, sizeof(info.text));

			//convert_data.quest_list.insert(std::pair<int, QUEST_DATA>(q_Index, info));
			convert_data.quest_list.push_back(info);
		}
	}
	catch (...)
	{
		ErrorMessageBox(lpMemScript->GetLastError());
	}

	delete lpMemScript;

	pugi::xml_document doc;
	pugi::xml_node declarationNode = doc.append_child(pugi::node_declaration);
	declarationNode.append_attribute("version") = "1.0";
	declarationNode.append_attribute("encoding") = "utf-8";

	pugi::xml_node root = doc.append_child("quest_system");

	auto settings = root.append_child("settings");

	settings.append_child("enable").text() = convert_data.enable;
	settings.append_child("npc_index").text() = convert_data.npc_id;
	settings.append_child("npc_map_index").text() = convert_data.npc_map;
	settings.append_child("npc_x").text() = convert_data.npc_x;
	settings.append_child("npc_y").text() = convert_data.npc_y;
	settings.append_child("add_for_each_party_member").text() = convert_data.AddForEachPartyMember;

	auto quest_list1 = root.append_child("quest_list");
	//MsgBox("size: %d", this->convert_data.quest_list.size());
	for (auto& it : this->convert_data.quest_list)
	{
		auto quest = quest_list1.append_child("quest");

		quest.append_attribute("type") = it.type;
		quest.append_attribute("min_level") = it.min_level;
		quest.append_attribute("min_reset") = it.min_reset;
		quest.append_attribute("min_greset") = it.min_greset;
		quest.append_attribute("monster_id") = it.monster_id;
		quest.append_attribute("monster_count") = it.monster_count;
		quest.append_attribute("text") = it.text;

		if(it.type == 0)
		{
			auto item = quest.append_child("item");

			item.append_attribute("item_type") = it.item_type;
			item.append_attribute("item_index") = it.item_index;
			item.append_attribute("level") = it.item_level;
			item.append_attribute("excellent") = it.item_excl;
			item.append_attribute("ancient") = it.item_anc;
			item.append_attribute("option") = it.item_opt;
			item.append_attribute("count") = it.item_count;
		}
		else if (it.type == 1)
		{
			auto currency = quest.append_child("currency");

			currency.append_attribute("level_up_points") = it.level_up_points;
			currency.append_attribute("zen") = it.zen;
			currency.append_attribute("wcoin_c") = it.wcoin_c;
			currency.append_attribute("wcoin_p") = it.wcoin_p;
			currency.append_attribute("wcoin_g") = it.wcoin_g;
			currency.append_attribute("credits") = it.credits;
		}

	}

	doc.save_file("quest.xml");
}


void CQuestSystem::LoadFile(char* path)
{
	pugi::xml_document file;
	pugi::xml_parse_result res = file.load_file(path);

	if (res.status != pugi::status_ok)
	{
		ErrorMessageBox("%s file load fail (%s)", path, res.description());
	}

	this->Init();

	auto quest_system = file.child("quest_system");

	this->enable = quest_system.child("settings").child("enable").text().as_int();
	this->npc_id = quest_system.child("settings").child("npc_index").text().as_int();
	this->npc_map = quest_system.child("settings").child("npc_map_index").text().as_int();
	this->npc_x = quest_system.child("settings").child("npc_x").text().as_int();
	this->npc_y = quest_system.child("settings").child("npc_y").text().as_int();
	this->AddForEachPartyMember = quest_system.child("settings").child("add_for_each_party_member").text().as_int();

	auto quest_node = quest_system.child("quest_list");

	int index = 1;

	for (auto quest = quest_node.child("quest"); quest; quest = quest.next_sibling())
	{
		QUEST_DATA info {};

		info.type = quest.attribute("type").as_int(0);
		info.min_level = quest.attribute("min_level").as_int(0);
		info.min_reset = quest.attribute("min_reset").as_int(0);
		info.min_greset = quest.attribute("min_greset").as_int(0);
		info.monster_id = quest.attribute("monster_id").as_int(0);
		info.monster_count = quest.attribute("monster_count").as_int(0);
		strcpy_s(info.text, quest.attribute("text").as_string(""));

		if(info.type == 0)
		{
			auto item_reward = quest.child("item");
			info.item_type = item_reward.attribute("item_type").as_int(0);
			info.item_index = item_reward.attribute("item_index").as_int(0);
			info.item_level = item_reward.attribute("level").as_int(0);
			info.item_excl = item_reward.attribute("excellent").as_int(0);
			info.item_anc = item_reward.attribute("ancient").as_int(0);
			info.item_opt = item_reward.attribute("option").as_int(0);
			info.item_dur = item_reward.attribute("durability").as_int(0);
			info.item_count = item_reward.attribute("count").as_int(0);
		}
		else
		{
			auto currency_reward = quest.child("currency");
			info.level_up_points = currency_reward.attribute("level_up_points").as_int(0);
			info.zen = currency_reward.attribute("zen").as_int(0);
			info.wcoin_c = currency_reward.attribute("wcoin_c").as_int(0);
			info.wcoin_p = currency_reward.attribute("wcoin_p").as_int(0);
			info.wcoin_g = currency_reward.attribute("wcoin_g").as_int(0);
			info.credits = currency_reward.attribute("credits").as_int(0);
		}

		this->quest_list.insert(std::pair(index,info));
		index++;
	}
}

bool CQuestSystem::QuestExist(int index)
{
	if (this->quest_list.find(index) == this->quest_list.end())
		return false;
	
	return true;
}
// -------------------------------------------------------------------------

QUEST_DATA* CQuestSystem::GetQuest(int index)
{
	auto it = this->quest_list.find(index);
	if (it == this->quest_list.end())
		return nullptr;

	return &it->second;
}


bool CQuestSystem::Dialog(LPOBJ lpObj, LPOBJ lpNpc)
{
	if (gProtection.GetCustomState(CUSTOM_QUEST_SYSTEM) == 0)
		return false;

	if (lpNpc->Class == this->npc_id && lpNpc->Map == this->npc_map && lpNpc->X == this->npc_x && lpNpc->Y == this->npc_y )
	{
		this->NpcTalk(lpObj);
		return true;
	}

	if(this->npc_map == -1 && this->npc_id == lpNpc->Class)
	{
		this->NpcTalk(lpObj);
		return true;
	}

	return false;
}
// -------------------------------------------------------------------------

void CQuestSystem::NpcTalk(LPOBJ lpObj)
{
	if (!gObjIsConnectedGP(lpObj->Index))
		return;

	bool state1 = this->QuestExist(lpObj->iQuestIndex + 1);
	bool state2 = this->QuestExist(lpObj->iQuestIndex);
	
	if((lpObj->iQuestIndex == 0 && !state1) ||
		(lpObj->iQuestIndex > 0 && !state2))
	{
		CQuestSystem::send_message(lpObj->Index, 0, "@[Quest] I have no more quests for you!");
		CQuestSystem::send_message(lpObj->Index, 0, "@[Quest] Please, come back later!");
		this->gc_send_quest_info(lpObj->Index, false);
		return;
	}

	if(lpObj->iQuestIndex == 0)
	{
		auto quest = this->GetQuest(1);

		if(quest == nullptr)
		{
			LogAdd(LOG_RED, "[QuestSystem] Quest with index (%d) not found.", 1);
			return;
		}
		
		if(lpObj->iQuestState != 0)
		{
			CQuestSystem::send_message(lpObj->Index, 0, "@[Quest] Wrong quest state! Expected 0, you have %d", lpObj->iQuestState);
			CQuestSystem::send_message(lpObj->Index, 0, "@[Quest] Contact server Administrator!");
			return;
		}

		MONSTER_INFO* lpMA = gMonsterManager.GetInfo(quest->monster_id);
		// ----
		if (lpMA == nullptr)
			return;
		// ----
		CQuestSystem::send_message(lpObj->Index, 0, "@[Quest] Quest #%d started!", lpObj->iQuestIndex + 1);
		CQuestSystem::send_message(lpObj->Index, 0, "@[Quest] You have to kill %d %s's!", quest->monster_count, lpMA->Name);

		if(strlen(quest->text))
			CQuestSystem::send_message(lpObj->Index, 0, "@[Quest] %s", quest->text);
		
		// ----
		lpObj->iQuestIndex += 1;
		lpObj->iQuestState = 1;
		lpObj->iQuestVar = 0;
		this->gc_send_quest_info(lpObj->Index);
	}
	else
	{
		auto quest = this->GetQuest(lpObj->iQuestIndex);

		if (quest == nullptr)
		{
			LogAdd(LOG_RED, "[QuestSystem] Quest with index (%d) not found.", lpObj->iQuestIndex);
			return;
		}
		
		MONSTER_INFO* lpMA = gMonsterManager.GetInfo(quest->monster_id);
		// ----
		if (lpMA == nullptr)
			return;
		
		if(lpObj->iQuestState == 0)
		{
			CQuestSystem::send_message(lpObj->Index, 0, "@[Quest] Quest #%d started!", lpObj->iQuestIndex);
			CQuestSystem::send_message(lpObj->Index, 0, "@[Quest] You have to kill %d %s's!", quest->monster_count, lpMA->Name);

			if (strlen(quest->text))
				CQuestSystem::send_message(lpObj->Index, 0, "@[Quest] %s", quest->text);
			// ----
			lpObj->iQuestState = 1;
			lpObj->iQuestVar = 0;
			this->gc_send_quest_info(lpObj->Index);
		}
		else if(lpObj->iQuestState == 1)
		{
			CQuestSystem::send_message(lpObj->Index, 0, "@[Quest] You haven't finished the Quest #%d!", lpObj->iQuestIndex);
			CQuestSystem::send_message(lpObj->Index, 0, "@[Quest] Killed %s's [%d/%d]!", lpMA->Name, lpObj->iQuestVar, quest->monster_count);
			
			if (strlen(quest->text))
				CQuestSystem::send_message(lpObj->Index, 0, "@[Quest] %s", quest->text);

			this->gc_send_quest_info(lpObj->Index);
		}
		else
		{
			this->make_reward(lpObj);
		}
	}
}

void CQuestSystem::connect(LPOBJ lpObj)
{
	if (this->enable && gProtection.GetCustomState(CUSTOM_QUEST_SYSTEM))
	{
		//Custom Quest
		if (lpObj->iQuestIndex > 0)
		{
			auto quest = this->GetQuest(lpObj->iQuestIndex);

			if (quest == nullptr)
			{
				LogAdd(LOG_RED, "[QuestSystem] Quest with index (%d) not found.", lpObj->iQuestIndex);
				return;
			}

			if (lpObj->iQuestState == 1 || lpObj->iQuestState == 2)
			{
				if (lpObj->iQuestVar == 0)
				{
					MONSTER_INFO* lpMA = gMonsterManager.GetInfo(quest->monster_id);

					if (lpMA)
					{
						CQuestSystem::send_message(lpObj->Index, 0, "@[Quest] Quest #%d in progress!", lpObj->iQuestIndex);
						CQuestSystem::send_message(lpObj->Index, 0, "@[Quest] You have to kill %d %s's!", quest->monster_count, lpMA->Name);

						if (strlen(quest->text))
							CQuestSystem::send_message(lpObj->Index, 0, "@[Quest] %s", quest->text);

						this->gc_send_quest_info(lpObj->Index);
					}
				}
				else if (lpObj->iQuestVar < quest->monster_count)
				{
					MONSTER_INFO* lpMA = gMonsterManager.GetInfo(quest->monster_id);

					if (lpMA)
					{
						CQuestSystem::send_message(lpObj->Index, 0, "@[Quest] Quest #%d in progress!", lpObj->iQuestIndex);
						CQuestSystem::send_message(lpObj->Index, 0, "@[Quest] Killed %s's [%d/%d]!", lpMA->Name, lpObj->iQuestVar, quest->monster_count);

						this->gc_send_quest_info(lpObj->Index);
					}
				}
				else
				{
					CQuestSystem::send_message(lpObj->Index, 0, "@[Quest] You have successfully finished Quest #%d!", lpObj->iQuestIndex);
					CQuestSystem::send_message(lpObj->Index, 0, "@[Quest] To get your prize talk to the Quest Master!");

					this->gc_send_quest_info(lpObj->Index);
				}
			}
			else
			{
				if (quest != nullptr)
				{
					CQuestSystem::send_message(lpObj->Index, 0, "@[Quest] There are no active quests!");
					CQuestSystem::send_message(lpObj->Index, 0, "@[Quest] Please, talk to the Quest Master to get one!");
				}
				else
				{
					CQuestSystem::send_message(lpObj->Index, 0, "@[Quest] There are no active quests!");
					CQuestSystem::send_message(lpObj->Index, 0, "@[Quest] There are no more quest for you!");
				}
			}
		}
		else if (lpObj->iQuestIndex == 0)
		{
			CQuestSystem::send_message(lpObj->Index, 0, "@[Quest] You haven't started quest's yet!");
			this->gc_send_quest_info(lpObj->Index);
		}
	}
}

void CQuestSystem::make_reward(LPOBJ lpObj)
{
	auto quest = this->GetQuest(lpObj->iQuestIndex);

	if (quest == nullptr)
	{
		LogAdd(LOG_RED, "[QuestSystem] Quest with index (%d) not found.", lpObj->iQuestIndex);
		return;
	}
	
	if (lpObj->iQuestVar >= quest->monster_count)
	{
		if (quest->type == 0)
		{
			if (!this->MakeItem(lpObj, lpObj->iQuestIndex))
				return;
		}
		else
		{
			if (quest->wcoin_c > 0)
			{
				CQuestSystem::send_message(lpObj->Index, 0, "@[Quest] You have received %d %s!", quest->wcoin_c, gMessage.GetMessage(524));
				gCashShop.GDCashShopAddPointSaveSend(lpObj->Index, nullptr, quest->wcoin_c, 0, 0);
			}
			
			if (quest->wcoin_p > 0)
			{
				CQuestSystem::send_message(lpObj->Index, 0, "@[Quest] You have received %d %s!", quest->wcoin_p, gMessage.GetMessage(525));
				gCashShop.GDCashShopAddPointSaveSend(lpObj->Index, nullptr, 0, quest->wcoin_p, 0);
			}
			
			if (quest->wcoin_g > 0)
			{
				CQuestSystem::send_message(lpObj->Index, 0, "@[Quest] You have received %d %s!", quest->wcoin_g, gMessage.GetMessage(526));
				gCashShop.GDCashShopAddPointSaveSend(lpObj->Index, nullptr, 0, 0, quest->wcoin_g);
			}

			if(quest->credits > 0)
			{
				lpObj->BankEx.Credits += quest->credits;
				gBankEx.GCUpdateBankEx(lpObj->Index);
				gBankEx.GDSavePoint(lpObj->Index);
				CQuestSystem::send_message(lpObj->Index, 0, "@[Quest] You have received %d Credits!", quest->credits);
			}
			
			if (quest->level_up_points > 0)
			{
				CQuestSystem::send_message(lpObj->Index, 0, "@[Quest] You have received %d Stat Points!", quest->level_up_points);
				lpObj->iQuestStat += quest->level_up_points;
				lpObj->LevelUpPoint += quest->level_up_points;

				gLog.Output(LOG_SUPER_POINTS, "[Quest System] [%d] [%d] Obtained %d points (total %d). QuestIndex: %d", lpObj->Account, lpObj->Name, quest->level_up_points, lpObj->iQuestStat, lpObj->iQuestIndex);
			}
			
			if (quest->zen > 0)
			{
				CQuestSystem::send_message(lpObj->Index, 0, "@[Quest] You have received %d %s!", quest->zen, gMessage.GetMessage(523));

				unsigned long addZen = lpObj->Money + quest->zen;

				if (addZen > 2000000000)
				{
					addZen = 2000000000;
					CQuestSystem::send_message(lpObj->Index, 0, "@[Quest] You have reached the maximum amount of %s!", gMessage.GetMessage(523));
				}
				
				lpObj->Money = addZen;
				GCMoneySend(lpObj->Index, lpObj->Money);
			}

			GCFireworksSend(lpObj, lpObj->X, lpObj->Y);

			GCNewCharacterInfoSend(lpObj);

			GDCharacterInfoSaveSend(lpObj->Index);
			// ----
			CQuestSystem::send_message(lpObj->Index, 0, "@[Quest] Congratulations!");
		}
		// ----
		lpObj->iQuestIndex++;
		lpObj->iQuestState = 0;
		lpObj->iQuestVar = 0;
		this->gc_send_quest_info(lpObj->Index, false);
	}
}

void CQuestSystem::MonsterKill(LPOBJ lpObj, LPOBJ lpTarget)
{
	if (gProtection.GetCustomState(CUSTOM_QUEST_SYSTEM) == 0)
	{
		return;
	}

	if (this->enable)
	{
		if (lpObj->iQuestState == 1 || lpObj->iQuestState == 2)
		{
			if (this->QuestExist(lpObj->iQuestIndex))
			{
				auto quest = this->GetQuest(lpObj->iQuestIndex);

				if (quest == nullptr)
				{
					LogAdd(LOG_RED, "[QuestSystem] Quest with index (%d) not found.", lpObj->iQuestIndex);
					return;
				}
				
				if (lpTarget->Class == quest->monster_id)
				{
					if (lpObj->iQuestVar >= quest->monster_count)
					{
						lpObj->iQuestState = 2;
					}
					else
					{
						lpObj->iQuestVar++;

						MONSTER_INFO* lpMA = gMonsterManager.GetInfo(quest->monster_id);

						if (lpMA != nullptr)
						{
							this->gc_send_quest_info(lpObj->Index);
							if (lpObj->iQuestVar == quest->monster_count)
							{
								GCFireworksSend(lpObj, lpObj->X, lpObj->Y);
							}
						}

						if (lpObj->iQuestVar >= quest->monster_count)
						{
							lpObj->iQuestState = 2;
						}
					}
				}
			}
		}
	}
}
// -------------------------------------------------------------------------

void CQuestSystem::send_message(int aIndex, int type, char* szMsg, ...)
{
	char szTemp[89];
	// ----
	va_list pArguments;
	va_start(pArguments, szMsg);
	vsprintf_s(szTemp, szMsg, pArguments); 
	va_end(pArguments);
	// ----
	PMSG_CHAT_WHISPER_RECV pWhisper;
	// ----
	if (type == 0)
	{
		pWhisper.header.type = 0xC1;
		pWhisper.header.size = sizeof(pWhisper);
		pWhisper.header.head = 0x00;
	}
	/*else if (type == 1)
	{
		GCServerMsgStringSend(szTemp, aIndex, 0x01);
		return;
	}*/
	else
	{
		pWhisper.header.type = 0xC1;
		pWhisper.header.size = sizeof(pWhisper);
		pWhisper.header.head = 0x02;
	}
	// ----
	strcpy_s(pWhisper.name, "");
	memcpy(pWhisper.message, szTemp, sizeof(szTemp));
	int len = strlen(pWhisper.message);
	// ----
	if (len > 0)
	{
		pWhisper.header.size -= sizeof(pWhisper.message);
		pWhisper.header.size += len + 1;
		// ----
		DataSend(aIndex, reinterpret_cast<LPBYTE>(&pWhisper), pWhisper.header.size);
	}
}
// -------------------------------------------------------------------------

void CQuestSystem::MonsterBarAdd(int aIndex, int MonsterID)
{
	//PMSG_MONSTERBAR_SEND pMsg;

	/*pMsg.h.set(0xFB,0x0C, sizeof(pMsg));

	pMsg.MonsterID = MonsterID;
	strcpy_s(pMsg.Text,"THIS IS TEST");
	pMsg.Red  = 91;
	pMsg.Green  = 133;
	pMsg.Blue  = 129;
	pMsg.Opacity = 0.6;

	DataSend(aIndex, (LPBYTE)&pMsg, sizeof(pMsg));*/
}

bool CQuestSystem::MakeItem(LPOBJ lpObj, int QuestIndex)
{
	auto quest = this->GetQuest(lpObj->iQuestIndex);

	if (quest == nullptr)
	{
		LogAdd(LOG_RED, "[QuestSystem] Quest with index (%d) not found.", lpObj->iQuestIndex);
		return false;
	}

	int x = 0;

	ITEM_INFO item;

	if (!gItemManager.GetInfo(GET_ITEM(quest->item_type, quest->item_index), &item))
	{
		this->send_message(lpObj->Index, 0, "@[Quest] Item with id %d don't exist", GET_ITEM(quest->item_type, quest->item_index));
		return false;
	}

	int y = item.Height;

	int temp_x = 0;

	for (int i = 0; i < quest->item_count; i++)
	{
		temp_x += item.Width;

		if(temp_x > 8)
		{
			temp_x -= 8;
			x = 8;
			y += item.Height;
		}
	}

	if (x == 0)
		x = temp_x;

	if (!gItemManager.CheckItemInventorySpace(lpObj, x, y))
	{
		this->send_message(lpObj->Index, 0, "@[Quest] Need %dx%d cells in inventory", x, y);
		return false;
	}

	this->send_message(lpObj->Index, 0, "@[Quest] Congratulations, here is your prize!");

	const int dur = quest->item_dur;

	if (dur == 0)
		gItemManager.GetItemDurability(GET_ITEM(quest->item_type, quest->item_index), quest->item_level, quest->item_excl, quest->item_anc);
	
	for (int i=0; i < quest->item_count; i++)
		GDCreateItemSend(lpObj->Index,0xEB,lpObj->X,lpObj->Y,GET_ITEM(quest->item_type, quest->item_index),
			quest->item_level,dur,0,0,quest->item_opt,lpObj->Index, quest->item_excl,quest->item_anc,0,0,nullptr,0xFF,0);

	GCFireworksSend(lpObj, lpObj->X, lpObj->Y);

	return true;
}

void CQuestSystem::gc_send_quest_info(int aIndex,bool state)
{
	PMSG_QUEST_EX_INFO_SEND pMsg;

	LPOBJ lpObj = &gObj[aIndex];

	pMsg.h.set(0xFB,0x96,sizeof(pMsg));

	auto quest = this->GetQuest(lpObj->iQuestIndex);

	if(quest == nullptr)
	{
		pMsg.info.max_kill_count = 0;
		pMsg.info.monster_index = 0;
	}
	else
	{
		pMsg.info.max_kill_count = quest->monster_count;
		pMsg.info.monster_index = quest->monster_id;
	}

	pMsg.info.kill_count = lpObj->iQuestVar;
	pMsg.info.quest_index = lpObj->iQuestIndex;
	pMsg.info.map_number = 0;
	pMsg.state = state;
	
	DataSend(aIndex,reinterpret_cast<BYTE*>(&pMsg),sizeof(pMsg));
}
