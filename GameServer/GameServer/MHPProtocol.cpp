#include "stdafx.h"
#include "MHPProtocol.h"
#include "Protect.h"

void MHPServerProtocolCore(BYTE head,BYTE* lpMsg,int size) // OK
{
	PROTECT_START

	switch(head)
	{
		case 0x00:
			
			break;
		default:
			break;
	}

	PROTECT_FINAL
}
