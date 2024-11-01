// MapManager.h: interface for the CMapManager class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

struct MAP_MANAGER_INFO
{
	int Index;
	int NonPK;
	int ViewRange;
	int ExperienceRate;
	int ItemDropRate;
	int ExcItemDropRate;
	int SetItemDropRate;
	int SocketItemDrop;
	int HelperEnable;
	int GensBattle;
	int OffTradeEnable;
	int OffExpEnable;
	int HPBarEnable;
	int ResetEnable;
	int GResetEnable;
	int MasterExpEnable;
	int MonsterCallEnable;
};

struct HEALTH_BAR_MAP
{
	BYTE mapNumber;
	BYTE isHealthBarEnable;
};

class CMapManager
{
public:
	CMapManager();
	virtual ~CMapManager();
	void Load(char* path);
	int GetMapNonPK(int index);
	int GetMapNonOutlaw(int index);
	int GetMapViewRange(int index);
	int GetMapExperienceRate(int index);
	int GetMapMasterExperienceRate(int index);
	int GetMapItemDropRate(int index);
	int GetMapExcItemDropRate(int index);
	int GetMapSetItemDropRate(int index);
	int GetMapSocketItemDrop(int index);
	int GetMapHelperEnable(int index);
	int GetMapGensBattle(int index);
	int GetMapOffTradeEnable(int index);
	int GetMapOffExpEnable(int index);
	int GetMapHPBarEnable(int index);
	int GetMapResetEnable(int index);
	int GetMapGResetEnable(int index);
	int GetMapMasterExpEnable(int index);
	int GetMapMonsterCallEnable(int index);
	// ----
	void GCSendHealthBar(int aIndex);
public:
	std::map<int,MAP_MANAGER_INFO> m_MapManagerInfo;
};

extern CMapManager gMapManager;
