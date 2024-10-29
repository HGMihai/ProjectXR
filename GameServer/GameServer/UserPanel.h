#pragma once
#if GAMESERVER_UPDATE == 603 || IS_NEW_CLIENT
// -------------------------------------------------------------------------------

//#include "user.h"
#include "Protocol.h"
// -------------------------------------------------------------------------------

struct USERPANEL_DATA
{
	bool UserSettings;
	bool UserReset;
	bool UserGReset;
	bool UserPKClear;
	bool UserChangeClass;
	bool UserOffExp;
	bool UserBuyVip;
	bool UserPackJwls;
	bool UserCrafting;
	bool UserBankEx;
	bool UserPartySearch;
	bool UserSmithy;
	bool UserEventTimer;
};

class UserPanel
{
public:
			UserPanel();
			~UserPanel();
	// ----
	void	Load();
	void	ReadData(char * File);
	// ----
	void	SendUserPanel(int aIndex);
	// ----
private:
	/*bool BuyWCoinP;
	bool BuyWCoinP2;
	bool TradeWCoinP;
	bool HPBar;
	bool Effects;
	bool Reset;
	bool ChangeClass;
	bool BuyVip;
	bool Camera;
	bool Fog;
	bool GrandReset;
	bool OffExp;
	bool CameraReset;
	bool Time;
	bool PKClear;
	bool JewelMix;
	bool PartySearch;*/
	USERPANEL_DATA m_UserPanelData;
	// ----
}; extern UserPanel g_UserPanel;
// -------------------------------------------------------------------------------


#pragma pack(push, 1)
struct USERPANEL_REQ
{
	PSBMSG_HEAD	Head;
	// ----
	USERPANEL_DATA UserPanelData;
};
#pragma pack(pop)
// -------------------------------------------------------------------------------
#endif