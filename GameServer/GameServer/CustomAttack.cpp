// CustomAttack.cpp: implementation of the CCustomAttack class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CustomAttack.h"
#include "ItemManager.h"
#include "Map.h"
#include "MasterSkillTree.h"
#include "Notice.h"
#include "SkillManager.h"
#include "SocketManager.h"
#include "Viewport.h"
//#include "Reconnect.h"
#include "Util.h"
#include "MemScript.h"
#include "Path.h"
#include "Party.h"
#include "EffectManager.h"
#include "MapManager.h"
#include "Message.h"
#include "OfflineMode.h"
#include "Protection.h"
#include "OfflineData.h"
#include "Monster.h"
#include "DSProtocol.h"
//#include <iomanip>
//#include <stdlib.h>
#include <list>
#include <string>
#include "JSProtocol.h"
#include "IpManager.h"

CCustomAttack gCustomAttack;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCustomAttack::CCustomAttack() // OK
{
	this->m_OffExpItems.clear();
#if USE_OFFEXP_RELOAD == TRUE
	this->m_Data.clear();
	//CloseHandle(this->File);
	//this->FileLoaded = false;
	this->AccountsRestored = 0;
#endif
}

CCustomAttack::~CCustomAttack() // OK
{

}

void CCustomAttack::Load(char* path)
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

	this->m_OffExpItems.clear();

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

			OFFEXP_ITEMS info;

			info.Group	= lpMemScript->GetNumber();

			info.Index = lpMemScript->GetAsNumber();

			this->m_OffExpItems.push_back(info);
		}
	}
	catch(...)
	{
		ErrorMessageBox(lpMemScript->GetLastError());
	}

	delete lpMemScript;
}

