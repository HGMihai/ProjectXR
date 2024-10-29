#include "stdafx.h"
#include "CustomStartItem.h"
#include "Util.h"
#include "GameMain.h"
#include "DSProtocol.h"
#include "ItemManager.h"

CCustomStartItem gCustomStartItem;

void CCustomStartItem::Load(char* path)
{
	pugi::xml_document file;
	pugi::xml_parse_result res = file.load_file(path);

	if(res.status != pugi::status_ok)
	{
		ErrorMessageBox("%s file load fail (%s)", path, res.description());
	}

	this->StartItemEnable = false;
	this->StartBuffEnable = false;

	this->m_ItemData.clear();
	this->m_BuffData.clear();

	auto CustomStartItem = file.child("CustomStartItem");
	auto CustomStartBuff = file.child("CustomStartBuff");

	this->StartItemEnable = CustomStartItem.attribute("Enable").as_bool();

	this->StartBuffEnable = CustomStartBuff.attribute("Enable").as_bool();

	for (auto item = CustomStartItem.child("Item"); item; item = item.next_sibling())
	{
		StartItemData info;

		info.Class = item.attribute("Class").as_int();

		info.Section = item.attribute("Section").as_int();

		info.Type = item.attribute("Type").as_int();

		info.Level = item.attribute("Level").as_int();

		info.Dur = item.attribute("Durability").as_int();

		info.Skill = item.attribute("Skill").as_int();

		info.Luck = item.attribute("Luck").as_int();

		info.Option = item.attribute("Option").as_int();

		info.ExcOption = item.attribute("ExcOption").as_int();

		info.AncOption = item.attribute("AncOption").as_int();

		info.JoHOption = item.attribute("JoHOption").as_int();

		info.ExOption = item.attribute("ExOption").as_int();

		info.Time = item.attribute("Time").as_int();

		info.SocketOpt[0] = item.attribute("SocketOpt1").as_int();

		info.SocketOpt[1] = item.attribute("SocketOpt2").as_int();

		info.SocketOpt[2] = item.attribute("SocketOpt3").as_int();

		info.SocketOpt[3] = item.attribute("SocketOpt4").as_int();

		info.SocketOpt[4] = item.attribute("SocketOpt5").as_int();

		this->m_ItemData.emplace_back(info);
	}

	for (auto item = CustomStartBuff.child("Item"); item; item = item.next_sibling())
	{
		StartBuffData info;

		info.Class = item.attribute("Class").as_int();

		info.Effect = item.attribute("Effect").as_int();

		info.value[0] = item.attribute("value1").as_int();

		info.value[1] = item.attribute("value2").as_int();

		info.value[2] = item.attribute("value3").as_int();

		info.value[3] = item.attribute("value4").as_int();

		info.Time = item.attribute("Time").as_int();

		this->m_BuffData.emplace_back(info);
	}
}

void CCustomStartItem::MakeStartItem(LPOBJ lpObj)
{
	if (!this->StartItemEnable)
	{
		return;	
	}

	for(auto it = this->m_ItemData.begin(); it != this->m_ItemData.end(); ++it)
	{
		if (it->Class != lpObj->Class)
		{
			continue;
		}
		
        time_t t = time(nullptr);
        localtime(&t);
		DWORD time = static_cast<DWORD>(t) + it->Time * 60;

		if (it->Time == 0)
		{
			time = 0;
		}

		GDCreateItemSend(lpObj->Index,0xEB,0,0,GET_ITEM(it->Section,it->Type),it->Level,it->Dur,it->Skill,it->Luck,it->Option,-1,it->ExcOption,it->AncOption,it->JoHOption,it->ExOption,it->SocketOpt,0xFF,time);
	}

	this->SendState(lpObj);
}

void CCustomStartItem::SendState(LPOBJ lpObj)
{
	PMSG_GD_STARTITEM_STATE_SEND pMsg;

	pMsg.h.set(0xFB,0x0A,sizeof(pMsg));

	memcpy(pMsg.Account,lpObj->Account,sizeof(lpObj->Account));
	memcpy(pMsg.Name,lpObj->Name,sizeof(lpObj->Name));

	pMsg.state = 1;

	gDataServerConnection.DataSend((BYTE*)&pMsg,pMsg.h.size);
}
