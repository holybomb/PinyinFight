#include "Utils.h"
#include "TextManager.h"
#include "../shaderProgram/shaderNode.h"
#include "textConvert.h"
#include "../scenes/gameBaseScene/GameBaseScene.h"

USING_NS_CC;
USING_NS_CC_EXT;
//
//-----------------------------------------------------------------------------
CCLabelTTF* Utils::copyCCLabelTTF(CCLabelTTF* pNode)
{
	if (pNode == NULL) return NULL;

	CCLabelTTF* pTemp = CCLabelTTF::create();
	if (pTemp)
	{
		// CCNode
		copyCCNode(pTemp, pNode);

		// CCLabelTTF
		pTemp->setFontName(pNode->getFontName());
		pTemp->setFontSize(pNode->getFontSize());
		pTemp->setOpacity(pNode->getOpacity());
		pTemp->setColor(pNode->getColor());
		pTemp->setDimensions(pNode->getDimensions());
		pTemp->setHorizontalAlignment(pNode->getHorizontalAlignment());
		pTemp->setVerticalAlignment(pNode->getVerticalAlignment());
		pTemp->setBlendFunc(pNode->getBlendFunc());
		pTemp->setString(pNode->getString());
	}
	return pTemp;
}

CCMenuItemImage* Utils::copyCCMenuItemImage(CCMenuItemImage* pNode)
{
	if (pNode == NULL) return NULL;

	CCMenuItemImage* pTemp = CCMenuItemImage::create();
	if (pTemp)
	{
		// CCNode
		copyCCNode(pTemp, pNode);

		// CCMenuItemImage
		CCNode* temp = NULL;
		CCSprite* pSprite = NULL;
		temp = pNode->getNormalImage();
		if (temp)
		{
			pSprite = (CCSprite*)temp;
			CCSprite* pTempSprite = CCSprite::create();
			copyCCSprite(pTempSprite, pSprite);
			pTemp->setNormalImage(pTempSprite);
		}

		temp = pNode->getSelectedImage();
		if (temp)
		{
			pSprite = (CCSprite*)temp;
			CCSprite* pTempSprite = CCSprite::create();
			copyCCSprite(pTempSprite, pSprite);
			pTemp->setSelectedImage(pTempSprite);
		}

		temp = pNode->getDisabledImage();
		if (temp)
		{
			pSprite = (CCSprite*)temp;
			CCSprite* pTempSprite = CCSprite::create();
			copyCCSprite(pTempSprite, pSprite);
			pTemp->setDisabledImage(pTempSprite);
		}
	}
	return pTemp;
}

void Utils::copyCCNode(cocos2d::CCNode* pDst, cocos2d::CCNode* pSrc)
{
	if (pDst == NULL || pSrc == NULL) return;

	pDst->setPosition(pSrc->getPosition());
	pDst->setContentSize(pSrc->getContentSize());
	pDst->setAnchorPoint(pSrc->getAnchorPoint());
	pDst->setScaleX(pSrc->getScaleX());
	pDst->setScaleY(pSrc->getScaleY());
	pDst->setRotation(pSrc->getRotation());
	pDst->setSkewX(pSrc->getSkewX());
	pDst->setSkewY(pSrc->getSkewY());
	pDst->setTag(pSrc->getTag());
	pDst->ignoreAnchorPointForPosition(pSrc->isIgnoreAnchorPointForPosition());
}

void Utils::copyCCSprite(cocos2d::CCSprite* pDst, cocos2d::CCSprite* pSrc)
{
	if (pDst == NULL || pSrc == NULL) return;
	copyCCNode(pDst, pSrc);

	pDst->setDisplayFrame(pSrc->displayFrame());

	pDst->setOpacity(pSrc->getOpacity());
	pDst->setColor(pSrc->getColor());
	pDst->setFlipX(pSrc->isFlipX());
	pDst->setFlipY(pSrc->isFlipY());
	pDst->setBlendFunc(pSrc->getBlendFunc());
}

