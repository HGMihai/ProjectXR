// ItemBagManager.cpp: implementation of the CItemBagManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ItemBagManager.h"
#include "MemScript.h"
#include "Path.h"
#include "Util.h"

CItemBagManager gItemBagManager;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CItemBagManager::CItemBagManager() // OK
{
	this->m_ItemBagManagerInfo.clear();
}

CItemBagManager::~CItemBagManager() // OK
{

}

void CItemBagManager::Load(char* path)
{
	pugi::xml_document file;
	pugi::xml_parse_result res = file.load_file(path);

	if(res.status != pugi::status_ok)
	{
		ErrorMessageBox("%s file load fail (%s)", path, res.description());
	}

	this->m_ItemBagManagerInfo.clear();

	pugi::xml_node EventbagManager = file.child("EventItemBagManager");
	for (pugi::xml_node ItemBag = EventbagManager.child("ItemBag"); ItemBag; ItemBag = ItemBag.next_sibling())
	{
		ITEM_BAG_MANAGER_INFO info;

		info.Index = ItemBag.attribute("Index").as_int();

		info.ItemIndex = ItemBag.attribute("ItemIndex").as_int();

		info.ItemLevel = ItemBag.attribute("ItemLevel").as_int();

		info.MonsterClass = ItemBag.attribute("MonsterClass").as_int();

		info.SpecialValue = ItemBag.attribute("SpecialValue").as_int();

		this->m_ItemBagManagerInfo.insert(std::pair<int,ITEM_BAG_MANAGER_INFO>(info.Index,info));
	}
}

//void CItemBagManager::Load(char* path) // OK
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
//	pugi::xml_document doc;
//	pugi::xml_node declarationNode = doc.append_child(pugi::node_declaration);
//    declarationNode.append_attribute("version")    = "1.0";
//    declarationNode.append_attribute("encoding")   = "utf-8";
//
//    pugi::xml_node root = doc.append_child("EventItemBagManager");
//
//	this->m_ItemBagManagerInfo.clear();
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
//			ITEM_BAG_MANAGER_INFO info;
//
//			info.Index = lpMemScript->GetNumber();
//
//			info.ItemIndex = lpMemScript->GetAsNumber();
//
//			info.ItemLevel = lpMemScript->GetAsNumber();
//
//			info.MonsterClass = lpMemScript->GetAsNumber();
//
//			info.SpecialValue = lpMemScript->GetAsNumber();
//
//			this->m_ItemBagManagerInfo.insert(std::pair<int,ITEM_BAG_MANAGER_INFO>(info.Index,info));
//
//			pugi::xml_node nodeChild = root.append_child("ItemBag");
//
//			nodeChild.append_attribute("Index") = info.Index;
//
//			nodeChild.append_attribute("ItemIndex") = info.ItemIndex;
//
//			nodeChild.append_attribute("ItemLevel") = info.ItemLevel;
//
//			nodeChild.append_attribute("MonsterClass") = info.MonsterClass;
//
//			nodeChild.append_attribute("SpecialValue") = info.SpecialValue;
//
//
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

