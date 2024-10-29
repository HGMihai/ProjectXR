// MonsterSetBase.h: interface for the CMonsterSetBase class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#define MAX_MSB_MONSTER 20000

struct MONSTER_SET_BASE_INFO
{
	int Type;
	int MonsterClass;
	int Map;
	int Dis;
	int X;
	int Y;
	int Dir;
	int TX;
	int TY;
	int Count;
	int Value;
};

enum eArrangeType
{
	Arrange_NpcSpawn = 0,
	Arrange_MultiMonsterSpawn = 1,
	Arrange_SingleMonsterSpawn = 2,
	Arrange_BossMonsterSpawn = 3,
	Arrange_EventMonsterSpawn = 4,
};

static char ArrangeText[5][128] =
{
	"NPC/Traps",
	"Multi Spawn",
	"Single Point Monsters",
	"Boss Multi Spawn",
	"Event Monster Spawn"
};


class CMonsterSetBase
{
public:
	CMonsterSetBase();
	virtual ~CMonsterSetBase();
	void Load(char* path);
	void generate_xml() const;
	void add_to_list(MONSTER_SET_BASE_INFO info);
	void LoadTXT(char* path);
	void SetInfo(MONSTER_SET_BASE_INFO info);
	bool GetPosition(int index,short map,short* ox,short* oy);
	bool GetBoxPosition(int map,int x,int y,int tx,int ty,short* ox,short* oy);
	void SetBoxPosition(int index,int map,int x,int y,int tx,int ty);
public:
	MONSTER_SET_BASE_INFO m_MonsterSetBaseInfo[MAX_MSB_MONSTER];
	std::map<int, std::map<int, std::vector<MONSTER_SET_BASE_INFO>>> txt_list;
	int m_count;
};

extern CMonsterSetBase gMonsterSetBase;

static const std::string msb_info_text =
R"(<!-- 
// ============================================================
// == www.ltp-team.com
// == (C) 2022 LTP-Team
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ### MonsterSpawn::Map ###
//	Number: Number of map to spawn the monster on, refer to MapManager.xml
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~ Same map index can be defined only one in file
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Name: Name of a map, used for in-file description and logging purpose
//
// ### MonsterSpawn::Map::Spot ###
//	Type: Type of spot:
//		~ 0: NPC/Traps
//		~ 1: Multiple Monsters Spawn
//		~ 2: Single Monster Spawn
//		~ 3: Boss Multi Spawn
//		~ 4: Event Monster Spawn
//	Description: Spot description, used for in-file description and logging purpose
//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~ Spot attributes description
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Index: Index of monster or NPC to spawn, refer to Monster.txt
//	Distance: Distance to spawn the monsters/NPC from original location
//	StartX: StartX Coordinate of monster spawn
//	StartY: StartY Coordinate of monster spawn
//	EndX: End X Coordinate of monster spawn
//	EndY: End Y Coordinate of monster spawn
//	Dir: Direction the monster/NPC looks at after spawn
//		~ 0: Central
//		~ 1: South-West
//		~ 2: South
//		~ 3: South-East
//		~ 4: East
//		~ 5: North-East
//		~ 6: North
//		~ 7: North-West
//		~ 8: West
//		~ -1: Random
//	Count: Count of monsters to spawn
//	Value: Value index from InvasionManager
//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// STRICTLY NO COMMENTS INSIDE TAGS
-->)";
