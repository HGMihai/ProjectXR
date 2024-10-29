#pragma once

class CComboSkillData
{
public:
	struct skill_data
	{
		unsigned __int8 check_quest;
		std::vector<int> m_first_skill;
		std::vector<int> m_second_skill;
	};

	CComboSkillData();
	~CComboSkillData();

	//static CComboSkillData* GetInstance();
	
	void load(char* path);

	// ----

	std::map<unsigned __int8, skill_data> m_skill_data_by_class;
}; extern CComboSkillData gComboSkillData;