void CCustomAttack::ReadCustomAttackInfo(char* section,char* path) // OK
{
	this->m_CustomAttackSwitch = GetPrivateProfileInt(section,"CustomAttackSwitch",0,path);

	this->m_CustomAttackEnable[0] = GetPrivateProfileInt(section,"CustomAttackEnable_AL0",0,path);

	this->m_CustomAttackEnable[1] = GetPrivateProfileInt(section,"CustomAttackEnable_AL1",0,path);

	this->m_CustomAttackEnable[2] = GetPrivateProfileInt(section,"CustomAttackEnable_AL2",0,path);

	this->m_CustomAttackEnable[3] = GetPrivateProfileInt(section,"CustomAttackEnable_AL3",0,path);

	this->m_CustomAttackRequireLevel[0] = GetPrivateProfileInt(section,"CustomAttackRequireLevel_AL0",0,path);

	this->m_CustomAttackRequireLevel[1] = GetPrivateProfileInt(section,"CustomAttackRequireLevel_AL1",0,path);

	this->m_CustomAttackRequireLevel[2] = GetPrivateProfileInt(section,"CustomAttackRequireLevel_AL2",0,path);

	this->m_CustomAttackRequireLevel[3] = GetPrivateProfileInt(section,"CustomAttackRequireLevel_AL3",0,path);

	this->m_CustomAttackRequireReset[0] = GetPrivateProfileInt(section,"CustomAttackRequireReset_AL0",0,path);

	this->m_CustomAttackRequireReset[1] = GetPrivateProfileInt(section,"CustomAttackRequireReset_AL1",0,path);

	this->m_CustomAttackRequireReset[2] = GetPrivateProfileInt(section,"CustomAttackRequireReset_AL2",0,path);

	this->m_CustomAttackRequireReset[3] = GetPrivateProfileInt(section,"CustomAttackRequireReset_AL3",0,path);

	GetPrivateProfileString(section,"CustomAttackText1","",this->m_CustomAttackText1,sizeof(this->m_CustomAttackText1),path);

	GetPrivateProfileString(section,"CustomAttackText2","",this->m_CustomAttackText2,sizeof(this->m_CustomAttackText2),path);

	GetPrivateProfileString(section,"CustomAttackText3","",this->m_CustomAttackText3,sizeof(this->m_CustomAttackText3),path);

	GetPrivateProfileString(section,"CustomAttackText4","",this->m_CustomAttackText4,sizeof(this->m_CustomAttackText4),path);

	GetPrivateProfileString(section,"CustomAttackText5","",this->m_CustomAttackText5,sizeof(this->m_CustomAttackText5),path);

	GetPrivateProfileString(section,"CustomAttackText6","",this->m_CustomAttackText6,sizeof(this->m_CustomAttackText6),path);

	this->m_CustomAttackOfflineSwitch = GetPrivateProfileInt(section,"CustomAttackOfflineSwitch",0,path);

	this->m_CustomAttackOfflineGPGain = GetPrivateProfileInt(section,"CustomAttackOfflineGPGain",0,path);

	this->m_CustomAttackOfflineEnable[0] = GetPrivateProfileInt(section,"CustomAttackOfflineEnable_AL0",0,path);

	this->m_CustomAttackOfflineEnable[1] = GetPrivateProfileInt(section,"CustomAttackOfflineEnable_AL1",0,path);

	this->m_CustomAttackOfflineEnable[2] = GetPrivateProfileInt(section,"CustomAttackOfflineEnable_AL2",0,path);

	this->m_CustomAttackOfflineEnable[3] = GetPrivateProfileInt(section,"CustomAttackOfflineEnable_AL3",0,path);

	this->m_CustomAttackOfflineRequireLevel[0] = GetPrivateProfileInt(section,"CustomAttackOfflineRequireLevel_AL0",0,path);

	this->m_CustomAttackOfflineRequireLevel[1] = GetPrivateProfileInt(section,"CustomAttackOfflineRequireLevel_AL1",0,path);

	this->m_CustomAttackOfflineRequireLevel[2] = GetPrivateProfileInt(section,"CustomAttackOfflineRequireLevel_AL2",0,path);

	this->m_CustomAttackOfflineRequireLevel[3] = GetPrivateProfileInt(section,"CustomAttackOfflineRequireLevel_AL3",0,path);

	this->m_CustomAttackOfflineRequireReset[0] = GetPrivateProfileInt(section,"CustomAttackOfflineRequireReset_AL0",0,path);

	this->m_CustomAttackOfflineRequireReset[1] = GetPrivateProfileInt(section,"CustomAttackOfflineRequireReset_AL1",0,path);

	this->m_CustomAttackOfflineRequireReset[2] = GetPrivateProfileInt(section,"CustomAttackOfflineRequireReset_AL2",0,path);

	this->m_CustomAttackOfflineRequireReset[3] = GetPrivateProfileInt(section,"CustomAttackOfflineRequireReset_AL3",0,path);

	this->m_CustomAttackOfflineRequireMoney[0] = GetPrivateProfileInt(section,"CustomAttackOfflineRequireMoney_AL0",0,path);

	this->m_CustomAttackOfflineRequireMoney[1] = GetPrivateProfileInt(section,"CustomAttackOfflineRequireMoney_AL1",0,path);

	this->m_CustomAttackOfflineRequireMoney[2] = GetPrivateProfileInt(section,"CustomAttackOfflineRequireMoney_AL2",0,path);

	this->m_CustomAttackOfflineRequireMoney[3] = GetPrivateProfileInt(section,"CustomAttackOfflineRequireMoney_AL3",0,path);

	this->m_CustomAttackOfflineMoneyFreeTime[0] = GetPrivateProfileInt(section, "CustomAttackOfflineMoneyFreeTime_AL0", 0, path);
	this->m_CustomAttackOfflineMoneyFreeTime[1] = GetPrivateProfileInt(section, "CustomAttackOfflineMoneyFreeTime_AL1", 0, path);
	this->m_CustomAttackOfflineMoneyFreeTime[2] = GetPrivateProfileInt(section, "CustomAttackOfflineMoneyFreeTime_AL2", 0, path);
	this->m_CustomAttackOfflineMoneyFreeTime[3] = GetPrivateProfileInt(section, "CustomAttackOfflineMoneyFreeTime_AL3", 0, path);

	this->m_CustomAttackOfflineOpenPSShop[0] = GetPrivateProfileInt(section,"CustomAttackOfflineOpenPSShop_AL0",0,path);

	this->m_CustomAttackOfflineOpenPSShop[1] = GetPrivateProfileInt(section,"CustomAttackOfflineOpenPSShop_AL1",0,path);

	this->m_CustomAttackOfflineOpenPSShop[2] = GetPrivateProfileInt(section,"CustomAttackOfflineOpenPSShop_AL2",0,path);

	this->m_CustomAttackOfflineOpenPSShop[3] = GetPrivateProfileInt(section,"CustomAttackOfflineOpenPSShop_AL3",0,path);

	GetPrivateProfileString(section,"CustomAttackOfflineText1","",this->m_CustomAttackOfflineText1,sizeof(this->m_CustomAttackOfflineText1),path);

	GetPrivateProfileString(section,"CustomAttackOfflineText2","",this->m_CustomAttackOfflineText2,sizeof(this->m_CustomAttackOfflineText2),path);

	GetPrivateProfileString(section,"CustomAttackOfflineText3","",this->m_CustomAttackOfflineText3,sizeof(this->m_CustomAttackOfflineText3),path);

	GetPrivateProfileString(section,"CustomAttackOfflineText4","",this->m_CustomAttackOfflineText4,sizeof(this->m_CustomAttackOfflineText4),path);

	GetPrivateProfileString(section,"CustomAttackOfflineText5","",this->m_CustomAttackOfflineText5,sizeof(this->m_CustomAttackOfflineText5),path);

	GetPrivateProfileString(section,"CustomAttackOfflineText6","",this->m_CustomAttackOfflineText6,sizeof(this->m_CustomAttackOfflineText6),path);

	GetPrivateProfileString(section,"CustomAttackOfflineText7","",this->m_CustomAttackOfflineText7,sizeof(this->m_CustomAttackOfflineText7),path);

	this->m_CustomAttackGetItems[0] = GetPrivateProfileInt(section,"PickItems_AL0",0,path);
	this->m_CustomAttackGetItems[1] = GetPrivateProfileInt(section,"PickItems_AL1",0,path);
	this->m_CustomAttackGetItems[2] = GetPrivateProfileInt(section,"PickItems_AL2",0,path);
	this->m_CustomAttackGetItems[3] = GetPrivateProfileInt(section,"PickItems_AL3",0,path);

	this->m_CustomAttackGetExc[0] = GetPrivateProfileInt(section,"PickExc_AL0",0,path);
	this->m_CustomAttackGetExc[1] = GetPrivateProfileInt(section,"PickExc_AL1",0,path);
	this->m_CustomAttackGetExc[2] = GetPrivateProfileInt(section,"PickExc_AL2",0,path);
	this->m_CustomAttackGetExc[3] = GetPrivateProfileInt(section,"PickExc_AL3",0,path);

	this->m_CustomAttackGetMoney[0] = GetPrivateProfileInt(section,"PickMoney_AL0",0,path);
	this->m_CustomAttackGetMoney[1] = GetPrivateProfileInt(section,"PickMoney_AL1",0,path);
	this->m_CustomAttackGetMoney[2] = GetPrivateProfileInt(section,"PickMoney_AL2",0,path);
	this->m_CustomAttackGetMoney[3] = GetPrivateProfileInt(section,"PickMoney_AL3",0,path);

	this->m_CustomAttackGetAnc[0] = GetPrivateProfileInt(section,"PickAnc_AL0",0,path);
	this->m_CustomAttackGetAnc[1] = GetPrivateProfileInt(section,"PickAnc_AL1",0,path);
	this->m_CustomAttackGetAnc[2] = GetPrivateProfileInt(section,"PickAnc_AL2",0,path);
	this->m_CustomAttackGetAnc[3] = GetPrivateProfileInt(section,"PickAnc_AL3",0,path);

	this->m_CustomAttackGetSocket[0] = GetPrivateProfileInt(section,"PickSocket_AL0",0,path);
	this->m_CustomAttackGetSocket[1] = GetPrivateProfileInt(section,"PickSocket_AL1",0,path);
	this->m_CustomAttackGetSocket[2] = GetPrivateProfileInt(section,"PickSocket_AL2",0,path);
	this->m_CustomAttackGetSocket[3] = GetPrivateProfileInt(section,"PickSocket_AL3",0,path);

	this->m_CustomAttackGetSelectedItems[0] = GetPrivateProfileInt(section,"PickSelectedItems_AL0",0,path);
	this->m_CustomAttackGetSelectedItems[1] = GetPrivateProfileInt(section,"PickSelectedItems_AL1",0,path);
	this->m_CustomAttackGetSelectedItems[2] = GetPrivateProfileInt(section,"PickSelectedItems_AL2",0,path);
	this->m_CustomAttackGetSelectedItems[3] = GetPrivateProfileInt(section,"PickSelectedItems_AL3",0,path);

	this->m_CustomAttackSpeed1[0] = GetPrivateProfileInt(section,"CustomAttackSpeed1_AL0",0,path);
	this->m_CustomAttackSpeed1[1] = GetPrivateProfileInt(section,"CustomAttackSpeed1_AL1",0,path);
	this->m_CustomAttackSpeed1[2] = GetPrivateProfileInt(section,"CustomAttackSpeed1_AL2",0,path);
	this->m_CustomAttackSpeed1[3] = GetPrivateProfileInt(section,"CustomAttackSpeed1_AL3",0,path);

	this->m_CustomAttackSpeed2[0] = GetPrivateProfileInt(section,"CustomAttackSpeed2_AL0",0,path);
	this->m_CustomAttackSpeed2[1] = GetPrivateProfileInt(section,"CustomAttackSpeed2_AL1",0,path);
	this->m_CustomAttackSpeed2[2] = GetPrivateProfileInt(section,"CustomAttackSpeed2_AL2",0,path);
	this->m_CustomAttackSpeed2[3] = GetPrivateProfileInt(section,"CustomAttackSpeed2_AL3",0,path);

	this->m_CustomAttackSpeed3[0] = GetPrivateProfileInt(section,"CustomAttackSpeed3_AL0",0,path);
	this->m_CustomAttackSpeed3[1] = GetPrivateProfileInt(section,"CustomAttackSpeed3_AL1",0,path);
	this->m_CustomAttackSpeed3[2] = GetPrivateProfileInt(section,"CustomAttackSpeed3_AL2",0,path);
	this->m_CustomAttackSpeed3[3] = GetPrivateProfileInt(section,"CustomAttackSpeed3_AL3",0,path);

	this->m_CustomAttackSpeed4[0] = GetPrivateProfileInt(section,"CustomAttackSpeed4_AL0",0,path);
	this->m_CustomAttackSpeed4[1] = GetPrivateProfileInt(section,"CustomAttackSpeed4_AL1",0,path);
	this->m_CustomAttackSpeed4[2] = GetPrivateProfileInt(section,"CustomAttackSpeed4_AL2",0,path);
	this->m_CustomAttackSpeed4[3] = GetPrivateProfileInt(section,"CustomAttackSpeed4_AL3",0,path);

	this->m_CustomAttackSpeed5[0] = GetPrivateProfileInt(section,"CustomAttackSpeed5_AL0",0,path);
	this->m_CustomAttackSpeed5[1] = GetPrivateProfileInt(section,"CustomAttackSpeed5_AL1",0,path);
	this->m_CustomAttackSpeed5[2] = GetPrivateProfileInt(section,"CustomAttackSpeed5_AL2",0,path);
	this->m_CustomAttackSpeed5[3] = GetPrivateProfileInt(section,"CustomAttackSpeed5_AL3",0,path);

	this->m_CustomAttackSpeed6[0] = GetPrivateProfileInt(section,"CustomAttackSpeed6_AL0",0,path);
	this->m_CustomAttackSpeed6[1] = GetPrivateProfileInt(section,"CustomAttackSpeed6_AL1",0,path);
	this->m_CustomAttackSpeed6[2] = GetPrivateProfileInt(section,"CustomAttackSpeed6_AL2",0,path);
	this->m_CustomAttackSpeed6[3] = GetPrivateProfileInt(section,"CustomAttackSpeed6_AL3",0,path);

	this->m_CustomAttackSpeed7[0] = GetPrivateProfileInt(section,"CustomAttackSpeed7_AL0",0,path);
	this->m_CustomAttackSpeed7[1] = GetPrivateProfileInt(section,"CustomAttackSpeed7_AL1",0,path);
	this->m_CustomAttackSpeed7[2] = GetPrivateProfileInt(section,"CustomAttackSpeed7_AL2",0,path);
	this->m_CustomAttackSpeed7[3] = GetPrivateProfileInt(section,"CustomAttackSpeed7_AL3",0,path);

	this->m_CustomAttackSpeed8[0] = GetPrivateProfileInt(section,"CustomAttackSpeed8_AL0",0,path);
	this->m_CustomAttackSpeed8[1] = GetPrivateProfileInt(section,"CustomAttackSpeed8_AL1",0,path);
	this->m_CustomAttackSpeed8[2] = GetPrivateProfileInt(section,"CustomAttackSpeed8_AL2",0,path);
	this->m_CustomAttackSpeed8[3] = GetPrivateProfileInt(section,"CustomAttackSpeed8_AL3",0,path);

	this->Load(gPath.GetFullPath("Item\\OffExpItems.txt"));
#if USE_OFFEXP_RELOAD == TRUE
	this->LoadOffExp(".\\OffExp.ini");
#endif
}

