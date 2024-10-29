#include "StdAfx.h"
#include "Smithy.h"
#include "GameMain.h"
//#include "..\\include\\ReadScript.h"
//#include "logproc.h"
#include "DSProtocol.h"
#include "Log.h"
#include "Notice.h"
//#include "..\common\SetItemOption.h"
//#include "ExUser.h"
//#include "ExLicense.h"
//#include "LogToFile.h"
#include "MemScript.h"
#include "Util.h"
#include "ItemManager.h"
#include "SetItemType.h"
#include "BankEx.h"
#include "SocketItemType.h"
#include "Path.h"
#include "Protection.h"

//CLogToFile g_SmithLog("Smith", ".\\LOG\\Smith", TRUE);

CSmithy gSmithy;

CSmithy::CSmithy()
{
	this->Init();
}

CSmithy::~CSmithy()
{
}

void CSmithy::Init()
{
	this->itemdata.clear();
	this->AllowAncExce = 0;
	this->MaxLevel = 0;
	this->LevelPrice = 0;
	this->LuckPrice = 0;
	this->AddPrice = 0;
	this->Opt1Price = 0;
	this->Opt2Price = 0;
	this->Opt3Price = 0;
	this->Opt4Price = 0;
	this->Opt5Price = 0;
	this->Opt6Price = 0;
	this->AncientPrice = 0;

	this->time_forever_price = 0;
	this->time_1day_price = 0;
	this->time_3days_price = 0;
	this->time_7days_price = 0;
	this->time_30days_price = 0;
	this->time_90days_price = 0;
}

void CSmithy::Load()
{
	if (!gProtection.GetCustomState(CUSTOM_SMITHY))
	{
		return;
	}
	this->Init();
	this->Read(gPath.GetFullPath("..\\LTPData\\Smithy.txt"));
}

void CSmithy::Read(char* path)
{
	CMemScript* lpMemScript = new CMemScript;

	if(lpMemScript == nullptr)
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

	try
	{
		while(true)
		{
			if(lpMemScript->GetToken() == TOKEN_END)
			{
				break;
			}
		
			int section = lpMemScript->GetNumber();

			while(true)
			{
				if(section == 8)
				{
					if(strcmp("end",lpMemScript->GetAsString()) == 0)
					{
						break;
					}

					this->time_forever_price = lpMemScript->GetNumber();
					this->time_1day_price = lpMemScript->GetAsNumber();
					this->time_3days_price = lpMemScript->GetAsNumber();
					this->time_7days_price = lpMemScript->GetAsNumber();
					this->time_30days_price = lpMemScript->GetAsNumber();
					this->time_90days_price = lpMemScript->GetAsNumber();
				}
				else if(section == 0)
				{
					if(strcmp("end",lpMemScript->GetAsString()) == 0)
					{
						break;
					}

					this->AllowAncExce = lpMemScript->GetNumber();
					this->MaxLevel = lpMemScript->GetAsNumber();
					this->MaxExcOptions = lpMemScript->GetAsNumber();
					this->LevelPrice = lpMemScript->GetAsNumber();
					this->LuckPrice = lpMemScript->GetAsNumber();
					this->AddPrice = lpMemScript->GetAsNumber();
					this->Opt1Price = lpMemScript->GetAsNumber();
					this->Opt2Price = lpMemScript->GetAsNumber();
					this->Opt3Price = lpMemScript->GetAsNumber();
					this->Opt4Price = lpMemScript->GetAsNumber();
					this->Opt5Price = lpMemScript->GetAsNumber();
					this->Opt6Price = lpMemScript->GetAsNumber();
					this->AncientPrice = lpMemScript->GetAsNumber();
					this->SocketPrice = lpMemScript->GetAsNumber();
				}
				else if (section == 1)
				{
					if(strcmp("end",lpMemScript->GetAsString()) == 0)
					{
						break;
					}

					SmithyDATA lpData;
					lpData.Type = lpMemScript->GetNumber();
					lpData.Index = lpMemScript->GetAsNumber();
					lpData.Class = CLASS_DW;
					lpData.Price = lpMemScript->GetAsNumber();
					this->itemdata.push_back(lpData);
				}
				else if (section == 2)
				{
					if(strcmp("end",lpMemScript->GetAsString()) == 0)
					{
						break;
					}

					SmithyDATA lpData;
					lpData.Type = lpMemScript->GetNumber();
					lpData.Index = lpMemScript->GetAsNumber();
					lpData.Class = CLASS_DK;
					lpData.Price = lpMemScript->GetAsNumber();
					this->itemdata.push_back(lpData);
				}
				else if (section == 3)
				{
					if(strcmp("end",lpMemScript->GetAsString()) == 0)
					{
						break;
					}

					SmithyDATA lpData;
					lpData.Type = lpMemScript->GetNumber();
					lpData.Index = lpMemScript->GetAsNumber();
					lpData.Class = CLASS_FE;
					lpData.Price = lpMemScript->GetAsNumber();
					this->itemdata.push_back(lpData);
				}
				else if (section == 4)
				{
					if(strcmp("end",lpMemScript->GetAsString()) == 0)
					{
						break;
					}

					SmithyDATA lpData;
					lpData.Type = lpMemScript->GetNumber();
					lpData.Index = lpMemScript->GetAsNumber();
					lpData.Class = CLASS_MG;
					lpData.Price = lpMemScript->GetAsNumber();
					this->itemdata.push_back(lpData);
				}
				else if (section == 5)
				{
					if(strcmp("end",lpMemScript->GetAsString()) == 0)
					{
						break;
					}

					SmithyDATA lpData;
					lpData.Type = lpMemScript->GetNumber();
					lpData.Index = lpMemScript->GetAsNumber();
					lpData.Class = CLASS_DL;
					lpData.Price = lpMemScript->GetAsNumber();
					this->itemdata.push_back(lpData);
				}
				else if (section == 6)
				{
					if(strcmp("end",lpMemScript->GetAsString()) == 0)
					{
						break;
					}

					SmithyDATA lpData;
					lpData.Type = lpMemScript->GetNumber();
					lpData.Index = lpMemScript->GetAsNumber();
					lpData.Class = CLASS_SU;
					lpData.Price = lpMemScript->GetAsNumber();
					this->itemdata.push_back(lpData);
				}
				else if (section == 7)
				{
					if(strcmp("end",lpMemScript->GetAsString()) == 0)
					{
						break;
					}

					SmithyDATA lpData;
					lpData.Type = lpMemScript->GetNumber();
					lpData.Index = lpMemScript->GetAsNumber();
					lpData.Class = CLASS_RF;
					lpData.Price = lpMemScript->GetAsNumber();
					this->itemdata.push_back(lpData);
				}
				else
				{
					break;
				}
			}
		}
	}
	catch(...)
	{
		ErrorMessageBox(lpMemScript->GetLastError());
	}

	delete lpMemScript;
}

