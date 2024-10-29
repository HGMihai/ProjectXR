#include "stdafx.h"
#include "PartySearch.h"
#include "Util.h"
#include "Party.h"
#include "DefaultClassInfo.h"
#include "Notice.h"
#include "Message.h"
#include "Protection.h"
#include "Guild.h"
#include "Union.h"
#include "GuildClass.h"

cPartySearch gPartySearch;

cPartySearch::cPartySearch()
{
	this->m_Registered.clear();
}

cPartySearch::~cPartySearch()
{
	
}

void cPartySearch::Load(char* section, char* path)
{
	if(FileExist(path) == 0)
	{
		ErrorMessageBox("%s file load fail (not found)",path);
	}
	this->NpcID = GetPrivateProfileInt(section,"NpcID",0,path);
	this->NpcMap = GetPrivateProfileInt(section,"NpcMap",0,path);
	this->NpcX = GetPrivateProfileInt(section,"NpcX",0,path);
	this->NpcY = GetPrivateProfileInt(section,"NpcY",0,path);

	this->DelAfterRelog = GetPrivateProfileInt(section,"DelAfterRelog",0,path);

	this->CanOpenSettings[0] = GetPrivateProfileInt(section,"CanOpenSettings_AL0",0,path);
	this->CanOpenSettings[1] = GetPrivateProfileInt(section,"CanOpenSettings_AL1",0,path);
	this->CanOpenSettings[2] = GetPrivateProfileInt(section,"CanOpenSettings_AL2",0,path);
	this->CanOpenSettings[3] = GetPrivateProfileInt(section,"CanOpenSettings_AL3",0,path);

	this->CanOpenMain[0] = GetPrivateProfileInt(section,"CanOpenMain_AL0",0,path);
	this->CanOpenMain[1] = GetPrivateProfileInt(section,"CanOpenMain_AL1",0,path);
	this->CanOpenMain[2] = GetPrivateProfileInt(section,"CanOpenMain_AL2",0,path);
	this->CanOpenMain[3] = GetPrivateProfileInt(section,"CanOpenMain_AL3",0,path);
}


void cPartySearch::PartySearchAddToList(PMSG_RECV_PARTYSEARCH_ADD* lpMsg, int aIndex)
{
	if (gProtection.GetCustomState(CUSTOM_PARTY_SEARCH) == 0)
	{
		return;
	}

	REGISTERED_INFO info;

	memcpy(info.Name,gObj[aIndex].Name,sizeof(info.Name));
	info.aIndex = aIndex;
	info.DarkKnight = lpMsg->DarkKnight;
	info.DarkWizard = lpMsg->DarkWizard;
	info.Elf = lpMsg->Elf;
	info.MagicGladiator = lpMsg->MagicGladiator;
	info.DarkLord = lpMsg->DarkLord;
	info.Summoner = lpMsg->Summoner;
	info.RageFighter = lpMsg->RageFighter;
	info.OnlyGuild = lpMsg->OnlyGuild;
	info.OnlyAlliance = lpMsg->OnlyAlliance;
	info.OneClass = lpMsg->OneClass;
	info.Level = lpMsg->Level;
	info.RequirePassword = lpMsg->RequirePassword;
	memcpy(info.Password,lpMsg->Password,sizeof(info.Password));

	if (lpMsg->SystemActive == true)
	{
		std::string nme(info.Name);

		std::transform(nme.begin(),nme.end(),nme.begin(),tolower);

		std::map<std::string,REGISTERED_INFO>::iterator it = this->m_Registered.find(nme);

		if(it == this->m_Registered.end())
		{
			this->m_Registered.insert(std::pair<std::string,REGISTERED_INFO>(nme,info));
			if (info.RequirePassword)
				gNotice.GCNoticeSendToAll(0,0,0,0,0,0,"New Private Auto Party created [%s] %s %d %d",gObj[aIndex].Name, gMessage.GetMap(gObj[aIndex].Map),gObj[aIndex].X,gObj[aIndex].Y);
			else
				gNotice.GCNoticeSendToAll(0,0,0,0,0,0,"New Auto Party created [%s] %s %d %d",gObj[aIndex].Name, gMessage.GetMap(gObj[aIndex].Map),gObj[aIndex].X,gObj[aIndex].Y);
		}
		else
		{
			it->second = info;
		}

		info.IsOnline = true;
	}
	else
	{
		std::string nme(info.Name);

		std::transform(nme.begin(),nme.end(),nme.begin(),tolower);

		std::map<std::string,REGISTERED_INFO>::iterator it = this->m_Registered.find(nme);

		if(it != this->m_Registered.end())
		{
			this->m_Registered.erase(it);
			return;
		}
	}
}

