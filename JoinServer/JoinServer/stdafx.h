#pragma once

#define WIN32_LEAN_AND_MEAN

#define _WIN32_WINNT _WIN32_WINNT_WINXP

#define JOINSERVER_TITLE "LTPTeam JoinServer"
#define JOINSERVER_VERSION "1.6.11"

#ifndef JOINSERVER_UPDATE
#define JOINSERVER_UPDATE 803
#endif

#ifndef PROTECT_STATE
#define PROTECT_STATE 0
#endif

#define PROTECT_PORT 44569

#define _WINSOCK_DEPRECATED_NO_WARNINGS

// System Include
#include <windows.h>
#include <winsock2.h>
#include <commctrl.h>
#include <sql.h>
#include <sqltypes.h>
#include <sqlext.h>
#include <process.h>
#include <iostream>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <time.h>
#include <math.h>
#include <map>
#include <vector>
#include <queue>
#include <string>
#include <algorithm>
#include <assert.h>
#include <fstream>
#include <Rpc.h>
#include <dbghelp.h>
#include <Psapi.h>

#include <Psapi.h>

#pragma comment(lib,"ws2_32.lib")
#pragma comment(lib,"comctl32.lib")
#pragma comment(lib,"Rpcrt4.lib")
#pragma comment(lib,"dbghelp.lib")
#pragma comment(lib,"Psapi.lib")

#ifdef _DEBUG
#pragma comment(lib, "..\\..\\Debug\\libbcrypt.lib")
#else
#pragma comment(lib, "..\\..\\Release\\libbcrypt.lib")
#endif

extern char CustomerName[32];
extern char CustomerHardwareId[36];
extern BOOL CaseSensitive;
extern BOOL MD5Encryption;
extern BOOL BCryptEncryption;
