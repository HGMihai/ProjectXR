#include "stdafx.h"
#include "Protection.h"
#include "resource.h"
#include "wininet.h"
#include <fstream>
#include <list>
#include <string>
#include <sstream>
#include "../../Util/curl/curl.h"
#include "computerHash.h"
#include "base64.h"
#include "Util.h"

CProtection gProtection;

void copy_to_clipboard(char* text)
{
	EmptyClipboard(); //очищаем буфер
	HGLOBAL hgBuffer = GlobalAlloc(GMEM_DDESHARE, 512);//выделяем память
	char* chBuffer = (char*)GlobalLock(hgBuffer); //блокируем память
	
	strcpy(chBuffer, LPCSTR(text));
	GlobalUnlock(hgBuffer);//разблокируем память
	SetClipboardData(CF_TEXT, hgBuffer);//помещаем текст в буфер обмена
	CloseClipboard(); //закрываем буфер обмена
}

LRESULT CALLBACK LicenseErrorBox(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_INITDIALOG:
			{
				SetWindowText(GetDlgItem(hDlg, IDC_EDIT1), gProtection.m_ComputerHardwareId);//strupr(gLicense.m_Data.Hash1));
				SetWindowText(GetDlgItem(hDlg, IDC_EDIT2), gProtection.ipAddr);//strupr(gLicense.m_Data.Hash1));
				return TRUE;
			}

		case WM_COMMAND:
			if ((wParam & 0xFFFF & 0xFFFF) == IDC_BTNCOPY)
			{
				if(OpenClipboard(nullptr))//открываем буфер обмена
				{
					
					char text[512];
					strcpy_s(text,"HardwareId: ");
					strcat_s(text,gProtection.m_ComputerHardwareId);
					strcat_s(text,"\r\n");
					strcat_s(text,"IpAddress: ");
					strcat_s(text,gProtection.ipAddr);

					copy_to_clipboard(text);
					
					EndDialog(hDlg, (wParam& 0xFFFF & 0xFFFF ));
					MessageBox(nullptr,"Registration data successfully copied to clipboard!","INFO",MB_OK | MB_ICONINFORMATION);
					return TRUE;
				}
			}
			else if ((wParam & 0xFFFF & 0xFFFF) == IDC_BTNCOPY_HARDWAREID)
			{
				if(OpenClipboard(nullptr))
				{
					char text[256];
					strcpy_s(text,gProtection.m_ComputerHardwareId);

					copy_to_clipboard(text);
					
					EndDialog(hDlg, (wParam& 0xFFFF & 0xFFFF ));
					MessageBox(nullptr,"Hardware ID successfully copied to clipboard!","INFO",MB_OK | MB_ICONINFORMATION);
					return TRUE;
				}
			}
			else if ((wParam & 0xFFFF & 0xFFFF) == IDC_BTNCOPY_IPADDRES)
			{
				if(OpenClipboard(nullptr))
				{
					char text[256];
					strcpy_s(text,gProtection.ipAddr);

					copy_to_clipboard(text);
					
					EndDialog(hDlg, (wParam& 0xFFFF & 0xFFFF ));
					MessageBox(nullptr,"IP address successfully copied to clipboard!","INFO",MB_OK | MB_ICONINFORMATION);
					return TRUE;
				}
			}
			else if ( ( wParam & 0xFFFF & 0xFFFF ) == IDOK || (wParam & 0xFFFF & 0xFFFF ) == IDCANCEL || (wParam & 0xFFFF & 0xFFFF) == IDC_BUTTON1) 
			{
				EndDialog(hDlg, (wParam& 0xFFFF & 0xFFFF ));
				return TRUE;
			}
			break;
	}
    return FALSE;
}

CProtection::CProtection()
{
	CLEAR_START

	VM_START

	srand((DWORD)time(nullptr));

	memset(this->m_ComputerHardwareId,0,sizeof(this->m_ComputerHardwareId));

	this->m_EncKey = rand()%256;

	this->m_LicenseActive = 0;

	this->m_Debt = 0;

	this->is_license_end = false;

	this->license_days_over = 0;

	memset(this->end_license_date_text,0,sizeof(this->end_license_date_text));

	this->m_CustomsData.clear();

	VM_END

	CLEAR_END
}


