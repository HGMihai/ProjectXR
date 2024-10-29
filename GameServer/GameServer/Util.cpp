#include "stdafx.h"
#include "Util.h"
#include "GameMain.h"
#include "HackCheck.h"
#include "ItemManager.h"
#include "SocketManager.h"
#include "Viewport.h"
#include "Log.h"
#include "Notice.h"
#include "Protection.h"

std::mt19937 seed;
std::uniform_int_distribution<int> dist;
short RoadPathTable[MAX_ROAD_PATH_TABLE] = { -1, -1, 0, -1, 1, -1, 1, 0, 1, 1, 0, 1, -1, 1, -1, 0 };

int SafeGetItem(int index) // OK
{
	return CHECK_ITEM(index);
}

void DatePlusDays( struct tm* date, int days )
{
    const time_t ONE_DAY = 24 * 60 * 60 ;

    // Seconds since start of epoch
    time_t date_seconds = mktime( date ) + (days * ONE_DAY) ;

    // Update caller's date
    // Use localtime because mktime converts to UTC so may change date
    *date = *localtime( &date_seconds ) ; ;
}

flt GetRoundValue(float value) // OK
{
	float integral;

	if(modf(value,&integral) > 0.5f)
	{
		return ceil(value);
	}

	return floor(value);
}

BYTE GetNewOptionCount(BYTE NewOption, BYTE decrease) // OK
{
	BYTE count = 0;

	for(int n=0;n < MAX_EXC_OPTION;n++)
	{
		if((NewOption & (1 << n)) != 0)
		{
			count++;
		}
	}

	count -= decrease;

	return count;
}

BYTE GetSocketOptionCount(BYTE SocketOption[5]) // OK
{
	BYTE count = 0;

	for(int n=0;n < MAX_SOCKET_OPTION;n++)
	{
		if(SocketOption[n] != 0xFF)
		{
			count++;
		}
	}

	return count;
}

BYTE GetPathPacketDirPos(int px,int py) // OK
{
	if(px <= -1 && py <= -1)
	{
		return 0;
	}
	else if(px <= -1 && py == 0)
	{
		return 7;
	}
	else if(px <= -1 && py >= 1)
	{
		return 6;
	}
	else if(px == 0 && py <= -1)
	{
		return 1;
	}
	else if(px == 0 && py >= 1)
	{
		return 5;
	}
	else if(px >= 1 && py <= -1)
	{
		return 2;
	}
	else if(px >= 1 && py == 0)
	{
		return 3;
	}
	else if(px >= 1 && py >= 1)
	{
		return 4;
	}

	return 0;
}

void PacketArgumentDecrypt(char* out_buff,char* in_buff,int size) // OK
{
	BYTE XorTable[3] = {0xFC,0xCF,0xAB};

	for(int n=0;n < size;n++)
	{
		out_buff[n] = in_buff[n]^XorTable[n%3];
	}
}

void ErrorMessageBox(char* message,...) // OK
{
	VM_START

	char buff[256];

	memset(buff,0,sizeof(buff));

	va_list arg;
	va_start(arg,message);
	vsprintf_s(buff,message,arg);
	va_end(arg);

	MessageBox(nullptr,buff,"Error",MB_OK | MB_ICONERROR);

	VM_END

	ExitProcess(0);
}

void MsgBox(char* message,...) // OK
{
	VM_START

	char buff[256];

	memset(buff,0,sizeof(buff));

	va_list arg;
	va_start(arg,message);
	vsprintf_s(buff,message,arg);
	va_end(arg);

	MessageBox(nullptr,buff,"Error",MB_OK | MB_ICONERROR);

	VM_END
}

void LogAdd(eLogColor color,char* text,...) // OK
{
#if (GAMESERVER_ROOT != 1)
	if (color == eLogColor::LOG_TEST)
	{
		return;
	}
#endif

	/*if (time(nullptr) > gProtection.m_end_license_date)
	{
		gProtection.is_license_end = true;
		gProtection.license_days_over = (CTime(time(nullptr)) - CTime(gProtection.m_end_license_date)).GetDays();
	}*/

	tm today;
	time_t ltime;
	time(&ltime);

	if(localtime_s(&today,&ltime) != 0)
	{
		return;
	}

	char time[32];

	if(asctime_s(time,sizeof(time),&today) != 0)
		return;

	char temp[1024];

	va_list arg;
	va_start(arg,text);
	vsprintf_s(temp,text,arg);
	va_end(arg);

	char log[1024];

	wsprintf(log," %.8s %s",&time[11],temp);

	if (gProtection.is_license_end)
	{
		gServerDisplayer.LogAddText(LOG_RED, end_date_text, strlen(end_date_text));

		if(gProtection.license_days_over >= 3)
			gNotice.GCNoticeSendToAll(0, 0, 0, 0, 0, 0, end_date_text);
	}

	gServerDisplayer.LogAddText(color,log,strlen(log));
}

