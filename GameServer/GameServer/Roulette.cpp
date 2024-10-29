#include "stdafx.h"
#include "Roulette.h"
#include "Util.h"
#include "RandomManager.h"

CRoulette gRoulette;

CRoulette::CRoulette()
{
	
}

void CRoulette::load(char* path)
{
	pugi::xml_document file;
	pugi::xml_parse_result res = file.load_file(path);

	if(res.status != pugi::status_ok)
	{
		ErrorMessageBox("%s file load fail (%s)", path, res.description());
	}

	this->m_roulette_data.clear();

	auto CustomStartItem = file.child("Roulette");

	this->is_enabled = CustomStartItem.attribute("Enabled").as_bool(false);

	int index=0;

	for (auto item = CustomStartItem.child("Item"); item; item = item.next_sibling())
	{
		ROULETTE_INFO info;

		info.index = index++;

		info.type = item.attribute("Type").as_int(-1);

		info.item_index = item.attribute("ItemIndex").as_int(-1);

		info.item_level = item.attribute("Level").as_int(-1);

		info.item_luck = item.attribute("Luck").as_int(-1);

		info.item_skill = item.attribute("Skill").as_int(-1);

		info.item_option = item.attribute("Option").as_int(-1);

		info.item_excellent = item.attribute("Excellent").as_int(-1);

		info.item_ancient = item.attribute("Ancient").as_int(-1);

		info.count = item.attribute("Count").as_int(0);

		info.points_type = item.attribute("PointsType").as_int(-1);

		info.rate = item.attribute("Rate").as_int(0);

		this->m_roulette_data.emplace_back(info);
	}

	if(this->m_roulette_data.size() > 8)
	{
		ErrorMessageBox("%s file load fail (Maximum roulette items: 8, your items: %d)", path, this->m_roulette_data.size());
	}
}

void CRoulette::send_roulette_data(int aIndex)
{
	BYTE send[8192];

	PMSG_SEND_ROULETTE pMsg;
	pMsg.header.set(0xF3, 0xFE, sizeof(pMsg));

	int size = sizeof(pMsg);

	pMsg.Count = 0;

	for (int i=0;i<this->m_roulette_data.size();i++)
	{
		memcpy(&send[size],&this->m_roulette_data[i],sizeof(this->m_roulette_data[i]));
		size += sizeof(this->m_roulette_data[i]);

		pMsg.Count++;
	}

	pMsg.header.size[0] = SET_NUMBERHB(size);

	pMsg.header.size[1] = SET_NUMBERLB(size);

	memcpy(send,&pMsg,sizeof(pMsg));
	DataSend(aIndex,send,size);
}

void CRoulette::recv_roulette_start(int aIndex)
{
	CRandomManager RandomManager;

	for(int i=0;i<this->m_roulette_data.size();i++)
		RandomManager.AddElement(this->m_roulette_data[i].index,this->m_roulette_data[i].rate);

	int element = 0;

	RandomManager.GetRandomElement(&element);

	PMSG_SEND_ROULETTE_INDEX pMsg;

	pMsg.header.set(0xFF,0x33,sizeof(pMsg));

	pMsg.index = element;

	DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);
}
