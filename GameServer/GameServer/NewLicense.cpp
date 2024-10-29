#include "StdAfx.h"
//#include "NewLicense.h"
//#include "wininet.h"
//#include "MD5.h"
//#include "resource.h"
//#include "Util.h"
//
//License gLicense;
//bool IsLicenseChecked = false;
//
//#define LICENSE	1
//
//// -------------------------------------------------------------------------------
//
//License::License()
//{
//	
//}
//// -------------------------------------------------------------------------------
//
//License::~License()
//{
//
//}
//// -------------------------------------------------------------------------------
//
//void License::Load(HINSTANCE hInst, HWND ghWnd)
//{
//	this->hInst = hInst;
//	this->ghWnd = ghWnd;
//	this->IsHomePC = false;
//	ZeroMemory(&this->m_Data, sizeof(this->m_Data));
//	// ----
//	SYSTEMTIME CurrentTime;
//	GetLocalTime(&CurrentTime);
//	// ----
//	this->GetDriveID();
//	this->GetAdapter();
//	int and = 555555555;
//	this->GetHash();
//	this->CheckKey();
//}
//
//// -------------------------------------------------------------------------------
//
//LPSTR License::GetText(LPSTR EncodedChar)
//{
//	BYTE XorKey[] = { 0x05, 0x01, 0x11 };
//	LPSTR Line = new char[strlen(EncodedChar)+1];
//	// ----
//	for( int i = 0; i < strlen(EncodedChar); i++ )
//	{
//		Line[i] = char(EncodedChar[i] ^ XorKey[i % 3]);
//	}
//	// ----
//	return Line;
//}
//// -------------------------------------------------------------------------------
//
//LRESULT CALLBACK About1(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
//{
//	switch (message)
//	{
//		case WM_INITDIALOG:
//			{
//				SetWindowText(GetDlgItem(hDlg, IDC_EDIT1), gLicense.m_Data.Hash1);//strupr(gLicense.m_Data.Hash1));
//				return TRUE;
//			}
//
//		case WM_COMMAND:
//			if ( ( wParam & 0xFFFF & 0xFFFF ) == IDOK || (wParam & 0xFFFF & 0xFFFF ) == IDCANCEL) 
//			{
//				EndDialog(hDlg, (wParam& 0xFFFF & 0xFFFF ));
//				return TRUE;
//			}
//			break;
//	}
//    return FALSE;
//}
//// -------------------------------------------------------------------------------
//
//char *substr(char *str, int number)
//{
//  return str + number;
//}
//
//void License::CheckKey()
//{
//	std::string URL;
//	URL.resize(_MAX_PATH);
//	sprintf(&URL[0], "http://guide.ltp-team.ru/license/getlicense.php?darb=%s", strupr(this->m_Data.Hash1));
//	URL.resize(strlen(URL.data())+ 1);
//	// ----
//	char Agent[200];
//	ZeroMemory(Agent, sizeof(Agent));
//	sprintf(Agent, "%s %s", "LTPNewGameServer", GAMESERVER_VERSION);
//	HINTERNET hSession	= InternetOpenA(Agent, INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
//	HINTERNET hHandle	= InternetOpenUrlA(hSession, URL.c_str(), NULL, 0, INTERNET_FLAG_DONT_CACHE, 0);
//	// ----
//	if( hSession == NULL || hHandle == NULL )
//	{
//		if (!strcmp(strupr(this->m_Data.Hash1),"2517BB9AE765F227827C28D5F39B52AD"))
//		{
//			this->IsHomePC = true;
//			return;
//		}
//		//this->SetDash();
//		DialogBoxA(this->hInst, (LPCTSTR)IDD_LICENSE_BOX, this->ghWnd, (DLGPROC)About1);
//		ExitProcess(0);
//		/*MessageBoxA(NULL, GetText("Fnkdrqh~k!Tws~w"), GetText("Ihr`ob`"), ERROR);
//		ExitProcess(0);*/
//	}
//	// ----
//	DWORD Number;
//	char Hash[100];
//	ZeroMemory(Hash, sizeof(Hash));
//	// ----
//	InternetReadFile(hHandle, Hash, 32, &Number);
//	InternetCloseHandle(hSession);
//	InternetCloseHandle(hHandle);
//	// ----
//
//	if( strcmp(Hash, "Work!") )
//	{
//		if (!strcmp(strupr(this->m_Data.Hash1),"2517BB9AE765F227827C28D5F39B52AD"))
//		{
//			this->IsHomePC = true;
//			return;
//		}
//		//MessageBoxA(NULL, "License Error!", "Error!", ERROR);
//		//this->SetDash();
//		DialogBoxA(this->hInst, (LPCTSTR)IDD_LICENSE_BOX, this->ghWnd, (DLGPROC)About1);
//		ExitProcess(0);
//	}
//	// Only for hide license check offset:
//	int A1 = 9;
//	int A8 = 942;
//	int A2 = 1289384;
//	float Module = 1.0f;
//	IsLicenseChecked = true;
//	double A3 = A2-A8-A1 + Module;
//	double T = A3 + A2;
//	
//	//VMEND
//}
//// -------------------------------------------------------------------------------
//
//void License::GetHash()
//{
//	//char Hash1[200], Hash2[200];
//	//ZeroMemory(Hash1, sizeof(Hash1)-1);
//	//ZeroMemory(Hash2, sizeof(Hash2)-1);
//	//// ----
//	//sprintf(Hash1, "%d%d%d%d%d%d%d%d%d",
//	//	this->m_Data.Adapter[4], this->m_Data.Adapter[2], this->m_Data.Adapter[7],
//	//	this->m_Data.Adapter[0], this->m_Data.Adapter[1], this->m_Data.DriveID,
//	//	this->m_Data.Adapter[3], this->m_Data.Adapter[6], this->m_Data.Adapter[5]);
//
//	DWORD VolumeSerialNumber;
//
//	if(GetVolumeInformation("C:\\",0,0,&VolumeSerialNumber,0,0,0,0) == 0)
//	{
//		ExitProcess(0);
//		//return;
//	}
//
//	UUID uuid;
//
//	UuidCreateSequential(&uuid);
//
//	SYSTEM_INFO SystemInfo;
//
//	GetSystemInfo(&SystemInfo);
//
//	char buffer[MAX_COMPUTERNAME_LENGTH+1];
//	DWORD size;
//	size=sizeof(buffer);
//	GetComputerName(buffer,&size);
//
//
//	DWORD Buff = atoi(buffer);
//
//	MessageBoxA(0,buffer,buffer,0);
//
//	DWORD ComputerHardwareId1 = VolumeSerialNumber ^ 0xF45BC123;;
//
//	DWORD ComputerHardwareId2 = *(DWORD*)(&uuid.Data4[2]) ^ 0x13B686FE;
//
//	DWORD ComputerHardwareId3 = ((*(WORD*)(&uuid.Data4[6]) & 0xFFFF) | (SystemInfo.wProcessorArchitecture << 16)) ^ 0x6D19A614;
//
//	DWORD ComputerHardwareId4 = ((SystemInfo.wProcessorLevel & 0xFFFF) | (SystemInfo.wProcessorRevision << 16)) ^ 0xA921E11D;
//
//	DWORD ComputerHardwareId5 = ((DWORD)Buff);
//
//	sprintf(this->m_Data.Hash1, "%08X-%08X-%08X-%08X-%08X", ComputerHardwareId1,ComputerHardwareId2,ComputerHardwareId3,ComputerHardwareId4, ComputerHardwareId5);
//	//MessageBoxA(0, this->m_Data.Hash1, this->m_Data.Hash1, 0);
//
//	//GetMD5(Hash1, strlen(Hash1), this->m_Data.Hash1);
//}
//// -------------------------------------------------------------------------------
//
//void License::GetAdapter()
//{
//	DWORD	BufferLength	= 0;
//	LPBYTE	Buffer			= 0;
//	// ----
//	if( ERROR_BUFFER_OVERFLOW == GetAdaptersInfo(0, &BufferLength) )
//	{
//		Buffer = new BYTE[BufferLength];
//	}
//	// ----
//	PIP_ADAPTER_INFO AdapterInfo = reinterpret_cast<PIP_ADAPTER_INFO>(Buffer);
//	GetAdaptersInfo(AdapterInfo, &BufferLength);
//	// ----
//	if( AdapterInfo )
//	{
//		for( int i = 0; i < MAX_ADAPTER_ADDRESS_LENGTH; i++ )
//		{
//			this->m_Data.Adapter[i] = AdapterInfo->Address[i];
//		}
//	}
//	else
//	{
//		MessageBoxA(NULL, GetText("@scjs1fnu`;17"), GetText("Ihr`ob`!tws~w"), ERROR);
//		//this->SendAlert(2, 2);
//		ExitProcess(0);
//	}
//	// ----
//	delete[] Buffer;
//}
//// -------------------------------------------------------------------------------
//
//void License::GetDriveID()
//{
//	char VolumeNameBuffer[100], FileSystemNameBuffer[100];
//	LPDWORD Len, SystemFlag;
//	GetVolumeInformationA(NULL, VolumeNameBuffer, 100, 
//		&this->m_Data.DriveID, Len, SystemFlag, FileSystemNameBuffer, 100);
//}
//// -------------------------------------------------------------------------------
//
//void License::GetCustomerName()
//{
//	std::string URL;
//	URL.resize(_MAX_PATH);
//	sprintf(&URL[0], "http://guide.ltp-team.ru//license//getcustomername.php?darb=%s", this->m_Data.Hash1);
//	URL.resize(strlen(URL.data())+ 1);
//	// ----
//	char Agent[200];
//	ZeroMemory(Agent, sizeof(Agent));
//	sprintf(Agent, "%s %s", "LTPNewGameServer", GAMESERVER_VERSION);
//	HINTERNET hSession	= InternetOpenA(Agent, INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
//	HINTERNET hHandle	= InternetOpenUrlA(hSession, URL.c_str(), NULL, 0, INTERNET_FLAG_DONT_CACHE, 0);
//	// ----
//	if( hSession == NULL || hHandle == NULL )
//	{
//		sprintf(this->CustomerName, "Error[0]! Please contact support!");
//		return;
//	}
//	// ----
//	DWORD Number;
//	char Hash[100];
//	ZeroMemory(Hash, sizeof(Hash));
//	// ----
//	InternetReadFile(hHandle, Hash, 32, &Number);
//	InternetCloseHandle(hSession);
//	InternetCloseHandle(hHandle);
//	// ----
//
//	if( !strcmp(Hash, "<!DOCTYPE HTML PUBLIC \"-//IETF//") )
//	{
//		sprintf(this->CustomerName, "Error[1]! Please contact support!");
//		return;
//	}
//	//strcat
//	//sprintf(this->CustomerName, Hash);
//	sprintf(this->CustomerName, "%s", Hash);
//	return;
//}
//
//void License::SetDash()
//{
//	strcpy(this->m_Data.Hash2, this->m_Data.Hash1);
//	this->m_Data.Hash1[0] = '\0';
//	// ----
//	for(int i = 0, j = 0; j < strlen(this->m_Data.Hash2); i++)
//	{
//		if( (i+1) % 5 == 0 && i != 0)
//		{
//			this->m_Data.Hash1[i] = '-';
//		}
//		else
//		{
//			this->m_Data.Hash1[i] = *(this->m_Data.Hash2 + j);
//			j++;
//		}  
//	}
//}