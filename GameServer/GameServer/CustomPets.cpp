// CustomPet.cpp: implementation of the CCustomPets class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CustomPets.h"
#include "MemScript.h"
#include "Util.h"
#include "Protection.h"

CCustomPets gCustomPets;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCustomPets::CCustomPets() // OK
{
	this->Init();
}

CCustomPets::~CCustomPets() // OK
{

}

void CCustomPets::Init() // OK
{
	//MsgBox("State = %d",gProtection.GetCustomState(CUSTOM_PETS));
	if (gProtection.GetCustomState(CUSTOM_PETS) == 0)
	{
		return;
	}

	for(int n=0;n < MAX_CUSTOM_PET;n++)
	{
		this->m_CustomPetsInfo[n].Index = -1;
	}
}

void CCustomPets::Load(char* path) // OK
{
	if (gProtection.GetCustomState(CUSTOM_PETS) == 0)
	{
		return;
	}

	CMemScript* lpMemScript = new CMemScript;

	if(lpMemScript == 0)
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

	static int Index = 0;

	this->Init();

	try
	{
		while(true)
		{
			if(lpMemScript->GetToken() == TOKEN_END)
			{
				break;
			}

			if(strcmp("end",lpMemScript->GetString()) == 0)
			{
				break;
			}

			CUSTOM_PETS_INFO info;

			memset(&info,0,sizeof(info));

			//info.Index = lpMemScript->GetNumber();
			

			info.Index = Index++;

			info.ItemIndex = lpMemScript->GetNumber();

			info.IncLife = lpMemScript->GetAsNumber();

			info.IncMana = lpMemScript->GetAsNumber();

			info.IncDamageRate = lpMemScript->GetAsNumber();

			info.IncAttackSpeed = lpMemScript->GetAsNumber();

			info.IncDoubleDamageRate = lpMemScript->GetAsNumber();

			info.CanRestoreHP = lpMemScript->GetAsNumber();

			this->SetInfo(info);
		}
	}
	catch(...)
	{
		ErrorMessageBox(lpMemScript->GetLastError());
	}

	Index = 0;

	delete lpMemScript;
}

void CCustomPets::SetInfo(CUSTOM_PETS_INFO info) // OK
{
	if(info.Index < 0 || info.Index >= MAX_CUSTOM_PET)
	{
		return;
	}

	this->m_CustomPetsInfo[info.Index] = info;
}

CUSTOM_PETS_INFO* CCustomPets::GetInfo(int index) // OK
{
	if(index < 0 || index >= MAX_CUSTOM_PET)
	{
		return 0;
	}

	if(this->m_CustomPetsInfo[index].Index != index)
	{
		return 0;
	}

	return &this->m_CustomPetsInfo[index];
}

CUSTOM_PETS_INFO* CCustomPets::GetInfoByItem(int ItemIndex) // OK
{
	if (gProtection.GetCustomState(CUSTOM_PETS) == 0)
	{
		return 0;

	}
	for(int n=0;n < MAX_CUSTOM_PET;n++)
	{
		CUSTOM_PETS_INFO* lpInfo = this->GetInfo(n);

		if(lpInfo == 0)
		{
			continue;
		}

		if(lpInfo->ItemIndex == ItemIndex)
		{
			return lpInfo;
		}
	}

	return 0;
}

bool CCustomPets::CheckCustomPet(int index) // OK
{
	if (gProtection.GetCustomState(CUSTOM_PETS) == 0)
	{
		return 0;
	}

	if(this->GetInfo(index) != 0)
	{
		return 1;
	}

	return 0;
}

bool CCustomPets::CheckCustomPetByItem(int ItemIndex) // OK
{
	if (gProtection.GetCustomState(CUSTOM_PETS) == 0)
	{
		return 0;
	}

	if(this->GetInfoByItem(ItemIndex) != 0)
	{
		return 1;
	}

	return 0;
}

