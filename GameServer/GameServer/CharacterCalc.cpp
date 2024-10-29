#include "stdafx.h"
#include "CharacterCalc.h"
#include "User.h"
#include "Util.h"

CCharacterCalc gCharacterCalc;

void CCharacterCalc::Load(char* path)
{
	this->is_enabled = false;

	//if(this->is_enabled)
	if(!this->m_Lua.DoFile(path))
		ExitProcess(0);

	this->m_Lua.Generic_Call("is_enabled",">i",&this->is_enabled);
}

int CCharacterCalc::calculate_damage_rate_pvp(int aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];
	
	int value = 0;
	
	this->m_Lua.Generic_Call("damage_rate_pvp","iiiiiiiii>i",lpObj->Class,lpObj->Strength,lpObj->Dexterity,lpObj->Vitality,lpObj->Energy, lpObj->Leadership,
		lpObj->Level, lpObj->Reset, lpObj->MasterReset,&value);

	return value;
}

int CCharacterCalc::calculate_damage_rate_pvm(int aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];
	
	int value = 0;
	
	this->m_Lua.Generic_Call("damage_rate_pvm","iiiiiiiii>i",lpObj->Class,lpObj->Strength,lpObj->Dexterity,lpObj->Vitality,lpObj->Energy, lpObj->Leadership,
		lpObj->Level, lpObj->Reset, lpObj->MasterReset,&value);

	return value;
}

int CCharacterCalc::calculate_damage_class_vs_class(int aIndex1, int aIndex2)
{
	LPOBJ lpObj1 = &gObj[aIndex1];
	LPOBJ lpObj2 = &gObj[aIndex2];

	int value = 0;

	this->m_Lua.Generic_Call("damage_rate_class_vs_class","iiiiiiiiiiiiiiiiii>i",lpObj1->Class,lpObj1->Strength,lpObj1->Dexterity,lpObj1->Vitality,lpObj1->Energy, lpObj1->Leadership,
		lpObj1->Level, lpObj1->Reset, lpObj1->MasterReset,lpObj2->Class,lpObj2->Strength,lpObj2->Dexterity,lpObj2->Vitality,lpObj2->Energy, lpObj2->Leadership,
		lpObj2->Level, lpObj2->Reset, lpObj2->MasterReset,&value);

	return value;
}

void CCharacterCalc::calculate_attack_speed(int aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];

	int physic_speed = 0;
	int magic_speed = 0;

	this->m_Lua.Generic_Call("attack_speed","iiiiiiiii>ii",lpObj->Class,lpObj->Strength,lpObj->Dexterity,lpObj->Vitality,lpObj->Energy, lpObj->Leadership,
		lpObj->Level, lpObj->Reset, lpObj->MasterReset,&physic_speed,&magic_speed);

	lpObj->PhysiSpeed = physic_speed;
	lpObj->MagicSpeed = magic_speed;
}

int CCharacterCalc::calculate_dk_damage_multiplier_rate(int aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];
	
	int value = 0;
	
	this->m_Lua.Generic_Call("dk_damage_multiplier_rate","iiiiiiiii>i",lpObj->Class,lpObj->Strength,lpObj->Dexterity,lpObj->Vitality,lpObj->Energy, lpObj->Leadership,
		lpObj->Level, lpObj->Reset, lpObj->MasterReset,&value);
	
	return value;
}

int CCharacterCalc::calculate_dl_damage_multiplier_rate(int aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];
	
	int value = 0;
	
	this->m_Lua.Generic_Call("dl_damage_multiplier_rate","iiiiiiiii>i",lpObj->Class,lpObj->Strength,lpObj->Dexterity,lpObj->Vitality,lpObj->Energy, lpObj->Leadership,
		lpObj->Level, lpObj->Reset, lpObj->MasterReset,&value);
	
	return value;
}

int CCharacterCalc::calculate_rf_damage_multiplier_rate1(int aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];
	
	int value = 0;
	
	this->m_Lua.Generic_Call("rf_damage_multiplier_rate_first","iiiiiiiii>i",lpObj->Class,lpObj->Strength,lpObj->Dexterity,lpObj->Vitality,lpObj->Energy, lpObj->Leadership,
		lpObj->Level, lpObj->Reset, lpObj->MasterReset,&value);

	return value;
}

int CCharacterCalc::calculate_rf_damage_multiplier_rate2(int aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];
	
	int value = 0;
	
	this->m_Lua.Generic_Call("rf_damage_multiplier_rate_second","iiiiiiiii>i",lpObj->Class,lpObj->Strength,lpObj->Dexterity,lpObj->Vitality,lpObj->Energy, lpObj->Leadership,
		lpObj->Level, lpObj->Reset, lpObj->MasterReset,&value);

	return value;
}

int CCharacterCalc::calculate_rf_damage_multiplier_rate3(int aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];
	
	int value = 0;
	
	this->m_Lua.Generic_Call("rf_damage_multiplier_rate_third","iiiiiiiii>i",lpObj->Class,lpObj->Strength,lpObj->Dexterity,lpObj->Vitality,lpObj->Energy, lpObj->Leadership,
		lpObj->Level, lpObj->Reset, lpObj->MasterReset,&value);
	
	return value;
}

int CCharacterCalc::calculate_defense(int aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];
	
	int value = 0;
	
	this->m_Lua.Generic_Call("calc_defense","iiiiiiiii>i",lpObj->Class,lpObj->Strength,lpObj->Dexterity,lpObj->Vitality,lpObj->Energy, lpObj->Leadership,
		lpObj->Level, lpObj->Reset, lpObj->MasterReset,&value);
	
	return value;
}
