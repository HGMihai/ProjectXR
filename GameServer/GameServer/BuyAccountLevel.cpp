#include "stdafx.h"
#include "BuyAccountLevel.h"
#include "ServerInfo.h"
#include "Notice.h"
#include "Message.h"
#include "JSProtocol.h"
#include "BankEx.h"
#include "Util.h"

cBuyAccountLevel gBuyAccountLevel;

cBuyAccountLevel::cBuyAccountLevel()
{
	this->Init();
}

cBuyAccountLevel::~cBuyAccountLevel()
{
	
}

void cBuyAccountLevel::Init()
{
	this->m_Price_AL1 = 0;
	this->m_Price_AL2 = 0;
	this->m_Price_AL3 = 0;
}

void cBuyAccountLevel::Load(char* path)
{
	this->m_Price_AL1				= GetPrivateProfileInt("Common", "Price_AL1", 0, path);
	this->m_Price_AL2				= GetPrivateProfileInt("Common", "Price_AL2", 0, path);
	this->m_Price_AL3				= GetPrivateProfileInt("Common", "Price_AL3", 0, path);
}

void cBuyAccountLevel::SendInfo(int aIndex)
{
	PMSG_BUYAL_DATA pRequest;
	pRequest.Head.set(0xFB,0x09, sizeof(pRequest));

	pRequest.Price_AL1 = this->m_Price_AL1;
	pRequest.Price_AL2 = this->m_Price_AL2;
	pRequest.Price_AL3 = this->m_Price_AL3;

	pRequest.Exp_AL1 = gServerInfo.m_AddExperienceRate[1];
	pRequest.Exp_AL2 = gServerInfo.m_AddExperienceRate[2];
	pRequest.Exp_AL3 = gServerInfo.m_AddExperienceRate[3];

	pRequest.Drop_AL1 = gServerInfo.m_ItemDropRate[1];
	pRequest.Drop_AL2 = gServerInfo.m_ItemDropRate[2];
	pRequest.Drop_AL3 = gServerInfo.m_ItemDropRate[3];

	DataSend(aIndex, (LPBYTE)&pRequest, sizeof(pRequest));
}

bool cBuyAccountLevel::CheckReq(int Type, int Days, LPOBJ lpObj)
{

	if (Type == 1)
	{
		if (lpObj->BankEx.Credits < this->m_Price_AL1*Days)
		{
			return false;
		}
	}

	if (Type == 2)
	{
		if (lpObj->BankEx.Credits < this->m_Price_AL2*Days)
		{
			return false;
		}
	}

	if (Type == 3)
	{
		if (lpObj->BankEx.Credits < this->m_Price_AL3*Days)
		{
			return false;
		}
	}
	return true;
}

void cBuyAccountLevel::RecvBuyVip(CG_BUYVIP_RECV* Data, int aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];

	if (Data->vipType == 4)
	{
		this->CheckStatus(lpObj);
		return;
	}

	int days = 1;

	if (Data->daysType == 0)
	{
		days = 1;
	}
	else if (Data->daysType == 1)
	{
		days = 7;
	}
	else if (Data->daysType == 2)
	{
		days = 14;
	}
	else if (Data->daysType == 3)
	{
		days = 30;
	}

	if( !this->CheckReq(Data->vipType, days, lpObj) )
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, "You need more credits to buy VIP");
		return;
	}

	int Price = 0;
	if (Data->vipType == 1)
	{
		Price = this->m_Price_AL1*days;
	}
	else if (Data->vipType == 2)
	{
		Price = this->m_Price_AL2*days;
	}
	else if (Data->vipType == 3)
	{
		Price = this->m_Price_AL3*days;
	}

	lpObj->BankEx.Credits -= Price;

	GJAccountLevelSaveSend(lpObj->Index,Data->vipType,86400*days);
	GJAccountLevelSend(lpObj->Index);

	char szStatus[50];
	switch (Data->vipType)
	{
		case 1: sprintf(szStatus, "Silver"); break;
		case 2: sprintf(szStatus, "Gold"); break;
		case 3: sprintf(szStatus, "Platinum"); break;
	}
	gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, "You successfully buy %s VIP type!", szStatus);

	gBankEx.GCUpdateBankEx(lpObj->Index);
	gBankEx.GDSavePoint(lpObj->Index);
}

void cBuyAccountLevel::CheckStatus(LPOBJ lpObj)
{
	char szStatus[50];

	if (lpObj->AccountLevel < 1)
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, "[VIP] You do not have VIP status. Buy it!");
		return;
	}

	switch (lpObj->AccountLevel)
	{
		case 1: sprintf(szStatus, "Silver");	break;
		case 2: sprintf(szStatus, "Gold");		break;
		case 3: sprintf(szStatus, "Platinum");	break;
	}
	//this->SendMsg(lpObj->m_Index, 0, "@[VIP] You used %s vip status. Time left: %d", szStatus, lpObj->AccountExpireDate);
	gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage((  248+lpObj->AccountLevel)),lpObj->AccountExpireDate);
}