void CSmithy::GCInitItem(int aIndex)
{
	if(gObjIsConnected(aIndex) == 0)
	{
		return;
	}

	GC_SmithyInit pMsg;
	//pMsg.h.set((LPBYTE)&pMsg, 0xFB, 0x1E,  sizeof(pMsg));
	pMsg.h.set(0xFB, 0x15,  sizeof(pMsg));
	pMsg.result = true;
	DataSend(aIndex,reinterpret_cast<LPBYTE>(&pMsg),sizeof(pMsg));
}

void CSmithy::GCItemSend(int aIndex)
{
	if (!gProtection.GetCustomState(CUSTOM_SMITHY))
	{
		return;
	}

	if(gObjIsConnected(aIndex) == 0)
	{
		return;
	}

	this->GCSettingsSend(aIndex);
	this->GCSendAncientList(aIndex);
	this->GCSendSocketList(aIndex);
	this->GCInitItem(aIndex);
	for(int i = 0; i < itemdata.size(); i++)
	{
		GC_SmithyDATA pMsg;
		pMsg.h.set(0xFB, 0x16,  sizeof(pMsg));
		pMsg.Class = itemdata[i].Class;
		pMsg.Type = itemdata[i].Type;
		pMsg.Index = itemdata[i].Index;
		pMsg.Price = itemdata[i].Price;
		DataSend(aIndex,reinterpret_cast<LPBYTE>(&pMsg),sizeof(pMsg));
	}
}

void CSmithy::GCSettingsSend(int aIndex)
{
	if(gObjIsConnected(aIndex) == 0)
	{
		return;
	}

	GC_SmithySettings pMsg;
	pMsg.h.set(0xFB, 0x14,  sizeof(pMsg));
	pMsg.AllowAncExce = this->AllowAncExce;
	pMsg.MaxExcOptions = this->MaxExcOptions;
	pMsg.MaxLevel = this->MaxLevel;
	pMsg.LevelPrice = this->LevelPrice;
	pMsg.LuckPrice = this->LuckPrice;
	pMsg.AddPrice = this->AddPrice;
	pMsg.Opt1Price = this->Opt1Price;
	pMsg.Opt2Price = this->Opt2Price;
	pMsg.Opt3Price = this->Opt3Price;
	pMsg.Opt4Price = this->Opt4Price;
	pMsg.Opt5Price = this->Opt5Price;
	pMsg.Opt6Price = this->Opt6Price;
	pMsg.AncientPrice = this->AncientPrice;
	pMsg.SocketPrice = this->SocketPrice;
	
	pMsg.time_forever_price = this->time_forever_price;
	pMsg.time_1day_price = this->time_1day_price;
	pMsg.time_3days_price = this->time_3days_price;
	pMsg.time_7days_price = this->time_7days_price;
	pMsg.time_30days_price = this->time_30days_price;
	pMsg.time_90days_price = this->time_90days_price;
	DataSend(aIndex,reinterpret_cast<LPBYTE>(&pMsg),sizeof(pMsg));
}