bool DataSend(int aIndex,BYTE* lpMsg,DWORD size) // OK
{
	//gLog.Output(LOG_CONNECT, "[DataSend] %x - %x - %x - %x - %x", lpMsg[0], lpMsg[1], lpMsg[2], lpMsg[3], lpMsg[4]);
	//LogAdd(LOG_TEST,"[DataSend] %x - %x - %x - %x - %x",lpMsg[0],lpMsg[1],lpMsg[2],lpMsg[3],lpMsg[4]);
	return gSocketManager.DataSend(aIndex,lpMsg,size);
}

void DataSendAll(BYTE* lpMsg,int size) // OK
{
	for(int n=OBJECT_START_USER;n < MAX_OBJECT;n++)
	{
		if(gObjIsConnected(n) != 0)
		{
			DataSend(n,lpMsg,size);
		}
	}
}

bool DataSendSocket(SOCKET socket,BYTE* lpMsg,DWORD size) // OK
{
	if(socket == INVALID_SOCKET)
	{
		return false;
	}

	#if(ENCRYPT_STATE==1)

	EncryptData(lpMsg,size);

	#endif

	int count=0,result;

	while(size > 0)
	{
		if((result=send(socket,(char*)&lpMsg[count],size,0)) == SOCKET_ERROR)
		{
			if(WSAGetLastError() != WSAEWOULDBLOCK)
			{
				return false;
			}
		}
		else
		{
			count += result;
			size -= result;
		}
	}

	return true;
}

void MsgSendV2(LPOBJ lpObj,BYTE* lpMsg,int size) // OK
{
	for(int n=0;n < MAX_VIEWPORT;n++)
	{
		if(lpObj->VpPlayer2[n].state != VIEWPORT_NONE && lpObj->VpPlayer2[n].type == OBJECT_USER)
		{
			DataSend(lpObj->VpPlayer2[n].index,lpMsg,size);
		}
	}
}

void CloseClient(int aIndex) // OK
{
	gSocketManager.Disconnect(aIndex);
}

void PostMessage1(char* name,char* message,char* text) // OK
{
	#if(GAMESERVER_UPDATE>=701)

	char buff[256] = {0};

	wsprintf(buff,message,name,text);

	for(int n=OBJECT_START_USER;n < MAX_OBJECT;n++)
	{
		if(gObjIsConnectedGP(n) != 0)
		{
			GCNewMessageSend(&gObj[n],buff);
		}
	}

	#else

	char buff[256] = {0};

	wsprintf(buff,message,text);

	int size = strlen(buff);

	//LogAdd(LOG_TEST,"MessageSize = %d",size);

	size = ((size>MAX_CHAT_MESSAGE_SIZE)?MAX_CHAT_MESSAGE_SIZE:size);

	PMSG_CHAT_WHISPER_SEND pMsg;

	pMsg.header.set(0x02,(sizeof(pMsg)-(sizeof(pMsg.message)-(size+1))));

	memcpy(pMsg.name,name,sizeof(pMsg.name));

	memcpy(pMsg.message,buff,size);

	pMsg.message[size] = 0;

	for(int n=OBJECT_START_USER;n < MAX_OBJECT;n++)
	{
		if(gObjIsConnectedGP(n) != 0)
		{
			DataSend(n,(BYTE*)&pMsg,pMsg.header.size);
		}
	}

	#endif
}

