#pragma once

#include "Protocol.h"
#include "User.h"
#include "Map.h"
#include "ServerInfo.h"

#define MAX_QUEST_TELEPORT 4

struct QuestNPCTeleportPos 
{
	int mapnum;	// 0
	int x;	// 4
	int y;	// 8
	int dir;	// C
};

class CQuestNpcTeleport	// size == 0x68
{

public:
	//
	CQuestNpcTeleport()
	{
		this->LastMapNumber = -1;
		this->RefCount = 0;
		InitializeCriticalSection(&this->CsRef);
		this->TimeCount = 0;
		this->m_QuestNPCTeleportPos[0].mapnum = MAP_DEVIAS;
		this->m_QuestNPCTeleportPos[0].x = 198;
		this->m_QuestNPCTeleportPos[0].y = 47;
		this->m_QuestNPCTeleportPos[0].dir = 2;
		this->m_QuestNPCTeleportPos[1].mapnum = MAP_LORENCIA;
		this->m_QuestNPCTeleportPos[1].x = 137;
		this->m_QuestNPCTeleportPos[1].y = 87;
		this->m_QuestNPCTeleportPos[1].dir = 1;
		this->m_QuestNPCTeleportPos[2].mapnum = MAP_NORIA;
		this->m_QuestNPCTeleportPos[2].x = 169;
		this->m_QuestNPCTeleportPos[2].y = 89;
		this->m_QuestNPCTeleportPos[2].dir = 2;
		this->m_QuestNPCTeleportPos[3].mapnum = MAP_ATLANS;
		this->m_QuestNPCTeleportPos[3].x = 17;
		this->m_QuestNPCTeleportPos[3].y = 25;
		this->m_QuestNPCTeleportPos[3].dir = 2;
	};	// line : 114

	//0041d7e0	-> 100%
	~CQuestNpcTeleport() 
	{
		DeleteCriticalSection(&this->CsRef);
	};	

	void TalkRefAdd()	// line : 118
	{
		EnterCriticalSection(&this->CsRef);
		this->RefCount++;
		LeaveCriticalSection(&this->CsRef);
		
	};	// line : 123


	void TalkRefDel()	// line : 126
	{
		EnterCriticalSection(&this->CsRef);
		this->RefCount--;

		if ( this->RefCount < 0 )
		{
			this->RefCount = 0;
		}
		
		LeaveCriticalSection(&this->CsRef);
	};	// line : 135

	//int __thiscall GetRefCount();

	void Run(int aIndex);

	int RefCount;	// 0
	int TimeCount; // 1C
	int LastMapNumber;	// 20
	QuestNPCTeleportPos m_QuestNPCTeleportPos[MAX_QUEST_TELEPORT];	// 24

private:
	CRITICAL_SECTION CsRef;	// 4
};
extern CQuestNpcTeleport gQuestNpcTeleport;

void gObjMonsterDieGiveItem(LPOBJ lpObj,LPOBJ lpTarget);
bool gObjSetPosMonster(int aIndex,int number);
bool gObjSetMonster(int aIndex,int MonsterClass);
int gObjSetMonsterTest(int aIndex, int MonsterClass, LPSTR FunctionName);
bool gObjMonsterRegen(LPOBJ lpObj);
bool gObjMonsterMoveCheck(LPOBJ lpObj,int tx,int ty);
void gObjMonsterInitHitDamage(LPOBJ lpObj);
void gObjMonsterSetHitDamage(LPOBJ lpObj,int aIndex,int damage);
int gObjMonsterDelHitDamageUser(LPOBJ lpObj);
int gObjMonsterGetTopHitDamageUser(LPOBJ lpObj);
int gObjMonsterGetTopHitDamageParty(LPOBJ lpObj,int PartyNumber,int* TopHitDamageUser);
int gObjAddMonster(int map);
int gObjAddPlayer(int map);
int gObjAddSummon();
int gObjSummonSetEnemy(LPOBJ lpObj,int aIndex);
void gObjSummonKill(int aIndex);
void gObjMonsterMagicAttack(LPOBJ lpObj,int skill);
void gObjMonsterBeattackRecv(BYTE* aRecv,int aIndex);
void gObjMonsterAttack(LPOBJ lpObj,LPOBJ lpTarget);
void gObjMonsterDie(LPOBJ lpObj,LPOBJ lpTarget);
void gObjMonsterStateProc(LPOBJ lpObj,int code,int aIndex,int SubCode);

