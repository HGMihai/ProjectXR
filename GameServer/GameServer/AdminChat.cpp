#include "stdafx.h"
#include "AdminChat.h"
#include "Util.h"
#include "Notice.h"
#include "JSProtocol.h"
#include "ObjectManager.h"
#include "DSProtocol.h"

CAdminChat gAdminChat;

CAdminChat::CAdminChat(): hList(nullptr), hDialog(nullptr)
{
	this->m_ChatList.clear();
	this->isDialogOpened = false;
	this->last_search_index = 0;
	this->last_search_text.clear();
}

int CAdminChat::StringToUTF8(CString & Text)
{
	const LPCTSTR MultiByte	= Text;
    const int Size			= MultiByteToWideChar(CP_UTF8, 0, MultiByte, -1, nullptr, 0);
    wchar_t * Output	= new wchar_t[Size];
	// ----
    MultiByteToWideChar(CP_UTF8, 0, MultiByte, -1, Output, Size);
    Text.Empty();
    Text = CString(Output);
	// ----
    delete [] Output;
	return Size;
}

/**
 * \brief 
 * \param name 
 * \param message
 * \param type 
 */
void CAdminChat::add_to_list(std::string name, std::string message, eChatType type)
{
	const std::string msg = "[" + (std::string)ChatTypeNames[(int)type] + "] [" + name + "] " + message;

	if (this->m_ChatList.size() >= MAX_ADMIN_CHAT_MESSAGE)
	{
		this->m_ChatList.erase(this->m_ChatList.begin());
	}

	CString TextUTF(msg.c_str());
	StringToUTF8(TextUTF);

	ADMIN_CHAT_DATA info;

	info.name = name;
	info.text = TextUTF;
	
	this->m_ChatList.emplace_back(info);

	if (this->isDialogOpened)
	{
		SendMessage(this->hList, LB_ADDSTRING, 0, reinterpret_cast<LPARAM>((const char*)TextUTF));

		SendMessage (this->hList, WM_VSCROLL, MAKEWPARAM (SB_BOTTOM, NULL), NULL);
		const std::string windowName = "[Admin Chat] (count: " + std::to_string(static_cast<long long>(this->m_ChatList.size())) + ")";
		SetWindowText(this->hDialog,windowName.c_str());
	}
	else
	{
		this->update_view(this->hList,this->hDialog);
	}
}

/**
 * \brief 
 * \param hInst
 * \param hDlg 
 */
void CAdminChat::update_view(HWND hInst, HWND hDlg)
{
	this->hList = hInst;

	this->hDialog = hDlg;
	
	SendMessage(hInst, LB_RESETCONTENT, NULL, NULL);

	for (auto it=this->m_ChatList.begin();it!=this->m_ChatList.end();++it)
	{
		SendMessage(hInst, LB_ADDSTRING, 0, reinterpret_cast<LPARAM>(it->text.c_str()));
	}

	SendMessage (hInst, WM_VSCROLL, MAKEWPARAM (SB_BOTTOM, NULL), NULL);
	const std::string name = "[Admin Chat] (count: " + std::to_string(static_cast<long long>(this->m_ChatList.size())) + ")";
	SetWindowText(hDlg,name.c_str());

}

void CAdminChat::set_dialog_opened(bool value)
{
	this->isDialogOpened = value;
}

bool CAdminChat::get_dialog_opened() const
{
	return this->isDialogOpened;
}



int CAdminChat::search_text(std::string text)
{
	int start_index = 0;
	if (text.compare(this->last_search_text) == 0)
	{
		start_index = this->last_search_index+1;
	}
	
	for(int i=start_index;i<this->m_ChatList.size();i++)
	{
		if (this->m_ChatList[i].text.find(text) != std::string::npos)
		{
			this->last_search_text = text;
			this->last_search_index = i;
			return i;
		}
	}
	
	return -1;
}

LPOBJ CAdminChat::gObjGetBySelectedIndex(int index)
{
	if (index == -1)
	{
		return nullptr;
	}

	if (index >= this->m_ChatList.size())
	{
		LogAdd(LOG_RED,"[AdminChat] Can't get character name! Index: %d Count: %d",index,this->m_ChatList.size());
		return nullptr;
	}
	
	ADMIN_CHAT_DATA *lpInfo = &this->m_ChatList[index];

	if (lpInfo == nullptr)
	{
		LogAdd(LOG_RED,"[AdminChat] Can't get character name! Index: %d",index);
		return nullptr;
	}

	LPOBJ lpObj = gObjFind((char*)lpInfo->name.c_str());

	if(lpObj == nullptr)
	{
		LogAdd(LOG_RED,"[AdminChat] Can't get character info! Name: %s",lpInfo->name.c_str());
		return nullptr;		
	}

	return lpObj;
}

void CAdminChat::ban_account(int index)
{
	LPOBJ lpObj = this->gObjGetBySelectedIndex(index);
	
	if (lpObj == nullptr)
	{
		return;
	}

	char text[256];
	sprintf(text,"You really want to ban '%s' account?",lpObj->Account);
	
	if (MessageBox(nullptr, text, "Ban confirmation", MB_YESNO | MB_ICONQUESTION) == IDYES)
	{
		LogAdd(LOG_GREEN,"[AdminChat] Account banned. Account: %s, Character: %s",lpObj->Account,lpObj->Name);

		gNotice.GCNoticeSend(lpObj->Index, 0, 0, 0, 0, 0, 0, "Your account banned. Goodbye!");	

		GJSetBlockCodeSend(lpObj->Index,1);

		gObjectManager.CharacterGameCloseSet(lpObj->Index, 2);
	}
}

void CAdminChat::ban_character(int index)
{
	LPOBJ lpObj = this->gObjGetBySelectedIndex(index);
	
	if (lpObj == nullptr)
	{
		return;
	}

	char text[256];
	sprintf(text,"You really want to ban '%s' character?",lpObj->Name);
	
	if (MessageBox(nullptr, text, "Ban confirmation", MB_YESNO | MB_ICONQUESTION) == IDYES)
	{
		LogAdd(LOG_GREEN,"[AdminChat] Character banned. Account: %s, Character: %s",lpObj->Account,lpObj->Name);

		gNotice.GCNoticeSend(lpObj->Index, 0, 0, 0, 0, 0, 0, "Your character banned. Goodbye!");	

		GDSetCharacterBlockSend(lpObj->Index,1);

		gObjectManager.CharacterGameCloseSet(lpObj->Index, 1);
	}
}

/**
 * \brief 
 * \param index 
 * \param delay in hours
 */
void CAdminChat::ban_chat(int index, int delay)
{
	LPOBJ lpObj = this->gObjGetBySelectedIndex(index);
	
	if (lpObj == nullptr)
	{
		return;
	}

	char text[256];
	sprintf(text,"You really want to ban chat '%s' character for %d hours?",lpObj->Account, delay);
	
	if (MessageBox(nullptr, text, "Ban confirmation", MB_YESNO | MB_ICONQUESTION) == IDYES)
	{
		lpObj->ChatLimitTime += delay*60;
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, "Your chat banned for %d minutes. Total: %d", delay*60,lpObj->ChatLimitTime);

		LogAdd(LOG_GREEN,"[AdminChat] Chat banned. Account: %s, Character: %s, Delay: %d",lpObj->Account,lpObj->Name,delay);
	}
}