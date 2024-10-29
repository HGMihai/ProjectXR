#pragma once

#include "ServerDisplayer.h"
#include "User.h"
#include "Protocol.h"

int SafeGetItem(int index);
void DatePlusDays( struct tm* date, int days );
flt GetRoundValue(float value);
BYTE GetNewOptionCount(BYTE NewOption, BYTE decrease=0);
BYTE GetSocketOptionCount(BYTE SocketOption[5]);
BYTE GetPathPacketDirPos(int px,int py);
void PacketArgumentDecrypt(char* out_buff,char* in_buff,int size);
void ErrorMessageBox(char* message,...);
void MsgBox(char* message,...);
void LogAdd(eLogColor color,char* text,...);
bool DataSend(int aIndex,BYTE* lpMsg,DWORD size);
void DataSendAll(BYTE* lpMsg,int size);
bool DataSendSocket(SOCKET socket,BYTE* lpMsg,DWORD size);
void MsgSendV2(LPOBJ lpObj,BYTE* lpMsg,int size);
void CloseClient(int aIndex);
void PostMessage1(char* name,char* message,char* text);
void PostMessage2(char* name,char* message,char* text);
void PostMessage3(char* name,char* message,char* text);
void PostMessage4(char* name,char* message,char* text);
void SetLargeRand();
long GetLargeRand();
bool IsBadFileLine(char *FileLine, int &Flag);
float GetPrivateProfileFloat(char * section, char * key, float def_value, char * file);
bool FileExist(LPCSTR path);
bool is_ring_item(int item_index);
bool is_pendant_item(int item_index);
void GC_RequestSend(int aIndex, BYTE head, BYTE subh);

int getNumberOfExcOptions(int checksum);

inline unsigned long RGBA(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha)
{
	return (red)+(green << 8) + (blue << 16) + (alpha << 24);
}

void SXGetCharacterInfo(int aIndex);
void GCCharacterInfo(LPOBJ lpObj);
void SXGetMosaic(int aIndex);

extern short RoadPathTable[MAX_ROAD_PATH_TABLE];

struct PMSG_SXUPPRONT
{
	PSBMSG_HEAD h;
	WORD Str;
	WORD AddStr;
	WORD Dex;
	WORD AddDex;
	WORD Vit;
	WORD AddVit;
	WORD Ene;
	WORD AddEne;
	WORD Leadership;
	WORD AddLeadership;
	float mPrec;
};

struct PMSG_SXCHARINFO
{
	/*233*/	PSBMSG_HEAD h;  //0-3
	/*237*/	WORD CriticalDamage;//����һ�������˺�
	/*239*/	WORD ExcellentDamage;//׿Խһ�������˺�
	/*241*/	WORD SkillDamageBonus;//���ܹ���������
	/*243*/	WORD Defense;//����������
	/*245*/	WORD Str;//����    //12
	/*247*/	WORD AddStr;//��������
	/*249*/	WORD Dex;//����  //16
	/*251*/	WORD AddDex;//��������
	/*253*/	WORD Vit;//����	 //20
	/*255*/	WORD AddVit;//��������
	/*257*/	WORD Energy;//����  //24
	/*259*/	WORD AddEnergy;//��������
	/*261*/	WORD Leadership;//ͳ��   //28
	/*263*/	WORD AddLeadership;
	/*265*/	WORD SDAttack;//Ϯ��ʱSD����%
	/*267*/	WORD IgnoreShieldGaugeRate;//SD���Ӽ���%
	/*269*/	WORD SDAttack1;//����ʱSD����%
	/*271*/	WORD MoneyAmountDropRate;//��ý��������%
	/*273*/	float IgnoreDefenseRate;//���ӷ���������%
	/*277*/	float HPRecovery; //�����Զ��ָ���
	/*281*/	float MPRecovery;//ħ���ָ���
	/*285*/	float StunRate;//��������
	/*289*/	float ResistStunRate;//�����ֿ�����%
	/*293*/	float fTripleDamageRationInfo;
	/*297*/	float ShieldDamageReduction;//���������˺�
	/*301*/	float fMonsterDieGetHP_info;//�������������ָ���
	/*305*/	float fMonsterDieGetMana_info;//��������ħ���ָ���
	/*309*/	float fMonsterDieGetSD_info;//��������SD�ָ���
	/*313*/	float SDRecovery;//SD�Զ��ָ���
	/*317*/	float DefensiveFullMPRestoreRate;//ħ��ֵ��ȫ�ָ�����
	/*321*/	float DefensiveFullHPRestoreRate;//������ȫ�ָ�����
	/*325*/	float OffensiveFullSDRestoreRate;//SD��ȫ�ָ�����
	/*329*/	float BPRecovery;//AG�Զ��ָ���
	/*333*/	float fWingDamageAbsorb_info;//�˺�������
	/*337*/	float BlockRate;//�ܷ�������
	/*341*/	float ParryRate;//�����񵵼���
	/*345*/	float AbsorbLife;//������������
	/*349*/	float AbsorbSD;//SD������
	/*353*/	float FullDamageReflectRate;//������������
	/*357*/	float fWingDamageIncRate_info;//�˺������
	/*361*/	float MPConsumptionRate;//ħ��ʹ�ü�����
	/*365*/	float CriticalDamageRate;//����һ���˺�����
	/*369*/	float ExcellentDamageRate;//׿Խһ���˺�����
	/*373*/	float DoubleDamageRate;//˫���˺�����
	// /*377*/	float  unk5;
	/*381*/	BYTE  DamageReduction;//�˺�������
	/*382*/	BYTE  BPConsumptionRate;//AGʹ�ü�����
	/*383*/	BYTE  DamageReflect;//�˺�������
	///*384*/	BYTE  unk36;

};

