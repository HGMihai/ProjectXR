#pragma once
#include "User.h"

#define MAX_ADMIN_CHAT_MESSAGE 1000

enum eChatType
{
	eCHAT_BAD_WORD = 0,
	eCHAT_COMMAND,
	eCHAT_POST,
	eCHAT_PARTY,
	eCHAT_GUILD,
	eCHAT_GUILD_NOTICE,
	eCHAT_ALLIANCE,
	eCHAT_GENS,
	eCHAT_SIMPLE,
};

static char ChatTypeNames[9][32] = {
	"Bad Word",
	"Command",
	"Post",
	"Party",
	"Guild",
	"Guild Notice",
	"Alliance",
	"Gens",
	"Chat"	
};

struct ADMIN_CHAT_DATA
{
	std::string name;
	std::string text;
};

class CAdminChat
{
public:
	CAdminChat();
	void add_to_list(std::string name, std::string message, eChatType type);
	void update_view(HWND hInst, HWND hDlg);
	bool get_dialog_opened() const;
	void set_dialog_opened(bool value);
	int search_text(std::string text);
	LPOBJ gObjGetBySelectedIndex(int index);
	void ban_account(int index);	
	void ban_character(int index);
	void ban_chat(int index, int delay);
	static int StringToUTF8(CString & Text);
	// ----
private:
	std::vector<ADMIN_CHAT_DATA> m_ChatList;
	bool isDialogOpened;
	HWND hList;
	HWND hDialog;
	std::string last_search_text;
	int last_search_index;
}; extern CAdminChat gAdminChat;