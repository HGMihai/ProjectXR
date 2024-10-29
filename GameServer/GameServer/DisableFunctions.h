#pragma once
#include "Protocol.h"

struct PMSG_DISABLE_SEND
{
	PSBMSG_HEAD	header;
	bool disableMuHelper;
	bool disableGensWindow;
	bool disableExpandedInventory;
};

class CDisableFunctions
{
public:
	CDisableFunctions();
	void Load(char* path);
	void GC_SendDisable(int aIndex);
	// ----
	bool m_disableMuHelper;
	bool m_disableGensWindow;
	bool m_disableExpandedInventory;
	bool m_disableElfBufferQuest;
	bool m_disableMasterSkill;
	bool m_disableItemLevelUpMoreThen13;
	
}; extern CDisableFunctions gDisableFunctions;