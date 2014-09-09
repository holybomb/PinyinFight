//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include "platform/CCPlatformMacros.h"
#include <stdio.h>

namespace   cocos2d {

class CC_DLL CRijndael_Utils
{
public:
	static void MakeKeyAES(const char* password);	 // ��������

	//////////////////////////////////////////////////////////////////////////
	// I ��ͨ�ӽ���
	// ����block���ӣ�����block�Ĳ���
	// ʡ�ڴ�
	static void EncryptAES(char* pBuffer, size_t size); // ����
	static void DecryptAES(char* pBuffer, size_t size); // ����

	//////////////////////////////////////////////////////////////////////////
	// II ��ȫ�ӽ���
	// �Զ����䵽block��������
	// ��Ҫ����һ�����������
	static char* FullEncryptAES(char* pBuffer, size_t size, size_t& outLen); // ����
	static char* FullDecryptAES(char* pBuffer, size_t size, size_t& outLen); // ����

private:
	static int padded_size(int size);
};

}//namespace   cocos2d 