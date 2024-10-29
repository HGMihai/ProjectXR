#pragma once

#define WIN32_LEAN_AND_MEAN

#define _WIN32_WINNT _WIN32_WINNT_WINXP

#define CONNECTSERVER_TITLE "LTPTeam ConnectServer"
#define CONNECTSERVER_VERSION "1.6.11"

#ifndef CONNECTSERVER_UPDATE
#define CONNECTSERVER_UPDATE 803
#endif

#ifndef PROTECT_STATE
#define PROTECT_STATE 0
#endif

#define PROTECT_PORT 44569

#define SEASON9DOT3 TRUE

// System Include
#include <windows.h>
#include <winsock2.h>
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

#pragma comment(lib,"ws2_32.lib")
#pragma comment(lib,"Rpcrt4.lib")
#pragma comment(lib,"dbghelp.lib")
#pragma comment(lib,"Psapi.lib")

extern char CustomerName[32];
extern char CustomerHardwareId[36];
extern long MaxIpConnection;

typedef unsigned __int64 QWORD;