void CSmithy::GCSendAncientList(int aIndex)
{
	BYTE send[4096];

	PMSG_SMITHY_ANCIENT_SEND pMsg;

	pMsg.header.set(0xF3,0xF3,0);

	int size = sizeof(pMsg);

	pMsg.count = 0;

	PMSG_SMITHY_ANCIENT info;

	for	(int i=0;i<gSetItemType.GetCountOfItems();i++)
	{
		SET_ITEM_TYPE_INFO* lpInfo = gSetItemType.GetInfoByIndex(i);
		if (lpInfo == nullptr)
		{
			continue;
		}

		info.index = lpInfo->Index;

		if (lpInfo->OptionIndex[0] != 0)
		{
			info.options[0] = 5;
		}
		else
		{
			info.options[0] = 0;
		}
		if (lpInfo->OptionIndex[1] != 0)
		{
			info.options[1] = 6;
		}
		else
		{
			info.options[1] = 0;
		}

		memcpy(&send[size],&info,sizeof(info));
		size += sizeof(info);

		pMsg.count++;
	}

	pMsg.header.size[0] = SET_NUMBERHB(size);

	pMsg.header.size[1] = SET_NUMBERLB(size);

	memcpy(send,&pMsg,sizeof(pMsg));
	DataSend(aIndex,send,size);
}

void CSmithy::GCSendSocketList(int aIndex)
{
	BYTE send[4096];

	PMSG_SMITHY_SOCKET_SEND pMsg;

	pMsg.header.set(0xF3,0xF4,0);

	int size = sizeof(pMsg);

	pMsg.count = 0;

	PMSG_SMITHY_SOCKET info;

	for(std::map<int,SOCKET_ITEM_TYPE_INFO>::iterator it = gSocketItemType.m_SocketItemTypeInfo.begin(); it != gSocketItemType.m_SocketItemTypeInfo.end();++it)
	{
		info.index = it->second.Index;

		memcpy(&send[size],&info,sizeof(info));
		size += sizeof(info);

		pMsg.count++;
	}

	pMsg.header.size[0] = SET_NUMBERHB(size);

	pMsg.header.size[1] = SET_NUMBERLB(size);

	memcpy(send,&pMsg,sizeof(pMsg));
	DataSend(aIndex,send,size);
}



