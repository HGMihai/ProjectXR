#include "StdAfx.h"
#include "PersonalShopEx.h"
#include "user.h"
#include "GameMain.h"
#include "Path.h"
#include "Util.h"
// ----------------------------------------------------------------------------------------------

#if(CUSTOM_PERSONAL_SHOP==TRUE)

PersonalShopEx g_PersonalShopEx;
// ----------------------------------------------------------------------------------------------

void PersonalShopEx::Init()
{
	this->Enable = TRUE;
}
// ----------------------------------------------------------------------------------------------

void PersonalShopEx::Load()
{
	this->Init();

	this->Read(gPath.GetFullPath("..\\LTPData\\PersonalShopEx.ini"));
}
// ----------------------------------------------------------------------------------------------

void PersonalShopEx::Read(char* File)
{
	this->Enable = GetPrivateProfileInt("Common", "Enable", 0, File);
	//devias
	this->NPC_CLASS = GetPrivateProfileInt("Common", "NPC_CLASS", 0, File);
	this->NPC_MAP = GetPrivateProfileInt("Common", "NPC_MAP", 0, File);
	this->NPC_X = GetPrivateProfileInt("Common", "NPC_X", 0, File);
	this->NPC_Y = GetPrivateProfileInt("Common", "NPC_Y", 0, File);
	/*this->NPC_MAP2 = GetPrivateProfileInt("Common", "NPC_MAP2", 0, File);
	this->NPC_X2 = GetPrivateProfileInt("Common", "NPC_X2", 0, File);
	this->NPC_Y2 = GetPrivateProfileInt("Common", "NPC_Y2", 0, File);
	this->NPC_MAP3 = GetPrivateProfileInt("Common", "NPC_MAP3", 0, File);
	this->NPC_X3 = GetPrivateProfileInt("Common", "NPC_X3", 0, File);
	this->NPC_Y3 = GetPrivateProfileInt("Common", "NPC_Y3", 0, File);
	this->NPC_MAP4 = GetPrivateProfileInt("Common", "NPC_MAP4", 0, File);
	this->NPC_X4 = GetPrivateProfileInt("Common", "NPC_X4", 0, File);
	this->NPC_Y4 = GetPrivateProfileInt("Common", "NPC_Y4", 0, File);*/

	this->BonusPrice = GetPrivateProfileInt("Common", "BonusPrice", 0, File);
}
// ----------------------------------------------------------------------------------------------

bool PersonalShopEx::NPC_Dialog(int aIndex, int aIndexNPC)
{
	if(!this->Enable)
	{
 		return false;
	}

	if(!gObjIsConnectedGP(aIndex))
	{
		return false;
	}

	LPOBJ lpUser = &gObj[aIndex];
	LPOBJ lpNpc = &gObj[aIndexNPC];

	if(lpNpc->Class	== this->NPC_CLASS && lpNpc->Map == this->NPC_MAP && lpNpc->X == this->NPC_X && lpNpc->Y == this->NPC_Y)
	{
		CG_PersonalPage aCG;
		aCG.Page = 0;
		this->Search(aIndex,&aCG);
		return true;
	}
	// ----
	return false;
}
// ----------------------------------------------------------------------------------------------

void PersonalShopEx::Search(int aIndex, CG_PersonalPage* aRecv)
{
	if(!this->Enable)
	{
		return;
	}

	if(!gObjIsConnectedGP(aIndex))
	{
		return;
	}

	LPOBJ lpUser = &gObj[aIndex];

	int Page = aRecv->Page;

	if(Page < 0 )
	{
		Page = 0;
	}

	if(lpUser->AttackCustomOffline != 0 || lpUser->PShopCustomOffline !=0)
	{
		return;
	}

	BYTE SendBuff[5000];
	int datasize = 8;
	short icount = 0;
	BYTE SendCount = 0;

	int StartCount = Page * 10;
	int EndCount =  StartCount + 10;

	for(int n=OBJ_STARTUSERINDEX;n<MAX_OBJECT;n++)
	{
		LPOBJ lpSearch = &gObj[n];
		if(lpSearch->Connected < OBJECT_PLAYING || !lpSearch->PShopOpen || n == aIndex) 
		{
			continue;
		}

		if( StartCount <= icount && EndCount > icount)
		{
			if(SendCount < 10)
			{
				GC_PersonalShopInfo psi;
				psi.aIndex = n;
				strncpy(psi.Name, lpSearch->Name, 10);
				strncpy(psi.PersonalText, lpSearch->PShopText, 36);
				psi.Value = 0;

				memcpy(&SendBuff[datasize],&psi,sizeof(psi));
				datasize += sizeof(psi);

				SendCount++;
			}
		}

		icount++;
	}

	SendBuff[0]=0xC2;	// packet header
	SendBuff[1]=SET_NUMBERHB(datasize);
	SendBuff[2]=SET_NUMBERLB(datasize);
	SendBuff[3]=0xFA;
	SendBuff[4]=0x09;
	SendBuff[5]=SET_NUMBERHB(icount);
	SendBuff[6]=SET_NUMBERLB(icount);
	SendBuff[7]=SendCount;
	DataSend(aIndex,SendBuff,datasize);
}

