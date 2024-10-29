#include "stdafx.h"
#include "TeleportPanel.h"
#include "Gate.h"
#include "CommandManager.h"
#include "Notice.h"
#include "Message.h"
#include "CashShop.h"
#include "Protection.h"
#include "Util.h"
#include "BankEx.h"

CTeleportPanel gTeleportPanel;

CTeleportPanel::CTeleportPanel()
{

}

CTeleportPanel::~CTeleportPanel()
{
	
}

void CTeleportPanel::load(char* path)
{
	if (gProtection.GetTeleportState() == false)
	{
		return;
	}
	
	pugi::xml_document file;
	pugi::xml_parse_result res = file.load_file(path);

	if(res.status != pugi::status_ok)
	{
		ErrorMessageBox("%s file load fail (%s)", path, res.description());
	}

	this->m_TeleportPanelInfo.clear();

	int counter = 0;

	pugi::xml_node custom_move = file.child("custom_move");

	this->m_NPCId = custom_move.attribute("npc_index").as_int();
	this->m_NPCMap = custom_move.attribute("npc_map").as_int();
	this->m_X = custom_move.attribute("npc_x").as_int();
	this->m_Y = custom_move.attribute("npc_y").as_int();
	this->pkclear_price = custom_move.attribute("pkclear_price").as_int();
	this->pkclear_price_type = custom_move.attribute("pkclear_price_type").as_int();
	
	for (pugi::xml_node move = custom_move.child("move"); move; move = move.next_sibling())
	{
		TELEPORT_PANEL_INFO info;
		
		info.index = counter;
		strcpy_s(info.text,move.attribute("text").as_string());
		info.gate = move.attribute("gate").as_int();
		info.min_greset = move.attribute("min_greset").as_int();
		info.price = move.attribute("price").as_int();
		info.price_type = move.attribute("price_type").as_int();

		this->m_TeleportPanelInfo.insert(std::pair<short,TELEPORT_PANEL_INFO>(counter++,info));
	}
}

void CTeleportPanel::send_open_window(int aIndex)
{
	BYTE send[8192];

	PMSG_TELEPORT_PANEL_INFO_SEND pMsg;

	pMsg.header.set(0xFB,0x21,0);

	int size = sizeof(pMsg);

	pMsg.count = 0;

	for(auto it = this->m_TeleportPanelInfo.begin(); it != this->m_TeleportPanelInfo.end(); ++it)
	{
		TELEPORT_PANEL_INFO info = it->second;

		memcpy(&send[size],&info,sizeof(info));
		size += sizeof(info);

		pMsg.count++;
	}

	pMsg.pkclear_price = this->pkclear_price;
	pMsg.pkclear_price_type = this->pkclear_price_type;

	pMsg.header.size[0] = SET_NUMBERHB(size);

	pMsg.header.size[1] = SET_NUMBERLB(size);

	memcpy(send,&pMsg,sizeof(pMsg));
	DataSend(aIndex,send,size);
}

bool CTeleportPanel::Dialog(LPOBJ lpObj, LPOBJ lpNpc)
{
	if (gProtection.GetTeleportState() == false)
	{
		return false;
	}

	if( lpNpc->Class		== this->m_NPCId 
		&&	lpNpc->Map			== this->m_NPCMap
		&&	lpNpc->X			== this->m_X
		&&	lpNpc->Y			== this->m_Y )
		{
			this->send_open_window(lpObj->Index);
			return true;
		}
	return false;
}

void CTeleportPanel::TeleportCallback(LPOBJ lpObj,int number,DWORD null,DWORD WCoinC,DWORD WCoinP,DWORD GoblinPoint)
{
	if(number + 1 > gTeleportPanel.m_TeleportPanelInfo.size())
	{
		//gCommandManager.CommandPKClear(lpObj, "");
		gTeleportPanel.clear_pk(lpObj,WCoinC,WCoinP,GoblinPoint);
		return;
	}

	auto move = gTeleportPanel.m_TeleportPanelInfo.find(number);

	if(move == gTeleportPanel.m_TeleportPanelInfo.end())
	{	
		return;
	}

	GATE_INFO info;

	if (gGate.GetInfo(move->second.gate, &info))
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

		if (move->second.min_greset != -1 && lpObj->MasterReset < move->second.min_greset)
		{
			gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,"You must be at least %d grand resets to move",move->second.min_greset);
			return;
		}

		if (info.AccountLevel != -1 && lpObj->AccountLevel < info.AccountLevel)
		{
			gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(494));
			return;
		}

		if(!gTeleportPanel.check_req(lpObj,number,WCoinC,WCoinP,GoblinPoint))
			return;

		switch (move->second.price_type)
		{
			case 0:
			lpObj->Money -= move->second.price;
			GCMoneySend(lpObj->Index, lpObj->Money);
			break;
		case 1:
			gCashShop.GDCashShopSubPointSaveSend(lpObj->Index,nullptr,move->second.price,0,0);
			break;
		case 2:
			gCashShop.GDCashShopSubPointSaveSend(lpObj->Index,nullptr,0,move->second.price,0);
			break;
		case 3:
			gCashShop.GDCashShopSubPointSaveSend(lpObj->Index,nullptr,0,0,move->second.price);
			break;
		case 4:
			lpObj->BankEx.Credits -= move->second.price;
			gBankEx.GCUpdateBankEx(lpObj->Index);
			gBankEx.GDSavePoint(lpObj->Index);
			break;
		}

		gObjMoveGate(lpObj->Index, move->second.gate);
	}
}

