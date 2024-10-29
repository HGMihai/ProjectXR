// MapManager.cpp: implementation of the CMapManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MapManager.h"
#include "MemScript.h"
#include "ServerInfo.h"
#include "Util.h"

CMapManager gMapManager;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMapManager::CMapManager() // OK
{
	this->m_MapManagerInfo.clear();
}

CMapManager::~CMapManager() // OK
{

}

void CMapManager::Load(char* path)
{
	pugi::xml_document file;
	pugi::xml_parse_result res = file.load_file(path);

	if(res.status != pugi::status_ok)
	{
		ErrorMessageBox("%s file load fail (%s)", path, res.description());
	}

	this->m_MapManagerInfo.clear();

	pugi::xml_node MapManager = file.child("MapManager");
	for (pugi::xml_node map = MapManager.child("Map"); map; map = map.next_sibling())
	{
		//int skillid = map.attribute("Index").as_int();
		//this->m_SkillCategories[skillid].SkillType = map.attribute("Type").as_int();

		MAP_MANAGER_INFO info;

		memset(&info,0,sizeof(info));

		info.Index = map.attribute("Index").as_int();

		info.NonPK = map.attribute("NonPK").as_int();

		info.ViewRange = map.attribute("ViewRange").as_int();

		info.ExperienceRate = map.attribute("ExpRate").as_int();

		info.ItemDropRate = map.attribute("ItemDropRate").as_int();

		info.ExcItemDropRate = map.attribute("ExcItemDropRate").as_int();

		info.SetItemDropRate = map.attribute("SetItemDropRate").as_int();

		info.SocketItemDrop = map.attribute("SocketItemDrop").as_int();

		#if(GAMESERVER_UPDATE>=603)

		info.HelperEnable = map.attribute("HelperEnable").as_int();

		#else

		info.HelperEnable = 0;

		#endif

		#if(GAMESERVER_UPDATE>=501)

		info.GensBattle = map.attribute("GensBattle").as_int();

		#else

		info.GensBattle = 0;

		#endif

		info.OffTradeEnable = map.attribute("OffTradeEnable").as_int();

		info.OffExpEnable = map.attribute("OffExpEnable").as_int();

		#if(GAMESERVER_UPDATE<=603)

		info.HPBarEnable = map.attribute("HealthBarEnable").as_int();

		#else

		info.HPBarEnable = 0;

		#endif

		info.ResetEnable = map.attribute("ResetEnable").as_int();

		info.GResetEnable = map.attribute("GResetEnable").as_int();

		info.MasterExpEnable = map.attribute("MasterExpEnable").as_int();

		info.MonsterCallEnable = map.attribute("MonsterCallEnable").as_int(1);

		this->m_MapManagerInfo.insert(std::pair<int,MAP_MANAGER_INFO>(info.Index,info));
	}
}

int CMapManager::GetMapNonPK(int index) // OK
{
	std::map<int,MAP_MANAGER_INFO>::iterator it = this->m_MapManagerInfo.find(index);

	if(it == this->m_MapManagerInfo.end())
	{
		return gServerInfo.m_NonPK;
	}
	else
	{
		return ((it->second.NonPK==-1)?gServerInfo.m_NonPK:((it->second.NonPK==1)?1:0));
	}
}

int CMapManager::GetMapNonOutlaw(int index) // OK
{
	std::map<int,MAP_MANAGER_INFO>::iterator it = this->m_MapManagerInfo.find(index);

	if(it == this->m_MapManagerInfo.end())
	{
		return 0;
	}
	else
	{
		return ((it->second.NonPK==-1)?0:((it->second.NonPK==2)?1:0));
	}
}

int CMapManager::GetMapViewRange(int index) // OK
{
	std::map<int,MAP_MANAGER_INFO>::iterator it = this->m_MapManagerInfo.find(index);

	if(it == this->m_MapManagerInfo.end())
	{
		return 15;
	}
	else
	{
		return ((it->second.ViewRange==-1)?15:it->second.ViewRange);
	}
}

int CMapManager::GetMapExperienceRate(int index) // OK
{
	std::map<int,MAP_MANAGER_INFO>::iterator it = this->m_MapManagerInfo.find(index);

	if(it == this->m_MapManagerInfo.end())
	{
		return 100;
	}
	else
	{
		return ((it->second.ExperienceRate==-1)?100:it->second.ExperienceRate);
	}
}

int CMapManager::GetMapMasterExperienceRate(int index) // OK
{
	std::map<int,MAP_MANAGER_INFO>::iterator it = this->m_MapManagerInfo.find(index);

	if(it == this->m_MapManagerInfo.end())
	{
		return 100;
	}
	else
	{
		return ((it->second.ExperienceRate==-1)?100:it->second.ExperienceRate);
	}
}

int CMapManager::GetMapItemDropRate(int index) // OK
{
	std::map<int,MAP_MANAGER_INFO>::iterator it = this->m_MapManagerInfo.find(index);

	if(it == this->m_MapManagerInfo.end())
	{
		return 100;
	}
	else
	{
		return ((it->second.ItemDropRate==-1)?100:it->second.ItemDropRate);
	}
}

int CMapManager::GetMapExcItemDropRate(int index) // OK
{
	std::map<int,MAP_MANAGER_INFO>::iterator it = this->m_MapManagerInfo.find(index);

	if(it == this->m_MapManagerInfo.end())
	{
		return 0;
	}
	else
	{
		return ((it->second.ExcItemDropRate==-1)?0:it->second.ExcItemDropRate);
	}
}

