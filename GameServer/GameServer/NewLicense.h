//#include <Iphlpapi.h>
//#pragma comment(lib, "iphlpapi.lib")
//#pragma comment(lib, "Wininet")
//
//#define Customers 5
//
//#define	LICENSE_SERVER			"ihr`ob`/k(utdl?us~*.E`re"
//#define LICENSE_MAX_CUSTOMID	1
//
//struct CUSTOMER_DATA1
//{
//	WORD	CustomerID;
//	bool	CustomState[LICENSE_MAX_CUSTOMID];
//	DWORD	DriveID;
//	BYTE	Adapter[MAX_ADAPTER_ADDRESS_LENGTH];
//	int		CurrentDay;
//	char	Hash1[100];
//	char	Hash2[100];
//	//char	Hash3[100];
//	char	Name[20];
//};
//
//class License
//{
//public:
//			License();
//			~License();
//	// ----
//	LPSTR	GetText(LPSTR EncodedChar);
//	void	GetAdapter();
//	int		HDD_ID;
//	void	CheckKey();
//	void	GetHash();
//	void	Load(HINSTANCE hInst, HWND ghWnd);
//	void	GetDriveID();
//	void	GetCustomerName();
//	void	SetDash();
//	// ----
//	// ----
//	char CustomerName[100];
//	HINSTANCE hInst;
//	HWND ghWnd;
//	bool IsHomePC;
//	// ----
//	CUSTOMER_DATA1 m_Data;
//	// ----
//}; extern License gLicense;
//
//extern bool IsLicenseChecked;