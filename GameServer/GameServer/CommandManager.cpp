// CommandManager.cpp: implementation of the CCommandManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CommandManager.h"
#include "Protection.h"
#include "BonusManager.h"
#include "CustomAttack.h"
#include "CustomStore.h"
#include "DSProtocol.h"
#include "GameMain.h"
#include "GameMaster.h"
#include "Guild.h"
#include "ItemManager.h"
#include "Log.h"
#include "MapServerManager.h"
#include "MasterSkillTree.h"
#include "Message.h"
#include "Move.h"
#include "Notice.h"
#include "ObjectManager.h"
#include "Protocol.h"
#include "Quest.h"
#include "QuestReward.h"
#include "ResetTable.h"
#include "ServerInfo.h"
#include "Util.h"

#include "NPCTeleport.h"
#include "MarrySystem.h"
#include "Pickup.h"

#include "QuestSystemEx.h"

#include "ShopManager.h"
#include "NpcTalk.h"
#include "CastleSiegeSync.h"
#include "BankEx.h"
#include "CashShop.h"
#include "Marry.h"
#include "MapManager.h"
#include "GrandResetSystem.h"
#include "MonsterCall.h"
#include "MonsterSpawn.h"
#include "CSProtocol.h"
#include "AdminChat.h"

CCommandManager gCommandManager;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCommandManager::CCommandManager() // OK
{

}

CCommandManager::~CCommandManager() // OK
{

}

//pugi::xml_document doc;
//	pugi::xml_node declarationNode = doc.append_child(pugi::node_declaration);
//	
//
//	pugi::xml_node root = doc.append_child("CommandManager");
    

void CCommandManager::Init() // OK
{
	for(int n=0;n < MAX_COMMAND;n++)
	{
		this->m_CommandInfo[n].code = -1;
	}

	//declarationNode.append_attribute("version")    = "1.0";
	//declarationNode.append_attribute("encoding")   = "utf-8";

	/*this->Add(gMessage.GetMessage(32),COMMAND_MOVE);
	this->Add(gMessage.GetMessage(33),COMMAND_POST);
	this->Add(gMessage.GetMessage(34),COMMAND_ADD_POINT1);
	this->Add(gMessage.GetMessage(35),COMMAND_ADD_POINT2);
	this->Add(gMessage.GetMessage(36),COMMAND_ADD_POINT3);
	this->Add(gMessage.GetMessage(37),COMMAND_ADD_POINT4);
	this->Add(gMessage.GetMessage(38),COMMAND_ADD_POINT5);
	this->Add(gMessage.GetMessage(39),COMMAND_PK_CLEAR);
	this->Add(gMessage.GetMessage(40),COMMAND_MONEY);
	this->Add(gMessage.GetMessage(41),COMMAND_CHANGE);
	this->Add(gMessage.GetMessage(42),COMMAND_WARE);
	this->Add(gMessage.GetMessage(43),COMMAND_RESET);
	this->Add(gMessage.GetMessage(44),COMMAND_GM_MOVE);
	this->Add(gMessage.GetMessage(45),COMMAND_GM_POST);
	this->Add(gMessage.GetMessage(46),COMMAND_TRACK);
	this->Add(gMessage.GetMessage(47),COMMAND_TRACE);
	this->Add(gMessage.GetMessage(48),COMMAND_DISCONNECT);
	this->Add(gMessage.GetMessage(49),COMMAND_FIREWORKS);
	this->Add(gMessage.GetMessage(50),COMMAND_MAKE);
	this->Add(gMessage.GetMessage(51),COMMAND_SKIN);
	this->Add(gMessage.GetMessage(52),COMMAND_SET_MONEY);
	this->Add(gMessage.GetMessage(53),COMMAND_NOTICE);
	this->Add(gMessage.GetMessage(54),COMMAND_MASTER_RESET);
	this->Add(gMessage.GetMessage(55),COMMAND_GUILD_WAR);
	this->Add(gMessage.GetMessage(56),COMMAND_BATTLE_SOCCER);
	this->Add(gMessage.GetMessage(57),COMMAND_REQUEST);
	this->Add(gCustomStore.m_CustomStoreCommandSyntax,COMMAND_CUSTOM_STORE);
	this->Add(gCustomStore.m_CustomStoreOfflineCommandSyntax,COMMAND_CUSTOM_STORE_OFFLINE);
	this->Add(gMessage.GetMessage(58),COMMAND_HIDE);
	this->Add(gCustomAttack.m_CustomAttackCommandSyntax,COMMAND_CUSTOM_ATTACK);
	this->Add(gCustomAttack.m_CustomAttackOfflineCommandSyntax,COMMAND_CUSTOM_ATTACK_OFFLINE);
	this->Add(gMessage.GetMessage(492), COMMAND_CUSTOM_TELEPORTER);
	this->Add(gMessage.GetMessage(500), COMMAND_CLEARINVENTORY);
	this->Add(gMessage.GetMessage(501), COMMAND_DCFRIEND);
	this->Add(gMessage.GetMessage(502), COMMAND_ISMARRY);
	this->Add(gMessage.GetMessage(503), COMMAND_ISMARRYONLINE);
	this->Add(gMessage.GetMessage(504), COMMAND_TRACEMARRY);
	this->Add(gMessage.GetMessage(505), COMMAND_DIVORCE);
	this->Add(gMessage.GetMessage(506), COMMAND_OFFEXP);
	this->Add(gMessage.GetMessage(507), COMMAND_PICKUP);
	this->Add(gMessage.GetMessage(510), COMMAND_SHOP);
	this->Add(gMessage.GetMessage(511), COMMAND_QUEST);
	this->Add(gMessage.GetMessage(513), COMMAND_PARTYLEADER);
	this->Add(gMessage.GetMessage(515), COMMAND_DROP);
	this->Add(gMessage.GetMessage(517), COMMAND_CHATBAN);
	this->Add(gMessage.GetMessage(518), COMMAND_CHATUNBAN);*/

	//doc.save_file("CommandManager.xml");
}

void CCommandManager::Load(char* path)
{
	pugi::xml_document file;
	pugi::xml_parse_result res = file.load_file(path);

	if(res.status != pugi::status_ok)
	{
		ErrorMessageBox("%s file load fail (%s)", path, res.description());
	}

	this->Init();

	pugi::xml_node CommandManager = file.child("CommandManager");
	for (pugi::xml_node command = CommandManager.child("Command"); command; command = command.next_sibling())
	{
		COMMAND_INFO info;

		strcpy_s(info.label, command.text().as_string());

		info.code = command.attribute("Index").as_int();

		info.Active = command.attribute("Active").as_int();

		info.GMCode = command.attribute("Access").as_int();

		info.MinLevel = command.attribute("MinLevel").as_int();

		info.MinReset = command.attribute("MinReset").as_int();

		info.PriceType = command.attribute("PriceType").as_int();

		info.Price[0] = command.attribute("Price_AL0").as_int();

		info.Price[1] = command.attribute("Price_AL1").as_int();

		info.Price[2] = command.attribute("Price_AL2").as_int();

		info.Price[3] = command.attribute("Price_AL3").as_int();

		info.AccountLevel = command.attribute("AccountLevel").as_int();

		this->Add(info);
	}
}

void CCommandManager::MainProc() // OK
{
	for(int n=OBJECT_START_USER;n < MAX_OBJECT;n++)
	{
		if(gObjIsConnectedGP(n) == 0)
		{
			continue;
		}

		if(gObj[n].AutoAddPointCount > 0)
		{
			this->CommandAddPointAutoProc(&gObj[n]);
		}

		if(gObj[n].AutoResetEnable != 0)
		{
			this->CommandResetAutoProc(&gObj[n]);
		}
	}
}

void CCommandManager::Add(COMMAND_INFO info) // OK
{
	for(int n=0;n < MAX_COMMAND;n++)
	{
		if(this->m_CommandInfo[n].code != -1)
		{
			continue;
		}

		this->m_CommandInfo[n].code = info.code;
		strcpy_s(this->m_CommandInfo[n].label,info.label);
		this->m_CommandInfo[n].Active = info.Active;
		this->m_CommandInfo[n].GMCode = info.GMCode;
		this->m_CommandInfo[n].MinLevel = info.MinLevel;
		this->m_CommandInfo[n].MinReset = info.MinReset;
		this->m_CommandInfo[n].PriceType = info.PriceType;
		this->m_CommandInfo[n].Price[0] = info.Price[0];
		this->m_CommandInfo[n].Price[1] = info.Price[1];
		this->m_CommandInfo[n].Price[2] = info.Price[2];
		this->m_CommandInfo[n].Price[3] = info.Price[3];
		this->m_CommandInfo[n].AccountLevel = info.AccountLevel;
		break;
	}
}

void CCommandManager::Add(char* label, int code)
{
	for(int n=0;n < MAX_COMMAND;n++)
	{
		if(this->m_CommandInfo[n].code != -1)
		{
			continue;
		}

		/*pugi::xml_node nodeChild = root.append_child("Command");

		nodeChild.text().set(label);

		nodeChild.append_attribute("Index") = code;

		nodeChild.append_attribute("Access") = 32;

		nodeChild.append_attribute("MinLevel") = 0;

		nodeChild.append_attribute("MinReset") = 0;

		nodeChild.append_attribute("PriceType") = 0;

		nodeChild.append_attribute("Price") = 0;

		nodeChild.append_attribute("AccountLevel") = 0;*/

		this->m_CommandInfo[n].code = code;
		strcpy_s(this->m_CommandInfo[n].label,label);
		break;
	}
}


long CCommandManager::GetNumber(char* arg,int pos) // OK
{
	int count=0,p=0;

	char buffer[60] = {0};

	int len = strlen(arg);

	len = ((len>=sizeof(buffer))?(sizeof(buffer)-1):len);

	for(int n=0;n < len;n++)
	{
		if(arg[n] == 0x20)
		{
			count++;
		}
		else if(count == pos)
		{
			buffer[p] = arg[n];
			p++;
		}
	}

	return atoi(buffer);
}

void CCommandManager::GetString(char* arg,char* out,int size,int pos) // OK
{
	int count=0,p=0;

	char buffer[60] = {0};

	int len = strlen(arg);

	len = ((len>=sizeof(buffer))?(sizeof(buffer)-1):len);

	for(int n=0;n < len;n++)
	{
		if(arg[n] == 0x20)
		{
			count++;
		}
		else if(count == pos)
		{
			buffer[p] = arg[n];
			p++;
		}
	}

	memcpy(out,buffer,(size-1));
}

long CCommandManager::GetCommandCode(char* label) // OK
{
	for(int n=0;n < MAX_COMMAND;n++)
	{
		if(this->m_CommandInfo[n].code == -1)
		{
			continue;
		}

		if(_stricmp(label,this->m_CommandInfo[n].label) == 0)
		{
			return this->m_CommandInfo[n].code;
		}
	}

	return 0;
}

char* CCommandManager::GetLabelByCode(int Code)
{
	for(int n=0;n < MAX_COMMAND;n++)
	{
		if(this->m_CommandInfo[n].code == -1)
		{
			continue;
		}

		//if(_stricmp(label,this->m_CommandInfo[n].label) == 0)
		if (Code == this->m_CommandInfo[n].code)
		{
			return this->m_CommandInfo[n].label;
		}
	}

	return 0;
}


COMMAND_INFO* CCommandManager::GetCommandInfo(int index)
{
	for(int n=0;n < MAX_COMMAND;n++)
	{
		if(this->m_CommandInfo[n].code == -1)
		{
			continue;
		}

		if(this->m_CommandInfo[n].code == index)
		{
			return &this->m_CommandInfo[n];
		}
	}

	return 0;
}

//int CCommandManager::CheckCommand(LPOBJ lpObj, int CommandCode)
//{
//	COMMAND_INFO* lpInfo = this->GetCommand(CommandCode);
//
//	if (lpObj->Authority < lpInfo->GMCode)
//	{
//		gNotice.GCNoticeSend(lpObj->Index,1, 0, 0, 0, 0, 0, "Your access code is wrong for this command");
//		return 0;
//	}
//
//	if (lpInfo->Price > 0)
//	{
//		switch(lpInfo->PriceType)
//		{
//			case 0:
//				{
//					if (lpObj->Money < lpInfo->Price)
//					{
//						gNotice.GCNoticeSend(lpObj->Index,1, 0, 0, 0, 0, 0, "You are short money for this command");
//						return 0;
//					}
//				}
//				break;
//		}
//	}
//	return 1;
//}

