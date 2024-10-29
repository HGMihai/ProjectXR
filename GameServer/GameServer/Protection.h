#pragma once

#pragma comment(lib, "iphlpapi.lib")
#pragma comment(lib, "Wininet")

#define PROTECT_START _asm _emit 0xEB _asm _emit 0x0E _asm _emit 0x14 _asm _emit 0x28 _asm _emit 0xCD _asm _emit 0xDB _asm _emit 0x50 _asm _emit 0x63 _asm _emit 0xAF _asm _emit 0x22 _asm _emit 0x75 _asm _emit 0xEE _asm _emit 0xB5 _asm _emit 0xD8 _asm _emit 0x43 _asm _emit 0x72
#define PROTECT_FINAL _asm _emit 0xEB _asm _emit 0x0E _asm _emit 0x37 _asm _emit 0xF2 _asm _emit 0xCD _asm _emit 0xDB _asm _emit 0x50 _asm _emit 0x63 _asm _emit 0xAF _asm _emit 0x22 _asm _emit 0x75 _asm _emit 0xEE _asm _emit 0xB5 _asm _emit 0xD8 _asm _emit 0x85 _asm _emit 0x61
#define CRASH_APPLICATION_MACRO _asm _emit 0xFF _asm _emit 0x25 _asm _emit 0x00 _asm _emit 0x00 _asm _emit 0x00 _asm _emit 0x00

#define CUSTOMS_COUNT 22

//9CB7A7BE-7E74038E-6D1903D5-AB21E109
enum eCustomsType
{
	CUSTOM_MARRY,
	CUSTOM_DIABLO,
	CUSTOM_QUEST_SYSTEM,
	CUSTOM_VOTEPARCER,
	CUSTOM_TVTEVENT,
	CUSTOM_STATSADD,
	CUSTOM_EXPANDED_CHARACTER,
	CUSTOM_PARTY_SEARCH,
	CUSTOM_OFFLINE_MODE,
	CUSTOM_SMITHY,
	CUSTOM_BUFFICON,
	CUSTOM_BUFFS,
	CUSTOM_PETS,
	CUSTOM_EVENTTIMER,
	CUSTOM_GUILD_WAREHOUSE,
	CUSTOM_ACHIEVEMENTS,
	CUSTOM_RANKINGS,
	CUSTOM_OPTION_REPLACE,
	CUSTOM_NEW_PARTY,
	ANTIHACK_SCREEN_SHOW,
	CUSTOM_ACTIVE_INVASIONS,
	CUSTOM_STAT,
};

enum eCustomers
{
	ant = 1,
	CrystalMu = 2,
	Freedom2 = 5,
	WorkPC = 7,
	mufighters = 9,
	AndreiMedoni = 10,
	DARKSOUL = 25,
	MuFireland = 31,
	MuLambda = 36,
	Creative = 48,
	SixtiFive = 50,
	Fairy_MU = 51,
	WARMU = 56,
	tararim = 67,
	MuLosenaElmar = 69,
	deep = 98,
	House_LTP = 112,
	Daniel = 118,
	eclips_new = 119,
	web_mous = 127,
	EvelinMU = 133,
	LIFE = 140,
	voodooz_1992 = 141,
	eclips_setup = 148,
	abyssalmu = 150,
	boruga = 155,
	DmitryBalan = 160,
	dextermu = 163,
	MacOS = 164,
	Marks = 171,
	LastHope = 175,
	CLASHMU = 177,
	MUFORTUNE = 182,
	novamu = 183,
	SkyNet = 184,
	MuFireland2 = 186,
	BloodGameMU = 187,
	RMOS = 188,
	SPARTAMU = 189,
	koren = 190,
	Galin = 191,
	Mihail = 192,
	Arturs = 193,
	Cristian = 194,
	muheroia = 195,
	radicalmu = 196,
	sadri = 197,
	zion = 198,
	mured = 199,
	Faerlina = 200,
	alex = 201,
	GeorgeBabaru = 202,
};

class CProtection
{
public:
	CProtection();
	virtual ~CProtection() = default;

	void Auth(HINSTANCE hIns, HWND hWnd);
	inline void ErrorMessageBox1();
	void InfoBox(char* message, ...);
	//void ConnectionErrorBox();
	static inline void SafeExitProcess();
	void SetCustomState(int index, bool value);
	bool GetCustomState(eCustomsType index);
	void InitDialog(HWND hWnd);
	bool GetTeleportState();
	bool check_access(eCustomers index) { return (this->m_Index == index) ? true : false; }
public:
	//char m_ComputerHardwareId[36];
	char m_ComputerHardwareId[256];
	BYTE m_EncKey;
	HINSTANCE hInst;
	HWND ghWnd;
	char CustomerName[100];
	char ipAddr[16];
	int m_end_license_date;
	char end_license_date_text[50];
	bool is_license_end;
	int license_days_over;
	int m_LicenseActive;
	int m_Debt;
	int m_Index;
	std::map<int, bool> m_CustomsData;

}; extern CProtection gProtection;

inline char* end_date_text = "Your license are expired.";
