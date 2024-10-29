#pragma once
#include "User.h"
#include "Protocol.h"
struct PackSystemData
{
	int JewelID;
	int JewelBundleID;
	int Price10;
	int Price20;
	int Price30;
	int PriceUnPack10;
	int PriceUnPack20;
	int PriceUnPack30;
};

struct PMSG_SEND_PACKSYSTEM
{
	PSWMSG_HEAD header;
	int Count;
};

struct CG_MIX_RECV
{
	PSBMSG_HEAD	h;
	int index;
	int level;
};

struct CG_UNMIX_RECV
{
	PSBMSG_HEAD	h;
	int index;
	int level;
};

struct CG_MIX_RESULT_SEND
{
	PSBMSG_HEAD	h;
	BYTE result;
};

class CPackSystem
{
public:
	/*CPackSystem();
	~CPackSystem();*/
	// ----
	void Load(char* path);
	void GCSendData(int aIndex);
	void CGMixRecv(CG_MIX_RECV* lpMsg, int aIndex);
	void CGUnMixRecv(CG_UNMIX_RECV* lpMsg, int aIndex);
	void GCPackSystemResultSend(int aIndex,BYTE result);
private:
	std::vector<PackSystemData> m_Data;
}; extern CPackSystem gPackSystem;