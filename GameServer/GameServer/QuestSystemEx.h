#pragma once
// -------------------------------------------------------------------------
#include "Protocol.h"
#include "User.h"
// -------------------------------------------------------------------------

#define MAX_QUESTS 300
// -------------------------------------------------------------------------

struct QUEST_DATA
{
	int type;
	int min_level;
	int min_reset;
	int min_greset;
	int monster_id;
	int monster_count;
	int monster_map;
	int item_type;
	int item_index;
	int item_level;
	int	item_excl;
	int	item_anc;
	int item_opt;
	int item_dur;
	int item_count;
	int level_up_points;
	int zen;
	int wcoin_c;
	int wcoin_p;
	int wcoin_g;
	int credits;

	char text[255];
};

struct PMSG_MONSTERBAR_SEND
{
	PSBMSG_HEAD	h;
	// ----
	int MonsterID;
	char Text[25];
	BYTE Red;
	BYTE Green;
	BYTE Blue;
	float Opacity;
};
// -------------------------------------------------------------------------

struct QUEST_INFO_EX
{
	unsigned short quest_index;
	unsigned int kill_count;
	unsigned int max_kill_count;
	unsigned short monster_index;
	unsigned char map_number;
};

struct PMSG_QUEST_EX_INFO_SEND
{
	PSBMSG_HEAD	h;
	QUEST_INFO_EX info;
	bool state;
};

struct convert_quest
{
	int enable;
	int npc_id;
	int npc_map;
	int npc_x;
	int npc_y;
	int AddForEachPartyMember;

	std::vector<QUEST_DATA> quest_list;
};

class CQuestSystem
{
public:
	CQuestSystem();
	virtual ~CQuestSystem();
	void Init();
	void Load();
	void LoadFile(char *path);
	void load_txt(char* path);
	// ----
	bool Dialog(LPOBJ lpObj, LPOBJ lpNpc);
	void NpcTalk(LPOBJ lpObj);
	void connect(LPOBJ lpObj);
	//void TalkToNpc(int aIndex);
	void make_reward(LPOBJ lpObj);
	bool QuestExist(int index);
	QUEST_DATA* GetQuest(int index);
	void MonsterKill(LPOBJ lpObj, LPOBJ lpTarget);
	// ----
	static void send_message(int aIndex, int type, char* szMsg, ...);
	static void MonsterBarAdd(int aIndex, int MonsterID);
	bool MakeItem(LPOBJ lpObj, int QuestIndex);
	void gc_send_quest_info(int aIndex, bool state=true);
	// ----
	//void Effect(LPOBJ lpUser);
	// ----
	int enable;
	int npc_id;
	int npc_map;
	int npc_x;
	int npc_y;
	int AddForEachPartyMember;
	// ----
	//QUESTDATA m_Quest[MAX_QUESTS];

	//std::vector<QUEST_DATA> quest_list;
	std::map<int,QUEST_DATA> quest_list;

	convert_quest convert_data;
	
	// ----
}; extern CQuestSystem gCustomQuest;
// -------------------------------------------------------------------------