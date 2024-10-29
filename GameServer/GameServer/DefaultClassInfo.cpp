// DefaultClassInfo.cpp: implementation of the CDefaultClassInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DefaultClassInfo.h"
#include "MemScript.h"
#include "Util.h"

CDefaultClassInfo gDefaultClassInfo;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDefaultClassInfo::CDefaultClassInfo() // OK
{
	this->Init();
}

CDefaultClassInfo::~CDefaultClassInfo() // OK
{

}

void CDefaultClassInfo::Init() // OK
{
	memset(this->m_DefaultClassInfo,0,sizeof(this->m_DefaultClassInfo));
}

void CDefaultClassInfo::Load(char* path)
{
	pugi::xml_document file;
	pugi::xml_parse_result res = file.load_file(path);

	if(res.status != pugi::status_ok)
	{
		ErrorMessageBox("%s file load fail (%s)", path, res.description());
	}
	
	this->Init();
	pugi::xml_node DefClassInfo = file.child("DefaultClassInfo");
	for (pugi::xml_node Class = DefClassInfo.child("Class"); Class; Class = Class.next_sibling())
	{
		DEFAULT_CLASS_INFO info;

		info.Class = Class.attribute("Index").as_int();

		info.Strength = Class.attribute("Strength").as_int();

		info.Dexterity = Class.attribute("Dexterity").as_int();

		info.Vitality = Class.attribute("Vitality").as_int();

		info.Energy = Class.attribute("Energy").as_int();

		info.Leadership = Class.attribute("Leadership").as_int();

		info.NewStat = Class.attribute("NewStat").as_int();

		info.MaxLife = Class.attribute("MaxLife").as_float();

		info.MaxMana = Class.attribute("MaxMana").as_float();

		info.LevelLife = Class.attribute("LevelLife").as_float();

		info.LevelMana = Class.attribute("LevelMana").as_float();

		info.VitalityToLife = Class.attribute("VitalityToLife").as_float();

		info.VitalityToShield = Class.attribute("VitalityToShield").as_float(1.0);

		info.EnergyToMana = Class.attribute("EnergyToMana").as_float();

		info.StartGate = Class.attribute("StartGate").as_int();

		this->SetInfo(info);
	}
}

void CDefaultClassInfo::SetInfo(DEFAULT_CLASS_INFO info) // OK
{
	if(CHECK_RANGE(info.Class,MAX_CLASS) == 0)
	{
		return;
	}

	this->m_DefaultClassInfo[info.Class] = info;
}

int CDefaultClassInfo::GetCharacterDefaultStat(int Class,int stat) // OK
{
	if(CHECK_RANGE(Class,MAX_CLASS) == 0)
	{
		return 0;
	}

	switch(stat)
	{
		case 0:
			return this->m_DefaultClassInfo[Class].Strength;
		case 1:
			return this->m_DefaultClassInfo[Class].Dexterity;
		case 2:
			return this->m_DefaultClassInfo[Class].Vitality;
		case 3:
			return this->m_DefaultClassInfo[Class].Energy;
		case 4:
			return this->m_DefaultClassInfo[Class].Leadership;
	}

	return 0;
}
