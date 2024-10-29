#include "stdafx.h"
#include "BotAlchemist.h"
#include "ObjectManager.h"
#include "Util.h"
#include "ItemManager.h"
#include "Monster.h"
#include "BankEx.h"
#include "Trade.h"
#include "DSProtocol.h"
#include "Notice.h"

CBotAlchemist gBotAlchemist;

void CBotAlchemist::Read(char * path)
{
	if(this->Enabled)
	{
		for(int botNum=0;botNum<MAX_BOTALCHEMIST;botNum++)
		{
			if(this->bot[botNum].Enabled)
			{
				int bIndex = this->bot[botNum].index;
				if(gObjIsConnected(bIndex))
					gObjDel(bIndex);
			}
		}
	}
	
	memset(&this->bot,0,sizeof(this->bot));
	
	this->Enabled = false;

	pugi::xml_document file;
	pugi::xml_parse_result res = file.load_file(path);

	if(res.status != pugi::status_ok)
	{
		ErrorMessageBox("%s file load fail (%s)", path, res.description());
	}

	this->Enabled = file.child("BotAlchemist").attribute("enable").as_bool();

	if(!this->Enabled)
		return;

	auto infos = file.child("BotAlchemist").child("Info");
	auto items = file.child("BotAlchemist").child("Items");
	auto settings = file.child("BotAlchemist").child("Settings");
	auto conditions = file.child("BotAlchemist").child("Conditions");
	
	for (auto info = infos.child("info"); info; info = info.next_sibling())
	{
		int bot_index = info.attribute("bot_index").as_int();

		if(bot_index < 0 || bot_index > MAX_BOTALCHEMIST-1)
		{
			MsgBox("BotAlchemist error: BotIndex:%d out of range!", bot_index);
			return;
		}

		this->bot[bot_index].Class = info.attribute("class").as_int();
		this->bot[bot_index].Rate = info.attribute("rate").as_int();
		this->bot[bot_index].Map = info.attribute("map").as_int();
		this->bot[bot_index].X = info.attribute("x").as_int();
		this->bot[bot_index].Y = info.attribute("y").as_int();
		this->bot[bot_index].Dir = info.attribute("dir").as_int();
		strncpy(this->bot[bot_index].Name,info.attribute("name").as_string(),sizeof(this->bot[bot_index].Name));

		this->bot[bot_index].Enabled = true;
	}

	for (auto item = items.child("item"); item; item = item.next_sibling())
	{
		int bot_index = item.attribute("bot_index").as_int();

		if(bot_index < 0 || bot_index > MAX_BOTALCHEMIST-1)
		{
			MsgBox("BotAlchemist error: BotIndex:%d out of range!", bot_index);
			return;
		}

		if(this->bot[bot_index].Enabled == false)
		{	
			MsgBox("BotAlchemist error: BotIndex:%d doesn't exist", bot_index);
			return;
		}

		int slot = item.attribute("slot").as_int();

		if(slot < 0 || slot > 8)
		{	
			MsgBox("BotAlchemist error: Min Slot 0 ; Max Slot 8");
			return;
		}

		int type = item.attribute("type").as_int();
		int index = item.attribute("index").as_int();

		this->bot[bot_index].body[slot].num = GET_ITEM(type,index);
		this->bot[bot_index].body[slot].level = item.attribute("level").as_int();
		this->bot[bot_index].body[slot].opt = item.attribute("opt").as_int();

		this->bot[bot_index].body[slot].Enabled = true;
	}

	for (auto setting = settings.child("setting"); setting; setting = setting.next_sibling())
	{
		int bot_index = setting.attribute("bot_index").as_int();

		if(bot_index < 0 || bot_index > MAX_BOTALCHEMIST-1)
		{
			MsgBox("BotAlchemist error: BotIndex:%d out of range!", bot_index);
			return;
		}

		if(this->bot[bot_index].Enabled == false)
		{	
			MsgBox("BotAlchemist error: BotIndex:%d doesn't exist", bot_index);
			return;
		}

		this->bot[bot_index].AllowLevel = setting.attribute("allow_level").as_int();
		this->bot[bot_index].AllowOpt = setting.attribute("allow_opt").as_int();
		this->bot[bot_index].AllowLuck = setting.attribute("allow_luck").as_int();
		this->bot[bot_index].AllowSkill = setting.attribute("allow_skill").as_int();
		this->bot[bot_index].AllowExc = setting.attribute("allow_excl").as_int();
	}	
	
	for (auto data = conditions.child("data"); data; data = data.next_sibling())
	{
		int bot_index = data.attribute("bot_index").as_int();

		if(bot_index < 0 || bot_index > MAX_BOTALCHEMIST-1)
		{
			MsgBox("BotAlchemist error: BotIndex:%d out of range!", bot_index);
			return;
		}

		if(this->bot[bot_index].Enabled == false)
		{	
			MsgBox("BotAlchemist error: BotIndex:%d doesn't exist", bot_index);
			return;
		}

		this->bot[bot_index].MinAccountLevel = data.attribute("vip_level").as_int();
		this->bot[bot_index].Zen = data.attribute("zen").as_int();
		this->bot[bot_index].Credits = data.attribute("credits").as_int();
		this->bot[bot_index].only_same_type = data.attribute("only_same_type").as_bool();
		this->bot[bot_index].only_same_index = data.attribute("only_same_index").as_bool();
		this->bot[bot_index].only_lower_index = data.attribute("only_if_destiny_lower").as_bool();
		this->bot[bot_index].accept_anc = data.attribute("accept_anc").as_bool();
		this->bot[bot_index].MaxLevel = data.attribute("max_level").as_int();
		this->bot[bot_index].MaxExc = data.attribute("max_excl").as_int();
	}
			
	LogAdd(LOG_BLUE,"[BotAlchemist] - %s file is Loaded",path);
}

