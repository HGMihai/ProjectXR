#include "stdafx.h"
#include "OptionReplace.h"
#include "MemScript.h"
#include "Util.h"
#include "Protection.h"

COptionReplace gOptionReplace;

COptionReplace::COptionReplace()
{
	this->m_OptionReplaceData.clear();
}


void COptionReplace::Load(char* path)
{
	if(!gProtection.GetCustomState(CUSTOM_OPTION_REPLACE))
	{
		return;
	}
	
	CMemScript* lpMemScript = new CMemScript;

	if(lpMemScript == nullptr)
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

	this->m_OptionReplaceData.clear();

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

			OPTION_REPLACE_INFO info;

			memset(&info,0,sizeof(info));

			info.ItemIndex = lpMemScript->GetNumber();

			info.OptionIndex = lpMemScript->GetAsNumber();

			info.ClientOptionIndex = lpMemScript->GetAsNumber();

			info.NewOptionIndex = lpMemScript->GetAsNumber();

			info.NewOptionValue = lpMemScript->GetAsNumber();

			this->m_OptionReplaceData.emplace_back(info);
		}
	}
	catch(...)
	{
		ErrorMessageBox(lpMemScript->GetLastError());
	}

	delete lpMemScript;
}

bool COptionReplace::IsItemOptionReplace(int itemIndex, int optionIndex)
{
	if(!gProtection.GetCustomState(CUSTOM_OPTION_REPLACE))
	{
		return false;
	}
	
	for (auto it=this->m_OptionReplaceData.begin();it!=this->m_OptionReplaceData.end();++it)
	{
		if (it->ItemIndex == itemIndex && it->OptionIndex == optionIndex)
		{
			return true;
		}
	}

	return false;
}

void COptionReplace::DoItemOptionReplace(int optionIndex, int index, CItem* lpItem)
{
	if(!gProtection.GetCustomState(CUSTOM_OPTION_REPLACE))
	{
		return;
	}
	
	for (auto it=this->m_OptionReplaceData.begin();it!=this->m_OptionReplaceData.end();++it)
	{
		if (it->ItemIndex == lpItem->m_Index && it->OptionIndex == optionIndex)
		{
			lpItem->m_SpecialIndex[index] = it->NewOptionIndex;

			lpItem->m_SpecialValue[index] = it->NewOptionValue;
		}
	}
}