void CCustomAttack::CommandCustomAttack(LPOBJ lpObj,char* arg) // OK
{
	if(this->m_CustomAttackSwitch == 0)
	{
		return;
	}

	if (gMapManager.GetMapOffExpEnable(lpObj->Map) == 0)
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessage.GetMessage(520));
		return;
	}

	if(this->m_CustomAttackEnable[lpObj->AccountLevel] == 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,this->m_CustomAttackText1);
		return;
	}

	if(((gMasterSkillTree.CheckMasterLevel(lpObj)==0)?lpObj->Level:(lpObj->Level+lpObj->MasterLevel)) < this->m_CustomAttackRequireLevel[lpObj->AccountLevel])
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,this->m_CustomAttackText2,this->m_CustomAttackRequireLevel[lpObj->AccountLevel]);
		return;
	}

	if(lpObj->Reset < this->m_CustomAttackRequireReset[lpObj->AccountLevel])
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,this->m_CustomAttackText3,this->m_CustomAttackRequireReset[lpObj->AccountLevel]);
		return;
	}

	if(lpObj->AttackCustom != 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,this->m_CustomAttackText4);
		return;
	}

	if(gMap[lpObj->Map].CheckAttr(lpObj->X,lpObj->Y,1) != 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,this->m_CustomAttackText5);
		return;
	}

	if(DE_MAP_RANGE(lpObj->Map) != 0 && CA_MAP_RANGE(lpObj->Map) != 0 || DS_MAP_RANGE(lpObj->Map) != 0 || BC_MAP_RANGE(lpObj->Map) != 0 || CC_MAP_RANGE(lpObj->Map) != 0 || IT_MAP_RANGE(lpObj->Map) != 0 || DA_MAP_RANGE(lpObj->Map) != 0 || DG_MAP_RANGE(lpObj->Map) != 0 || IG_MAP_RANGE(lpObj->Map) != 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,this->m_CustomAttackText5);
		return;
	}

	int SkillNumber = SKILL_NONE;

	if(this->GetAttackSkill(lpObj,&SkillNumber) == 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,this->m_CustomAttackText6);
		return;
	}

	lpObj->AttackCustom = 1;

	lpObj->AttackCustomSkill = SkillNumber;

	lpObj->AttackCustomDelay = GetTickCount();

	lpObj->AttackCustomZoneX = lpObj->X;

	lpObj->AttackCustomZoneY = lpObj->Y;

	lpObj->AttackCustomZoneMap = lpObj->Map;

	lpObj->AttackCustomOffline = 0;

	lpObj->AttackCustomOfflineTime = 0;

	lpObj->AttackCustomOfflineMoneyDelay = 0;

	lpObj->AttackCustomOfflineMoneyFreeDelay = 0;
}

void CCustomAttack::CommandCustomAttackOffline(LPOBJ lpObj,char* arg) // OK
{
	if(this->m_CustomAttackOfflineSwitch == 0)
	{
		return;
	}

	if (gMapManager.GetMapOffExpEnable(lpObj->Map) == 0)
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, gMessage.GetMessage(520));
		return;
	}

	if(this->m_CustomAttackOfflineEnable[lpObj->AccountLevel] == 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,this->m_CustomAttackOfflineText1);
		return;
	}

	if(((gMasterSkillTree.CheckMasterLevel(lpObj)==0)?lpObj->Level:(lpObj->Level+lpObj->MasterLevel)) < this->m_CustomAttackOfflineRequireLevel[lpObj->AccountLevel])
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,this->m_CustomAttackOfflineText2,this->m_CustomAttackOfflineRequireLevel[lpObj->AccountLevel]);
		return;
	}

	if(((int)lpObj->Reset) < this->m_CustomAttackOfflineRequireReset[lpObj->AccountLevel])
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,this->m_CustomAttackOfflineText3,this->m_CustomAttackOfflineRequireReset[lpObj->AccountLevel]);
		return;
	}

	if(((int)lpObj->Money) < this->m_CustomAttackOfflineRequireMoney[lpObj->AccountLevel])
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,this->m_CustomAttackOfflineText4,this->m_CustomAttackOfflineRequireMoney[lpObj->AccountLevel]);
		return;
	}

	if(lpObj->AttackCustom == 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,this->m_CustomAttackOfflineText5);
		return;
	}

	if(gMap[lpObj->Map].CheckAttr(lpObj->X,lpObj->Y,1) != 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,this->m_CustomAttackOfflineText6);
		return;
	}

	if(DE_MAP_RANGE(lpObj->Map) != 0 && CA_MAP_RANGE(lpObj->Map) != 0 || DS_MAP_RANGE(lpObj->Map) != 0 || BC_MAP_RANGE(lpObj->Map) != 0 || CC_MAP_RANGE(lpObj->Map) != 0 || IT_MAP_RANGE(lpObj->Map) != 0 || DA_MAP_RANGE(lpObj->Map) != 0 || DG_MAP_RANGE(lpObj->Map) != 0 || IG_MAP_RANGE(lpObj->Map) != 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,this->m_CustomAttackOfflineText6);
		return;
	}

	if (this->m_CustomAttackOfflineOpenPSShop[lpObj->AccountLevel] == 1)
	{
		lpObj->PShopOpen = true;
		sprintf(lpObj->PShopText, this->m_CustomAttackOfflineText7);
	}

	if(lpObj->CloseCount > 0)
	{
		return;
	}

	//lpObj->CloseCount = 1;

	//lpObj->CloseType = 2;

	//lpObj->EnableDelCharacter = 1;

	//lpObj->Socket = INVALID_SOCKET;

	lpObj->AttackCustomOffline = 1;

	lpObj->AttackCustomOfflineTime = 0;

	lpObj->AttackCustomOfflineMoneyDelay = GetTickCount();

	lpObj->AttackCustomOfflineMoneyFreeDelay = GetTickCount();

	lpObj->Money -= this->m_CustomAttackOfflineRequireMoney[lpObj->AccountLevel];

	GJSetOffExpSend(lpObj, 1);

	//gOfflineData.SetOffline(lpObj,1);
#if USE_OFFEXP_RELOAD == TRUE
	this->AddToList(lpObj);
#endif

	GCCloseClientSend(lpObj->Index,2);
	//gReconnect.CloseClient(lpObj);
	//closesocket(lpObj->PerSocketContext->Socket);
}

