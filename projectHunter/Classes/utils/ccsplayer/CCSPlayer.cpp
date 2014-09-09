#include "CCSPlayer.h"
#include "../../shaderProgram/shaderNode.h"
#include "../../CSystem/CSystem.h"

USING_NS_CC;
USING_NS_CC_EXT;

#define DEBUG_CCSPLAYER_RECT false

CCSPlayer::CCSPlayer()
	: _ccsNode(NULL)
	, m_pListener(NULL)
	, m_bFlipX(false)
	, m_bFlipY(false)
	, _opacity(255)
	, _showRect(false)
{
	for(int i = 0 ; i < ANIM_PLAYER_EFFECT_MAX; i++)
	{
		m_effectState[i] = 0;
		m_effectStateCache[i] = 0;
	}

	m_clipAnimType = CLIP_ANIM_NONE;

}

CCSPlayer::~CCSPlayer()
{

}

void CCSPlayer::reset()
{
	m_bFlipX = false;
	m_bFlipY = false;
	_opacity = 255;
	_showRect = false;
	setScale(1.0);
}

//深克隆
/*CCSPlayer::CCSPlayer(CCSPlayer *obj)
{
	//m_pListener = NULL;
	//_ccsNode = new cocos2d::extension::CCArmature(*obj->_ccsNode);
}*/

void CCSPlayer::addArmatureFileInfo(const char* pCCSFileName)
{
	// load resource directly
	CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo(pCCSFileName);
}

void CCSPlayer::addArmatureFileInfo(const char *imagePath, const char *plistPath, const char *configFilePath)
{
	// load resource directly
	CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo(imagePath, plistPath, configFilePath);
}

void CCSPlayer::purge()
{
	CCArmatureDataManager::sharedArmatureDataManager()->purge();
}

CCSPlayer* CCSPlayer::create(const char* pAniName)
{
	CCSPlayer* model = new CCSPlayer();
	if (model && model->initWithAniName(pAniName))
	{
		// LONG_TYPE time = CSystem::currentTimeMillis();
		// CCLOG("%lld", time);

		model->autorelease();
		return model;
	}
	CC_SAFE_DELETE(model);
	return NULL;
}

bool CCSPlayer::initWithAniName(const char* pAniName)
{
	if (CCSprite::init())
	{
		LONG_TYPE t1 = CSystem::currentTimeMillis();
		_ccsNode = CCArmature::create(pAniName);

		// CCLog("time 1 = %d %s", (int)(CSystem::currentTimeMillis() - t1),pAniName);
		if (_ccsNode)
		{
			_ccsNode->getAnimation()->playWithIndex(0);
// 			ccBlendFunc blendFunc = {GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA};
// 			_ccsNode->setBlendFunc(blendFunc);
			
			CCDictionary* dic = _ccsNode->getBoneDic();
			CCDictElement* pElement = NULL;
			CCDICT_FOREACH(dic, pElement)
			{
				string str = pElement->getStrKey();
				CCBone* bone = ((CCBone*)pElement->getObject());
				if (str.find("DAO_") != string::npos)
				{
					// 改变刀光混合方式
					ccBlendFunc blendFunc = {GL_ONE, GL_ONE};
					bone->setBlendFunc(blendFunc);

					// 改变刀光的shader
					bone->setShaderProgram(CCShaderCache::sharedShaderCache()->programForKey(kCCShader_PositionTextureColor));
				}
				else if (str.find("JIAN_") != string::npos)
				{
					// 改变剑雨混合方式
					ccBlendFunc blendFunc = { GL_DST_COLOR, GL_ONE };
					bone->setBlendFunc(blendFunc);

					// 改变剑雨的shader
					bone->setShaderProgram(CCShaderCache::sharedShaderCache()->programForKey(kCCShader_PositionTextureColor));
				}
			}
			addChild(_ccsNode);
			return true;
		}
		return false;
	}
	return false;
}

void CCSPlayer::play(const char *animationName, int durationTo, int durationTween, int loop, int tweenEasing)
{
	_CCSPlayerPlayPara.animationIndex = -1;
	_CCSPlayerPlayPara.animationName = animationName;
	_CCSPlayerPlayPara.durationTo = durationTo;
	_CCSPlayerPlayPara.durationTween = durationTween;
	_CCSPlayerPlayPara.loop = loop;
	_CCSPlayerPlayPara.tweenEasing = tweenEasing;

	_ccsNode->getAnimation()->play(animationName, durationTo, durationTween, loop, tweenEasing);
}

