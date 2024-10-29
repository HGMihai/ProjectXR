#pragma once
#include "Protection.h"
#include "string"

struct  LICENSE_DATA
{
	char CustomerName[64];
	char hwid[256];
	int activeDays;
	int customState[CUSTOMS_COUNT];
};

class cOfflineProtection
{
public:
	cOfflineProtection();
	~cOfflineProtection();
	void CreateHwidFile();
	bool Check();
	// ----
	std::string HardwareId;
	LICENSE_DATA m_LicenseData;
}; extern cOfflineProtection gOfflineProtection;