#if (TEST_COMMAND_FIX != 1)
bool CCommandManager::ManagementCore(LPOBJ lpObj,char* message) // OK
#else
void CCommandManager::ManagementCore(LPOBJ lpObj,char* message)
#endif
{
	char command[32] = {0};

	memset(command,0,sizeof(command));

	gCommandManager.GetString(message,command,sizeof(command),0);

#if (TEST_COMMAND_FIX != 1)
	int code = gCommandManager.GetCommandCode(command);
	if (code != 0)
	{
		gCashShop.GDCashShopRecievePointSend(lpObj->Index,(DWORD)&CCommandManager::ManagementCodeCallback,(DWORD)message,0);
		return 1;
	}
#else
	//char command[32] = {0};

	//memset(command,0,sizeof(command));

	//gCommandManager.GetString(message,command,sizeof(command),0);

	int code = gCommandManager.GetCommandCode(command);

	char* argument = &message[strlen(command)];

	if(argument[0] == 0x20)
	{
		argument++;
	}

	if (code != COMMAND_POST)
	{
		gAdminChat.add_to_list(lpObj->Name,message,eChatType::eCHAT_SIMPLE);
	}

	COMMAND_INFO* lpInfo = gCommandManager.GetCommandInfo(code);

	if (lpInfo == 0)
	{
		return;
	}

	if (lpInfo->Active == 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1, 0, 0, 0, 0, 0, "This command not active...");
		return;
	}

	if (lpObj->Authority < lpInfo->GMCode)
	{
		gNotice.GCNoticeSend(lpObj->Index,1, 0, 0, 0, 0, 0, "Your access code is wrong for this command");
		return;
	}

	if (lpInfo->Price[lpObj->AccountLevel] > 0)
	{
		switch(lpInfo->PriceType)
		{
			case 0:
				{
					if (lpObj->Money < lpInfo->Price[lpObj->AccountLevel])
					{
						gNotice.GCNoticeSend(lpObj->Index,1, 0, 0, 0, 0, 0, "Need more %d %s for this command", lpInfo->Price[lpObj->AccountLevel] - lpObj->Money,gMessage.GetMessage(523));
						return;
					}
				}
				break;
			case 4:
				{
					if (lpObj->BankEx.Credits < lpInfo->Price[lpObj->AccountLevel])
					{
						gNotice.GCNoticeSend(lpObj->Index,1, 0, 0, 0, 0, 0, "Need more %d %s for this command", lpInfo->Price[lpObj->AccountLevel] - lpObj->BankEx.Credits,gMessage.GetMessage(527));
						return;
					}
				}
				break;
			default:
				{
					gNotice.GCNoticeSend(lpObj->Index,1, 0, 0, 0, 0, 0, "Wrong price type!");
					return;
				}
		}
	}

	if (lpObj->AccountLevel < lpInfo->AccountLevel)
	{
		gNotice.GCNoticeSend(lpObj->Index,1, 0, 0, 0, 0, 0, "Your AccountLevel is small for this command");
		return;
	}

	if (lpObj->Level < lpInfo->MinLevel)
	{
		gNotice.GCNoticeSend(lpObj->Index,1, 0, 0, 0, 0, 0, "Your Level is small for this command");
		return;
	}

	if (lpObj->Reset < lpInfo->MinReset)
	{
		gNotice.GCNoticeSend(lpObj->Index,1, 0, 0, 0, 0, 0, "Your Reset is small for this command");
		return;
	}

	switch(code)
	{
		case COMMAND_MOVE:
			gCommandManager.CommandMove(lpObj,argument);
			break;
		case COMMAND_POST:
			gCommandManager.CommandPost(lpObj,argument);
			break;
		case COMMAND_ADD_POINT1:
			gCommandManager.CommandAddPoint(lpObj,argument,0);
			break;
		case COMMAND_ADD_POINT2:
			gCommandManager.CommandAddPoint(lpObj,argument,1);
			break;
		case COMMAND_ADD_POINT3:
			gCommandManager.CommandAddPoint(lpObj,argument,2);
			break;
		case COMMAND_ADD_POINT4:
			gCommandManager.CommandAddPoint(lpObj,argument,3);
			break;
		case COMMAND_ADD_POINT5:
			gCommandManager.CommandAddPoint(lpObj,argument,4);
			break;
		case COMMAND_ADD_POINT6:
			gCommandManager.CommandAddPoint(lpObj, argument, 5);
			break;
		case COMMAND_PK_CLEAR:
			gCommandManager.CommandPKClear(lpObj,argument);
			break;
		case COMMAND_MONEY:
			gCommandManager.CommandMoney(lpObj,argument);
			break;
		case COMMAND_CHANGE:
			gCommandManager.CommandChange(lpObj,argument);
			break;
		case COMMAND_WARE:
			gCommandManager.CommandWare(lpObj,argument);
			break;
		case COMMAND_RESET:
			gCommandManager.CommandReset(lpObj,argument);
			break;
		case COMMAND_GM_MOVE:
			gCommandManager.CommandGMMove(lpObj,argument);
			break;
		case COMMAND_GM_POST:
			gCommandManager.CommandGMPost(lpObj,argument);
			break;
		case COMMAND_TRACK:
			gCommandManager.CommandTrack(lpObj,argument);
			break;
		case COMMAND_TRACE:
			gCommandManager.CommandTrace(lpObj,argument);
			break;
		case COMMAND_DISCONNECT:
			gCommandManager.CommandDisconnect(lpObj,argument);
			break;
		case COMMAND_FIREWORKS:
			gCommandManager.CommandFireworks(lpObj,argument);
			break;
		case COMMAND_MAKE:
			gCommandManager.CommandMake(lpObj,argument);
			break;
		case COMMAND_DROP:
			gCommandManager.CommandDrop(lpObj, argument);
			break;
		case COMMAND_SKIN:
			gCommandManager.CommandSkin(lpObj,argument);
			break;
		case COMMAND_SET_MONEY:
			gCommandManager.CommandSetMoney(lpObj,argument);
			break;
		case COMMAND_NOTICE:
			gCommandManager.CommandNotice(lpObj,argument);
			break;
		case COMMAND_SET_CREDITS:
			gCommandManager.CommandSetCretits(lpObj,argument);
			break;
		/*case COMMAND_MASTER_RESET:
			gCommandManager.CommandMasterReset(lpObj,argument);
			break;*/
		case COMMAND_GUILD_WAR:
			gCommandManager.CommandGuildWar(lpObj,argument);
			break;
		case COMMAND_BATTLE_SOCCER:
			gCommandManager.CommandBattleSoccer(lpObj,argument);
			break;
		case COMMAND_REQUEST:
			gCommandManager.CommandRequest(lpObj,argument);
			break;
		case COMMAND_CUSTOM_STORE:
			gCustomStore.CommandCustomStore(lpObj,argument);
			break;
		case COMMAND_CUSTOM_STORE_OFFLINE:
			gCustomStore.CommandCustomStoreOffline(lpObj,argument);
			break;
		case COMMAND_HIDE:
			gCommandManager.CommandHide(lpObj,argument);
			break;
		case COMMAND_CUSTOM_ATTACK:
			gCustomAttack.CommandCustomAttack(lpObj,argument);
			break;
		case COMMAND_CUSTOM_ATTACK_OFFLINE:
			gCustomAttack.CommandCustomAttackOffline(lpObj,argument);
			break;
		case COMMAND_CUSTOM_TELEPORTER:
			gTeleporter.CommandCustomTeleport(lpObj, argument);
			break;
		case COMMAND_CLEARINVENTORY:
			gCommandManager.CommandClearInventory(lpObj);
			break;
		case COMMAND_DCFRIEND:
			gCommandManager.CommandDCFriend(lpObj, argument);
			break;
		case COMMAND_ISMARRY:
			gMarrySystem.IsMarry(lpObj, argument);
			break;
		case COMMAND_ISMARRYONLINE:
			gMarrySystem.IsMarryOnline(lpObj);
			break;
		case COMMAND_TRACEMARRY:
			gMarrySystem.TraceMarry(lpObj);
			break;
		case COMMAND_DIVORCE:
			gMarrySystem.MarryDivorce(lpObj);
			break;
		case COMMAND_OFFEXP:
			{
				gCustomAttack.CommandCustomAttack(lpObj,argument);
				gCustomAttack.CommandCustomAttackOffline(lpObj,argument);
			//gCustomAttack.RestoreOffline();
			}
			break;
		case COMMAND_PICKUP:
			gPickup.PickCommand(lpObj, argument);
			break;
		case COMMAND_QUEST:
			gCommandManager.CommandQuest(lpObj);
			break;
		case COMMAND_PARTYLEADER:
			gCommandManager.CommandPartyLeader(lpObj, argument);
			break;
		case COMMAND_CHATBAN:
			gCommandManager.CommandChatBan(lpObj, argument);
			break;
		case COMMAND_CHATUNBAN:
			gCommandManager.CommandChatUnBan(lpObj,argument);
			break;
		case COMMAND_MARRYPROP:
			gCommandManager.CommandMarryProp(lpObj,argument);
			break;
		case COMMAND_MARRYACCEPT:
			gCommandManager.CommandMarryAccept(lpObj,argument);
			break;
		case COMMAND_MARRYTP:
			gCommandManager.CommandMarryTP(lpObj,argument);
			break;
		case COMMAND_MARRYDIVORCE:
			gCommandManager.CommandMarryDivorce(lpObj,argument);
			break;
		case COMMAND_MLRESET:
			gObjRebuildMasterSkillTree(lpObj);
			//gCashShop.GDCashShopRecievePointSend(lpObj->Index,(DWORD)&CCommandManager::CommandMLResetCallback,0,0);
			break;
		/*case COMMAND_MONSTERCALL:
			gCommandManager.CommandMonsterCall(lpObj,argument);
			break;*/
		case COMMAND_MONSTERSPAWN:
			gMonsterSpawn.CreateMonster(lpObj,argument);
			break;
		case COMMAND_EXCHANGEWCOINP:
			gCommandManager.CommandExchangeWcoinP(lpObj,argument);
			break;
		case COMMAND_SEND1:
			gCommandManager.SendZenCustomVault(lpObj,argument);
			break;
		case COMMAND_SEND2:
			gCommandManager.SendBlessCustomVault(lpObj,argument);
			break;
		case COMMAND_SEND3:
			gCommandManager.SendSoulCustomVault(lpObj,argument);
			break;
		case COMMAND_SEND4:
			gCommandManager.SendChaosCustomVault(lpObj,argument);
			break;
		case COMMAND_SEND5:
			gCommandManager.SendLifeCustomVault(lpObj,argument);
			break;
		case COMMAND_SEND6:
			gCommandManager.SendCreationCustomVault(lpObj,argument);
			break;
		case COMMAND_RECV1:
			gCommandManager.RecvZenCustomVault(lpObj,argument);
			break;
		case COMMAND_RECV2:
			gCommandManager.RecvBlessCustomVault(lpObj,argument);
			break;
		case COMMAND_RECV3:
			gCommandManager.RecvSoulCustomVault(lpObj,argument);
			break;
		case COMMAND_RECV4:
			gCommandManager.RecvChaosCustomVault(lpObj,argument);
			break;
		case COMMAND_RECV5:
			gCommandManager.RecvLifeCustomVault(lpObj,argument);
			break;
		case COMMAND_RECV6:
			gCommandManager.RecvCreationCustomVault(lpObj,argument);
			break;
		case COMMAND_PACKIU:
			gCommandManager.PackIudexItem(lpObj,argument);
			break;
		case COMMAND_UNPACKIU:
			gCommandManager.UnPackIudexItem(lpObj,argument);
			break;
			case COMMAND_SENDGUARDIAN:
			gCommandManager.SendGuardianCustomVault(lpObj,argument);
			break;
		case COMMAND_SENDHARMONY:
			gCommandManager.SendHarmonyCustomVault(lpObj,argument);
			break;
		case COMMAND_SENDLOWSTONE:
			gCommandManager.SendLowStoneCustomVault(lpObj,argument);
			break;
		case COMMAND_SENDHIGHSTONE:
			gCommandManager.SendHighStoneCustomVault(lpObj,argument);
			break;
		case COMMAND_SENDGEMSTONE:
			gCommandManager.SendGemStoneCustomVault(lpObj,argument);
			break;
		case COMMAND_RECVGUARDIAN:
			gCommandManager.RecvGuardianCustomVault(lpObj,argument);
			break;
		case COMMAND_RECVHARMONY:
			gCommandManager.RecvHarmonyCustomVault(lpObj,argument);
			break;
		case COMMAND_RECVLOWSTONE:
			gCommandManager.RecvLowStoneCustomVault(lpObj,argument);
			break;
		case COMMAND_RECVHIGHSTONE:
			gCommandManager.RecvHighStoneCustomVault(lpObj,argument);
			break;
		case COMMAND_RECVGEMSTONE:
			gCommandManager.RecvGemStoneCustomVault(lpObj,argument);
			break;
		default:
			return;
	}

	if (lpInfo->Price[lpObj->AccountLevel] <= 0)
	{
		return;
	}

	switch(lpInfo->PriceType)
	{
		case 0:
			lpObj->Money -= lpInfo->Price[lpObj->AccountLevel];
			GCMoneySend(lpObj->Index, lpObj->Money);
			break;
		/*case 1:
			gCashShop.GDCashShopSubPointSaveSend(lpObj->Index,0,lpInfo->Price[lpObj->AccountLevel],0,0);
			break;
		case 2:
			gCashShop.GDCashShopSubPointSaveSend(lpObj->Index,0,0,lpInfo->Price[lpObj->AccountLevel],0);
			break;
		case 3:
			gCashShop.GDCashShopSubPointSaveSend(lpObj->Index,0,0,0,lpInfo->Price[lpObj->AccountLevel]);
			break;*/
		case 4:
			lpObj->BankEx.Credits -= lpInfo->Price[lpObj->AccountLevel];
			gBankEx.GCUpdateBankEx(lpObj->Index);
			gBankEx.GDSavePoint(lpObj->Index);
			break;
		default:
			gNotice.GCNoticeSend(lpObj->Index,1, 0, 0, 0, 0, 0, "Wrong price type!");
			break;
	}
#endif
	

#if (TEST_COMMAND_FIX != 1)
	return false;
#endif
}

void CCommandManager::ManagementCodeCallback(LPOBJ lpObj,char message[MAX_CHAT_MESSAGE_SIZE],DWORD slot,DWORD WCoinC,DWORD WCoinP,DWORD GoblinPoint)
{
	char command[32] = {0};

	memset(command,0,sizeof(command));

	gCommandManager.GetString(message,command,sizeof(command),0);

	int code = gCommandManager.GetCommandCode(command);

	char* argument = &message[strlen(command)];

	if(argument[0] == 0x20)
	{
		argument++;
	}

	COMMAND_INFO* lpInfo = gCommandManager.GetCommandInfo(code);

	if (lpInfo == 0)
	{
		return;
	}

	if (lpInfo->Active == 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1, 0, 0, 0, 0, 0, "This command not active...");
		return;
	}

	if (lpObj->Authority < lpInfo->GMCode)
	{
		gNotice.GCNoticeSend(lpObj->Index,1, 0, 0, 0, 0, 0, "Your access code is wrong for this command");
		return;
	}

	if (lpInfo->Price[lpObj->AccountLevel] > 0)
	{
		switch(lpInfo->PriceType)
		{
			case 0:
				{
					if (lpObj->Money < lpInfo->Price[lpObj->AccountLevel])
					{
						gNotice.GCNoticeSend(lpObj->Index,1, 0, 0, 0, 0, 0, "Need more %d %s for this command", lpInfo->Price[lpObj->AccountLevel] - lpObj->Money,gMessage.GetMessage(523));
						return;
					}
				}
				break;
			case 1:
				{
					if (WCoinC < lpInfo->Price[lpObj->AccountLevel])
					{
						gNotice.GCNoticeSend(lpObj->Index,1, 0, 0, 0, 0, 0, "Need more %d %s for this command", lpInfo->Price[lpObj->AccountLevel] - WCoinC,gMessage.GetMessage(524));
						return;
					}
				}
				break;
			case 2:
				{
					if (WCoinP < lpInfo->Price[lpObj->AccountLevel])
					{
						gNotice.GCNoticeSend(lpObj->Index,1, 0, 0, 0, 0, 0, "Need more %d %s for this command", lpInfo->Price[lpObj->AccountLevel] - WCoinP,gMessage.GetMessage(525));
						return;
					}
				}
				break;
			case 3:
				{
					if (GoblinPoint < lpInfo->Price[lpObj->AccountLevel])
					{
						gNotice.GCNoticeSend(lpObj->Index,1, 0, 0, 0, 0, 0, "Need more %d %s for this command", lpInfo->Price[lpObj->AccountLevel] - GoblinPoint,gMessage.GetMessage(526));
						return;
					}
				}
				break;
			case 4:
				{
					if (lpObj->BankEx.Credits < lpInfo->Price[lpObj->AccountLevel])
					{
						gNotice.GCNoticeSend(lpObj->Index,1, 0, 0, 0, 0, 0, "Need more %d %s for this command", lpInfo->Price[lpObj->AccountLevel] - lpObj->BankEx.Credits,gMessage.GetMessage(527));
						return;
					}
				}
				break;
			default:
				{
					gNotice.GCNoticeSend(lpObj->Index,1, 0, 0, 0, 0, 0, "Wrong price type!");
					return;
				}
		}
	}

	if (lpObj->AccountLevel < lpInfo->AccountLevel)
	{
		gNotice.GCNoticeSend(lpObj->Index,1, 0, 0, 0, 0, 0, "Your AccountLevel is small for this command");
		return;
	}

	if (lpObj->Level < lpInfo->MinLevel)
	{
		gNotice.GCNoticeSend(lpObj->Index,1, 0, 0, 0, 0, 0, "Your Level is small for this command");
		return;
	}

	if (lpObj->Reset < lpInfo->MinReset)
	{
		gNotice.GCNoticeSend(lpObj->Index,1, 0, 0, 0, 0, 0, "Your Reset is small for this command");
		return;
	}

	switch(code)
	{
		case COMMAND_MOVE:
			gCommandManager.CommandMove(lpObj,argument);
			break;
		case COMMAND_POST:
			gCommandManager.CommandPost(lpObj,argument);
			break;
		case COMMAND_ADD_POINT1:
			gCommandManager.CommandAddPoint(lpObj,argument,0);
			break;
		case COMMAND_ADD_POINT2:
			gCommandManager.CommandAddPoint(lpObj,argument,1);
			break;
		case COMMAND_ADD_POINT3:
			gCommandManager.CommandAddPoint(lpObj,argument,2);
			break;
		case COMMAND_ADD_POINT4:
			gCommandManager.CommandAddPoint(lpObj,argument,3);
			break;
		case COMMAND_ADD_POINT5:
			gCommandManager.CommandAddPoint(lpObj,argument,4);
			break;
		case COMMAND_ADD_POINT6:
			gCommandManager.CommandAddPoint(lpObj, argument, 5);
			break;
		case COMMAND_PK_CLEAR:
			gCommandManager.CommandPKClear(lpObj,argument);
			break;
		case COMMAND_MONEY:
			gCommandManager.CommandMoney(lpObj,argument);
			break;
		case COMMAND_CHANGE:
			gCommandManager.CommandChange(lpObj,argument);
			break;
		case COMMAND_WARE:
			gCommandManager.CommandWare(lpObj,argument);
			break;
		case COMMAND_RESET:
			gCommandManager.CommandReset(lpObj,argument);
			break;
		case COMMAND_GM_MOVE:
			gCommandManager.CommandGMMove(lpObj,argument);
			break;
		case COMMAND_GM_POST:
			gCommandManager.CommandGMPost(lpObj,argument);
			break;
		case COMMAND_TRACK:
			gCommandManager.CommandTrack(lpObj,argument);
			break;
		case COMMAND_TRACE:
			gCommandManager.CommandTrace(lpObj,argument);
			break;
		case COMMAND_DISCONNECT:
			gCommandManager.CommandDisconnect(lpObj,argument);
			break;
		case COMMAND_FIREWORKS:
			gCommandManager.CommandFireworks(lpObj,argument);
			break;
		case COMMAND_MAKE:
			gCommandManager.CommandMake(lpObj,argument);
			break;
		case COMMAND_DROP:
			gCommandManager.CommandDrop(lpObj, argument);
			break;
		case COMMAND_SKIN:
			gCommandManager.CommandSkin(lpObj,argument);
			break;
		case COMMAND_SET_MONEY:
			gCommandManager.CommandSetMoney(lpObj,argument);
			break;
		case COMMAND_NOTICE:
			gCommandManager.CommandNotice(lpObj,argument);
			break;
		case COMMAND_SET_CREDITS:
			gCommandManager.CommandSetCretits(lpObj,argument);
			break;
		/*case COMMAND_MASTER_RESET:
			gCommandManager.CommandMasterReset(lpObj,argument);
			break;*/
		case COMMAND_GUILD_WAR:
			gCommandManager.CommandGuildWar(lpObj,argument);
			break;
		case COMMAND_BATTLE_SOCCER:
			gCommandManager.CommandBattleSoccer(lpObj,argument);
			break;
		case COMMAND_REQUEST:
			gCommandManager.CommandRequest(lpObj,argument);
			break;
		case COMMAND_CUSTOM_STORE:
			gCustomStore.CommandCustomStore(lpObj,argument);
			break;
		case COMMAND_CUSTOM_STORE_OFFLINE:
			gCustomStore.CommandCustomStoreOffline(lpObj,argument);
			break;
		case COMMAND_HIDE:
			gCommandManager.CommandHide(lpObj,argument);
			break;
		case COMMAND_CUSTOM_ATTACK:
			gCustomAttack.CommandCustomAttack(lpObj,argument);
			break;
		case COMMAND_CUSTOM_ATTACK_OFFLINE:
			gCustomAttack.CommandCustomAttackOffline(lpObj,argument);
			break;
		case COMMAND_CUSTOM_TELEPORTER:
			gTeleporter.CommandCustomTeleport(lpObj, argument);
			break;
		case COMMAND_CLEARINVENTORY:
			gCommandManager.CommandClearInventory(lpObj);
			break;
		case COMMAND_DCFRIEND:
			gCommandManager.CommandDCFriend(lpObj, argument);
			break;
		case COMMAND_ISMARRY:
			gMarrySystem.IsMarry(lpObj, argument);
			break;
		case COMMAND_ISMARRYONLINE:
			gMarrySystem.IsMarryOnline(lpObj);
			break;
		case COMMAND_TRACEMARRY:
			gMarrySystem.TraceMarry(lpObj);
			break;
		case COMMAND_DIVORCE:
			gMarrySystem.MarryDivorce(lpObj);
			break;
		case COMMAND_OFFEXP:
			{
				gCustomAttack.CommandCustomAttack(lpObj,argument);
				gCustomAttack.CommandCustomAttackOffline(lpObj,argument);
			//gCustomAttack.RestoreOffline();
			}
			break;
		case COMMAND_PICKUP:
			gPickup.PickCommand(lpObj, argument);
			break;
		case COMMAND_QUEST:
			gCommandManager.CommandQuest(lpObj);
			break;
		case COMMAND_PARTYLEADER:
			gCommandManager.CommandPartyLeader(lpObj, argument);
			break;
		case COMMAND_CHATBAN:
			gCommandManager.CommandChatBan(lpObj, argument);
			break;
		case COMMAND_CHATUNBAN:
			gCommandManager.CommandChatUnBan(lpObj,argument);
			break;
		case COMMAND_MARRYPROP:
			gCommandManager.CommandMarryProp(lpObj,argument);
			break;
		case COMMAND_MARRYACCEPT:
			gCommandManager.CommandMarryAccept(lpObj,argument);
			break;
		case COMMAND_MARRYTP:
			gCommandManager.CommandMarryTP(lpObj,argument);
			break;
		case COMMAND_MARRYDIVORCE:
			gCommandManager.CommandMarryDivorce(lpObj,argument);
			break;
		case COMMAND_MLRESET:
			gObjRebuildMasterSkillTree(lpObj);
			//gCashShop.GDCashShopRecievePointSend(lpObj->Index,(DWORD)&CCommandManager::CommandMLResetCallback,0,0);
			break;
		/*case COMMAND_MONSTERCALL:
			gCommandManager.CommandMonsterCall(lpObj,argument);
			break;*/
		case COMMAND_MONSTERSPAWN:
			gMonsterSpawn.CreateMonster(lpObj,argument);
			break;
		case COMMAND_EXCHANGEWCOINP:
			gCommandManager.CommandExchangeWcoinP(lpObj,argument);
			break;
		case COMMAND_SEND1:
			gCommandManager.SendZenCustomVault(lpObj,argument);
			break;
		case COMMAND_SEND2:
			gCommandManager.SendBlessCustomVault(lpObj,argument);
			break;
		case COMMAND_SEND3:
			gCommandManager.SendSoulCustomVault(lpObj,argument);
			break;
		case COMMAND_SEND4:
			gCommandManager.SendChaosCustomVault(lpObj,argument);
			break;
		case COMMAND_SEND5:
			gCommandManager.SendLifeCustomVault(lpObj,argument);
			break;
		case COMMAND_SEND6:
			gCommandManager.SendCreationCustomVault(lpObj,argument);
			break;
		case COMMAND_RECV1:
			gCommandManager.RecvZenCustomVault(lpObj,argument);
			break;
		case COMMAND_RECV2:
			gCommandManager.RecvBlessCustomVault(lpObj,argument);
			break;
		case COMMAND_RECV3:
			gCommandManager.RecvSoulCustomVault(lpObj,argument);
			break;
		case COMMAND_RECV4:
			gCommandManager.RecvChaosCustomVault(lpObj,argument);
			break;
		case COMMAND_RECV5:
			gCommandManager.RecvLifeCustomVault(lpObj,argument);
			break;
		case COMMAND_RECV6:
			gCommandManager.RecvCreationCustomVault(lpObj,argument);
			break;
		case COMMAND_PACKIU:
			gCommandManager.PackIudexItem(lpObj,argument);
			break;
		case COMMAND_UNPACKIU:
			gCommandManager.UnPackIudexItem(lpObj,argument);
			break;
		case COMMAND_SENDGUARDIAN:
			gCommandManager.SendGuardianCustomVault(lpObj,argument);
			break;
		case COMMAND_SENDHARMONY:
			gCommandManager.SendHarmonyCustomVault(lpObj,argument);
			break;
		case COMMAND_SENDLOWSTONE:
			gCommandManager.SendLowStoneCustomVault(lpObj,argument);
			break;
		case COMMAND_SENDHIGHSTONE:
			gCommandManager.SendHighStoneCustomVault(lpObj,argument);
			break;
		case COMMAND_SENDGEMSTONE:
			gCommandManager.SendGemStoneCustomVault(lpObj,argument);
			break;
		case COMMAND_RECVGUARDIAN:
			gCommandManager.RecvGuardianCustomVault(lpObj,argument);
			break;
		case COMMAND_RECVHARMONY:
			gCommandManager.RecvHarmonyCustomVault(lpObj,argument);
			break;
		case COMMAND_RECVLOWSTONE:
			gCommandManager.RecvLowStoneCustomVault(lpObj,argument);
			break;
		case COMMAND_RECVHIGHSTONE:
			gCommandManager.RecvHighStoneCustomVault(lpObj,argument);
			break;
		case COMMAND_RECVGEMSTONE:
			gCommandManager.RecvGemStoneCustomVault(lpObj,argument);
			break;
		default:
			return;
	}

	if (lpInfo->Price[lpObj->AccountLevel] <= 0)
	{
		return;
	}

	switch(lpInfo->PriceType)
	{
		case 0:
			lpObj->Money -= lpInfo->Price[lpObj->AccountLevel];
			GCMoneySend(lpObj->Index, lpObj->Money);
			break;
		case 1:
			gCashShop.GDCashShopSubPointSaveSend(lpObj->Index,0,lpInfo->Price[lpObj->AccountLevel],0,0);
			break;
		case 2:
			gCashShop.GDCashShopSubPointSaveSend(lpObj->Index,0,0,lpInfo->Price[lpObj->AccountLevel],0);
			break;
		case 3:
			gCashShop.GDCashShopSubPointSaveSend(lpObj->Index,0,0,0,lpInfo->Price[lpObj->AccountLevel]);
			break;
		case 4:
			lpObj->BankEx.Credits -= lpInfo->Price[lpObj->AccountLevel];
			gBankEx.GCUpdateBankEx(lpObj->Index);
			gBankEx.GDSavePoint(lpObj->Index);
			break;
		default:
			gNotice.GCNoticeSend(lpObj->Index,1, 0, 0, 0, 0, 0, "Wrong price type!");
			break;
	}
}

