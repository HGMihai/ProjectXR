#pragma once

#include "Protocol.h"

struct CSUM_LIST_INFO
{
	char path[MAX_PATH];
	DWORD checksum;
};

struct SDHP_CHECKSUM_LIST_SEND
{
	PSWMSG_HEAD header;
	BYTE isFirstSend;
	WORD MaxCount;
	BYTE count;
};

class CFileCheck
{
public:
	void Load(char* path);
	void GCChecksumListSend(int aIndex);
	std::vector<CSUM_LIST_INFO> gChecksumListInfo;
}; extern CFileCheck gFileCheck;