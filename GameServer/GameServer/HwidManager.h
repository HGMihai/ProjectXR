#pragma once

#include "User.h"
#include "Protocol.h"

struct HardwareId_INFO
{
	char HardwareId[36];
	WORD HardwareIdCount;
	int aIndex;
	int MaxAccountLevel;
};

struct CG_HWID_SEND
{
	PSBMSG_HEAD	Head;
	char HardwareId[36];
	char DLLVersion[20];
	DWORD AntihackCRC;
	DWORD DllCRC;
	DWORD MainCRC;
};

class CHwidManager
{
public:
	CHwidManager();
	virtual ~CHwidManager();
	bool CheckHwid(char* HardwarewId, int aIndex);
	void InsertHwid(char* HardwarewId, int aIndex);
	void RemoveHwid(char* HardwarewId);
	void ConnectHwid(CG_HWID_SEND *lpMsg, LPOBJ lpObj);
private:
	std::map<std::string,HardwareId_INFO> m_HwidInfo;

}; extern CHwidManager gHwidManager;