void CProtection::Auth(HINSTANCE hIns, HWND hWnd)
{
	strcpy_s(this->CustomerName, "release");
	this->is_license_end = false;

	for (int custom_id = 0; custom_id < CUSTOMS_COUNT; custom_id++)
	{
		if (custom_id == CUSTOM_MARRY || custom_id == CUSTOM_DIABLO || custom_id == CUSTOM_VOTEPARCER)
			continue;

		this->SetCustomState(custom_id, true); // Устанавливаем статус кастому.
	}
}

void CProtection::ErrorMessageBox1()
{
	memset(this->m_ComputerHardwareId,0,sizeof(this->m_ComputerHardwareId));

	this->SafeExitProcess();
}

void CProtection::InfoBox(char* message,...)
{
	VM_START

	char buff[256]{};

	memset(buff,0,sizeof(buff));

	va_list arg;
	va_start(arg,message);
	vsprintf_s(buff,message,arg);
	va_end(arg);

	MessageBox(nullptr,buff,"Info",MB_OK | MB_ICONWARNING);

	VM_END
}

void CProtection::SafeExitProcess()
{
	while(true)
	{
		TerminateProcess(GetCurrentProcess(),0);
		CRASH_APPLICATION_MACRO
	}
}

void CProtection::SetCustomState(int index, bool value)
{
	if (index == CUSTOM_VOTEPARCER)
	{
		value = false;
	}
	
	std::map<int,bool>::iterator it = this->m_CustomsData.find(index);

	if (it == this->m_CustomsData.end())
	{
		this->m_CustomsData.insert(std::pair<int,bool>(index,value));
	}
	else
	{
		it->second = value;
	}
}

bool CProtection::GetCustomState(eCustomsType index)
{
	if (this->m_CustomsData.empty())
		return false;
	
	const auto it = this->m_CustomsData.find(index);

	if (it != this->m_CustomsData.end())
		return it->second;

	return false;
}

