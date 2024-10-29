#include "stdafx.h"
#include "resource.h"
#include "GameServer.h"
#include "GameMain.h"
#include "JSProtocol.h"
#include "Message.h"
#include "MiniDump.h"
#include "Notice.h"
#include "QueueTimer.h"
#include "ServerDisplayer.h"
#include "ServerInfo.h"
#include "SocketManager.h"
#include "SocketManagerUdp.h"
#include "Util.h"
#include "NewLicense.h"
#include "Protection.h"
#include "OfflineProtection.h"
#include "AdminChat.h"
#include "BloodCastle.h"
#include "DevilSquare.h"
#include "ChaosCastle.h"
#include "IllusionTemple.h"
#include "CustomEventDrop.h"
#include "CastleSiege.h"
#include "Crywolf.h"
#include "CastleDeep.h"
#include "DupeScanner.h"
#include "crash-report.h"
#include "SocketManagerSimple.h"
//#include "BotBuffer.h"

HINSTANCE hInst;
TCHAR szTitle[MAX_LOADSTRING];
TCHAR szWindowClass[MAX_LOADSTRING];
HWND hWnd;
HWND hStatusWindow;
//HWND hLicenseInfo;

//static int pParts[5]; //массив частей строки состояния 
//static short cx; //вспомогательная переменная для вычисления координат