//**************************************************************************//
// RAW FUNCTIONS ***********************************************************//
//**************************************************************************//

struct PMSG_MAGICATTACK_RESULT
{
	PBMSG_HEAD h;	// C3:19
	BYTE MagicNumberH;	// 3
	BYTE MagicNumberL;	// 4
	BYTE SourceNumberH;	// 5
	BYTE SourceNumberL;	// 6
	BYTE TargetNumberH;	// 7
	BYTE TargetNumberL;	// 8
};

struct PMSG_MAGICATTACK
{
	PBMSG_HEAD h;
	BYTE MagicNumberH;	// 3
	BYTE MagicNumberL;	// 4
	BYTE NumberH;	// 5
	BYTE NumberL;	// 6
	BYTE Dis;	// 7
};

struct PMSG_BEATTACK_COUNT
{
	PBMSG_HEAD h;	// C1:D7
	BYTE MagicNumberH;	// 3
	BYTE MagicNumberL;	// 4
	BYTE X;	// 5
	BYTE Y;	// 6
	BYTE Serial;	// 7
	BYTE Count;	// 8
};

struct PMSG_BEATTACK
{
	BYTE NumberH;	// 0
	BYTE NumberL;	// 1
	BYTE MagicKey;	// 2
};

struct PMSG_DURATION_MAGIC_RECV
{
	PBMSG_HEAD h;
	BYTE MagicNumberH;	// 3
	BYTE MagicNumberL;	// 4
	BYTE X;	// 5
	BYTE Y;	// 6
	BYTE Dir;	// 7
	BYTE Dis;	// 8
	BYTE TargetPos;	// 9
	BYTE NumberH;	// A
	BYTE NumberL;	// B
	BYTE MagicKey;	// C
};

struct PMSG_ATTACK
{
	PBMSG_HEAD h;
	BYTE NumberH;	// 3
	BYTE NumberL;	// 4
	BYTE AttackAction;	// 5
	BYTE DirDis;	// 6
};

struct PMSG_MOVE
{
	PBMSG_HEAD h;	// C1:1D
	BYTE X;	// 3
	BYTE Y;	// 4
	BYTE Path[8];	// 5
};

int gObjMonsterViewportIsCharacter(LPOBJ lpObj);
BOOL gObjMonsterGetTargetPos(LPOBJ lpObj);
BOOL gObjGetTargetPos(LPOBJ lpObj, int sx, int sy, int & tx , int & ty);
int gObjMonsterSearchEnemy(LPOBJ lpObj, BYTE objtype);
int gObjGuardSearchEnemy(LPOBJ lpObj);
void gObjMonsterProcess(LPOBJ lpObj);
BOOL PathFindMoveMsgSend(LPOBJ lpObj);
void gObjMonsterMoveAction(LPOBJ lpObj);
void gObjMonsterBaseAct(LPOBJ lpObj);
void gObjTrapAttackEnemySearchX(LPOBJ lpObj, int count);
void gObjTrapAttackEnemySearchY(LPOBJ lpObj, int count);
void gObjTrapAttackEnemySearch(LPOBJ lpObj);
void gObjTrapAttackEnemySearchRange(LPOBJ lpObj,int iRange);
void gObjMonsterTrapAct(LPOBJ lpObj);
void gObjMonsterStateProcCase0(LPOBJ lpObj,int aIndex);