void PersonalShopEx::UpdatePSInventoryUser(int aIndex)
{
	//if(!this->Enable)
	//{
	//	return;
	//}

	if(!this->Enable)
	{
 		return;
	}

	if(!gObjIsConnectedGP(aIndex))
	{
		return;
	}

	LPOBJ lpUser = &gObj[aIndex];

	int datasize = 6;
	BYTE icount = 0;
	BYTE SendBuff[5000];

	for ( int n=INVENTORY_EXT4_SIZE;n<INVENTORY_SIZE;n++)
	{
		if ( lpUser->Inventory1[n].IsItem() == TRUE )
		{
			GC_PSInventoryValue pMsg;

			pMsg.ItemPos = n;
			//ItemByteConvert(pMsgIL.ItemInfo, lpObjTarget->Inventory1[n]);
			//pMsgIL.PShopItemValue = lpObjTarget->Inventory1[n].m_iPShopValue;
			pMsg.Zen = lpUser->Inventory1[n].m_PShopValue;
			pMsg.Bonus = lpUser->Inventory1[n].m_iPShopBonus;
			pMsg.Bless = lpUser->Inventory1[n].m_PShopBlessValue;
			pMsg.Soul = lpUser->Inventory1[n].m_PShopSoulValue;
			pMsg.Chaos = lpUser->Inventory1[n].m_PShopChaosValue;
			pMsg.Life = lpUser->Inventory1[n].m_PShopLifeValue;

			memcpy(&SendBuff[datasize], &pMsg, sizeof(pMsg));
			
			datasize += sizeof(pMsg);

			icount++;
		}
	}

	SendBuff[0]=0xC2;	// packet header
	SendBuff[1]=SET_NUMBERHB(datasize);
	SendBuff[2]=SET_NUMBERLB(datasize);
	SendBuff[3]=0xFA;
	SendBuff[4]=0x0A;
	SendBuff[5]=icount;
	DataSend(aIndex,SendBuff,datasize);
}

void PersonalShopEx::UpdatePSInventoryTarget(int aIndex, int aTargetIndex)
{
	if(!this->Enable)
	{
 		return;
	}

	if(!gObjIsConnectedGP(aIndex))
	{
		return;
	}

	if(!gObjIsConnectedGP(aTargetIndex))
	{
		return;
	}

	LPOBJ lpUser = &gObj[aIndex];
	LPOBJ lpTarget = &gObj[aTargetIndex];

	int datasize = 8;
	BYTE icount = 0;
	BYTE SendBuff[5000];

	for ( int n=INVENTORY_EXT4_SIZE;n<INVENTORY_SIZE;n++)
	{
		if ( lpTarget->Inventory1[n].IsItem() == TRUE )
		{
			GC_PSInventoryValue pMsg;

			pMsg.ItemPos = n;
			//ItemByteConvert(pMsgIL.ItemInfo, lpObjTarget->Inventory1[n]);
			//pMsgIL.PShopItemValue = lpObjTarget->Inventory1[n].m_iPShopValue;
			pMsg.Zen = lpTarget->Inventory1[n].m_PShopValue;
			pMsg.Bonus = lpTarget->Inventory1[n].m_iPShopBonus;
			pMsg.Bless = lpTarget->Inventory1[n].m_PShopBlessValue;
			pMsg.Soul = lpTarget->Inventory1[n].m_PShopSoulValue;
			pMsg.Chaos = lpTarget->Inventory1[n].m_PShopChaosValue;
			pMsg.Life = lpTarget->Inventory1[n].m_PShopLifeValue;

			LogAdd(LOG_TEST,"index = %d zen_value = %d",n,lpTarget->Inventory1[n].m_PShopValue);

			memcpy(&SendBuff[datasize], &pMsg, sizeof(pMsg));
			
			datasize += sizeof(pMsg);

			icount++;
		}
	}

	SendBuff[0]=0xC2;	// packet header
	SendBuff[1]=SET_NUMBERHB(datasize);
	SendBuff[2]=SET_NUMBERLB(datasize);
	SendBuff[3]=0xFA;
	SendBuff[4]=0x0B;
	SendBuff[5]=SET_NUMBERHB(aTargetIndex);
	SendBuff[6]=SET_NUMBERLB(aTargetIndex);
	SendBuff[7]=icount;
	DataSend(aIndex,SendBuff,datasize);
}

