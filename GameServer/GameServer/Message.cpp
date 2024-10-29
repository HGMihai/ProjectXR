// Message.cpp: implementation of the CMessage class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Message.h"
#include "MemScript.h"
#include "Util.h"

CMessage gMessage;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMessage::CMessage() // OK
{
	memset(this->m_DefaultMessage,0,sizeof(this->m_DefaultMessage));

	this->m_MessageInfo.clear();
}

CMessage::~CMessage() // OK
{

}

void CMessage::Load(char* path)
{
	pugi::xml_document file;
	pugi::xml_parse_result res = file.load_file(path);

	if(res.status != pugi::status_ok)
	{
		ErrorMessageBox("%s file load fail (%s)", path, res.description());
	}

	this->m_MessageInfo.clear();
	this->m_Map.clear();

	pugi::xml_node Message = file.child("Message");
	for (pugi::xml_node msg = Message.child("Msg"); msg; msg = msg.next_sibling())
	{
		MESSAGE_INFO info;

		info.Index = msg.attribute("Index").as_int();

		strcpy_s(info.Message,msg.attribute("Text").as_string());

		this->m_MessageInfo.insert(std::pair<int,MESSAGE_INFO>(info.Index,info));
	}

	pugi::xml_node map = file.child("Map");
	for (pugi::xml_node msg = map.child("Msg"); msg; msg = msg.next_sibling())
	{
		MAP_INFO info;
		info.Index = msg.attribute("ID").as_int();
		//std::string szText = msg.attribute("Text").as_string();
		strcpy_s(info.Name,msg.attribute("Text").as_string());

		this->m_Map.insert(std::pair<int, MAP_INFO>(info.Index, info));
	}
}

//void CMessage::Load(char* path) // OK
//{
//	CMemScript* lpMemScript = new CMemScript;
//
//	if(lpMemScript == 0)
//	{
//		ErrorMessageBox(MEM_SCRIPT_ALLOC_ERROR,path);
//		return;
//	}
//
//	if(lpMemScript->SetBuffer(path) == 0)
//	{
//		ErrorMessageBox(lpMemScript->GetLastError());
//		delete lpMemScript;
//		return;
//	}
//
//	this->m_MessageInfo.clear();
//	pugi::xml_document doc;
//	pugi::xml_node declarationNode = doc.append_child(pugi::node_declaration);
//    declarationNode.append_attribute("version")    = "1.0";
//    declarationNode.append_attribute("encoding")   = "utf-8";
//
//    pugi::xml_node root = doc.append_child("Message");
//
//	try
//	{
//		while(true)
//		{
//			if(lpMemScript->GetToken() == TOKEN_END)
//			{
//				break;
//			}
//
//			if(strcmp("end",lpMemScript->GetString()) == 0)
//			{
//				break;
//			}
//
//			MESSAGE_INFO info;
//
//			info.Index = lpMemScript->GetNumber();
//
//			strcpy_s(info.Message,lpMemScript->GetAsString());
//
//			this->m_MessageInfo.insert(std::pair<int,MESSAGE_INFO>(info.Index,info));
//
//			pugi::xml_node nodeChild = root.append_child("Msg");
//
//			nodeChild.append_attribute("Index") = info.Index;
//
//			nodeChild.append_attribute("Text") = info.Message;
//		}
//	}
//	catch(...)
//	{
//		ErrorMessageBox(lpMemScript->GetLastError());
//	}
//
//	doc.save_file("MonsterList.xml");
//	delete lpMemScript;
//}

char* CMessage::GetMessage(int index) // OK
{
	std::map<int,MESSAGE_INFO>::iterator it = this->m_MessageInfo.find(index);

	if(it == this->m_MessageInfo.end())
	{
		wsprintf(this->m_DefaultMessage,"Could not find message %d!",index);
		return this->m_DefaultMessage;
	}
	else
	{
		return it->second.Message;
	}
}

char* CMessage::GetMap(int MapID)
{
	//std::map<int,MAP_INFO>::iterator it = this->m_Map.find(MapID);
	std::map<int,MAP_INFO>::iterator it = this->m_Map.find(MapID);

	if(it == this->m_Map.end())
	{
		wsprintf(this->m_DefaultMessage,"Could not find Map %d!",MapID);
		return this->m_DefaultMessage;
	}
	else
	{
		return it->second.Name;
	}
}
