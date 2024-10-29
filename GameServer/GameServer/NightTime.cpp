#include "stdafx.h"
#include "NightTime.h"
#include "MemScript.h"
#include "Util.h"

cNightTime gNightTime;

cNightTime::cNightTime()
{
	
}

cNightTime::~cNightTime()
{
	
}

void cNightTime::Load(char* path)
{
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

	for(int i=0;i<MAX_MAP;i++)
	{
		this->Maps[i] = -1;
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

					//TVT_EVENT_START_TIME info;

					WorkTime.Year= lpMemScript->GetNumber();

					WorkTime.Month = lpMemScript->GetAsNumber();

					WorkTime.Day = lpMemScript->GetAsNumber();

					WorkTime.DayOfWeek = lpMemScript->GetAsNumber();

					WorkTime.Hour = lpMemScript->GetAsNumber();

					WorkTime.Minute = lpMemScript->GetAsNumber();

					WorkTime.Second = lpMemScript->GetAsNumber();

					WorkTime.AlarmTime = lpMemScript->GetAsNumber();

					WorkTime.EventTime = lpMemScript->GetAsNumber();

					//this->m_TvTEventInfo[index].StartTime.push_back(info);
				}
				else if(section == 1)
				{
					if(strcmp("end",lpMemScript->GetAsString()) == 0)
					{
						break;
					}

					strcpy_s(this->Messages.StartMessage,lpMemScript->GetString());
					strcpy_s(this->Messages.EndMessage,lpMemScript->GetAsString());
				}
				else if(section == 2)
				{
					if(strcmp("end",lpMemScript->GetAsString()) == 0)
					{
						break;
					}

					static int Index = 0;;
					this->Maps[Index++]=lpMemScript->GetNumber();
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

bool cNightTime::Check()
{
	time_t TimeLocal;
	struct tm * LocalT;
	time(&TimeLocal);
	// ----
	LocalT = localtime(&TimeLocal); 
	// -----
	if (this->WorkTime.DayOfWeek == -1)
	{
		//if ()
	}
	else
	{
		
	}

	return true;
}