void cPartySearch::PartySearchDelFromList(char* Name)
{
	if (this->DelAfterRelog == false)
	{
		return;
	}

	std::string nme(Name);

	std::transform(nme.begin(),nme.end(),nme.begin(),tolower);

	std::map<std::string,REGISTERED_INFO>::iterator it = this->m_Registered.find(nme);

	if(it != this->m_Registered.end())
	{
		this->m_Registered.erase(it);
	}
}


void cPartySearch::GCPartyListSend(int aIndex)
{
	BYTE send[4096];

	PMSG_SEND_PARTYLIST pMsg;
	pMsg.header.set(0xF3, 0xF0, sizeof(pMsg));

	int size = sizeof(pMsg);

	pMsg.Count = 0;

	PMSG_PARTYLIST info;

	for(std::map<std::string,REGISTERED_INFO>::iterator it=this->m_Registered.begin();it != this->m_Registered.end();it++)
	{
		LPOBJ lpObj = gObjFind(it->second.Name);

		if (lpObj == 0)
		{
			it->second.IsOnline = false;
		}
		else
		{
			std::string name(it->second.Name);

			std::transform(name.begin(),name.end(),name.begin(),tolower);

			std::map<std::string,REGISTERED_INFO>::iterator iter = this->m_Registered.find(name);

			iter->second.aIndex = lpObj->Index;

			iter->second.IsOnline = true;
		}

		if (it->second.IsOnline == true)
		{
			memcpy(info.Name,it->second.Name,sizeof(info.Name));
			info.Map = lpObj->Map;
			info.X = lpObj->X;
			info.Y = lpObj->Y;
			info.Level = it->second.Level;
			
			info.OnlyGuild = it->second.OnlyGuild;

			info.OnlyAlliance = it->second.OnlyAlliance;

			info.RequirePassword = it->second.RequirePassword;

			/*if (info->second.OnlyGuild == true &&(lpObj->GuildNumber != lpTarget->GuildNumber))
			{
				gNotice.GCNoticeSend(lpObj->Index,1,0, 0, 0, 0, 0, "You are not in the right guild!");
				return;
			}*/

			info.IsSameGuild = false;
			info.IsSameAlliance = false;

			if (info.OnlyGuild == true &&(lpObj->GuildNumber == gObj[aIndex].GuildNumber))
			{
				info.IsSameGuild = true;
			}

			if (info.OnlyAlliance == true)
			{
				if ((lpObj->Guild != 0 && lpObj->Guild->GuildUnion != 0) && (gObj[aIndex].Guild != 0 && gObj[aIndex].Guild->GuildUnion != 0))
				{
					if (lpObj->Guild->GuildUnion == gObj[aIndex].Guild->GuildUnion)
					{
						info.IsSameAlliance = true;
					}

					/*int iUnionNum = lpObj->Guild->GuildUnion;
					int iGuildCount =0;
					int iGuildList[100] ={0};

					if ( gUnionManager.GetGuildUnionMemberList(iUnionNum, iGuildCount, iGuildList) == TRUE)
					{
						for ( int i=0;i<iGuildCount;i++)
						{
							GUILD_INFO_STRUCT * lpGuildInfo = gGuildClass.SearchGuild_Number(iGuildList[i]);

							if ( lpGuildInfo == NULL )
								continue;

							if ()
						}
					}*/
				}

				/*if (lpObj->GuildNumber > 0)
				{
				if (lpObj->Guild->GuildUnion == 0 || (lpObj->Guild->GuildUnion != gObj[aIndex].Guild->GuildUnion))
				{
				info.IsSameAlliance = true;
				}
				}*/

			}

			info.Count = 1;

			if(OBJECT_RANGE(lpObj->PartyNumber) != 0)
			{
				info.Count = gParty.GetMemberCount(lpObj->PartyNumber);
			}

			bool HaveDW = true;
			bool HaveDK = true;
			bool HaveFE = true;
			bool HaveMG = true;
			bool HaveDL = true;
			bool HaveSU = true;
			bool HaveRF = true;

			if (it->second.OneClass == true)
			{
				if (info.Count != 1)
				{
					for (int i=0;i<info.Count;i++)
					{
						LPOBJ lpTarget = &gObj[gParty.m_PartyInfo[lpObj->PartyNumber].Index[i]];

						if (lpTarget->Class == CLASS_DW)
						{
							HaveDW = false;
						}
						else if (lpTarget->Class == CLASS_DK)
						{
							HaveDK = false;
						}
						else if (lpTarget->Class == CLASS_FE)
						{
							HaveFE = false;
						}
						else if (lpTarget->Class == CLASS_MG)
						{
							HaveMG = false;
						}
						else if (lpTarget->Class == CLASS_DL)
						{
							HaveDL = false;
						}
						else if (lpTarget->Class == CLASS_SU)
						{
							HaveSU = false;
						}
						else if (lpTarget->Class == CLASS_RF)
						{
							HaveRF = false;
						}
					}
				}
				else
				{
					if (lpObj->Class == CLASS_DW)
					{
						HaveDW = false;
					}
					else if (lpObj->Class == CLASS_DK)
					{
						HaveDK = false;
					}
					else if (lpObj->Class == CLASS_FE)
					{
						HaveFE = false;
					}
					else if (lpObj->Class == CLASS_MG)
					{
						HaveMG = false;
					}
					else if (lpObj->Class == CLASS_DL)
					{
						HaveDL = false;
					}
					else if (lpObj->Class == CLASS_SU)
					{
						HaveSU = false;
					}
					else if (lpObj->Class == CLASS_RF)
					{
						HaveRF = false;
					}
				}

				info.DarkWizard = HaveDW;
				info.DarkKnight = HaveDK;
				info.Elf = HaveFE;
				info.MagicGladiator = HaveMG;
				info.DarkLord = HaveDL;
				info.Summoner = HaveSU;
				info.RageFighter = HaveRF;
			}
			else
			{
				info.DarkWizard = it->second.DarkWizard;
				info.DarkKnight = it->second.DarkKnight;
				info.Elf = it->second.Elf;
				info.MagicGladiator = it->second.MagicGladiator;
				info.DarkLord = it->second.DarkLord;
				info.Summoner = it->second.Summoner;
				info.RageFighter = it->second.RageFighter;
			}

			memcpy(&send[size],&info,sizeof(info));
			size += sizeof(info);

			pMsg.Count++;
		}
	}

	pMsg.header.size[0] = SET_NUMBERHB(size);

	pMsg.header.size[1] = SET_NUMBERLB(size);

	memcpy(send,&pMsg,sizeof(pMsg));
	DataSend(aIndex,send,size);
}


