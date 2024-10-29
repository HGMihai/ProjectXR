#include "stdafx.h"
#include "RegionSystem.h"
#include "Util.h"

CRegionSystem gRegionSystem;

CRegionSystem::CRegionSystem()
{
	this->m_region_data.clear();
}

void CRegionSystem::load(char* path)
{
	pugi::xml_document file;
	pugi::xml_parse_result res = file.load_file(path);

	if(res.status != pugi::status_ok)
	{
		ErrorMessageBox("%s file load fail (%s)", path, res.description());
	}
	
	this->m_region_data.clear();

	pugi::xml_node region_node = file.child("RegionSystem");

	this->castle_siege_server_code = region_node.attribute("CastleSiegeServerCode").as_int();

	for (pugi::xml_node region = region_node.child("Region"); region; region = region.next_sibling())
	{
		REGION_DATA info;

		info.server_code = region.attribute("ServerCode").as_int();
		info.region_name = region.attribute("RegionName").as_string();

		this->m_region_data.insert(std::pair<std::string,REGION_DATA>(std::string(info.region_name),info));
	}
}

short CRegionSystem::get_server_code_by_name(char* region_name)
{
	///LogAdd(LOG_TEST,"[get_region] start, text: %s",region_name);
	std::map<std::string,REGION_DATA>::iterator it = this->m_region_data.find(std::string(region_name));

	if(it == this->m_region_data.end())
	{
		//LogAdd(LOG_TEST,"[get_region] end cant found region");
		return -1;
	}

	//LogAdd(LOG_TEST,"[get_region] end. return %d region",it->second.server_code);
	return it->second.server_code;
}