bool CCustomPets::CheckRestoreHPByItem(int ItemIndex)
{
	if (gProtection.GetCustomState(CUSTOM_PETS) == 0)
	{
		return 0;
	}

	CUSTOM_PETS_INFO* lpInfo = this->GetInfoByItem(ItemIndex);

	if(lpInfo != 0)
	{
		if (lpInfo->CanRestoreHP)
		{
			return 1;
		}
	}

	return 0;
}


int CCustomPets::GetCustomPetIndex(int ItemIndex) // OK
{
	if (gProtection.GetCustomState(CUSTOM_PETS) == 0)
	{
		return 0;
	}

	CUSTOM_PETS_INFO* lpInfo = this->GetInfoByItem(ItemIndex);

	if(lpInfo == 0)
	{
		return 0;
	}

	return lpInfo->Index;
}

//int CCustomPets::GetCustomPetExpRate(int ItemIndex)
//{
//	CUSTOM_PETS_INFO* lpInfo = this->GetInfoByItem(ItemIndex);
//
//	if(lpInfo == 0)
//	{
//		return 0;
//	}
//
//	return lpInfo->IncExpRate;
//}
//
//int CCustomPets::GetCustomPetMasterExpRate(int ItemIndex)
//{
//	CUSTOM_PETS_INFO* lpInfo = this->GetInfoByItem(ItemIndex);
//
//	if(lpInfo == 0)
//	{
//		return 0;
//	}
//
//	return lpInfo->IncMasterExpRate;
//}

int CCustomPets::GetCustomPetDamageRate(int ItemIndex) // OK
{
	CUSTOM_PETS_INFO* lpInfo = this->GetInfoByItem(ItemIndex);

	if(lpInfo == 0)
	{
		return 0;
	}

	return (lpInfo->IncDamageRate);
}

int CCustomPets::GetCustomPetLife(int ItemIndex) // OK
{
	CUSTOM_PETS_INFO* lpInfo = this->GetInfoByItem(ItemIndex) ;

	if(lpInfo == 0)
	{
		return 0;
	}

	return (lpInfo->IncLife);
}

int CCustomPets::GetCustomPetMana(int ItemIndex) // OK
{
	CUSTOM_PETS_INFO* lpInfo = this->GetInfoByItem(ItemIndex);

	if(lpInfo == 0)
	{
		return 0;
	}

	return (lpInfo->IncMana);
}

int CCustomPets::GetCustomPetAttackSpeed(int ItemIndex) // OK
{
	CUSTOM_PETS_INFO* lpInfo = this->GetInfoByItem(ItemIndex);

	if(lpInfo == 0)
	{
		return 0;
	}

	return (lpInfo->IncAttackSpeed);
}
int CCustomPets::GetCustomPetDoubleDamageRate(int ItemIndex) // OK
{
	CUSTOM_PETS_INFO* lpInfo = this->GetInfoByItem(ItemIndex);

	if(lpInfo == 0)
	{
		return 0;
	}

	return (lpInfo->IncDoubleDamageRate);
}

void CCustomPets::CalcCustomPetOption(LPOBJ lpObj,bool flag)
{
	if (gProtection.GetCustomState(CUSTOM_PETS) == 0)
	{
		return;
	}

	if(flag != 0)
	{
		return;
	}
	
	CItem* Helper = &lpObj->Inventory[8];

	if(this->CheckCustomPetByItem(Helper->m_Index) != 0)
	{
		//lpObj->EffectOption.AddExperienceRate += GetCustomPetExpRate(Helper->m_Index);
		//lpObj->EffectOption.AddMasterExperienceRate += GetCustomPetMasterExpRate(Helper->m_Index);
		lpObj->AddLife += GetCustomPetLife(Helper->m_Index);
		lpObj->AddMana += GetCustomPetMana(Helper->m_Index);
		lpObj->PhysiSpeed += GetCustomPetAttackSpeed(Helper->m_Index);
		lpObj->MagicSpeed += GetCustomPetAttackSpeed(Helper->m_Index);
		lpObj->DoubleDamageRate += GetCustomPetDoubleDamageRate(Helper->m_Index);
	}
}