void CCommandManager::CommandMove(LPOBJ lpObj,char* arg) // OK
{
	MOVE_INFO MoveInfo;

	if(gMove.GetInfoByName(arg,&MoveInfo) != 0)
	{
		gMove.Move(lpObj,MoveInfo.Index);
		return;
	}
}

void CCommandManager::CommandPost(LPOBJ lpObj,char* arg) // OK
{
	if(gServerInfo.m_CommandPostSwitch == 0)
	{
		return;
	}

	if(gServerInfo.m_CommandPostEnable[lpObj->AccountLevel] == 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(64));
		return;
	}

	if(lpObj->ChatLimitTime > 0 && gServerInfo.m_CommandBanChatSwitch != 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(269),lpObj->ChatLimitTime);
		return;
	}

	if(lpObj->Level < gServerInfo.m_CommandPostLevel[lpObj->AccountLevel])
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(65),gServerInfo.m_CommandPostLevel[lpObj->AccountLevel]);
		return;
	}

	if(lpObj->Reset < gServerInfo.m_CommandPostReset[lpObj->AccountLevel])
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(66),gServerInfo.m_CommandPostReset[lpObj->AccountLevel]);
		return;
	}

	/*if(lpObj->Money < ((DWORD)gServerInfo.m_CommandPostMoney[lpObj->AccountLevel]))
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(67),gServerInfo.m_CommandPostMoney[lpObj->AccountLevel]);
		return;
	}*/

	DWORD tick = (GetTickCount()-lpObj->PostTime)/1000;

	if(tick < ((DWORD)gServerInfo.m_CommandPostDelay[lpObj->AccountLevel]) && lpObj->Authority != 32)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(68),(gServerInfo.m_CommandPostDelay[lpObj->AccountLevel]-tick));
		return;
	}

	lpObj->PostTime = GetTickCount();

	//lpObj->Money -= gServerInfo.m_CommandPostMoney[lpObj->AccountLevel];

	GCMoneySend(lpObj->Index,lpObj->Money);

	if(gServerInfo.m_CommandPostType == 0)
	{
		PostMessage1(lpObj->Name,gMessage.GetMessage(69),arg);
	}
	else if(gServerInfo.m_CommandPostType == 1)
	{
		PostMessage2(lpObj->Name,gMessage.GetMessage(69),arg);
	}
	else if(gServerInfo.m_CommandPostType == 2)
	{
		PostMessage3(lpObj->Name,gMessage.GetMessage(69),arg);
	}
	else if(gServerInfo.m_CommandPostType == 3)
	{
		PostMessage4(lpObj->Name,gMessage.GetMessage(69),arg);
	}
	else if(gServerInfo.m_CommandPostType == 4)
	{
		GDGlobalPostSend(gMapServerManager.GetMapServerGroup(),0,lpObj->Name,arg);
	}
	else if(gServerInfo.m_CommandPostType == 5)
	{
		GDGlobalPostSend(gMapServerManager.GetMapServerGroup(),1,lpObj->Name,arg);
	}
	else if(gServerInfo.m_CommandPostType == 6)
	{
		GDGlobalPostSend(gMapServerManager.GetMapServerGroup(),2,lpObj->Name,arg);
	}
	else if(gServerInfo.m_CommandPostType == 7)
	{
		GDGlobalPostSend(gMapServerManager.GetMapServerGroup(),3,lpObj->Name,arg);
	}

	gAdminChat.add_to_list(lpObj->Name,arg,eChatType::eCHAT_POST);

	gLog.Output(LOG_COMMAND,"[CommandPost][%s][%s] - (Message: %s)",lpObj->Account,lpObj->Name,arg);
}

void CCommandManager::CommandAddPoint(LPOBJ lpObj,char* arg,int type) // OK
{
	if(gServerInfo.m_CommandAddPointEnable[lpObj->AccountLevel] == 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(70));
		return;
	}

	if(lpObj->Class != CLASS_DL && type == 4)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(71));
		return;
	}

	char mode[5] = {0};

	this->GetString(arg,mode,sizeof(mode),0);

	if(strcmp(mode,"auto") == 0)
	{
		this->CommandAddPointAuto(lpObj,arg,type);
		return;
	}

	int amount = this->GetNumber(arg,0);

	if(amount <= 0 || lpObj->LevelUpPoint < amount)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(72));
		return;
	}

	if(gObjectManager.CharacterLevelUpPointAdd(lpObj,type,amount) == 0)
	{
		return;
	}

	GCMoneySend(lpObj->Index,lpObj->Money);

	GCNewCharacterInfoSend(lpObj);

	gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(74),amount,lpObj->LevelUpPoint);

	gLog.Output(LOG_COMMAND,"[CommandAddPoint][%s][%s] - (Type: %d, Amount: %d)",lpObj->Account,lpObj->Name,type,amount);
}

void CCommandManager::CommandAddPointAuto(LPOBJ lpObj,char* arg,int type) // OK
{
	if(gServerInfo.m_CommandAddPointAutoEnable[lpObj->AccountLevel] == 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(70));
		return;
	}

	if (type == 5 && !gProtection.GetCustomState(CUSTOM_STAT))
		return;

	int amount = this->GetNumber(arg,1);

	if(amount > 0)
	{
		lpObj->AutoAddPointCount = 0;

		lpObj->AutoAddPointStats[type] = amount;

		lpObj->AutoAddPointCount = ((lpObj->AutoAddPointStats[0]>0)?(lpObj->AutoAddPointCount+1):lpObj->AutoAddPointCount);

		lpObj->AutoAddPointCount = ((lpObj->AutoAddPointStats[1]>0)?(lpObj->AutoAddPointCount+1):lpObj->AutoAddPointCount);

		lpObj->AutoAddPointCount = ((lpObj->AutoAddPointStats[2]>0)?(lpObj->AutoAddPointCount+1):lpObj->AutoAddPointCount);

		lpObj->AutoAddPointCount = ((lpObj->AutoAddPointStats[3]>0)?(lpObj->AutoAddPointCount+1):lpObj->AutoAddPointCount);

		lpObj->AutoAddPointCount = ((lpObj->AutoAddPointStats[4]>0)?(lpObj->AutoAddPointCount+1):lpObj->AutoAddPointCount);

		lpObj->AutoAddPointCount = ((lpObj->AutoAddPointStats[5] > 0) ? (lpObj->AutoAddPointCount + 1) : lpObj->AutoAddPointCount);

		GCMoneySend(lpObj->Index,lpObj->Money);

		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(114),amount);

		gLog.Output(LOG_COMMAND,"[CommandAddPointAuto][%s][%s] - (Type: %d, Amount: %d)",lpObj->Account,lpObj->Name,type,amount);
	}
	else
	{
		lpObj->AutoAddPointCount = 0;

		lpObj->AutoAddPointStats[type] = 0;

		lpObj->AutoAddPointCount = ((lpObj->AutoAddPointStats[0]>0)?(lpObj->AutoAddPointCount+1):lpObj->AutoAddPointCount);

		lpObj->AutoAddPointCount = ((lpObj->AutoAddPointStats[1]>0)?(lpObj->AutoAddPointCount+1):lpObj->AutoAddPointCount);

		lpObj->AutoAddPointCount = ((lpObj->AutoAddPointStats[2]>0)?(lpObj->AutoAddPointCount+1):lpObj->AutoAddPointCount);

		lpObj->AutoAddPointCount = ((lpObj->AutoAddPointStats[3]>0)?(lpObj->AutoAddPointCount+1):lpObj->AutoAddPointCount);

		lpObj->AutoAddPointCount = ((lpObj->AutoAddPointStats[4] > 0) ? (lpObj->AutoAddPointCount + 1) : lpObj->AutoAddPointCount);

		lpObj->AutoAddPointCount = ((lpObj->AutoAddPointStats[5]>0)?(lpObj->AutoAddPointCount+1):lpObj->AutoAddPointCount);

		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(115));
	}
}

void CCommandManager::CommandPKClear(LPOBJ lpObj,char* arg) // OK
{
	if(gServerInfo.m_CommandPKClearEnable[lpObj->AccountLevel] == 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(75));
		return;
	}

	if(lpObj->PKLevel <= 3)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(76));
		return;
	}

	GCMoneySend(lpObj->Index,lpObj->Money);

	lpObj->PKLevel = 3;

	GCPKLevelSend(lpObj->Index,lpObj->PKLevel);

	gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(78));

	gLog.Output(LOG_COMMAND,"[CommandPKClear][%s][%s] - (PKLevel: %d)",lpObj->Account,lpObj->Name,lpObj->PKLevel);
}

void CCommandManager::CommandMoney(LPOBJ lpObj,char* arg) // OK
{
	if(gServerInfo.m_CommandMoneySwitch == 0)
	{
		return;
	}

	if(gServerInfo.m_CommandMoneyEnable[lpObj->AccountLevel] == 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(79));
		return;
	}

	int money = this->GetNumber(arg,0);

	if(money < 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(80));
		return;
	}

	if(money == 0)
	{
		lpObj->Money = 0;
	}
	else if(gObjCheckMaxMoney(lpObj->Index,money) == 0)
	{
		lpObj->Money = MAX_MONEY;
	}
	else
	{
		lpObj->Money += money;
	}

	GCMoneySend(lpObj->Index,lpObj->Money);

	gLog.Output(LOG_COMMAND,"[CommandMoney][%s][%s] - (Money: %d)",lpObj->Account,lpObj->Name,money);
}

void CCommandManager::CommandChange(LPOBJ lpObj,char* arg) // OK
{
	if(gServerInfo.m_CommandChangeEnable[lpObj->AccountLevel] == 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(81));
		return;
	}

	int ChangeUp = lpObj->ChangeUp;

	if(lpObj->Class == CLASS_MG || lpObj->Class == CLASS_DL || lpObj->Class == CLASS_RF)
	{
		ChangeUp++;
	}

	if(ChangeUp >= gServerInfo.m_CommandChangeLimit[lpObj->AccountLevel])
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(83));
		return;
	}

	GCMoneySend(lpObj->Index,lpObj->Money);

	for(int n=0;n <= ((ChangeUp==0)?3:6);n++)
	{
		if(gQuest.CheckQuestListState(lpObj,n,QUEST_FINISH) == 0)
		{
			if(n != 3 || lpObj->Class == CLASS_DK)
			{
				gQuest.AddQuestList(lpObj,n,QUEST_ACCEPT);
				gQuestReward.InsertQuestReward(lpObj,n);
				gQuest.AddQuestList(lpObj,n,QUEST_FINISH);
			}
		}
	}

	lpObj->SendQuestInfo = 0;

	gQuest.GCQuestInfoSend(lpObj->Index);

	gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(84));

	gLog.Output(LOG_COMMAND,"[CommandChange][%s][%s] - (ChangeUp: %d)",lpObj->Account,lpObj->Name,ChangeUp);
}

