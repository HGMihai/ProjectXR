// MonsterSetBase.cpp: implementation of the CMonsterSetBase class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MonsterSetBase.h"
#include "MapServerManager.h"
#include "MemScript.h"
#include "Message.h"
#include "MonsterManager.h"
#include "Path.h"
#include "Util.h"

CMonsterSetBase gMonsterSetBase;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMonsterSetBase::CMonsterSetBase() : m_count(0) // OK
{
}

CMonsterSetBase::~CMonsterSetBase() // OK
{

}

void CMonsterSetBase::Load(char* path)
{
	char current_path[256];

	strcpy_s(current_path, path);

	//if(FileExist(gPath.GetFullPath("Monster\\MonsterSetBase_to_xml.txt")))
	//if(FileExist("Convert\\MonsterSetBase.txt"))
	if(std::filesystem::exists("Convert\\MonsterSetBase.txt"))
	{
		std::string dir = "Convert\\Generated";

		if (!std::filesystem::is_directory(dir) || !std::filesystem::exists(dir)) { // Check if src folder exists
			std::filesystem::create_directory(dir); // create src folder
		}

		this->m_count = 0;
		this->txt_list.clear();

		this->LoadTXT("Convert\\MonsterSetBase.txt");

		if(std::filesystem::exists("Convert\\MonsterSetBaseCS.txt"))
			this->LoadTXT("Convert\\MonsterSetBaseCS.txt");

		this->generate_xml();

		return;
	}

	pugi::xml_document file;
	pugi::xml_parse_result res = file.load_file(current_path);

	if (res.status != pugi::status_ok)
	{
		ErrorMessageBox("%s file load fail (%s)", current_path, res.description());
	}

	this->m_count = 0;

	pugi::xml_node main = file.child("MonsterSpawn");

	for (pugi::xml_node map = main.child("Map"); map; map = map.next_sibling())
	{
		int iMapNumber = map.attribute("Number").as_int();

		if (MAP_RANGE(iMapNumber) == false)
		{
			continue;
		}

		for (pugi::xml_node spot = map.child("Spot"); spot; spot = spot.next_sibling())
		{
			int iSpotType = spot.attribute("Type").as_int();

			if (iSpotType < Arrange_NpcSpawn || iSpotType > Arrange_EventMonsterSpawn)
			{
				ErrorMessageBox("Error in %s file: Wrong Spot Type: %d", current_path, iSpotType);
				continue;
			}

			for (pugi::xml_node spawn = spot.child("Spawn"); spawn; spawn = spawn.next_sibling())
			{
				MONSTER_SET_BASE_INFO info = {};

				info.MonsterClass = spawn.attribute("Index").as_int();

				if (info.MonsterClass == -1)
				{
					continue;
				}

				info.Map = iMapNumber;
				info.Dis = spawn.attribute("Distance").as_int();
				info.X = spawn.attribute("StartX").as_int();
				info.Y = spawn.attribute("StartY").as_int();
				info.Type = iSpotType;

				if (iSpotType == Arrange_MultiMonsterSpawn || iSpotType == Arrange_BossMonsterSpawn)
				{
					info.TX = spawn.attribute("EndX").as_int();
					info.TY = spawn.attribute("EndY").as_int();
				}
				else if (iSpotType == Arrange_SingleMonsterSpawn)
				{
					info.X = (info.X - 3) + GetLargeRand() % 7;
					info.Y = (info.Y - 3) + GetLargeRand() % 7;
				}

				info.Dir = spawn.attribute("Dir").as_int();

				if(info.Dir == -1)
					info.Dir = rand() % 8;

				if (iSpotType == Arrange_MultiMonsterSpawn || iSpotType == Arrange_BossMonsterSpawn)
				{
					info.Count = spawn.attribute("Count").as_int();

					if (iSpotType == Arrange_BossMonsterSpawn)
						info.Value = spawn.attribute("Value").as_int();

					for (int i = 0; i < info.Count; i++)
						this->SetInfo(info);
				}
				else
				{
					this->SetInfo(info);
				}
			}
		}
	}
}


