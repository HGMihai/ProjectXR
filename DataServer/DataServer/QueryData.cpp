#include "stdafx.h"
#include "QueryData.h"
#include "Util.h"

std::string get_profile_string(LPCSTR name, LPCSTR key, LPCSTR def, LPCSTR filename)
{
    char temp[1024];
    GetPrivateProfileString(name, key, def, temp, sizeof(temp), filename);
    return std::string(temp);
}

CQueryData gQueryData;

CQueryData::CQueryData()
{
	
}

void CQueryData::load(char* path)
{
	this->QueryString[0] = get_profile_string("Query","RankingsQuery1","",path);
	//this->QueryString[1] = get_profile_string("Query","RankingsQuery2","",path);

	LogAdd(LOG_RED,(char*)this->QueryString[0].c_str());
	//LogAdd(LOG_RED,(char*)this->QueryString[1].c_str());
	//GetPrivateProfileString("Query","RankingsQuery1","",this->QueryString[0],sizeof(CustomerName),".\\DataServer.ini");
	//GetPrivateProfileString("Query","RankingsQuery2","",CustomerName,sizeof(CustomerName),".\\DataServer.ini");
}