void CBotAlchemist::MakeBot()
{
	if(this->Enabled)
	{
		for(int botNum=0;botNum<MAX_BOTALCHEMIST;botNum++)
		{
			if(this->bot[botNum].Enabled)
			{
				int aIndex = gObjAddMonster(bot[botNum].Map);

				if(aIndex == 0)
				{
					aIndex = gObjAddMonster(bot[botNum].Map);
					gObjDel(0);
				}

				if(aIndex >= 0)
				{
					LPOBJ lpObj = &gObj[aIndex];
					this->bot[botNum].index = aIndex;
					lpObj->PosNum = (WORD)-1;
					lpObj->X = this->bot[botNum].X;
					lpObj->Y = this->bot[botNum].Y;
					lpObj->OldX = this->bot[botNum].X;
					lpObj->OldY = this->bot[botNum].Y;
					lpObj->TX = this->bot[botNum].X;
					lpObj->TY = this->bot[botNum].Y;
					lpObj->MTX = this->bot[botNum].X;
					lpObj->MTY = this->bot[botNum].Y;
					lpObj->Dir = this->bot[botNum].Dir;
					lpObj->Map = this->bot[botNum].Map;
					lpObj->Live = TRUE;
					lpObj->PathCount = 0;
					lpObj->IsBot = 2;
					lpObj->Type = OBJECT_USER;
					gObjSetMonster(aIndex,this->bot[botNum].Class);

					lpObj->ChangeUp = this->bot[botNum].Class % 16;	// Set Second Type of Character
					lpObj->Class = this->bot[botNum].Class / 16;
					lpObj->Level = 400;
					lpObj->Life = 0;
					lpObj->MaxLife = 0;
					lpObj->Mana = 0;
					lpObj->MaxMana = 0;
					lpObj->Experience = 0;
					lpObj->Shield = 100;
					lpObj->MaxShield = 100;
					lpObj->DBClass = this->bot[botNum].Class;
					
					lpObj->Inventory = new CItem[INVENTORY_EXT4_SIZE];					
					lpObj->Inventory1 = new CItem[INVENTORY_EXT4_SIZE];
					lpObj->InventoryMap1 = new BYTE[INVENTORY_EXT4_SIZE];
					lpObj->InventoryMap = new BYTE[INVENTORY_EXT4_SIZE];

					for (int i=0;i<INVENTORY_EXT4_SIZE;i++)
					{
						lpObj->Inventory[i].Clear();
						lpObj->Inventory1[i].Clear();
					}

					memset(&lpObj->InventoryMap[0], (BYTE)-1, INVENTORY_EXT4_SIZE);
					memset(&lpObj->InventoryMap1[0], (BYTE)-1, INVENTORY_EXT4_SIZE);

					strncpy(lpObj->Name,this->bot[botNum].Name,sizeof(lpObj->Name));


					for(int i=0;i<9;i++)
					{
						if(this->bot[botNum].body[i].num >= 0 && this->bot[botNum].body[i].Enabled == true)
						{
							CItem lpItem;

							lpObj->Inventory[i].Clear();

							lpItem.Clear();

							lpItem.m_Index = this->bot[botNum].body[i].num;

							lpItem.m_SkillOption = 0;

							lpItem.m_LuckOption = 1;

							lpItem.m_Option = this->bot[botNum].body[i].opt;

							lpItem.m_Level = this->bot[botNum].body[i].level;

							lpItem.m_NewOption = 63;

							lpItem.m_SetOption = 5;

							lpItem.m_Durability = 255.0f;

							lpItem.m_Serial = 0;

							lpItem.m_ItemOptionEx = 0;

							lpItem.m_IsPeriodicItem = false;

							lpItem.m_IsExpiredItem = 0;

							memset(lpItem.m_SocketOption,0xFF,MAX_SOCKET_OPTION);

							lpItem.m_JewelOfHarmonyOption = 0;

							lpItem.m_SocketOptionBonus = 0xFF;

							lpItem.Convert(lpItem.m_Index,lpItem.m_SkillOption,lpItem.m_LuckOption,lpItem.m_Option,lpItem.m_NewOption,lpItem.m_SetOption,lpItem.m_JewelOfHarmonyOption,lpItem.m_ItemOptionEx,lpItem.m_SocketOption,lpItem.m_SocketOptionBonus);

							gItemManager.InventoryAddItem(aIndex,lpItem,i);
						}
					}
					
					lpObj->Inventory1 = lpObj->Inventory;
					lpObj->InventoryMap1 = lpObj->InventoryMap;
					gObjectManager.CharacterMakePreviewCharSet(aIndex);

					lpObj->AttackType = 0;
					lpObj->BotSkillAttack = 0;					

					lpObj->Attribute = 100;
					lpObj->TargetNumber = (WORD)-1;
					lpObj->ActionState.Emotion = 0;
					lpObj->ActionState.Attack = 0;
					lpObj->ActionState.EmotionCount = 0;
					lpObj->PathCount = 0;
					lpObj->BotPower = 0;
					lpObj->BotDefense = this->bot[botNum].MinAccountLevel;
					lpObj->BotLife = 1;
					lpObj->BotMaxLife = 1;
					
					lpObj->BotLvlUpDefense = 1;
					lpObj->BotLvlUpPower = 1;
					lpObj->BotLvlUpLife = 1;
					lpObj->BotLvlUpMana = 1;
					lpObj->BotLvlUpExp = 1;
					lpObj->BotLvlUpMaxLevel = 1;

					lpObj->MoveRange = 1;
					lpObj->BotFollowMe = 0;
					lpObj->NextExperience = gLevelExperience[lpObj->Level];

					GCChatSend(lpObj,"I'll COMBINE YOUR ITEMS!");
					GCActionSend(lpObj,ACTION_SALUTE1,aIndex,aIndex);

					LogAdd(LOG_BLUE, "[BotAlchemist][Bot:%d] Successfully set (aIndex: %d)",botNum,aIndex);
				}
			}
		}
	}
}

