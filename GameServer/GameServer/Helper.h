// Helper.h: interface for the CHelper class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "Protocol.h"

#define MAX_HELPER_STAGE 5

//**********************************************//
//************ Client -> GameServer ************//
//**********************************************//

struct PMSG_HELPER_DATA_RECV
{
	PWMSG_HEAD header; // C1:AE
	BYTE result;
	BYTE data[256];
	/*BYTE gap01[64];
	BYTE Item1[16];
	BYTE Item2[16];
	BYTE Item3[16];
	BYTE Item4[16];
	BYTE Item5[16];
	BYTE Item6[16];
	BYTE Item7[16];
	BYTE Item8[16];
	BYTE Item9[16];
	BYTE Item10[16];
	BYTE Item11[16];
	BYTE Item12[16];*/
	/* 
	 * 0x03 (1 байт ) - диаметр ударов
	 * 0x08 (2 байта) - скилл удара
	 * 0x64 (16 байт) - итем
	 * 12 итемов максимум
	 */
};

struct PMSG_HELPER_START_RECV
{
	PSBMSG_HEAD header; // C1:BF:51
	BYTE type;
};

//**********************************************//
//************ GameServer -> Client ************//
//**********************************************//

struct PMSG_HELPER_DATA_SEND
{
	PWMSG_HEAD header; // C1:AE
	BYTE result;
	BYTE data[256];
};

struct PMSG_HELPER_START_SEND
{
	PSBMSG_HEAD header; // C1:BF:51
	DWORD time;
	DWORD money;
	BYTE result;
};

//**********************************************//
//********** DataServer -> GameServer **********//
//**********************************************//

struct SDHP_HELPER_DATA_RECV
{
	PSWMSG_HEAD header; // C1:17:00
	WORD index;
	char account[11];
	char name[11];
	BYTE result;
	BYTE data[256];
};

//**********************************************//
//********** GameServer -> DataServer **********//
//**********************************************//

struct SDHP_HELPER_DATA_SEND
{
	PSBMSG_HEAD header; // C1:17:00
	WORD index;
	char account[11];
	char name[11];
};

struct SDHP_HELPER_DATA_SAVE_SEND
{
	PSWMSG_HEAD header; // C1:17:30
	WORD index;
	char account[11];
	char name[11];
	BYTE data[256];
};

//**********************************************//
//**********************************************//
//**********************************************//

class CHelper
{
public:
	CHelper();
	virtual ~CHelper();
	void MainProc();
	void DisableHelper(LPOBJ lpObj);
	void CGHelperDataRecv(PMSG_HELPER_DATA_RECV* lpMsg,int aIndex);
	void CGHelperStartRecv(PMSG_HELPER_START_RECV* lpMsg,int aIndex);
	void GCHelperStartSend(int aIndex,int time,int money,int result);
	void DGHelperDataRecv(SDHP_HELPER_DATA_RECV* lpMsg);
	void GDHelperDataSend(int aIndex);
	void GDHelperDataSaveSend(int aIndex,BYTE* data);
};

extern CHelper gHelper;