int APIENTRY WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nCmdShow) // OK
{
	VM_START

	//CMiniDump::Start();

	CrashReporter crashReporter;
	(void)crashReporter; //prevents unused warnings

	LoadString(hInstance,IDS_APP_TITLE,szTitle,MAX_LOADSTRING);
	LoadString(hInstance,IDC_GAMESERVER,szWindowClass,MAX_LOADSTRING);

	MyRegisterClass(hInstance);

	if(InitInstance(hInstance,nCmdShow) == 0)
	{
		return 0;
	}

	SetLargeRand();

	if (!gOfflineProtection.Check())
	{
		gProtection.Auth(hInst, hWnd);
	}

	gServerInfo.ReadStartupInfo("GameServerInfo",".\\Data\\GameServerInfo - Common.dat");

	/*#if(PROTECT_STATE==1)

	#if(GAMESERVER_UPDATE>=801)
	gProtect.StartAuth(AUTH_SERVER_TYPE_S8_GAME_SERVER);
	#elif(GAMESERVER_UPDATE>=601)
	gProtect.StartAuth(AUTH_SERVER_TYPE_S6_GAME_SERVER);
	#elif(GAMESERVER_UPDATE>=401)
	gProtect.StartAuth(AUTH_SERVER_TYPE_S4_GAME_SERVER);
	#else
	gProtect.StartAuth(AUTH_SERVER_TYPE_S2_GAME_SERVER);
	#endif

	#endif*/

	char buff[256];

	wsprintf(buff,"[%s %s] %s",GAMESERVER_TITLE, GAMESERVER_VERSION,gServerInfo.m_ServerName);
	SetWindowText(hWnd,buff);
	
	gServerDisplayer.Init(hWnd,hStatusWindow);

	WSADATA wsa;

	if(WSAStartup(MAKEWORD(2,2),&wsa) == 0)
	{
		if(gSocketManager.Start((WORD)gServerInfo.m_ServerPort) == 0)
		{
			LogAdd(LOG_RED,"Could not start GameServer");
		}
		else
		{
			/*if (gSocketManagerSimple.Start(44410) == 0)
			{
				ErrorMessageBox("Could not start WebServer connection");
			}*/

			GameMainInit(hWnd);

			JoinServerConnect(WM_JOIN_SERVER_MSG_PROC);

			DataServerConnect(WM_DATA_SERVER_MSG_PROC);

			gSocketManagerUdp.Connect(gServerInfo.m_ConnectServerAddress,(WORD)gServerInfo.m_ConnectServerPort);

			SetTimer(hWnd,WM_TIMER_1000,1000,nullptr);

			SetTimer(hWnd,WM_TIMER_10000,10000,nullptr);

			gQueueTimer.CreateTimer(QUEUE_TIMER_MONSTER,100,&QueueTimerCallback);

			gQueueTimer.CreateTimer(QUEUE_TIMER_MONSTER_MOVE,100,&QueueTimerCallback);

			gQueueTimer.CreateTimer(QUEUE_TIMER_MONSTER_AI,100,&QueueTimerCallback);

			gQueueTimer.CreateTimer(QUEUE_TIMER_MONSTER_AI_MOVE,100,&QueueTimerCallback);

			gQueueTimer.CreateTimer(QUEUE_TIMER_EVENT,100,&QueueTimerCallback);

			gQueueTimer.CreateTimer(QUEUE_TIMER_VIEWPORT,1000,&QueueTimerCallback);

			gQueueTimer.CreateTimer(QUEUE_TIMER_FIRST,1000,&QueueTimerCallback);

			gQueueTimer.CreateTimer(QUEUE_TIMER_CLOSE,1000,&QueueTimerCallback);

			gQueueTimer.CreateTimer(QUEUE_TIMER_MATH_AUTHENTICATOR,10000,&QueueTimerCallback);

			gQueueTimer.CreateTimer(QUEUE_TIMER_ACCOUNT_LEVEL,60000,&QueueTimerCallback);

			gQueueTimer.CreateTimer(QUEUE_TIMER_VOTE_PARCER, 1800000/*1800000*/ /*60000*/, &QueueTimerCallback);

			//gQueueTimer.CreateTimer(QUEUE_TIMER_CHECK_VERSION,3600000,&QueueTimerCallback);

			gQueueTimer.CreateTimer(QUEUE_TIMER_DUPE_SCANNER, gDupeScanner.get_check_delay(),&QueueTimerCallback);
		}
	}
	else
	{
		LogAdd(LOG_RED,"WSAStartup() failed with error: %d",WSAGetLastError());
	}

	gServerDisplayer.PaintAllInfo();

	SetTimer(hWnd,WM_TIMER_2000,2000,nullptr);

	HACCEL hAccelTable = LoadAccelerators(hInstance,(LPCTSTR)IDC_GAMESERVER);

	MSG msg;

	while(GetMessage(&msg,nullptr,0,0) != 0)
	{
		if(TranslateAccelerator(msg.hwnd,hAccelTable,&msg) == 0)
		{
			TranslateMessage(&msg);
			DispatchMessageA(&msg);
		}
	}

	//CMiniDump::Clean();

	VM_END

	return msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance) // OK
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = (WNDPROC)WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance,(LPCTSTR)IDI_ICON1);
	wcex.hCursor = LoadCursor(nullptr,IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName = (LPCSTR)IDC_GAMESERVER;
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance,(LPCTSTR)IDI_ICON1);

	return RegisterClassEx(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance,int nCmdShow) // OK
{
	hInst = hInstance;

	//hWnd = CreateWindowA(szWindowClass,szTitle,WS_OVERLAPPEDWINDOW | WS_THICKFRAME,CW_USEDEFAULT,0,GAMESERVER_WIDTH,GAMESERVER_HEIGHT,0,0,hInstance,0);

	hWnd = CreateWindowExA(0,szWindowClass,szTitle,WS_OVERLAPPED+WS_CAPTION+WS_SYSMENU+WS_VISIBLE+WS_MINIMIZEBOX,CW_USEDEFAULT,0,GAMESERVER_WIDTH,GAMESERVER_HEIGHT, nullptr, nullptr,hInstance, nullptr);

	if(hWnd == nullptr)
		return 0;

	//hStatusWindow = CreateStatusWindow(WS_CHILD | WS_VISIBLE," ",hWnd,40000);
	hStatusWindow = CreateWindowExA(0,STATUSCLASSNAME, nullptr, WS_CHILD | WS_VISIBLE, 0, 0, 0, 0,hWnd,(HMENU)100,hInstance, nullptr);

	int iQueueBarWidths[] = {100, 250, 340, 440, 540, 740, -1};
	SendMessage(hStatusWindow, SB_SETPARTS, 7, (LPARAM)iQueueBarWidths);

	ShowWindow(hWnd,nCmdShow);
	UpdateWindow(hWnd);
	
	return 1;
}