bool CCustomAttack::GetAttackSkill(LPOBJ lpObj,int* SkillNumber) // OK
{
	CSkill* lpSkill = 0;

	 if (lpObj->Class == CLASS_MG)
    {
        if(lpObj->Inventory[0].m_Index >= GET_ITEM(5,0) && lpObj->Inventory[0].m_Index < GET_ITEM(6,0))
        {
            lpSkill = ((lpSkill==0)?gSkillManager.GetSkill(lpObj,SKILL_LIGHTNING_STORM):lpSkill);
            lpSkill = ((lpSkill==0)?gSkillManager.GetSkill(lpObj,SKILL_EVIL_SPIRIT):lpSkill);
        }
        else
        {
            lpSkill = ((lpSkill==0)?gSkillManager.GetSkill(lpObj,SKILL_SWORD_SLASH):lpSkill);
            lpSkill = ((lpSkill==0)?gSkillManager.GetSkill(lpObj,SKILL_POWER_SLASH):lpSkill);
        }
       
        if(lpSkill == 0)
        {
            return 0;
        }
        else
        {
            (*SkillNumber) = lpSkill->m_index;
            return 1;
        }
    }

	switch(lpObj->Class)
	{
		case CLASS_DW:
			lpSkill = ((lpSkill==0)?gSkillManager.GetSkill(lpObj,SKILL_ICE_STORM):lpSkill);
			lpSkill = ((lpSkill==0)?gSkillManager.GetSkill(lpObj,SKILL_EVIL_SPIRIT):lpSkill);
			lpSkill = ((lpSkill==0)?gSkillManager.GetSkill(lpObj,SKILL_INFERNO):lpSkill);
			break;
		case CLASS_DK:
			lpSkill = ((lpSkill==0)?gSkillManager.GetSkill(lpObj,SKILL_TWISTING_SLASH):lpSkill);
			lpSkill = ((lpSkill==0)?gSkillManager.GetSkill(lpObj,SKILL_DEATH_STAB):lpSkill);
			break;
		case CLASS_FE:
			lpSkill = ((lpSkill==0)?gSkillManager.GetSkill(lpObj,SKILL_TRIPLE_SHOT):lpSkill);
			lpSkill = ((lpSkill==0)?gSkillManager.GetSkill(lpObj,SKILL_FIVE_SHOT):lpSkill);
			break;
		case CLASS_MG:
			lpSkill = ((lpSkill==0)?gSkillManager.GetSkill(lpObj,SKILL_SWORD_SLASH):lpSkill);
			lpSkill = ((lpSkill==0)?gSkillManager.GetSkill(lpObj,SKILL_LIGHTNING_STORM):lpSkill);
			lpSkill = ((lpSkill==0)?gSkillManager.GetSkill(lpObj,SKILL_POWER_SLASH):lpSkill);
			lpSkill = ((lpSkill==0)?gSkillManager.GetSkill(lpObj,SKILL_EVIL_SPIRIT):lpSkill);
			break;
		case CLASS_DL:
			lpSkill = ((lpSkill==0)?gSkillManager.GetSkill(lpObj,SKILL_BIRDS):lpSkill);
			lpSkill = ((lpSkill==0)?gSkillManager.GetSkill(lpObj,SKILL_FIRE_BURST):lpSkill);
			lpSkill = ((lpSkill==0)?gSkillManager.GetSkill(lpObj,SKILL_FORCE):lpSkill);
			break;
		case CLASS_SU:
			lpSkill = ((lpSkill==0)?gSkillManager.GetSkill(lpObj,SKILL_RED_STORM):lpSkill);
			lpSkill = ((lpSkill==0)?gSkillManager.GetSkill(lpObj,SKILL_CHAIN_LIGHTNING):lpSkill);
			break;
		case CLASS_RF:
			lpSkill = ((lpSkill==0)?gSkillManager.GetSkill(lpObj,SKILL_DRAGON_LORE):lpSkill);
			lpSkill = ((lpSkill==0)?gSkillManager.GetSkill(lpObj,SKILL_PHOENIX_SHOT):lpSkill);
			lpSkill = ((lpSkill==0)?gSkillManager.GetSkill(lpObj,SKILL_LARGE_RING_BLOWER):lpSkill);
			break;
	}

	if(lpSkill == 0)
	{
		if (lpObj->Class == CLASS_FE)
		{
			lpSkill = ((lpSkill==0)?gSkillManager.GetSkill(lpObj,SKILL_HEAL):lpSkill);
			lpSkill = ((lpSkill==0)?gSkillManager.GetSkill(lpObj,SKILL_GREATER_DEFENSE):lpSkill);
			lpSkill = ((lpSkill==0)?gSkillManager.GetSkill(lpObj,SKILL_GREATER_DAMAGE):lpSkill);

			if (lpSkill == 0)
			{
				return 0;
			}
			else return 1;
		}
		return 0;
	}
	else
	{
		(*SkillNumber) = lpSkill->m_index;
		return 1;
	}
}

bool CCustomAttack::GetTargetMonster(LPOBJ lpObj,int SkillNumber,int* MonsterIndex) // OK
{
	int NearestDistance = 100;

	for(int n=0;n < MAX_VIEWPORT;n++)
	{
		if(lpObj->VpPlayer2[n].state == VIEWPORT_NONE || OBJECT_RANGE(lpObj->VpPlayer2[n].index) == 0 || lpObj->VpPlayer2[n].type != OBJECT_MONSTER)
		{
			continue;
		}

		if(gSkillManager.CheckSkillTarget(lpObj,lpObj->VpPlayer2[n].index,-1,lpObj->VpPlayer2[n].type) == 0)
		{
			continue;
		}

		if(gObjCalcDistance(lpObj,&gObj[lpObj->VpPlayer2[n].index]) >= NearestDistance)
		{
			continue;
		}

		if(gSkillManager.CheckSkillRange(SkillNumber,lpObj->X,lpObj->Y,gObj[lpObj->VpPlayer2[n].index].X,gObj[lpObj->VpPlayer2[n].index].Y) != 0)
		{
			(*MonsterIndex) = lpObj->VpPlayer2[n].index;
			NearestDistance = gObjCalcDistance(lpObj,&gObj[lpObj->VpPlayer2[n].index]);
			continue;
		}

		if(gSkillManager.CheckSkillRadio(SkillNumber,lpObj->X,lpObj->Y,gObj[lpObj->VpPlayer2[n].index].X,gObj[lpObj->VpPlayer2[n].index].Y) != 0)
		{
			(*MonsterIndex) = lpObj->VpPlayer2[n].index;
			NearestDistance = gObjCalcDistance(lpObj,&gObj[lpObj->VpPlayer2[n].index]);
			continue;
		}
	}

	return ((NearestDistance==100)?0:1);
}

bool CCustomAttack::CheckRequireMoney(LPOBJ lpObj) // OK
{
	if(lpObj->AttackCustomOffline == 0)
	{
		return 1;
	}

	if((GetTickCount()-((DWORD)lpObj->AttackCustomOfflineMoneyDelay)) < 60000)
	{
		return 1;
	}

	if (GetTickCount() - lpObj->AttackCustomOfflineMoneyFreeDelay < this->m_CustomAttackOfflineMoneyFreeTime[lpObj->AccountLevel] * 60000)
		return true;

	if(((int)lpObj->Money) < this->m_CustomAttackOfflineRequireMoney[lpObj->AccountLevel])
	{
		return 0;
	}

	lpObj->Money -= this->m_CustomAttackOfflineRequireMoney[lpObj->AccountLevel];

	lpObj->AttackCustomOfflineMoneyDelay = GetTickCount();

	return 1;
}