void CMonsterSetBase::generate_xml() const
{
	pugi::xml_document doc;
	pugi::xml_node declarationNode = doc.append_child(pugi::node_declaration);
	declarationNode.append_attribute("version") = "1.0";
	declarationNode.append_attribute("encoding") = "utf-8";

	doc.append_child(pugi::node_pcdata).set_value(msb_info_text.c_str());
	doc.append_child(pugi::node_pcdata).set_value("");

	pugi::xml_node root = doc.append_child("MonsterSpawn");

	for (const auto& map_pair : this->txt_list)
	{
		auto map = root.append_child("Map");
		map.append_attribute("Number") = map_pair.first;
		map.append_attribute("Name") = gMessage.GetMap(map_pair.first);

		for (const auto& spot_pair : map_pair.second)
		{
			auto spot = map.append_child("Spot");
			spot.append_attribute("Type") = spot_pair.first;
			spot.append_attribute("Description") = ArrangeText[spot_pair.first];

			for (const auto& spawn_vec : spot_pair.second)
			{
				auto spawn = spot.append_child("Spawn");
				spawn.append_attribute("Index") = spawn_vec.MonsterClass;
				spawn.append_attribute("Distance") = spawn_vec.Dis;
				spawn.append_attribute("StartX") = spawn_vec.X;
				spawn.append_attribute("StartY") = spawn_vec.Y;

				if (spawn_vec.Type == Arrange_MultiMonsterSpawn || spawn_vec.Type == Arrange_BossMonsterSpawn)
				{
					spawn.append_attribute("EndX") = spawn_vec.TX;
					spawn.append_attribute("EndY") = spawn_vec.TY;
				}

				spawn.append_attribute("Dir") = spawn_vec.Dir;

				if (spawn_vec.Type == Arrange_MultiMonsterSpawn || spawn_vec.Type == Arrange_BossMonsterSpawn)
				{
					spawn.append_attribute("Count") = spawn_vec.Count;

					if (spawn_vec.Type == Arrange_BossMonsterSpawn)
						spawn.append_attribute("Value") = spawn_vec.Value;
				}
				spot.insert_child_after(pugi::node_comment, spawn).set_value(gMonsterManager.get_name(spawn_vec.MonsterClass));
			}
		}
	}

	std::string path = "Convert\\MonsterSetBase";
	std::string xml_path = "Convert\\Generated\\MonsterSpawn";

	if(std::filesystem::exists(xml_path+".xml"))
		std::filesystem::rename(xml_path + ".xml", xml_path + ".bak");

	if (std::filesystem::exists(path+"CS.txt"))
		std::filesystem::rename(path + "CS.txt", path + "CS.bak");

	doc.save_file((xml_path + ".xml").c_str());

	std::filesystem::rename(path + ".txt", path + ".bak");

	MsgBox("[%s] convert complete.\nCheck new file in \"Convert\\Generated\" folder.\nPlease, reboot gameserver", (path + ".txt").c_str());
}

void CMonsterSetBase::add_to_list(MONSTER_SET_BASE_INFO info)
{
	auto pair = this->txt_list.find(info.Map);
	
	if(pair == this->txt_list.end())
	{
		std::vector<MONSTER_SET_BASE_INFO> vec;
		vec.push_back(info);

		std::map<int, std::vector<MONSTER_SET_BASE_INFO>> data;
		data.insert(std::make_pair(info.Type, vec));

		this->txt_list.insert(std::make_pair(info.Map, data));
	}
	else
	{
		auto type_pair = pair->second.find(info.Type);

		if(type_pair == pair->second.end())
		{
			std::vector<MONSTER_SET_BASE_INFO> vec;
			vec.push_back(info);

			pair->second.insert(std::make_pair(info.Type, vec));
		}
		else
		{
			type_pair->second.push_back(info);
		}
	}
}