void CItemBagManager::LoadEventItemBag() // OK
{
	std::map<int,int> LoadEventItemBag;

	char wildcard_path[MAX_PATH];

	wsprintf(wildcard_path,"%s*",gPath.GetFullPath("EventItemBag\\"));

	WIN32_FIND_DATA data;

	HANDLE file = FindFirstFile(wildcard_path,&data);

	if(file == INVALID_HANDLE_VALUE)
	{
		return;
	}

	do
	{
		if((data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0)
		{
			if(isdigit(data.cFileName[0]) != 0 && isdigit(data.cFileName[1]) != 0 && isdigit(data.cFileName[2]) != 0)
			{
				if(data.cFileName[3] == ' ' && data.cFileName[4] == '-' && data.cFileName[5] == ' ')
				{
					std::map<int,ITEM_BAG_MANAGER_INFO>::iterator it = this->m_ItemBagManagerInfo.find(atoi(data.cFileName));

					if(it != this->m_ItemBagManagerInfo.end())
					{
						if(LoadEventItemBag.find(it->first) == LoadEventItemBag.end())
						{
							char path[MAX_PATH];

							wsprintf(path,"EventItemBag\\%s",data.cFileName);

							it->second.ItemBag.Load(gPath.GetFullPath(path));

							LoadEventItemBag.insert(std::pair<int,int>(it->first,1));
						}
					}
				}
			}
		}
	}
	while(FindNextFile(file,&data) != 0);

	// check if itembag not loaded
	for (auto it = this->m_ItemBagManagerInfo.begin(); it != this->m_ItemBagManagerInfo.end(); ++it)
	{
		auto loaded = LoadEventItemBag.find(it->first);

		if (loaded == LoadEventItemBag.end())
			ErrorMessageBox("ItemBag with [%d] index not loaded!", it->first);
	}
}

bool CItemBagManager::GetItemByItemIndex(int ItemIndex,int ItemLevel,LPOBJ lpObj,CItem* lpItem) // OK
{
	for(std::map<int,ITEM_BAG_MANAGER_INFO>::iterator it=this->m_ItemBagManagerInfo.begin();it != this->m_ItemBagManagerInfo.end();it++)
	{
		if(it->second.ItemIndex != -1 && it->second.ItemIndex == ItemIndex && (it->second.ItemLevel == -1 || it->second.ItemLevel == ItemLevel))
		{
			return it->second.ItemBag.GetItem(lpObj,lpItem);
		}
	}

	return 0;
}

bool CItemBagManager::GetItemByMonsterClass(int MonsterClass,LPOBJ lpObj,CItem* lpItem) // OK
{
	for(std::map<int,ITEM_BAG_MANAGER_INFO>::iterator it=this->m_ItemBagManagerInfo.begin();it != this->m_ItemBagManagerInfo.end();it++)
	{
		if(it->second.MonsterClass != -1 && it->second.MonsterClass == MonsterClass)
		{
			return it->second.ItemBag.GetItem(lpObj,lpItem);
		}
	}

	return 0;
}

bool CItemBagManager::GetItemBySpecialValue(int SpecialValue,LPOBJ lpObj,CItem* lpItem) // OK
{
	for(std::map<int,ITEM_BAG_MANAGER_INFO>::iterator it=this->m_ItemBagManagerInfo.begin();it != this->m_ItemBagManagerInfo.end();it++)
	{
		if(it->second.SpecialValue != -1 && it->second.SpecialValue == SpecialValue)
		{
			return it->second.ItemBag.GetItem(lpObj,lpItem);
		}
	}

	return 0;
}

bool CItemBagManager::DropItemByItemIndex(int ItemIndex,int ItemLevel,LPOBJ lpObj,int map,int x,int y) // OK
{
	for(std::map<int,ITEM_BAG_MANAGER_INFO>::iterator it=this->m_ItemBagManagerInfo.begin();it != this->m_ItemBagManagerInfo.end();it++)
	{
		if(it->second.ItemIndex != -1 && it->second.ItemIndex == ItemIndex && (it->second.ItemLevel == -1 || it->second.ItemLevel == ItemLevel))
		{
			return it->second.ItemBag.DropItem(lpObj,map,x,y);
		}
	}

	return 0;
}

bool CItemBagManager::DropItemByMonsterClass(int MonsterClass,LPOBJ lpObj,int map,int x,int y) // OK
{
	for(std::map<int,ITEM_BAG_MANAGER_INFO>::iterator it=this->m_ItemBagManagerInfo.begin();it != this->m_ItemBagManagerInfo.end();it++)
	{
		if(it->second.MonsterClass != -1 && it->second.MonsterClass == MonsterClass)
		{
			return it->second.ItemBag.DropItem(lpObj,map,x,y);
		}
	}

	return 0;
}

bool CItemBagManager::DropItemBySpecialValue(int SpecialValue,LPOBJ lpObj,int map,int x,int y) // OK
{
	for(std::map<int,ITEM_BAG_MANAGER_INFO>::iterator it=this->m_ItemBagManagerInfo.begin();it != this->m_ItemBagManagerInfo.end();it++)
	{
		if(it->second.SpecialValue != -1 && it->second.SpecialValue == SpecialValue)
		{
			return it->second.ItemBag.DropItem(lpObj,map,x,y);
		}
	}

	return 0;
}
