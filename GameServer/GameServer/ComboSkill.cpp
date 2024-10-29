// ComboSkill.cpp: implementation of the CComboSkill class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ComboSkill.h"
#include "ComboSkillData.h"
#include "User.h"
#include "Quest.h"
#include "SkillManager.h"
#include "ServerDisplayer.h"
#include "Util.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

void CComboSkill::Init() // OK
{
	this->m_time = 0;
	this->m_skill[0] = 0xFFFF;
	this->m_skill[1] = 0xFFFF;
	this->m_index = -1;
}

int CComboSkill::GetSkillType(WORD skill, int character_class) // OK
{
	auto data = gComboSkillData.m_skill_data_by_class.find(character_class);

	if(data == gComboSkillData.m_skill_data_by_class.end())
		return -1;

	for(int i=0; i < data->second.m_first_skill.size(); i++)
	{
		if(skill == data->second.m_first_skill[i])
		{		
			return 0;
		}
	}

	for(int i=0; i < data->second.m_second_skill.size(); i++)
	{	
		if(skill == data->second.m_second_skill[i])
		{					
			return 1;
		}
	}
	
	/*if(skill == SKILL_FALLING_SLASH || skill == SKILL_LUNGE || skill == SKILL_UPPERCUT || skill == SKILL_CYCLONE || skill == SKILL_SLASH)
	{
		return 0;
	}
	
	if(skill == SKILL_TWISTING_SLASH || skill == SKILL_RAGEFUL_BLOW || skill == SKILL_DEATH_STAB || skill == SKILL_FROZEN_STAB || skill == SKILL_BLOOD_STORM)
	{
		return 1;
	}*/
	
	return -1;
}

bool CComboSkill::CheckCombo(WORD skill, int aIndex) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	auto data = gComboSkillData.m_skill_data_by_class.find(lpObj->Class);

	if(data == gComboSkillData.m_skill_data_by_class.end())
		return false;

	if(lpObj->Class == CLASS_DK)
		if(gQuest.CheckQuestListState(lpObj,/*data->second.check_quest*/3,QUEST_FINISH) == 0)
			return false;
	
	int type = this->GetSkillType(skill,lpObj->Class);

	if(type == -1)
	{
		this->Init();
		return false;
	}

	if(type == 0)
	{
		this->m_time = GetTickCount()+3000;
		this->m_skill[0] = skill;
		this->m_index = 0;
		return false;
	}

	if(type == 1)
	{
		if(this->m_time < GetTickCount())
		{
			this->Init();
			return false;
		}

		if(this->m_skill[0] == 0xFFFF)
		{
			this->Init();
			return false;
		}

		if(this->m_index == 0)
		{
			this->m_time = GetTickCount()+3000;
			this->m_skill[1] = skill;
			this->m_index = 1;
			return false;
		}
		
		if(this->m_index == 1 && this->m_skill[1] != skill)
		{
			this->Init();
			return true;
		}
	}

	this->Init();
	return false;
}
