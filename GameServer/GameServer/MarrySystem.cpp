#include "stdafx.h"
#include "MarrySystem.h"
#include "Protection.h"
#include "Message.h"
#include "Notice.h"
#include "Util.h"
#include "CommandManager.h"
#include "DefaultClassInfo.h"
#include "CashShop.h"
#include "Map.h"
#include "PcPoint.h"

CMarrySystem gMarrySystem;

CMarrySystem::CMarrySystem() // Constructor
{
	
}

CMarrySystem::~CMarrySystem() // Destructor
{
	
}

void CMarrySystem::ResetConfigs()
{
    Config.MarrySystemEnabled = 0;
    Config.MarryNpcNum = 0;
    Config.NumWitnesses = 0; 
    Config.DivorceNpcNum = 0;

	for (int i=0; i < 4; i++)
	{
		Config.MarryHomoSexual[i] = 0;
		Config.MarryCostZen[i] = 0;
		Config.MarryCostPCPoint[i] = 0;
		Config.MarryCostWCoinC[i] = 0;
		Config.MarryCostWCoinP[i] = 0;
		Config.MarryMinLvl[i] = 0;
		Config.IsMarry[i] = 0;
		Config.MarryTrace[i] = 0;
		Config.MarryTraceLvl[i] = 0;
		Config.MarryOnline[i] = 0;
		Config.MarryExpBonus[i] = 0;
		Config.IsDivorce[i] = 0;
		Config.DivorceCostZen[i] = 0;
		Config.DivorceCostPCPoint[i] = 0;
		Config.DivorceCostWCoinC[i] = 0;
		Config.DivorceCostWCoinP[i] = 0;
		Config.DivorceMinLvl[i] = 0;
	}

    Config.TakeItemsForMarry = 0;
//	Config.TakeItemsForDivorce = 0;

    for (int i=0; i < MAXMARRYITEM; i++)
    {
        Config.MarryItems[i].Num = 0;

        Config.MarryItems[i].Group = 0;

        Config.MarryItems[i].Type = 0;

        Config.MarryItems[i].Lvl = 0;

        Config.MarryItems[i].Luck = 0;

        Config.MarryItems[i].Skill = 0;

        Config.MarryItems[i].Opt = 0;

        Config.MarryItems[i].Exc = 0;

    }
}

