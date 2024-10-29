#pragma once

#include "MuLua.h"

class CCharacterCalc
{
public:
	void Load(char* path);
	int calculate_damage_rate_pvp(int aIndex);
	int calculate_damage_rate_pvm(int aIndex);
	int calculate_damage_class_vs_class(int aIndex1, int aIndex2);
	void calculate_attack_speed(int aIndex);
	int calculate_dk_damage_multiplier_rate(int aIndex);
	int calculate_dl_damage_multiplier_rate(int aIndex);
	int calculate_rf_damage_multiplier_rate1(int aIndex);
	int calculate_rf_damage_multiplier_rate2(int aIndex);
	int calculate_rf_damage_multiplier_rate3(int aIndex);
	int calculate_defense(int aIndex);
	
	MULua m_Lua;
	bool is_enabled;
}; extern CCharacterCalc gCharacterCalc;