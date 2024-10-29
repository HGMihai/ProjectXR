#include "stdafx.h"
#include "CostumeSystem.h"
#include "User.h"
#include "Util.h"
#include "ItemManager.h"

CCostumeSystem gCostumeSystem;

CCostumeSystem::CCostumeSystem()
{
}

CCostumeSystem::~CCostumeSystem()
{
}

void CCostumeSystem::load(char* path)
{
#ifndef _EMKA_
	return;
#endif
	pugi::xml_document file;
	const pugi::xml_parse_result res = file.load_file(path);

	if(res.status != pugi::status_ok)
	{
		ErrorMessageBox("%s file load fail (%s)", path, res.description());
	}

	this->m_CostumeInfo.clear();

	const auto costumeSystem = file.child("CostumeSystem");

	for(auto item = costumeSystem.child("item"); item; item=item.next_sibling())
	{
		COSTUME_LIST info;
		info.costumeInfo.costumeItemIndex = item.attribute("index").as_int(0);
		info.costumeInfo.itemIndex = item.attribute("costumeIndex").as_int(0);
		info.costumeInfo.costumeLevel = item.attribute("costumeLevel").as_int(0);

		info.costumeOptionColor = item.attribute("OptionColor").as_int(12);

		info.costumeOptions.Option1 = item.attribute("Opt1Value").as_int(0);
		info.costumeOptions.Option2 = item.attribute("Opt2Value").as_int(0);
		info.costumeOptions.Option3 = item.attribute("Opt3Value").as_int(0);
		info.costumeOptions.Option4 = item.attribute("Opt4Value").as_int(0);
		info.costumeOptions.Option5 = item.attribute("Opt5Value").as_int(0);
		info.costumeOptions.Option6 = item.attribute("Opt6Value").as_int(0);

		this->m_CostumeInfo.insert(std::pair<int,COSTUME_LIST>(info.costumeInfo.costumeItemIndex,info));
	}
}

void CCostumeSystem::GCCostumeSend(LPOBJ lpObj, int aIndex)
{
#ifndef _EMKA_
	return;
#endif
	if (this->m_CostumeInfo.size() == 0)
	{
		return;
	}
	
	const auto it = this->m_CostumeInfo.find(lpObj->Inventory[8].m_Index);

	PMSG_COSTUME_SEND pMsg;

	pMsg.header.set(0xFB,0x93,sizeof(pMsg));
	
	if(it == this->m_CostumeInfo.end())
	{
		pMsg.info.costumeItemIndex = -1;
		pMsg.info.itemIndex = -1;
		pMsg.info.costumeLevel = -1;
	}
	else
	{
		pMsg.info.costumeItemIndex = it->second.costumeInfo.costumeItemIndex;
		pMsg.info.itemIndex = it->second.costumeInfo.itemIndex;
		pMsg.info.costumeLevel = it->second.costumeInfo.costumeLevel;
	}

	pMsg.aIndex = lpObj->Index;

	DataSend(aIndex,(BYTE*)&pMsg,sizeof(pMsg));

	if (lpObj->Index == aIndex)
	{
		MsgSendV2(lpObj,(BYTE*)&pMsg, sizeof(pMsg));
	}
}

void CCostumeSystem::GCCostumeListSend(int aIndex)
{
#ifndef _EMKA_
	return;
#endif
	if (this->m_CostumeInfo.size() == 0)
	{
		return;
	}
	
	BYTE send[8192];

	PMSG_COSTUME_INFO_SEND pMsg;

	pMsg.header.set(0xF3,0xFB,0);

	int size = sizeof(pMsg);

	pMsg.count = 0;

	COSTUME_LIST info;

	for(auto it = this->m_CostumeInfo.begin(); it != this->m_CostumeInfo.end(); ++it)
	{
		info = it->second;
		/*info.costumeInfo = it->second.costumeInfo;
		info.costumeOptions = it->second.costumeOptions;
		info.costumeOptionColor = it->second.costumeOptionColor;*/
		memcpy(&send[size],&info,sizeof(info));
		size += sizeof(info);

		pMsg.count++;
	}

	pMsg.header.size[0] = SET_NUMBERHB(size);

	pMsg.header.size[1] = SET_NUMBERLB(size);

	memcpy(send,&pMsg,sizeof(pMsg));
	DataSend(aIndex,send,size);
}

bool CCostumeSystem::checkIsCostumeItem(int itemId)
{
	auto it = this->m_CostumeInfo.find(itemId);

	if (it == this->m_CostumeInfo.end())
	{
		return 0;
	}

	return 1;
}


int CCostumeSystem::GetCostumeSystemNewOptionValue(int itemId, int optionNumber)
{
	auto it = this->m_CostumeInfo.find(itemId);

	if (it == this->m_CostumeInfo.end())
	{
		return 0;
	}

	switch (optionNumber)
	{
		case 0:
			return it->second.costumeOptions.Option1;
		case 1:
			return it->second.costumeOptions.Option2;
		case 2:
			return it->second.costumeOptions.Option3;
		case 3:
			return it->second.costumeOptions.Option4;
		case 4:
			return it->second.costumeOptions.Option5;
		case 5:
			return it->second.costumeOptions.Option6;
		default:
			return 0;
	}
	
}
