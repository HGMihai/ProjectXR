#pragma once

void DecryptData(BYTE* lpMsg,int size, int aIndex);
void EncryptData(BYTE* lpMsg,int size);
void MHPDecryptData(BYTE* lpMsg,int size);
void MHPEncryptData(BYTE* lpMsg,int size);
void InitHackCheck();