void Utils::copyCCScale9Sprite(cocos2d::extension::CCScale9Sprite* pDst, cocos2d::extension::CCScale9Sprite* pSrc)
{
	if (pDst == NULL || pSrc == NULL) return;
		copyCCNode(pDst, pSrc);
	CCRect rect = pSrc->boundingBox();
	CCRect capInsets = pSrc->getCapInsets();
	//const char* file = pSrc->getSpriteFileName().c_str();
	CCSpriteFrame* srcFrame = pSrc->getSpriteFrame();
	if(!srcFrame)
		return;
	CCSpriteFrame* spriteFrame = dynamic_cast<CCSpriteFrame*>(srcFrame->copyWithZone(NULL));
	pDst->initWithSpriteFrame(spriteFrame,capInsets);
	//pDst->setDisplayFrame(pSrc->displayFrame());

	pDst->setOpacity(pSrc->getOpacity());
	pDst->setColor(pSrc->getColor());
}

void Utils::copyCCControl(CCControl* pDst, CCControl* pSrc)
{
	if (pDst == NULL || pSrc == NULL) return;
	copyCCNode(pDst, pSrc);
	// XXX 没有用CCControl属性
}

void Utils::copyCCControlButton(CCControlButton* pDst, CCControlButton* pSrc)
{
	if (pDst == NULL || pSrc == NULL) return;
	copyCCControl(pDst, pSrc);
	pDst->setZoomOnTouchDown(false); // pDst->setZoomOnTouchDown(pSrc->getZoomOnTouchDown());
	pDst->setTitleForState(pSrc->getTitleForState(CCControlStateNormal), CCControlStateNormal);
	pDst->setTitleForState(pSrc->getTitleForState(CCControlStateHighlighted), CCControlStateHighlighted);
	pDst->setTitleForState(pSrc->getTitleForState(CCControlStateDisabled), CCControlStateDisabled);
	pDst->setTitleTTFForState(pSrc->getTitleTTFForState(CCControlStateNormal), CCControlStateNormal);
	pDst->setTitleTTFForState(pSrc->getTitleTTFForState(CCControlStateHighlighted), CCControlStateHighlighted);
	pDst->setTitleTTFForState(pSrc->getTitleTTFForState(CCControlStateDisabled), CCControlStateDisabled);
	pDst->setTitleTTFSizeForState(pSrc->getTitleTTFSizeForState(CCControlStateNormal), CCControlStateNormal);
	pDst->setTitleTTFSizeForState(pSrc->getTitleTTFSizeForState(CCControlStateHighlighted), CCControlStateHighlighted);
	pDst->setTitleTTFSizeForState(pSrc->getTitleTTFSizeForState(CCControlStateDisabled), CCControlStateDisabled);
	pDst->setLabelAnchorPoint(pSrc->getLabelAnchorPoint());
	pDst->setPreferredSize(pSrc->getPreferredSize());

	// XXX 浅copy
	CCScale9Sprite* _Normal = pSrc->getBackgroundSpriteForState(CCControlStateNormal);
	CCScale9Sprite* _Highlighted = pSrc->getBackgroundSpriteForState(CCControlStateHighlighted);
	CCScale9Sprite* _Disabled = pSrc->getBackgroundSpriteForState(CCControlStateDisabled);

	CCScale9Sprite* pNormal = CCScale9Sprite::create();
	CCScale9Sprite* pHighlighted = CCScale9Sprite::create();
	CCScale9Sprite* pDisabled = CCScale9Sprite::create();

	copyCCScale9Sprite(pNormal,_Normal);
	copyCCScale9Sprite(pHighlighted,_Highlighted);
	copyCCScale9Sprite(pDisabled,_Disabled);
	/*pNormal->retain();
	pHighlighted->retain();
	pDisabled->retain();
	pNormal->removeFromParent();
	pHighlighted->removeFromParent();
	pDisabled->removeFromParent();
	pSrc->getBackgroundSpriteDispatchTable()->removeAllObjects();*/

	pDst->setBackgroundSpriteForState(pNormal, CCControlStateNormal);
	pDst->setBackgroundSpriteForState(pHighlighted, CCControlStateHighlighted);
	pDst->setBackgroundSpriteForState(pDisabled, CCControlStateDisabled);
	//pNormal->release();
	//pHighlighted->release();
	//pDisabled->release();

	pDst->setTitleColorForState(pSrc->getTitleColorForState(CCControlStateNormal), CCControlStateNormal);
	pDst->setTitleColorForState(pSrc->getTitleColorForState(CCControlStateHighlighted), CCControlStateHighlighted);
	pDst->setTitleColorForState(pSrc->getTitleColorForState(CCControlStateDisabled), CCControlStateDisabled);

	pDst->setTouchPriority(pSrc->getTouchPriority());
}