void CCommandManager::CommandWare(LPOBJ lpObj,char* arg) // OK
{
	/*if(gServerInfo.m_CommandWareSwitch == 0)
	{
		return;
	}

	if(gServerInfo.m_CommandWareEnable[lpObj->AccountLevel] == 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(85));
		return;
	}

	if(lpObj->Interface.use != 0 || lpObj->State == OBJECT_DELCMD || lpObj->DieRegen != 0 || lpObj->Teleport != 0 || lpObj->LoadWarehouse != 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(86));
		return;
	}

	int number = this->GetNumber(arg,0);

	if(number < 0 || number >= gServerInfo.m_CommandWareNumber[lpObj->AccountLevel])
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(87));
		return;
	}

	lpObj->WarehouseMoney = 0;

	memset(lpObj->WarehouseMap,0xFF,WAREHOUSE_SIZE);

	for(int n=0;n < WAREHOUSE_SIZE;n++)
	{
		lpObj->Warehouse[n].Clear();
	}

	lpObj->Interface.use = 0;

	lpObj->Interface.type = INTERFACE_NONE;

	lpObj->Interface.state = 0;

	lpObj->LoadWarehouse = 0;

	lpObj->WarehouseNumber = number;

	gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(88),number);

	gLog.Output(LOG_COMMAND,"[CommandWare][%s][%s] - (Number: %d)",lpObj->Account,lpObj->Name,number);*/
}

void CCommandManager::CommandReset(LPOBJ lpObj,char* arg) // OK
{
	if(gServerInfo.m_CommandResetSwitch == 0)
	{
		return;
	}

	if(gServerInfo.m_CommandResetEnable[lpObj->AccountLevel] == 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(89));
		return;
	}

	char mode[5] = {0};

	this->GetString(arg,mode,sizeof(mode),0);

	if(strcmp(mode,"auto") == 0)
	{
		this->CommandResetAuto(lpObj,arg);
		return;
	}

	if(lpObj->Interface.use != 0 || lpObj->State == OBJECT_DELCMD || lpObj->DieRegen != 0 || lpObj->Teleport != 0 || lpObj->PShopOpen != 0 || lpObj->SkillSummonPartyTime != 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(90));
		return;
	}

	if(((gMasterSkillTree.CheckMasterLevel(lpObj)==0)?lpObj->Level:(lpObj->Level+lpObj->MasterLevel)) < gResetTable.GetResetLevel(lpObj))
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(91),gResetTable.GetResetLevel(lpObj));
		return;
	}

	if(lpObj->Money < ((DWORD)gResetTable.GetResetMoney(lpObj)))
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(92),gResetTable.GetResetMoney(lpObj));
		return;
	}

	if(lpObj->Reset >= gServerInfo.m_CommandResetLimit[lpObj->AccountLevel])
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(93),gServerInfo.m_CommandResetLimit[lpObj->AccountLevel]);
		return;
	}

	if (gServerInfo.m_CommandResetNeedItem > 0)
	{
		lpObj->ChaosLock = 1;
		int count = gItemManager.GetInventoryItemCount(lpObj,gServerInfo.m_CommandResetItemID, gServerInfo.m_CommandResetItemLevel);
		
		int ItemCount = 0;
		if (lpObj->MasterReset == 0)
		{
			ItemCount = gServerInfo.m_CommandResetItemCount[lpObj->AccountLevel] * (lpObj->Reset + 1);
		}
		else
		{
			ItemCount = gServerInfo.m_CommandResetItemCount[lpObj->AccountLevel] * (lpObj->Reset + 1) + gServerInfo.m_CommandResetItemGR * lpObj->MasterReset;
		}

		if (count < ItemCount)
		{
			ITEM_INFO ItemInfo;
			gItemManager.GetInfo(gServerInfo.m_CommandResetItemID, &ItemInfo);
			gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(499), ItemInfo.Name);
			lpObj->ChaosLock = 0;
			return;
		}

		//gItemManager.DeleteInventoryItemCount(lpObj,gServerInfo.m_CommandResetItemID,gServerInfo.m_CommandResetItemLevel,gServerInfo.m_CommandResetItemCount[lpObj->AccountLevel]);

		lpObj->ChaosLock = 0;
	}

	if(gServerInfo.m_CommandResetCheckItem[lpObj->AccountLevel] != 0)
	{
		for(int n=0;n < INVENTORY_WEAR_SIZE;n++)
		{
			if(lpObj->Inventory[n].IsItem() != 0)
			{
				gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(94));
				return;
			}
		}
	}

	if(((lpObj->CommandManagerTransaction[0]==0)?(lpObj->CommandManagerTransaction[0]++):lpObj->CommandManagerTransaction[0]) != 0)
	{
		return;
	}

	SDHP_COMMAND_RESET_SEND pMsg;

	pMsg.header.set(0x0F,0x00,sizeof(pMsg));

	pMsg.index = lpObj->Index;

	memcpy(pMsg.account,lpObj->Account,sizeof(pMsg.account));

	memcpy(pMsg.name,lpObj->Name,sizeof(pMsg.name));

	gDataServerConnection.DataSend((BYTE*)&pMsg,pMsg.header.size);
}

void CCommandManager::CommandResetAuto(LPOBJ lpObj,char* arg) // OK
{
	if(gServerInfo.m_CommandResetAutoEnable[lpObj->AccountLevel] == 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(89));
		return;
	}

	if(lpObj->AutoResetEnable == 0)
	{
		lpObj->AutoResetEnable = 1;

		lpObj->AutoResetStats[0] = this->GetNumber(arg,1);

		lpObj->AutoResetStats[1] = this->GetNumber(arg,2);

		lpObj->AutoResetStats[2] = this->GetNumber(arg,3);

		lpObj->AutoResetStats[3] = this->GetNumber(arg,4);

		lpObj->AutoResetStats[4] = ((lpObj->Class==CLASS_DL)?this->GetNumber(arg,5):0);

		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(116));
	}
	else
	{
		lpObj->AutoResetEnable = 0;

		lpObj->AutoResetStats[0] = 0;

		lpObj->AutoResetStats[1] = 0;

		lpObj->AutoResetStats[2] = 0;

		lpObj->AutoResetStats[3] = 0;

		lpObj->AutoResetStats[4] = 0;

		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(117));
	}
}

void CCommandManager::CommandGMMove(LPOBJ lpObj,char* arg) // OK
{
	if(gServerInfo.m_CommandGMMoveSwitch == 0)
	{
		return;
	}

	if(gGameMaster.CheckGameMasterLevel(lpObj,gServerInfo.m_CommandGMMoveGameMasterLevel) == 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(96));
		return;
	}

	char name[11] = {0};

	this->GetString(arg,name,sizeof(name),0);

	int map = this->GetNumber(arg,1);

	int x = this->GetNumber(arg,2);

	int y = this->GetNumber(arg,3);

	LPOBJ lpTarget = gObjFind(name);

	if(lpTarget == 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(97));
		return;
	}

	gObjTeleport(lpTarget->Index,map,x,y);

	gLog.Output(LOG_COMMAND,"[CommandGMMove][%s][%s] - (Name: %s, Map: %d, X: %d, Y: %d)",lpObj->Account,lpObj->Name,name,map,x,y);
}

void CCommandManager::CommandGMPost(LPOBJ lpObj,char* arg) // OK
{
	if(gServerInfo.m_CommandGMPostSwitch == 0)
	{
		return;
	}

	if(gGameMaster.CheckGameMasterLevel(lpObj,gServerInfo.m_CommandGMPostGameMasterLevel) == 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(98));
		return;
	}

	gNotice.GCNoticeSendToAll(0,0,0,0,0,0,"[%s] %s",lpObj->Name,arg);

	gLog.Output(LOG_COMMAND,"[CommandGMPost][%s][%s] - (Message: %s)",lpObj->Account,lpObj->Name,arg);

	gLog.Output(LOG_COMMAND,"(Message: %s) %x %x %x",arg,arg[0],arg[1],arg[2]);
}

void CCommandManager::CommandTrack(LPOBJ lpObj,char* arg) // OK
{
	if(gServerInfo.m_CommandTrackSwitch == 0)
	{
		return;
	}

	if(gGameMaster.CheckGameMasterLevel(lpObj,gServerInfo.m_CommandTrackGameMasterLevel) == 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(99));
		return;
	}

	char name[11] = {0};

	this->GetString(arg,name,sizeof(name),0);

	LPOBJ lpTarget = gObjFind(name);

	if(lpTarget == 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(100));
		return;
	}

	gObjTeleport(lpObj->Index,lpTarget->Map,lpTarget->X,lpTarget->Y);

	gLog.Output(LOG_COMMAND,"[CommandTrack][%s][%s] - (Name: %s)",lpObj->Account,lpObj->Name,name);
}

void CCommandManager::CommandTrace(LPOBJ lpObj,char* arg) // OK
{
	if(gServerInfo.m_CommandTraceSwitch == 0)
	{
		return;
	}

	if(gGameMaster.CheckGameMasterLevel(lpObj,gServerInfo.m_CommandTraceGameMasterLevel) == 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(101));
		return;
	}

	char name[11] = {0};

	this->GetString(arg,name,sizeof(name),0);

	LPOBJ lpTarget = gObjFind(name);

	if(lpTarget == 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(102));
		return;
	}

	gObjTeleport(lpTarget->Index,lpObj->Map,lpObj->X,lpObj->Y);

	gLog.Output(LOG_COMMAND,"[CommandTrace][%s][%s] - (Name: %s)",lpObj->Account,lpObj->Name,name);
}

void CCommandManager::CommandDisconnect(LPOBJ lpObj,char* arg) // OK
{
	if(gServerInfo.m_CommandDisconnectSwitch == 0)
	{
		return;
	}
	if(gGameMaster.CheckGameMasterLevel(lpObj,gServerInfo.m_CommandDisconnectGameMasterLevel) == 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(103));
		return;
	}
	char name[11] = {0};

	this->GetString(arg,name,sizeof(name),0);

	LPOBJ lpTarget = gObjFind(name);

	if(lpTarget == 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(104));
		return;
	}
	
	if (lpTarget->AttackCustomOffline != 0 || lpTarget->PShopCustomOffline != 0)
	{
		/*lpTarget->AttackCustomOffline = 0;
		lpTarget->PShopCustomOffline = 0;*/
		gObjDelOffExp(lpTarget->Index);	
	}
	else
	{
		gObjUserKill(lpTarget->Index);
	}
	

	gLog.Output(LOG_COMMAND,"[CommandDisconnect][%s][%s] - (Name: %s)",lpObj->Account,lpObj->Name,name);
}

void CCommandManager::CommandFireworks(LPOBJ lpObj,char* arg) // OK
{
	if(gServerInfo.m_CommandFireworksSwitch == 0)
	{
		return;
	}

	if(gGameMaster.CheckGameMasterLevel(lpObj,gServerInfo.m_CommandFireworksGameMasterLevel) == 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(105));
		return;
	}

	for(int n=0;n < 15;n++)
	{
		GCFireworksSend(lpObj,(lpObj->X+(((GetLargeRand()%5)*2)-4)),(lpObj->Y+(((GetLargeRand()%5)*2)-4)));
	}

	gLog.Output(LOG_COMMAND,"[CommandFireworks][%s][%s] - (Map: %d, X: %d, Y: %d)",lpObj->Account,lpObj->Name,lpObj->Map,lpObj->X,lpObj->Y);
}

void CCommandManager::CommandMake(LPOBJ lpObj,char* arg) // OK
{
	if(gServerInfo.m_CommandMakeSwitch == 0)
	{
		return;
	}

	if(gGameMaster.CheckGameMasterLevel(lpObj,gServerInfo.m_CommandMakeGameMasterLevel) == 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(106));
		return;
	}
	//LogAdd(LOG_RED, "type %d state %d use %d", lpObj->Interface.type, lpObj->Interface.state, lpObj->Interface.use);
	int count = this->GetNumber(arg, 0);
	int section = this->GetNumber(arg,1);
	int type = this->GetNumber(arg,2);
	int level = this->GetNumber(arg,3);
	int skill = this->GetNumber(arg,4);
	int luck = this->GetNumber(arg,5);
	int option = this->GetNumber(arg,6);
	int exc = this->GetNumber(arg,7);
	int set = this->GetNumber(arg,8);

	//lpObj->BankEx.Credits += 100;

	int dur = gItemManager.GetItemDurability(GET_ITEM(section,type),level,exc,set);

	for (int i=0; i < count; i++)
	{
		if (section == -1)
		{
			for (int i=7;i<12;i++)
			{
				GDCreateItemSend(lpObj->Index,0xEB,0,0,GET_ITEM(i,type),level,dur,skill,luck,option,-1,exc,set,0,0,0,0xFF,0);
			}
		}
		else
		{
			GDCreateItemSend(lpObj->Index,0xEB,0,0,GET_ITEM(section,type),level,dur,skill,luck,option,-1,exc,set,0,0,0,0xFF,0);
		}
	}

	gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(107));

	gLog.Output(LOG_COMMAND,"[CommandMake][%s][%s] - (Count: %d, Section: %d, Type: %d, Level: %d, Skill: %d, Luck: %d, Option: %d, Exc: %d, Set: %d)",lpObj->Account,lpObj->Name,count,section,type,level,skill,luck,option,exc,set);
}

void CCommandManager::CommandDrop(LPOBJ lpObj, char* arg)
{
	if(gServerInfo.m_CommandDropSwitch == 0)
	{
		return;
	}

	if(gGameMaster.CheckGameMasterLevel(lpObj,gServerInfo.m_CommandDropGameMasterLevel) == 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(106));
		return;
	}

	//int count = this->GetNumber(arg, 0);
	int section = this->GetNumber(arg,0);
	int type = this->GetNumber(arg,1);
	int level = this->GetNumber(arg,2);
	int skill = this->GetNumber(arg,3);
	int luck = this->GetNumber(arg,4);
	int option = this->GetNumber(arg,5);
	int exc = this->GetNumber(arg,6);
	int set = this->GetNumber(arg,7);

	//for (int i=0; i < count; i++)
	//{
	GDCreateItemSend(lpObj->Index,lpObj->Map,lpObj->X,lpObj->Y,GET_ITEM(section,type),level,0,skill,luck,option,-1,exc,set,0,0,0,0xFF,0);
	//}

	gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(107));

	gLog.Output(LOG_COMMAND,"[CommandDrop][%s][%s] - (Section: %d, Type: %d, Level: %d, Skill: %d, Luck: %d, Option: %d, Exc: %d, Set: %d)",lpObj->Account,lpObj->Name,section,type,level,skill,luck,option,exc,set);
}

void CCommandManager::CommandSkin(LPOBJ lpObj,char* arg) // OK
{
	if(gServerInfo.m_CommandSkinSwitch == 0)
	{
		return;
	}

	if(gGameMaster.CheckGameMasterLevel(lpObj,gServerInfo.m_CommandSkinGameMasterLevel) == 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(108));
		return;
	}

	char name[11] = {0};

	this->GetString(arg,name,sizeof(name),0);

	LPOBJ lpTarget = gObjFind(name);

	if(lpTarget == 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(109));
		return;
	}
	
	lpTarget->Change = this->GetNumber(arg,1);

	gObjViewportListProtocolCreate(lpTarget);

	gObjectManager.CharacterUpdateMapEffect(lpTarget);

	gLog.Output(LOG_COMMAND,"[CommandSkin][%s][%s] - (Name: %s, Change: %d)",lpObj->Account,lpObj->Name,name,lpTarget->Change);
}

void CCommandManager::CommandSetMoney(LPOBJ lpObj,char* arg) // OK
{
	if(gServerInfo.m_CommandSetMoneySwitch == 0)
	{
		return;
	}

	if(gGameMaster.CheckGameMasterLevel(lpObj,gServerInfo.m_CommandSetMoneyGameMasterLevel) == 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(110));
		return;
	}

	char name[11] = {0};

	this->GetString(arg,name,sizeof(name),0);

	LPOBJ lpTarget = gObjFind(name);

	if(lpTarget == 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(111));
		return;
	}

	int money = this->GetNumber(arg,1);

	if(money < 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(112));
		return;
	}

	if(money == 0)
	{
		lpTarget->Money = 0;
	}
	else if(gObjCheckMaxMoney(lpTarget->Index,money) == 0)
	{
		lpTarget->Money = MAX_MONEY;
	}
	else
	{
		lpTarget->Money += money;
	}

	GCMoneySend(lpTarget->Index,lpTarget->Money);

	gLog.Output(LOG_COMMAND,"[CommandSetMoney][%s][%s] - (Name: %s, Money: %d)",lpObj->Account,lpObj->Name,name,money);
}


