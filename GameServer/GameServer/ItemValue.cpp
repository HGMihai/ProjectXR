// ItemValue.cpp: implementation of the CItemValue class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ItemValue.h"
#include "ItemManager.h"
#include "ItemStack.h"
#include "MemScript.h"
#include "Util.h"

CItemValue gItemValue;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CItemValue::CItemValue() // OK
{
	this->m_ItemValueInfo.clear();
}

CItemValue::~CItemValue() // OK
{

}

void CItemValue::Load(char* path) // OK
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

	this->m_ItemValueInfo.clear();

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

			ITEM_VALUE_INFO info;

			info.Index = lpMemScript->GetNumber();

			info.Level = lpMemScript->GetAsNumber();

			info.Grade = lpMemScript->GetAsNumber();

			info.AncOption = lpMemScript->GetAsNumber();

			info.OptionEx = lpMemScript->GetAsNumber();

			info.Type = lpMemScript->GetAsNumber();

			info.BuyPrice = lpMemScript->GetAsNumber();

			info.SellPrice = lpMemScript->GetAsNumber();

			if (info.OptionEx > 1)
			{
				info.OptionEx = 1;
			}

			this->m_ItemValueInfo.push_back(info);
			//this->m_ItemValueInfo.insert(std::pair<int, ITEM_VALUE_INFO>(info.Index,info));
		}
	}
	catch(...)
	{
		ErrorMessageBox(lpMemScript->GetLastError());
	}

	delete lpMemScript;
}

bool CItemValue::GetItemValue(CItem* lpItem, int* type, int* BuyPrice, int* SellPrice) // OK
{	
	for(auto it=this->m_ItemValueInfo.begin();it != this->m_ItemValueInfo.end();++it)
	{
		if(it->Index == lpItem->m_Index)
		{
			if(it->Level == -1 || it->Level == lpItem->m_Level)
			{
				if(it->Grade == -1 || it->Grade == lpItem->m_NewOption)
				{
					if (it->AncOption == -1 || it->AncOption == lpItem->m_SetOption)
					{
						if (it->OptionEx == -1 || it->OptionEx == lpItem->Is380Item())
						{
							if(gItemStack.GetItemMaxStack(it->Index, it->Level) == 0 || it->Index == GET_ITEM(4,7) || it->Index == GET_ITEM(4,15))
							{
								(*type) = it->Type;
								(*BuyPrice) = it->BuyPrice;
								(*SellPrice) = it->SellPrice;
								return true;
							}
							else
							{
								(*type) = 0;
								(*BuyPrice) = (int)(it->BuyPrice*lpItem->m_Durability);
								(*SellPrice) = (int)(it->SellPrice*lpItem->m_Durability);
								return true;
							}
						}
					}
				}
			}
		}
	}

	return false;
}

void CItemValue::GCItemValueSend(int aIndex, std::vector<ITEM_VALUE_INFO>::iterator it)
{
	BYTE send[8192];

	PMSG_SEND_ITEM_VALUE pMsg{};

	pMsg.header.set(0xFB, 0xA0, 0);

	int size = sizeof(pMsg);

	pMsg.count = 0;

	if (it == this->m_ItemValueInfo.begin())
		pMsg.is_first = true;
	else
		pMsg.is_first = false;
	
	for (; it != this->m_ItemValueInfo.end(); ++it)
	{
		ITEM_VALUE_INFO info = (ITEM_VALUE_INFO)*it;
		memcpy(&send[size], &info, sizeof(info));
		size += sizeof(info);
		pMsg.count++;
		
		if (pMsg.count >= 250)
		{
			if (it < this->m_ItemValueInfo.end())
				++it;
			
			break;
		}
	}

	pMsg.header.size[0] = SET_NUMBERHB(size);

	pMsg.header.size[1] = SET_NUMBERLB(size);

	memcpy(send, &pMsg, sizeof(pMsg));
	DataSend(aIndex, send, size);

	if(it != this->m_ItemValueInfo.end())
		this->GCItemValueSend(aIndex, it);
}

std::vector<ITEM_VALUE_INFO>::iterator CItemValue::GetBeginIterator()
{
	return this->m_ItemValueInfo.begin();
}