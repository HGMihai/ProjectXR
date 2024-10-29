#include "stdafx.h"
#include "OfflineData.h"
#include "GameMain.h"

cOfflineData gOfflineData;

cOfflineData::cOfflineData()
{
	this->m_OfflineData.clear();
}

cOfflineData::~cOfflineData()
{
	
}

void cOfflineData::SetOffline(LPOBJ lpObj, int type)
{
	if (this->m_OfflineData.find(lpObj->Account) == this->m_OfflineData.end())
	{
		OFFLINE_DATA info;

		memcpy(info.AccountID,lpObj->Account,sizeof(info.AccountID));
		memcpy(info.Name,lpObj->Name,sizeof(info.Name));
		info.Type = type;

		this->m_OfflineData.insert(std::pair<std::string,OFFLINE_DATA>(lpObj->Account,info));

		this->SendOffline(info);
	}
}

void cOfflineData::DeleteOffline(LPOBJ lpObj)
{
	std::map<std::string,OFFLINE_DATA>::iterator it = this->m_OfflineData.find(std::string(lpObj->Account));

	if(it != this->m_OfflineData.end())
	{
		this->SendDelffline(it->second);
		this->m_OfflineData.erase(it);
	}
}


void cOfflineData::SendOffline(OFFLINE_DATA info)
{
	GD_OFFLINE_DATA_SEND pMsg;
	pMsg.h.set(0xFB, 0x07, sizeof(pMsg));
	memcpy(pMsg.AccountID,info.AccountID,sizeof(pMsg.AccountID));
	memcpy(pMsg.Name,info.Name,sizeof(pMsg.Name));
	pMsg.Type = info.Type;
	gDataServerConnection.DataSend((BYTE*)&pMsg,pMsg.h.size);
}

void cOfflineData::SendDelffline(OFFLINE_DATA info)
{
	GD_OFFLINE_DATA_DEL_SEND pMsg;
	pMsg.h.set(0xFB, 0x08, sizeof(pMsg));
	memcpy(pMsg.AccountID,info.AccountID,sizeof(pMsg.AccountID));
	gDataServerConnection.DataSend((BYTE*)&pMsg,pMsg.h.size);
}
