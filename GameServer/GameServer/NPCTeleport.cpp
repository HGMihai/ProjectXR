#include "stdafx.h"
#include "NPCTeleport.h"
#include "MemScript.h"
#include "Message.h"
#include "Util.h"
#include "CommandManager.h"
#include "CashShop.h"
#include "PcPoint.h"
#include "Notice.h"
#include "Gate.h"
#include "ItemManager.h"


NPCTeleport gTeleporter;

NPCTeleport::NPCTeleport() // OK
{
	this->m_Teleports_Command.clear();
	this->m_Teleports_NPC.clear();
}

NPCTeleport::~NPCTeleport() // OK
{
	
}

void NPCTeleport::Load(char* path) // OK
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

	this->m_Teleports_Command.clear();
	this->m_Teleports_NPC.clear();

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

					TELEPORTS_COMMAND_DATA info;

					strcpy_s(info.Command,lpMemScript->GetString());

					info.TargetGate = lpMemScript->GetAsNumber();

					info.ReqWCC = lpMemScript->GetAsNumber();

					info.ReqWCP = lpMemScript->GetAsNumber();

					info.ReqWGP = lpMemScript->GetAsNumber();

					info.ReqZen = lpMemScript->GetAsNumber();

					info.ReqItem = lpMemScript->GetAsNumber();

					info.ReqItemLevel = lpMemScript->GetAsNumber();

					info.ReqItemCount = lpMemScript->GetAsNumber();

					this->m_Teleports_Command.push_back(info);
				}
				else if (section == 1)
				{
					if(strcmp("end",lpMemScript->GetAsString()) == 0)
					{
						break;
					}

					TELEPORTS_NPC_DATA info;

					info.NPC = lpMemScript->GetNumber();

					info.Map = lpMemScript->GetAsNumber();

					info.X = lpMemScript->GetAsNumber();

					info.Y = lpMemScript->GetAsNumber();

					strcpy_s(info.Name,lpMemScript->GetAsString());

					info.TargetGate = lpMemScript->GetAsNumber();

					info.ReqWCC = lpMemScript->GetAsNumber();

					info.ReqWCP = lpMemScript->GetAsNumber();

					info.ReqWGP = lpMemScript->GetAsNumber();

					info.ReqZen = lpMemScript->GetAsNumber();

					info.ReqItem = lpMemScript->GetAsNumber();

					info.ReqItemLevel = lpMemScript->GetAsNumber();

					info.ReqItemCount = lpMemScript->GetAsNumber();

					this->m_Teleports_NPC.push_back(info);
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

void NPCTeleport::CommandCustomTeleport(LPOBJ lpObj, char* arg) // OK
{
	char zone[32] = {0};

	gCommandManager.GetString(arg,zone,sizeof(zone),0);

#if(GAMESERVER_UPDATE>=501)
	gCashShop.GDCashShopRecievePointSend(lpObj->Index,(DWORD)&NPCTeleport::TeleportCallback,(DWORD)zone,0);
#else
	gPcPoint.GDPcPointRecievePointSend(lpObj->Index,(DWORD)&NPCTeleport::TeleportCallback,(DWORD)zone,0);
#endif
}

void NPCTeleport::TeleportCallback(LPOBJ lpObj,char command[32],DWORD slot,DWORD WCoinC,DWORD WCoinP,DWORD GoblinPoint) // OK
{
	if (strcmp(command, "") != 0)
	{
		for(std::vector<TELEPORTS_COMMAND_DATA>::iterator it=gTeleporter.m_Teleports_Command.begin();it != gTeleporter.m_Teleports_Command.end();it++)
		{
			if(strcmp(command,it->Command) == 0)
			{
				WCOINS_STRUCT wCoins;
				wCoins.WCoinC = WCoinC;
				wCoins.WCoinP = WCoinP;
				wCoins.GoblinPoint = GoblinPoint;

				TELEPORT_DATA info;
				strcpy_s(info.Command,it->Command);
				info.TargetGate = it->TargetGate;
				info.ReqWCC = it->ReqWCC;
				info.ReqWCP = it->ReqWCP;
				info.ReqWGP = it->ReqWGP;
				info.ReqZen = it->ReqZen;
				info.ReqItem = it->ReqItem;
				info.ReqItemLevel = it->ReqItemLevel;
				info.ReqItemCount = it->ReqItemCount;

				gTeleporter.Teleport(lpObj, info, wCoins);
			}
		}
	}
}

bool NPCTeleport::Dialog(LPOBJ lpObj, LPOBJ lpNpc)
{
	for(std::vector<TELEPORTS_NPC_DATA>::iterator it=gTeleporter.m_Teleports_NPC.begin();it != gTeleporter.m_Teleports_NPC.end();it++)
	{
		if( lpNpc->Class		== it->NPC 
		&&	lpNpc->Map			== it->Map
		&&	lpNpc->X			== it->X
		&&	lpNpc->Y			== it->Y )
		{
			gCashShop.GDCashShopRecievePointSend(lpObj->Index,(DWORD)&NPCTeleport::TeleportNpcCallback,(DWORD)lpNpc,0);
			return true;
		}
	}
	return false;
}

void NPCTeleport::TeleportNpcCallback(LPOBJ lpObj,LPOBJ lpNpc,DWORD null,DWORD WCoinC,DWORD WCoinP,DWORD GoblinPoint)
{
	for(std::vector<TELEPORTS_NPC_DATA>::iterator it=gTeleporter.m_Teleports_NPC.begin();it != gTeleporter.m_Teleports_NPC.end();it++)
	{
		if( lpNpc->Class		== it->NPC 
		&&	lpNpc->Map			== it->Map
		&&	lpNpc->X			== it->X
		&&	lpNpc->Y			== it->Y )
		{
			WCOINS_STRUCT wCoins;
			wCoins.WCoinC = WCoinC;
			wCoins.WCoinP = WCoinP;
			wCoins.GoblinPoint = GoblinPoint;

			TELEPORT_DATA info;
			strcpy_s(info.Command,it->Name);
			info.TargetGate = it->TargetGate;
			info.ReqWCC = it->ReqWCC;
			info.ReqWCP = it->ReqWCP;
			info.ReqWGP = it->ReqWGP;
			info.ReqZen = it->ReqZen;
			info.ReqItem = it->ReqItem;
			info.ReqItemLevel = it->ReqItemLevel;
			info.ReqItemCount = it->ReqItemCount;

			gTeleporter.Teleport(lpObj, info, wCoins);
		}
	}
}

void NPCTeleport::Teleport(LPOBJ lpObj, TELEPORT_DATA data, WCOINS_STRUCT wCoins) // OK
{
	GATE_INFO info;

	if (gGate.GetInfo(data.TargetGate, &info))
	{
		if (info.MinLevel != -1 && lpObj->Level < info.MinLevel)
		{
			gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(230),info.MinLevel);
			return;
		}

		if (info.MinReset != -1 && lpObj->Reset < info.MinReset)
		{
			gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(231),info.MinReset);
			return;
		}

		if (info.AccountLevel != -1 && lpObj->AccountLevel < info.AccountLevel)
		{
			gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(494));
			return;
		}

		if (wCoins.WCoinC < (DWORD)data.ReqWCC)
		{
			gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(495), data.ReqWCC);
			return;
		}

		if (wCoins.WCoinP < (DWORD)data.ReqWCP)
		{
			gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(496), data.ReqWCP);
			return;
		}

		if (wCoins.GoblinPoint < (DWORD)data.ReqWGP)
		{
			gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(497), data.ReqWGP);
			return;
		}

		if (lpObj->Money < (DWORD)data.ReqZen)
		{
			gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(498), data.ReqZen);
			return;
		}

		if (data.ReqItemCount > 0)
		{
			if (!this->CheckItem(lpObj, data))
			{
				return;
			}
		}

		gObjMoveGate(lpObj->Index, data.TargetGate);

		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(493),data.Command);

		lpObj->Money -= data.ReqZen;

		GCMoneySend(lpObj->Index,lpObj->Money);

		#if(GAMESERVER_UPDATE>=501)

		gCashShop.GDCashShopSubPointSaveSend(lpObj->Index,0,data.ReqWCC,data.ReqWCP,data.ReqWGP);

		#else

		gPcPoint.GDPcPointSubPointSaveSend(lpObj->Index,data->ReqWGP);

		gPcPoint.GDPcPointPointSend(lpObj->Index);

		#endif
	}
}

bool NPCTeleport::CheckItem(LPOBJ lpObj, TELEPORT_DATA data) // OK
{
	lpObj->ChaosLock = 1;
	int count = gItemManager.GetInventoryItemCount(lpObj,data.ReqItem, data.ReqItemLevel);

	if (count < data.ReqItemCount)
	{
		ITEM_INFO ItemInfo;
		gItemManager.GetInfo(data.ReqItem, &ItemInfo);
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(499), ItemInfo.Name);
		lpObj->ChaosLock = 0;
		return false;
	}

	gItemManager.DeleteInventoryItemCount(lpObj,data.ReqItem,data.ReqItemLevel,data.ReqItemCount);

	lpObj->ChaosLock = 0;
	return true;
}