#include "stdafx.h"
#include "HackCheck.h"
#include "ServerInfo.h"
#include "Protection.h"
#include "Util.h"

BYTE EncDecKey1;
BYTE EncDecKey2;
BYTE MHPEncDecKey1;
BYTE MHPEncDecKey2;
char MHPCustomerName[32];

void DecryptData(BYTE* lpMsg,int size, int aIndex) // OK
{
	if(MHPEncDecKey1 != 0 || MHPEncDecKey2 != 0)
	{
		MHPDecryptData(lpMsg,size);
	}

	for(int n=0;n < size;n++)
	{
		lpMsg[n] = (lpMsg[n]^EncDecKey1)-EncDecKey2;
	}

	if(gServerInfo.UseWrongPacketsCheck == 2)
	{
		unsigned long long packetCounter = gObj[aIndex].recv_index--;

		BYTE arr[sizeof(packetCounter)];

		memcpy(arr, (char*)&packetCounter, sizeof(packetCounter));

		WORD EncDecKey = 0;

		BYTE Key1 = (BYTE)0x9A;
		BYTE Key2 = (BYTE)0x43;

		for (int n = 0; n < sizeof(arr); n++)
		{
			EncDecKey += (BYTE)(arr[n] ^ gServerInfo.m_ServerSerial[(n % sizeof(gServerInfo.m_ServerSerial))]);
		}

		Key1 += LOBYTE(EncDecKey);
		Key2 += HIBYTE(EncDecKey);

		for(int n=0;n < size;n++)
		{
			lpMsg[n] = (lpMsg[n]^Key1)-Key2;
		}

		//gObj[aIndex].recv_index--;
	}

	//LogAdd(LOG_TEST,"Size: %d, Index: %llu",size,gObj[aIndex].recv_index);
}

void EncryptData(BYTE* lpMsg,int size) // OK
{
	for(int n=0;n < size;n++)
	{
		lpMsg[n] = (lpMsg[n]+EncDecKey2)^EncDecKey1;
	}

	if(MHPEncDecKey1 != 0 || MHPEncDecKey2 != 0)
	{
		MHPEncryptData(lpMsg,size);
	}
}

void MHPDecryptData(BYTE* lpMsg,int size) // OK
{
	for(int n=0;n < size;n++)
	{
		lpMsg[n] = (lpMsg[n]^MHPEncDecKey1)-MHPEncDecKey2;
	}
}

void MHPEncryptData(BYTE* lpMsg,int size) // OK
{
	for(int n=0;n < size;n++)
	{
		lpMsg[n] = (lpMsg[n]+MHPEncDecKey2)^MHPEncDecKey1;
	}
}

void InitHackCheck() // OK
{
	VM_START

	WORD EncDecKey = 0;

	for(int n=0;n < /*sizeof(gProtection.CustomerName)*/32;n++)
	{
		EncDecKey += (BYTE)(gProtection.CustomerName[n]^gServerInfo.m_ServerSerial[(n%sizeof(gServerInfo.m_ServerSerial))]);
	}

	EncDecKey1 = (BYTE)0xA8;
	EncDecKey2 = (BYTE)0x16;

	EncDecKey1 += LOBYTE(EncDecKey);
	EncDecKey2 += HIBYTE(EncDecKey);

	GetPrivateProfileString("MHPServerInfo","CustomerName","",MHPCustomerName,sizeof(MHPCustomerName),"..\\Data\\Hack\\MHPServer.ini");

	MHPEncDecKey1 = GetPrivateProfileInt("MHPServerInfo","EncDecKey1",0,"..\\Data\\Hack\\MHPServer.ini");

	MHPEncDecKey2 = GetPrivateProfileInt("MHPServerInfo","EncDecKey2",0,"..\\Data\\Hack\\MHPServer.ini");

	if(MHPEncDecKey1 != 0 || MHPEncDecKey2 != 0)
	{
		WORD MHPEncDecKey = 0;

		for(int n=0;n < sizeof(MHPCustomerName);n++)
		{
			MHPEncDecKey += MHPCustomerName[n];
		}

		MHPEncDecKey1 += LOBYTE(MHPEncDecKey);
		MHPEncDecKey2 += HIBYTE(MHPEncDecKey);
	}

	VM_END
}