void CMarrySystem::Load(char* section,char* path)
{
	if (gProtection.GetCustomState(CUSTOM_MARRY) == 0)
	{
		return;
	}

	ResetConfigs();

	Config.MarrySystemEnabled = GetPrivateProfileInt(section, "MarrySystemEnabled", 0, path);

    if(!Config.MarrySystemEnabled)
	{
        return;
	}

	Config.MarryHomoSexual[0] = GetPrivateProfileInt(section, "MarryHomoSexual_AL0", 0, path);
	Config.MarryHomoSexual[1] = GetPrivateProfileInt(section, "MarryHomoSexual_AL1", 0, path);
	Config.MarryHomoSexual[2] = GetPrivateProfileInt(section, "MarryHomoSexual_AL2", 0, path);
	Config.MarryHomoSexual[3] = GetPrivateProfileInt(section, "MarryHomoSexual_AL3", 0, path);

	Config.MarryCostZen[0] = GetPrivateProfileInt(section, "MarryCostZen_AL0", 0, path);
	Config.MarryCostZen[1] = GetPrivateProfileInt(section, "MarryCostZen_AL1", 0, path);
	Config.MarryCostZen[2] = GetPrivateProfileInt(section, "MarryCostZen_AL2", 0, path);
	Config.MarryCostZen[3] = GetPrivateProfileInt(section, "MarryCostZen_AL3", 0, path);

	Config.MarryCostPCPoint[0] = GetPrivateProfileInt(section, "MarryCostPCPoint_AL0", 0, path);
	Config.MarryCostPCPoint[1] = GetPrivateProfileInt(section, "MarryCostPCPoint_AL1", 0, path);
	Config.MarryCostPCPoint[2] = GetPrivateProfileInt(section, "MarryCostPCPoint_AL2", 0, path);
	Config.MarryCostPCPoint[3] = GetPrivateProfileInt(section, "MarryCostPCPoint_AL3", 0, path);

	Config.MarryCostWCoinP[0] = GetPrivateProfileInt(section, "MarryCostWCoinP_AL0", 0, path);
	Config.MarryCostWCoinP[1] = GetPrivateProfileInt(section, "MarryCostWCoinP_AL1", 0, path);
	Config.MarryCostWCoinP[2] = GetPrivateProfileInt(section, "MarryCostWCoinP_AL2", 0, path);
	Config.MarryCostWCoinP[3] = GetPrivateProfileInt(section, "MarryCostWCoinP_AL3", 0, path);

	Config.MarryCostWCoinC[0] = GetPrivateProfileInt(section, "MarryCostWCoinC_AL0", 0, path);
	Config.MarryCostWCoinC[1] = GetPrivateProfileInt(section, "MarryCostWCoinC_AL1", 0, path);
	Config.MarryCostWCoinC[2] = GetPrivateProfileInt(section, "MarryCostWCoinC_AL2", 0, path);
	Config.MarryCostWCoinC[3] = GetPrivateProfileInt(section, "MarryCostWCoinC_AL3", 0, path);

	Config.MarryMinLvl[0] = GetPrivateProfileInt(section, "MarryMinLvl_AL0", 0, path);
	Config.MarryMinLvl[1] = GetPrivateProfileInt(section, "MarryMinLvl_AL1", 0, path);
	Config.MarryMinLvl[2] = GetPrivateProfileInt(section, "MarryMinLvl_AL2", 0, path);
	Config.MarryMinLvl[3] = GetPrivateProfileInt(section, "MarryMinLvl_AL3", 0, path);

	Config.MarryNpcNum = GetPrivateProfileInt(section, "MarryNpcNum", 0, path);

	Config.MarryTrace[0] = GetPrivateProfileInt(section, "MarryTrace_AL0", 0, path);
	Config.MarryTrace[1] = GetPrivateProfileInt(section, "MarryTrace_AL1", 0, path);
	Config.MarryTrace[2] = GetPrivateProfileInt(section, "MarryTrace_AL2", 0, path);
	Config.MarryTrace[3] = GetPrivateProfileInt(section, "MarryTrace_AL3", 0, path);

	Config.MarryOnline[0] = GetPrivateProfileInt(section, "MarryOnline_AL0", 0, path);
	Config.MarryOnline[1] = GetPrivateProfileInt(section, "MarryOnline_AL1", 0, path);
	Config.MarryOnline[2] = GetPrivateProfileInt(section, "MarryOnline_AL2", 0, path);
	Config.MarryOnline[3] = GetPrivateProfileInt(section, "MarryOnline_AL3", 0, path);

	Config.IsMarry[0] = GetPrivateProfileInt(section, "IsMarry_AL0", 0, path);
	Config.IsMarry[1] = GetPrivateProfileInt(section, "IsMarry_AL1", 0, path);
	Config.IsMarry[2] = GetPrivateProfileInt(section, "IsMarry_AL2", 0, path);
	Config.IsMarry[3] = GetPrivateProfileInt(section, "IsMarry_AL3", 0, path);

	Config.MarryTraceLvl[0] = GetPrivateProfileInt(section, "MarryTraceLvlReq_AL0", 0, path);
	Config.MarryTraceLvl[1] = GetPrivateProfileInt(section, "MarryTraceLvlReq_AL1", 0, path);
	Config.MarryTraceLvl[2] = GetPrivateProfileInt(section, "MarryTraceLvlReq_AL2", 0, path);
	Config.MarryTraceLvl[3] = GetPrivateProfileInt(section, "MarryTraceLvlReq_AL3", 0, path);

	Config.MarryExpBonus[0] = GetPrivateProfileInt(section, "MarryExpBonus_AL0", 0, path);
	Config.MarryExpBonus[1] = GetPrivateProfileInt(section, "MarryExpBonus_AL1", 0, path);
	Config.MarryExpBonus[2] = GetPrivateProfileInt(section, "MarryExpBonus_AL2", 0, path);
	Config.MarryExpBonus[3] = GetPrivateProfileInt(section, "MarryExpBonus_AL3", 0, path);

	Config.NumWitnesses = GetPrivateProfileInt(section, "MarryNumWitnesses", 0, path);

	Config.IsDivorce[0] = GetPrivateProfileInt(section, "DivorceEnabled_AL0", 0, path);
	Config.IsDivorce[1] = GetPrivateProfileInt(section, "DivorceEnabled_AL1", 0, path);
	Config.IsDivorce[2] = GetPrivateProfileInt(section, "DivorceEnabled_AL2", 0, path);
	Config.IsDivorce[3] = GetPrivateProfileInt(section, "DivorceEnabled_AL3", 0, path);

	Config.DivorceCostZen[0] = GetPrivateProfileInt(section, "DivorceCostZen_AL0", 0, path);
	Config.DivorceCostZen[1] = GetPrivateProfileInt(section, "DivorceCostZen_AL1", 0, path);
	Config.DivorceCostZen[2] = GetPrivateProfileInt(section, "DivorceCostZen_AL2", 0, path);
	Config.DivorceCostZen[3] = GetPrivateProfileInt(section, "DivorceCostZen_AL3", 0, path);

	Config.DivorceCostPCPoint[0] = GetPrivateProfileInt(section, "DivorceCostPCPoint_AL0", 0, path);
	Config.DivorceCostPCPoint[1] = GetPrivateProfileInt(section, "DivorceCostPCPoint_AL1", 0, path);
	Config.DivorceCostPCPoint[2] = GetPrivateProfileInt(section, "DivorceCostPCPoint_AL2", 0, path);
	Config.DivorceCostPCPoint[3] = GetPrivateProfileInt(section, "DivorceCostPCPoint_AL3", 0, path);

	Config.DivorceCostWCoinC[0] = GetPrivateProfileInt(section, "DivorceCostWCoinC_AL0", 0, path);
	Config.DivorceCostWCoinC[1] = GetPrivateProfileInt(section, "DivorceCostWCoinC_AL1", 0, path);
	Config.DivorceCostWCoinC[2] = GetPrivateProfileInt(section, "DivorceCostWCoinC_AL2", 0, path);
	Config.DivorceCostWCoinC[3] = GetPrivateProfileInt(section, "DivorceCostWCoinC_AL3", 0, path);

	Config.DivorceCostWCoinP[0] = GetPrivateProfileInt(section, "DivorceCostWCoinP_AL0", 0, path);
	Config.DivorceCostWCoinP[1] = GetPrivateProfileInt(section, "DivorceCostWCoinP_AL1", 0, path);
	Config.DivorceCostWCoinP[2] = GetPrivateProfileInt(section, "DivorceCostWCoinP_AL2", 0, path);
	Config.DivorceCostWCoinP[3] = GetPrivateProfileInt(section, "DivorceCostWCoinP_AL3", 0, path);

	Config.DivorceMinLvl[0] = GetPrivateProfileInt(section, "DivorceMinLvl_AL0", 0, path);
	Config.DivorceMinLvl[1] = GetPrivateProfileInt(section, "DivorceMinLvl_AL1", 0, path);
	Config.DivorceMinLvl[2] = GetPrivateProfileInt(section, "DivorceMinLvl_AL2", 0, path);
	Config.DivorceMinLvl[3] = GetPrivateProfileInt(section, "DivorceMinLvl_AL3", 0, path);

	Config.TakeItemsForMarry = GetPrivateProfileInt(section, "TakeItemsForMarry", 0, path);


    char Items[40];

    if(Config.TakeItemsForMarry != 0)
    {
        for(int y = 0; y < Config.TakeItemsForMarry; y++)
        {
            wsprintf(Items, "Config.Item[%d].Num", y+1);
            Config.MarryItems[y].Num		= GetPrivateProfileInt(section, Items, 0, path);

            wsprintf(Items, "Config.Item[%d].Index", y+1);
            Config.MarryItems[y].Group	= GetPrivateProfileInt(section, Items, 0, path);

            wsprintf(Items, "Config.Item[%d].Type", y+1);
            Config.MarryItems[y].Type	= GetPrivateProfileInt(section, Items, 0, path);

            wsprintf(Items, "Config.Item[%d].Lvl", y+1);
            Config.MarryItems[y].Lvl		= GetPrivateProfileInt(section, Items, 0, path);

            wsprintf(Items, "Config.Item[%d].Luck", y+1);
            Config.MarryItems[y].Luck	= GetPrivateProfileInt(section, Items, 0, path);

            wsprintf(Items, "Config.Item[%d].Skill", y+1);
            Config.MarryItems[y].Skill	= GetPrivateProfileInt(section, Items, 0, path);

            wsprintf(Items, "Config.Item[%d].Opt", y+1);
            Config.MarryItems[y].Opt		= GetPrivateProfileInt(section, Items, 0, path);

            wsprintf(Items, "Config.Item[%d].Exc", y+1);
            Config.MarryItems[y].Exc		= GetPrivateProfileInt(section, Items, 0, path);
        }
    }
}

