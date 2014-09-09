#ifndef __UTILS_H__
#define __UTILS_H__

#include "cocos2d.h"
#include "cocos-ext.h"
#include <string>
#include "../Database/base/XTipMsg_const.h"
#include "support/zip_support/ZipUtils.h"
#include <zlib.h>
#define CHECK_ERR(err, msg) { \
	if (err != Z_OK) { \
	fprintf(stderr, "%s error: %d\n", msg, err); \
	return -1; \
	} \
}

#ifdef Z_SOLO

void *myalloc OF((void *, unsigned, unsigned));
void myfree OF((void *, void *));

void *myalloc(q, n, m)
	void *q;
unsigned n, m;
{
	q = Z_NULL;
	return calloc(n, m);
}

void myfree(void *q, void *p)
{
	q = Z_NULL;
	free(p);
}

static alloc_func zalloc = myalloc;
static free_func zfree = myfree;

#else /* !Z_SOLO */

static alloc_func zalloc = (alloc_func)0;
static free_func zfree = (free_func)0;
#endif /* Z_SOLO */

using namespace std;

enum GuildTimeType
{
	GuildTimeType_Online,	// 在线
	GuildTimeType_Welfare,	// 福利
};

struct ShadowTextFontDefinition
{
public:
	ShadowTextFontDefinition()
		: fontSize(22)
		, fontColor(ccc3(243,196,58))
		, fontName("kaiti")
		, strokeColor(ccBLACK)
		, strokeSize(2)
	{
	}

	std::string str;

	int fontSize;
	ccColor3B fontColor;
	std::string fontName;

	ccColor3B strokeColor;
	int strokeSize;

	CCPoint offset;
};
class XCCMenu;
class Utils
{
public:
	//-----------------------------------------------------------------------------
	static void copyCCScale9Sprite(cocos2d::extension::CCScale9Sprite* pDst, cocos2d::extension::CCScale9Sprite* pSrc);
	static cocos2d::CCLabelTTF* copyCCLabelTTF(cocos2d::CCLabelTTF* pNode);
	static cocos2d::CCMenuItemImage* copyCCMenuItemImage(cocos2d::CCMenuItemImage* pNode);
	static void copyCCNode(cocos2d::CCNode* pDst, cocos2d::CCNode* pSrc);
	static void copyCCSprite(cocos2d::CCSprite* pDst, cocos2d::CCSprite* pSrc);
	static void copyCCControl(cocos2d::extension::CCControl* pDst, cocos2d::extension::CCControl* pSrc);
	static void copyCCControlButton(cocos2d::extension::CCControlButton* pDst, cocos2d::extension::CCControlButton* pSrc);
	// 深拷贝
	static cocos2d::CCNode* copyWholeCCNode(cocos2d::CCNode* pNode);
	static cocos2d::CCNode* copySubCCNode(cocos2d::CCNode* pNode);


	// 格式化数值显示
	static string formatValue(int value);


	//-----------------------------------------------------------------------------
	static void FileCopy(std::string psrcFileName, std::string pdesFileName) ;
	static bool isFileExist(std::string pFileName) ;

	//-----------------------------------------------------------------------------
	static bool replace(std::string& str, const std::string& from, const std::string& to);
	static void replaceAll(std::string& str, const std::string& from, const std::string& to);
	static string trim(string& str);
	static vector<string> split(const string& str, string sep = ",");

	static bool ccc3BEqual(cocos2d::ccColor3B a, cocos2d::ccColor3B b);
	//-----------------------------------------------------------------------------
	static void openTip(int ID);
	//-----------------------------------------------------------------------------
	static string itos(int value);
	static string ftos(float value);

	//-----------------------------------------------------------------------------
	// 置灰CCControlButton
	static void setEnableCCControlButton(cocos2d::extension::CCControlButton* btn, bool enable);
	// 置灰CCMenuItemImage
	static void setEnableCCMenuItemImage(cocos2d::CCMenuItemImage* btn, bool enable);

	//复制一个灰色的sprite
	static CCSprite* copyGraySprite(CCSprite* spr ,CCTexture2D *pTexture);

	static std::string getPinyinLetter(std::string pWholePinyin);
	static std::string getPinyinStr(string pWord);
	static bool checkTextNumberOrLetter(std::string str);
	static CCSprite* getShadowText(CCNode* parent,const ShadowTextFontDefinition& fontDef);
};

#endif /* __UTILS_H__ */