void CTeleportPanel::RecvTeleportPanel(PMSG_TELEPORTPANEL_RECV* Data, int aIndex)
{
	if (gProtection.GetTeleportState() == false)
	{
		return;
	}

	gCashShop.GDCashShopRecievePointSend(aIndex,(DWORD)&CTeleportPanel::TeleportCallback,(DWORD)Data->Number,0);	
	//this->Teleport(&gObj[aIndex], Data->Number);
}

bool CTeleportPanel::check_req(LPOBJ lpObj, int index, DWORD WCoinC, DWORD WCoinP, DWORD GoblinPoint)
{
	auto move = gTeleportPanel.m_TeleportPanelInfo.find(index);

	if(move == gTeleportPanel.m_TeleportPanelInfo.end())
		return false;

	switch (move->second.price_type)
		{
			case 0:
				if(lpObj->Money < move->second.price)
				{
					gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(498), move->second.price);
					return false;
				}
			break;
			case 1:
				if(WCoinC < move->second.price)
				{
					gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(495), move->second.price);
					return false;
				}
			break;
			case 2:
				if(WCoinP < move->second.price)
				{
					gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(496), move->second.price);
					return false;
				}
			break;
			case 3:
				if(GoblinPoint < move->second.price)
				{
					gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(497), move->second.price);
					return false;
				}
			break;
			case 4:
				if(lpObj->BankEx.Credits < move->second.price)
				{
					gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(523), move->second.price);
					return false;
				}
			break;
		}
	return true;
}


void CTeleportPanel::clear_pk(LPOBJ lpObj, DWORD WCoinC, DWORD WCoinP, DWORD GoblinPoint)
{
	if(lpObj->PKLevel <= 3)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(76));
		return;
	}

	switch (this->pkclear_price_type)
		{
			case 0:
				if(lpObj->Money < this->pkclear_price)
				{
					gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(498), this->pkclear_price);
					return;
				}
			break;
			case 1:
				if(WCoinC < this->pkclear_price)
				{
					gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(495), this->pkclear_price);
					return;
				}
			break;
			case 2:
				if(WCoinP < this->pkclear_price)
				{
					gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(496), this->pkclear_price);
					return;
				}
			break;
			case 3:
				if(GoblinPoint < this->pkclear_price)
				{
					gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(497), this->pkclear_price);
					return;
				}
			break;
			case 4:
				if(lpObj->BankEx.Credits < this->pkclear_price)
				{
					gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(523), this->pkclear_price);
					return;
				}
			break;
		}

	switch (this->pkclear_price_type)
		{
		case 0:
			lpObj->Money -= this->pkclear_price;
			GCMoneySend(lpObj->Index, lpObj->Money);
			break;
		case 1:
			gCashShop.GDCashShopSubPointSaveSend(lpObj->Index,nullptr,this->pkclear_price,0,0);
			break;
		case 2:
			gCashShop.GDCashShopSubPointSaveSend(lpObj->Index,nullptr,0,this->pkclear_price,0);
			break;
		case 3:
			gCashShop.GDCashShopSubPointSaveSend(lpObj->Index,nullptr,0,0,this->pkclear_price);
			break;
		case 4:
			lpObj->BankEx.Credits -= this->pkclear_price;
			gBankEx.GCUpdateBankEx(lpObj->Index);
			gBankEx.GDSavePoint(lpObj->Index);
			break;
		}

	lpObj->PKLevel = 3;

	GCPKLevelSend(lpObj->Index,lpObj->PKLevel);

	gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(78));
}