bool CMarrySystem::CheckGender(LPOBJ lpObj, LPOBJ lpTarget)
{
	if (gProtection.GetCustomState(CUSTOM_MARRY) == 0)
	{
		return false;
	}

	if (lpTarget == NULL || lpObj == NULL)
	{
        return false;
	}

    int aGender = MAN, tGender = MAN;

	if (lpObj->Class == CLASS_FE || lpObj->Class == CLASS_SU)
	{
		aGender = WOMAN;
	}
	else
	{
		aGender = MAN;
	}
 

	if (lpTarget->Class == CLASS_FE || lpTarget->Class == CLASS_SU)
	{
		tGender = WOMAN;
	}
	else
	{
		tGender = MAN;
	}

    if(tGender == aGender)
    {
		if(Config.MarryHomoSexual[lpObj->AccountLevel] == 1)
        {
            if(tGender == MAN)
            {
				gNotice.GCNoticeSendToAll(0,0,0,0,0,0,"[Marriage] It would be gey wedding!");
                return true;
            }
            else
            {
				gNotice.GCNoticeSendToAll(0,0,0,0,0,0,"[Marriage] It would be lesbian wedding!");
                return true;
            }
        }
        else
        {
            gNotice.GCNoticeSendToAll(0,0,0,0,0,0,"[Marriage] We don't support untraditional weddings!!! Marriage canceled!!!");
            return false;
        }
    }
    return true;
}