bool cPartySearch::Dialog(LPOBJ lpObj, LPOBJ lpNpc)
{
	if (gProtection.GetCustomState(CUSTOM_PARTY_SEARCH) == 0)
	{
		return false;
	}

	if( lpNpc->Class		== this->NpcID 
	&&	lpNpc->Map			== this->NpcMap
	&&	lpNpc->X			== this->NpcX
	&&	lpNpc->Y			== this->NpcY )
	{
		if (this->CanOpenMain[lpObj->AccountLevel] == 1)
		{
			this->GCPartyListSend(lpObj->Index);
			return true;
		}
	}
	return false;
}

void cPartySearch::RequestParty(PMSG_PARTY_REQ_REQ* lpMsg, int aIndex)
{
	if (gProtection.GetCustomState(CUSTOM_PARTY_SEARCH) == 0)
	{
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];
	LPOBJ lpTarget = gObjFind(lpMsg->Name);

	if (lpTarget == NULL)
	{
        return;
	}

	std::string name(lpTarget->Name);

	std::transform(name.begin(),name.end(),name.begin(),tolower);

	std::map<std::string,REGISTERED_INFO>::iterator info = this->m_Registered.find(name);
	
	if (info->second.RequirePassword)
	{
		if (!lpMsg->needPassword)
		{
			gNotice.GCNoticeSend(lpObj->Index,1,0, 0, 0, 0, 0, "Something went wrong. Contact Administrator.");
			return;
		}

		if (strcmp(info->second.Password,lpMsg->Password) != 0)
		{
			gNotice.GCNoticeSend(lpObj->Index,1,0, 0, 0, 0, 0, "You entered the wrong password. Try again...");
			return;
		}
	}

	/*if (strcmp(info->second.))*/

	if (lpObj->PartyNumber >= 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0, 0, 0, 0, 0, "You already in party!");
		return;
	}

	if (!strcmp(lpObj->Name,lpTarget->Name))
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0, 0, 0, 0, 0, "You can not add yourself to the party!");
		return;
	}

	if (lpTarget->PartyNumber >= 0)
	{
		if (lpObj->PartyNumber == lpTarget->PartyNumber)
		{
			gNotice.GCNoticeSend(lpObj->Index,1,0, 0, 0, 0, 0, "You are already in this group!");
			return;
		}
	}

	//if (!strcmp(lpObj->Guild))

	if (info->second.OnlyGuild == true &&(lpObj->GuildNumber != lpTarget->GuildNumber))
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0, 0, 0, 0, 0, "You are not in the right guild!");
		return;
	}

	if(gParty.Create(lpTarget->Index) == 0)
	{
		if(gParty.AddMember(lpTarget->PartyNumber,lpObj->Index) == 0)
		{
			gParty.GCPartyResultSend(lpObj->Index,2);
			gParty.GCPartyResultSend(lpTarget->Index,2);
		}
	}
	else
	{
		if(gParty.AddMember(lpTarget->PartyNumber,lpObj->Index) == 0)
		{
			gParty.GCPartyResultSend(lpObj->Index,2);
			gParty.GCPartyResultSend(lpTarget->Index,2);
			gParty.Destroy(lpTarget->PartyNumber);
		}
	}
}

