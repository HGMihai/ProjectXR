#pragma once

struct REGION_DATA
{
	int server_code;
	std::string region_name;
};

class CRegionSystem
{
public:
	CRegionSystem();
	void load(char* path);
	short get_server_code_by_name(char*region_name);
	int get_cs_server_code() {return castle_siege_server_code;}
private:
	int castle_siege_server_code;
	std::map<std::string,REGION_DATA> m_region_data;
}; extern CRegionSystem gRegionSystem;