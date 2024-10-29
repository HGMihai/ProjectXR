// Shop.cpp: implementation of the CShop class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Shop.h"

#include "ItemValue.h"
#include "MemScript.h"
#include "User.h"
#include "Util.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CShop::CShop() // OK
{
	this->Init();
}

CShop::~CShop() // OK
{

}

void CShop::Init() // OK
{
	for(int n=0;n < SHOP_SIZE;n++)
	{
		this->m_Item[n].Clear();
		this->m_InventoryMap[n] = -1;
	}
}

void CShop::Load(char* path) // OK
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

	this->Init();

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

			int ItemIndex = SafeGetItem(GET_ITEM(lpMemScript->GetNumber(),lpMemScript->GetAsNumber()));

			int ItemLevel = lpMemScript->GetAsNumber();

			int ItemDurability = lpMemScript->GetAsNumber();

			int ItemOption1 = lpMemScript->GetAsNumber();

			int ItemOption2 = lpMemScript->GetAsNumber();

			int ItemOption3 = lpMemScript->GetAsNumber();

			int ItemNewOption = lpMemScript->GetAsNumber();

			int AncOption = lpMemScript->GetAsNumber();

			int ItemOptionEx = lpMemScript->GetAsNumber();

			int PriceType = 0;
			int BuyValue = 0;
			int SellValue = 0;

			if (ItemOptionEx > 0)
			{
				ItemOptionEx = 128;
			}

			this->InsertItem(ItemIndex,ItemLevel,ItemDurability,ItemOption1,ItemOption2,ItemOption3,ItemNewOption,AncOption,ItemOptionEx,0);
		}
	}
	catch(...)
	{
		ErrorMessageBox(lpMemScript->GetLastError());
	}

	delete lpMemScript;
}

void CShop::LoadDonateShop(char* path, int ShopIndex)
{
	pugi::xml_document file;
	pugi::xml_parse_result res = file.load_file(path);

	if(res.status != pugi::status_ok)
	{
		ErrorMessageBox("%s file load fail (%s)", path, res.description());
	}

	this->ShopIndex = ShopIndex;
	
	pugi::xml_node Shop = file.child("Shop");
	for (pugi::xml_node item = Shop.child("Item"); item; item = item.next_sibling())
	{
		DONATE_SHOP_ITEMS info;

		info.itemIndex = SafeGetItem(GET_ITEM(item.attribute("Type").as_int(),item.attribute("Index").as_int()));

		info.itemLevel = item.attribute("Level").as_int();

		info.itemDurability = item.attribute("Dur").as_int();

		info.itemOption1 = item.attribute("Skill").as_int();

		info.itemOption2 = item.attribute("Luck").as_int();

		info.itemOption3 = item.attribute("Option").as_int();

		info.itemNewOption = item.attribute("Exc").as_int();

		info.itemAncient = item.attribute("Anc").as_int();

		info.itemOptionEx = item.attribute("OptionEx").as_int();

		info.itemSocketCount = item.attribute("SocketCount").as_int();

		info.ItemPriceType = item.attribute("PriceType").as_int();
		info.itemBuyValue = item.attribute("BuyValue").as_int();
		info.itemSellValue = item.attribute("SellValue").as_int();
		
		/*int ItemIndex = SafeGetItem(GET_ITEM(item.attribute("Type").as_int(),item.attribute("Index").as_int()));

		int ItemLevel = item.attribute("Level").as_int();

		int ItemDurability = item.attribute("Dur").as_int();

		int ItemOption1 = item.attribute("Skill").as_int();

		int ItemOption2 = item.attribute("Luck").as_int();

		int ItemOption3 = item.attribute("Option").as_int();

		int ItemNewOption = item.attribute("Exc").as_int();

		int AncOption = item.attribute("Anc").as_int();

		int ItemOptionEx = item.attribute("OptionEx").as_int();

		int PriceType = item.attribute("PriceType").as_int();
		int BuyValue = item.attribute("BuyValue").as_int();
		int SellValue = item.attribute("SellValue").as_int();*/

		if (info.itemOptionEx > 0)
		{
			info.itemOptionEx = 128;
		}

		this->InsertDonateItem(&info);
		//this->InsertDonateItem(ItemIndex,ItemLevel,ItemDurability,ItemOption1,ItemOption2,ItemOption3,ItemNewOption,AncOption,ItemOptionEx,PriceType,BuyValue,SellValue);
	}
}


