#include "StdAfx.h"
#include "Protection.h"
#if GAMESERVER_UPDATE == 603 || IS_NEW_CLIENT
#include "UserPanel.h"
#include "GameMain.h"
#include "Util.h"
#include "Path.h"
// -------------------------------------------------------------------------------

UserPanel g_UserPanel;
// -------------------------------------------------------------------------------

UserPanel::UserPanel()
{

}
// -------------------------------------------------------------------------------

UserPanel::~UserPanel()
{

}
// -------------------------------------------------------------------------------

void UserPanel::Load()
{
	this->ReadData(gPath.GetFullPath("..\\LTPData\\UserPanel.ini"));
}
// -------------------------------------------------------------------------------

void UserPanel::ReadData(char * File)
{
	this->m_UserPanelData.UserSettings		 = GetPrivateProfileInt("UserPanel", "Settings", 0, File);	
	this->m_UserPanelData.UserReset			 = GetPrivateProfileInt("UserPanel", "Reset", 0, File);
	this->m_UserPanelData.UserGReset		 = GetPrivateProfileInt("UserPanel", "GReset", 0, File);
	this->m_UserPanelData.UserPKClear		 = GetPrivateProfileInt("UserPanel", "PKClear", 0, File);
	this->m_UserPanelData.UserChangeClass	 = GetPrivateProfileInt("UserPanel", "ChangeClass", 0, File);
	this->m_UserPanelData.UserOffExp		 = GetPrivateProfileInt("UserPanel", "OffExp", 0, File);
	this->m_UserPanelData.UserBuyVip		 = GetPrivateProfileInt("UserPanel", "BuyVip", 0, File);
	this->m_UserPanelData.UserPackJwls		 = GetPrivateProfileInt("UserPanel", "PackJewels", 0, File);
	this->m_UserPanelData.UserCrafting		 = GetPrivateProfileInt("UserPanel", "Crafting", 0, File);
	this->m_UserPanelData.UserBankEx		 = GetPrivateProfileInt("UserPanel", "BankEx", 0, File);
	this->m_UserPanelData.UserPartySearch	 = GetPrivateProfileInt("UserPanel", "PartySearch", 0, File);
	this->m_UserPanelData.UserSmithy		 = GetPrivateProfileInt("UserPanel", "Smithy", 0, File);
	this->m_UserPanelData.UserEventTimer	= GetPrivateProfileInt("UserPanel", "EventTimer", 0, File);
}
// -------------------------------------------------------------------------------

void UserPanel::SendUserPanel(int aIndex)
{
	USERPANEL_REQ pRequest;
	pRequest.Head.set(0xFB,0x00, sizeof(pRequest));

	// ----
	pRequest.UserPanelData = this->m_UserPanelData;
	//pRequest.BuyWCoinP			= this->BuyWCoinP;
	//pRequest.BuyWCoinP2			= this->BuyWCoinP2;
	//pRequest.TradeWCoinP		= this->TradeWCoinP;
	//pRequest.HPBar				= this->HPBar;
	//pRequest.Effects			= this->Effects;
	//pRequest.Reset				= this->Reset;
	//pRequest.ChangeClass		= this->ChangeClass;
	//pRequest.BuyVip				= this->BuyVip;
	//pRequest.Camera				= this->Camera;
	//pRequest.Fog				= this->Fog;
	//pRequest.GrandReset			= this->GrandReset;
	//pRequest.OffExp				= this->OffExp;
	//pRequest.CameraReset		= this->CameraReset;
	//pRequest.Time				= this->Time;
	//pRequest.PKClear			= this->PKClear;
	//pRequest.JewelMix			= this->JewelMix;
	//pRequest.PartySearch		= this->PartySearch;
	//// ----
	//if (gProtection.GetCustomState(CUSTOM_PARTY_SEARCH) == 0)
	//{
	//	pRequest.PartySearch		= false;
	//}
	// ----
	DataSend(aIndex, (LPBYTE)&pRequest, sizeof(pRequest));
}
// -------------------------------------------------------------------------------
#endif