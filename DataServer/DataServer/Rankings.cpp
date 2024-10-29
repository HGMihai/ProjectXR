#include "stdafx.h"
#include "Rankings.h"
#include "QueryManager.h"
#include "SocketManager.h"
#include "Util.h"
#include "QueryData.h"

CRankings gRankings;

void CRankings::GDRecvRequest(PMSG_DB_REQ_SEND* lpMsg, int Index)
{
	BYTE send[8192];
	
	SDHP_RANKINGS_INFO_SEND pMsg;

	pMsg.header.set(0xFB,0x05,0);

	int size = sizeof(pMsg);
	
	pMsg.aIndex = lpMsg->aIndex;

	pMsg.count = 0;

	SDHP_RANKINGS_INFO info;
	
	//if(gQueryManager.ExecQuery("SELECT TOP 100 [c].[Name], [c].[cLevel], [c].[Class], [c].[MapNumber], [c].[ResetCount], [c].[MasterResetCount], ISNULL([gr].[Family], 0) as [Family], ISNULL([msk].[MasterLevel], 0) as [MasterLevel], ISNULL([ms].[ConnectStat], 0) as [ConnectStat], [mi].[AccountLevel] FROM [dbo].[Character] [c] LEFT JOIN [dbo].[Gens_Rank] [gr] on [gr].[Name] = [c].[Name] LEFT JOIN [dbo].[MasterSkillTree] [msk] on [msk].[Name] = [c].[Name] LEFT JOIN [dbo].[MEMB_STAT] [ms] on [ms].[memb___id] = [c].[AccountID] LEFT JOIN [dbo].[MEMB_INFO] [mi] on [mi].[memb___id] = [c].[AccountID] ORDER BY [c].[MasterResetCount] DESC, [c].[ResetCount] DESC, [c].[cLevel] DESC") != 0/* || gQueryManager.Fetch() == SQL_NO_DATA*/)
	if(gQueryManager.ExecQuery((char*)gQueryData.QueryString[0].c_str()) != 0/* || gQueryManager.Fetch() == SQL_NO_DATA*/)
	{
		while(gQueryManager.Fetch() != SQL_NO_DATA)
		{
			gQueryManager.GetAsString("Name",info.name,sizeof(info.name));
			info.level = gQueryManager.GetAsInteger("cLevel");
			info.classId = gQueryManager.GetAsInteger("Class");
			info.map = gQueryManager.GetAsInteger("MapNumber");
			info.reset = gQueryManager.GetAsInteger("ResetCount");
			info.masterReset = gQueryManager.GetAsInteger("MasterResetCount");
			info.gensFamily = gQueryManager.GetAsInteger("Family");
			info.masterLevel = gQueryManager.GetAsInteger("MasterLevel");
			info.connectStat = gQueryManager.GetAsInteger("ConnectStat");
			info.accountLevel = gQueryManager.GetAsInteger("AccountLevel");

			//LogAdd(LOG_RED,"[%s] %d %d %d %d %d %d %d",info.name, info.level, info.classId, info.map, info.reset, info.masterReset, info.gensFamily, info.masterLevel);

			memcpy(&send[size],&info,sizeof(info));
			size += sizeof(info);

			pMsg.count++;
		}
	}

	gQueryManager.Close();

	pMsg.header.size[0] = SET_NUMBERHB(size);
	pMsg.header.size[1] = SET_NUMBERLB(size);

	memcpy(send,&pMsg,sizeof(pMsg));

	gSocketManager.DataSend(Index,send,size);
	/*
	dbo.Character:
		Name
		cLevel
		Class
		MapNumber
		ResetCount
		MasterResetCount
	dbo.Gens_Rank
		Family
	dbo.MasterSkillTree
		MasterLevel
	*/
}

void CRankings::GDCharacterInfoRequest(GD_RANKINGS_INFO_REQUEST_RECV* lpMsg, int Index)
{
	DG_RANKINGS_INFO_REQUEST_SEND pMsg;

	pMsg.header.set(0xFB,0x06,sizeof(pMsg));

	pMsg.aIndex = lpMsg->aIndex;
	
	if(gQueryManager.ExecQuery("select [c].Strength, [c].Dexterity, [c].Vitality, [c].Energy, [c].Leadership, [c].PkLevel, [c].PkCount, [c].MapPosX, [c].MapPosY, [c].QuestIndex, [c].QuestStat, Inventory, ISNULL([gm].[G_Name], '-') as [G_Name] from [dbo].[Character] [c] LEFT JOIN [dbo].GuildMember [gm] on [gm].[Name] = [c].[Name] where [c].Name = '%s'",lpMsg->name) == 0 || gQueryManager.Fetch() == SQL_NO_DATA)
	{
		gQueryManager.Close();

		pMsg.result = 0;
	}
	else
	{
		pMsg.result = 1;

		pMsg.Strength = gQueryManager.GetAsInteger("Strength");
		pMsg.Dexterity = gQueryManager.GetAsInteger("Dexterity");
		pMsg.Vitality = gQueryManager.GetAsInteger("Vitality");
		pMsg.Energy = gQueryManager.GetAsInteger("Energy");
		pMsg.Leadership = gQueryManager.GetAsInteger("Leadership");
		pMsg.PkLevel = gQueryManager.GetAsInteger("PkLevel");
		pMsg.PkCount = gQueryManager.GetAsInteger("PkCount");
		pMsg.MapPosX = gQueryManager.GetAsInteger("MapPosX");
		pMsg.MapPosY = gQueryManager.GetAsInteger("MapPosY");
		pMsg.QuestIndex = gQueryManager.GetAsInteger("QuestIndex");
		pMsg.QuestStat = gQueryManager.GetAsInteger("QuestStat");
		gQueryManager.GetAsString("G_Name",pMsg.GuildName,sizeof(pMsg.GuildName));
		gQueryManager.GetAsBinary("Inventory",pMsg.Inventory[0],sizeof(pMsg.Inventory));

		gQueryManager.Close();
	}

	gSocketManager.DataSend(Index,(BYTE*)&pMsg,sizeof(pMsg));

	/*
	dbo.Character
		Strength
		Dexterity
		Vitality
		Energy
		Leadership
		PkLevel
		PkCount
		MapPosX
		MapPosY
		QuestIndex
		Inventory

	dbo.GuildMember
		G_Name
	*/
}

/*
Name:
Quest Index:
Super Points:
Inventory:
*/