int CMapManager::GetMapSetItemDropRate(int index) // OK
{
	std::map<int,MAP_MANAGER_INFO>::iterator it = this->m_MapManagerInfo.find(index);

	if(it == this->m_MapManagerInfo.end())
	{
		return 0;
	}
	else
	{
		return ((it->second.SetItemDropRate==-1)?0:it->second.SetItemDropRate);
	}
}

int CMapManager::GetMapSocketItemDrop(int index) // OK
{
	std::map<int,MAP_MANAGER_INFO>::iterator it = this->m_MapManagerInfo.find(index);

	if(it == this->m_MapManagerInfo.end())
	{
		return 0;
	}
	else
	{
		return ((it->second.SocketItemDrop==-1)?0:it->second.SocketItemDrop);
	}
}

int CMapManager::GetMapHelperEnable(int index) // OK
{
	std::map<int,MAP_MANAGER_INFO>::iterator it = this->m_MapManagerInfo.find(index);

	if(it == this->m_MapManagerInfo.end())
	{
		return 0;
	}
	else
	{
		return ((it->second.HelperEnable==-1)?0:it->second.HelperEnable);
	}
}

int CMapManager::GetMapGensBattle(int index) // OK
{
	std::map<int,MAP_MANAGER_INFO>::iterator it = this->m_MapManagerInfo.find(index);

	if(it == this->m_MapManagerInfo.end())
	{
		return 0;
	}
	else
	{
		return ((it->second.GensBattle==-1)?0:it->second.GensBattle);
	}
}

int CMapManager::GetMapOffTradeEnable(int index)
{
	std::map<int,MAP_MANAGER_INFO>::iterator it = this->m_MapManagerInfo.find(index);

	if(it == this->m_MapManagerInfo.end())
	{
		return 0;
	}
	else
	{
		return ((it->second.OffTradeEnable==-1)?0:it->second.OffTradeEnable);
	}
}

int CMapManager::GetMapOffExpEnable(int index)
{
	std::map<int,MAP_MANAGER_INFO>::iterator it = this->m_MapManagerInfo.find(index);

	if(it == this->m_MapManagerInfo.end())
	{
		return 0;
	}
	else
	{
		return ((it->second.OffExpEnable==-1)?0:it->second.OffExpEnable);
	}
}

int CMapManager::GetMapHPBarEnable(int index)
{
	std::map<int,MAP_MANAGER_INFO>::iterator it = this->m_MapManagerInfo.find(index);

	if(it == this->m_MapManagerInfo.end())
	{
		return 0;
	}
	else
	{
		return ((it->second.HPBarEnable==-1)?0:it->second.HPBarEnable);
	}
}

int CMapManager::GetMapResetEnable(int index)
{
	std::map<int,MAP_MANAGER_INFO>::iterator it = this->m_MapManagerInfo.find(index);

	if(it == this->m_MapManagerInfo.end())
	{
		return 0;
	}
	else
	{
		return ((it->second.ResetEnable==-1)?0:it->second.ResetEnable);
	}
}

int CMapManager::GetMapGResetEnable(int index)
{
	std::map<int,MAP_MANAGER_INFO>::iterator it = this->m_MapManagerInfo.find(index);

	if(it == this->m_MapManagerInfo.end())
	{
		return 0;
	}
	else
	{
		return ((it->second.GResetEnable==-1)?0:it->second.GResetEnable);
	}
}

int CMapManager::GetMapMasterExpEnable(int index)
{
	std::map<int,MAP_MANAGER_INFO>::iterator it = this->m_MapManagerInfo.find(index);

	if(it == this->m_MapManagerInfo.end())
	{
		return 0;
	}
	else
	{
		return ((it->second.MasterExpEnable==-1)?0:it->second.MasterExpEnable);
	}
}

int CMapManager::GetMapMonsterCallEnable(int index)
{
	std::map<int,MAP_MANAGER_INFO>::iterator it = this->m_MapManagerInfo.find(index);

	if(it == this->m_MapManagerInfo.end())
	{
		return 0;
	}
	else
	{
		return ((it->second.MonsterCallEnable==-1)?0:it->second.MonsterCallEnable);
	}
}

void CMapManager::GCSendHealthBar(int aIndex)
{
	//PMSG_HEALTH_BAR_MAP_SEND pMsg;
	//pMsg.header.set(0xFB,0x20,sizeof(pMsg));

	//pMsg.count = m_MapManagerInfo.size();

	///*for(int i=0;i<this->m_MapManagerInfo.size();i++)
	//{
	//	pMsg.map_data[i].mapNumber = m_MapManagerInfo[i].Index;
	//	pMsg.map_data[i].isHealthBarEnable = it->second.HPBarEnable;
	//}*/

	//int i=0;
	//
	//for(auto it = m_MapManagerInfo.begin(); it != m_MapManagerInfo.end(); ++it)
	//{
	//	pMsg.map_data[i].mapNumber = it->second.Index;
	//	pMsg.map_data[i].isHealthBarEnable = it->second.HPBarEnable;

	//	LogAdd(LOG_TEST, "[%d] count: %d map: %d enabled: %d",i,pMsg.count,pMsg.map_data[i].mapNumber,pMsg.map_data[i].isHealthBarEnable);
	//	i++;
	//}

	//DataSend(aIndex,(BYTE*)&pMsg,sizeof(pMsg));
}
