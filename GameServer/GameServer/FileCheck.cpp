#include "stdafx.h"
#include "FileCheck.h"
#include "MemScript.h"
#include "Util.h"


CFileCheck gFileCheck;

void CFileCheck::Load(char* path)
{
	CMemScript* lpMemScript = new CMemScript;

	if(lpMemScript == nullptr)
	{
		return;
	}

	if(lpMemScript->SetBuffer(path) == 0)
	{
		ErrorMessageBox(lpMemScript->GetLastError());
		delete lpMemScript;
		return;
	}

	gChecksumListInfo.clear();

	try
	{
		while(true)
		{
			if(lpMemScript->GetToken() == TOKEN_END)
			{
				break;
			}

			if(strcmp("end",lpMemScript->GetString()) == 0)
			{
				break;
			}

			CSUM_LIST_INFO TempList;

			strcpy_s(TempList.path,lpMemScript->GetString());

			TempList.checksum = lpMemScript->GetAsDword();

			gChecksumListInfo.push_back(TempList);
		}
	}
	catch(...)
	{
		ErrorMessageBox("%s file read error",path);
	}

	delete lpMemScript;
}

void CFileCheck::GCChecksumListSend(int aIndex) // OK
{
	std::vector<CSUM_LIST_INFO>::iterator MakeList = this->gChecksumListInfo.begin();

	do
	{
		BYTE send[8192];

		SDHP_CHECKSUM_LIST_SEND pMsg;

		pMsg.header.set(0xF3,0xFC,0);

		int size = sizeof(pMsg);

		pMsg.MaxCount = this->gChecksumListInfo.size();

		pMsg.count = 0;

		pMsg.isFirstSend = 0;

		if (MakeList == this->gChecksumListInfo.begin())
		{
			pMsg.isFirstSend = 1;
		}

		CSUM_LIST_INFO info;

		for(;MakeList != this->gChecksumListInfo.end();MakeList++)
		{
			memcpy(info.path,MakeList->path,sizeof(info.path));

			info.checksum = MakeList->checksum;

			if((size+sizeof(info)) > sizeof(send))
			{
				break;
			}
			else
			{
				memcpy(&send[size],&info,sizeof(info));
				size += sizeof(info);

				pMsg.count++;
			}
		}

		pMsg.header.size[0] = SET_NUMBERHB(size);

		pMsg.header.size[1] = SET_NUMBERLB(size);

		memcpy(send,&pMsg,sizeof(pMsg));

		DataSend(aIndex,send,size);

		//gSocketManager.DataSend(index,send,size);
	}
	while(MakeList != this->gChecksumListInfo.end());
}