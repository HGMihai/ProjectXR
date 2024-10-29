#include "stdafx.h"
#include "SuperPoints.h"
#include "MemScript.h"
#include "Util.h"
#include "ItemManager.h"
#include "DSProtocol.h"
#include "ObjectManager.h"
#include "Notice.h"
#include "Log.h"

cSuperPoints gSuperPoints;

cSuperPoints::cSuperPoints()
{
	this->m_SuperPointsInfo.clear();
	this->m_NpcData.NPC_Id = 0;
	this->m_NpcData.NPC_Map = 0;
	this->m_NpcData.NPC_X = 0;
	this->m_NpcData.NPC_Y = 0;
}

cSuperPoints::~cSuperPoints()
{
	
}

void cSuperPoints::Load(char* path)
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

	this->m_SuperPointsInfo.clear();

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
				if (section == 0)
				{
					if(strcmp("end",lpMemScript->GetAsString()) == 0)
					{
						break;
					}

					this->m_NpcData.NPC_Id = lpMemScript->GetNumber();
					this->m_NpcData.NPC_Map = lpMemScript->GetAsNumber();
					this->m_NpcData.NPC_X = lpMemScript->GetAsNumber();
					this->m_NpcData.NPC_Y = lpMemScript->GetAsNumber();
				}
				else if (section == 1)
				{
					if(strcmp("end",lpMemScript->GetAsString()) == 0)
					{
						break;
					}

					SUPERPOINTS_INFO info;

					info.ItemID = lpMemScript->GetNumber();

					info.ItemLevel = lpMemScript->GetAsNumber();

					info.Points = lpMemScript->GetAsNumber();

					this->m_SuperPointsInfo.push_back(info);
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

bool cSuperPoints::Dialog(LPOBJ lpObj, LPOBJ lpNpc)
{
	if( lpNpc->Class	== this->m_NpcData.NPC_Id 
		&&	lpNpc->Map	== this->m_NpcData.NPC_Map
		&&	lpNpc->X	== this->m_NpcData.NPC_X
		&&	lpNpc->Y	== this->m_NpcData.NPC_Y )
	{
		this->CheckItem(lpObj);
		return true;
	}
	return false;
}

void cSuperPoints::CheckItem(LPOBJ lpObj)
{
	lpObj->ChaosLock = 1;

	std::vector<SUPERPOINTS_INFO>::iterator it = this->m_SuperPointsInfo.begin();

	for(; it != this->m_SuperPointsInfo.end(); it++)
	{
		if (gItemManager.GetInventoryItemCount(lpObj,it->ItemID, it->ItemLevel) > 0)
		{
			gItemManager.DeleteInventoryItemCount(lpObj,it->ItemID, it->ItemLevel,1);

			lpObj->iQuestStat+=it->Points;
			lpObj->LevelUpPoint+=it->Points;

			gObjectManager.CharacterCalcAttribute(lpObj->Index);

			GCNewCharacterInfoSend(lpObj);

			GDCharacterInfoSaveSend(lpObj->Index);

			gLog.Output(LOG_SUPER_POINTS,"[SuperPoints]: [%s] [%s] Obtained %d (total %d) points for change item id %d level %d",lpObj->Account,lpObj->Name, it->Points,lpObj->iQuestStat,it->ItemID,it->ItemLevel);

			lpObj->ChaosLock = 0;

			return;
		}
	}
	
	gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, "You dont have enough items");

	lpObj->ChaosLock = 0;
}