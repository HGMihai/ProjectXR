// ShopManager.cpp: implementation of the CShopManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ShopManager.h"
#include "CastleSiegeSync.h"
#include "MemScript.h"
#include "NpcTalk.h"
#include "Path.h"
#include "Util.h"
#include "DonateShop.h"

CShopManager gShopManager;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CShopManager::CShopManager() // OK
{
	this->m_ShopManagerInfo.clear();
}

CShopManager::~CShopManager() // OK
{

}

void CShopManager::Load(char* path)
{
	pugi::xml_document file;
	pugi::xml_parse_result res = file.load_file(path);

	if(res.status != pugi::status_ok)
	{
		ErrorMessageBox("%s file load fail (%s)", path, res.description());
	}

	this->m_ShopManagerInfo.clear();

	pugi::xml_node ShopManager = file.child("ShopManager");
	for (pugi::xml_node shop = ShopManager.child("Shop"); shop; shop = shop.next_sibling())
	{
		SHOP_MANAGER_INFO info;

		info.Index = shop.attribute("Index").as_int();

		info.MonsterClass = shop.attribute("Class").as_int();

		#if (GAMESERVER_UPDATE == 603 || GAMESERVER_UPDATE == 601)
		info.DonateShopIndex = shop.attribute("DonateShopIndex").as_int();
		#endif

		this->m_ShopManagerInfo.insert(std::pair<int,SHOP_MANAGER_INFO>(info.Index,info));
	}
}

void CShopManager::LoadShop() // OK
{
	std::map<int,int> LoadShop;

	char wildcard_path[MAX_PATH];

	wsprintf(wildcard_path,"%s*",gPath.GetFullPath("Shop\\"));

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
					std::map<int,SHOP_MANAGER_INFO>::iterator it = this->m_ShopManagerInfo.find(atoi(data.cFileName));

					if(it != this->m_ShopManagerInfo.end())
					{
						if(LoadShop.find(it->first) == LoadShop.end())
						{
							char path[MAX_PATH];

							wsprintf(path,"Shop\\%s",data.cFileName);
#if (GAMESERVER_UPDATE == 603 || GAMESERVER_UPDATE == 601)
							if (it->second.DonateShopIndex == -1)
							{
								it->second.Shop.Load(gPath.GetFullPath(path));
							}
#endif
							LoadShop.insert(std::pair<int,int>(it->first,1));
						}
					}
				}
			}
		}
	}
	while(FindNextFile(file,&data) != 0);
}

void CShopManager::LoadDonateShop() // OK
{
	std::map<int,int> LoadShop;

	char wildcard_path[MAX_PATH];

	wsprintf(wildcard_path,"%s*",gPath.GetFullPath("Shop\\DonateShop\\"));

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
					std::map<int,SHOP_MANAGER_INFO>::iterator it = this->m_ShopManagerInfo.find(atoi(data.cFileName));

					if(it != this->m_ShopManagerInfo.end())
					{
						if(LoadShop.find(it->first) == LoadShop.end())
						{
							char path[MAX_PATH];

							wsprintf(path,"Shop\\DonateShop\\%s",data.cFileName);
#if (GAMESERVER_UPDATE == 603)
							if (it->second.DonateShopIndex != -1)
							{
								it->second.Shop.LoadDonateShop(gPath.GetFullPath(path), it->second.Index);
							}
#endif
							LoadShop.insert(std::pair<int,int>(it->first,1));
						}
					}
				}
			}
		}
	}
	while(FindNextFile(file,&data) != 0);
}

void CShopManager::ReloadShopInterface() // OK
{
	for(int n=OBJECT_START_USER;n < MAX_OBJECT;n++)
	{
		if(gObjIsConnectedGP(n) != 0)
		{
			if(gObj[n].Interface.use != 0 && gObj[n].Interface.type == INTERFACE_SHOP)
			{
				gObj[n].Interface.state = 1;

				PMSG_NPC_TALK_SEND pMsg;

				pMsg.header.setE(0x30,sizeof(pMsg));

				pMsg.result = 0;

				DataSend(n,(BYTE*)&pMsg,pMsg.header.size);

				this->GCShopItemListSendByIndex(gObj[n].TargetShopNumber,n);

				GCTaxInfoSend(n,2,gCastleSiegeSync.GetTaxRateStore(n));

				//gDonateShop.GCDonateShopInfo(n);
			}
		}
	}
}

long CShopManager::GetShopNumber(int MonsterClass) // OK
{
	for(std::map<int,SHOP_MANAGER_INFO>::iterator it=this->m_ShopManagerInfo.begin();it != this->m_ShopManagerInfo.end();it++)
	{
		if(it->second.MonsterClass != -1 && it->second.MonsterClass == MonsterClass)
		{
			return it->second.Index;
		}
	}

	return -1;
}