void CShop::ShopItemSet(int slot,BYTE type) // OK
{
	if(SHOP_INVENTORY_RANGE(slot) == 0)
	{
		return;
	}

	ITEM_INFO ItemInfo;

	if(gItemManager.GetInfo(this->m_Item[slot].m_Index,&ItemInfo) == 0)
	{
		return;
	}

	int x = slot%8;
	int y = slot/8;

	if((x+ItemInfo.Width) > 8 || (y+ItemInfo.Height) > 15)
	{
		return;
	}

	for(int sy=0;sy < ItemInfo.Height;sy++)
	{
		for(int sx=0;sx < ItemInfo.Width;sx++)
		{
			this->m_InventoryMap[(((sy+y)*8)+(sx+x))] = type;
		}
	}
}

BYTE CShop::ShopRectCheck(int x,int y,int width,int height) // OK
{
	if((x+width) > 8 || (y+height) > 15)
	{
		return 0xFF;
	}

	for(int sy=0;sy < height;sy++)
	{
		for(int sx=0;sx < width;sx++)
		{
			if(this->m_InventoryMap[(((sy+y)*8)+(sx+x))] != 0xFF)
			{
				return 0xFF;
			}
		}
	}

	return ((y*8)+x);
}

void CShop::InsertItem(int ItemIndex,int ItemLevel,int ItemDurability,int ItemOption1,int ItemOption2,int ItemOption3,int ItemNewOption,int AncOption,int ItemOptionEx,int ItemValue) // OK
{
	ITEM_INFO ItemInfo;

	if(gItemManager.GetInfo(ItemIndex,&ItemInfo) == 0)
	{
		return;
	}

	for(int y=0;y < 15;y++)
	{
		for(int x=0;x < 8;x++)
		{
			if(this->m_InventoryMap[((y*8)+x)] == 0xFF)
			{
				BYTE slot = this->ShopRectCheck(x,y,ItemInfo.Width,ItemInfo.Height);

				if(slot != 0xFF)
				{
					this->m_Item[slot].m_Level = ItemLevel;
					this->m_Item[slot].m_Durability = (float)((ItemDurability==0)?gItemManager.GetItemDurability(ItemIndex,ItemLevel,ItemNewOption,AncOption):ItemDurability);
					this->m_Item[slot].Convert(ItemIndex,ItemOption1,ItemOption2,ItemOption3,ItemNewOption,AncOption,0,ItemOptionEx,0,0xFF);
					this->m_Item[slot].m_PcPointValue = ItemValue;
					this->ShopItemSet(slot,1);
					return;
				}
			}
		}
	}
}

void CShop::InsertDonateItem(DONATE_SHOP_ITEMS* lpInfo/*int ItemIndex,int ItemLevel,int ItemDurability,int ItemOption1,int ItemOption2,int ItemOption3,int ItemNewOption,int AncOption,int ItemOptionEx,int PriceType,int BuyValue,int SellValue*/) // OK
{
	ITEM_INFO ItemInfo;

	if(gItemManager.GetInfo(lpInfo->itemIndex,&ItemInfo) == 0)
	{
		return;
	}
	
	for(int y=0;y < 15;y++)
	{
		for(int x=0;x < 8;x++)
		{
			if(this->m_InventoryMap[((y*8)+x)] == 0xFF)
			{
				BYTE slot = this->ShopRectCheck(x,y,ItemInfo.Width,ItemInfo.Height);

				if(slot != 0xFF)
				{
					this->m_Item[slot].m_Level = lpInfo->itemLevel;
					this->m_Item[slot].m_Durability = (float)((lpInfo->itemDurability==0)?gItemManager.GetItemDurability(lpInfo->itemIndex,lpInfo->itemLevel,lpInfo->itemNewOption,lpInfo->itemAncient):lpInfo->itemDurability);

					BYTE SocketOption[5] = {0xFF,0xFF,0xFF,0xFF,0xFF};
					for( int i = 0; i < 5; i++ )
					{
						if( i < lpInfo->itemSocketCount )
						{
							SocketOption[i]=0xFE;
						}
					}		

					this->m_Item[slot].Convert(lpInfo->itemIndex,lpInfo->itemOption1,lpInfo->itemOption2,lpInfo->itemOption3,lpInfo->itemNewOption,lpInfo->itemAncient,0,lpInfo->itemOptionEx,SocketOption,0xFF);
					//this->m_Item[slot].m_PcPointValue = ItemValue;
					this->m_Item[slot].m_PriceType = lpInfo->ItemPriceType;


					if (lpInfo->ItemPriceType == CREDITS)
					{
						this->m_Item[slot].m_BuyCredits = lpInfo->itemBuyValue;
						this->m_Item[slot].m_SellCredits = lpInfo->itemSellValue;
					}
					else if (lpInfo->ItemPriceType == WCOINC)
					{
						this->m_Item[slot].m_BuyWCoinC = lpInfo->itemBuyValue;
						this->m_Item[slot].m_SellWCoinC = lpInfo->itemSellValue;
					}
					else if (lpInfo->ItemPriceType == WCOINP)
					{
						this->m_Item[slot].m_BuyWCoinP = lpInfo->itemBuyValue;
						this->m_Item[slot].m_SellWCoinP = lpInfo->itemSellValue;
					}
					else if (lpInfo->ItemPriceType == WCOING)
					{
						this->m_Item[slot].m_BuyWCoinG = lpInfo->itemBuyValue;
						this->m_Item[slot].m_SellWCoinG = lpInfo->itemSellValue;
					}
					else // money
					{
						this->m_Item[slot].m_BuyMoney = lpInfo->itemBuyValue;
						this->m_Item[slot].m_SellMoney = lpInfo->itemSellValue;
					}

					this->m_Item[slot].m_IsExpiredItem = true/*this->ShopIndex*/;
					this->ShopItemSet(slot,1);
					return;
				}
			}
		}
	}
}