// 深拷贝
cocos2d::CCNode* Utils::copyWholeCCNode(cocos2d::CCNode* pNode)
{
	CCNode* node = CCNode::create();
	copyCCNode(node, pNode);

	CCArray* child = pNode->getChildren();
	CCObject* obj;
	CCARRAY_FOREACH(child, obj)
	{
		node->addChild(copySubCCNode((CCNode*)obj));
	}

	return node;
}

/**
 * 需要注意顺序
 * 父节点只能是普通的Node
 */
CCNode* Utils::copySubCCNode(CCNode* pNode)
{
	CCLabelTTF* label = dynamic_cast<CCLabelTTF*>(pNode);
	if (label)
	{
		return copyCCLabelTTF(label);
	}

	CCSprite* spr = dynamic_cast<CCSprite*>(pNode);
	if (spr)
	{
		CCSprite* tmp = CCSprite::create();
		copyCCSprite(tmp, spr);
		return tmp;
	}

	CCMenuItemImage* item = dynamic_cast<CCMenuItemImage*>(pNode);
	if (item)
	{
		return copyCCMenuItemImage(item);
	}

	CCControlButton* btn = dynamic_cast<CCControlButton*>(pNode);
	if (btn)
	{
		CCControlButton* tmp = CCControlButton::create();
		copyCCControlButton(tmp, btn);
		return tmp;
	}

	return copyWholeCCNode(pNode);
}

// 格式化数值显示
string Utils::formatValue(int value)
{
	char str[20] = { 0 };
	// 超过100000变10万 超过 100000万变10亿
	int temp = value / 10000;
	if (temp == 0)
	{
		sprintf(str, "%d", value);
	}
	else
	{
		value = temp;
		temp = value / 10000;
		if (temp == 0)
		{
			sprintf(str, "%d%s", value, GET_STRING(TEXT_TEN_THOUSAND).c_str());
		}
		else
		{
			sprintf(str, "%d%s", temp, GET_STRING(TEXT_HUNDRED_MILLION).c_str());
		}
	}
	return string(str);
}

//-----------------------------------------------------------------------------
bool Utils::isFileExist(std::string pFileName)
{
	FILE *fp = fopen(pFileName.c_str(),"r");
	if(fp)
	{
		fclose(fp);
		return true;
	}
	return false;
}

void Utils::FileCopy(std::string psrcFileName, std::string pdesFileName)
{
	unsigned long len = 0;
	unsigned char *data = NULL;

	data = CCFileUtils::sharedFileUtils()->getFileData(psrcFileName.c_str(),"rb",&len);
	if(data==NULL)
		return;
	FILE *fp = fopen(pdesFileName.c_str(),"wb");
	fwrite( data, sizeof(char), len, fp);
	fclose(fp);
	delete []data;
	data = NULL;
}

bool Utils::replace(std::string& str, const std::string& from, const std::string& to)
{
	size_t start_pos = str.find(from);
	if(start_pos == std::string::npos)
		return false;
	str.replace(start_pos, from.length(), to);
	return true;
}

void Utils::replaceAll(std::string& str, const std::string& from, const std::string& to)
{
	if(from.empty())
		return;
	size_t start_pos = 0;
	while((start_pos = str.find(from, start_pos)) != std::string::npos) {
		str.replace(start_pos, from.length(), to);
		start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
	}
}

