#pragma once

#include "User.h"
#include "Protocol.h"

struct OFFLINE_DATA
{
	char AccountID[11];
	char Name[11];
	int Type;
};

struct GD_OFFLINE_DATA_SEND
{
	PSBMSG_HEAD	h;
	// ----
	char AccountID[11];
	char Name[11];
	int Type;
};

struct GD_OFFLINE_DATA_DEL_SEND
{
	PSBMSG_HEAD	h;
	// ----
	char AccountID[11];
};

class cOfflineData
{
public:
	cOfflineData();
	virtual ~cOfflineData();
	void SetOffline(LPOBJ lpObj, int type);
	void DeleteOffline(LPOBJ lpObj);
	void SendOffline(OFFLINE_DATA info);
	void SendDelffline(OFFLINE_DATA info);
private:
	std::map<std::string,OFFLINE_DATA> m_OfflineData;
}; extern cOfflineData gOfflineData;
