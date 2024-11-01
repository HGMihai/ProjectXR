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
	/*237*/	WORD CriticalDamage;//幸运一击增加伤害
	/*239*/	WORD ExcellentDamage;//卓越一击增加伤害
	/*241*/	WORD SkillDamageBonus;//技能攻击力增加
	/*243*/	WORD Defense;//基本防御力
	/*245*/	WORD Str;//力量    //12
	/*247*/	WORD AddStr;//附加力量
	/*249*/	WORD Dex;//敏捷  //16
	/*251*/	WORD AddDex;//附加敏捷
	/*253*/	WORD Vit;//体力	 //20
	/*255*/	WORD AddVit;//附加体力
	/*257*/	WORD Energy;//智力  //24
	/*259*/	WORD AddEnergy;//附加智力
	/*261*/	WORD Leadership;//统率   //28
	/*263*/	WORD AddLeadership;
	/*265*/	WORD SDAttack;//袭击时SD比率%
	/*267*/	WORD IgnoreShieldGaugeRate;//SD无视几率%
	/*269*/	WORD SDAttack1;//攻击时SD比率%
	/*271*/	WORD MoneyAmountDropRate;//获得金币增加率%
	/*273*/	float IgnoreDefenseRate;//无视防御力几率%
	/*277*/	float HPRecovery; //生命自动恢复量
	/*281*/	float MPRecovery;//魔法恢复量
	/*285*/	float StunRate;//束缚几率
	/*289*/	float ResistStunRate;//束缚抵抗几率%
	/*293*/	float fTripleDamageRationInfo;
	/*297*/	float ShieldDamageReduction;//盾牌吸收伤害
	/*301*/	float fMonsterDieGetHP_info;//怪物死亡生命恢复量
	/*305*/	float fMonsterDieGetMana_info;//怪物死亡魔法恢复量
	/*309*/	float fMonsterDieGetSD_info;//怪物死亡SD恢复量
	/*313*/	float SDRecovery;//SD自动恢复量
	/*317*/	float DefensiveFullMPRestoreRate;//魔法值完全恢复几率
	/*321*/	float DefensiveFullHPRestoreRate;//生命完全恢复几率
	/*325*/	float OffensiveFullSDRestoreRate;//SD完全恢复几率
	/*329*/	float BPRecovery;//AG自动恢复量
	/*333*/	float fWingDamageAbsorb_info;//伤害吸收率
	/*337*/	float BlockRate;//盾防御几率
	/*341*/	float ParryRate;//武器格档几率
	/*345*/	float AbsorbLife;//生命力吸收量
	/*349*/	float AbsorbSD;//SD吸收量
	/*353*/	float FullDamageReflectRate;//反弹攻击几率
	/*357*/	float fWingDamageIncRate_info;//伤害提高率
	/*361*/	float MPConsumptionRate;//魔法使用减少率
	/*365*/	float CriticalDamageRate;//幸运一击伤害几率
	/*369*/	float ExcellentDamageRate;//卓越一击伤害几率
	/*373*/	float DoubleDamageRate;//双倍伤害几率
	// /*377*/	float  unk5;
	/*381*/	BYTE  DamageReduction;//伤害减少率
	/*382*/	BYTE  BPConsumptionRate;//AG使用减少率
	/*383*/	BYTE  DamageReflect;//伤害反射率
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
/*76*/		int nAttributeCriticalDamageRate_info;//属性致命一击
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
///**/		int DefenseRecovery;//防御恢复
///**/		int AbsorbHP;//生命吸收%
///**/		int Bleeding;//产生出血
///**/		int Paralyzing;//产生麻痹
///**/		int Bind;//产生束缚
///**/		int Punish;//产生天罚
///**/		int Blind;//产生失明效果的抵抗力%
///**/		int RestoStrElem;//对强大属性的抵抗力%
///**/		int RestoEleDmg;//对属性攻击的抵抗力%
};