void CCommandManager::CommandChatBan(LPOBJ lpObj, char* arg)
{
	if(gServerInfo.m_CommandBanChatSwitch == 0)
	{
		return;
	}

	if(gGameMaster.CheckGameMasterLevel(lpObj,gServerInfo.m_CommandBanChatGameMasterLevel) == 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,"You do not have permission to use this command!");
		return;
	}

	/*if ((lpObj->AuthorityCode & 0x20 ) != 0x20)
	{
		return;
	}*/

	char name[11] = {0};

	this->GetString(arg,name,sizeof(name),0);

	int BanTime = this->GetNumber(arg,1);

	LPOBJ lpTarget = gObjFind(name);

	if(lpTarget == 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,"You have entered an invalid target");
		return;
	}

	lpTarget->ChatLimitTime = BanTime;

	gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, "%s get %d minutes(s) of chat block", lpTarget->Name, BanTime);
	gNotice.GCNoticeSend(lpTarget->Index, 1, 0, 0, 0, 0, 0, "Your chat banned for %d minutes", BanTime);

	gLog.Output(LOG_COMMAND,"[CommandBanChat][%s][%s] - (Name: %s, BanTime: %d minutes)",lpObj->Account,lpObj->Name,name,BanTime);
}

void CCommandManager::CommandChatUnBan(LPOBJ lpObj, char* arg)
{
	if(gServerInfo.m_CommandBanChatSwitch == 0)
	{
		return;
	}

	if(gGameMaster.CheckGameMasterLevel(lpObj,gServerInfo.m_CommandBanChatGameMasterLevel) == 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,"You do not have permission to use this command!");
		return;
	}
	/*if ((lpObj->AuthorityCode & 0x20 ) != 0x20)
	{
		return;
	}*/

	char name[11] = {0};

	this->GetString(arg,name,sizeof(name),0);

	LPOBJ lpTarget = gObjFind(name);

	if(lpTarget == 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,"You have entered an invalid target");
		return;
	}

	lpTarget->ChatLimitTime = 0;

	gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, "%s chat successfully unbanned", lpTarget->Name);
	gNotice.GCNoticeSend(lpTarget->Index, 1, 0, 0, 0, 0, 0, "Your chat unbanned");

	gLog.Output(LOG_COMMAND,"[CommandBanChat][%s][%s] - (Name: %s)",lpObj->Account,lpObj->Name,name);
}

void CCommandManager::CommandNotice(LPOBJ lpObj,char* arg) // OK
{
	if(gServerInfo.m_CommandNoticeSwitch == 0)
	{
		return;
	}

	if(gGameMaster.CheckGameMasterLevel(lpObj,gServerInfo.m_CommandNoticeGameMasterLevel) == 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(113));
		return;
	}

	GDGlobalNoticeSend(gMapServerManager.GetMapServerGroup(),0,0,0,0,0,0,arg);

	gLog.Output(LOG_COMMAND,"[CommandNotice][%s][%s] - (Message: %s)",lpObj->Account,lpObj->Name,arg);
}

void CCommandManager::CommandSetCretits(LPOBJ lpObj, char* arg)
{
	char name[11] = {0};

	this->GetString(arg,name,sizeof(name),0);

	int CreditsValue = this->GetNumber(arg,1);

	LPOBJ lpTarget = gObjFind(name);

	if(lpTarget == 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,"You have entered an invalid target");
		return;
	}

	if (CreditsValue == 0)
	{
		lpTarget->BankEx.Credits = 0;
	}
	else
	{
		lpTarget->BankEx.Credits += CreditsValue;
	}

	gNotice.GCNoticeSend(lpTarget->Index, 1, 0, 0, 0, 0, 0, "You got %d %s! Balance: %d", CreditsValue,gMessage.GetMessage(527), lpTarget->BankEx.Credits);
	gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, "%s successfully get %d %s", lpTarget->Name, CreditsValue,gMessage.GetMessage(527));

	gBankEx.GCUpdateBankEx(lpTarget->Index);

	/*lpTarget->ChatLimitTime = BanTime;

	gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, "%s get %d minutes(s) of chat block", lpTarget->Name, BanTime);
	gNotice.GCNoticeSend(lpTarget->Index, 1, 0, 0, 0, 0, 0, "Your chat banned for %d minutes", BanTime);*/

	gLog.Output(LOG_COMMAND,"[CommandSetMoney][%s][%s] - (Name: %s, Credits: %d)",lpObj->Account,lpObj->Name,name,CreditsValue);
}

void CCommandManager::CommandGuildWar(LPOBJ lpObj,char* arg) // OK
{
	char GuildName[9] = {0};

	this->GetString(arg,GuildName,sizeof(GuildName),0);

	if(strlen(GuildName) >= 1)
	{
		GCGuildWarRequestResult(GuildName,lpObj->Index,0);
	}
}

void CCommandManager::CommandBattleSoccer(LPOBJ lpObj,char* arg) // OK
{
	char GuildName[9] = {0};

	this->GetString(arg,GuildName,sizeof(GuildName),0);

	if(strlen(GuildName) >= 1)
	{
		GCGuildWarRequestResult(GuildName,lpObj->Index,1);
	}
}

void CCommandManager::CommandRequest(LPOBJ lpObj,char* arg) // OK
{
	char mode[5] = {0};

	this->GetString(arg,mode,sizeof(mode),0);

	if(strcmp(mode,"on") == 0)
	{
		lpObj->Option |= 1;
		lpObj->Option |= 2;
		lpObj->Option &= ~4;

		memset(lpObj->AutoPartyPassword,0,sizeof(lpObj->AutoPartyPassword));

		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(126));
	}
	else if(strcmp(mode,"off") == 0)
	{
		lpObj->Option &= ~1;
		lpObj->Option &= ~2;
		lpObj->Option &= ~4;

		memset(lpObj->AutoPartyPassword,0,sizeof(lpObj->AutoPartyPassword));

		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(127));
	}
	else if(strcmp(mode,"auto") == 0)
	{
		lpObj->Option |= 1;
		lpObj->Option |= 2;
		lpObj->Option |= 4;

		memset(lpObj->AutoPartyPassword,0,sizeof(lpObj->AutoPartyPassword));

		this->GetString(arg,lpObj->AutoPartyPassword,sizeof(lpObj->AutoPartyPassword),1);

		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(128));
	}
}

void CCommandManager::CommandHide(LPOBJ lpObj,char* arg) // OK
{
	if(gServerInfo.m_CommandHideSwitch == 0)
	{
		return;
	}

	if(gGameMaster.CheckGameMasterLevel(lpObj,gServerInfo.m_CommandHideGameMasterLevel) == 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(135));
		return;
	}

	if(gEffectManager.CheckEffect(lpObj,EFFECT_INVISIBILITY) == 0)
	{
		gEffectManager.AddEffect(lpObj,0,EFFECT_INVISIBILITY,0,0,0,0,0);

		gObjViewportListProtocolDestroy(lpObj);

		gLog.Output(LOG_COMMAND,"[CommandHide][%s][%s] - (State: %d)",lpObj->Account,lpObj->Name,1);
	}
	else
	{
		gEffectManager.DelEffect(lpObj,EFFECT_INVISIBILITY);

		gObjViewportListProtocolCreate(lpObj);

		gLog.Output(LOG_COMMAND,"[CommandHide][%s][%s] - (State: %d)",lpObj->Account,lpObj->Name,0);
	}
}

void CCommandManager::CommandAddPointAutoProc(LPOBJ lpObj) // OK
{
	if(lpObj->LevelUpPoint == 0 || lpObj->AutoAddPointCount == 0)
	{
		return;
	}

	int AddStatCount = lpObj->AutoAddPointCount;

	int* stat[6] = {&lpObj->Strength,&lpObj->Dexterity,&lpObj->Vitality,&lpObj->Energy,&lpObj->Leadership,&lpObj->NewStat};

	for(int n=0;n < 6;n++)
	{
		if(lpObj->AutoAddPointStats[n] > 0)
		{
			if(AddStatCount == 0)
			{
				break;
			}

			int AddStat = lpObj->LevelUpPoint/AddStatCount;

			AddStat = (((lpObj->LevelUpPoint%AddStatCount)>0)?(AddStat+1):AddStat);

			AddStat = ((AddStat>lpObj->AutoAddPointStats[n])?lpObj->AutoAddPointStats[n]:AddStat);

			AddStat = ((((*stat[n])+AddStat)>gServerInfo.m_MaxStatPoint[lpObj->AccountLevel])?(gServerInfo.m_MaxStatPoint[lpObj->AccountLevel]-(*stat[n])):AddStat);

			AddStatCount--;

			(*stat[n]) += AddStat;

			lpObj->LevelUpPoint -= AddStat;

			lpObj->AutoAddPointStats[n] -= (((*stat[n])>=gServerInfo.m_MaxStatPoint[lpObj->AccountLevel])?lpObj->AutoAddPointStats[n]:AddStat);

			lpObj->AutoAddPointCount = 0;

			lpObj->AutoAddPointCount = ((lpObj->AutoAddPointStats[0]>0)?(lpObj->AutoAddPointCount+1):lpObj->AutoAddPointCount);

			lpObj->AutoAddPointCount = ((lpObj->AutoAddPointStats[1]>0)?(lpObj->AutoAddPointCount+1):lpObj->AutoAddPointCount);

			lpObj->AutoAddPointCount = ((lpObj->AutoAddPointStats[2]>0)?(lpObj->AutoAddPointCount+1):lpObj->AutoAddPointCount);

			lpObj->AutoAddPointCount = ((lpObj->AutoAddPointStats[3]>0)?(lpObj->AutoAddPointCount+1):lpObj->AutoAddPointCount);

			lpObj->AutoAddPointCount = ((lpObj->AutoAddPointStats[4]>0)?(lpObj->AutoAddPointCount+1):lpObj->AutoAddPointCount);
		}
	}

	gObjectManager.CharacterCalcAttribute(lpObj->Index);

	GCNewCharacterInfoSend(lpObj);
}

void CCommandManager::CommandClearInventory(LPOBJ lpObj)
{
	int MaxValue = gItemManager.GetInventoryMaxValue(lpObj);

	for (int i = INVENTORY_WEAR_SIZE; i < MaxValue; i++)
	{
		gItemManager.InventoryDelItem(lpObj->Index,i);
		gItemManager.GCItemDeleteSend(lpObj->Index,i,1);
	}
}

void CCommandManager::CommandDCFriend(LPOBJ lpObj,char* arg)
{
	if(gServerInfo.m_CommandDisconnectSwitch == 0)
	{
		return;
	}

	char Account[11] = {0};

	char Password[11] = {0};

	this->GetString(arg,Account,sizeof(Account),0);
	this->GetString(arg,Password, sizeof(Password), 1);

	LPOBJ lpTarget = gObjFindByAcc(Account);

	if(lpTarget == 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(104));
		return;
	}

	if (strcmp(lpTarget->Password,Password) != 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,"Password incorect!");
		return;
	}

	if (lpTarget->AttackCustomOffline != 0 || lpTarget->PShopCustomOffline != 0)
	{
		//lpTarget->AttackCustomOffline = 0;
		//lpTarget->PShopCustomOffline = 0;
		gObjDelOffExp(lpTarget->Index);
	}
	else
	{
		gObjUserKill(lpTarget->Index);
	}

	//gObjUserKill(lpTarget->Index);

	gLog.Output(LOG_COMMAND,"[CommandDCFriend][%s][%s] - (Account: %s Password: %s)",lpObj->Account,lpObj->Name,Account, Password);
}

void CCommandManager::CommandQuest(LPOBJ lpObj)
{
	if (gProtection.GetCustomState(CUSTOM_QUEST_SYSTEM) == 0)
	{
		return;
	}

	int num = lpObj->iQuestIndex;
			
	if (num < 0 || num > MAX_QUESTS-1)
		return;	
	gCustomQuest.send_message(lpObj->Index, 0, "@[Quest] %s", gCustomQuest.quest_list[num].text);
}

void CCommandManager::CommandPartyLeader(LPOBJ lpObj, char* arg)
{
	char Name[11] = {0};

	this->GetString(arg,Name,sizeof(Name),0);

	LPOBJ lpTarget = gObjFind(Name);

	if(lpTarget == 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(514), Name);
		return;
	}

	gParty.SetLeader(lpObj->Index, lpTarget->Index);
}

void CCommandManager::CommandResetAutoProc(LPOBJ lpObj) // OK
{
	if(lpObj->Interface.use != 0 || lpObj->State == OBJECT_DELCMD || lpObj->DieRegen != 0 || lpObj->Teleport != 0 || lpObj->PShopOpen != 0 || lpObj->SkillSummonPartyTime != 0)
	{
		return;
	}

	if(((gMasterSkillTree.CheckMasterLevel(lpObj)==0)?lpObj->Level:(lpObj->Level+lpObj->MasterLevel)) < gResetTable.GetResetLevel(lpObj))
	{
		return;
	}

	if(lpObj->Money < ((DWORD)gResetTable.GetResetMoney(lpObj)))
	{
		return;
	}

	if(lpObj->Reset >= gServerInfo.m_CommandResetLimit[lpObj->AccountLevel])
	{
		return;
	}

	if(gServerInfo.m_CommandResetCheckItem[lpObj->AccountLevel] != 0)
	{
		for(int n=0;n < INVENTORY_WEAR_SIZE;n++)
		{
			if(lpObj->Inventory[n].IsItem() != 0)
			{
				return;
			}
		}
	}

	if(((lpObj->CommandManagerTransaction[0]==0)?(lpObj->CommandManagerTransaction[0]++):lpObj->CommandManagerTransaction[0]) != 0)
	{
		return;
	}

	SDHP_COMMAND_RESET_SEND pMsg;

	pMsg.header.set(0x0F,0x00,sizeof(pMsg));

	pMsg.index = lpObj->Index;

	memcpy(pMsg.account,lpObj->Account,sizeof(pMsg.account));

	memcpy(pMsg.name,lpObj->Name,sizeof(pMsg.name));

	gDataServerConnection.DataSend((BYTE*)&pMsg,pMsg.header.size);
}