void CCustomAttack::OnAttackClose(LPOBJ lpObj) // OK
{
	if(lpObj->AttackCustom != 0)
	{
		lpObj->AttackCustom = 0;
		lpObj->AttackCustomSkill = 0;
		lpObj->AttackCustomDelay = 0;
		lpObj->AttackCustomZoneX = 0;
		lpObj->AttackCustomZoneY = 0;
		lpObj->AttackCustomZoneMap = 0;
		lpObj->OfflineMode.UseBuff = 0;
		lpObj->OfflineMode.PickZen = 0;
		lpObj->OfflineMode.PickJewels = 0;
		lpObj->OfflineMode.PickExe = 0;
		lpObj->OfflineMode.PickAnc = 0;
		lpObj->OfflineMode.PickSelected = 0;
#if (USE_OFFLINEMODE_HELPER_PICK == TRUE)
		for (int i=0;i<100;i++)
		{
			lpObj->MuHelperItems[i] = -1;
		}
#endif
	}

	if(lpObj->AttackCustomOffline == 1)
	{
		lpObj->AttackCustomOffline = 2;
		lpObj->AttackCustomOfflineTime = 5;
	}
}

void CCustomAttack::OnAttackSecondProc(LPOBJ lpObj) // OK
{
	if(lpObj->AttackCustomOffline != 0)
	{
		if(lpObj->AttackCustomOffline == 2)
		{
			if((--lpObj->AttackCustomOfflineTime) == 0)
			{
				lpObj->AttackCustomOffline = 0;
				lpObj->AttackCustomOfflineTime = 0;
				lpObj->AttackCustomOfflineMoneyDelay = 0;
				lpObj->AttackCustomOfflineMoneyFreeDelay = 0;
				gObjDel(lpObj->Index);
			}
		}

		lpObj->CheckSumTime = GetTickCount();
		lpObj->ConnectTickCount = GetTickCount();
		lpObj->PcPointPointTime = ((this->m_CustomAttackOfflineGPGain==0)?GetTickCount():lpObj->PcPointPointTime);
		lpObj->CashShopGoblinPointTime = ((this->m_CustomAttackOfflineGPGain==0)?GetTickCount():lpObj->CashShopGoblinPointTime);
	}
}

void CCustomAttack::OnAttackAlreadyConnected(LPOBJ lpObj) // OK
{
	if(lpObj->AttackCustomOffline != 0)
	{
		//gOfflineData.DeleteOffline(lpObj);
#if USE_OFFEXP_RELOAD == TRUE
		this->DelFromList(lpObj->Account);
#endif
		lpObj->AttackCustomOffline = 0;
		lpObj->AttackCustomOfflineTime = 0;
		lpObj->AttackCustomOfflineMoneyDelay = 0;
		lpObj->AttackCustomOfflineMoneyFreeDelay = 0;
		gObjDel(lpObj->Index);	
	}
}

void CCustomAttack::OnAttackMonsterAndMsgProc(LPOBJ lpObj) // OK
{
	CSkill* lpSkill;

	if(lpObj->AttackCustom == 0)
	{
		return;
	}

	if(this->CheckRequireMoney(lpObj) == 0)
	{
		this->OnAttackClose(lpObj);
		return;
	}

	if(gMap[lpObj->Map].CheckAttr(lpObj->X,lpObj->Y,1) != 0)
	{
		this->OnAttackClose(lpObj);
		return;
	}

	bool Attack = false;

	if(lpObj->Dexterity > 0 && lpObj->Dexterity <= 3000)
	{
		lpObj->AttackCustomSpeed++;
		if(lpObj->AttackCustomSpeed >= this->m_CustomAttackSpeed1[lpObj->AccountLevel])
		{
			Attack = true;
			lpObj->AttackCustomSpeed = 0;
		}
	}
	else if (lpObj->Dexterity > 3000 && lpObj->Dexterity <= 6000)
	{
		lpObj->AttackCustomSpeed++;
		if(lpObj->AttackCustomSpeed >= this->m_CustomAttackSpeed2[lpObj->AccountLevel])
		{
			Attack = true;
			lpObj->AttackCustomSpeed = 0;
		}
	}
	else if (lpObj->Dexterity > 6000 && lpObj->Dexterity <= 9000)
	{
		lpObj->AttackCustomSpeed++;
		if(lpObj->AttackCustomSpeed >= this->m_CustomAttackSpeed3[lpObj->AccountLevel])
		{
			Attack = true;
			lpObj->AttackCustomSpeed = 0;
		}
	}
	else if (lpObj->Dexterity > 9000 && lpObj->Dexterity <= 12000)
	{
		lpObj->AttackCustomSpeed++;
		if(lpObj->AttackCustomSpeed >= this->m_CustomAttackSpeed4[lpObj->AccountLevel])
		{
			Attack = true;
			lpObj->AttackCustomSpeed = 0;
		}
	}
	else if (lpObj->Dexterity > 12000 && lpObj->Dexterity <= 15000)
	{
		lpObj->AttackCustomSpeed++;
		if(lpObj->AttackCustomSpeed >= this->m_CustomAttackSpeed5[lpObj->AccountLevel])
		{
			Attack = true;
			lpObj->AttackCustomSpeed = 0;
		}
	}
	else if (lpObj->Dexterity > 15000 && lpObj->Dexterity <= 18000)
	{
		lpObj->AttackCustomSpeed++;
		if(lpObj->AttackCustomSpeed >= this->m_CustomAttackSpeed6[lpObj->AccountLevel])
		{
			Attack = true;
			lpObj->AttackCustomSpeed = 0;
		}
	}
	else if (lpObj->Dexterity > 18000 && lpObj->Dexterity <= 21000)
	{
		lpObj->AttackCustomSpeed++;
		if(lpObj->AttackCustomSpeed >= this->m_CustomAttackSpeed7[lpObj->AccountLevel])
		{
			Attack = true;
			lpObj->AttackCustomSpeed = 0;
		}
	}
	else if (lpObj->Dexterity > 21000 && lpObj->Dexterity <= 24000)
	{
		lpObj->AttackCustomSpeed++;
		if(lpObj->AttackCustomSpeed >= this->m_CustomAttackSpeed8[lpObj->AccountLevel])
		{
			Attack = true;
			lpObj->AttackCustomSpeed = 0;
		}
	}
	else if (lpObj->Dexterity > 24000)
	{
		Attack = true;
	}
	else
	{
		return;
	}

	if(Attack == false)
	{
		return;
	}

	if((lpSkill=gSkillManager.GetSkill(lpObj,lpObj->AttackCustomSkill)) == 0)
	{
		if (lpObj->Class == CLASS_FE)
		{
			lpSkill = ((lpSkill==0)?gSkillManager.GetSkill(lpObj,SKILL_HEAL):lpSkill);
			lpSkill = ((lpSkill==0)?gSkillManager.GetSkill(lpObj,SKILL_GREATER_DEFENSE):lpSkill);
			lpSkill = ((lpSkill==0)?gSkillManager.GetSkill(lpObj,SKILL_GREATER_DAMAGE):lpSkill);

			if (lpSkill == 0)
			{
				this->OnAttackClose(lpObj);
				return;
			}
		}
		else
		{
			this->OnAttackClose(lpObj);
			return;
		}
	}

	if(gViewport.CheckViewportObjectPosition(lpObj->Index,lpObj->AttackCustomZoneMap,lpObj->AttackCustomZoneX,lpObj->AttackCustomZoneY,5) == 0)
	{
		this->OnAttackClose(lpObj);
		return;
	}

	if (gProtection.GetCustomState(CUSTOM_OFFLINE_MODE) == true)
	{
		gOfflineMode.PickItems(lpObj);
	}

	if (m_CustomAttackGetItems[lpObj->AccountLevel])
	{
		this->GetItemOnFloor(lpObj);
	}

	if(((lpObj->Life*100)/(lpObj->MaxLife+lpObj->AddLife)) < 50)
	{
		PMSG_ITEM_USE_RECV pMsg;

		pMsg.header.set(0x26,sizeof(pMsg));

		pMsg.SourceSlot = 0xFF;

		pMsg.SourceSlot = ((pMsg.SourceSlot==0xFF)?gItemManager.GetInventoryItemSlot(lpObj,GET_ITEM(14,3),-1):pMsg.SourceSlot);

		pMsg.SourceSlot = ((pMsg.SourceSlot==0xFF)?gItemManager.GetInventoryItemSlot(lpObj,GET_ITEM(14,2),-1):pMsg.SourceSlot);

		pMsg.SourceSlot = ((pMsg.SourceSlot==0xFF)?gItemManager.GetInventoryItemSlot(lpObj,GET_ITEM(14,1),-1):pMsg.SourceSlot);

		pMsg.TargetSlot = 0xFF;

		pMsg.type = 0;

		if(INVENTORY_FULL_RANGE(pMsg.SourceSlot) != 0){gItemManager.CGItemUseRecv(&pMsg,lpObj->Index);}
	}

	if(((lpObj->Mana*100)/(lpObj->MaxMana+lpObj->AddMana)) < 50)
	{
		PMSG_ITEM_USE_RECV pMsg;

		pMsg.header.set(0x26,sizeof(pMsg));

		pMsg.SourceSlot = 0xFF;

		pMsg.SourceSlot = ((pMsg.SourceSlot==0xFF)?gItemManager.GetInventoryItemSlot(lpObj,GET_ITEM(14,6),-1):pMsg.SourceSlot);

		pMsg.SourceSlot = ((pMsg.SourceSlot==0xFF)?gItemManager.GetInventoryItemSlot(lpObj,GET_ITEM(14,4),-1):pMsg.SourceSlot);

		pMsg.SourceSlot = ((pMsg.SourceSlot==0xFF)?gItemManager.GetInventoryItemSlot(lpObj,GET_ITEM(14,3),-1):pMsg.SourceSlot);

		pMsg.TargetSlot = 0xFF;

		pMsg.type = 0;

		if(INVENTORY_FULL_RANGE(pMsg.SourceSlot) != 0){gItemManager.CGItemUseRecv(&pMsg,lpObj->Index);}
	}

	if((GetTickCount()-((DWORD)lpObj->AttackCustomDelay)) >= (((((DWORD)lpObj->PhysiSpeed)*5)>1500)?0:(1500-(((DWORD)lpObj->PhysiSpeed)*5))))
	{
		//this->ElfBuffs(lpObj);

		if (lpObj->OfflineMode.UseBuff == true && gProtection.GetCustomState(CUSTOM_OFFLINE_MODE) == true)
		{
			gOfflineMode.UseBuffs(lpObj);
		}
		else if (gProtection.GetCustomState(CUSTOM_OFFLINE_MODE) == false)
		{
			this->ElfBuffs(lpObj);
		}


		int MonsterIndex = -1;

		lpObj->AttackCustomDelay = GetTickCount();

		if(this->GetTargetMonster(lpObj,lpSkill->m_index,&MonsterIndex) != 0)
		{
			switch(lpSkill->m_skill)
			{
				case SKILL_EVIL_SPIRIT:
				case SKILL_INFERNO:
				case SKILL_TRIPLE_SHOT:
					this->SendMultiSkillAttack(lpObj,MonsterIndex,lpSkill->m_index);
					break;
				case SKILL_ICE_STORM:
				case SKILL_TWISTING_SLASH:
				case SKILL_RAGEFUL_BLOW:
				case SKILL_FROZEN_STAB:
				case SKILL_POWER_SLASH:
				case SKILL_FIRE_SCREAM:
				case SKILL_RED_STORM:
				case SKILL_GHOST_PHANTOM:
				case SKILL_FIVE_SHOT:
				case SKILL_SWORD_SLASH:
				case SKILL_LIGHTNING_STORM:
				case SKILL_BIRDS:
				case SKILL_DRAGON_LORE:
					this->SendDurationSkillAttack(lpObj,MonsterIndex,lpSkill->m_index);
					break;
				case SKILL_FORCE:
				case SKILL_FIRE_BURST:
				case SKILL_CHAIN_LIGHTNING:
				case SKILL_ICE_ARROW:
				case SKILL_LARGE_RING_BLOWER:
				case SKILL_PHOENIX_SHOT:
					this->SendSkillAttack(lpObj,MonsterIndex,lpSkill->m_index);
					break;
				case SKILL_DARK_SIDE:
					this->SendRFSkillAttack(lpObj,MonsterIndex,lpSkill->m_index);
					break;
			}
		}
	}
}