void PostMessage2(char* name,char* message,char* text) // OK
{
	#if(GAMESERVER_UPDATE>=701)

	char buff[256] = {0};

	wsprintf(buff,message,name,text);

	for(int n=OBJECT_START_USER;n < MAX_OBJECT;n++)
	{
		if(gObjIsConnectedGP(n) != 0)
		{
			GCNewMessageSend(&gObj[n],buff);
		}
	}

	#else

	char buff[256] = {'~'};

	wsprintf(&buff[1],message,text);

	int size = strlen(buff);

	size = ((size>MAX_CHAT_MESSAGE_SIZE)?MAX_CHAT_MESSAGE_SIZE:size);

	PMSG_CHAT_SEND pMsg;

	pMsg.header.set(0x00,(sizeof(pMsg)-(sizeof(pMsg.message)-(size+1))));

	memcpy(pMsg.name,name,sizeof(pMsg.name));

	memcpy(pMsg.message,buff,size);

	pMsg.message[size] = 0;

	for(int n=OBJECT_START_USER;n < MAX_OBJECT;n++)
	{
		if(gObjIsConnectedGP(n) != 0)
		{
			DataSend(n,(BYTE*)&pMsg,pMsg.header.size);
		}
	}

	#endif
}

void PostMessage3(char* name,char* message,char* text) // OK
{
	#if(GAMESERVER_UPDATE>=701)

	char buff[256] = {0};

	wsprintf(buff,message,name,text);

	for(int n=OBJECT_START_USER;n < MAX_OBJECT;n++)
	{
		if(gObjIsConnectedGP(n) != 0)
		{
			GCNewMessageSend(&gObj[n],buff);
		}
	}

	#else

	char buff[256] = {'@'};

	wsprintf(&buff[1],message,text);

	int size = strlen(buff);

	size = ((size>MAX_CHAT_MESSAGE_SIZE)?MAX_CHAT_MESSAGE_SIZE:size);

	PMSG_CHAT_SEND pMsg;

	pMsg.header.set(0x00,(sizeof(pMsg)-(sizeof(pMsg.message)-(size+1))));

	memcpy(pMsg.name,name,sizeof(pMsg.name));

	memcpy(pMsg.message,buff,size);

	pMsg.message[size] = 0;

	for(int n=OBJECT_START_USER;n < MAX_OBJECT;n++)
	{
		if(gObjIsConnectedGP(n) != 0)
		{
			DataSend(n,(BYTE*)&pMsg,pMsg.header.size);
		}
	}

	#endif
}

void PostMessage4(char* name,char* message,char* text) // OK
{
	#if(GAMESERVER_UPDATE>=701)

	char buff[256] = {0};

	wsprintf(buff,message,name,text);

	for(int n=OBJECT_START_USER;n < MAX_OBJECT;n++)
	{
		if(gObjIsConnectedGP(n) != 0)
		{
			GCNewMessageSend(&gObj[n],buff);
		}
	}

	#else

	char buff[256] = {'$'};

	wsprintf(&buff[1],message,text);

	int size = strlen(buff);

	size = ((size>MAX_CHAT_MESSAGE_SIZE)?MAX_CHAT_MESSAGE_SIZE:size);

	PMSG_CHAT_SEND pMsg;

	pMsg.header.set(0x00,(sizeof(pMsg)-(sizeof(pMsg.message)-(size+1))));

	memcpy(pMsg.name,name,sizeof(pMsg.name));

	memcpy(pMsg.message,buff,size);

	pMsg.message[size] = 0;

	for(int n=OBJECT_START_USER;n < MAX_OBJECT;n++)
	{
		if(gObjIsConnectedGP(n) != 0)
		{
			DataSend(n,(BYTE*)&pMsg,pMsg.header.size);
		}
	}

	#endif
}

void SetLargeRand() // OK
{
	seed = std::mt19937(std::random_device()());
	dist = std::uniform_int_distribution<int>(0,2147483647);
}

long GetLargeRand() // OK
{
	return dist(seed);
}

bool IsBadFileLine(char *FileLine, int &Flag)
{
	if(Flag == 0)
	{
		if(isdigit(FileLine[0]))
		{
			Flag = FileLine[0] - 48;
			return true;
		}
	}
	else if(Flag < 0 || Flag > 9)
	{
		Flag = 0;
	}

	if(!strncmp(FileLine, "end", 3))
	{
		Flag = 0;
		return true;
	}

	if(FileLine[0] == '/' || FileLine[0] == '\n')
		return true;

	size_t size = strlen(FileLine);

	for(UINT i = 0; i < size; i++)
	{
		if(isalnum(FileLine[i]))
			return false;
	}
	return true;
}

