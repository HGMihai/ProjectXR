// CustomPet.h: interface for the CCustomPet class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "Item.h"
#include "User.h"

#define MAX_CUSTOM_PET 500

struct CUSTOM_PETS_INFO
{
	int Index;
	int ItemIndex;
	//int IncExpRate;
	//int IncMasterExpRate;
	int IncDamageRate;
	int IncLife;
	int IncMana;
	int IncAttackSpeed;
	int IncDoubleDamageRate;
	int CanRestoreHP;
	//char ModelName[32];
};

class CCustomPets
{
public:
	CCustomPets();
	virtual ~CCustomPets();
	void Init();
	void Load(char* path);
	void SetInfo(CUSTOM_PETS_INFO info);
	CUSTOM_PETS_INFO* GetInfo(int index);
	CUSTOM_PETS_INFO* GetInfoByItem(int ItemIndex);
	bool CheckCustomPet(int index);
	bool CheckCustomPetByItem(int ItemIndex);
	bool CheckRestoreHPByItem(int ItemIndex);
	int GetCustomPetIndex(int ItemIndex);

	//int GetCustomPetExpRate(int ItemIndex);
	//int GetCustomPetMasterExpRate(int ItemIndex);
	int GetCustomPetDamageRate(int ItemIndex);
	int GetCustomPetLife(int ItemIndex);
	int GetCustomPetMana(int ItemIndex);
	int GetCustomPetAttackSpeed(int ItemIndex);
	int GetCustomPetDoubleDamageRate(int ItemIndex);
	void CalcCustomPetOption(LPOBJ lpObj,bool flag);
public:
	CUSTOM_PETS_INFO m_CustomPetsInfo[MAX_CUSTOM_PET];
};

extern CCustomPets gCustomPets;