unsigned long __stdcall TickMarriage(LPOBJ lpObj)
{
	if (gProtection.GetCustomState(CUSTOM_MARRY) == 0)
	{
		return 0;
	}

    LPOBJ nObj = gMarrySystem.NpcObj;
    LPOBJ kObj1 = gMarrySystem.gObj1;
    LPOBJ kObj2 = gMarrySystem.gObj2;

    gMarrySystem.NpcUse = true;

    int i;

    if(nObj == NULL)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,"[Marriage] Enter /yes or /no. You have 30s.");
	}
    else
    {
        Sleep(3000);
        if(!gMarrySystem.NpcUse)
		{
			return 1;
		}

        gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,"[Marriage] Enter /yes or /no. You have 30s.");
    }

    for(i = 0; i < 150; i++)
    {
        Sleep(100);
        if(!gMarrySystem.NpcUse) return 1;
    }

    if(nObj == NULL)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,"[Marriage] 15 seconds to answer.");
	}
    else
	{
		GCChatTargetSend(lpObj,nObj->Index,"[Marriage] 15 seconds to answer.");
        //gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,"[Marriage] 15 seconds to answer.");
	}

    for(i = 0; i < 50; i++)
    {
        Sleep(100);
        if(!gMarrySystem.NpcUse) return 1;
    }

    if(nObj == NULL)
	{
        gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,"[Marriage] 10 seconds to answer.");
	}
    else
	{
		GCChatTargetSend(lpObj,nObj->Index,"[Marriage] 10 seconds to answer.");
        //gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,"[Marriage] 10 seconds to answer.");
	}

    for(i = 0; i < 50; i++)
    {
        Sleep(100);
        if(!gMarrySystem.NpcUse) return 1;
    }

    if(nObj == NULL)
	{
        gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,"[Marriage] 5 seconds to answer.");
	}
    else
	{
		GCChatTargetSend(lpObj,nObj->Index,"[Marriage] 5 seconds to answer.");
       // gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,"[Marriage] 5 seconds to answer.");
	}

    for(i = 0; i < 20; i++)
    {
        Sleep(100);

        if(!gMarrySystem.NpcUse) return 1;
    }

    if(nObj == NULL)
	{
        gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,"[Marriage] 3 seconds to answer.");
	}
    else
	{
		GCChatTargetSend(lpObj,nObj->Index,"[Marriage] 3 seconds to answer.");
        //gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,"[Marriage] 3 seconds to answer.");
	}

    for(i = 0; i < 10; i++)
    {
        Sleep(100);

        if(!gMarrySystem.NpcUse) return 1;
    }
    if(nObj == NULL)
	{
        gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,"[Marriage] 2 seconds to answer.");
	}
    else
	{
		GCChatTargetSend(lpObj,nObj->Index,"[Marriage] 2 seconds to answer.");
        //gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,"[Marriage] 2 seconds to answer.");
	}

    for(i = 0; i < 10; i++)
    {
        Sleep(100);
        if(!gMarrySystem.NpcUse) return 1;
    }

    if(nObj == NULL)
	{
        gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,"[Marriage] 1 second to answer.");
	}
    else
	{
		GCChatTargetSend(lpObj,nObj->Index,"[Marriage] 1 seconds to answer.");
        //gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,"[Marriage] 1 second to answer.");
	}

    for(i = 0; i < 10; i++)
    {
        Sleep(100);

        if(!gMarrySystem.NpcUse) return 1;
    }

    if(lpObj->MarryType < 10)
	{
		gMarrySystem.EndMarriageFalse(lpObj);
	}

    return 1;
}


void CMarrySystem::CheckMarryCost(LPOBJ lpObj, LPOBJ lpNpc, int Type)
{
	if (gProtection.GetCustomState(CUSTOM_MARRY) == 0)
	{
		return;
	}

#if(GAMESERVER_UPDATE>=501)
	gCashShop.GDCashShopRecievePointSend(lpObj->Index,(DWORD)&CMarrySystem::CheckMarryCostCallback,(DWORD)Type,(DWORD)lpNpc);
#else
	gPcPoint.GDPcPointRecievePointSend(lpObj->Index,(DWORD)&CMarrySystem::CheckMarryCostCallback,(DWORD)Type,(DWORD)lpNpc);
#endif
}