void CCommandManager::DGCommandResetRecv(SDHP_COMMAND_RESET_RECV* lpMsg) // OK
{
	if(gObjIsAccountValid(lpMsg->index,lpMsg->account) == 0)
	{
		LogAdd(LOG_RED,"[DGCommandResetRecv] Invalid Account [%d](%s)",lpMsg->index,lpMsg->account);
		CloseClient(lpMsg->index);
		return;
	}

	LPOBJ lpObj = &gObj[lpMsg->index];

	lpObj->CommandManagerTransaction[0] = 0;

	if(lpMsg->ResetDay >= ((DWORD)gServerInfo.m_CommandResetLimitDay[lpObj->AccountLevel]))
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(129),gServerInfo.m_CommandResetLimitDay[lpObj->AccountLevel]);
		return;
	}

	if(lpMsg->ResetWek >= ((DWORD)gServerInfo.m_CommandResetLimitWek[lpObj->AccountLevel]))
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(130),gServerInfo.m_CommandResetLimitWek[lpObj->AccountLevel]);
		return;
	}

	if(lpMsg->ResetMon >= ((DWORD)gServerInfo.m_CommandResetLimitMon[lpObj->AccountLevel]))
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(131),gServerInfo.m_CommandResetLimitMon[lpObj->AccountLevel]);
		return;
	}

	//lpObj->Money -= gServerInfo.m_CommandResetMoney[lpObj->AccountLevel];
	lpObj->Money -= (DWORD)gResetTable.GetResetMoney(lpObj);

	GCMoneySend(lpObj->Index,lpObj->Money);

	lpObj->BankEx.Credits += gServerInfo.m_CommandResetGiveCredits[lpObj->AccountLevel];

	gBankEx.GDSavePoint(lpObj->Index);

	if (gServerInfo.m_CommandResetNeedItem > 0)
	{
		lpObj->ChaosLock = 1;

		int ItemCount = 0;
		if (lpObj->MasterReset == 0)
		{
			ItemCount = gServerInfo.m_CommandResetItemCount[lpObj->AccountLevel] * (lpObj->Reset + 1);
		}
		else
		{
			ItemCount = gServerInfo.m_CommandResetItemCount[lpObj->AccountLevel] * (lpObj->Reset + 1) + gServerInfo.m_CommandResetItemGR * lpObj->MasterReset;
		}

		gItemManager.DeleteInventoryItemCount(lpObj,gServerInfo.m_CommandResetItemID,gServerInfo.m_CommandResetItemLevel,ItemCount);

		lpObj->ChaosLock = 0;
	}

	lpObj->Level = ((gServerInfo.m_CommandResetStartLevel[lpObj->AccountLevel]==-1)?(lpObj->Level-gServerInfo.m_CommandResetStartLevel[lpObj->AccountLevel]):gServerInfo.m_CommandResetStartLevel[lpObj->AccountLevel]);

	lpObj->Experience = gLevelExperience[lpObj->Level-1];

	lpObj->Reset += gBonusManager.GetBonusValue(lpObj,BONUS_INDEX_RESET_AMOUNT,gServerInfo.m_CommandResetCount[lpObj->AccountLevel],-1,-1,-1,-1);

	lpMsg->ResetDay += gBonusManager.GetBonusValue(lpObj,BONUS_INDEX_RESET_AMOUNT,gServerInfo.m_CommandResetCount[lpObj->AccountLevel],-1,-1,-1,-1);

	lpMsg->ResetWek += gBonusManager.GetBonusValue(lpObj,BONUS_INDEX_RESET_AMOUNT,gServerInfo.m_CommandResetCount[lpObj->AccountLevel],-1,-1,-1,-1);

	lpMsg->ResetMon += gBonusManager.GetBonusValue(lpObj,BONUS_INDEX_RESET_AMOUNT,gServerInfo.m_CommandResetCount[lpObj->AccountLevel],-1,-1,-1,-1);

	lpObj->AutoAddPointCount = 0;

	lpObj->AutoAddPointStats[0] = 0;

	lpObj->AutoAddPointStats[1] = 0;

	lpObj->AutoAddPointStats[2] = 0;

	lpObj->AutoAddPointStats[3] = 0;

	lpObj->AutoAddPointStats[4] = 0;

	if(gServerInfo.m_CommandResetQuest[lpObj->AccountLevel] != 0)
	{
		lpObj->DBClass = ((lpObj->DBClass/16)*16);
		lpObj->ChangeUp = lpObj->DBClass%16;

		gObjectManager.CharacterMakePreviewCharSet(lpObj->Index);

		BYTE Class = (lpObj->ChangeUp*16);
		Class -= (Class/32);
		Class += (lpObj->Class*32);

		gQuest.GCQuestRewardSend(lpObj->Index,201,Class);

		memset(lpObj->Quest,0xFF,sizeof(lpObj->Quest));

		gQuest.GCQuestInfoSend(lpObj->Index);
	}

	if(gServerInfo.m_CommandResetSkill[lpObj->AccountLevel] != 0)
	{
		lpObj->MasterLevel = 0;

		lpObj->MasterPoint = 0;

		for(int n=0;n < MAX_SKILL_LIST;n++)
		{
			lpObj->Skill[n].Clear();
		}

		for(int n=0;n < MAX_MASTER_SKILL_LIST;n++)
		{
			lpObj->MasterSkill[n].Clear();
		}

		gMasterSkillTree.GCMasterSkillListSend(lpObj->Index);

		gSkillManager.GCSkillListSend(lpObj,0);

		gObjectManager.CharacterCalcAttribute(lpObj->Index);

		gMasterSkillTree.GCMasterInfoSend(lpObj);
	}

	if(gServerInfo.m_CommandResetType == 1)
	{
		int point = gResetTable.GetResetPoint(lpObj);

		point = (point*gServerInfo.m_CommandResetPointRate[lpObj->Class])/100;

		point += (lpObj->Level-1)*gServerInfo.m_LevelUpPoint[lpObj->Class];

		point += ((gQuest.CheckQuestListState(lpObj,2,QUEST_FINISH)==0)?0:((lpObj->Level>220)?((lpObj->Level-220)*gServerInfo.m_PlusStatPoint):0));

		point += gQuest.GetQuestRewardLevelUpPoint(lpObj);

		point += lpObj->FruitAddPoint;

		if (gProtection.GetCustomState(CUSTOM_QUEST_SYSTEM) == 1)
		{
			point += lpObj->iQuestStat;
		}
#if (GAMESERVER_UPDATE == 603)
		point += gGRSystem.GetGRPoints(lpObj);
#endif
		lpObj->LevelUpPoint = point;

		lpObj->Strength = gDefaultClassInfo.m_DefaultClassInfo[lpObj->Class].Strength;
		lpObj->Dexterity = gDefaultClassInfo.m_DefaultClassInfo[lpObj->Class].Dexterity;
		lpObj->Vitality = gDefaultClassInfo.m_DefaultClassInfo[lpObj->Class].Vitality;
		lpObj->Energy = gDefaultClassInfo.m_DefaultClassInfo[lpObj->Class].Energy;
		lpObj->Leadership = gDefaultClassInfo.m_DefaultClassInfo[lpObj->Class].Leadership;
		lpObj->NewStat = gDefaultClassInfo.m_DefaultClassInfo[lpObj->Class].NewStat;
	}

	/*if(gServerInfo.m_CommandMasterResetSwitch != 0 && gServerInfo.m_CommandMasterResetType == 1)
	{
		int point = 0;

		point = gServerInfo.m_CommandMasterResetPoint[lpObj->AccountLevel]*lpObj->MasterReset;

		point = (point*gServerInfo.m_CommandMasterResetPointRate[lpObj->Class])/100;

		lpObj->LevelUpPoint += point;
	}*/

	gObjectManager.CharacterCalcAttribute(lpObj->Index);

	GCNewCharacterInfoSend(lpObj);

	GDCharacterInfoSaveSend(lpObj->Index);

	GDResetInfoSaveSend(lpObj->Index,lpMsg->ResetDay,lpMsg->ResetWek,lpMsg->ResetMon);

	gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(95),lpObj->Reset);

	if(gServerInfo.m_CommandResetMove[lpObj->AccountLevel] != 0)
	{
		gObjMoveGate(lpObj->Index, gDefaultClassInfo.m_DefaultClassInfo[lpObj->Class].StartGate);
		/*switch(lpObj->Class)
		{
			case CLASS_DW:
				gObjMoveGate(lpObj->Index,17);
				break;
			case CLASS_DK:
				gObjMoveGate(lpObj->Index,17);
				break;
			case CLASS_FE:
				gObjMoveGate(lpObj->Index,27);
				break;
			case CLASS_MG:
				gObjMoveGate(lpObj->Index,17);
				break;
			case CLASS_DL:
				gObjMoveGate(lpObj->Index,17);
				break;
			case CLASS_SU:
				gObjMoveGate(lpObj->Index,267);
				break;
			case CLASS_RF:
				gObjMoveGate(lpObj->Index,17);
				break;
		}*/
	}

	lpObj->AutoAddPointStats[0] = lpObj->AutoResetStats[0];

	lpObj->AutoAddPointStats[1] = lpObj->AutoResetStats[1];

	lpObj->AutoAddPointStats[2] = lpObj->AutoResetStats[2];

	lpObj->AutoAddPointStats[3] = lpObj->AutoResetStats[3];

	lpObj->AutoAddPointStats[4] = lpObj->AutoResetStats[4];

	lpObj->AutoAddPointCount = ((lpObj->AutoAddPointStats[0]>0)?(lpObj->AutoAddPointCount+1):lpObj->AutoAddPointCount);

	lpObj->AutoAddPointCount = ((lpObj->AutoAddPointStats[1]>0)?(lpObj->AutoAddPointCount+1):lpObj->AutoAddPointCount);

	lpObj->AutoAddPointCount = ((lpObj->AutoAddPointStats[2]>0)?(lpObj->AutoAddPointCount+1):lpObj->AutoAddPointCount);

	lpObj->AutoAddPointCount = ((lpObj->AutoAddPointStats[3]>0)?(lpObj->AutoAddPointCount+1):lpObj->AutoAddPointCount);

	lpObj->AutoAddPointCount = ((lpObj->AutoAddPointStats[4]>0)?(lpObj->AutoAddPointCount+1):lpObj->AutoAddPointCount);

	this->CommandAddPointAutoProc(lpObj);

	gLog.Output(LOG_COMMAND,"[CommandReset][%s][%s] - (Reset: %d)",lpObj->Account,lpObj->Name,lpObj->Reset);
}

//void CCommandManager::DGCommandMasterResetRecv(SDHP_COMMAND_MASTER_RESET_RECV* lpMsg) // OK
//{
//	if(gObjIsAccountValid(lpMsg->index,lpMsg->account) == 0)
//	{
//		LogAdd(LOG_RED,"[DGCommandMasterResetRecv] Invalid Account [%d](%s)",lpMsg->index,lpMsg->account);
//		CloseClient(lpMsg->index);
//		return;
//	}
//
//	LPOBJ lpObj = &gObj[lpMsg->index];
//
//	lpObj->CommandManagerTransaction[0] = 0;
//
//	if(lpMsg->MasterResetDay >= ((DWORD)gServerInfo.m_CommandMasterResetLimitDay[lpObj->AccountLevel]))
//	{
//		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(132),gServerInfo.m_CommandMasterResetLimitDay[lpObj->AccountLevel]);
//		return;
//	}
//
//	if(lpMsg->MasterResetWek >= ((DWORD)gServerInfo.m_CommandMasterResetLimitWek[lpObj->AccountLevel]))
//	{
//		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(133),gServerInfo.m_CommandMasterResetLimitWek[lpObj->AccountLevel]);
//		return;
//	}
//
//	if(lpMsg->MasterResetMon >= ((DWORD)gServerInfo.m_CommandMasterResetLimitMon[lpObj->AccountLevel]))
//	{
//		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(134),gServerInfo.m_CommandMasterResetLimitMon[lpObj->AccountLevel]);
//		return;
//	}
//
//	lpObj->Money -= gServerInfo.m_CommandMasterResetMoney[lpObj->AccountLevel];
//
//	GCMoneySend(lpObj->Index,lpObj->Money);
//
//	lpObj->Level = ((gServerInfo.m_CommandMasterResetStartLevel[lpObj->AccountLevel]==-1)?(lpObj->Level-gServerInfo.m_CommandMasterResetStartLevel[lpObj->AccountLevel]):gServerInfo.m_CommandMasterResetStartLevel[lpObj->AccountLevel]);
//
//	lpObj->Experience = gLevelExperience[lpObj->Level-1];
//
//	lpObj->Reset = ((gServerInfo.m_CommandMasterResetStartReset[lpObj->AccountLevel]==-1)?(lpObj->Reset-gServerInfo.m_CommandMasterResetReset[lpObj->AccountLevel]):gServerInfo.m_CommandMasterResetStartReset[lpObj->AccountLevel]);
//
//	lpObj->MasterReset += gBonusManager.GetBonusValue(lpObj,BONUS_INDEX_MASTER_RESET_AMOUNT,gServerInfo.m_CommandMasterResetCount[lpObj->AccountLevel],-1,-1,-1,-1);
//
//	lpMsg->MasterResetDay += gBonusManager.GetBonusValue(lpObj,BONUS_INDEX_MASTER_RESET_AMOUNT,gServerInfo.m_CommandMasterResetCount[lpObj->AccountLevel],-1,-1,-1,-1);
//
//	lpMsg->MasterResetWek += gBonusManager.GetBonusValue(lpObj,BONUS_INDEX_MASTER_RESET_AMOUNT,gServerInfo.m_CommandMasterResetCount[lpObj->AccountLevel],-1,-1,-1,-1);
//
//	lpMsg->MasterResetMon += gBonusManager.GetBonusValue(lpObj,BONUS_INDEX_MASTER_RESET_AMOUNT,gServerInfo.m_CommandMasterResetCount[lpObj->AccountLevel],-1,-1,-1,-1);
//
//	if(gServerInfo.m_CommandMasterResetQuest[lpObj->AccountLevel] != 0)
//	{
//		lpObj->DBClass = ((lpObj->DBClass/16)*16);
//		lpObj->ChangeUp = lpObj->DBClass%16;
//
//		gObjectManager.CharacterMakePreviewCharSet(lpObj->Index);
//
//		BYTE Class = (lpObj->ChangeUp*16);
//		Class -= (Class/32);
//		Class += (lpObj->Class*32);
//
//		gQuest.GCQuestRewardSend(lpObj->Index,201,Class);
//
//		memset(lpObj->Quest,0xFF,sizeof(lpObj->Quest));
//
//		gQuest.GCQuestInfoSend(lpObj->Index);
//	}
//
//	if(gServerInfo.m_CommandMasterResetSkill[lpObj->AccountLevel] != 0)
//	{
//		lpObj->MasterLevel = 0;
//
//		lpObj->MasterPoint = 0;
//
//		for(int n=0;n < MAX_SKILL_LIST;n++)
//		{
//			lpObj->Skill[n].Clear();
//		}
//
//		for(int n=0;n < MAX_MASTER_SKILL_LIST;n++)
//		{
//			lpObj->MasterSkill[n].Clear();
//		}
//
//		gMasterSkillTree.GCMasterSkillListSend(lpObj->Index);
//
//		gSkillManager.GCSkillListSend(lpObj,0);
//
//		gObjectManager.CharacterCalcAttribute(lpObj->Index);
//
//		gMasterSkillTree.GCMasterInfoSend(lpObj);
//	}
//
//	if(gServerInfo.m_CommandMasterResetType == 1)
//	{
//		int point = 0;
//
//		point = gServerInfo.m_CommandMasterResetPoint[lpObj->AccountLevel]*lpObj->MasterReset;
//
//		point = (point*gServerInfo.m_CommandMasterResetPointRate[lpObj->Class])/100;
//
//		point += (lpObj->Level-1)*gServerInfo.m_LevelUpPoint[lpObj->Class];
//
//		point += ((gQuest.CheckQuestListState(lpObj,2,QUEST_FINISH)==0)?0:((lpObj->Level>220)?((lpObj->Level-220)*gServerInfo.m_PlusStatPoint):0));
//
//		point += gQuest.GetQuestRewardLevelUpPoint(lpObj);
//
//		point += lpObj->FruitAddPoint;
//
//		point += lpObj->iQuestStat;
//
//		lpObj->LevelUpPoint = point;
//
//		lpObj->Strength = gDefaultClassInfo.m_DefaultClassInfo[lpObj->Class].Strength;
//		lpObj->Dexterity = gDefaultClassInfo.m_DefaultClassInfo[lpObj->Class].Dexterity;
//		lpObj->Vitality = gDefaultClassInfo.m_DefaultClassInfo[lpObj->Class].Vitality;
//		lpObj->Energy = gDefaultClassInfo.m_DefaultClassInfo[lpObj->Class].Energy;
//		lpObj->Leadership = gDefaultClassInfo.m_DefaultClassInfo[lpObj->Class].Leadership;
//	}
//
//	if(gServerInfo.m_CommandResetSwitch != 0 && gServerInfo.m_CommandResetType == 1)
//	{
//		int point = gResetTable.GetResetPoint(lpObj);
//
//		point = (point*gServerInfo.m_CommandResetPointRate[lpObj->Class])/100;
//
//		lpObj->LevelUpPoint += point;
//	}
//
//	gObjectManager.CharacterCalcAttribute(lpObj->Index);
//
//	GCNewCharacterInfoSend(lpObj);
//
//	GDCharacterInfoSaveSend(lpObj->Index);
//
//	GDMasterResetInfoSaveSend(lpObj->Index,lpMsg->MasterResetDay,lpMsg->MasterResetWek,lpMsg->MasterResetMon);
//
//	gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(125),lpObj->MasterReset);
//
//	if(gServerInfo.m_CommandMasterResetMove[lpObj->AccountLevel] != 0)
//	{
//		gObjMoveGate(lpObj->Index, gDefaultClassInfo.m_DefaultClassInfo[lpObj->Class].StartGate);
//		/*switch(lpObj->Class)
//		{
//			case CLASS_DW:
//				gObjMoveGate(lpObj->Index,17);
//				break;
//			case CLASS_DK:
//				gObjMoveGate(lpObj->Index,17);
//				break;
//			case CLASS_FE:
//				gObjMoveGate(lpObj->Index,27);
//				break;
//			case CLASS_MG:
//				gObjMoveGate(lpObj->Index,17);
//				break;
//			case CLASS_DL:
//				gObjMoveGate(lpObj->Index,17);
//				break;
//			case CLASS_SU:
//				gObjMoveGate(lpObj->Index,267);
//				break;
//			case CLASS_RF:
//				gObjMoveGate(lpObj->Index,17);
//				break;
//		}*/
//	}
//
//	gLog.Output(LOG_COMMAND,"[CommandMasterReset][%s][%s] - (MasterReset: %d)",lpObj->Account,lpObj->Name,lpObj->MasterReset);
//}

void CCommandManager::CommandMarryProp(LPOBJ lpObj, char* arg)
{
	char Name[11] = {0};

	this->GetString(arg,Name,sizeof(Name),0);

	LPOBJ lpTarget = gObjFind(Name);

	if(lpTarget == 0)
	{
		return;
	}

	Marry.Propose(lpObj->Index,lpTarget->Index);
}

void CCommandManager::CommandMarryAccept(LPOBJ lpObj, char* arg)
{
	Marry.Accept(lpObj->Index);
}

void CCommandManager::CommandMarryTP(LPOBJ lpObj, char* arg)
{
	if(lpObj->IsMarried == 0)
		return;

	if(strlen(lpObj->MarryName) <= 0)
		return;

	LPOBJ lpTarget = gObjFind(lpObj->MarryName);

	if(lpTarget == 0)
	{
		return;
	}

	if(this->CheckTraceMarryCondition(lpObj, lpTarget) == FALSE)
	{
		return;
	}

	//int nSrcMapNumber = lpObj->Map;
	//int nTarMapNumber = lpTarget->Map;

	gObjTeleport(lpObj->Index, lpTarget->Map, lpTarget->X, lpTarget->Y);
	/*if (nSrcMapNumber != nTarMapNumber)
	{
		g_SendNPCInfo.SendNpcCoordinate(aIndex, nTarMapNumber, 0);
		g_SendNPCInfo.SendPortalCoordinate(aIndex, nTarMapNumber);
	}*/
}

