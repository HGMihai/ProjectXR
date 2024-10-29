#include "stdafx.h"
#include "DisableFunctions.h"
#include "Util.h"

CDisableFunctions gDisableFunctions;

CDisableFunctions::CDisableFunctions()
{
	 this->m_disableMuHelper = false;
	 this->m_disableGensWindow = false;
	 this->m_disableExpandedInventory = false;
	 this->m_disableElfBufferQuest = false;
}

void CDisableFunctions::Load(char* path)
{
	this->m_disableMuHelper = GetPrivateProfileInt("Common", "DisableMuHelper", 0, path);
	this->m_disableGensWindow = GetPrivateProfileInt("Common", "DisableGensWindow", 0, path);
	this->m_disableExpandedInventory = GetPrivateProfileInt("Common", "DisableExpandedInventory", 0, path);
	this->m_disableElfBufferQuest = GetPrivateProfileInt("Common", "DisableElfBufferQuest", 0, path);
	this->m_disableMasterSkill = GetPrivateProfileInt("Common", "DisableMasterSkill", 0, path);
	this->m_disableItemLevelUpMoreThen13 = GetPrivateProfileInt("Common", "DisableItemLevelUpMoreThen13", 0, path);
}

void CDisableFunctions::GC_SendDisable(int aIndex)
{
	PMSG_DISABLE_SEND pMsg;
	
	pMsg.header.set(0xFF,0x01,sizeof(pMsg));

	pMsg.disableMuHelper = this->m_disableMuHelper;
	pMsg.disableGensWindow = this->m_disableGensWindow;
	pMsg.disableExpandedInventory = this->m_disableExpandedInventory;

	DataSend(aIndex,(BYTE*)&pMsg,sizeof(pMsg));
}
