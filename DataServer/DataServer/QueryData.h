#pragma once

#define QUERY_COUNT 2

class CQueryData
{
public:
	CQueryData();
	void load(char* path);

	std::string QueryString[QUERY_COUNT];
}; extern CQueryData gQueryData;