void CProtection::InitDialog(HWND hWnd)
{
	this->ghWnd = hWnd;

	std::stringstream str ;
	str << "License [ " << this->CustomerName << " ] Information";

	SetWindowText(hWnd,str.str().c_str());

	SetWindowText(GetDlgItem(this->ghWnd, IDC_LICENSENAME), gProtection.CustomerName);
	SetWindowText(GetDlgItem(this->ghWnd, IDC_LICENSEHWID), gProtection.m_ComputerHardwareId);
	SetWindowText(GetDlgItem(this->ghWnd, IDC_LICENSEIP), gProtection.ipAddr);

	SendMessage(GetDlgItem(this->ghWnd, IDC_MARRY), BM_SETCHECK, (this->GetCustomState(eCustomsType::CUSTOM_MARRY) == 1) ? BST_CHECKED : BST_UNCHECKED, NULL);
	SendMessage(GetDlgItem(this->ghWnd, IDC_QUESTSYSTEM), BM_SETCHECK, (this->GetCustomState(eCustomsType::CUSTOM_QUEST_SYSTEM) == 1) ? BST_CHECKED : BST_UNCHECKED, NULL);
	SendMessage(GetDlgItem(this->ghWnd, IDC_VOTEPARCER), BM_SETCHECK, (this->GetCustomState(eCustomsType::CUSTOM_VOTEPARCER) == 1) ? BST_CHECKED : BST_UNCHECKED, NULL);
	SendMessage(GetDlgItem(this->ghWnd, IDC_TVTEVENT), BM_SETCHECK, (this->GetCustomState(eCustomsType::CUSTOM_TVTEVENT) == 1) ? BST_CHECKED : BST_UNCHECKED, NULL);
	SendMessage(GetDlgItem(this->ghWnd, IDC_STATSADD), BM_SETCHECK, (this->GetCustomState(eCustomsType::CUSTOM_STATSADD) == 1) ? BST_CHECKED : BST_UNCHECKED, NULL);
	SendMessage(GetDlgItem(this->ghWnd, IDC_EXPANDEDCHARS), BM_SETCHECK, (this->GetCustomState(eCustomsType::CUSTOM_EXPANDED_CHARACTER) == 1) ? BST_CHECKED : BST_UNCHECKED, NULL);
	SendMessage(GetDlgItem(this->ghWnd, IDC_PARTYSEARCH), BM_SETCHECK, (this->GetCustomState(eCustomsType::CUSTOM_PARTY_SEARCH) == 1) ? BST_CHECKED : BST_UNCHECKED, NULL);
	SendMessage(GetDlgItem(this->ghWnd, IDC_OFFLINEMODE), BM_SETCHECK, (this->GetCustomState(eCustomsType::CUSTOM_OFFLINE_MODE) == 1) ? BST_CHECKED : BST_UNCHECKED, NULL);
	SendMessage(GetDlgItem(this->ghWnd, IDC_SMITHY), BM_SETCHECK, (this->GetCustomState(eCustomsType::CUSTOM_SMITHY) == 1) ? BST_CHECKED : BST_UNCHECKED, NULL);
	SendMessage(GetDlgItem(this->ghWnd, IDC_BUFFICON), BM_SETCHECK, (this->GetCustomState(eCustomsType::CUSTOM_BUFFICON) == 1) ? BST_CHECKED : BST_UNCHECKED, NULL);
	SendMessage(GetDlgItem(this->ghWnd, IDC_BUFFS), BM_SETCHECK, (this->GetCustomState(eCustomsType::CUSTOM_BUFFS) == 1) ? BST_CHECKED : BST_UNCHECKED, NULL);
	SendMessage(GetDlgItem(this->ghWnd, IDC_NEWPETS), BM_SETCHECK, (this->GetCustomState(eCustomsType::CUSTOM_PETS) == 1) ? BST_CHECKED : BST_UNCHECKED, NULL);
	SendMessage(GetDlgItem(this->ghWnd, IDC_EVENTTIMER), BM_SETCHECK, (this->GetCustomState(eCustomsType::CUSTOM_EVENTTIMER) == 1) ? BST_CHECKED : BST_UNCHECKED, NULL);
	SendMessage(GetDlgItem(this->ghWnd, IDC_GUILDWARE), BM_SETCHECK, (this->GetCustomState(eCustomsType::CUSTOM_GUILD_WAREHOUSE) == 1) ? BST_CHECKED : BST_UNCHECKED, NULL);
	SendMessage(GetDlgItem(this->ghWnd, IDC_ACHIVEMENTS), BM_SETCHECK, (this->GetCustomState(eCustomsType::CUSTOM_ACHIEVEMENTS) == 1) ? BST_CHECKED : BST_UNCHECKED, NULL);
	SendMessage(GetDlgItem(this->ghWnd, IDC_RANKINGS), BM_SETCHECK, (this->GetCustomState(eCustomsType::CUSTOM_RANKINGS) == 1) ? BST_CHECKED : BST_UNCHECKED, NULL);
	SendMessage(GetDlgItem(this->ghWnd, IDC_OPTION_REPLACE), BM_SETCHECK, (this->GetCustomState(eCustomsType::CUSTOM_OPTION_REPLACE) == 1) ? BST_CHECKED : BST_UNCHECKED, NULL);
	SendMessage(GetDlgItem(this->ghWnd, IDC_NEW_PARTY), BM_SETCHECK, (this->GetCustomState(eCustomsType::CUSTOM_NEW_PARTY) == 1) ? BST_CHECKED : BST_UNCHECKED, NULL);
	SendMessage(GetDlgItem(this->ghWnd, IDC_ACTIVE_INVASIONS), BM_SETCHECK, (this->GetCustomState(eCustomsType::CUSTOM_ACTIVE_INVASIONS) == 1) ? BST_CHECKED : BST_UNCHECKED, NULL);
	SendMessage(GetDlgItem(this->ghWnd, IDC_CUSTOM_STAT), BM_SETCHECK, (this->GetCustomState(eCustomsType::CUSTOM_STAT) == 1) ? BST_CHECKED : BST_UNCHECKED, NULL);
}

bool CProtection::GetTeleportState()
{
	if(this->m_Index == eCustomers::WorkPC || this->m_Index == eCustomers::voodooz_1992)
		return true;
	return false;
}
