// ServerDisplayer.h: interface for the CServerDisplayer class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#define MAX_LOG_TEXT_LINE 33
#define MAX_LOG_TEXT_SIZE 120

#define BACKCOLOR	RGB(0, 45, 0)

enum eLogColor
{
	LOG_BLACK = 0,
	LOG_RED = 1,
	LOG_GREEN = 2,
	LOG_BLUE = 3,
	LOG_TEST = 1,
};

struct LOG_DISPLAY_INFO
{
	char text[MAX_LOG_TEXT_SIZE];
	eLogColor color;
};

class CServerDisplayer
{
public:
	CServerDisplayer();
	virtual ~CServerDisplayer();
	void Init(HWND hWnd, HWND hStatusWindow);
	void Run();
	void SetWindowName();
	void UpdateInfoBar();
	void PaintAllInfo();
	void LogTextPaint();
	void Interface(HWND hWnd);
	void LogAddText(eLogColor color,char* text,int size);
	void CheckVersion();
private:
	HWND m_hwnd;
	HWND m_hStatusWindow;
	HFONT m_font;
	HBRUSH m_brush[3];
	LOG_DISPLAY_INFO m_log[MAX_LOG_TEXT_LINE];
	int m_count;
	char m_DisplayerText[4][64];
	char m_DeveloperText[3][64];
	//char m_UpdatesText[3][64];
	int m_Version;
};

extern CServerDisplayer gServerDisplayer;
