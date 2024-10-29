// ComboSkill.h: interface for the CComboSkill class.
//
//////////////////////////////////////////////////////////////////////

#pragma once
//#include "ComboSkillData.h"

class CComboSkill/* : CComboSkillData*/
{
public:
	void Init();
	static int GetSkillType(WORD skill, int character_class);
	bool CheckCombo(WORD skill, int aIndex);
public:
	DWORD m_time;
	WORD m_skill[2];
	int m_index;
};
