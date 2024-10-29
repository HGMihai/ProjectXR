#include "stdafx.h"
#include "resource.h"
#include "JoinServer.h"
#include "AccountManager.h"
#include "AllowableIpList.h"
#include "MiniDump.h"
#include "Protect.h"
#include "QueryManager.h"
#include "ServerDisplayer.h"
#include "SocketManager.h"
#include "SocketManagerUdp.h"
#include "ThemidaSDK.h"
#include "Util.h"

HINSTANCE hInst;
TCHAR szTitle[MAX_LOADSTRING];
TCHAR szWindowClass[MAX_LOADSTRING];
HWND hWnd;
char CustomerName[32];
char CustomerHardwareId[36];
BOOL CaseSensitive;
BOOL MD5Encryption;
BOOL BCryptEncryption;

int APIENTRY WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nCmdShow) // OK
{
	VM_START

	CMiniDump::Start();

	LoadString(hInstance,IDS_APP_TITLE,szTitle,MAX_LOADSTRING);
	LoadString(hInstance,IDC_JOINSERVER,szWindowClass,MAX_LOADSTRING);

	MyRegisterClass(hInstance);

	if(InitInstance(hInstance,nCmdShow) == 0)
	{
		return 0;
	}

	GetPrivateProfileString("JoinServerInfo","CustomerName","",CustomerName,sizeof(CustomerName),".\\JoinServer.ini");

	GetPrivateProfileString("JoinServerInfo","CustomerHardwareId","",CustomerHardwareId,sizeof(CustomerHardwareId),".\\JoinServer.ini");

	#if(PROTECT_STATE==1)

	#if(JOINSERVER_UPDATE>=801)
	gProtect.StartAuth(AUTH_SERVER_TYPE_S8_JOIN_SERVER);
	#elif(JOINSERVER_UPDATE>=601)
	gProtect.StartAuth(AUTH_SERVER_TYPE_S6_JOIN_SERVER);
	#elif(JOINSERVER_UPDATE>=401)
	gProtect.StartAuth(AUTH_SERVER_TYPE_S4_JOIN_SERVER);
	#else
	gProtect.StartAuth(AUTH_SERVER_TYPE_S2_JOIN_SERVER);
	#endif

	#endif

	char buff[256];

	wsprintf(buff,"[%s %s] (QueueSize : %d) (AccountCount : %d/%d)",JOINSERVER_TITLE, JOINSERVER_VERSION,0,0,0);

	SetWindowText(hWnd,buff);

	gServerDisplayer.Init(hWnd);

	WSADATA wsa;

	if(WSAStartup(MAKEWORD(2,2),&wsa) == 0)
	{
		char JoinServerODBC[32] = {0};

		char JoinServerUSER[32] = {0};

		char JoinServerPASS[32] = {0};

		GetPrivateProfileString("JoinServerInfo","JoinServerODBC","",JoinServerODBC,sizeof(JoinServerODBC),".\\JoinServer.ini");

		GetPrivateProfileString("JoinServerInfo","JoinServerUSER","",JoinServerUSER,sizeof(JoinServerUSER),".\\JoinServer.ini");

		GetPrivateProfileString("JoinServerInfo","JoinServerPASS","",JoinServerPASS,sizeof(JoinServerPASS),".\\JoinServer.ini");

		WORD JoinServerPort = GetPrivateProfileInt("JoinServerInfo","JoinServerPort",55970,".\\JoinServer.ini");

		char ConnectServerAddress[16] = {0};

		GetPrivateProfileString("JoinServerInfo","ConnectServerAddress","127.0.0.1",ConnectServerAddress,sizeof(ConnectServerAddress),".\\JoinServer.ini");

		WORD ConnectServerPort = GetPrivateProfileInt("JoinServerInfo","ConnectServerPort",55557,".\\JoinServer.ini");

		CaseSensitive = GetPrivateProfileInt("JoinServerInfo","CaseSensitive",0,".\\JoinServer.ini");

		MD5Encryption = GetPrivateProfileInt("JoinServerInfo","MD5Encryption",0,".\\JoinServer.ini");

		BCryptEncryption= GetPrivateProfileInt("JoinServerInfo", "BCryptEncryption", 0, ".\\JoinServer.ini");

		if(gQueryManager.Connect(JoinServerODBC,JoinServerUSER,JoinServerPASS) == 0)
		{
			LogAdd(LOG_RED,"Could not connect to database");
		}
		else
		{
			if(gSocketManager.Start(JoinServerPort) == 0)
			{
				gQueryManager.Disconnect();
			}
			else
			{
				if(gSocketManagerUdp.Connect(ConnectServerAddress,ConnectServerPort) == 0)
				{
					gSocketManager.Clean();

					gQueryManager.Disconnect();
				}
				else
				{
					gAllowableIpList.Load("AllowableIpList.txt");

					SetTimer(hWnd,TIMER_1000,1000,0);
				}
			}
		}
	}
	else
	{
		LogAdd(LOG_RED,"WSAStartup() failed with error: %d",WSAGetLastError());
	}

	gServerDisplayer.PaintAllInfo();

	SetTimer(hWnd,TIMER_2000,2000,0);

	HACCEL hAccelTable = LoadAccelerators(hInstance,(LPCTSTR)IDC_JOINSERVER);

	MSG msg;

	while(GetMessage(&msg,0,0,0) != 0)
	{
		if(TranslateAccelerator(msg.hwnd,hAccelTable,&msg) == 0)
		{
			TranslateMessage(&msg);
			DispatchMessageA(&msg);
		}
	}

	CMiniDump::Clean();

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
	wcex.hCursor = LoadCursor(0,IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName = (LPCSTR)IDC_JOINSERVER;
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance,(LPCTSTR)IDI_ICON1);

	return RegisterClassEx(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance,int nCmdShow) // OK
{
	hInst = hInstance;

	hWnd = CreateWindow(szWindowClass,szTitle,WS_OVERLAPPEDWINDOW | WS_THICKFRAME,CW_USEDEFAULT,0,600,600,0,0,hInstance,0);

	if(hWnd == 0)
	{
		return 0;
	}

	ShowWindow(hWnd,nCmdShow);
	UpdateWindow(hWnd);
	return 1;
}

LRESULT CALLBACK WndProc(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam) // OK
{
	switch(message)
	{
		case WM_COMMAND:
			switch(LOWORD(wParam))
			{
				case IDM_ABOUT:
					DialogBox(hInst,(LPCTSTR)IDD_ABOUTBOX,hWnd,(DLGPROC)About);
					break;
				case ID_OPTIONS_DISCONNECT:
					DialogBox(hInst,(LPCTSTR)IDD_DISCONNECT,hWnd,(DLGPROC)Disconnect);
					break;
				case IDM_EXIT:
					if(MessageBox(0,"Are you sure to terminate JoinServer?","Ask terminate server",MB_YESNO | MB_ICONQUESTION) == IDYES)
					{
						DestroyWindow(hWnd);
					}
					break;
				default:
					return DefWindowProc(hWnd,message,wParam,lParam);
			}
			break;
		case WM_TIMER:
			switch(wParam)
			{
				case TIMER_1000:
					JoinServerLiveProc();
					gAccountManager.DisconnectProc();
					break;
				case TIMER_2000:
					gServerDisplayer.Run();
					break;
				default:
					break;
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
			SetClassLong(hDlg, GCL_HICON, (long)LoadIcon(0, IDI_APPLICATION));
			hlst = GetDlgItem(hDlg, IDC_LIST1);
			gAccountManager.GetAccounts(hlst);

			return 1;
			break;
		case WM_COMMAND:
			int wmId    = LOWORD(wParam);
			int wmEvent = HIWORD(wParam);

			switch(wmId)
			{
				case IDC_RELOAD:
					{
						HWND hlst;
						SetClassLong(hDlg, GCL_HICON, (long)LoadIcon(0, IDI_APPLICATION));
						hlst = GetDlgItem(hDlg, IDC_LIST1);
						SendMessage(hlst, LB_RESETCONTENT, 0, NULL);
						gAccountManager.GetAccounts(hlst);
					}
					break;
				case IDC_DISCONNECT:
					{
						char text[11]={0};
						HWND hlst;
						SetClassLong(hDlg, GCL_HICON, (long)LoadIcon(0, IDI_APPLICATION));
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
							JGDisconnectAccountSend(text);
							SendMessage(hlst, LB_DELETESTRING, (WPARAM)iSelected, NULL);
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