void CMarrySystem::CheckMarryCostCallback(LPOBJ lpObj,int Type,LPOBJ lpNpc,DWORD WCoinC,DWORD WCoinP,DWORD GoblinPoint) // OK
{
	if (gProtection.GetCustomState(CUSTOM_MARRY) == 0)
	{
		return;
	}

	WCOINS_STRUCT_MAARRY wcstruct;
	wcstruct.WCoinC = WCoinC;
	wcstruct.WCoinP = WCoinP;
	wcstruct.GoblinPoint = GoblinPoint;

	if(lpObj->IsMarried == 0 && lpObj->MarryType == -1 && gMarrySystem.gObj1 == NULL && gMarrySystem.gObj2 == NULL)
    {
		if(!gMarrySystem.CheckCost(lpObj, wcstruct, 0)) return;

		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,"[Marriage] Do you really want to marry?");

		gMarrySystem.NpcObj = lpNpc;
        lpObj->MarryType = 0;
        gMarrySystem.gObj1 = lpObj;
        DWORD ThreadID;
		HANDLE hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)TickMarriage, (void*)lpObj, 0, &ThreadID);
		
        if ( hThread == 0 )
        {
			LogAdd(LOG_RED, "CreateThread() failed with error %d", GetLastError());
            return;
        }

        CloseHandle(hThread);
	}
	else if(lpObj->IsMarried == 0 && gMarrySystem.gObj2 != NULL && gMarrySystem.gObj1 == lpObj)
    {
        if(!gMarrySystem.CheckCost(lpObj, wcstruct, 1))
        {
			gMarrySystem.CancelMarry(lpObj);
            return;
        }

		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,"[Marriage] %s, do you really want to marry on %s?",lpObj->Name , gMarrySystem.gObj2->Name);

        gMarrySystem.NpcObj = lpNpc;
        lpObj->MarryType = 1;

        DWORD ThreadID;
        HANDLE hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)TickMarriage, (void*)lpObj, 0, &ThreadID);

        if ( hThread == 0 )
        {
            LogAdd(LOG_RED, "CreateThread() failed with error %d", GetLastError());
            return;
        }

        CloseHandle(hThread);
    }
	else if(lpObj->IsMarried == 0 && gMarrySystem.gObj1 != NULL && lpObj->MarryType == -1)
    {
        if(!gMarrySystem.CheckGender(lpObj, gMarrySystem.gObj1))
        {
			gMarrySystem.CancelMarry(lpObj);
            return;
        }

		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,"[Marriage] %s, do you really want to marry on %s?",lpObj->Name , gMarrySystem.gObj1->Name);

        gMarrySystem.NpcObj = lpNpc;
        lpObj->MarryType = 2;

        gMarrySystem.gObj2 = lpObj;
        DWORD ThreadID;
        HANDLE hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)TickMarriage, (void*)lpObj, 0, &ThreadID);

        if ( hThread == 0 )
        {
			LogAdd(LOG_RED, "CreateThread() failed with error %d", GetLastError());
            return;
        }

        CloseHandle(hThread);
    }
}

bool CMarrySystem::CheckCost(LPOBJ lpObj, WCOINS_STRUCT_MAARRY wcstruct, int type)
{
	if (gProtection.GetCustomState(CUSTOM_MARRY) == 0)
	{
		return 0;
	}

	if(gMarrySystem.Config.MarryMinLvl[lpObj->AccountLevel] > lpObj->Level)
    {
		// Need More LVL
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,"[Marriage] You need more level");
        return false;
    }

	if (lpObj->Money < gMarrySystem.Config.MarryCostZen[lpObj->AccountLevel])
	{
		//Need More zen!
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,"[Marriage] You need more %s!",gMessage.GetMessage(523));
		return false;
	}

	if(gMarrySystem.Config.NumWitnesses > 0)
    {
        int Num = 0;
        for(int i = OBJECT_START_USER; i <= MAX_OBJECT; i++)
        {
			if(!gObjIsConnectedGP(i)) //явл€етс€ ли ќЅ∆ игроком
			{
				continue;
			}
			LPOBJ aObj = &gObj[i];
            if(lpObj->Map != aObj->Map) continue;
            if(lpObj->X <= aObj->X-10 || lpObj->X >= aObj->X+10) continue;
            if(lpObj->Y <= aObj->Y-10 || lpObj->Y >= aObj->Y+10) continue;
            Num++;
        }
        if((Num - 2) < gMarrySystem.Config.NumWitnesses)
        {
            // Need More Witnesses
			gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,"[Marriage] You need more Witnesses");
            return false;
        }
    }

	if (wcstruct.GoblinPoint < gMarrySystem.Config.MarryCostPCPoint[lpObj->AccountLevel])
	{
		// Need more PCPoint
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,"[Marriage] You need more %s!",gMessage.GetMessage(526));
		return false;
	}

	if (wcstruct.WCoinC < gMarrySystem.Config.MarryCostWCoinC[lpObj->AccountLevel])
	{
		// Need more WCoinC
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,"[Marriage] You need more %s!",gMessage.GetMessage(524));
		return false;
	}

	if (wcstruct.WCoinP < gMarrySystem.Config.MarryCostWCoinP[lpObj->AccountLevel])
	{
		// Need more WCoinP
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,"[Marriage] You need more %s!",gMessage.GetMessage(525));
		return false;
	}

	if (type == 1)
	{
		lpObj->Money -= gMarrySystem.Config.MarryCostZen[lpObj->AccountLevel];

		GCMoneySend(lpObj->Index,lpObj->Money);

		#if(GAMESERVER_UPDATE>=501)

		gCashShop.GDCashShopSubPointSaveSend(lpObj->Index,0,gMarrySystem.Config.MarryCostWCoinC[lpObj->AccountLevel],gMarrySystem.Config.MarryCostWCoinP[lpObj->AccountLevel],gMarrySystem.Config.MarryCostPCPoint[lpObj->AccountLevel]);

		#else

		gPcPoint.GDPcPointSubPointSaveSend(lpObj->Index,(DWORD)gMarrySystem.Config.MarryCostPCPoint);

		gPcPoint.GDPcPointPointSend(lpObj->Index);

		#endif
	}

	return true;
}


