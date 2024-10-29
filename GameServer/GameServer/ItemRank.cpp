#include "StdAfx.h"
#if GAMESERVER_UPDATE == 603
#include "ItemRank.h"
#include "MemScript.h"
#include "Util.h"
#include "ItemManager.h"
// -------------------------------------------------------------------------------

ItemRank gItemRank;
// -------------------------------------------------------------------------------

ItemRank::ItemRank()
{
	this->m_Data.clear();
}
// -------------------------------------------------------------------------------

ItemRank::~ItemRank()
{

}
// -------------------------------------------------------------------------------

void ItemRank::Load(char* path) // OK
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

	this->m_Data.clear();

	try
	{
		while(true)
		{
			if(lpMemScript->GetToken() == TOKEN_END)
			{
				break;
			}

			int section = lpMemScript->GetNumber();

			while(true)
			{

				if (section == 0)// RankList
				{
					if(strcmp("end",lpMemScript->GetAsString()) == 0)
					{
						break;
					}

					ItemRank_Data lpData;
					// ----
					lpData.RankID = lpMemScript->GetNumber();
					// ----
					lpData.OptionValue[0] = lpMemScript->GetAsNumber();
					// ----
					lpData.OptionValue[1] = lpMemScript->GetAsNumber();
					// ----
					lpData.OptionValue[2] = lpMemScript->GetAsNumber();
					// ----
					lpData.OptionValue[3] = lpMemScript->GetAsNumber();
					// ----
					lpData.OptionValue[4] = lpMemScript->GetAsNumber();
					// ----
					lpData.OptionValue[5] = lpMemScript->GetAsNumber();
					// ----
					this->m_Data.push_back(lpData);
				}
				else if (section == 1)
				{
					if(strcmp("end",lpMemScript->GetAsString()) == 0)
					{
						break;
					}

					ITEMRANK_LIST lpData;
					WORD ItemType = -1, ItemIndex = -1;
					// ----
					lpData.RankID = lpMemScript->GetNumber();
					// ----
					ItemType = lpMemScript->GetAsNumber();
					// ----
					ItemIndex = lpMemScript->GetAsNumber();
					// ----
					lpData.ItemType = GET_ITEM(ItemType, ItemIndex);
					// ----
					short Index = this->GetDataIndex(lpData.RankID);
					// ----
					if( Index == -1 )
					{
						ErrorMessageBox("[ItemRank] Invalid RankID (Item: %d/%d - RankID: %d)", 
							ItemType, ItemIndex, lpData.RankID);
					}
					// ----
					this->m_Data[Index].ItemList.push_back(lpData);
				}
				else
				{
					break;
				}
			}	
		}
	}
	catch(...)
	{
		ErrorMessageBox(lpMemScript->GetLastError());
	}

	delete lpMemScript;
}
// -------------------------------------------------------------------------------

bool ItemRank::IsRankItem(WORD ItemType)
{
	for( int i = 0; i < this->m_Data.size(); i++ )
	{
		for( int j = 0; j < this->m_Data[i].ItemList.size(); j++ )
		{
			if( this->m_Data[i].ItemList[j].ItemType == ItemType )
			{
				return true;
			}
		}
	}
	// ----
	return false;
}
// -------------------------------------------------------------------------------

short ItemRank::GetValue(WORD ItemType, BYTE OptionType)
{
	for( int i = 0; i < this->m_Data.size(); i++ )
	{
		for( int j = 0; j < this->m_Data[i].ItemList.size(); j++ )
		{
			if( this->m_Data[i].ItemList[j].ItemType == ItemType )
			{
				return this->m_Data[i].OptionValue[OptionType];
			}
		}
	}
	// ----
	return -1;
}
// -------------------------------------------------------------------------------

short ItemRank::GetDataIndex(BYTE RankID)
{
	for( int i = 0; i < this->m_Data.size(); i++ )
	{
		if( this->m_Data[i].RankID == RankID )
		{
			return i;
		}
	}
	// ----
	return -1;
}
// -------------------------------------------------------------------------------

void ItemRank::CalcItemRank(LPOBJ lpObj, bool flag)
{
	for(int n=0;n < INVENTORY_WEAR_SIZE;n++)
	{
		if(lpObj->Inventory[n].IsItem() == 0 || lpObj->Inventory[n].m_IsValidItem == 0 || lpObj->Inventory[n].IsExcItem() == 0)
		{
			continue;
		}

		CItem* Item = &lpObj->Inventory[n];
		int Value = 0;

		if ((Item->m_NewOption & 0x20) == 0x20 )
		{
			Value = this->GetValue(Item->m_Index, 0);
		}
	}
}
// -------------------------------------------------------------------------------

void ItemRank::SetItemRank(LPOBJ lpObj, CItem* lpItem)
{
	
}

#endif