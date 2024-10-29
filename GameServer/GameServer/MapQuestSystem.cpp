#include "stdafx.h"
#include "MapQuestSystem.h"
#include "ItemManager.h"
#include "Protection.h"
#include "Util.h"

CMapQuestSystem gMapQuestSystem;

CMapQuestSystem::CMapQuestSystem(): enable_(0), npc_id_(0), npc_map_(0), npc_x_(0), npc_y_(0)
{
	this->quest_data_.clear();
}

void CMapQuestSystem::load(char* path)
{
	pugi::xml_document file;
	pugi::xml_parse_result res = file.load_file(path);

	if (res.status != pugi::status_ok)
	{
		ErrorMessageBox("%s file load fail (%s)", path, res.description());
	}

	this->quest_data_.clear();

	auto quest_system = file.child("quest_system");

	this->enable_ = quest_system.child("settings").child("enable").text().as_int();
	this->npc_id_ = quest_system.child("settings").child("npc_index").text().as_int();
	this->npc_map_ = quest_system.child("settings").child("npc_map_index").text().as_int();
	this->npc_x_ = quest_system.child("settings").child("npc_x").text().as_int();
	this->npc_y_ = quest_system.child("settings").child("npc_y").text().as_int();

	auto quest_node = quest_system.child("quest_list");

	for(auto map_node=quest_node.child("map"); map_node; map_node=map_node.next_sibling())
	{
		int map_index = map_node.attribute("id").as_int();

		for (auto quest = map_node.child("quest"); quest; quest = quest.next_sibling())
		{
			MAP_QUEST_DATA info{};

			info.item_req.clear();
			info.monster_req.clear();

			info.item_bag_reward.is_set = false;
			info.currency_reward.is_set = false;
			info.item_reward.is_set = false;

			info.index = quest.attribute("id").as_int();
			info.min_level = quest.attribute("min_level").as_int(0);
			info.min_reset = quest.attribute("min_reset").as_int(0);
			info.min_greset = quest.attribute("min_greset").as_int(0);
			strcpy_s(info.text, quest.attribute("text").as_string(""));

			auto req_node = quest.child("requirements");

			if(!req_node.empty())
			{
				for (auto req_monster = req_node.child("monster"); req_monster; req_monster = req_monster.next_sibling("monster"))
				{
					MONSTER_REQUIREMENTS monster;

					monster.index = req_monster.attribute("id").as_int();
					monster.count = req_monster.attribute("count").as_int();

					LogAdd(LOG_GREEN, "Load monster [map: %d, quest: %d] id: %d count: %d", map_index, info.index, monster.index, monster.count);


					info.monster_req.push_back(monster);
				}

				for (auto req_item = req_node.child("item"); req_item; req_item = req_item.next_sibling("item"))
				{
					ITEM_REQUIREMENTS item;

					item.index = GET_ITEM(req_item.attribute("type").as_int(),
						req_item.attribute("id").as_int());
					item.level = req_item.attribute("level").as_int();
					item.option = req_item.attribute("option").as_int();
					item.excl = req_item.attribute("excl").as_int();
					item.anc = req_item.attribute("anc").as_int();
					item.luck = req_item.attribute("luck").as_int();
					item.skill = req_item.attribute("skill").as_int();
					item.count = req_item.attribute("count").as_int();

					info.item_req.push_back(item);
				}
			}

			auto reward_node = quest.child("reward");

			if(!reward_node.empty())
			{
				auto reward_currency = reward_node.child("currency");

				if (!reward_currency.empty())
				{
					info.currency_reward.is_set = true;
					info.currency_reward.exp = reward_currency.attribute("exp").as_int();
					info.currency_reward.points = reward_currency.attribute("points").as_int();
					info.currency_reward.zen = reward_currency.attribute("zen").as_int();
					info.currency_reward.wcoin_c = reward_currency.attribute("wcoin_c").as_int();
					info.currency_reward.wcoin_p = reward_currency.attribute("wcoin_p").as_int();
					info.currency_reward.wcoin_g = reward_currency.attribute("wcoin_g").as_int();
					info.currency_reward.credits = reward_currency.attribute("credits").as_int();
				}

				auto reward_item = reward_node.child("item");

				if (!reward_item.empty())
				{
					info.item_reward.is_set = true;
					info.item_reward.index = GET_ITEM(reward_item.attribute("type").as_int(),
						reward_item.attribute("id").as_int());
					info.item_reward.excl = reward_item.attribute("excl").as_int();
					info.item_reward.anc = reward_item.attribute("anc").as_int();
					info.item_reward.opt = reward_item.attribute("opt").as_int();
					info.item_reward.dur = reward_item.attribute("dur").as_int();
					info.item_reward.skill = reward_item.attribute("skill").as_int();
					info.item_reward.luck = reward_item.attribute("luck").as_int();
					info.item_reward.count = reward_item.attribute("count").as_int();
				}
				
				auto reward_item_bag = reward_node.child("item_bag");

				if(!reward_item_bag.empty())
				{
					info.item_bag_reward.is_set = true;
					info.item_bag_reward.index = reward_item_bag.attribute("index").as_int();
					strcpy_s(info.item_bag_reward.text, reward_item_bag.attribute("text").as_string(""));
				}
			}

			this->insert(map_index, &info);
		}
	}
}