int PersonalShopEx::GetMoney(int aIndex, int Pos)
{

	if(!gObjIsConnectedGP(aIndex))
	{
		return 0;
	}

	LPOBJ lpUser = &gObj[aIndex];

	int Zen = lpUser->Inventory1[Pos].m_PShopValue;
	int Bonus = lpUser->Inventory1[Pos].m_iPShopBonus;
	int Bless = lpUser->Inventory1[Pos].m_PShopBlessValue;
	int Soul = lpUser->Inventory1[Pos].m_PShopSoulValue;
	int Chaos = lpUser->Inventory1[Pos].m_PShopChaosValue;
	int Life = lpUser->Inventory1[Pos].m_PShopLifeValue;

	bool Result = 0;

	if( Zen > 0 )
	{
		Result = 1;
	}
	if( Bless > 0)
	{
		Result = 1;
	}
	if( Soul > 0)
	{
		Result = 1;
	}
	if( Chaos > 0)
	{
		Result = 1;
	}
	if( Life > 0)
	{
		Result = 1;
	}

	if(!this->BonusPrice)
	{
		if(Bonus > 0)
		{
			Result = 0;
		}
	}
	else
	{
		if(Bonus > 0)
		{
			Result = 1;
		}
	}

	return Result;
}

bool PersonalShopEx::CheckPrice(int Zen, int Bonus, int Bless, int Soul, int Chaos, int Life)
{
	bool Result = 0;

	if( Zen > 0 )
	{
		Result = 1;
	}
	if( Bless > 0)
	{
		Result = 1;
	}
	if( Soul > 0)
	{
		Result = 1;
	}
	if( Chaos > 0)
	{
		Result = 1;
	}
	if( Life > 0)
	{
		Result = 1;
	}

	if(!this->BonusPrice)
	{
		if(Bonus > 0)
		{
			Result = 0;
		}
	}
	else
	{
		if(Bonus > 0)
		{
			Result = 1;
		}
	}

	return Result;
}

bool PersonalShopEx::PShop_CheckInventoryPrice(short aIndex)
{
	if(!gObjIsConnectedGP(aIndex))
	{
		return false;
	}

	LPOBJ lpObj = &gObj[aIndex];

	bool Result = false;

	for(int i = INVENTORY_EXT4_SIZE; i < INVENTORY_SIZE; i++)
	{
		if(lpObj->Inventory1[i].IsItem() == 1)
		{
			Result = false;

			if(lpObj->Inventory1[i].m_PShopValue > 0 )
			{
				Result = true;
			}
			if(lpObj->Inventory1[i].m_PShopBlessValue > 0)
			{
				Result = true;
			}
			if(lpObj->Inventory1[i].m_PShopSoulValue > 0)
			{
				Result = true;
			}
			if(lpObj->Inventory1[i].m_PShopChaosValue > 0)
			{
				Result = true;
			}
			if(lpObj->Inventory1[i].m_PShopLifeValue > 0)
			{
				Result = true;
			}

			if(!this->BonusPrice)
			{
				if(lpObj->Inventory1[i].m_iPShopBonus > 0)
				{
					Result = false;
				}
			}
			else
			{
				if(lpObj->Inventory1[i].m_iPShopBonus > 0)
				{
					Result = true;
				}
			}

			if(Result == false)
			{
				break;
			}
		}
	}
	return Result;
}

#endif