void CMonsterSetBase::LoadTXT(char* path) // OK
{
	auto lpMemScript = new CMemScript;

	if(lpMemScript == nullptr)
	{
		ErrorMessageBox(MEM_SCRIPT_ALLOC_ERROR,path);
		return;
	}

	if(lpMemScript->SetBuffer(path) == 0)
	{
		ErrorMessageBox(lpMemScript->GetLastError());
		delete lpMemScript;
		return;
	}

	try
	{
		while(true)
		{
			if(lpMemScript->GetToken() == TOKEN_END)
			{
				break;
			}

			int section = lpMemScript->GetNumber();

			while(true)
			{
				if(strcmp("end",lpMemScript->GetAsString()) == 0)
				{
					break;
				}

				MONSTER_SET_BASE_INFO info;

				memset(&info,0,sizeof(info));

				info.Type = section;

				info.MonsterClass = lpMemScript->GetNumber();

				info.Map = lpMemScript->GetAsNumber();

				info.Dis = lpMemScript->GetAsNumber();

				info.X = lpMemScript->GetAsNumber();

				info.Y = lpMemScript->GetAsNumber();

				if(section == 1 || section == 3)
				{
					info.TX = lpMemScript->GetAsNumber();
					info.TY = lpMemScript->GetAsNumber();
				}
				else if(section == 2)
				{
					info.X = (info.X-3)+GetLargeRand()%7;
					info.Y = (info.Y-3)+GetLargeRand()%7;
				}

				info.Dir = lpMemScript->GetAsNumber();

				if(section == 1 || section == 3)
				{
					info.Count = lpMemScript->GetAsNumber();

					if(section == 3)
					{
						info.Value = lpMemScript->GetAsNumber();
					}

					for(int n=0;n < info.Count;n++)
					{
						this->SetInfo(info);
					}
				}
				else
				{
					this->SetInfo(info);
				}

				this->add_to_list(info);
			}
		}
	}
	
	catch(...)
	{
		ErrorMessageBox(lpMemScript->GetLastError());
	}
	//MsgBox("MSB loaded. Total = %d",this->m_count);
	delete lpMemScript;
}

void CMonsterSetBase::SetInfo(MONSTER_SET_BASE_INFO info) // OK
{
	/*if (info.Type == 3 || info.Type == 4)
	{
		return;
	}*/

	if(this->m_count < 0 || this->m_count >= MAX_MSB_MONSTER)
	{
		//LogAdd(LOG_TEST,"[ERROR] count = %d monsterId = %d map = %d X = %d Y = %d",this->m_count,info.MonsterClass,info.Map,info.X,info.Y);
		return;
	}
	
	if(gMapServerManager.CheckMapServer(info.Map) == 0)
	{
		return;
	}

	info.Dir = ((info.Dir==-1)?(GetLargeRand()%8):info.Dir);

	//LogAdd(LOG_TEST,"[GOOD] count = %d monsterId = %d map = %d X = %d Y = %d",this->m_count,info.MonsterClass,info.Map,info.X,info.Y);
	
	this->m_MonsterSetBaseInfo[this->m_count++] = info;

	//txt_list.insert(std::make_pair(info.Map, info));
}

bool CMonsterSetBase::GetPosition(int index,short map,short* ox,short* oy) // OK
{
	if(index < 0 || index >= MAX_MSB_MONSTER)
	{
		return false;
	}

	MONSTER_SET_BASE_INFO* lpInfo = &this->m_MonsterSetBaseInfo[index];
	
	if(lpInfo->Type == 0 || lpInfo->Type == 4)
	{
		(*ox) = lpInfo->X;
		(*oy) = lpInfo->Y;
		return true;
	}
	if(lpInfo->Type == 1 || lpInfo->Type == 3)
	{
		return this->GetBoxPosition(map,lpInfo->X,lpInfo->Y,lpInfo->TX,lpInfo->TY,ox,oy);
	}
	if(lpInfo->Type == 2)
	{
		return this->GetBoxPosition(map,(lpInfo->X-3),(lpInfo->Y-3),(lpInfo->X+3),(lpInfo->Y+3),ox,oy);
	}

	return false;
}

bool CMonsterSetBase::GetBoxPosition(int map,int x,int y,int tx,int ty,short* ox,short* oy) // OK
{
	for(int n=0;n < 100;n++)
	{
		int subx = tx-x;
		int suby = ty-y;

		subx = ((subx<1)?1:subx);
		suby = ((suby<1)?1:suby);

		subx = x+(GetLargeRand()%subx);
		suby = y+(GetLargeRand()%suby);

		if(gMap[map].CheckAttr(subx,suby,1) == 0 && gMap[map].CheckAttr(subx,suby,4) == 0 && gMap[map].CheckAttr(subx,suby,8) == 0)
		{
			(*ox) = subx;
			(*oy) = suby;
			return true;
		}
	}

	return false;
}

void CMonsterSetBase::SetBoxPosition(int index,int map,int x,int y,int tx,int ty) // OK
{
	if(index < 0 || index >= MAX_MSB_MONSTER)
	{
		return;
	}

	MONSTER_SET_BASE_INFO* lpInfo = &this->m_MonsterSetBaseInfo[index];

	lpInfo->Map = map;
	lpInfo->X = x;
	lpInfo->Y = y;
	lpInfo->TX = tx;
	lpInfo->TY = ty;
}