void CMapQuestSystem::insert(int map_index, MAP_QUEST_DATA* info)
{
	auto it = this->quest_data_.find(map_index);

	if(it == this->quest_data_.end())
	{
		std::map<int, MAP_QUEST_DATA> new_map;
		new_map.insert(std::make_pair(info->index, *info));
		this->quest_data_.insert(std::make_pair(map_index, new_map));
	}
	else
	{
		it->second.insert(std::make_pair(info->index, *info));
	}
}

bool CMapQuestSystem::dialog(LPOBJ lpObj, LPOBJ lpNpc)
{
	if (gProtection.GetCustomState(CUSTOM_QUEST_SYSTEM) == 0)
		return false;

	if (lpNpc->Class == this->npc_id_
		&& lpNpc->Map == this->npc_map_
		&& lpNpc->X == this->npc_x_
		&& lpNpc->Y == this->npc_y_)
	{
		this->send_maplist(lpObj->Index);
		return true;
	}
	return false;
}

void CMapQuestSystem::send_maplist(int aIndex)
{
	BYTE send[8192];

	PMSG_MAP_QUEST_SYSTEM_LIST_SEND pMsg{};

	pMsg.header.set(0xFB, 0x99, 0);

	int size = sizeof(pMsg);

	pMsg.count = 0;

	pMsg.MapQuestSystemMap = gObj[aIndex].MapQuestSystemMap;
	pMsg.MapQuestSystemIndex = gObj[aIndex].MapQuestSystemIndex;
	pMsg.MapQuestSystemState = gObj[aIndex].MapQuestSystemState;

	PMSG_MAP_QUEST_SYSTEM_MAP_LIST info{};

	for (auto& item : this->quest_data_)
	{
		info.map_index = item.first;
		info.quest_count = item.second.size();

		memcpy(&send[size], &info, sizeof(info));
		size += sizeof(info);

		pMsg.count++;
	}

	pMsg.header.size[0] = SET_NUMBERHB(size);

	pMsg.header.size[1] = SET_NUMBERLB(size);

	memcpy(send, &pMsg, sizeof(pMsg));
	DataSend(aIndex, send, size);
}

