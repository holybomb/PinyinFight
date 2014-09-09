///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*
#include <Windows.h>
#include <stdio.h>
#include <strsafe.h>
#include "CRijndael.h"

#define DEFAULT_KEY_SIZE 16
#define DEFAULT_SIZE_128 128
#define DEFAULT_SIZE_192 192
#define DEFAULT_SIZE_256 256

BOOL main(int argc, PTCHAR argv[])
{
	INT iKeyLength = 0;
	CHAR szPlaintText[DEFAULT_SIZE_192] = { 0 };
	CHAR szCipHerText[DEFAULT_SIZE_192] = { 0 };
	CHAR szKey[DEFAULT_KEY_SIZE] = { "0123456789abcde" };
	CRijndael AES;
	AES.MakeKey(szKey, CRijndael::sm_chain0);
	iKeyLength = AES.GetKeyLength();
	if (DEFAULT_KEY_SIZE != iKeyLength)
	{
		printf("Get default key length is error./n");
		return FALSE;
	}

	StringCchCopyA(szPlaintText,
		DEFAULT_SIZE_192,
		"This is a plainttext! "
		"I will call encryption methods."
		"Make it become ciphertext.");
	AES.Encrypt(szPlaintText, szCipHerText, DEFAULT_SIZE_192);
	printf("CipherText:%s/n", szCipHerText);
	AES.Decrypt(szCipHerText, szPlaintText, DEFAULT_SIZE_192);
	printf("PlaintText:%s/n", szPlaintText);
	system("pause");
	return TRUE;
}
*/