string Utils::trim(string& str)
{    
	string::size_type pos = str.find_first_not_of(' ');   
	if (pos == string::npos)   
	{       
		return str;    
	}    
	string::size_type pos2 = str.find_last_not_of(' ');    
	if (pos2 != string::npos)    
	{       
		return str.substr(pos, pos2 - pos + 1);    
	}    
	return str.substr(pos);
}

vector<string> Utils::split(const string& str, string sep)
{    
	vector<string> ret_;
	if (str.empty())   
	{        
		return ret_;    
	}
	string tmp;    
	string::size_type pos_begin = str.find_first_not_of(sep);   
	string::size_type comma_pos = 0;   
	while (pos_begin != string::npos)   
	{        
		comma_pos = str.find(sep, pos_begin);      
		if (comma_pos != string::npos)       
		{            
			tmp = str.substr(pos_begin, comma_pos - pos_begin);       
			pos_begin = comma_pos + sep.length();      
		}    
		else     
		{           
			tmp = str.substr(pos_begin);      
			pos_begin = comma_pos;       
		}       
		if (!tmp.empty())    
		{          
			ret_.push_back(tmp);    
			tmp.clear();        
		}    
	}   
	return ret_;
}

bool Utils::ccc3BEqual(ccColor3B a, ccColor3B b)
{
	return a.r == b.r && a.g == b.g && a.b == b.b;
}

//-----------------------------------------------------------------------------
void Utils::openTip(int ID)
{
	GameBaseScene* scene = dynamic_cast<GameBaseScene*>(CCDirector::sharedDirector()->getRunningScene());
	if (scene)
	{
		scene->openTipGame(DBManager::sharedDBManager()->getTipMsg(ID)->TipMsg);
	}
}
string Utils::itos(int value)
{
	char str[10] = { 0 };
	sprintf(str, "%d", value);
	return string(str);
}

string Utils::ftos(float value)
{
	char str[10] = { 0 };
	sprintf(str, "%f", value);
	return string(str);
}

//--------------------存档相关-----------------------
void Utils::setEnableCCControlButton(CCControlButton* btn, bool enable)
{
	btn->setEnabled(enable);
	if (enable)
	{
		btn->getBackgroundSpriteForState(CCControlStateNormal)->getBatchNode()->setShaderProgram(CCShaderCache::sharedShaderCache()->programForKey(kCCShader_PositionTextureColor));
	}
	else
	{
		CCGLProgram* program = shaderNode::getGrayShader();
		btn->getBackgroundSpriteForState(CCControlStateDisabled)->getBatchNode()->setShaderProgram(program);
		//program->release();
	}
}

CCSprite* Utils::copyGraySprite(CCSprite* spr ,CCTexture2D *pTexture)
{
	CCSprite *copySpr = CCSprite::create("image/SkillIcon/tb_jineng_zixia1.png");
	copySpr->setPosition(spr->getPosition());
	copySpr->setZOrder(spr->getZOrder());
	
	if (copySpr && pTexture)
	{
		copySpr->setTexture(pTexture);	
	}
	spr->getParent()->addChild(copySpr);

	CCGLProgram* program = shaderNode::getGrayShader();
	copySpr->setShaderProgram(program);
	//program->release();
	

	return copySpr;
}


void Utils::setEnableCCMenuItemImage(cocos2d::CCMenuItemImage* btn, bool enable)
{
	btn->setEnabled(enable);
	if (enable)
	{
		CCSprite* spr = dynamic_cast<CCSprite*>(btn->getNormalImage());
		if(spr)
			spr->setShaderProgram(CCShaderCache::sharedShaderCache()->programForKey(kCCShader_PositionTextureColor));
	}
	else
	{
		CCSprite* spr = dynamic_cast<CCSprite*>(btn->getDisabledImage());
		if (spr)
		{
			CCGLProgram* program = shaderNode::getGrayShader();
			spr->setShaderProgram(program);
			//program->release();
		}
			
	}
}