void CSmithy::CGCreateItem(CG_SmithyCreateItem* aRecv, int aIndex)
{
	if (!gProtection.GetCustomState(CUSTOM_SMITHY))
	{
		return;
	}

	if(gObjIsConnected(aIndex) == 0)
	{
		return;
	}

	LPOBJ lpUser = &gObj[aIndex];

	if(lpUser->Connected < OBJECT_ONLINE)
	{
		return;
	}

	unsigned short id = aRecv->Item;

	if(id > itemdata.size())
	{
		return;
	}

	SmithyDATA smItem =  this->itemdata[id];

	short iType = GET_ITEM(smItem.Type,smItem.Index);

	double result = smItem.Price;
	result += aRecv->ItemLevel * this->LevelPrice;
	result += aRecv->ItemAdd * this->AddPrice;
	BYTE NewOption = 0;
	int SetOption = 0;

	DWORD duration = 0;
	

	if(aRecv->ItemLuck)
	{
		result += this->LuckPrice;
	}
	if(aRecv->ItemOpt1)
	{
		result += this->Opt1Price;
		 NewOption |= 0x20;
	}
	if(aRecv->ItemOpt2)
	{
		result += this->Opt2Price;
		NewOption |= 0x10;
	}
	if(aRecv->ItemOpt3)
	{
		result += this->Opt3Price;
		NewOption |= 0x08;
	}
	if(aRecv->ItemOpt4)
	{
		result += this->Opt4Price;
		NewOption |= 0x04;
	}
	if(aRecv->ItemOpt5)
	{
		result += this->Opt5Price;
		NewOption |= 0x02;
	}
	if(aRecv->ItemOpt6)
	{
		result += this->Opt6Price;
		NewOption |= 0x01;
	}
	if(aRecv->Ancient)
	{
		result += this->AncientPrice;
		//SetOption = gSetItemType.GetSetOption(iType);
		
		SetOption = aRecv->Ancient;
		/*if (aRecv->Ancient == 1)
		{
			SetOption = 5;
		}
		else if (aRecv->Ancient == 2)
		{
			SetOption = 6;
		}*/
	}

	if (aRecv->ItemSocket)
	{
		result += this->SocketPrice;
	}

	if (aRecv->ItemTime != 0)
	{
		time_t t = time(nullptr);
        localtime(&t);
		
		if (aRecv->ItemTime == smithy_time_values::smithy_forever)
		{
			result = result / 100 * this->time_forever_price;
			duration = 0;
		}
		else if (aRecv->ItemTime == smithy_time_values::smithy_1day)
		{
			result = result / 100 * this->time_1day_price;
			duration = static_cast<DWORD>(t) + 1 * 86400;
		}
		else if (aRecv->ItemTime == smithy_time_values::smithy_3day)
		{
			result = result / 100 * this->time_3days_price;
			duration = static_cast<DWORD>(t) + 3 * 86400;
		}
		else if (aRecv->ItemTime == smithy_time_values::smithy_7day)
		{
			result = result / 100 * this->time_7days_price;
			duration = static_cast<DWORD>(t) + 7 * 86400;
		}
		else if (aRecv->ItemTime == smithy_time_values::smithy_30day)
		{
			result = result / 100 * this->time_30days_price;
			duration = static_cast<DWORD>(t) + 30 * 86400;
		}
		else if (aRecv->ItemTime == smithy_time_values::smithy_90day)
		{
			result = result / 100 * this->time_90days_price;
			duration = static_cast<DWORD>(t) + 90 * 86400;
		}
	}

	ITEM_INFO ItemInfo;

	if(gItemManager.GetInfo(iType,&ItemInfo) == 0)
	{
		LogAdd(LOG_RED, "[Donate Shop] Error [0x1] Contact support! Item Don't Exist [%d]",iType);
		return;
	}
					
	if(gItemManager.CheckItemInventorySpace(lpUser,ItemInfo.Height,ItemInfo.Width) == false)
	{
		gNotice.GCNoticeSend(aIndex,1,0,0,0,0,0,"[Donate Shop]: Need more space in the inventory (%dx%d)", ItemInfo.Height,ItemInfo.Width);
		return;
	}

	const int total_price = static_cast<int>(std::ceil(result));

	if(total_price > lpUser->BankEx.Credits)
	{
		gNotice.GCNoticeSend(aIndex,1,0,0,0,0,0,"[Donate Shop]: Price: %d.You need more credits", total_price);
		return;
	}

	lpUser->BankEx.Credits -= total_price;
	gBankEx.GCUpdateBankEx(lpUser->Index);
	gBankEx.GDSavePoint(lpUser->Index);

	BYTE SocketBonus	= (BYTE)-1;
	BYTE SocketOption[5] = {0xFF,0xFF,0xFF,0xFF,0xFF};

	if( gSocketItemType.CheckSocketItemType(iType) )
	{
		int socket_count = gSocketItemType.GetSocketItemMaxSocket(iType);
		for( int i = 0; i < 5; i++ )
		{
			if( i < socket_count )
			{
				if (aRecv->ItemSocket == 1)
				{
					SocketOption[i]=0xFE;
				}
			}
		}		
	}
	//ItemSerialCreateSend(aIndex, 235, lpUser->X, lpUser->Y, iType, aRecv->ItemLevel, 0, 1, aRecv->ItemLuck, aRecv->ItemAdd,
	//		aIndex, NewOption, SetOption, SocketBonus, SocketOption);

	GDCreateItemSend(aIndex,235,lpUser->X,lpUser->Y,iType,aRecv->ItemLevel,0,1,aRecv->ItemLuck,aRecv->ItemAdd,aIndex,NewOption,SetOption,0,0,SocketOption,SocketBonus,duration);
	
	gLog.Output(LOG_SMITHY, "Item buy! Price:%d - Login: %s Name: %s, iType ID: %d,Level: %d,Dur: %d,Skill: %d, Luck: %d, Opt: %d, Exe: %d, Anc: %d, Socket: %d, Duration: %d seconds",
					total_price,lpUser->Account, lpUser->Name, iType, aRecv->ItemLevel, 0, 1, aRecv->ItemLuck, aRecv->ItemAdd, NewOption, SetOption,aRecv->ItemSocket,duration);


	//}
	gAchievements.BuyDonateShopItem(lpUser->Index, total_price);
	
	
	//MsgOutput(aIndex, "[Tokens Shop]: Item Created!");
	gNotice.GCNoticeSend(aIndex,1,0,0,0,0,0,"[Donate Shop]: Item Created!");
}