void CMapQuestSystem::send_quests(int aIndex, int map_index)
{
	BYTE send[8192];

	PMSG_MAP_QUEST_SYSTEM_LIST_SEND pMsg{};

	pMsg.header.set(0xFB, 0x9A, 0);

	int size = sizeof(pMsg);

	pMsg.count = 0;

	MAP_QUEST_SYSTEM_QUEST_LIST info{};

	auto map = this->quest_data_.find(map_index);

	if(map != this->quest_data_.end())
	{
		for (auto& item : map->second)
		{
			info.quest_index = item.first;
			strcpy_s(info.text, item.second.text);
			/*info.monster_index = -1;
			info.monster_count = -1;
			info.item_index = -1;
			info.item_count = -1;

			if (item.second.monster_req.is_set)
			{
				info.monster_index = item.second.monster_req.index;
				info.monster_count = item.second.monster_req.count;
			}

			if (item.second.item_req.is_set)
			{
				info.item_index = item.second.item_req.index;
				info.item_count = item.second.item_req.count;
				info.item_level = item.second.item_req.level;
			}*/

			memcpy(&send[size], &info, sizeof(info));
			size += sizeof(info);

			pMsg.count++;
		}
	}

	pMsg.header.size[0] = SET_NUMBERHB(size);

	pMsg.header.size[1] = SET_NUMBERLB(size);

	memcpy(send, &pMsg, sizeof(pMsg));
	DataSend(aIndex, send, size);
}

void CMapQuestSystem::send_quest_info(int aIndex, int map_index, int quest_index)
{
	BYTE send[8192];

	MAP_QUEST_SYSTEM_QUEST_INFO_SEND pMsg{};

	pMsg.header.set(0xFB, 0x9B, 0);

	int size = sizeof(pMsg);

	pMsg.req_item_count = 0;
	pMsg.req_monster_count = 0;

	auto map = this->quest_data_.find(map_index);

	if (map == this->quest_data_.end())
		return;

	auto quest = map->second.find(quest_index);

	if (quest == map->second.end())
		return;

	//LogAdd(LOG_BLUE, "item vector size: %d", quest->second.item_req.size());
	//LogAdd(LOG_BLUE, "monster vector size: %d", quest->second.monster_req.size());

	pMsg.currency_reward = quest->second.currency_reward;
	pMsg.item_bag_reward = quest->second.item_bag_reward;
	pMsg.item_reward = quest->second.item_reward;

	//for(auto it = quest->second.item_req.begin(); it != quest->second.item_req.end();++it)
	for (auto& item : quest->second.item_req)
	{
		memcpy(&send[size], &item, sizeof(item));
		size += sizeof(item);

		LogAdd(LOG_GREEN, "item size: %d", sizeof(item));

		pMsg.req_item_count++;
	}

	for (auto& item : quest->second.monster_req)
	{
		memcpy(&send[size], &item, sizeof(item));
		size += sizeof(item);

		LogAdd(LOG_GREEN, "monster size: %d", sizeof(item));

		pMsg.req_monster_count++;
	}

	//if (map != this->quest_data_.end())
	//{
	//	for (auto& item : map->second)
	//	{
	//		info.quest_index = item.first;
	//		strcpy_s(info.text, item.second.text);
	//		/*info.monster_index = -1;
	//		info.monster_count = -1;
	//		info.item_index = -1;
	//		info.item_count = -1;

	//		if (item.second.monster_req.is_set)
	//		{
	//			info.monster_index = item.second.monster_req.index;
	//			info.monster_count = item.second.monster_req.count;
	//		}

	//		if (item.second.item_req.is_set)
	//		{
	//			info.item_index = item.second.item_req.index;
	//			info.item_count = item.second.item_req.count;
	//			info.item_level = item.second.item_req.level;
	//		}*/

	//		memcpy(&send[size], &info, sizeof(info));
	//		size += sizeof(info);

	//		pMsg.count++;
	//	}
	//}

	pMsg.header.size[0] = SET_NUMBERHB(size);

	pMsg.header.size[1] = SET_NUMBERLB(size);

	memcpy(send, &pMsg, sizeof(pMsg));
	LogAdd(LOG_RED, "%d %d Size: %d", map_index, quest_index, size);
	DataSend(aIndex, send, size);
}

void CMapQuestSystem::receive_request(PMSG_MAP_QUEST_SYSTEM_REQUEST* lpMsg, int aIndex)
{
	if (lpMsg->type == 0)
		this->send_quests(aIndex, lpMsg->selected_map);
	else
		this->send_quest_info(aIndex, lpMsg->selected_map, lpMsg->selected_quest);

}