CCSprite* Utils::getShadowText(CCNode* parent,const ShadowTextFontDefinition& fontDef)
{
	CCLabelTTF *label = CCLabelTTF::create(fontDef.str.c_str(), fontDef.fontName.c_str(), fontDef.fontSize);
	CCSize textureSize = label->getContentSize();
	textureSize.width += 2 * fontDef.strokeSize;
	textureSize.height += 2 * fontDef.strokeSize;
	//call to clear error
	glGetError();
	CCRenderTexture *rt = CCRenderTexture::create(textureSize.width, textureSize.height);
	if(!rt)
	{
		//ERROR_MSG("create render texture failed !!!!");
		parent->addChild(label);
		return label;
	}

	label->setColor(fontDef.strokeColor);

	ccBlendFunc originalBlend = label->getBlendFunc();
	ccBlendFunc func = {GL_SRC_ALPHA, GL_ONE};
	label->setBlendFunc(func);
	label->setAnchorPoint(ccp(0.5, 0.5));

	rt->begin();
	for(int i = 0; i < 360; i += 15)
	{
		float r = CC_DEGREES_TO_RADIANS(i);
		label->setPosition(ccp(
			textureSize.width * 0.5f + sin(r) * fontDef.strokeSize,
			textureSize.height * 0.5f + cos(r) * fontDef.strokeSize));
		label->visit();
	}
	label->setColor(fontDef.fontColor);
	label->setBlendFunc(originalBlend);
	label->setPosition(ccp(textureSize.width * 0.5f, textureSize.height * 0.5f));
	label->visit();
	rt->end();

	CCTexture2D *texture = rt->getSprite()->getTexture();
	texture->setAntiAliasTexParameters();
	CCSprite*	textStr = CCSprite::createWithTexture(rt->getSprite()->getTexture());
	parent->setContentSize(textStr->getContentSize());
	textStr->setAnchorPoint(ccp(0.5, 0.5));
	textStr->setPosition(fontDef.offset);
	textStr->setFlipY(true);
	parent->addChild(textStr,parent->getChildrenCount(), parent->getChildrenCount());
	return textStr;
}

std::string Utils::getPinyinStr(string pWord)
{
	string pp = pWord;
	char p[1000];
	int i;
	for( i=0;i<pp.length();i++)
		p[i] = pp[i];
	p[i] = '\0';
	printf("%s\n",p);
	cout<<p;
	std::string pinyinStr = textConvert::getPinYin(p);//GET_STRING(TEXT_JUQING_SPLASH_BEGIN2);
	return pinyinStr;
}
std::string Utils::getPinyinLetter(std::string pWholePinyin)
{
	vector<std::string> arr = split(pWholePinyin,"'");
	string result = "";
	for(int i =0;i<arr.size();i++)
	{
		string str = arr.at(i);
		char first = str.at(0);
		int num = 1;
		if(first == 'Z' || first == 'S' || first == 'C' || first == 'z' || first == 's' || first == 'c' )
		{
			char sec = str.at(1);
			if(sec == 'h')
			{
				num = 2;
			}
		}
		result +=str.substr(0,num)+",";
	}
	result = result.substr(0,result.size()-1);
	return result;
}

// 检验是否包含特殊字符
bool Utils::checkTextNumberOrLetter(std::string str)
{
	int badNum = 0;

	int size = str.length();
	if (size <= 0)
		return false;

	char* pStr = new char[size];

	strcpy(pStr, str.c_str());
	for (int i = 0; i < size; i++)
	{
		if ((pStr[i]>=48 && pStr[i]<=57)
			|| (pStr[i]>=65 && pStr[i]<=90)
			|| (pStr[i]>=97 && pStr[i]<=122)
			|| (pStr[i]==95)
		)
			continue;
		else
		/*if (ispunct(pStr[i]))*/
		{
			badNum ++;
		}
	}
	//delete[] pStr;
	bool res = true;
	if (badNum > 0)
	{
		res = false;
	}
	return res;
}