void CCustomAttack::ElfBuffs(LPOBJ lpObj)
{
	int dis;
	int SearchSkill = 0;
	SYSTEMTIME time;
	GetLocalTime(&time);
	if( time.wSecond == 0  || time.wSecond == 10 || 
	    time.wSecond == 20 || time.wSecond == 30 ||
	    time.wSecond == 40 || time.wSecond == 50 )
	{
		int partynum = -1;
		int partycount;
		int number;

		LPOBJ lpPartyObj;

		partynum = lpObj->PartyNumber;

		if(partynum != -1)
		{
			partycount = gParty.m_PartyInfo[partynum].Count;

			for(int j = 0; j < MAX_PARTY_USER; j++)
			{
				number = gParty.m_PartyInfo[partynum].Index[j];

				if (number >= 0)
				{
					lpPartyObj = &gObj[number];
					if(lpObj->Map == lpPartyObj->Map)
					{
						dis = gObjCalcDistance(lpObj,lpPartyObj);

						if (dis <= 6)
						{
							if (!gEffectManager.CheckEffect(lpPartyObj, EFFECT_GREATER_DEFENSE))
							{
								if (gSkillManager.GetSkill(lpObj, SKILL_GREATER_DEFENSE) != NULL)
								{
									this->SendSkillAttack(lpObj, lpPartyObj->Index, SKILL_GREATER_DEFENSE);
								}
							}

							if (!gEffectManager.CheckEffect(lpPartyObj, EFFECT_GREATER_DAMAGE))
							{
								if (gSkillManager.GetSkill(lpObj, SKILL_GREATER_DAMAGE) != NULL)
								{
									this->SendSkillAttack(lpObj, lpPartyObj->Index, SKILL_GREATER_DAMAGE);
								}
							}

							if (gSkillManager.GetSkill(lpObj, SKILL_HEAL) != NULL)
							{
								this->SendSkillAttack(lpObj, lpPartyObj->Index, SKILL_HEAL);
							}

							if (!gEffectManager.CheckEffect(lpObj, EFFECT_INFINITY_ARROW))
							{
								if (gSkillManager.GetSkill(lpObj, SKILL_INFINITY_ARROW) != NULL)
								{
									this->SendSkillAttack(lpObj, lpObj->Index, SKILL_INFINITY_ARROW);
								}
							}
						}
					}
				}
			}
		}
		else
		{
			if (!gEffectManager.CheckEffect(lpObj, EFFECT_GREATER_DEFENSE))
			{
				if (gSkillManager.GetSkill(lpObj, SKILL_GREATER_DEFENSE) != NULL)
				{
					this->SendSkillAttack(lpObj, lpObj->Index, SKILL_GREATER_DEFENSE);
				}
			}

			if (!gEffectManager.CheckEffect(lpObj, EFFECT_GREATER_DAMAGE))
			{
				if (gSkillManager.GetSkill(lpObj, SKILL_GREATER_DAMAGE) != NULL)
				{
					this->SendSkillAttack(lpObj, lpObj->Index, SKILL_GREATER_DAMAGE);
				}
			}

			if (gSkillManager.GetSkill(lpObj, SKILL_HEAL) != NULL)
			{
				this->SendSkillAttack(lpObj, lpObj->Index, SKILL_HEAL);
			}

			if (!gEffectManager.CheckEffect(lpObj, EFFECT_INFINITY_ARROW))
			{
				if (gSkillManager.GetSkill(lpObj, SKILL_INFINITY_ARROW) != NULL)
				{
					this->SendSkillAttack(lpObj, lpObj->Index, SKILL_INFINITY_ARROW);
				}
			}
		}
	}
}

