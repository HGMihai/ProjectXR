#include "stdafx.h"
#include "Pickup.h"
#include "MemScript.h"
#include "Message.h"
#include "Notice.h"
#include "Util.h"
#include "CommandManager.h"
#include "Map.h"

Pickup gPickup;

Pickup::Pickup()
{
	this->m_PickupStruct.clear();
	for (int i = 0; i<4; i++)
	{
		this->IsEnabled[i] = 0;
		this->PriceZen[i] = 0;
	}
}

Pickup::~Pickup()
{

}

void Pickup::Load(char* path)
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

	this->m_PickupStruct.clear();

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

			PICKUP info;

			info.ItemIndex	= lpMemScript->GetNumber();

			info.ItemID = lpMemScript->GetAsNumber();

			this->m_PickupStruct.push_back(info);
		}
	}
	catch(...)
	{
		ErrorMessageBox(lpMemScript->GetLastError());
	}

	delete lpMemScript;
}

void Pickup::ReadCustomPickInfo(char* section,char* path)
{
	this->IsEnabled[0] = GetPrivateProfileInt(section,"PickupEnabled_AL0",0,path);
	this->IsEnabled[1] = GetPrivateProfileInt(section,"PickupEnabled_AL1",0,path);
	this->IsEnabled[2] = GetPrivateProfileInt(section,"PickupEnabled_AL2",0,path);
	this->IsEnabled[3] = GetPrivateProfileInt(section,"PickupEnabled_AL3",0,path);

	this->PriceZen[0] = GetPrivateProfileInt(section,"ZenPrice_AL0",0,path);
	this->PriceZen[1] = GetPrivateProfileInt(section,"ZenPrice_AL1",0,path);
	this->PriceZen[2] = GetPrivateProfileInt(section,"ZenPrice_AL2",0,path);
	this->PriceZen[3] = GetPrivateProfileInt(section,"ZenPrice_AL3",0,path);

	GetPrivateProfileString(section,"CommandArg1","",this->Command1,sizeof(this->Command1),path);
	GetPrivateProfileString(section,"CommandArg2","",this->Command2,sizeof(this->Command2),path);
	GetPrivateProfileString(section,"CommandArg3","",this->Command3,sizeof(this->Command3),path);
}

void Pickup::PickMsgProc(LPOBJ lpObj)
{
	if (this->IsEnabled[lpObj->AccountLevel] == 0)
	{
		return;
	}

	if (lpObj->PickType == PICKUP_NONE)
	{
		return;
	}

	for (int c=0; c<MAX_MAP_ITEM; c++)
	{
		if (gMap[lpObj->Map].m_Item[c].IsItem() == 0)
		{
			continue;
		}

		for (int i=0; i<6; i++)
		{
			for (int n=0; n<6; n++)
			{
				int X = (gMap[lpObj->Map].m_Item[c].m_X - 3) + i;
				int Y = (gMap[lpObj->Map].m_Item[c].m_Y - 3) + n;

				if (X == lpObj->X && Y == lpObj->Y)
				{
					for(std::vector<PICKUP>::iterator it=this->m_PickupStruct.begin();it != this->m_PickupStruct.end();it++)
					{
						PMSG_ITEM_GET_RECV pMsg;
						pMsg.index[0] = SET_NUMBERHB(c);
						pMsg.index[1] = SET_NUMBERLB(c);

						if (gMap[lpObj->Map].m_Item[c].m_Index == GET_ITEM(it->ItemIndex, it->ItemID))
						{
							if (lpObj->PickType & PICKUP_JEWELS)
							{
								gItemManager.CGItemGetRecv(&pMsg, lpObj->Index);
							}
						}
						else if (gMap[lpObj->Map].m_Item[c].IsExcItem())
						{
							if (lpObj->PickType & PICKUP_EXCELLENT)
							{
								gItemManager.CGItemGetRecv(&pMsg, lpObj->Index);
							}
						}
					}
				}				
			}
		}
	}

}

void Pickup::PickCommand(LPOBJ lpObj, char* arg)
{
	if (this->IsEnabled[lpObj->AccountLevel] == 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(494));
		return;
	}

	if (lpObj->Money < this->PriceZen[lpObj->AccountLevel])
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(498), this->PriceZen[lpObj->AccountLevel]);
		return;
	}

	char name[20] = {0};

	gCommandManager.GetString(arg,name,sizeof(name),0);
	if(strcmp(name,this->Command1) == 0)
	{
		if ((lpObj->PickType & PICKUP_EXCELLENT) == 0)
		{
			lpObj->PickType += PICKUP_EXCELLENT;
			gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,"You successfully activated pickup %s", this->Command1);
		}
		else
		{
			gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,"You already used this command!");
		}
	}

	if(strcmp(name,this->Command2) == 0)
	{
		if ((lpObj->PickType & PICKUP_JEWELS) == 0)
		{
			lpObj->PickType += PICKUP_JEWELS;
			gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,"You successfully activated pickup %s", this->Command2);
		}
		else
		{
			gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,"You already used this command!");
		}
	}
	if(strcmp(name,this->Command3) == 0)
	{
		lpObj->PickType = PICKUP_NONE;
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,"You have successfully disabled pickup");
	}

	lpObj->Money -= this->PriceZen[lpObj->AccountLevel];
	GCMoneySend(lpObj->Index, lpObj->Money);
}