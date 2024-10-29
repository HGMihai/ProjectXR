#pragma once

#define WIN32_LEAN_AND_MEAN

#define _WIN32_WINNT _WIN32_WINNT_WINXP

#define DATASERVER_TITLE "LTPTeam DataServer"
#define DATASERVER_VERSION "1.6.11"

#ifndef DATASERVER_UPDATE
#define DATASERVER_UPDATE 603
#endif

#ifndef PROTECT_STATE
#define PROTECT_STATE 0
#endif

#define MULTI_CHARACTER

#define PROTECT_PORT 44569

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
#include <Rpc.h>
#include <dbghelp.h>
#include <Psapi.h>
#include <atltime.h>

#pragma comment(lib,"ws2_32.lib")
#pragma comment(lib,"comctl32.lib")
#pragma comment(lib,"Rpcrt4.lib")
#pragma comment(lib,"dbghelp.lib")
#pragma comment(lib,"Psapi.lib")

extern char CustomerName[32];
extern char CustomerHardwareId[36];

typedef unsigned __int64 QWORD;