void CMarrySystem::EndMarriageFalse(LPOBJ lpObj)
{
	if (gProtection.GetCustomState(CUSTOM_MARRY) == 0)
	{
		return;
	}

	if(lpObj->MarryType == 1 || lpObj->MarryType == 2)
    {
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,"%s don't answer me. Marriage canceled", lpObj->Name);
    }
    if(lpObj->MarryType == 0)
    {
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,"You don't answer me. Marriage canceled");
    }

    lpObj->MarryType = -1;

    if(gObj1 != NULL)
        gObj1->MarryType = -1;
    if(gObj2 != NULL)
        gObj2->MarryType = -1;

    NpcUse = false;
    //cMarry.fIndex = 0;
    gObj1 = NULL;
    gObj2 = NULL;
    NpcObj = NULL;
}

void CMarrySystem::FireworkOnTheScreen(LPOBJ gObj)
{
	PMSG_FIREWORKS_SEND pMsg;
	pMsg.header.set(0xF3,0x40,sizeof(pMsg));
	pMsg.type = 0;
	for(int x = 0; x < 10; x+=2)
	{
		for(int y = 0; y < 10; y+=2)
		{
			pMsg.x = gObj->X + x - 5;
			pMsg.y = gObj->Y + y - 5;
			DataSend(gObj->Index,(BYTE*)&pMsg,pMsg.header.size);
			MsgSendV2(gObj,(BYTE*)&pMsg,pMsg.header.size);
		}
	}
}

unsigned long __stdcall FireMarriage()
{
    LPOBJ nObj = gMarrySystem.NpcObj;
    LPOBJ kObj1 = gMarrySystem.gObj1;
    LPOBJ kObj2 = gMarrySystem.gObj2;
	LogAdd(LOG_RED, "Name %s Name %s", kObj1->Name, kObj2->Name);

	PMSG_FIREWORKS_SEND pMsg;
	pMsg.header.set(0xF3,0x40,sizeof(pMsg));
	pMsg.type = 0;
	for(int y = 0; y < 2; y++)
    {

        pMsg.x = (BYTE)kObj1->X;
        pMsg.y = (BYTE)kObj1->Y;
		DataSend(kObj1->Index,(BYTE*)&pMsg,pMsg.header.size);
		MsgSendV2(kObj1,(BYTE*)&pMsg,pMsg.header.size);

        pMsg.x = (BYTE)kObj2->X;
        pMsg.y = (BYTE)kObj2->Y;
		DataSend(kObj2->Index,(BYTE*)&pMsg,pMsg.header.size);
		MsgSendV2(kObj2,(BYTE*)&pMsg,pMsg.header.size);
    }

    Sleep(3000);

    gMarrySystem.FireworkOnTheScreen(kObj1);

    Sleep(1000);

	gNotice.GCNoticeSendToAll(0,0,0,0,0,0,"[Marriage] Congratulations with the wedding %s and %s!", kObj1->Name, kObj2->Name);

    strcpy(gMarrySystem.gObj1->MarryName, gMarrySystem.gObj2->Name);
    strcpy(gMarrySystem.gObj2->MarryName, gMarrySystem.gObj1->Name);
    gMarrySystem.gObj1->IsMarried = 1;
    gMarrySystem.gObj2->IsMarried = 1;

    gMarrySystem.NpcUse = false;
    //Marry.fIndex = 0;
    gMarrySystem.gObj1 = NULL;
    gMarrySystem.gObj2 = NULL;
    gMarrySystem.NpcObj = NULL;

    return 1;
}

void CMarrySystem::EndMarriageTrue()
{
	if (gProtection.GetCustomState(CUSTOM_MARRY) == 0)
	{
		return;
	}

    DWORD ThreadID;
    HANDLE hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)FireMarriage, NULL, 0, &ThreadID);

    if ( hThread == 0 )
    {
        LogAdd(LOG_RED, "CreateThread() failed with error %d", GetLastError());
        return;
    }

    CloseHandle(hThread);
}

bool CMarrySystem::NPCTalk(LPOBJ lpNpc, LPOBJ lpObj)
{
	if (gProtection.GetCustomState(CUSTOM_MARRY) == 0)
	{
		return 0;
	}

	if (lpNpc->Class == Config.MarryNpcNum)
	{
		this->StartMarriage(lpNpc, lpObj);
		return 1;
	}
	return 0;
}

void CMarrySystem::CancelMarry(LPOBJ lpObj)
{
	if (gProtection.GetCustomState(CUSTOM_MARRY) == 0)
	{
		return;
	}

	lpObj->MarryType = -1;
    if(gObj1 != NULL)
        gObj1->MarryType = -1;
    if(gObj2 != NULL)
        gObj2->MarryType = -1;

    NpcUse = false;
    //cMarry.fIndex = 0;
    gObj1 = NULL;
    gObj2 = NULL;
    NpcObj = NULL;
}