bool CShop::GetItem(CItem* lpItem,int slot) // OK
{
	if(SHOP_INVENTORY_RANGE(slot) != 0)
	{
		if(this->m_Item[slot].IsItem() != 0)
		{
			(*lpItem) = this->m_Item[slot];
			return 1;
		}
	}

	return 0;
}

int CShop::GetItemPrice(CItem* lpItem, int* type)
{
	for(int n=0;n < SHOP_SIZE;n++)
	{
		if(this->m_Item[n].IsItem() != 0)
		{
			if (this->m_Item[n].m_Index == lpItem->m_Index && this->m_Item[n].m_Level == lpItem->m_Level && 
				this->m_Item[n].m_SkillOption == lpItem->m_SkillOption && this->m_Item[n].m_LuckOption == lpItem->m_LuckOption &&
				this->m_Item[n].m_NewOption == lpItem->m_NewOption && this->m_Item[n].m_SetOption == lpItem->m_SetOption &&
				this->m_Item[n].m_ItemOptionEx == lpItem->m_ItemOptionEx)
			{
				(*type) = this->m_Item[n].m_PriceType;

				switch (this->m_Item[n].m_PriceType)
				{
					case CREDITS: return this->m_Item[n].m_SellCredits;
					case WCOINC: return this->m_Item[n].m_SellWCoinC;
					case WCOINP: return this->m_Item[n].m_SellWCoinP;
					case WCOING: return this->m_Item[n].m_SellWCoinG;
					default: return this->m_Item[n].m_SellMoney;
				}
			}
		}
	}

	return 0;
}


long CShop::GetItemCount() // OK
{
	int count = 0;

	for(int n=0;n < SHOP_SIZE;n++)
	{
		if(this->m_Item[n].IsItem() != 0)
		{
			count++;
		}
	}

	return count;
}

bool CShop::GCShopItemListSend(int aIndex) // OK
{
	BYTE send[2048];

	PMSG_SHOP_ITEM_LIST_SEND pMsg;

	pMsg.header.set(0x31,0);

	int size = sizeof(pMsg);

	pMsg.type = 0;

	pMsg.count = 0;

	PMSG_SHOP_ITEM_LIST info;

	for(int n=0;n < SHOP_SIZE;n++)
	{
		if(this->m_Item[n].IsItem() != 0)
		{
			info.slot = n;

			gItemManager.ItemByteConvert(info.ItemInfo,this->m_Item[n]);

			memcpy(&send[size],&info,sizeof(info));
			size += sizeof(info);

			pMsg.count++;
		}
	}

	pMsg.header.size[0] = SET_NUMBERHB(size);
	pMsg.header.size[1] = SET_NUMBERLB(size);

	memcpy(send,&pMsg,sizeof(pMsg));

	DataSend(aIndex,send,size);

	return 1;
}
