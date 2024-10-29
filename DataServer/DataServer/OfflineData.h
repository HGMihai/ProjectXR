#pragma once

#include "DataServerProtocol.h"

struct GD_OFFLINE_DATA_RECV
{
	PSBMSG_HEAD	h;
	// ----
	char AccountID[11];
	char Name[11];
	int Type;
};

struct GD_OFFLINE_DATA_DEL_RECV
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
	void OfflineDataRecv(GD_OFFLINE_DATA_RECV* lpMsg, int aIndex);
	void OfflineDelRecv(GD_OFFLINE_DATA_DEL_RECV* lpMsg, int aIndex);
}; extern cOfflineData gOfflineData;