void CMarrySystem::StartMarriage(LPOBJ lpNpc, LPOBJ lpObj)
{
	if (gProtection.GetCustomState(CUSTOM_MARRY) == 0)
	{
		return;
	}

	if ( (lpObj->Index < 0 || lpObj->Index > MAX_OBJECT-1) && (lpNpc->Index < 0 || lpNpc->Index > MAX_OBJECT-1)) //¬ходит ли он в границы
	{
        return;
	}

	if(!gObjIsConnectedGP(lpObj->Index)) //явл€етс€ ли ќЅ∆ игроком
	{
        return;
	}
	this->CheckMarryCost(lpObj, lpNpc, 0);
}

bool CMarrySystem::YesCommand(LPOBJ lpObj)
{
	if (gProtection.GetCustomState(CUSTOM_MARRY) == 0)
	{
		return 0;
	}

	if(!this->NpcUse)
	{
		return true;
	}

	int TypeMarry = -1;

	if(lpObj->MarryType != -1)
	{
		TypeMarry = lpObj->MarryType;
	}

	switch(TypeMarry)
	{
		case 0:
		{
			gNotice.GCNoticeSendToAll(0,0,0,0,0,0,"Devias Marriage!");

			this->NpcUse = false;

			break;
		}
		default:
		{
			lpObj->MarryType = 3;

			if(gObj1->MarryType == 3 && gObj2->MarryType == 3)
			{
				this->EndMarriageTrue();	//RUN THEARD
			}

			this->NpcUse = false;

			break;
		}
	}

	return true;
}

bool CMarrySystem::NoCommand(LPOBJ lpObj)
{
	if (gProtection.GetCustomState(CUSTOM_MARRY) == 0)
	{
		return 0;
	}

	if(!this->NpcUse)
	{
		return true;
	}

	int TypeMarry = -1;

	if(lpObj->MarryType != -1)
	{
		TypeMarry = lpObj->MarryType;
	}

	LPOBJ nObj = gMarrySystem.NpcObj;

	switch(TypeMarry)
	{
		case 0:
		{
			if(nObj == NULL)
			{
				gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,"You called off the wedding");
			}
			else
			{
				GCChatTargetSend(lpObj,nObj->Index,"[Marriage] You called off the wedding.");
				//gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,"You called off the wedding");
			}

			if(gMarrySystem.gObj1 != NULL)
			{
				gObj1->MarryType = -1;
			}

			if(gMarrySystem.gObj2 != NULL)
			{
				gObj2->MarryType = -1;
			}

			gMarrySystem.NpcUse = false;

			gMarrySystem.gObj1 = NULL;
			gMarrySystem.gObj2 = NULL;
			gMarrySystem.NpcObj = NULL;

			break;
		}
		default:
		{
			gNotice.GCNoticeSendToAll(0,0,0,0,0,0,"The wedding was canceled by %s!", lpObj->Name);

			if(gMarrySystem.gObj1 != NULL)
			{
				gObj1->MarryType = -1;
			}

			if(gMarrySystem.gObj2 != NULL)
			{
				gObj2->MarryType = -1;
			}

			gMarrySystem.NpcUse = false;

			gMarrySystem.gObj1 = NULL;
			gMarrySystem.gObj2 = NULL;
			gMarrySystem.NpcObj = NULL;

			break;
		}
	}

	return true;
}

void CMarrySystem::IsMarry(LPOBJ lpObj, char* arg)
{
	if (gProtection.GetCustomState(CUSTOM_MARRY) == 0)
	{
		return;
	}

	if (this->Config.IsMarry[lpObj->AccountLevel] != 1)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,"[IsMarry] Function Disabled!");
		return;
	}

	char name[11] = {0};

	gCommandManager.GetString(arg,name,sizeof(name),0);

	LPOBJ lpTarget = gObjFind(name);

	if(lpTarget == 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,"[Marry] Player's offline or doesn't exist!!!");
		return;
	}

	if (lpTarget->IsMarried == 1)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,"[Marry] %s is married on %s!!!", lpTarget->Name, lpTarget->MarryName);
	}
	else
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,"[Marry] %s is not married!!!", lpTarget->Name);
	}

}

void CMarrySystem::IsMarryOnline(LPOBJ lpObj)
{
	if (gProtection.GetCustomState(CUSTOM_MARRY) == 0)
	{
		return;
	}

	if (this->Config.MarryOnline[lpObj->AccountLevel] != 1)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,"[MarryOnline] Function Disabled!");
		return;
	}

	if (lpObj->IsMarried == 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,"[Marry] You are not married!");
		return;
	}

	LPOBJ lpTarget = gObjFind(lpObj->MarryName);

	if (lpTarget == 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,"[Marry] %s is offline", lpObj->MarryName);
	}
	else
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,"[Marry] %s is now online!", lpObj->MarryName);
	}
}