LRESULT CALLBACK WndProc(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam) // OK
{
	switch(message)
	{
		case WM_SIZE:
			/*MoveWindow(hStatusWindow, 0,0,0,0, TRUE);
			pParts[0]=100;
			pParts[1]=250;
			pParts[2]=340;
			pParts[3]=440;
			pParts[4]=540;
			SendMessage(hStatusWindow, SB_SETPARTS, 5, (LPARAM)pParts);*/
			break;
		case WM_COMMAND:
			switch(LOWORD(wParam))
			{
				case IDM_ABOUT:
					DialogBox(hInst,(LPCTSTR)IDD_ABOUTBOX,hWnd,(DLGPROC)About);
					break;
				case IDM_EXIT:
					if(MessageBox(nullptr,"Are you sure to terminate GameServer?","Ask terminate server",MB_YESNO | MB_ICONQUESTION) == IDYES)
					{
						DestroyWindow(hWnd);
					}
					break;
				case IDM_FILE_ALLUSERLOGOUT:
					gObjAllLogOut();
					break;
				case IDM_FILE_ALLUSERDISCONNECT:
					gObjAllDisconnect();
					break;
				case IDM_FILE_1MINUTESERVERCLOSE:
					if(gCloseMsg == 0)
					{
						gCloseMsg = 1;
						gCloseMsgTime = 60;
						gNotice.GCNoticeSendToAll(0,0,0,0,0,0,gMessage.GetMessage(487));
					}
					break;
				case IDM_FILE_3MINUTESERVERCLOSE:
					if(gCloseMsg == 0)
					{
						gCloseMsg = 1;
						gCloseMsgTime = 180;
						gNotice.GCNoticeSendToAll(0,0,0,0,0,0,gMessage.GetMessage(488));
					}
					break;
				case IDM_FILE_5MINUTESERVERCLOSE:
					if(gCloseMsg == 0)
					{
						gCloseMsg = 1;
						gCloseMsgTime = 300;
						gNotice.GCNoticeSendToAll(0,0,0,0,0,0,gMessage.GetMessage(489));
					}
					break;
				case ID_RELOAD_RELOADLTPDATA:
					gServerInfo.ReadLTPData();
					break;
				case IDM_RELOAD_RELOADCASHSHOP:
					gServerInfo.ReadCashShopInfo();
					break;
				case IDM_RELOAD_RELOADCHAOSMIX:
					gServerInfo.ReadChaosMixInfo();
					break;
				case IDM_RELOAD_RELOADCHARACTER:
					gServerInfo.ReadCharacterInfo();
					break;
				case IDM_RELOAD_RELOADCOMMAND:
					gServerInfo.ReadCommandInfo();
					break;
				case IDM_RELOAD_RELOADCOMMON:
					gServerInfo.ReadCommonInfo();
					break;
				case IDM_RELOAD_RELOADCUSTOM:
					gServerInfo.ReadCustomInfo();
					break;
				case IDM_RELOAD_RELOADEVENT:
					gServerInfo.ReadEventInfo();
					break;
				case IDM_RELOAD_RELOADEVENTITEMBAG:
					gServerInfo.ReadEventItemBagInfo();
					break;
				case IDM_RELOAD_RELOADHACK:
					gServerInfo.ReadHackInfo();
					break;
				case IDM_RELOAD_RELOADITEM:
					gServerInfo.ReadItemInfo();
					break;
				case IDM_RELOAD_RELOADMONSTER:
					gServerInfo.ReloadMonsterInfo();
					break;
				case IDM_RELOAD_RELOADMOVE:
					gServerInfo.ReadMoveInfo();
					break;
				case IDM_RELOAD_RELOADQUEST:
					gServerInfo.ReadQuestInfo();
					break;
				case IDM_RELOAD_RELOADSHOP:
					gServerInfo.ReadShopInfo();
					break;
				case IDM_RELOAD_RELOADSKILL:
					gServerInfo.ReadSkillInfo();
					break;
				case IDM_RELOAD_RELOADUTIL:
					gServerInfo.ReadUtilInfo();
					break;
				case ID_EVENTS_BLOODCASTLE:
					gBloodCastle.start_now();
					break;
				case ID_EVENTS_DEVILSQUARE:
					gDevilSquare.start_now();
					break;
				case ID_EVENTS_CHAOSCASTLE:
					gChaosCastle.start_now();
					break;
				case ID_EVENTS_ILLUSIONTEMPLE:
					gIllusionTemple.start_now();
					break;
				case ID_EVENTS_DROP:
					gCustomEventDrop.start_now();
					break;
				case ID_EVENTS_STARTCASTLESIEGE:
					#if(GAMESERVER_TYPE==1)
					gCastleSiege.start();
					#endif
					break;
				case ID_EVENTS_STARTCRYWOLF:
					gCrywolf.start_now();
					break;
				case ID_EVENTS_STARTLORENDEEP:
					gCastleDeep.start_now();
					break;
				case ID_OPTIONS_DISCONNECT:
					DialogBox(hInst,(LPCTSTR)IDD_DISCONNECT,hWnd,(DLGPROC)Disconnect);
					break;
				case ID_OPTIONS_ADMINCHAT:
					{
					//DialogBox(hInst,(LPCTSTR)IDD_CHAT,hWnd,(DLGPROC)AdminChat);
					auto dialog = CreateDialog(hInst,(LPCTSTR)IDD_CHAT,hWnd,(DLGPROC)AdminChat);
					ShowWindow(dialog, SW_SHOW);
					}
					break;
				case ID_LICENSE_SHOW:
					DialogBoxA(hInst, (LPCTSTR)IDD_LICENSEINFO, hWnd, (DLGPROC)LicenseBox);
					break;
				case ID_LICENSE_CHECK:
					gProtection.Auth(hInst, hWnd);
					break;
				default:
					return DefWindowProc(hWnd,message,wParam,lParam);
			}
			break;
		case WM_TIMER:
			switch(wParam)
			{
				case WM_TIMER_1000:
					GJServerUserInfoSend();
					ConnectServerInfoSend();
					break;
				case WM_TIMER_2000:
					gObjCountProc();
					gServerDisplayer.Run();
					break;
				case WM_TIMER_10000:
					JoinServerReconnect(hWnd,WM_JOIN_SERVER_MSG_PROC);
					DataServerReconnect(hWnd,WM_DATA_SERVER_MSG_PROC);
					break;
			}
			break;
		case WM_JOIN_SERVER_MSG_PROC:
			JoinServerMsgProc(wParam,lParam);
			break;
		case WM_DATA_SERVER_MSG_PROC:
			DataServerMsgProc(wParam,lParam);
			break;
		case WM_CLOSE:
			if (MessageBox(nullptr, "Are you sure to terminate GameServer?", "Ask terminate server", MB_YESNO | MB_ICONQUESTION) == IDYES)
			{
				DestroyWindow(hWnd);
			}
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hWnd,message,wParam,lParam);
	}

	return 0;
}

