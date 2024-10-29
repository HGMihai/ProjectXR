// Message.h: interface for the CMessage class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

struct MESSAGE_INFO
{
	int Index;
	char Message[128];
};

struct MAP_INFO
{
	int Index;
	char Name[64];
};

class CMessage
{
public:
	CMessage();
	virtual ~CMessage();
	void Load(char* path);
	char* GetMessage(int index);
	char *GetMap(int MapID);
private:
	char m_DefaultMessage[128];
	std::map<int,MESSAGE_INFO> m_MessageInfo;
	std::map<int, MAP_INFO> m_Map;
};

extern CMessage gMessage;
