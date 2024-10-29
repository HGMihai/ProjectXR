#include "stdafx.h"
#include "ComboSkillData.h"
#include "Util.h"

CComboSkillData gComboSkillData;

CComboSkillData::CComboSkillData()
{
	this->m_skill_data_by_class.clear();
}

CComboSkillData::~CComboSkillData() { }

//CComboSkillData* CComboSkillData::GetInstance()
//{
//	static CComboSkillData sComboSkillData;
//	return &sComboSkillData;
//}


void CComboSkillData::load(char* path)
{
	this->m_skill_data_by_class.clear();

	pugi::xml_document file;
	pugi::xml_parse_result res = file.load_file(path);

	if(res.status != pugi::status_ok)
		ErrorMessageBox("%s file load fail (%s)", path, res.description());

	this->m_skill_data_by_class.clear();

	for (auto skill_list = file.child("ComboSkill").child("Class"); skill_list; skill_list = skill_list.next_sibling())
	{
		unsigned __int8 index = skill_list.attribute("index").as_int();

		skill_data info;

		info.check_quest = skill_list.attribute("check_quest").as_int();

		info.m_first_skill.clear();
		info.m_second_skill.clear();

		for (auto list_first = skill_list.child("SkillListFirst").child("Skill"); list_first; list_first = list_first.next_sibling())
		{
			info.m_first_skill.push_back(list_first.attribute("index").as_int());
		}

		for (auto list_second = skill_list.child("SkillListSecond").child("Skill"); list_second; list_second = list_second.next_sibling())
		{
			info.m_second_skill.push_back(list_second.attribute("index").as_int());
		}

		this->m_skill_data_by_class.insert(std::pair<unsigned __int8, skill_data>(index,info));
	}
}