void CCSPlayer::playWithIndex(int animationIndex, int durationTo, int durationTween,  int loop, int tweenEasing)
{
	_CCSPlayerPlayPara.animationIndex = animationIndex;
	_CCSPlayerPlayPara.animationName = "";
	_CCSPlayerPlayPara.durationTo = durationTo;
	_CCSPlayerPlayPara.durationTween = durationTween;
	_CCSPlayerPlayPara.loop = loop;
	_CCSPlayerPlayPara.tweenEasing = tweenEasing;

	_ccsNode->getAnimation()->playWithIndex(animationIndex, durationTo, durationTween, loop, tweenEasing);
}

void CCSPlayer::replay()
{
	if (_CCSPlayerPlayPara.animationIndex != -1)
	{
		playWithIndex(_CCSPlayerPlayPara.animationIndex, _CCSPlayerPlayPara.durationTo, _CCSPlayerPlayPara.durationTween, _CCSPlayerPlayPara.loop, _CCSPlayerPlayPara.tweenEasing);
	}
	else
	{
		if(!_CCSPlayerPlayPara.animationName.empty())
		{
			play(_CCSPlayerPlayPara.animationName.c_str(), _CCSPlayerPlayPara.durationTo, _CCSPlayerPlayPara.durationTween, _CCSPlayerPlayPara.loop, _CCSPlayerPlayPara.tweenEasing);
		}
	}
}

void CCSPlayer::pause()
{
	_ccsNode->getAnimation()->pause();
}

void CCSPlayer::resume()
{
	_ccsNode->getAnimation()->resume();
}

void CCSPlayer::draw()
{
	if (DEBUG_CCSPLAYER_RECT || _showRect)
	{
		// 碰撞框
		CCRect rect = getRect(kRectTypeCollide, true);
		ccDrawColor4B(0, 255, 0, 255);
		ccDrawRect(rect.origin, ccp(rect.getMaxX(), rect.getMaxY()));
		
		// 攻击框
		rect = getRect(kRectTypeAttack, true);
		ccDrawColor4B(255, 0, 0, 255);
		ccDrawRect(rect.origin, ccp(rect.getMaxX(), rect.getMaxY()));

		// 全局框
		rect = getRect(kRectTypeBoundBox, true);
		ccDrawColor4B(0, 0, 255, 255);
		ccDrawRect(rect.origin, ccp(rect.getMaxX(), rect.getMaxY()));
	}
	updateFx();
}

cocos2d::CCRect CCSPlayer::getRect(RectType rectType, bool isSelf)
{
	CCRect rect; 

	if (rectType == kRectTypeBoundBox)
	{
		rect = _ccsNode->boundingBox();
	}
	else
	{
		int count = _ccsNode->getChildren()->count();
		CCBone* node = dynamic_cast<CCBone *>(_ccsNode->getChildren()->objectAtIndex(count - 1 - rectType));
		if (node)
		{
			rect = node->getDisplayManager()->getBoundingBox();
		}
	}

	if (!isSelf)
	{
		if (m_bFlipX)
		{
			float x = -rect.origin.x - rect.size.width;
			rect = CCRect(x, rect.origin.y, rect.size.width, rect.size.height);
		}

		if (m_bFlipY)
		{
			float y = -rect.origin.y - rect.size.height;
			rect = CCRect(rect.origin.x, y, rect.size.width, rect.size.height);
		}
	}

	return rect;
}

void CCSPlayer::animationEvent(cocos2d::extension::CCArmature *armature, MovementEventType movementType, const char *movementID)
{
	if (m_pListener && m_pfnSelector) {
			(m_pListener->*m_pfnSelector)(this, movementID, movementType);
	}
}

void CCSPlayer::setTarget(CCObject* target, SEL_CCS_CB selector)
{
	m_pListener = target;
	m_pfnSelector = selector;

	_ccsNode->getAnimation()->setMovementEventCallFunc(this, movementEvent_selector(CCSPlayer::animationEvent));
}