void CMarrySystem::TraceMarry(LPOBJ lpObj)
{
	if (gProtection.GetCustomState(CUSTOM_MARRY) == 0)
	{
		return;
	}

	if (this->Config.MarryTrace[lpObj->AccountLevel] != 1)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,"[TraceMarry] Function Disabled!");
		return;
	}

	if (lpObj->Level < Config.MarryTraceLvl[lpObj->AccountLevel])
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,"[TraceMarry] You need more level!");
	}

	LPOBJ lpTarget = gObjFind(lpObj->MarryName);

	if (lpObj->IsMarried == 0 || lpTarget == 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,"[Marry] You are not married or your marriage is offline!");
		return;
	}

	if((lpTarget->Map == MAP_ICARUS || lpTarget->Map == MAP_KANTURU3) && (lpObj->Inventory[7].IsItem() == 0 && lpObj->Inventory[8].m_Index != GET_ITEM(13,3) && lpObj->Inventory[8].m_Index != GET_ITEM(13,4) && lpObj->Inventory[8].m_Index != GET_ITEM(13,37))) // Dinorant,Fenrir
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(228));
		return;
	}

	if((lpTarget->Map >= 11 && lpTarget->Map <= 17) || lpTarget->Map == 52 || (lpTarget->Map >= 18 && lpTarget->Map <= 23) || lpTarget->Map == 53 || lpTarget->Map == 9 || lpTarget->Map == 32 || (lpTarget->Map >= 45 && lpTarget->Map <= 50))
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,"[Marry] Your marriage is inside event map, you cannot teleport");
		return;
	}

	gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,"[Marry] You successfully moved to %s!", lpTarget->Name);
	gNotice.GCNoticeSend(lpTarget->Index,1,0,0,0,0,0,"[Marry] %s successfully moved to you!", lpObj->Name);

	gObjTeleport(lpObj->Index, lpTarget->Map, lpTarget->X, lpTarget->Y);
}

void CMarrySystem::MarryDivorce(LPOBJ lpObj)
{
	if (gProtection.GetCustomState(CUSTOM_MARRY) == 0)
	{
		return;
	}

	if (this->Config.IsDivorce[lpObj->AccountLevel] != 1)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,"[Divorce] Function Disabled!");
		return;
	}

	if (lpObj->IsMarried == 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,"[Divorce] You are not married!");
		return;
	}

#if(GAMESERVER_UPDATE>=501)
	gCashShop.GDCashShopRecievePointSend(lpObj->Index,(DWORD)&CMarrySystem::MarryDivorceCallback,0,0);
#else
	gPcPoint.GDPcPointRecievePointSend(lpObj->Index,(DWORD)&CMarrySystem::MarryDivorceCallback,0,0);
#endif

	
}

void CMarrySystem::MarryDivorceCallback(LPOBJ lpObj,int Type,LPOBJ lpNpc,DWORD WCoinC,DWORD WCoinP,DWORD GoblinPoint)
{
	if (gProtection.GetCustomState(CUSTOM_MARRY) == 0)
	{
		return;
	}

	if(gMarrySystem.Config.DivorceMinLvl[lpObj->AccountLevel] > lpObj->Level)
    {
		// Need More LVL
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,"[Divorce] You need more level");
        return;
    }

	if (lpObj->Money < gMarrySystem.Config.DivorceCostZen[lpObj->AccountLevel])
	{
		//Need More zen!
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,"[Divorce] You need more %s!",gMessage.GetMessage(523));
		return;
	}

	if (GoblinPoint < gMarrySystem.Config.DivorceCostPCPoint[lpObj->AccountLevel])
	{
		// Need more PCPoint
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,"[Divorce] You need more %s!",gMessage.GetMessage(526));
		return;
	}

	if (WCoinC < gMarrySystem.Config.DivorceCostWCoinC[lpObj->AccountLevel])
	{
		// Need more WCoinC
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,"[Divorce] You need more %s!",gMessage.GetMessage(524));
		return;
	}

	if (WCoinP < gMarrySystem.Config.DivorceCostWCoinP[lpObj->AccountLevel])
	{
		// Need more WCoinP
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,"[Divorce] You need more %s!",gMessage.GetMessage(525));
		return;
	}


	LPOBJ lpTarget = gObjFind(lpObj->MarryName);

	if (lpTarget == 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,"[Divorce] Your ex. marriage is offline.");
		return;
	}

	lpObj->IsMarried = 0;
	lpTarget->IsMarried = 0;

	lpObj->Money -= gMarrySystem.Config.DivorceCostZen[lpObj->AccountLevel];

	GCMoneySend(lpObj->Index,lpObj->Money);

#if(GAMESERVER_UPDATE>=501)

	gCashShop.GDCashShopSubPointSaveSend(lpObj->Index,0,gMarrySystem.Config.DivorceCostWCoinC[lpObj->AccountLevel],gMarrySystem.Config.DivorceCostWCoinP[lpObj->AccountLevel],gMarrySystem.Config.DivorceCostPCPoint[lpObj->AccountLevel]);

#else

	gPcPoint.GDPcPointSubPointSaveSend(lpObj->Index,(DWORD)gMarrySystem.Config.MarryCostPCPoint);

	gPcPoint.GDPcPointPointSend(lpObj->Index);

#endif

	gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,"[Marry] You are divorced!");
	gNotice.GCNoticeSend(lpTarget->Index,1,0,0,0,0,0,"[Marry] You divorced with %s!", lpObj->Name);
}