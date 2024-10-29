#pragma once
#include "Protocol.h"
#include "User.h"

struct PMSG_MAP_QUEST_SYSTEM_REQUEST
{
	PSBMSG_HEAD	header;
	int selected_map;
	int selected_quest;
	int type;
};

struct MONSTER_REQUIREMENTS
{
	int index;
	int count;
};

struct ITEM_REQUIREMENTS
{
	int index;
	int level;
	int option;
	int excl;
	int anc;
	int skill;
	int luck;
	int count;
};

struct CURRENCY_REWARD
{
	bool is_set;
	int exp;
	int points;
	int zen;
	int wcoin_c;
	int wcoin_p;
	int wcoin_g;
	int credits;
};

struct ITEM_REWARD
{
	bool is_set;
	int index;
	int level;
	int	excl;
	int	anc;
	int opt;
	int dur;
	int skill;
	int luck;
	int count;
};

struct ITEM_BAG_REWARD
{
	bool is_set;
	int index;
	char text[255];
};

struct PMSG_MAP_QUEST_SYSTEM_LIST_SEND
{
	PSWMSG_HEAD header;
	BYTE count;
	int MapQuestSystemMap;
	int MapQuestSystemIndex;
	BYTE MapQuestSystemState;
};

struct PMSG_MAP_QUEST_SYSTEM_MAP_LIST
{
	int map_index;
	int quest_count;
};

struct MAP_QUEST_SYSTEM_QUEST_LIST
{
	int quest_index;
	char text[255];
};

struct MAP_QUEST_SYSTEM_QUEST_INFO_SEND
{
	PSWMSG_HEAD header;
	BYTE req_item_count;
	BYTE req_monster_count;
	CURRENCY_REWARD currency_reward;
	ITEM_REWARD item_reward;
	ITEM_BAG_REWARD item_bag_reward;
};

struct MAP_QUEST_DATA
{
	MAP_QUEST_DATA(): index(false), min_level(false), min_reset(false), min_greset(false), item_bag_reward(), text{""}
	{
		currency_reward.is_set = false;
		item_reward.is_set = false;
	}

	int index;
	int min_level;
	int min_reset;
	int min_greset;

	std::vector<MONSTER_REQUIREMENTS> monster_req;
	std::vector<ITEM_REQUIREMENTS> item_req;
	CURRENCY_REWARD currency_reward;
	ITEM_REWARD item_reward;
	ITEM_BAG_REWARD item_bag_reward;

	char text[255];
};

class CMapQuestSystem
{
public:
	CMapQuestSystem();
	void load(char* path);
	void insert(int map_index, MAP_QUEST_DATA* info);
	bool dialog(LPOBJ lpObj, LPOBJ lpNpc);
	void send_maplist(int aIndex);
	void send_quests(int aIndex, int map_index);
	void send_quest_info(int aIndex, int map_index, int quest_index);
	void receive_request(PMSG_MAP_QUEST_SYSTEM_REQUEST* lpMsg, int aIndex);
private:
	int enable_;
	int npc_id_;
	int npc_map_;
	int npc_x_;
	int npc_y_;
	// ----
	std::map<int, std::map<int, MAP_QUEST_DATA>> quest_data_;
}; extern CMapQuestSystem gMapQuestSystem;