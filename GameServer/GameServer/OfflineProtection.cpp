#include "stdafx.h"
#include "OfflineProtection.h"
#include "computerHash.h"
#include "LuaExport.h"
#include "string"

cOfflineProtection gOfflineProtection;

cOfflineProtection::cOfflineProtection()
{
	
}

cOfflineProtection::~cOfflineProtection()
{
	
}

void cOfflineProtection::CreateHwidFile()
{
	FILE* file;

	if(fopen_s(&file,"HardwareId.txt","w") != 0)
	{
		MessageBoxA(0,"Cant open",0,0);
		return;
	}

	fprintf(file, "%s", HardwareId.c_str());

	fclose(file);
}

bool cOfflineProtection::Check()
{
	this->HardwareId = cComputerHash::machineHash();

	HANDLE file = CreateFile(".\\license.ltp",GENERIC_READ,FILE_SHARE_READ,0,OPEN_EXISTING,FILE_ATTRIBUTE_ARCHIVE,0);

	if(file == INVALID_HANDLE_VALUE)
	{
		this->CreateHwidFile();
		//MessageBox(0,"HardwareId file created! \nCheck \"HardwareId.txt\" in GameServer folder!","INFO",MB_OK | MB_ICONINFORMATION);
		//MsgBox("HardwareId file created! \nCheck \"HardwareId.txt\" in GameServer folder!");
		//ExitProcess(0);
		return 0;
	}

	if(GetFileSize(file,0) != sizeof(LICENSE_DATA))
	{
		CloseHandle(file);
		return 0;
	}

	DWORD OutSize = 0;

	if(ReadFile(file,&this->m_LicenseData,sizeof(LICENSE_DATA),&OutSize,0) == 0)
	{
		CloseHandle(file);
		return 0;
	}

	/*std::string val = (char*)&this->m_LicenseData;

	std::string ret = gProtection.base64_decode(val);

	for(int n=0;n < sizeof(LICENSE_DATA);n++)
	{
		((BYTE*)&this->m_LicenseData)[n] = ret.c_str()[n];
	}*/

	for(int n=0;n < sizeof(LICENSE_DATA);n++)
	{
		((BYTE*)&this->m_LicenseData)[n] += (BYTE)(0xE9^HIBYTE(n));
		((BYTE*)&this->m_LicenseData)[n] ^= (BYTE)(0x6E^LOBYTE(n));
	}

	

	CloseHandle(file);

	if (!strcmp(this->HardwareId.c_str(),this->m_LicenseData.hwid))
	{
		for(int i=0;i<CUSTOMS_COUNT;i++)
		{
			//MsgBox("Index = %d state = %d",i,this->m_LicenseData.customState[i]);
			gProtection.SetCustomState(i,this->m_LicenseData.customState[i]);
		}

		strcpy_s(gProtection.CustomerName,this->m_LicenseData.CustomerName); // Читаем имя пользователя из главного узла.
		strcpy_s(gProtection.m_ComputerHardwareId,this->HardwareId.c_str());

//		gProtection.m_DaysLeft = this->m_LicenseData.activeDays;

		return true;
	}

	return false;
}