BOOL CBotAlchemist::IsInTrade(int aIndex)
{
	int number = this->GetBotIndex(gObj[aIndex].TargetNumber);
	
	if(number != -1)
		return 1;
	
	return 0;
}

int CBotAlchemist::GetBotIndex(int aIndex)
{
	for(int i=0;i<MAX_BOTALCHEMIST;i++)
	{
		if(this->bot[i].Enabled == true)
		{
			if(this->bot[i].index == aIndex)
				return i;
		}
	}
	
	return -1;
}

bool CBotAlchemist::AllowExc(BYTE BotNum, BYTE ExcOpt)
{				
	int dwExOpCount = getNumberOfExcOptions(ExcOpt);

	if(dwExOpCount < this->bot[BotNum].MaxExc)
		return true;
	
	return false;
}

BYTE CBotAlchemist::Alchemy(int aIndex,int BotNum)
{
	char text_buffer[512]={0};
	int first_item_index=-1;
	int second_item_index=-1;
	int count=0;

	if(gObjIsConnected(aIndex) == 0)
	{
		return 0;
	}

	if((gObj[aIndex].TradeMoney - this->bot[BotNum].Zen) < 0)
	{
		wsprintf(text_buffer,"I Need %d Zen!",this->bot[BotNum].Zen );
		GCChatTargetSend(&gObj[this->bot[BotNum].index],aIndex,text_buffer);
		LogAdd(LOG_RED,"[BotAlchemist][Bot:%d](%s) Insufficient Zen",BotNum,gObj[aIndex].Account);
		goto Cancel;
	}
	
	if(gObj[aIndex].BankEx.Credits - this->bot[BotNum].Credits < 0)
	{
		wsprintf(text_buffer,"I Need %d Credits!",this->bot[BotNum].Credits );
		GCChatTargetSend(&gObj[this->bot[BotNum].index],aIndex,text_buffer);
		LogAdd(LOG_RED,"[BotAlchemist][Bot:%d](%s) Insufficient PcPoints",BotNum,gObj[aIndex].Account);
		goto Cancel;
	}

	if(this->bot[BotNum].MinAccountLevel > 0)
	{
		if(this->bot[BotNum].MinAccountLevel > gObj[aIndex].AccountLevel)
		{
			wsprintf(text_buffer,"Need VIP level %d!",this->bot[BotNum].MinAccountLevel );
			GCChatTargetSend(&gObj[this->bot[BotNum].index],aIndex,text_buffer);
			LogAdd(LOG_RED,"[BotAlchemist][Bot:%d](%s) Need more Vip",BotNum,gObj[aIndex].Account);
			goto Cancel;
		}
	}

	for(int n = 0; n < TRADE_SIZE; n++)
	{
		if(gObj[aIndex].Trade[n].IsItem() == 1)
		{
			if((n >= 0 && n<=3) || (n >= 8 && n<=11) || (n >= 16 && n<=19) || (n >= 24 && n<=27))
			{
				first_item_index = n;
			}else
			{
				second_item_index = n;
			}

			count++;
		}
	}
	
	if(count != 2)
	{
		GCChatTargetSend(&gObj[this->bot[BotNum].index],aIndex,"You must put only 2 items!");
		LogAdd(LOG_RED,"[BotAlchemist](%s)(%s) Items Count Error (%d)",gObj[aIndex].Account,gObj[aIndex].Name,count);
		goto Cancel;
	}

	if(first_item_index == -1 || second_item_index == -1)
	{
		GCChatTargetSend(&gObj[this->bot[BotNum].index],aIndex,"You must put two items!");
		LogAdd(LOG_RED,"[BotAlchemist](%s)(%s) Items Position Error",gObj[aIndex].Account,gObj[aIndex].Name);
		goto Cancel;
	}
	
	if((gObj[aIndex].Trade[first_item_index].m_Index >= 6144) || (gObj[aIndex].Trade[second_item_index].m_Index >= 6144))
	{
		GCChatTargetSend(&gObj[this->bot[BotNum].index],aIndex,"Only weapons and sets are allowed!");
		LogAdd(LOG_RED,"[BotAlchemist](%s)(%s) Items Error",gObj[aIndex].Account,gObj[aIndex].Name);
		goto Cancel;
	}

	if(this->bot[BotNum].only_same_index)
	{
		if(gObj[aIndex].Trade[first_item_index].m_Index != gObj[aIndex].Trade[second_item_index].m_Index)
		{
			GCChatTargetSend(&gObj[this->bot[BotNum].index],aIndex,"You must put two identical items!");
			LogAdd(LOG_RED,"[BotAlchemist](%s)(%s) Item Index Error",gObj[aIndex].Account,gObj[aIndex].Name);
			goto Cancel;
		}
	}

	if(this->bot[BotNum].only_same_type)
	{
		int first_type = gObj[aIndex].Trade[first_item_index].m_Index/512;
		int second_type = gObj[aIndex].Trade[second_item_index].m_Index/512;
		
		if(first_type != second_type)
		{
			GCChatTargetSend(&gObj[this->bot[BotNum].index],aIndex,"You must put two items of same type!");
			LogAdd(LOG_RED,"[BotAlchemist](%s)(%s) Item Types Error",gObj[aIndex].Account,gObj[aIndex].Name);
			goto Cancel;
		}
	}

	if(this->bot[BotNum].only_lower_index)
	{
		int first_level = gItemManager.GetInfo(gObj[aIndex].Trade[first_item_index].m_Index)->Level;
		int second_level = gItemManager.GetInfo(gObj[aIndex].Trade[second_item_index].m_Index)->Level;

		if(first_level > second_level)
		{
			if(first_level > (second_level + 10))
			{
				GCChatTargetSend(&gObj[this->bot[BotNum].index],aIndex,"Only lower items than source works with my alchemy!");
				LogAdd(LOG_RED,"[BotAlchemist](%s)(%s) Item Destiny Lower Index Error",gObj[aIndex].Account,gObj[aIndex].Name);
				goto Cancel;
			}
		}
	}

	if(this->bot[BotNum].accept_anc == 0 && gObj[aIndex].Trade[first_item_index].m_SetOption > 0)
	{
		GCChatTargetSend(&gObj[this->bot[BotNum].index],aIndex,"I cant work with Ancient Items!");
		LogAdd(LOG_RED,"[BotAlchemist](%s)(%s) Ancient Item Error",gObj[aIndex].Account,gObj[aIndex].Name);
		goto Cancel;
	}

	int random = GetLargeRand()%100;
	bool failed = false;

	if(random > this->bot[BotNum].Rate )
	{
		failed = true;
	}

	if(this->bot[BotNum].AllowLuck == 1)
	{
		if(gObj[aIndex].Trade[first_item_index].m_LuckOption == 0 && gObj[aIndex].Trade[second_item_index].m_LuckOption == 1)
		{
			if(!failed)
				gObj[aIndex].Trade[first_item_index].m_LuckOption = 1;
			gObj[aIndex].Trade[second_item_index].m_LuckOption = 0;
		}
	}
	
	if(this->bot[BotNum].AllowLevel == 1)
	{
		int fLevel = gObj[aIndex].Trade[first_item_index].m_Level;
		if(fLevel < this->bot[BotNum].MaxLevel)
		{
			int sLevel = gObj[aIndex].Trade[second_item_index].m_Level;
			if ((fLevel + sLevel) > this->bot[BotNum].MaxLevel)
			{
				sLevel -= (this->bot[BotNum].MaxLevel - fLevel);
				if(!failed)
					fLevel = this->bot[BotNum].MaxLevel;
			}else
			{
				if(!failed)
					fLevel += sLevel;
				sLevel = 0;
			}
			gObj[aIndex].Trade[first_item_index].m_Level = fLevel;
			gObj[aIndex].Trade[second_item_index].m_Level = sLevel;
		}
	}
	
	if(this->bot[BotNum].AllowSkill == 1)
	{
		if(gObj[aIndex].Trade[first_item_index].m_Index < 3584)
		{
			if(gObj[aIndex].Trade[first_item_index].m_SkillOption == 0 && gObj[aIndex].Trade[second_item_index].m_SkillOption == 1)
			{
				if(!failed)
					gObj[aIndex].Trade[first_item_index].m_SkillOption = 1;
				gObj[aIndex].Trade[second_item_index].m_SkillOption = 0;
			}
		}
	}
	
	if(this->bot[BotNum].AllowOpt == 1)
	{
		int fOpt = gObj[aIndex].Trade[first_item_index].m_Option;
		if(fOpt < 7)
		{
			int sOpt = gObj[aIndex].Trade[second_item_index].m_Option;
			if ((fOpt + sOpt) > 7)
			{
				sOpt -= (7 - fOpt);
				if(!failed)
					fOpt = 7;
			}else
			{
				if(!failed)
					fOpt += sOpt;
				sOpt = 0;
			}
			gObj[aIndex].Trade[first_item_index].m_Option = fOpt;
			gObj[aIndex].Trade[second_item_index].m_Option = sOpt;
		}
	}
	
	if(this->bot[BotNum].AllowExc == 1)
	{
		if(gObj[aIndex].Trade[first_item_index].m_NewOption != 63)
		{
			if(gObj[aIndex].Trade[second_item_index].m_NewOption != 0)
			{		
				if(this->AllowExc(BotNum,gObj[aIndex].Trade[first_item_index].m_NewOption) == true)
				{
					BYTE fLife		= gObj[aIndex].Trade[first_item_index].IsExtLifeAdd();
					BYTE fMana		= gObj[aIndex].Trade[first_item_index].IsExtManaAdd();
					BYTE fDmg		= gObj[aIndex].Trade[first_item_index].IsExtDamageMinus();
					BYTE fReflect	= gObj[aIndex].Trade[first_item_index].IsExtDamageReflect();
					BYTE fDef		= gObj[aIndex].Trade[first_item_index].IsExtDefenseSuccessfull();
					BYTE fMoney		= gObj[aIndex].Trade[first_item_index].IsExtMonsterMoney();
					
					BYTE sLife		= gObj[aIndex].Trade[second_item_index].IsExtLifeAdd();
					BYTE sMana		= gObj[aIndex].Trade[second_item_index].IsExtManaAdd();
					BYTE sDmg		= gObj[aIndex].Trade[second_item_index].IsExtDamageMinus();
					BYTE sReflect	= gObj[aIndex].Trade[second_item_index].IsExtDamageReflect();
					BYTE sDef		= gObj[aIndex].Trade[second_item_index].IsExtDefenseSuccessfull();
					BYTE sMoney		= gObj[aIndex].Trade[second_item_index].IsExtMonsterMoney();

					
					if(fLife == 0 && sLife != 0)
					{
						if(!failed)
							gObj[aIndex].Trade[first_item_index].m_NewOption |= 0x20;
						gObj[aIndex].Trade[second_item_index].m_NewOption &= ~0x20;
					}
					if(this->AllowExc(BotNum,gObj[aIndex].Trade[first_item_index].m_NewOption) == true)
					{
						if(fMana == 0 && sMana != 0)
						{
							if(!failed)
								gObj[aIndex].Trade[first_item_index].m_NewOption |= 0x10;
							gObj[aIndex].Trade[second_item_index].m_NewOption &= ~0x10;
						}
					}
		
					if(this->AllowExc(BotNum,gObj[aIndex].Trade[first_item_index].m_NewOption) == true)
					{
						if(fDmg == 0 && sDmg != 0)
						{
							if(!failed)
								gObj[aIndex].Trade[first_item_index].m_NewOption |= 8;
							gObj[aIndex].Trade[second_item_index].m_NewOption &= ~8;
						}
					}
					
					if(this->AllowExc(BotNum,gObj[aIndex].Trade[first_item_index].m_NewOption) == true)
					{
						if(fReflect == 0 && sReflect != 0)
						{
							if(!failed)
								gObj[aIndex].Trade[first_item_index].m_NewOption |= 4;
							gObj[aIndex].Trade[second_item_index].m_NewOption &= ~4;
						}
					}
					
					if(this->AllowExc(BotNum,gObj[aIndex].Trade[first_item_index].m_NewOption) == true)
					{
						if(fDef == 0 && sDef != 0)
						{
							if(!failed)
								gObj[aIndex].Trade[first_item_index].m_NewOption |= 2;
							gObj[aIndex].Trade[second_item_index].m_NewOption &= ~2;
						}
					}
					
					if(this->AllowExc(BotNum,gObj[aIndex].Trade[first_item_index].m_NewOption) == true)
					{
						if(fMoney == 0 && sMoney != 0)
						{
							if(!failed)
								gObj[aIndex].Trade[first_item_index].m_NewOption |= 1;
							gObj[aIndex].Trade[second_item_index].m_NewOption &= ~1;
						}
					}
				}
			}
		}
	}

	gObj[aIndex].Trade[first_item_index].m_Durability = gItemManager.GetItemDurability(gObj[aIndex].Trade[first_item_index].m_Index,gObj[aIndex].Trade[first_item_index].m_Level,gObj[aIndex].Trade[first_item_index].m_NewOption,gObj[aIndex].Trade[first_item_index].m_SetOption);
	gObj[aIndex].Trade[second_item_index].m_Durability = gItemManager.GetItemDurability(gObj[aIndex].Trade[second_item_index].m_Index,gObj[aIndex].Trade[second_item_index].m_Level,gObj[aIndex].Trade[second_item_index].m_NewOption,gObj[aIndex].Trade[second_item_index].m_SetOption);
	
	int a = gItemManager.InventoryInsertItem(aIndex,gObj[aIndex].Trade[first_item_index]);
	int b = gItemManager.InventoryInsertItem(aIndex,gObj[aIndex].Trade[second_item_index]);
	
	if(a == 255 || b == 255)
	{
		GCChatTargetSend(&gObj[this->bot[BotNum].index],aIndex,"Insufficient space in inventory!");
		LogAdd(LOG_RED,"[BotAlchemist](%s)(%s) Doesn't have space on inventory",gObj[aIndex].Account,gObj[aIndex].Name);
		goto Cancel;
	}

	if(failed)
	{
		GCChatTargetSend(&gObj[this->bot[BotNum].index],aIndex,"Transmutation failed!");
		LogAdd(LOG_RED,"[BotAlchemist][Bot:%d](%s)(%s) Alchemy Failed",BotNum,gObj[aIndex].Account,gObj[aIndex].Name);
	}
	else
	{
		GCChatTargetSend(&gObj[this->bot[BotNum].index],aIndex,"Transmutation success!");
		LogAdd(LOG_RED,"[BotAlchemist][Bot:%d](%s)(%s) Alchemy Success",BotNum,gObj[aIndex].Account,gObj[aIndex].Name);
	}

	gObj[aIndex].Money -= this->bot[BotNum].Zen;
	
	if(this->bot[BotNum].Credits > 0)
	{
		gObj[aIndex].BankEx.Credits -= this->bot[BotNum].Credits;
		gBankEx.GCUpdateBankEx(aIndex);
		gBankEx.GDSavePoint(aIndex);
	}

	gObjInventoryCommit(aIndex);
	gObj[aIndex].TargetNumber = -1;
	gObj[aIndex].Interface.use = 0;
	gObj[aIndex].Interface.type = INTERFACE_NONE;
	gObj[aIndex].Interface.state = 0;
	gObj[aIndex].TradeOk = false;
	gObj[aIndex].TradeMoney = 0;
	GCMoneySend(aIndex,gObj[aIndex].Money);
	gTrade.GCTradeResultSend(aIndex,1);
	gItemManager.GCItemListSend(aIndex);
	gObjectManager.CharacterMakePreviewCharSet(aIndex);
	GDCharacterInfoSaveSend(aIndex);

	gTrade.ClearTrade(&gObj[aIndex]);
	
	for(int n = 0; n < TRADE_SIZE; n++)
	{
		gObj[aIndex].Trade[n].Clear();
	}
	
	gObjNotifyUpdateUnionV1(&gObj[aIndex]);
	gObjNotifyUpdateUnionV2(&gObj[aIndex]);
	
	return 1;

Cancel:
	gTrade.ResetTrade(aIndex);
	gTrade.GCTradeResultSend(aIndex,3);
	return 0;
}