float GetPrivateProfileFloat(char * section, char * key, float def_value, char * file)
{
	char	result[255];
	char	def[255];
	// ----
	wsprintf(def, "%f", def_value);
	GetPrivateProfileStringA(section, key, def, result, 255, file); 
	float float_result = atof(result);
	// ----
	return float_result;
}

bool FileExist(LPCSTR path)
{
	HANDLE file = CreateFile(path,GENERIC_READ,FILE_SHARE_READ,nullptr,OPEN_EXISTING,FILE_ATTRIBUTE_ARCHIVE,nullptr);

	if(file == INVALID_HANDLE_VALUE)
	{
		CloseHandle(file);
		return false;
	}

	CloseHandle(file);
	return true;
}

bool is_ring_item(int item_index)
{
	switch(item_index)
	{
		case GET_ITEM(13,8):
		case GET_ITEM(13,9):
		case GET_ITEM(13,21):
		case GET_ITEM(13,22):
		case GET_ITEM(13,23):
		case GET_ITEM(13,24):
			return true;
	}
	return false;
}

bool is_pendant_item(int item_index)
{
	switch(item_index)
	{
		case GET_ITEM(13,12):
		case GET_ITEM(13,13):
		case GET_ITEM(13,25):
		case GET_ITEM(13,26):
		case GET_ITEM(13,27):
		case GET_ITEM(13,28):
			return true;
	}
	return false;
}

void GC_RequestSend(int aIndex, BYTE head, BYTE subh)
{
	PMSG_REQUEST pMsg;
	pMsg.header.set(head, subh, sizeof(pMsg));
	DataSend(aIndex, (BYTE*)&pMsg, pMsg.header.size);
}

int getNumberOfExcOptions(int checksum)
{
	int opt_count=0;
	int opt_codes[6];

	opt_codes[0]=1;
	opt_codes[1]=2;
	opt_codes[2]=4;
	opt_codes[3]=8;
	opt_codes[4]=16;
	opt_codes[5]=32;

	if (checksum>0)
	{
		for (int i=0;i<6;i++)
		{
			int and_val=checksum & opt_codes[i];
			if (and_val != 0)
				opt_count+=1;
		}
	}

	return opt_count;
}

#if (GAMESERVER_UPDATE > 803)