void cPartySearch::SendPartySettings(LPOBJ lpObj)
{
	if (gProtection.GetCustomState(CUSTOM_PARTY_SEARCH) == 0)
	{
		return;
	}

	std::string nme(lpObj->Name);

	std::transform(nme.begin(),nme.end(),nme.begin(),tolower);

	std::map<std::string,REGISTERED_INFO>::iterator it = this->m_Registered.find(nme);

	if(it == this->m_Registered.end())
	{
		return;
	}

	PMSG_PARTYSETTINGS_SEND pMsg;

	pMsg.header.set(0xFB, 0x12, sizeof(pMsg));

	pMsg.SystemActive = true;

	pMsg.OnlyGuild = it->second.OnlyGuild;

	pMsg.OnlyAlliance = it->second.OnlyAlliance;

	pMsg.OneClass = it->second.OneClass;

	pMsg.Level = it->second.Level;

	pMsg.DarkWizard = it->second.DarkWizard;

	pMsg.DarkKnight = it->second.DarkKnight;

	pMsg.Elf = it->second.Elf;

	pMsg.MagicGladiator = it->second.MagicGladiator;

	pMsg.DarkLord = it->second.DarkLord;

	pMsg.Summoner = it->second.Summoner;

	pMsg.RageFighter = it->second.RageFighter;

	pMsg.RequiredPassword = it->second.RequirePassword;

	memcpy(pMsg.Password,it->second.Password,sizeof(pMsg.Password));
	//memcpy(info.Password,lpMsg->Password,sizeof(info.Password));

	DataSend(lpObj->Index,(LPBYTE)&pMsg, sizeof(pMsg));
}
