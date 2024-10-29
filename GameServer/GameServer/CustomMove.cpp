#include "stdafx.h"
#include "CustomMove.h"
#include "Util.h"
#include <utility>

CCustomMove gCustomMove;

CCustomMove::CCustomMove()
{
	this->m_CustomMoveInfo.clear();
}

void CCustomMove::load(char* path)
{
	pugi::xml_document file;
	pugi::xml_parse_result res = file.load_file(path);

	if(res.status != pugi::status_ok)
	{
		ErrorMessageBox("%s file load fail (%s)", path, res.description());
	}

	this->m_CustomMoveInfo.clear();

	int counter = 0;

	pugi::xml_node PackSystem = file.child("CustomMove");
	
	for (pugi::xml_node move = PackSystem.child("move"); move; move = move.next_sibling())
	{
		CUSTOM_MOVE_INFO info;
		
		info.index = counter;
		strcpy_s(info.text,move.attribute("text").as_string());
		info.map_number = move.attribute("map_number").as_int();
		info.min_level = move.attribute("min_level").as_int();
		info.min_reset = move.attribute("min_reset").as_int();
		info.min_greset = move.attribute("min_greset").as_int();
		info.min_account_level = move.attribute("min_account_level").as_int();
		info.price = move.attribute("price").as_int();
		info.price_type = move.attribute("price_type").as_int();

		this->m_CustomMoveInfo.insert(std::pair<short,CUSTOM_MOVE_INFO>(counter++,info));
	}
}

void CCustomMove::move_data_send(int aIndex)
{
	BYTE send[8192];

	PMSG_CUSTOM_MOVE_INFO_SEND pMsg;

	pMsg.header.set(0xFB,0x20,0);

	int size = sizeof(pMsg);

	pMsg.count = 0;

	for(auto it = this->m_CustomMoveInfo.begin(); it != this->m_CustomMoveInfo.end(); ++it)
	{
		CUSTOM_MOVE_INFO info = it->second;

		memcpy(&send[size],&info,sizeof(info));
		size += sizeof(info);

		pMsg.count++;
	}

	pMsg.header.size[0] = SET_NUMBERHB(size);

	pMsg.header.size[1] = SET_NUMBERLB(size);

	memcpy(send,&pMsg,sizeof(pMsg));
	DataSend(aIndex,send,size);
}