void SXGetCharacterInfo(int aIndex)
{
		/*unsigned char jj[]={0xC1,0x94,0xEC,0x29,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1C,0x00,0x00,0x00,0x14,0x00,0x00,0x00,0x19,0x00,0x00,0x00,0x0A,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xA4,0x70,0x3D,0x3F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x66,0x66,0xA6,0x3F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xC0,0x40,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x24
};*/
#if(LSTNAME)
	PROTECT_START
#endif
		PMSG_SXCHARINFO pMsg;
		memset(&pMsg,0,sizeof(pMsg));

		LPOBJ lpObj = &gObj[aIndex];

		pMsg.h.set(0xEC, 0x29, sizeof(PMSG_SXCHARINFO));

		pMsg.CriticalDamage=lpObj->CriticalDamage+lpObj->EffectOption.AddCriticalDamage;

		pMsg.ExcellentDamage=lpObj->ExcellentDamage+lpObj->EffectOption.AddExcellentDamage;

		pMsg.SkillDamageBonus =lpObj->SkillDamageBonus;

		pMsg.Defense = lpObj->Defense;

		pMsg.Str=lpObj->Strength;

		pMsg.AddStr=lpObj->AddStrength;

		pMsg.Dex=lpObj->Dexterity;

		pMsg.AddDex=lpObj->AddDexterity;

		pMsg.Vit=lpObj->Vitality;

		pMsg.AddVit=lpObj->AddVitality;

		pMsg.Energy=lpObj->Energy;

		pMsg.AddEnergy=lpObj->AddEnergy;

		pMsg.Leadership=lpObj->Leadership;

		pMsg.AddLeadership=lpObj->AddLeadership;

		pMsg.SDAttack=0;

		pMsg.IgnoreShieldGaugeRate=lpObj->IgnoreShieldGaugeRate;

		pMsg.SDAttack1=0;

		pMsg.MoneyAmountDropRate=100-lpObj->MoneyAmountDropRate;

		pMsg.IgnoreDefenseRate= (float)0;//(lpObj->m_MPSkillOpt.IgnoreDefenseRate+lpObj->EffectOption.AddIgnoreDefenseRate);

		pMsg.HPRecovery= (float)lpObj->HPRecovery+lpObj->EffectOption.AddHPRecovery;

		pMsg.MPRecovery= (float)lpObj->MPRecovery+lpObj->EffectOption.AddMPRecovery;

		
		pMsg.ResistStunRate = (float)lpObj->ResistStunRate;
		
		pMsg.ShieldDamageReduction = (float)lpObj->ShieldDamageReduction;

		pMsg.fTripleDamageRationInfo = 0;//lpObj->m_MPSkillOpt.TripleDamageRate;

		pMsg.fMonsterDieGetHP_info = (float)lpObj->HuntHP;

		pMsg.fMonsterDieGetMana_info = (float)lpObj->HuntMP;

		pMsg.fMonsterDieGetSD_info = (float)lpObj->HuntSD;

		pMsg.SDRecovery= (float)(lpObj->SDRecovery+lpObj->EffectOption.AddSDRecovery);

		pMsg.DefensiveFullMPRestoreRate= (float)0;//lpObj->m_MPSkillOpt.DefensiveFullMPRestoreRate;

		pMsg.DefensiveFullHPRestoreRate= (float)0;//lpObj->m_MPSkillOpt.DefensiveFullHPRestoreRate;

		pMsg.OffensiveFullSDRestoreRate = (float)0;//lpObj->m_MPSkillOpt.OffensiveFullSDRestoreRate;

		pMsg.BPRecovery= (float)(lpObj->BPRecovery+lpObj->EffectOption.AddBPRecovery);
		//
		pMsg.BlockRate = 0;//lpObj->BlockRate;

		pMsg.ParryRate = 0;//lpObj->ParryRate;

		pMsg.AbsorbLife = 0;//lpObj->AbsorbLife;

		pMsg.AbsorbSD=0;//lpObj->AbsorbSD;

		pMsg.FullDamageReflectRate = (float)0;//lpObj->m_MPSkillOpt.FullDamageReflectRate;
		//
		pMsg.MPConsumptionRate = (float)0;//(100-lpObj->MPConsumptionRate);

		pMsg.CriticalDamageRate= (float)0;//(lpObj->m_MPSkillOpt.CriticalDamageRate + lpObj->EffectOption.AddCriticalDamageRate);

		pMsg.ExcellentDamageRate= (float)0;//(lpObj->m_MPSkillOpt.ExcellentDamageRate + lpObj->EffectOption.AddExcellentDamageRate);

		pMsg.DoubleDamageRate=(float)0;// lpObj->m_MPSkillOpt.DoubleDamageRate;


		for(int n=0;n < MAX_DAMAGE_REDUCTION;n++)
		{
			pMsg.DamageReduction += lpObj->DamageReduction[n];
		}
		pMsg.DamageReduction +=lpObj->EffectOption.AddDamageReduction;

		pMsg.BPConsumptionRate =lpObj->BPConsumptionRate;

		pMsg.DamageReflect = lpObj->DamageReflect+lpObj->EffectOption.AddDamageReflect;

		pMsg.StunRate = gMasterSkillTree.GetMasterSkillValue(lpObj, MASTER_SKILL_ADD_MACE_MASTERY);
				//pMsg.unk2 = 0;
		pMsg.fWingDamageIncRate_info = 0;//lpObj->DamageIncRate;

		pMsg.fWingDamageAbsorb_info = 0;//lpObj->DamageDecRate;
				//pMsg.unk5 = 0;

		DataSend(lpObj->Index, (LPBYTE)&pMsg, sizeof(PMSG_SXCHARINFO));
#if(LSTNAME)
		PROTECT_FINAL
#endif
}

