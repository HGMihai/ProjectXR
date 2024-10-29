// ResetTable.cpp: implementation of the CResetTable class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ResetTable.h"
#include "MemScript.h"
#include "ServerInfo.h"
#include "Util.h"

CResetTable gResetTable;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CResetTable::CResetTable() // OK
{
	this->m_ResetTableInfo.clear();
}

CResetTable::~CResetTable() // OK
{

}

void CResetTable::Load(char* path)
{
	pugi::xml_document file;
	pugi::xml_parse_result res = file.load_file(path);

	if (res.status != pugi::status_ok)
	{
		ErrorMessageBox("Failed to load %s file (%s)", path, res.description());
		return;
	}

	this->m_ResetTableInfo.clear();

	pugi::xml_node main = file.child("ResetTable");

	for (pugi::xml_node Info = main.child("Info"); Info; Info = Info.next_sibling())
	{
		RESET_TABLE_INFO Table;

		Table.MinReset = Info.attribute("MinReset").as_int();

		Table.MaxReset = Info.attribute("MaxReset").as_int();

		Table.MinGReset = Info.attribute("MinGReset").as_int();

		Table.MaxGReset = Info.attribute("MaxGReset").as_int();

		Table.Level[0] = Info.attribute("Level_AL0").as_int();

		Table.Level[1] = Info.attribute("Level_AL1").as_int();

		Table.Level[2] = Info.attribute("Level_AL2").as_int();

		Table.Level[3] = Info.attribute("Level_AL3").as_int();

		Table.Money[0] = Info.attribute("Money_AL0").as_int();

		Table.Money[1] = Info.attribute("Money_AL1").as_int();

		Table.Money[2] = Info.attribute("Money_AL2").as_int();

		Table.Money[3] = Info.attribute("Money_AL3").as_int();

		Table.Point[0] = Info.attribute("Point_AL0").as_int();

		Table.Point[1] = Info.attribute("Point_AL1").as_int();

		Table.Point[2] = Info.attribute("Point_AL2").as_int();

		Table.Point[3] = Info.attribute("Point_AL3").as_int();

		this->m_ResetTableInfo.push_back(Table);
	}
}

int CResetTable::GetResetLevel(LPOBJ lpObj) // OK
{
	for(std::vector<RESET_TABLE_INFO>::iterator it=this->m_ResetTableInfo.begin();it != this->m_ResetTableInfo.end();it++)
	{
		if(((lpObj->Reset+1) >= it->MinReset && (lpObj->Reset+1) <= it->MaxReset) && ((lpObj->MasterReset+1) >= it->MinGReset && (lpObj->MasterReset+1) <= it->MaxGReset))
		{
			if(it->Level[lpObj->AccountLevel] == -1)
			{
				return gServerInfo.m_CommandResetLevel[lpObj->AccountLevel];
			}
			else
			{
				return it->Level[lpObj->AccountLevel];
			}
		}
	}

	return gServerInfo.m_CommandResetLevel[lpObj->AccountLevel];
}

int CResetTable::GetResetMoney(LPOBJ lpObj) // OK
{
	for(std::vector<RESET_TABLE_INFO>::iterator it=this->m_ResetTableInfo.begin();it != this->m_ResetTableInfo.end();it++)
	{
		if(((lpObj->Reset+1) >= it->MinReset && (lpObj->Reset+1) <= it->MaxReset) && ((lpObj->MasterReset+1) >= it->MinGReset && (lpObj->MasterReset+1) <= it->MaxGReset))
		{
			if(it->Money[lpObj->AccountLevel] == -1)
			{
				return gServerInfo.m_CommandResetMoney[lpObj->AccountLevel] * (lpObj->Reset + 1);
			}
			else
			{
				return it->Money[lpObj->AccountLevel] * (lpObj->Reset + 1);
			}
		}
	}

	return gServerInfo.m_CommandResetMoney[lpObj->AccountLevel];
}

int CResetTable::GetResetPoint(LPOBJ lpObj) // OK
{
	int point = 0;

	for(int n=1;n <= lpObj->Reset;n++)
	{
		int AddPoint = gServerInfo.m_CommandResetPoint[lpObj->AccountLevel];

		for(int i=1;i<=lpObj->MasterReset;i++)
		{
			for(std::vector<RESET_TABLE_INFO>::iterator it=this->m_ResetTableInfo.begin();it != this->m_ResetTableInfo.end();it++)
			{
				if((n >= it->MinReset && n <= it->MaxReset) && (i>=it->MinGReset && i<=it->MaxGReset))
				{
					if(it->Point[lpObj->AccountLevel] == -1)
					{
						AddPoint = gServerInfo.m_CommandResetPoint[lpObj->AccountLevel];
						break;
					}
					else
					{
						AddPoint = it->Point[lpObj->AccountLevel];
						break;
					}
				}
			}
		}

		point += AddPoint;
	}

	return point;
}
