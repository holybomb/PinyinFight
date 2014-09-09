//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include "platform/CCPlatformMacros.h"
#include <stdio.h>

namespace   cocos2d {

class CC_DLL CRijndael_Utils
{
public:
	static void MakeKeyAES(const char* password);	 // 设置密码

	//////////////////////////////////////////////////////////////////////////
	// I 普通加解密
	// 少于block不加，超出block的不加
	// 省内存
	static void EncryptAES(char* pBuffer, size_t size); // 加密
	static void DecryptAES(char* pBuffer, size_t size); // 解密

	//////////////////////////////////////////////////////////////////////////
	// II 完全加解密
	// 自动扩充到block的整数倍
	// 需要申请一个扩充的数组
	static char* FullEncryptAES(char* pBuffer, size_t size, size_t& outLen); // 加密
	static char* FullDecryptAES(char* pBuffer, size_t size, size_t& outLen); // 解密

private:
	static int padded_size(int size);
};

}//namespace   cocos2d 