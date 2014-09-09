#include   <stdio.h> 
#include   <string.h>   
#ifndef __TEXT_CONVERT_H__
#define __TEXT_CONVERT_H__

//////////////////////////////////////////////////////////////////////////
/// 汉字转拼音函数 适合gb2312和utf8
//////////////////////////////////////////////////////////////////////////
#define HZ2PY_OUTPUT_BUF_ARRAY_SIZE 5120	//一个字段的拼音最长长度
#define HZ2PY_MAX_PINYIN_SIZE 30			//一个字的拼音最长长度
#define HZ2PY_UTF8_CHECK_LENGTH 20			//检查是否为utf8编码时所检查的字符长度
class textConvert
{
public:
	static bool safeAddToOutbuf(char* outbuf,int iOutbuf,char* pinyinValue,int iPinyinValue);
	static int is_utf8_string(char *utf);
	static void pinyin_utf8(char* inbuf,char* outbuf, bool m_blnSimaple=false,bool polyphone_support=false,bool m_blnFirstBig=false,bool m_blnAllBiG=false,bool m_LetterEnd=true, bool m_unknowSkip=true,bool m_filterPunc=true);
	static int findLetter(int nCode, char* strValue);
	static void pinyin_gb2312(char * inbuf, char * outbuf, bool m_blnSimaple = false, bool polyphone_support = false, bool m_blnFirstBig = false, bool m_blnAllBiG = false, bool m_LetterEnd = false, bool m_unknowSkip = true, bool m_filterPunc = true); 

	static char* getPinYin(char* inbuf);
};


#endif // __TEXT_CONVERT_H__