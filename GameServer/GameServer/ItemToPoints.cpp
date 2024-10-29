#include "stdafx.h"
#include "ItemToPoints.h"
#include "Util.h"
#include "ItemManager.h"
#include "DSProtocol.h"
#include "ObjectManager.h"
#include "Notice.h"
#include "Message.h"
#include "GrandResetSystem.h"
#include "ResetSystem.h"
#include "Log.h"

CItemToPoints gItemToPoints;

CItemToPoints::CItemToPoints()
{
	this->m_Enable = false;
	this->m_NpcID = -1;
	this->m_NpcMap = -1;
	this->m_NpcX = -1;
	this->m_NpcY = -1;
	this->m_Data.clear();
}

void CItemToPoints::Load(char* path)
{
	pugi::xml_document file;
	auto res = file.load_file(path);

	if(res.status != pugi::status_ok)
	{
		ErrorMessageBox("%s file load fail (%s)", path, res.description());
	}

	this->m_Data.clear();
	auto ItemToPoints = file.child("ItemToPoints");

	this->m_Enable = ItemToPoints.attribute("Enable").as_bool(false);
	this->m_NpcID = ItemToPoints.attribute("NpcID").as_int();
	this->m_NpcMap = ItemToPoints.attribute("NpcMap").as_int();
	this->m_NpcX = ItemToPoints.attribute("NpcX").as_int();
	this->m_NpcY = ItemToPoints.attribute("NpcY").as_int();

	for (auto item = ItemToPoints.child("Item"); item; item = item.next_sibling())
	{
		ItemToPointsData info;

		int type = item.attribute("Type").as_int();

		int id = item.attribute("ID").as_int();

		info.ItemID = GET_ITEM(type,id);

		info.ItemLevel = item.attribute("ItemLevel").as_int();

		info.Points = item.attribute("RewardPoints").as_int();

		info.Time = item.attribute("Time").as_int();

		this->m_Data.push_back(info);
	}
}

bool CItemToPoints::Dialog(LPOBJ lpObj, LPOBJ lpNpc)
{
	if( this->m_Enable
		&& lpNpc->Class	== this->m_NpcID
		&&	lpNpc->Map	== this->m_NpcMap
		&&	lpNpc->X	== this->m_NpcX
		&&	lpNpc->Y	== this->m_NpcY )
	{
		this->CheckItem(lpObj);
		return true;
	}
	return false;
}

void CItemToPoints::CheckItem(LPOBJ lpObj)
{
	lpObj->ChaosLock = 1;

	for(auto it = this->m_Data.begin(); it != this->m_Data.end(); ++it)
	{
		if (gItemManager.GetInventoryItemCount(lpObj,it->ItemID, it->ItemLevel) > 0)
		{
			gItemManager.DeleteInventoryItemCount(lpObj,it->ItemID, it->ItemLevel,1);

			lpObj->iQuestStat+=it->Points;
			lpObj->LevelUpPoint+=it->Points;

			//this->GDAddItem(lpObj, it->Points,it->Time);

			lpObj->PointsTime+=it->Time;
			lpObj->PointsCount+=it->Points;

			this->Update(lpObj);

			lpObj->ChaosLock = 0;

			gLog.Output(LOG_SUPER_POINTS,"[TemporaryPoints]: [%s] [%s] Obtained %d points (total %d) for %d min for change item id %d level %d",lpObj->Account,lpObj->Name, it->Points,lpObj->iQuestStat,it->Time,it->ItemID,it->ItemLevel);

			return;
		}
	}

	gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, "You dont have enough items");

	lpObj->ChaosLock = 0;
}

void CItemToPoints::Tick(LPOBJ lpObj)
{
	if(lpObj->PointsTime > 0)
	{
		lpObj->PointsTimeSec++;

		if(lpObj->PointsTimeSec > 60)
		{
			lpObj->PointsTimeSec = 0;
			lpObj->PointsTime--;

			if(lpObj->PointsTime < 1)
			{
				this->ResetStats(lpObj);
				lpObj->iQuestStat-=lpObj->PointsCount;
				lpObj->LevelUpPoint-=lpObj->PointsCount;

				gLog.Output(LOG_SUPER_POINTS,"[TemporaryPoints] [%s] [%s] Deleted %d points (total %d)",lpObj->Account,lpObj->Name,lpObj->PointsCount,lpObj->iQuestStat);

				lpObj->PointsTime = 0;
				lpObj->PointsCount = 0;

				this->Update(lpObj);
				gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(532));
			}
		}
	}
}

void CItemToPoints::Update(LPOBJ lpObj)
{
	gObjectManager.CharacterCalcAttribute(lpObj->Index);

	GCNewCharacterInfoSend(lpObj);

	GDCharacterInfoSaveSend(lpObj->Index);
}

void CItemToPoints::ResetStats(LPOBJ lpObj)
{
	int NewPoints = lpObj->Strength + lpObj->Dexterity + lpObj->Vitality + lpObj->Energy/* + lpObj->Leadership*/;

	if (lpObj->Class == CLASS_DL)
	{
#if (GAMESERVER_UPDATE == 603)
		NewPoints += lpObj->Leadership-((g_ResetSystem.m_BonusCommand*lpObj->Reset)+(gGRSystem.m_BonusCommand*lpObj->MasterReset));
#endif
	}

	lpObj->Strength	= gDefaultClassInfo.m_DefaultClassInfo[lpObj->Class].Strength;
	lpObj->Dexterity	= gDefaultClassInfo.m_DefaultClassInfo[lpObj->Class].Dexterity;
	lpObj->Energy		= gDefaultClassInfo.m_DefaultClassInfo[lpObj->Class].Energy;
	lpObj->Vitality	= gDefaultClassInfo.m_DefaultClassInfo[lpObj->Class].Vitality;
	lpObj->Leadership	= gDefaultClassInfo.m_DefaultClassInfo[lpObj->Class].Leadership;
	lpObj->NewStat = gDefaultClassInfo.m_DefaultClassInfo[lpObj->Class].NewStat;

	/*if (lpObj->Class == CLASS_DL)
	{
#if (GAMESERVER_UPDATE == 603)
		lpObj->Leadership += (g_ResetSystem.m_BonusCommand*lpObj->Reset)+(gGRSystem.m_BonusCommand*lpObj->MasterReset);
#endif
	}*/

	lpObj->LevelUpPoint += (NewPoints - ( gDefaultClassInfo.m_DefaultClassInfo[lpObj->Class].Strength + gDefaultClassInfo.m_DefaultClassInfo[lpObj->Class].Dexterity + 
		gDefaultClassInfo.m_DefaultClassInfo[lpObj->Class].Energy + gDefaultClassInfo.m_DefaultClassInfo[lpObj->Class].Vitality + gDefaultClassInfo.m_DefaultClassInfo[lpObj->Class].Leadership));

	this->Update(lpObj);
	//GCNewCharacterInfoSend(lpObj);
}