long CShopManager::GetClassByShopNumber(int ShopNumber)
{
	for(std::map<int,SHOP_MANAGER_INFO>::iterator it=this->m_ShopManagerInfo.begin();it != this->m_ShopManagerInfo.end();it++)
	{
		if(it->second.Index == ShopNumber)
		{
			return it->second.MonsterClass;
		}
	}

	return -1;
}


#if (GAMESERVER_UPDATE == 603)
long CShopManager::GetShopNumberByDonateShopIndex(int MonsterClass, int DonateShopIndex)
{
	for(std::map<int,SHOP_MANAGER_INFO>::iterator it=this->m_ShopManagerInfo.begin();it != this->m_ShopManagerInfo.end();it++)
	{
		if(it->second.MonsterClass == MonsterClass && it->second.DonateShopIndex == DonateShopIndex)
		{
			return it->second.Index;
		}
	}

	return -1;
}

long CShopManager::GetDonateShopCount(int MonsterClass)
{
	int Count = 0;
	for(std::map<int,SHOP_MANAGER_INFO>::iterator it=this->m_ShopManagerInfo.begin();it != this->m_ShopManagerInfo.end();it++)
	{
		if(it->second.MonsterClass == MonsterClass && it->second.DonateShopIndex >= 0)
		{
			Count++;
		}
	}

	return Count;
}

long CShopManager::IsDonateShop(int MonsterClass)
{
	for(std::map<int,SHOP_MANAGER_INFO>::iterator it=this->m_ShopManagerInfo.begin();it != this->m_ShopManagerInfo.end();it++)
	{
		if(it->second.MonsterClass != -1 && it->second.MonsterClass == MonsterClass && it->second.DonateShopIndex != -1)
		{
			return 1;
		}
	}

	return 0;
}
#endif

bool CShopManager::GetItemByIndex(int index,CItem* lpItem,int slot) // OK
{
	std::map<int,SHOP_MANAGER_INFO>::iterator it = this->m_ShopManagerInfo.find(index);

	if(it == this->m_ShopManagerInfo.end())
	{
		return 0;
	}
	else
	{
		return it->second.Shop.GetItem(lpItem,slot);
	}
}

int CShopManager::GetPriceByItem(CItem* lpItem, int* type)
{
	int price = 0;

	for (auto& it : this->m_ShopManagerInfo)
	{
		price = it.second.Shop.GetItemPrice(lpItem, type);

		if (price != 0)
			return price;
	}

	return 0;
	/*std::map<int,SHOP_MANAGER_INFO>::iterator it = this->m_ShopManagerInfo.find(lpItem->m_IsExpiredItem);

	if(it == this->m_ShopManagerInfo.end())
	{
		return 0;
	}
	else
	{
		return it->second.Shop.GetItemPrice(lpItem, type);
	}*/
}


bool CShopManager::GetItemByMonsterClass(int MonsterClass,CItem* lpItem,int slot) // OK
{
	for(std::map<int,SHOP_MANAGER_INFO>::iterator it=this->m_ShopManagerInfo.begin();it != this->m_ShopManagerInfo.end();it++)
	{
		if(it->second.MonsterClass != -1 && it->second.MonsterClass == MonsterClass)
		{
			return it->second.Shop.GetItem(lpItem,slot);
		}
	}

	return 0;
}

long CShopManager::GetItemCountByIndex(int index) // OK
{
	std::map<int,SHOP_MANAGER_INFO>::iterator it = this->m_ShopManagerInfo.find(index);

	if(it == this->m_ShopManagerInfo.end())
	{
		return 0;
	}
	else
	{
		return it->second.Shop.GetItemCount();
	}
}

long CShopManager::GetItemCountByMonsterClass(int MonsterClass) // OK
{
	for(std::map<int,SHOP_MANAGER_INFO>::iterator it=this->m_ShopManagerInfo.begin();it != this->m_ShopManagerInfo.end();it++)
	{
		if(it->second.MonsterClass != -1 && it->second.MonsterClass == MonsterClass)
		{
			return it->second.Shop.GetItemCount();
		}
	}

	return 0;
}

bool CShopManager::GCShopItemListSendByIndex(int index,int aIndex) // OK
{
	std::map<int,SHOP_MANAGER_INFO>::iterator it = this->m_ShopManagerInfo.find(index);

	if(it == this->m_ShopManagerInfo.end())
	{
		return 0;
	}
	else
	{
		return it->second.Shop.GCShopItemListSend(aIndex);
	}
}

bool CShopManager::GCShopItemListSendByMonsterClass(int MonsterClass,int aIndex) // OK
{
	for(std::map<int,SHOP_MANAGER_INFO>::iterator it=this->m_ShopManagerInfo.begin();it != this->m_ShopManagerInfo.end();it++)
	{
		if(it->second.MonsterClass != -1 && it->second.MonsterClass == MonsterClass)
		{
			return it->second.Shop.GCShopItemListSend(aIndex);
		}
	}

	return 0;
}