void GCCharacterInfo(LPOBJ lpObj)
{	//C1 1C EC 25 20 00 00 00 14 00 00 00 19 00 00 00 0A 00 00 00 00 00 00 00 A2 45 76 3F 
#if(LSTNAME)
	PROTECT_START
#endif
	PMSG_SXUPPRONT pMsg;

	pMsg.h.set(0xEC, 0x25, sizeof(PMSG_SXUPPRONT));

	pMsg.h.size = sizeof(PMSG_SXUPPRONT);

	pMsg.Str = lpObj->Strength;

	pMsg.AddStr = lpObj->AddStrength;

	pMsg.Dex = lpObj->Dexterity;

	pMsg.AddDex = lpObj->AddDexterity;

	pMsg.Vit = lpObj->Vitality;

	pMsg.AddVit = lpObj->AddVitality;

	pMsg.Ene = lpObj->Energy;

	pMsg.AddEne = lpObj->AddEnergy;

	pMsg.Leadership=lpObj->Leadership;

	pMsg.AddLeadership = lpObj->AddLeadership;

	pMsg.mPrec = 10;//lpObj->m_MPSkillOpt.MPRecoveryRate + lpObj->MPRecovery;

	DataSend(lpObj->Index, (LPBYTE)&pMsg, sizeof(PMSG_SXUPPRONT));
#if(LSTNAME)
	PROTECT_FINAL
#endif
}

void SXGetMosaic(int aIndex)//ÏâÇ¶
{
#if(LSTNAME)
	PROTECT_START
#endif
	PMSG_SXMOSAIC pMsg;
	memset(&pMsg,0,sizeof(pMsg));
	LPOBJ lpObj = &gObj[aIndex];
	pMsg.h.set(0xEC,0x26,sizeof(pMsg));
	pMsg.nAttributeAttackPowerMin_info =lpObj->ElementalDamageMin;
	pMsg.nAttributeAttackPowerMax_info =lpObj->ElementalDamageMax;
	pMsg.nAttributeAttackPowerPVP_Min_info =lpObj->ElementalDamageMin;
	pMsg.nAttributeAttackPowerPVP_Max_info =lpObj->ElementalDamageMax;
	pMsg.nAttributeAttackSuccessRatePVE_info = lpObj->ElementalAttackSuccessRate;
	pMsg.nAttributeAttackSuccessRatePVP_info = lpObj->ElementalAttackSuccessRate;
	pMsg.nAttributeDefencePowerPVE_info = lpObj->ElementalDefense;
	pMsg.nAttributeDefencePowerPVP_info = lpObj->ElementalDefense;
	pMsg.nAttributeDefenceSuccessRatePVE_info = lpObj->ElementalDefenseSuccessRate;
	pMsg.nAttributeDefenceSuccessRatePVP_info = lpObj->ElementalDefenseSuccessRate;

	pMsg.nRubyAttackType = lpObj->ElementalAttribute;//»ð
	pMsg.nRubyAttackValue =0;

	pMsg.nEmeAttackType = 0;
	pMsg.nEmeAttackValue = 0;	//Ë®
	pMsg.nSapphDefeseType = 0;
	pMsg.nSapphDefeseValue =0;	//ÍÁ
	pMsg.nTopaAttackType = 0;
	pMsg.nTopaAttackAttack = 0;	//·ç

	pMsg.nAttributeDamageInc_PVE_info = 0;
	pMsg.nAttributeDamageInc_PVP_info = 0;
	pMsg.nAttributeDamageAbsorbs_PVE_info = 0;
	pMsg.nAttributeDamageAbsorbs_PVP_info = 0;
	pMsg.nAttributeShieldAbsortion_info = 0;
	pMsg.nAttributeLifeAbsortion_info = 0;
	pMsg.nAttributeBleedingEffectOccur_info = 0;//lpObj->PentagramJewelOption.Bleeding;
	pMsg.nAttributeEffectsCausedParalysis_info = 0;//lpObj->PentagramJewelOption.Paralyzing;
	pMsg.nAttributeBoundEffectOccur_info = 0;
	pMsg.nAttributeWraithEffectOccur_info = 0;
	pMsg.nAttributeEffectsCausedBlindness_info = 0;//lpObj->PentagramJewelOption.Binding;
	pMsg.nAttributeResistantToStrongProperty_info = 0;
	pMsg.nAttributeResistantToAttributeAttacks_info = 0;
	pMsg.nAttributeAddBunNoErtelOptionEffect_info = 0;
	pMsg.nAttributeAddGaHoErtelOptionEffect_info = 0;
	pMsg.nAttributeCriticalDamageRate_info = lpObj->PentagramOption.AddElementalCriticalDamageRate;


	DataSend(lpObj->Index, (LPBYTE)&pMsg, sizeof(PMSG_SXMOSAIC));
#if(LSTNAME)
	PROTECT_FINAL
#endif
}
#endif