// Filter.cpp: implementation of the CFilter class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Filter.h"
#include "MemScript.h"
#include "Util.h"

CFilter gFilter;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFilter::CFilter() // OK
{
	//this->m_count = 0;
}

CFilter::~CFilter() // OK
{

}

void CFilter::Load(char* path) // OK
{
	this->m_SwearWords.clear();

	pugi::xml_document file;
	pugi::xml_parse_result res = file.load_file(path);

	if (res.status != pugi::status_ok)
	{
		ErrorMessageBox("%s file load failed - %s", path, res.description());
		return;
	}

	pugi::xml_node main_section = file.child("Filter");
	std::string Swear;

	for (pugi::xml_node word = main_section.child("Restrict"); word; word = word.next_sibling())
	{
		Swear = word.attribute("Word").as_string();
		this->m_SwearWords.push_back(Swear);
	}

	LogAdd(LOG_BLACK,"Loaded %d prohibited words", this->m_SwearWords.size());
	/*CMemScript* lpMemScript = new CMemScript;

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

	this->m_count = 0;

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

			FILTER_INFO info;

			memset(&info,0,sizeof(info));

			strcpy_s(info.label,lpMemScript->GetString());

			this->SetInfo(info);
		}
	}
	catch(...)
	{
		ErrorMessageBox(lpMemScript->GetLastError());
	}

	delete lpMemScript;*/
}

//void CFilter::SetInfo(FILTER_INFO info) // OK
//{
//	if(this->m_count < 0 || this->m_count >= MAX_FILTER_SYNTAX)
//	{
//		return;
//	}
//
//	this->m_FilterInfo[this->m_count++] = info;
//}

//void CFilter::CheckSyntax(char* text) // OK
//{
//	for(int n=0;n < this->m_count;n++)
//	{
//		char* temp = text;
//
//		while(true)
//		{
//			temp = strstr(temp,this->m_FilterInfo[n].label);
//
//			if(temp == 0)
//			{
//				break;
//			}
//
//			int len = strlen(this->m_FilterInfo[n].label);
//
//			memset(temp,0x2A,len);
//
//			temp += len;
//		}
//	}
//}

bool CFilter::CheckSyntax(char* text)
{
	std::vector<std::string>::iterator It;
	for(It = this->m_SwearWords.begin(); It != this->m_SwearWords.end(); ++It)
	{
		if(strstr(text, It->c_str()) != 0)
		{
			return true;
		}
	}
	return false;
}