LRESULT CALLBACK About(HWND hDlg,UINT message,WPARAM wParam,LPARAM lParam) // OK
{
	switch(message)
	{
		case WM_INITDIALOG:
			return 1;
		case WM_COMMAND:
			if(LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
			{
				EndDialog(hDlg,LOWORD(wParam));
				return 1;
			}
			break;
	}

	return 0;
}

LRESULT CALLBACK Disconnect(HWND hDlg,UINT message,WPARAM wParam,LPARAM lParam) // OK
{
	switch(message)
	{
		case WM_INITDIALOG:
			HWND hlst;
			SetClassLong(hDlg, GCL_HICON, (long)LoadIcon(nullptr, IDI_APPLICATION));
			hlst = GetDlgItem(hDlg, IDC_LIST1);
		
			for(int n=OBJECT_START_USER;n < MAX_OBJECT;n++)
			{
				if(gObjIsConnectedGP(n) != 0)
				{
					SendMessage(hlst, LB_ADDSTRING, 0, (LPARAM) gObj[n].Account);
				}
			}

			return 1;
		case WM_COMMAND:
			int wmId    = LOWORD(wParam);

			switch(wmId)
			{
				case IDC_RELOAD:
					{
						SetClassLong(hDlg, GCL_HICON, (long)LoadIcon(nullptr, IDI_APPLICATION));
						hlst = GetDlgItem(hDlg, IDC_LIST1);
						SendMessage(hlst, LB_RESETCONTENT, 0, NULL);
						//gAccountManager.GetAccounts(hlst);
						for(int n=OBJECT_START_USER;n < MAX_OBJECT;n++)
						{
							if(gObjIsConnectedGP(n) != 0)
							{
								SendMessage(hlst, LB_ADDSTRING, 0, (LPARAM) gObj[n].Account);
							}
						}
					}
					break;
				case IDC_DISCONNECT:
					{
						char text[11]={0};
						SetClassLong(hDlg, GCL_HICON, (long)LoadIcon(nullptr, IDI_APPLICATION));
						hlst = GetDlgItem(hDlg, IDC_LIST1);

						int count = SendMessage(hlst, LB_GETCOUNT, 0, 0);
						int iSelected = -1;

						// go through the items and find the first selected one
						for (int i = 0; i < count; i++)
						{
						  // check if this item is selected or not..
							if (SendMessage(hlst, LB_GETSEL, i, 0) > 0)
							{
								// yes, we only want the first selected so break.
								iSelected = i;
								break;
							}
						}

						// get the text of the selected item
						if (iSelected != -1)
						{
							SendMessage(hlst, LB_GETTEXT, (WPARAM)iSelected , (LPARAM)text);
							//JGDisconnectAccountSend(text);
							LPOBJ lpTarget = gObjFindByAcc(text);
							if (lpTarget)
							{
								if (lpTarget->AttackCustomOffline != 0 || lpTarget->PShopCustomOffline != 0)
								{
									gObjDelOffExp(lpTarget->Index);
								}
								
								gObjUserKill(lpTarget->Index);
								SendMessage(hlst, LB_DELETESTRING, (WPARAM)iSelected, NULL);
							}
						}
					}
					break;
			}

			if(LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
			{
				EndDialog(hDlg,LOWORD(wParam));
				return 1;
			}
			break;
	}

	return 0;
}

std::wstring strtowstr(const std::string &str)
{
	wchar_t *wszTo = new wchar_t[str.length() + 1];
	wszTo[str.size()] = L'\0';
	MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, wszTo, (int)str.length());
	std::wstring wstrTo = wszTo;
	delete[] wszTo;
	return wstrTo;
}

std::string wstrtostr(const std::wstring &wstr)
{
	char* szTo = new char[wstr.length() * 2 + 1];
	szTo[wstr.size() * 2] = '\0';
	WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, szTo, (int)wstr.length() * 2, nullptr, nullptr);
	std::string strTo = szTo;
	delete[] szTo;
	return strTo;
}

