#pragma once

#define WIN32_LEAN_AND_MEAN

#define _WIN32_WINNT _WIN32_WINNT_WINXP

#define GAMESERVER_TITLE "LTP GameServer"
#define GAMESERVER_VERSION "1.6.11"

#ifndef GAMESERVER_TYPE
#define GAMESERVER_TYPE 0
#endif

#ifndef GAMESERVER_EXTRA
#define GAMESERVER_EXTRA 1

#endif
//#define _EMKA_

#ifndef GAMESERVER_UPDATE
#define GAMESERVER_UPDATE 603
#endif

#ifndef GAMESERVER_LANGUAGE
#define GAMESERVER_LANGUAGE 1
#endif

#if (GAMESERVER_UPDATE > 803)
#define GAMESERVER_LANGUAGE 1
#endif

#define SEASON9DOT3 TRUE

#ifndef PROTECT_STATE
#define PROTECT_STATE 1
#endif

#define USE_MHP_SERVER TRUE

//#if (GAMESERVER_UPDATE >= 803)
//#define GAMESERVER_EXTRA 0
////#define ENCRYPT_STATE 0
//#endif

#ifndef ENCRYPT_STATE
#define ENCRYPT_STATE 1
#endif

#ifdef IS_NEW_CLIENT
#define ENCRYPT_STATE 0
#endif

#define USE_TELEPORTPANEL FALSE

#define USE_TEST_CUSTOMSMANAGER TRUE

#define USE_OFFEXP_RELOAD FALSE

#define USE_OFFLINEMODE_HELPER_PICK TRUE

#define USE_USERLIMIT	FALSE
#define MAX_USERCOUNT	3

#define PROTECT_PORT 1356

#define USE_TESTOFFEXP FALSE

#define CUSTOM_PERSONAL_SHOP FALSE

#define NEW_PARTY_CUSTOM TRUE

#define MAX_LEVEL_TEST 0

#define GAMESERVER_WIDTH  960 // Width of window def:800
#define GAMESERVER_HEIGHT  660 // Height of window def:600

#define CS_SET_CLASS_SX(x)		( ((x)<<4))		 //
#define CS_GET_CLASS_SX(x)		( (((x)>>4)<<4)) //
#define CS_GET_CHANGEUP_SX(x)	( ((x)&0x07) )	
#define CS_GET_CHANGEUP_SXII(x)	( (((x)&14)>>1) )	
#define CS_SET_CHANGEUP_SX		0x08
#define CS_SET_CHANGEUP_SXI		0xA
#define CS_SET_CHANGEUP2_SX		0x04
#define CS_SET_CHANGEUP4_SXII    0x02
#define CS_SET_CHANGEUP_SXII		0x08
#define CS_SET_CHANGEUP2_SXII		0x04
#define DBI_GET_TYPE(x) ( ((x)&((16-1)<<8))>>4 )
#define DBI_GET_INDEX(x) ( (x) & 0xFF )
#define CS_SET_BOOTS1(x) ( ((x) & 0x1E0) >> 5 )
#define CS_SET_BOOTS2(x) ( ((x) & 0x10 ) >> 1 )
#define CS_SET_BOOTS3(x) ( ((x) & 0x0F ) << 4 )
#define CS_SET_ARMOR1(x) ( ((x) & 0x1E0) >> 1 )
#define CS_SET_ARMOR2(x) ( ((x) & 0x10 ) << 2 )
#define CS_SET_ARMOR3(x) ( ((x) & 0x0F )      )
#define CS_SET_PANTS1(x) ( ((x) & 0x1E0) >> 5 )
#define CS_SET_PANTS2(x) ( ((x) & 0x10 ) << 1 )
#define CS_SET_PANTS3(x) ( ((x) & 0x0F ) << 4 )
#define CS_SET_GLOVES1(x) ( ((x) & 0x1E0) >> 1 )
#define CS_SET_GLOVES2(x) ( ((x) & 0x10 )      )
#define CS_SET_GLOVES3(x) ( ((x) & 0x0F )      )
#define CS_SET_HELMET1(x) ( ((x) & 0x1E0) >> 5 )
#define CS_SET_HELMET2(x) ( ((x) & 0x10 ) << 3 )
#define CS_SET_HELMET3(x) ( ((x) & 0x0F ) << 4 )
#define CS_SET_SMALLLEVEL_RH(x)		( (x)       )
#define CS_SET_SMALLLEVEL_LH(x)		( (x) << 3  ) 
#define CS_SET_SMALLLEVEL_HELMET(x)	( (x) << 6  )
#define CS_SET_SMALLLEVEL_ARMOR(x)	( (x) << 9  )
#define CS_SET_SMALLLEVEL_PANTS(x)	( (x) << 12 )
#define CS_SET_SMALLLEVEL_GLOVES(x)	( (x) << 15 )
#define CS_SET_SMALLLEVEL_BOOTS(x)	( (x) << 18 )
#define CS_SET_SMALLLEVEL1(x)		( ((x) >> 16) & 0xFF )
#define CS_SET_SMALLLEVEL2(x)		( ((x) >> 8 ) & 0xFF )
#define CS_SET_SMALLLEVEL3(x)		((x) & 0xFF )

// System Include
#include <windows.h>
#include <winsock2.h>
#include <mswSock.h>
#include <commctrl.h>
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <map>
#include <vector>
#include <queue>
#include <random>
#include <Rpc.h>
#include <algorithm>
#include <string>
#include <atltime.h>
#include <dbghelp.h>
#include <Psapi.h>
#include <CommCtrl.h>


#include "..\\..\\Util\\libpugixml\\pugixml\\pugixml.hpp"
#include "..\\..\\Util\\liblua\\lua-5.4.3\\src\\lua.hpp"
//#include "..\\..\\Util\\VMProtect\\VMProtectSDK.h"


#define CURL_STATICLIB

#ifdef _DEBUG
#pragma comment(lib,"..\\..\\Util\\curl\\libcurl_a_debug.lib")
#pragma comment(lib, "..\\..\\Debug\\liblua.lib")
#pragma comment(lib, "..\\..\\Debug\\libpugixml.lib")
#else
#pragma comment(lib,"..\\..\\Util\\curl\\libcurl_a.lib")
#pragma comment(lib, "..\\..\\Release\\liblua.lib")
#pragma comment(lib, "..\\..\\Release\\libpugixml.lib")
#endif

#include "../../Util/WinLicenseSDK/Include/C/WinlicenseSDK.h"
#include "../../Util/WinLicenseSDK/Include/C/SecureEngineSDK.h"


#if(GAMESERVER_UPDATE>=701)
#if(NDEBUG==0)
#pragma comment(lib,"..\\..\\Util\\cryptopp\\Debug\\cryptlib.lib")
#else
#pragma comment(lib,"..\\..\\Util\\cryptopp\\Release\\cryptlib.lib")
#endif
#pragma comment(lib,"..\\..\\Util\\mapm\\mapm.lib")
#endif

#pragma comment(lib,"ComCtl32.Lib")
#pragma comment(lib,"wldap32.lib")
#pragma comment(lib,"Normaliz.lib")
#pragma comment(lib,"Crypt32.lib")
#pragma comment(lib,"ws2_32.lib")
#pragma comment(lib,"Rpcrt4.lib")
#pragma comment(lib,"dbghelp.lib")
#pragma comment(lib,"Psapi.lib")
#pragma comment(lib,"urlmon.lib")

#include <filesystem>

//double _HUGE;
typedef char chr;

typedef float flt;

typedef short shrt;

typedef unsigned __int64 QWORD;