void CCustomAttack::SendSkillAttack(LPOBJ lpObj,int aIndex,int SkillNumber) // OK
{
	PMSG_SKILL_ATTACK_RECV pMsg;

	pMsg.header.set(0x19,sizeof(pMsg));

	#if(GAMESERVER_UPDATE>=701)

	pMsg.skillH = SET_NUMBERHB(SkillNumber);

	pMsg.skillL = SET_NUMBERLB(SkillNumber);

	pMsg.indexH = SET_NUMBERHB(aIndex);

	pMsg.indexL = SET_NUMBERLB(aIndex);

	#else

	pMsg.skill[0] = SET_NUMBERHB(SkillNumber);

	pMsg.skill[1] = SET_NUMBERLB(SkillNumber);

	pMsg.index[0] = SET_NUMBERHB(aIndex);

	pMsg.index[1] = SET_NUMBERLB(aIndex);

	#endif

	pMsg.dis = 0;

	gSkillManager.CGSkillAttackRecv(&pMsg,lpObj->Index);
}

void CCustomAttack::SendMultiSkillAttack(LPOBJ lpObj,int aIndex,int SkillNumber) // OK
{
	this->SendDurationSkillAttack(lpObj,aIndex,SkillNumber);

	BYTE send[256];

	PMSG_MULTI_SKILL_ATTACK_RECV pMsg;

	pMsg.header.set(PROTOCOL_CODE4,sizeof(pMsg));

	int size = sizeof(pMsg);

	#if(GAMESERVER_UPDATE>=701)

	pMsg.skillH = SET_NUMBERHB(SkillNumber);

	pMsg.skillL = SET_NUMBERLB(SkillNumber);

	#else

	pMsg.skill[0] = SET_NUMBERHB(SkillNumber);

	pMsg.skill[1] = SET_NUMBERLB(SkillNumber);

	#endif

	pMsg.x = (BYTE)lpObj->X;

	pMsg.y = (BYTE)lpObj->Y;

	pMsg.serial = 0;

	pMsg.count = 0;

	PMSG_MULTI_SKILL_ATTACK info;

	for(int n=0;n < MAX_VIEWPORT;n++)
	{
		if(lpObj->VpPlayer2[n].state == VIEWPORT_NONE || OBJECT_RANGE(lpObj->VpPlayer2[n].index) == 0 || lpObj->VpPlayer2[n].type != OBJECT_MONSTER)
		{
			continue;
		}

		int index = lpObj->VpPlayer2[n].index;

		if(gSkillManager.CheckSkillTarget(lpObj,index,aIndex,lpObj->VpPlayer2[n].type) == 0)
		{
			continue;
		}

		if(gSkillManager.CheckSkillRadio(SkillNumber,lpObj->X,lpObj->Y,gObj[index].X,gObj[index].Y) == 0)
		{
			continue;
		}

		#if(GAMESERVER_UPDATE>=701)

		info.indexH = SET_NUMBERHB(index);

		info.indexL = SET_NUMBERLB(index);

		#else

		info.index[0] = SET_NUMBERHB(index);

		info.index[1] = SET_NUMBERLB(index);

		#endif

		info.MagicKey = 0;

		memcpy(&send[size],&info,sizeof(info));
		size += sizeof(info);

		if(CHECK_SKILL_ATTACK_COUNT(pMsg.count) == 0)
		{
			break;
		}
	}

	pMsg.header.size = size;

	memcpy(send,&pMsg,sizeof(pMsg));

	gSkillManager.CGMultiSkillAttackRecv((PMSG_MULTI_SKILL_ATTACK_RECV*)send,lpObj->Index,0);
}

void CCustomAttack::SendDurationSkillAttack(LPOBJ lpObj,int aIndex,int SkillNumber) // OK
{
	PMSG_DURATION_SKILL_ATTACK_RECV pMsg;

	pMsg.header.set(0x1E,sizeof(pMsg));

	#if(GAMESERVER_UPDATE>=701)

	pMsg.skillH = SET_NUMBERHB(SkillNumber);

	pMsg.skillL = SET_NUMBERLB(SkillNumber);

	#else

	pMsg.skill[0] = SET_NUMBERHB(SkillNumber);

	pMsg.skill[1] = SET_NUMBERLB(SkillNumber);

	#endif

	pMsg.x = (BYTE)gObj[aIndex].X;

	pMsg.y = (BYTE)gObj[aIndex].Y;

	pMsg.dir = (gSkillManager.GetSkillAngle(gObj[aIndex].X,gObj[aIndex].Y,lpObj->X,lpObj->Y)*255)/360;

	pMsg.dis = 0;

	pMsg.angle = (gSkillManager.GetSkillAngle(lpObj->X,lpObj->Y,gObj[aIndex].X,gObj[aIndex].Y)*255)/360;

	#if(GAMESERVER_UPDATE>=803)

	pMsg.indexH = SET_NUMBERHB(aIndex);

	pMsg.indexL = SET_NUMBERLB(aIndex);

	#else

	pMsg.index[0] = SET_NUMBERHB(aIndex);

	pMsg.index[1] = SET_NUMBERLB(aIndex);

	#endif

	pMsg.MagicKey = 0;

	if (SkillNumber == SKILL_RAGEFUL_BLOW || SkillNumber == SKILL_FROZEN_STAB)
	{
		if ((GetTickCount() - lpObj->RagefulTick) < 1200)
		{
			return;
		}
		lpObj->RagefulTick = GetTickCount();
	}

	gSkillManager.CGDurationSkillAttackRecv(&pMsg,lpObj->Index);
}

void CCustomAttack::SendRFSkillAttack(LPOBJ lpObj,int aIndex,int SkillNumber) // OK
{
	PMSG_SKILL_DARK_SIDE_RECV Msg;

	Msg.skill[0] = SET_NUMBERHB(SkillNumber);

	Msg.skill[1] = SET_NUMBERLB(SkillNumber);

	Msg.index[0] = SET_NUMBERHB(aIndex);

	Msg.index[1] = SET_NUMBERLB(aIndex);

	gSkillManager.CGSkillDarkSideRecv(&Msg,lpObj->Index);

	PMSG_RAGE_FIGHTER_SKILL_ATTACK_RECV pMsg;

	pMsg.header.set(0x19,sizeof(pMsg));

	#if(GAMESERVER_UPDATE>=701)

	pMsg.skillH = SET_NUMBERHB(SkillNumber);

	pMsg.skillL = SET_NUMBERLB(SkillNumber);

	pMsg.indexH = SET_NUMBERHB(aIndex);

	pMsg.indexL = SET_NUMBERLB(aIndex);

	#else

	pMsg.skill[0] = SET_NUMBERHB(SkillNumber);

	pMsg.skill[1] = SET_NUMBERLB(SkillNumber);

	pMsg.index[0] = SET_NUMBERHB(aIndex);

	pMsg.index[1] = SET_NUMBERLB(aIndex);

	#endif

	pMsg.dis = 0;

	//gSkillManager.CGSkillAttackRecv(&pMsg,lpObj->Index);
	gSkillManager.CGRageFighterSkillAttackRecv(&pMsg,lpObj->Index);
	this->SendDurationSkillAttack(lpObj,aIndex,SkillNumber);
}