struct PMSG_SXMOSAIC
{
/**/		PSBMSG_HEAD h;
/*4*/		int nAttributeAttackPowerMin_info;
/*8*/		int nAttributeAttackPowerMax_info;
/*12*/		int nAttributeAttackPowerPVP_Min_info;
/*16*/		int nAttributeAttackPowerPVP_Max_info;
/*20*/		int nAttributeAttackSuccessRatePVE_info;
/*24*/		int nAttributeAttackSuccessRatePVP_info;
/*28*/		int nAttributeDefencePowerPVE_info;
/*32*/		int nAttributeDefencePowerPVP_info;
/*36*/		int nAttributeDefenceSuccessRatePVE_info;
/*40*/		int nAttributeDefenceSuccessRatePVP_info;
/**/		int nRubyAttackType;
/**/		int nRubyAttackValue;
/**/		int nEmeAttackType;//2
/**/		int nEmeAttackValue;
/**/		int nSapphDefeseType;
/**/		int nSapphDefeseValue;
/**/		int nTopaAttackType;
/**/		int nTopaAttackAttack;
/*76*/		int nAttributeCriticalDamageRate_info;//��������һ��
			int nAttributeDamageInc_PVE_info;
			int nAttributeDamageInc_PVP_info;
			int nAttributeDamageAbsorbs_PVE_info;
			int nAttributeDamageAbsorbs_PVP_info;
			int nAttributeShieldAbsortion_info;
			int nAttributeLifeAbsortion_info;
			int nAttributeBleedingEffectOccur_info;
			int nAttributeEffectsCausedParalysis_info;
			int nAttributeBoundEffectOccur_info;
			int nAttributeWraithEffectOccur_info;
			int nAttributeEffectsCausedBlindness_info;
			int nAttributeResistantToStrongProperty_info;
			int nAttributeResistantToAttributeAttacks_info;
			int nAttributeAddBunNoErtelOptionEffect_info;
			int nAttributeAddGaHoErtelOptionEffect_info;



///*80*/		int unk8;
///*84*/		int Type6;
///**/		int unk10;
///**/		int unk11;
///**/		int DefenseRecovery;//�����ָ�
///**/		int AbsorbHP;//��������%
///**/		int Bleeding;//������Ѫ
///**/		int Paralyzing;//�������
///**/		int Bind;//��������
///**/		int Punish;//�����췣
///**/		int Blind;//����ʧ��Ч���ĵֿ���%
///**/		int RestoStrElem;//��ǿ�����Եĵֿ���%
///**/		int RestoEleDmg;//�����Թ����ĵֿ���%
};