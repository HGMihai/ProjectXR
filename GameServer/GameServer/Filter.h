// Filter.h: interface for the CFilter class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#define MAX_FILTER_SYNTAX 1000

struct FILTER_INFO
{
	char label[16];
};

class CFilter
{
public:
	CFilter();
	virtual ~CFilter();
	//void Init();
	void Load(char* path);
	//void SetInfo(FILTER_INFO info);
	bool CheckSyntax(char* text);
private:
	//FILTER_INFO m_FilterInfo[MAX_FILTER_SYNTAX];
	//int m_count;

	std::vector<std::string> m_SwearWords;
};

extern CFilter gFilter;