void CCustomAttack::GetItemOnFloor(LPOBJ lpObj)
{
	for (int c=0; c<MAX_MAP_ITEM; c++)
	{
		if (gMap[lpObj->Map].m_Item[c].IsItem() == 0)
		{
			continue;
		}

		for (int i=0; i<6; i++)
		{
			for (int n=0; n<6; n++)
			{
				int X = (gMap[lpObj->Map].m_Item[c].m_X - 3) + i;
				int Y = (gMap[lpObj->Map].m_Item[c].m_Y - 3) + n;

				if (X == lpObj->X && Y == lpObj->Y)
				{
					for(std::vector<OFFEXP_ITEMS>::iterator it=this->m_OffExpItems.begin();it != this->m_OffExpItems.end();it++)
					{
						PMSG_ITEM_GET_RECV pMsg;
						pMsg.index[0] = SET_NUMBERHB(c);
						pMsg.index[1] = SET_NUMBERLB(c);

						if (gMap[lpObj->Map].m_Item[c].m_Index == GET_ITEM(14, 15))
						{
							if (this->m_CustomAttackGetMoney[lpObj->AccountLevel])
							{
								gItemManager.CGItemGetRecv(&pMsg, lpObj->Index);
							}
						}
						else if (gMap[lpObj->Map].m_Item[c].IsExcItem())
						{
							if (this->m_CustomAttackGetExc[lpObj->AccountLevel])
							{
								gItemManager.CGItemGetRecv(&pMsg, lpObj->Index);
							}
						}
						else if (gMap[lpObj->Map].m_Item[c].IsSetItem())
						{
							if (this->m_CustomAttackGetAnc[lpObj->AccountLevel])
							{
								gItemManager.CGItemGetRecv(&pMsg, lpObj->Index);
							}
						}
						else if (gMap[lpObj->Map].m_Item[c].IsSocketItem())
						{
							if (this->m_CustomAttackGetSocket[lpObj->AccountLevel])
							{
								gItemManager.CGItemGetRecv(&pMsg, lpObj->Index);
							}
						}
						else if (gMap[lpObj->Map].m_Item[c].m_Index == GET_ITEM(it->Group, it->Index))
						{
							if (this->m_CustomAttackGetSelectedItems[lpObj->AccountLevel])
							{
								gItemManager.CGItemGetRecv(&pMsg, lpObj->Index);
							}
						}
					}
				}				
			}
		}
	}
}

#if USE_OFFEXP_RELOAD == TRUE
void CCustomAttack::RestoreOffline()
{
//	for (int i=0;i<this->m_Data.size();i++)
	for(std::map<std::string,OFFEXP_DATA>::iterator it=this->m_Data.begin();it != this->m_Data.end();it++)
	{
		if (gObjFindByAcc(it->second.Account) != 0)
		{
			continue;
		}

		int aIndex = gObjAddSearch(0,it->second.IP);

		if(aIndex >= 0)
		{
			gObjAdd(0,it->second.IP,aIndex);
			gObj[aIndex].LoginMessageSend++;
			GJConnectAccountSend(aIndex,it->second.Account,it->second.Password,it->second.IP);
			//gObj[aIndex].Connected = OBJECT_LOGGED;
			gObj[aIndex].Socket = INVALID_SOCKET;
			/*memcpy(gObj[aIndex].Account,it->second.Account,sizeof(gObj[aIndex].Account));
			GDCharacterInfoSend(aIndex,it->second.Name);

			gObj[aIndex].IsOffExpReconnect = true;*/

			LogAdd(LOG_BLACK,"[ OfflineMode ]  [%s|%s] restored", it->second.Account,it->second.Name);
		}
	}
}

OFFEXP_DATA* CCustomAttack::GetOffExpInfo(LPOBJ lpObj)
{
	std::map<std::string,OFFEXP_DATA>::iterator it = this->m_Data.find(lpObj->Account);

	if(it != this->m_Data.end())
	{
		if (strcmp(lpObj->Name,it->second.Name) == 0)
		{
			return &it->second;
		}
	}

	return 0;
}

OFFEXP_DATA* CCustomAttack::GetOffExpInfoByAccount(LPOBJ lpObj)
{
	std::map<std::string,OFFEXP_DATA>::iterator it = this->m_Data.find(lpObj->Account);

	if(it != this->m_Data.end())
	{
		return &it->second;
	}

	return 0;
}


void CCustomAttack::LoadOffExp(char* path)
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

			OFFEXP_DATA info;

			strcpy_s(info.Account,lpMemScript->GetString());
			strcpy_s(info.Password,lpMemScript->GetAsString());
			strcpy_s(info.Name,lpMemScript->GetAsString());
			info.SkillID = lpMemScript->GetAsNumber();
			info.UseBuffs = lpMemScript->GetAsNumber();
			info.Pick = lpMemScript->GetAsNumber();
			strcpy_s(info.IP,lpMemScript->GetAsString());

			this->m_Data.insert(std::pair<std::string,OFFEXP_DATA>(info.Account,info));
		}
	}
	catch(...)
	{
		ErrorMessageBox(lpMemScript->GetLastError());
	}

	delete lpMemScript;
	//int n = 0;
	//std::fstream F;
	////открываем файл в режиме чтения
	//F.open(path);

	//std::string str;
	//std::list<std::string> Strings;

	////string del = "DELETE";


	////если открытие файла прошло корректно, то
	//if (F)
	//{
	//	//цикл для чтения значений из файла; выполнение цикла прервется,
	//	//когда достигнем конца файла, в этом случае F.eof() вернет истину.
	//	while (!F.eof())
	//	{
	//		std::getline(F, str);
	//		Strings.push_back(str);
	//	}

	//	F.close();

	//	//auto it = Strings.begin();

	//	/*for (; it != Strings.end(); it++)
	//		cout << *it << endl;*/
	//}
	//else
	//{
	//	LogAdd(LOG_TEST,"Cant Open File");
	//}

	//for (auto it = Strings.begin(); it != Strings.end(); it++) //проводим из списка значения те что остались
	//{
	//	LogAdd(LOG_TEST,"%s ",it->c_str());
	//	//cout << *it << endl;
	//}


	//std::ofstream fout("C:\\Users\\Studia\\Desktop\\1.txt", std::ios_base::out | std::ios_base::trunc); //открываем файл для записи, предварительно очищая его


	//for (auto it = Strings.begin(); it != Strings.end(); it++) //проводим из списка значения те что остались
	//{
	//	fout << *it << std::endl; // вывод в консоль
	//} 



	//fout.close(); //закрываем файл


	//если открытие файла прошло некорректно, то вывод
	//сообщения об отсутствии такого файла
	//else cout<<" Файл не существует"<<endl;
}


void CCustomAttack::AddToList(LPOBJ lpObj)
{
	if (this->m_Data.find(lpObj->Account) != this->m_Data.end())
	{
		return;
	}

	OFFEXP_DATA info;

	strcpy_s(info.Account,lpObj->Account);
	strcpy_s(info.Password,lpObj->Password);
	strcpy_s(info.Name,lpObj->Name);
	info.SkillID = lpObj->AttackCustomSkill;
	info.UseBuffs = lpObj->OfflineMode.UseBuff;
	strcpy_s(info.IP,lpObj->IpAddr);

	if (lpObj->OfflineMode.PickZen != 0)
	{
		info.Pick+=1;
	}
	if (lpObj->OfflineMode.PickJewels != 0)
	{
		info.Pick+=2;
	}
	if (lpObj->OfflineMode.PickExe != 0)
	{
		info.Pick+=4;
	}
	if (lpObj->OfflineMode.PickAnc != 0)
	{
		info.Pick+=8;
	}
	if (lpObj->OfflineMode.PickSelected != 0)
	{
		info.Pick+=16;
	}

	this->m_Data.insert(std::pair<std::string,OFFEXP_DATA>(info.Account,info));

	this->WriteToFile();
}

void CCustomAttack::DelFromList(char* account)
{
	std::map<std::string,OFFEXP_DATA>::iterator it = this->m_Data.find(account);

	if(it != this->m_Data.end())
	{
		this->m_Data.erase(it);
	}

	this->WriteToFile();
}

void CCustomAttack::WriteToFile()
{
	std::ofstream fout(".\\OffExp.ini", std::ios_base::out | std::ios_base::trunc); //открываем файл для записи, предварительно очищая его

	//for	(int i=0;i<this->m_Data.size();i++)
	for(std::map<std::string,OFFEXP_DATA>::iterator it=this->m_Data.begin();it != this->m_Data.end();it++)
	{
		fout << "\"" << it->second.Account << "\"" << "\t"  << "\"" << it->second.Password  << "\"" << "\t"  << "\"" << it->second.Name  << "\"" << "\t" << it->second.SkillID << "\t" << it->second.UseBuffs << "\t" << it->second.Pick << "\t"  << "\"" << it->second.IP  << "\"" << std::endl; // вывод в консоль
	}

	fout<<"end";

	fout.close(); //закрываем файл
}
#endif