void CCommandManager::CommandMarryDivorce(LPOBJ lpObj, char* arg)
{
	Marry.Divorce(lpObj->Index);
}

void CCommandManager::CommandMonsterCall(LPOBJ lpObj, char* arg)
{ 
	int MobID = this->GetNumber(arg,0);
	int Count = this->GetNumber(arg,1);

//	gMonsterCall.SetMonster(lpObj,MobID,Count);
}

void CCommandManager::CommandExchangeWcoinP(LPOBJ lpObj, char* arg)
{
	int value = this->GetNumber(arg,0);

	gCashShop.GDCashShopRecievePointSend(lpObj->Index,(DWORD)&CCommandManager::CommandExchangeWcoinPCallback,(DWORD)value,0);
}

void CCommandManager::CommandExchangeWcoinPCallback(LPOBJ lpObj, int value, int kek, DWORD WCoinC, DWORD WCoinP, DWORD GoblinPoint)
{
	if (WCoinC < value)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,"You dont have enough %s",gMessage.GetMessage(524));
		return;
	}

	lpObj->BankEx.Credits += value;
	gBankEx.GCUpdateBankEx(lpObj->Index);
	gBankEx.GDSavePoint(lpObj->Index);

	gCashShop.GDCashShopSubPointSaveSend(lpObj->Index,0,value,0,0);

	gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,"You successfully exchanged %d %s to %s",value,gMessage.GetMessage(524),gMessage.GetMessage(527));
}



BOOL CCommandManager::CheckTraceMarryCondition(LPOBJ lpObj, LPOBJ lpTargetObj)
{
	if(lpObj->IsMarried == 0)
	{
		return FALSE;
	}

	if(strlen(lpObj->MarryName) <= 0)
	{
		return FALSE;
	}

	if(lpObj->Level < 200)
	{
		return FALSE;
	}

	if(lpTargetObj->Level < 200)
	{
		return FALSE;
	}


	if(BC_MAP_RANGE(lpTargetObj->Map) || DS_MAP_RANGE(lpTargetObj->Map) || CC_MAP_RANGE(lpTargetObj->Map) || IT_MAP_RANGE(lpTargetObj->Map) ||
		DG_MAP_RANGE(lpTargetObj->Map) || IG_MAP_RANGE(lpTargetObj->Map) || CW_MAP_RANGE(lpTargetObj->Map) || lpTargetObj->Map == MAP_CASTLE_SIEGE ||
		lpTargetObj->Map == MAP_LAND_OF_TRIALS || gMapManager.GetMapGensBattle(lpTargetObj->Map) || lpTargetObj->Map == MAP_KANTURU3 || KALIMA_MAP_RANGE(lpTargetObj->Map) ||
		lpTargetObj->Map == MAP_ACHERON2 || lpTargetObj->Map == MAP_DEVENTER2 )
	{
		return FALSE;
	}

	/*LPMOVE_COMMAND_DATA lpMoveData = gMoveCommand.GetMoveCommandDataByMapNumber(lpTargetObj->MapNumber);

	if ( lpMoveData == NULL )
	{
		return FALSE;
	}

	if (lpMoveData->NeedLevel > lpObj->Level)
	{
		MsgOutput(lpObj->m_Index,Lang.GetText(0,612));
		return FALSE;
	}*/

	MOVE_INFO MoveInfo;

	if(gMove.GetInfo(lpTargetObj->Map,&MoveInfo) == 0)
	{
		return false;
	}

	if (MoveInfo.MinLevel > lpObj->Level || MoveInfo.MinReset > lpObj->Reset || MoveInfo.AccountLevel > lpObj->AccountLevel)
	{
		return FALSE;
	}

	if (MoveInfo.MaxLevel < lpObj->Level || MoveInfo.MaxReset < lpObj->Reset)
	{
		return FALSE;
	}

	if(lpTargetObj->Map == MAP_ICARUS)
	{
		if ( (lpObj->Inventory[7].m_Index != GET_ITEM(13,3) &&
			 lpObj->Inventory[7].m_Index != GET_ITEM(13,30) &&
			 lpObj->Inventory[7].m_Index != GET_ITEM(13,37) &&
			 lpObj->Inventory[7].m_Index != GET_ITEM(12,36) &&//Season 4.5 addon
			 lpObj->Inventory[7].m_Index != GET_ITEM(12,37) &&//Season 4.5 addon
			 lpObj->Inventory[7].m_Index != GET_ITEM(12,38) &&//Season 4.5 addon
			 lpObj->Inventory[7].m_Index != GET_ITEM(12,39) &&//Season 4.5 addon
			 lpObj->Inventory[7].m_Index != GET_ITEM(12,40) &&//Season 4.5 addon
			 lpObj->Inventory[7].m_Index != GET_ITEM(12,41) &&//Season 4.5 addon
			 lpObj->Inventory[7].m_Index != GET_ITEM(12,42) &&//Season 4.5 addon
			 lpObj->Inventory[7].m_Index != GET_ITEM(12,43) &&//Season 4.5 addon
			 lpObj->Inventory[7].m_Index != GET_ITEM(13,4) &&//Season 4.5 addon
			 !(lpObj->Inventory[7].m_Index >= GET_ITEM(12,262) && lpObj->Inventory[7].m_Index <= GET_ITEM(12,265)) &&
			 !(lpObj->Inventory[7].m_Index >= GET_ITEM(12,130) && lpObj->Inventory[7].m_Index <= GET_ITEM(12,135)) &&
			 (lpObj->Inventory[7].m_Index < GET_ITEM(12,0) || lpObj->Inventory[7].m_Index > GET_ITEM(12,6)) ) ||
			 lpObj->Inventory[7].m_Index == GET_ITEM(13,2) ||
			 lpObj->Inventory[11].m_Index == GET_ITEM(13,10) ||
			 lpObj->Inventory[10].m_Index == GET_ITEM(13,10)  )
		{
			return FALSE;
		}
	}

	if (lpObj->PKLevel > 4)
	{
		return FALSE;
	}

	return TRUE;
}

void CCommandManager::CommandMLResetCallback(LPOBJ lpObj,DWORD null,DWORD null1,DWORD WCoinC,DWORD WCoinP,DWORD GoblinPoint)
{
	if(gServerInfo.m_CommandMLResetSwitch == 0)
	{
		return;
	}

	if (gMasterSkillTree.CheckMasterLevel(lpObj) == 0)
	{
		return;
	}

	if( lpObj->Money < gServerInfo.m_CommandMLResetPrice_Zen[lpObj->AccountLevel] )
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,"You need more %s to Reset ML",gMessage.GetMessage(523));
		return;
	}
	//	----
	if( WCoinC < gServerInfo.m_CommandMLResetPrice_WCoinC[lpObj->AccountLevel] )
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,"You need more %s to Reset ML",gMessage.GetMessage(524));
		return;
	}
	//	----
	if( WCoinP < gServerInfo.m_CommandMLResetPrice_WCoinP[lpObj->AccountLevel] )
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,"You need more %s to Reset ML",gMessage.GetMessage(525));
		return;
	}
	//	----
	if( GoblinPoint < gServerInfo.m_CommandMLResetPrice_WCoinG[lpObj->AccountLevel] )
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,"You need more %s to Reset ML",gMessage.GetMessage(526));
		return;
	}
	//	----
	if( lpObj->BankEx.Credits < gServerInfo.m_CommandMLResetPrice_Credits[lpObj->AccountLevel] )
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,"You need more %s to Reset ML",gMessage.GetMessage(527));
		return;
	}

	if (gObjRebuildMasterSkillTree(lpObj) == 0)
	{
		return;
	}

	lpObj->Money -= gServerInfo.m_CommandMLResetPrice_Zen[lpObj->AccountLevel];
	gCashShop.GDCashShopSubPointSaveSend(lpObj->Index,0,gServerInfo.m_CommandMLResetPrice_WCoinC[lpObj->AccountLevel],gServerInfo.m_CommandMLResetPrice_WCoinP[lpObj->AccountLevel],gServerInfo.m_CommandMLResetPrice_WCoinG[lpObj->AccountLevel]);
	lpObj->BankEx.Credits -= gServerInfo.m_CommandMLResetPrice_Credits[lpObj->AccountLevel];
	gBankEx.GCUpdateBankEx(lpObj->Index);
	gBankEx.GDSavePoint(lpObj->Index);
	GCMoneySend(lpObj->Index,lpObj->Money);
}

void CCommandManager::SendZenCustomVault(LPOBJ lpObj,char* arg) // OK
{
	if(lpObj->Interface.use != 0 || lpObj->State == OBJECT_DELCMD || lpObj->DieRegen != 0 || lpObj->Teleport != 0 || lpObj->PShopOpen != 0 || lpObj->SkillSummonPartyTime != 0)
	{
		return;
	}

	int Zen = abs(atoi(arg));

	if ( lpObj->Money >= Zen )
	{
		lpObj->Money -= Zen;
		lpObj->BankEx.Money += Zen;

		GCMoneySend(lpObj->Index, lpObj->Money);
		//CustomVault::SendInfo(lpObj->Index);
		gBankEx.GDSavePoint(lpObj->Index);
		gBankEx.GCUpdateBankEx(lpObj->Index);
	}
	else
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,"You have insufficient funds.");
	}
}

void CCommandManager::RecvZenCustomVault(LPOBJ lpObj,char* arg) // OK
{
	if(lpObj->Interface.use != 0 || lpObj->State == OBJECT_DELCMD || lpObj->DieRegen != 0 || lpObj->Teleport != 0 || lpObj->PShopOpen != 0 || lpObj->SkillSummonPartyTime != 0)
	{
		return;
	}

	int Zen = abs(atoi(arg));

	if ( lpObj->BankEx.Money >= Zen )
	{
		if ( lpObj->Money + Zen > MAX_MONEY )
		{
			gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,"You cannot possess more than %d zen.", MAX_MONEY);
			return;
		}

		lpObj->Money += Zen;
		lpObj->BankEx.Money -= Zen;

		GCMoneySend(lpObj->Index, lpObj->Money);
		gBankEx.GDSavePoint(lpObj->Index);
		gBankEx.GCUpdateBankEx(lpObj->Index);
	}
	else
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,"You have insufficient funds.");
	}
}

void CCommandManager::SendBlessCustomVault(LPOBJ lpObj,char* arg)
{
	if(lpObj->Interface.use != 0 || lpObj->State == OBJECT_DELCMD || lpObj->DieRegen != 0 || lpObj->Teleport != 0 || lpObj->PShopOpen != 0 || lpObj->SkillSummonPartyTime != 0)
	{
		return;
	}

	int Count = abs(atoi(arg));

	int BlessCount = gItemManager.GetInventoryItemCount(lpObj, GET_ITEM(14, 13), 0);

	if ( Count > BlessCount )
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,"You have insufficient quantity.");
		return;
	}

	gItemManager.DeleteInventoryItemCount(lpObj, GET_ITEM(14, 13), 0, Count);

	lpObj->BankEx.JBless += Count;

	gBankEx.GDSavePoint(lpObj->Index);
	gBankEx.GCUpdateBankEx(lpObj->Index);
}

void CCommandManager::RecvBlessCustomVault(LPOBJ lpObj,char* arg)
{
	if(lpObj->Interface.use != 0 || lpObj->State == OBJECT_DELCMD || lpObj->DieRegen != 0 || lpObj->Teleport != 0 || lpObj->PShopOpen != 0 || lpObj->SkillSummonPartyTime != 0)
	{
		return;
	}

	int Count = abs(atoi(arg));

	if ( Count > lpObj->BankEx.JBless )
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,"You have insufficient funds.");
		return;
	}

	int EmptySlot = gItemManager.GetInventoryEmptySlotCount(lpObj);

	if ( EmptySlot < Count )
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,"You have insufficient space.");
		return;
	}

	for(int n=0;n < Count;n++)
	{
		GDCreateItemSend(lpObj->Index,0xEB,0,0,GET_ITEM(14, 13),0,0,0,0,0,-1,0,0,0,0,0,0xFF,0);
	}

	lpObj->BankEx.JBless -= Count;

	gBankEx.GDSavePoint(lpObj->Index);
	gBankEx.GCUpdateBankEx(lpObj->Index);
}

void CCommandManager::SendSoulCustomVault(LPOBJ lpObj,char* arg)
{
	if(lpObj->Interface.use != 0 || lpObj->State == OBJECT_DELCMD || lpObj->DieRegen != 0 || lpObj->Teleport != 0 || lpObj->PShopOpen != 0 || lpObj->SkillSummonPartyTime != 0)
	{
		return;
	}

	int Count = abs(atoi(arg));

	int BlessCount = gItemManager.GetInventoryItemCount(lpObj, GET_ITEM(14, 14), 0);

	if ( Count > BlessCount )
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,"You have insufficient quantity.");
		return;
	}

	gItemManager.DeleteInventoryItemCount(lpObj, GET_ITEM(14, 14), 0, Count);

	lpObj->BankEx.JSoul += Count;

	gBankEx.GDSavePoint(lpObj->Index);
	gBankEx.GCUpdateBankEx(lpObj->Index);
}

void CCommandManager::RecvSoulCustomVault(LPOBJ lpObj,char* arg)
{
	if(lpObj->Interface.use != 0 || lpObj->State == OBJECT_DELCMD || lpObj->DieRegen != 0 || lpObj->Teleport != 0 || lpObj->PShopOpen != 0 || lpObj->SkillSummonPartyTime != 0)
	{
		return;
	}

	int Count = abs(atoi(arg));

	if ( Count > lpObj->BankEx.JSoul )
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,"You have insufficient funds.");
		return;
	}

	int EmptySlot = gItemManager.GetInventoryEmptySlotCount(lpObj);

	if ( EmptySlot < Count )
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,"You have insufficient space.");
		return;
	}

	for(int n=0;n < Count;n++)
	{
		GDCreateItemSend(lpObj->Index,0xEB,0,0,GET_ITEM(14, 14),0,0,0,0,0,-1,0,0,0,0,0,0xFF,0);
	}

	lpObj->BankEx.JSoul -= Count;

	gBankEx.GDSavePoint(lpObj->Index);
	gBankEx.GCUpdateBankEx(lpObj->Index);
}

void CCommandManager::SendChaosCustomVault(LPOBJ lpObj,char* arg)
{
	if(lpObj->Interface.use != 0 || lpObj->State == OBJECT_DELCMD || lpObj->DieRegen != 0 || lpObj->Teleport != 0 || lpObj->PShopOpen != 0 || lpObj->SkillSummonPartyTime != 0)
	{
		return;
	}

	int Count = abs(atoi(arg));

	int BlessCount = gItemManager.GetInventoryItemCount(lpObj, GET_ITEM(12, 15), 0);

	if ( Count > BlessCount )
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,"You have insufficient quantity.");
		return;
	}

	gItemManager.DeleteInventoryItemCount(lpObj, GET_ITEM(12, 15), 0, Count);

	lpObj->BankEx.JChaos += Count;

	gBankEx.GDSavePoint(lpObj->Index);
	gBankEx.GCUpdateBankEx(lpObj->Index);
}

void CCommandManager::RecvChaosCustomVault(LPOBJ lpObj,char* arg)
{
	if(lpObj->Interface.use != 0 || lpObj->State == OBJECT_DELCMD || lpObj->DieRegen != 0 || lpObj->Teleport != 0 || lpObj->PShopOpen != 0 || lpObj->SkillSummonPartyTime != 0)
	{
		return;
	}

	int Count = abs(atoi(arg));

	if ( Count > lpObj->BankEx.JChaos )
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,"You have insufficient funds.");
		return;
	}

	int EmptySlot = gItemManager.GetInventoryEmptySlotCount(lpObj);

	if ( EmptySlot < Count )
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,"You have insufficient space.");
		return;
	}

	for(int n=0;n < Count;n++)
	{
		GDCreateItemSend(lpObj->Index,0xEB,0,0,GET_ITEM(12, 15),0,0,0,0,0,-1,0,0,0,0,0,0xFF,0);
	}

	lpObj->BankEx.JChaos -= Count;

	gBankEx.GDSavePoint(lpObj->Index);
	gBankEx.GCUpdateBankEx(lpObj->Index);
}

void CCommandManager::SendLifeCustomVault(LPOBJ lpObj,char* arg)
{
	if(lpObj->Interface.use != 0 || lpObj->State == OBJECT_DELCMD || lpObj->DieRegen != 0 || lpObj->Teleport != 0 || lpObj->PShopOpen != 0 || lpObj->SkillSummonPartyTime != 0)
	{
		return;
	}

	int Count = abs(atoi(arg));

	int BlessCount = gItemManager.GetInventoryItemCount(lpObj, GET_ITEM(14, 16), 0);

	if ( Count > BlessCount )
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,"You have insufficient quantity.");
		return;
	}

	gItemManager.DeleteInventoryItemCount(lpObj, GET_ITEM(14, 16), 0, Count);

	lpObj->BankEx.JLife += Count;

	gBankEx.GDSavePoint(lpObj->Index);
	gBankEx.GCUpdateBankEx(lpObj->Index);
}