LRESULT CALLBACK AdminChat(HWND hDlg,UINT message,WPARAM wParam,LPARAM lParam) // OK
{
	HWND sliderBar = GetDlgItem(hDlg, IDC_SLIDER1);
	switch(message)
	{
		case WM_INITDIALOG:
			HWND hlst;
			SetClassLong(hDlg, GCL_HICON, (long)LoadIcon(nullptr, IDI_APPLICATION));
			hlst = GetDlgItem(hDlg, IDC_LIST_CHAT);
			gAdminChat.set_dialog_opened(true);
			gAdminChat.update_view(hlst,hDlg);
		
			SendMessage(sliderBar, TBM_SETRANGEMIN	, false, 1);
		    SendMessage(sliderBar, TBM_SETRANGEMAX, false, 240);
		    SendMessage(sliderBar, TBM_SETTICFREQ	, false, 1);

			return 1;
		case WM_HSCROLL:
			{
				if (sliderBar == (HWND)lParam)
			    {
					int curNumb = SendMessage(sliderBar, TBM_GETPOS, 0, 0);
					char text[128];

					if (curNumb < 24)
					{
						sprintf(text,"%d hours",curNumb);
					}
					else
					{
						int days = curNumb / 24;
						int hours = curNumb % 24;
						if (hours != 0)
						{
							sprintf(text,"%d days %d hours",days, hours);
						}
						else
						{
							sprintf(text,"%d days",days);
						}
					}
					
					SetWindowText(GetDlgItem(hDlg, IDC_BAN_TIME), text);
			    }
			}
		break;
		
		case WM_COMMAND:
			int wmId    = LOWORD(wParam);
			int wmEvent = HIWORD(wParam);

			if(wmEvent == LBN_SELCHANGE)
			{
				hlst = GetDlgItem(hDlg, IDC_LIST_CHAT);

				int count = SendMessage(hlst, LB_GETCOUNT, 0, 0);
				int iSelected = -1;

				// go through the items and find the first selected one
				for (int i = 0; i < count; i++)
				{
				  // check if this item is selected or not..
					if (SendMessage(hlst, LB_GETSEL, i, 0) > 0)
					{
						// yes, we only want the first selected so break.
						iSelected = i;
						break;
					}
				}

				LPOBJ lpObj = gAdminChat.gObjGetBySelectedIndex(iSelected);

				if(lpObj != nullptr)
				{
					char text[128];
					sprintf(text,"Account: %s, Character: %s, IP: %s",lpObj->Account,lpObj->Name,lpObj->IpAddr);
					SetWindowText(GetDlgItem(hDlg, IDC_ACCOUNT_TEXT_BOX), text);
				}
			}
		
			switch(wmId)
			{
				case IDC_CHAT_SEARCH_BUTTON:
					{
						hlst = GetDlgItem(hDlg, IDC_LIST_CHAT);
						HWND hEdit = GetDlgItem(hDlg, IDC_CHAT_SEARCH);
						wchar_t txt[1024];
						GetWindowTextW(hEdit, txt, sizeof(txt));
						std::string buffer = wstrtostr(txt);

						CString TextUTF(buffer.c_str());
						gAdminChat.StringToUTF8(TextUTF);

						std::string temp = TextUTF;

						int index = gAdminChat.search_text(temp);

						if (index != -1)
						{
							SendMessage(hlst, LB_SETCURSEL, index, 0);

							LPOBJ lpObj = gAdminChat.gObjGetBySelectedIndex(index);

							if(lpObj != nullptr)
							{
								char text[128];
								sprintf(text,"Account: %s, Character: %s, IP: %s",lpObj->Account,lpObj->Name,lpObj->IpAddr);
								SetWindowText(GetDlgItem(hDlg, IDC_ACCOUNT_TEXT_BOX), text);
							}
						}
					}
				break;
				case IDC_CHAT_SEND:
					{
						HWND hEdit = GetDlgItem(hDlg, IDC_CHAT_TEXTBOX);
						wchar_t txt[1024];
						GetWindowTextW(hEdit, txt, sizeof(txt));
						bool state = IsDlgButtonChecked(hDlg,IDC_CHAT_CHECK);
						
						std::string buffer = wstrtostr(txt);
						
						int length = buffer.length();
						if (length>MAX_CHAT_MESSAGE_SIZE-1)
						{
							length = MAX_CHAT_MESSAGE_SIZE-1;
						}
						char *msg = new char[length+1];
						memcpy(msg,buffer.c_str(),length);
						msg[length] = '\0';
						
						
						if (state == 0)
						{
							PostMessage1("ADMIN",msg,"");
						}
						else
						{
							gNotice.GCNoticeSendToAll(0,0,0,0,0,0,msg);
						}
						delete[] msg;
					}
				break;
				case IDC_CHAT_BAN_ACCOUNT:
					{
						hlst = GetDlgItem(hDlg, IDC_LIST_CHAT);

						int count = SendMessage(hlst, LB_GETCOUNT, 0, 0);
						int iSelected = -1;

						for (int i = 0; i < count; i++)
						{
							if (SendMessage(hlst, LB_GETSEL, i, 0) > 0)
							{
								iSelected = i;
								break;
							}
						}

						if (iSelected != -1)
						{
							gAdminChat.ban_account(iSelected);
						}
					}
				break;
				case IDC_CHAT_BAN_CHARACTER:
					{
						hlst = GetDlgItem(hDlg, IDC_LIST_CHAT);

						int count = SendMessage(hlst, LB_GETCOUNT, 0, 0);
						int iSelected = -1;

						for (int i = 0; i < count; i++)
						{
							if (SendMessage(hlst, LB_GETSEL, i, 0) > 0)
							{
								iSelected = i;
								break;
							}
						}

						if (iSelected != -1)
						{
							gAdminChat.ban_character(iSelected);
						}
					}
				break;
				case IDC_CHAT_BAN_CHAT:
					{
						hlst = GetDlgItem(hDlg, IDC_LIST_CHAT);

						int count = SendMessage(hlst, LB_GETCOUNT, 0, 0);
						int iSelected = -1;

						for (int i = 0; i < count; i++)
						{
							if (SendMessage(hlst, LB_GETSEL, i, 0) > 0)
							{
								iSelected = i;
								break;
							}
						}

						if (iSelected != -1)
						{
							int curNumb = SendMessage(sliderBar, TBM_GETPOS, 0, 0); // Получаем текущее положение слайдера
							
							gAdminChat.ban_chat(iSelected,curNumb);
						}
					}
				break;
			}

			if(LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
			{
				EndDialog(hDlg,LOWORD(wParam));
				gAdminChat.set_dialog_opened(false);
				return 1;
			}
			break;
	}

	return 0;
}

LRESULT CALLBACK LicenseBox(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_INITDIALOG:
			{
				gProtection.InitDialog(hDlg);
				return TRUE;
			}
			break;
		case WM_COMMAND:
			if(LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
			{
				EndDialog(hDlg,LOWORD(wParam));
				return 1;
			}
			break;
	}
    return FALSE;
}