void CCSPlayer::setFlipX(bool flipX)
{
	if (m_bFlipX != flipX)
	{
		setScaleX(getScaleX() * -1);
		m_bFlipX = flipX;
	}
}

void CCSPlayer::setFlipY(bool flipY)
{
	if (m_bFlipY != flipY)
	{
		setScaleY(getScaleY() * -1);
		m_bFlipY = flipY;
	}
}

void CCSPlayer::setOpacity(GLubyte opacity)
{
	_opacity = opacity;
	_ccsNode->setOpacity(opacity);
}

float CCSPlayer::getCurrentFrame()
{
	return _ccsNode->getAnimation()->getCurrentFrameIndex();
}

void CCSPlayer::beforeDraw()
{
	if(m_clipAnimType != CLIP_ANIM_NONE)
	{
		//CC_NODE_DRAW_SETUP();
		glEnable(GL_SCISSOR_TEST);
		CCPoint cp = getParent()->convertToWorldSpace(getPosition());
		CCSize s = CCDirector::sharedDirector()->getWinSize();
		float curFrameHeight = getRect(kRectTypeBoundBox).size.height;
		if(m_clipAnimType == CLIP_ANIM_DOWN)
		{
			CCEGLView::sharedOpenGLView()->setScissorInPoints(cp.x - s.width / 2, cp.y + isOpenActorWaveClipY, s.width, s.height);
		}
		else if(m_clipAnimType == CLIP_ANIM_DAOYING)
		{
			float y = cp.y - curFrameHeight * WATER_WAVE_HIGH_SCALE;
			CCEGLView::sharedOpenGLView()->setScissorInPoints(cp.x - s.width / 2, y, s.width, curFrameHeight * WATER_WAVE_HIGH_SCALE - getPositionY());
		}
	}
}

void CCSPlayer::afterDraw()
{
	if(m_clipAnimType != CLIP_ANIM_NONE)
	{
		glDisable(GL_SCISSOR_TEST);
	}
}

// 换装
void CCSPlayer::changeAvatar(std::string& typeName)
{
	if (!typeName.empty())
	{
		string plistStr = "plist/" + typeName + ".plist";
		CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(plistStr.c_str());
	}

	CCDictionary* dic = _ccsNode->getBoneDic();
	CCDictElement* pElement = NULL;
	CCDICT_FOREACH(dic, pElement)
	{
		CCBone* bone = ((CCBone*)pElement->getObject());
		changeAvatar(typeName, bone);
	}
}

void CCSPlayer::changeAvatar(std::string& typeName, CCBone* bone)
{
	CCArray* list = bone->getDisplayManager()->getDecorativeDisplayList();
	CCObject* pObj = NULL;
	CCARRAY_FOREACH(list, pObj)
	{
		CCDecorativeDisplay* display = (CCDecorativeDisplay*)pObj;
		CCSkin* skin = (CCSkin*)(display->getDisplay());
		if (skin == NULL)
		{
			continue;
		}

		string str = skin->getDisplayName();
		if (str.find("wuqi_") != string::npos)
		{
			skin->retain();

			if (str.find("/") != string::npos)
			{
				str = str.substr(str.find("/") + 1);
			}

			if (!typeName.empty() && str.find(typeName) != string::npos)
			{
				// 重复
				return;
			}

			size_t start_pos = str.find("wuqi_");
			if (start_pos == 0)
			{
				if (typeName.empty())
				{
					return;
				}
				// 原始换新的
				display->getDisplayData()->displayName = typeName + "_" + str;
			}
			else
			{
				// 新的换回原始的
				display->getDisplayData()->displayName = str.substr(start_pos);
			}
			bone->addDisplay(display->getDisplayData(), display->getDisplayData()->displayIndex);
			skin->release();
		}
	}
}

//不能删，主要是ccs load机制的问题
string CCSPlayer::getCocostudioPath(const char *spriteName)
{
	char cocosSpriteName[255] = { 0 };
	sprintf(cocosSpriteName, "dragonBone/%s/%s.ExportJson", spriteName, spriteName);
	return string(cocosSpriteName);
}

#include "CCSPlayerShader.hpp"