void CBotAlchemist::TradeOk(int aIndex)
{
	int number = this->GetBotIndex(gObj[aIndex].TargetNumber);

	if(number == -1)
	{			
		gObj[aIndex].Interface.state = 0;
		gObj[aIndex].Interface.type = 0;
		gObj[aIndex].TargetShopNumber = -1;
		gObj[aIndex].Transaction = 0;

		return;
	}
	
	this->Alchemy(aIndex,number);
}

BOOL CBotAlchemist::TradeOpen(int aIndex, int bot_index)
{
	if(gObjIsConnected(aIndex) == 0)
	{
		return 0;
	}
	
	if(gObjIsConnected(bot_index) == 0)
	{
		return 0;
	}

	int number = this->GetBotIndex(bot_index);
	
	if(number == -1)
		return 0;
	
	char text_buffer[512]={0};
	
	LPOBJ lpObj = &gObj[aIndex];
	LPOBJ lpBot = &gObj[bot_index];

	if(this->bot[number].MinAccountLevel > lpObj->AccountLevel)
	{
		GCChatTargetSend(&gObj[this->bot[number].index],aIndex,"Im only work for VIPs!");
		LogAdd(LOG_RED,"[BotAlchemist][Bot:%d](%s) Your VIP level is too small.",number,gObj[aIndex].Account);
		return 0;
	}

	if((lpObj->BankEx.Credits - this->bot[number].Credits) < 0)
	{
		wsprintf(text_buffer,"I Need %d Credits!",this->bot[number].Credits );
		GCChatTargetSend(&gObj[this->bot[number].index],aIndex,text_buffer);
		LogAdd(LOG_RED,"[BotAlchemist][Bot:%d](%s) Insufficient PCPoints",number,gObj[aIndex].Account);
		return 0;
	}
	
	if(this->Enabled == TRUE)
	{
		if ( lpObj->Interface.use > 0 )
		{
			return 0;
		}
		
		for(int n = 0; n < TRADE_SIZE; n++)
		{
			lpObj->Trade[n].Clear();
		}
		memset(lpObj->TradeMap, (BYTE)-1, TRADE_SIZE );

		gObjInventoryTransaction(lpObj->Index);
		gTrade.GCTradeResponseSend(lpObj->Index, true, lpBot->Name, 400, 0);
		gTrade.GCTradeOkButtonSend(lpObj->Index, 1);
		lpObj->Interface.state = 1;
		lpObj->Interface.use = 1;
		lpObj->Interface.type = 1;
		lpObj->TradeMoney = 0;
		lpObj->TargetNumber =lpBot->Index;
		lpObj->Transaction = 1;

		LogAdd(LOG_RED,"[BotAlchemist] (%s)(%s) OPEN",gObj[aIndex].Account,gObj[aIndex].Name);
		GCChatTargetSend(&gObj[this->bot[number].index],aIndex,"Im Ready!");

		if(this->bot[number].Credits > 0)
		{
			wsprintf(text_buffer,"I Need %d PCPoints!",this->bot[number].Credits );
			gNotice.GCNoticeSend(aIndex,1,0,0,0,0,0,text_buffer);
		}
			
		if(this->bot[number].Zen > 0)
		{
			wsprintf(text_buffer,"I Need %d Zen!",this->bot[number].Zen );
			gNotice.GCNoticeSend(aIndex,1,0,0,0,0,0,text_buffer);
		}
	}
	
	return 1;
}

void CBotAlchemist::TradeCancel(int aIndex)
{
	gTrade.ResetTrade(aIndex);
	gTrade.GCTradeResultSend(aIndex,3);
}