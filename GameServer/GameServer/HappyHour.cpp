#include "stdafx.h"
#include "HappyHour.h"
#include "MemScript.h"
#include "Util.h"
#include "ServerInfo.h"
#include "Notice.h"


CHappyHour gHappyHour;

CHappyHour::CHappyHour()
{
	this->m_HappyHour.clear();
	this->EventID = -1;
	this->Enabled = 0;
	this->Tick = 0;
}

CHappyHour::~CHappyHour()
{
	
}

void CHappyHour::Load(char* path)
{
	this->DefaultExp[0] = GetPrivateProfileInt("GameServerInfo","AddExperienceRate_AL0",0,".\\Data\\GameServerInfo - Common.dat");
	this->DefaultExp[1] = GetPrivateProfileInt("GameServerInfo","AddExperienceRate_AL1",0,".\\Data\\GameServerInfo - Common.dat");
	this->DefaultExp[2] = GetPrivateProfileInt("GameServerInfo","AddExperienceRate_AL2",0,".\\Data\\GameServerInfo - Common.dat");
	this->DefaultExp[3] = GetPrivateProfileInt("GameServerInfo","AddExperienceRate_AL3",0,".\\Data\\GameServerInfo - Common.dat");

	this->DefaultDrop[0] = GetPrivateProfileInt("GameServerInfo","ItemDropRate_AL0",0,".\\Data\\GameServerInfo - Common.dat");
	this->DefaultDrop[1] = GetPrivateProfileInt("GameServerInfo","ItemDropRate_AL1",0,".\\Data\\GameServerInfo - Common.dat");
	this->DefaultDrop[2] = GetPrivateProfileInt("GameServerInfo","ItemDropRate_AL2",0,".\\Data\\GameServerInfo - Common.dat");
	this->DefaultDrop[3] = GetPrivateProfileInt("GameServerInfo","ItemDropRate_AL3",0,".\\Data\\GameServerInfo - Common.dat");

	CMemScript* lpMemScript = new CMemScript;

	if(lpMemScript == 0)
	{
		ErrorMessageBox(MEM_SCRIPT_ALLOC_ERROR,path);
		return;
	}

	if(lpMemScript->SetBuffer(path) == 0)
	{
		ErrorMessageBox(lpMemScript->GetLastError());
		delete lpMemScript;
		return;
	}

	try
	{
		while(true)
		{
			if(lpMemScript->GetToken() == TOKEN_END)
			{
				break;
			}

			int section = lpMemScript->GetNumber();

			while(true)
			{
				if(section == 0)
				{
					if(strcmp("end",lpMemScript->GetAsString()) == 0)
					{
						break;
					}

					this->Enabled = lpMemScript->GetNumber();

					this->AddExpirience[0] = lpMemScript->GetAsNumber();
					this->AddExpirience[1] = lpMemScript->GetAsNumber();
					this->AddExpirience[2] = lpMemScript->GetAsNumber();
					this->AddExpirience[3] = lpMemScript->GetAsNumber();

					this->AddDrop[0] = lpMemScript->GetAsNumber();
					this->AddDrop[1] = lpMemScript->GetAsNumber();
					this->AddDrop[2] = lpMemScript->GetAsNumber();
					this->AddDrop[3] = lpMemScript->GetAsNumber();

				}
				else if(section == 1)
				{
					if(strcmp("end",lpMemScript->GetAsString()) == 0)
					{
						break;
					}

					HAPPYHOUR_TIME info;


					info.sIndex = lpMemScript->GetNumber();

					info.sHour = lpMemScript->GetAsNumber();

					info.sMin = lpMemScript->GetAsNumber();

					info.sDuration = lpMemScript->GetAsNumber();

					info.sDuration *=60;

					this->m_HappyHour.push_back(info);
				}
				else
				{
					break;
				}
			}
		}
	}
	catch(...)
	{
		ErrorMessageBox(lpMemScript->GetLastError());
	}

	delete lpMemScript;
}

void CHappyHour::StartEvent(int ID)
{
	if (!this->Enabled)
	{
		return;
	}

	if (this->EventID == -1)
	{
		LogAdd(LOG_RED, "[Error] [%s] Please, check configs!"__FUNCTION__);
		return;
	}
	this->EventID = ID;

	//gServerInfo.m_AddExperienceRate[0] += this->AddExpirience[0];

	for (int i=0; i<4;i++)
	{
		gServerInfo.m_AddExperienceRate[i] += this->AddExpirience[i];
		gServerInfo.m_ItemDropRate[i] += this->AddDrop[i];
	}
	//gAddExperience += this->PlusExp;
	//gItemDropPer += this->PlusDrop;

	char text[256];
	sprintf(text, "[Happy Hour]: Started. Duration %d Min", this->m_HappyHour[ID].sDuration);
	//AllSendServerMsg(text);
	gNotice.GCNoticeSendToAll(0,0,0,0,0,0,text);
}

void CHappyHour::CloseEvent()
{
	for (int i=0; i<4;i++)
	{
		gServerInfo.m_AddExperienceRate[i] = this->DefaultExp[i];
		gServerInfo.m_ItemDropRate[i] = this->DefaultDrop[i];
	}
	gNotice.GCNoticeSendToAll(0,0,0,0,0,0,"[Happy Hour]: Event has finished");
}

void CHappyHour::TickTime()
{
	if (!this->Enabled)
	{
		return;
	}

	//for (int i=0; i<this->m_HappyHour.size(); i++)
	for(std::vector<HAPPYHOUR_TIME>::iterator it=this->m_HappyHour.begin();it != this->m_HappyHour.end();it++)
	{
		if (it->sIndex == this->EventID)
		{
			this->Tick++;
			if (this->Tick >= it->sDuration)
			{
				this->Tick=0;
				this->EventID = -1;
				this->CloseEvent();
			}
		}
	}

	SYSTEMTIME t;
	GetLocalTime(&t);

	if (t.wSecond == 00)
	{
		//for (int i=0; i<this->m_HappyHour.size(); i++)
		for(std::vector<HAPPYHOUR_TIME>::iterator it=this->m_HappyHour.begin();it != this->m_HappyHour.end();it++)
		{
			if (t.wHour == it->sHour && t.wMinute == it->sMin)
			{
				this->EventID = it->sIndex;
				this->StartEvent(it->sIndex);
			}
		}
	}
}