void CCommandManager::RecvLifeCustomVault(LPOBJ lpObj,char* arg)
{
	if(lpObj->Interface.use != 0 || lpObj->State == OBJECT_DELCMD || lpObj->DieRegen != 0 || lpObj->Teleport != 0 || lpObj->PShopOpen != 0 || lpObj->SkillSummonPartyTime != 0)
	{
		return;
	}

	int Count = abs(atoi(arg));

	if ( Count > lpObj->BankEx.JLife )
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,"You have insufficient funds.");
		return;
	}

	int EmptySlot = gItemManager.GetInventoryEmptySlotCount(lpObj);

	if ( EmptySlot < Count )
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,"You have insufficient space.");
		return;
	}

	for(int n=0;n < Count;n++)
	{
		GDCreateItemSend(lpObj->Index, 0xEB, 0, 0, GET_ITEM(14, 16), 0, 0, 0, 0, 0, -1, 0, 0, 0, 0, 0, 0xFF, 0);
	}

	lpObj->BankEx.JLife -= Count;

	gBankEx.GDSavePoint(lpObj->Index);
	gBankEx.GCUpdateBankEx(lpObj->Index);
}

void CCommandManager::SendCreationCustomVault(LPOBJ lpObj,char* arg)
{
	if(lpObj->Interface.use != 0 || lpObj->State == OBJECT_DELCMD || lpObj->DieRegen != 0 || lpObj->Teleport != 0 || lpObj->PShopOpen != 0 || lpObj->SkillSummonPartyTime != 0)
	{
		return;
	}

	int Count = abs(atoi(arg));

	int BlessCount = gItemManager.GetInventoryItemCount(lpObj, GET_ITEM(14, 22), 0);

	if ( Count > BlessCount )
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, "You have insufficient quantity.");
		return;
	}

	gItemManager.DeleteInventoryItemCount(lpObj, GET_ITEM(14, 22), 0, Count);

	lpObj->BankEx.JCreation += Count;

	gBankEx.GDSavePoint(lpObj->Index);
	gBankEx.GCUpdateBankEx(lpObj->Index);
}

void CCommandManager::RecvCreationCustomVault(LPOBJ lpObj,char* arg)
{
	if(lpObj->Interface.use != 0 || lpObj->State == OBJECT_DELCMD || lpObj->DieRegen != 0 || lpObj->Teleport != 0 || lpObj->PShopOpen != 0 || lpObj->SkillSummonPartyTime != 0)
	{
		return;
	}

	int Count = abs(atoi(arg));

	if ( Count > lpObj->BankEx.JCreation )
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,"You have insufficient funds.");
		return;
	}

	int EmptySlot = gItemManager.GetInventoryEmptySlotCount(lpObj);

	if ( EmptySlot < Count )
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,"You have insufficient space.");
		return;
	}

	for(int n=0;n < Count;n++)
	{
		GDCreateItemSend(lpObj->Index, 0xEB, 0, 0, GET_ITEM(14, 22), 0, 0, 0, 0, 0, -1, 0, 0, 0, 0, 0, 0xFF, 0);
	}

	lpObj->BankEx.JCreation -= Count;

	gBankEx.GDSavePoint(lpObj->Index);
	gBankEx.GCUpdateBankEx(lpObj->Index);
}

void CCommandManager::SendGuardianCustomVault(LPOBJ lpObj,char* arg)
{
	if(lpObj->Interface.use != 0 || lpObj->State == OBJECT_DELCMD || lpObj->DieRegen != 0 || lpObj->Teleport != 0 || lpObj->PShopOpen != 0 || lpObj->SkillSummonPartyTime != 0)
	{
		return;
	}

	int Count = abs(atoi(arg));

	int BlessCount = gItemManager.GetInventoryItemCount(lpObj, GET_ITEM(14, 31), 0);

	if ( Count > BlessCount )
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, "You have insufficient quantity.");
		return;
	}

	gItemManager.DeleteInventoryItemCount(lpObj, GET_ITEM(14, 31), 0, Count);

	lpObj->BankEx.JGuardian += Count;

	gBankEx.GDSavePoint(lpObj->Index);
	gBankEx.GCUpdateBankEx(lpObj->Index);
}

void CCommandManager::RecvGuardianCustomVault(LPOBJ lpObj,char* arg)
{
	if(lpObj->Interface.use != 0 || lpObj->State == OBJECT_DELCMD || lpObj->DieRegen != 0 || lpObj->Teleport != 0 || lpObj->PShopOpen != 0 || lpObj->SkillSummonPartyTime != 0)
	{
		return;
	}

	int Count = abs(atoi(arg));

	if ( Count > lpObj->BankEx.JGuardian )
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,"You have insufficient funds.");
		return;
	}

	int EmptySlot = gItemManager.GetInventoryEmptySlotCount(lpObj);

	if ( EmptySlot < Count )
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,"You have insufficient space.");
		return;
	}

	for(int n=0;n < Count;n++)
	{
		GDCreateItemSend(lpObj->Index, 0xEB, 0, 0, GET_ITEM(14, 31), 0, 0, 0, 0, 0, -1, 0, 0, 0, 0, 0, 0xFF, 0);
	}

	lpObj->BankEx.JGuardian -= Count;

	gBankEx.GDSavePoint(lpObj->Index);
	gBankEx.GCUpdateBankEx(lpObj->Index);
}

void CCommandManager::SendHarmonyCustomVault(LPOBJ lpObj,char* arg)
{
	if(lpObj->Interface.use != 0 || lpObj->State == OBJECT_DELCMD || lpObj->DieRegen != 0 || lpObj->Teleport != 0 || lpObj->PShopOpen != 0 || lpObj->SkillSummonPartyTime != 0)
	{
		return;
	}

	int Count = abs(atoi(arg));

	int BlessCount = gItemManager.GetInventoryItemCount(lpObj, GET_ITEM(14, 42), 0);

	if ( Count > BlessCount )
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, "You have insufficient quantity.");
		return;
	}

	gItemManager.DeleteInventoryItemCount(lpObj, GET_ITEM(14, 42), 0, Count);

	lpObj->BankEx.JHarmony += Count;

	gBankEx.GDSavePoint(lpObj->Index);
	gBankEx.GCUpdateBankEx(lpObj->Index);
}

void CCommandManager::RecvHarmonyCustomVault(LPOBJ lpObj,char* arg)
{
	if(lpObj->Interface.use != 0 || lpObj->State == OBJECT_DELCMD || lpObj->DieRegen != 0 || lpObj->Teleport != 0 || lpObj->PShopOpen != 0 || lpObj->SkillSummonPartyTime != 0)
	{
		return;
	}

	int Count = abs(atoi(arg));

	if ( Count > lpObj->BankEx.JHarmony )
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,"You have insufficient funds.");
		return;
	}

	int EmptySlot = gItemManager.GetInventoryEmptySlotCount(lpObj);

	if ( EmptySlot < Count )
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,"You have insufficient space.");
		return;
	}

	for(int n=0;n < Count;n++)
	{
		GDCreateItemSend(lpObj->Index, 0xEB, 0, 0, GET_ITEM(14, 42), 0, 0, 0, 0, 0, -1, 0, 0, 0, 0, 0, 0xFF, 0);
	}

	lpObj->BankEx.JHarmony -= Count;

	gBankEx.GDSavePoint(lpObj->Index);
	gBankEx.GCUpdateBankEx(lpObj->Index);
}

void CCommandManager::SendLowStoneCustomVault(LPOBJ lpObj,char* arg)
{
	if(lpObj->Interface.use != 0 || lpObj->State == OBJECT_DELCMD || lpObj->DieRegen != 0 || lpObj->Teleport != 0 || lpObj->PShopOpen != 0 || lpObj->SkillSummonPartyTime != 0)
	{
		return;
	}

	int Count = abs(atoi(arg));

	int BlessCount = gItemManager.GetInventoryItemCount(lpObj, GET_ITEM(14, 43), 0);

	if ( Count > BlessCount )
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, "You have insufficient quantity.");
		return;
	}

	gItemManager.DeleteInventoryItemCount(lpObj, GET_ITEM(14, 43), 0, Count);

	lpObj->BankEx.JLowStone += Count;

	gBankEx.GDSavePoint(lpObj->Index);
	gBankEx.GCUpdateBankEx(lpObj->Index);
}

void CCommandManager::RecvLowStoneCustomVault(LPOBJ lpObj,char* arg)
{
	if(lpObj->Interface.use != 0 || lpObj->State == OBJECT_DELCMD || lpObj->DieRegen != 0 || lpObj->Teleport != 0 || lpObj->PShopOpen != 0 || lpObj->SkillSummonPartyTime != 0)
	{
		return;
	}

	int Count = abs(atoi(arg));

	if ( Count > lpObj->BankEx.JLowStone )
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,"You have insufficient funds.");
		return;
	}

	int EmptySlot = gItemManager.GetInventoryEmptySlotCount(lpObj);

	if ( EmptySlot < Count )
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,"You have insufficient space.");
		return;
	}

	for(int n=0;n < Count;n++)
	{
		GDCreateItemSend(lpObj->Index, 0xEB, 0, 0, GET_ITEM(14, 43), 0, 0, 0, 0, 0, -1, 0, 0, 0, 0, 0, 0xFF, 0);
	}

	lpObj->BankEx.JLowStone -= Count;

	gBankEx.GDSavePoint(lpObj->Index);
	gBankEx.GCUpdateBankEx(lpObj->Index);
}

void CCommandManager::SendHighStoneCustomVault(LPOBJ lpObj,char* arg)
{
	if(lpObj->Interface.use != 0 || lpObj->State == OBJECT_DELCMD || lpObj->DieRegen != 0 || lpObj->Teleport != 0 || lpObj->PShopOpen != 0 || lpObj->SkillSummonPartyTime != 0)
	{
		return;
	}

	int Count = abs(atoi(arg));

	int BlessCount = gItemManager.GetInventoryItemCount(lpObj, GET_ITEM(14, 44), 0);

	if ( Count > BlessCount )
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, "You have insufficient quantity.");
		return;
	}

	gItemManager.DeleteInventoryItemCount(lpObj, GET_ITEM(14, 44), 0, Count);

	lpObj->BankEx.JHighStone += Count;

	gBankEx.GDSavePoint(lpObj->Index);
	gBankEx.GCUpdateBankEx(lpObj->Index);
}

void CCommandManager::RecvHighStoneCustomVault(LPOBJ lpObj,char* arg)
{
	if(lpObj->Interface.use != 0 || lpObj->State == OBJECT_DELCMD || lpObj->DieRegen != 0 || lpObj->Teleport != 0 || lpObj->PShopOpen != 0 || lpObj->SkillSummonPartyTime != 0)
	{
		return;
	}

	int Count = abs(atoi(arg));

	if ( Count > lpObj->BankEx.JHighStone )
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,"You have insufficient funds.");
		return;
	}

	int EmptySlot = gItemManager.GetInventoryEmptySlotCount(lpObj);

	if ( EmptySlot < Count )
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,"You have insufficient space.");
		return;
	}

	for(int n=0;n < Count;n++)
	{
		GDCreateItemSend(lpObj->Index, 0xEB, 0, 0, GET_ITEM(14, 44), 0, 0, 0, 0, 0, -1, 0, 0, 0, 0, 0, 0xFF, 0);
	}

	lpObj->BankEx.JHighStone -= Count;

	gBankEx.GDSavePoint(lpObj->Index);
	gBankEx.GCUpdateBankEx(lpObj->Index);
}

void CCommandManager::SendGemStoneCustomVault(LPOBJ lpObj,char* arg)
{
	if(lpObj->Interface.use != 0 || lpObj->State == OBJECT_DELCMD || lpObj->DieRegen != 0 || lpObj->Teleport != 0 || lpObj->PShopOpen != 0 || lpObj->SkillSummonPartyTime != 0)
	{
		return;
	}

	int Count = abs(atoi(arg));

	int BlessCount = gItemManager.GetInventoryItemCount(lpObj, GET_ITEM(14, 41), 0);

	if ( Count > BlessCount )
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, "You have insufficient quantity.");
		return;
	}

	gItemManager.DeleteInventoryItemCount(lpObj, GET_ITEM(14, 41), 0, Count);

	lpObj->BankEx.JGemStone += Count;

	gBankEx.GDSavePoint(lpObj->Index);
	gBankEx.GCUpdateBankEx(lpObj->Index);
}

void CCommandManager::RecvGemStoneCustomVault(LPOBJ lpObj,char* arg)
{
	if(lpObj->Interface.use != 0 || lpObj->State == OBJECT_DELCMD || lpObj->DieRegen != 0 || lpObj->Teleport != 0 || lpObj->PShopOpen != 0 || lpObj->SkillSummonPartyTime != 0)
	{
		return;
	}

	int Count = abs(atoi(arg));

	if ( Count > lpObj->BankEx.JGemStone )
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,"You have insufficient funds.");
		return;
	}

	int EmptySlot = gItemManager.GetInventoryEmptySlotCount(lpObj);

	if ( EmptySlot < Count )
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,"You have insufficient space.");
		return;
	}

	for(int n=0;n < Count;n++)
	{
		GDCreateItemSend(lpObj->Index, 0xEB, 0, 0, GET_ITEM(14, 41), 0, 0, 0, 0, 0, -1, 0, 0, 0, 0, 0, 0xFF, 0);
	}

	lpObj->BankEx.JGemStone -= Count;

	gBankEx.GDSavePoint(lpObj->Index);
	gBankEx.GCUpdateBankEx(lpObj->Index);
}

void CCommandManager::PackIudexItem(LPOBJ lpObj, char* arg)
{
	if(lpObj->Interface.use != 0 || lpObj->State == OBJECT_DELCMD || lpObj->DieRegen != 0 || lpObj->Teleport != 0 || lpObj->PShopOpen != 0 || lpObj->SkillSummonPartyTime != 0)
	{
		return;
	}

	int Type = this->GetNumber(arg,0);

	if(gObjIsConnectedGP(lpObj->Index) == 0)
	{
		return;
	}

	if (Type != 10 && Type != 20 && Type != 30)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,"Wrong type");
		return;
	}

	if(lpObj->ChaosLock != 0)
	{
		//this->GCPackSystemResultSend(aIndex,0);
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,"Something went wrong!");
		return;
	}

	lpObj->ChaosLock = 1;

	int count = gItemManager.GetInventoryItemCount(lpObj,GET_ITEM(14,207),0);

	if (count < Type)
	{
		lpObj->ChaosLock = 0;
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,"You dont have enough items!");
		return;
	}

	gItemManager.DeleteInventoryItemCount(lpObj,GET_ITEM(14,207),0,Type);

	GDCreateItemSend(lpObj->Index,0xEB,0,0,GET_ITEM(14,208),(Type/10)-1,0,0,0,0,-1,0,0,0,0,0,0xFF,0);

	//this->GCPackSystemResultSend(aIndex,1);

	lpObj->ChaosLock = 0;
}

void CCommandManager::UnPackIudexItem(LPOBJ lpObj, char* arg)
{
	if(lpObj->Interface.use != 0 || lpObj->State == OBJECT_DELCMD || lpObj->DieRegen != 0 || lpObj->Teleport != 0 || lpObj->PShopOpen != 0 || lpObj->SkillSummonPartyTime != 0)
	{
		return;
	}

	int Type = this->GetNumber(arg,0);

	if(gObjIsConnectedGP(lpObj->Index) == 0)
	{
		return;
	}

	if (Type != 10 && Type != 20 && Type != 30)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,"Wrong type");
		return;
	}

	if(lpObj->ChaosLock != 0)
	{
		//this->GCPackSystemResultSend(aIndex,0);
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,"Something went wrong!");
		return;
	}

	lpObj->ChaosLock = 1;

	int level = (Type/10)-1;

	int count = gItemManager.GetInventoryItemCount(lpObj,GET_ITEM(14,208),level);

	if (count <= 0)
	{
		//this->GCJewelUnMixResultSend(aIndex,5);
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,"You dont have enough items!");
		lpObj->ChaosLock = 0;
		return;
	}

	if (gItemManager.GetInventoryEmptySlotCount(lpObj) < Type)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,"Need more free space!");
		//this->GCJewelUnMixResultSend(aIndex,5);
		lpObj->ChaosLock = 0;
		return;
	}

	int slot = gItemManager.GetInventoryItemSlot(lpObj,GET_ITEM(14,208),level);

	gItemManager.InventoryDelItem(lpObj->Index,slot);

	gItemManager.GCItemDeleteSend(lpObj->Index,slot,1);

	for(int n=0;n < Type;n++)
	{
		GDCreateItemSend(lpObj->Index,0xEB,0,0,GET_ITEM(14,207),0,0,0,0,0,-1,0,0,0,0,0,0xFF,0);
	}

	//this->GCJewelUnMixResultSend(aIndex,1);

	lpObj->ChaosLock = 0;
}
