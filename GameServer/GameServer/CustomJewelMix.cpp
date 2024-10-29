#include "stdafx.h"
#include "CustomJewelMix.h"
#include "MemScript.h"
#include "Util.h"

CCustomJewelMix gCustomJewelMix;

CCustomJewelMix::CCustomJewelMix()
{
	this->m_JewelMixInfo.clear();
}

CCustomJewelMix::~CCustomJewelMix()
{
	
}

void CCustomJewelMix::Load(char* path)
{
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

	this->m_JewelMixInfo.clear();

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

			JEWEL_MIX_DATA info;

			info.JewelID = lpMemScript->GetNumber();
			info.JewelBoundleID = lpMemScript->GetAsNumber();
			strncpy(info.Text, lpMemScript->GetAsString(), sizeof(info.Text));
			this->m_JewelMixInfo.push_back(info);
		}
	}
	catch(...)
	{
		ErrorMessageBox(lpMemScript->GetLastError());
	}

	delete lpMemScript;
}

