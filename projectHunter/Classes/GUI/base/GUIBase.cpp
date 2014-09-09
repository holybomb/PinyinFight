#include "GUIBase.h"
#include "util/XCCBReader.h"
//#include "../../soundManager/soundManager.h"
#include "../../scenes/gameBaseScene/GameBaseScene.h"
#include "../../Database/base/DB_Text_const.h"
#include "../../utils/TextManager.h"

USING_NS_CC;
USING_NS_CC_EXT;

#define  USING_CACHE false
enum 
{
	kTagBlack = 1000,
	kTagLoading,
};

GUIBase::GUIBase(void)
: m_bFocus(false)
, m_bEnabled(true)
, m_bLoop(false)
, m_bAutoClose(false)
, _GUIScreenType(kGUIScreenFull)
, m_pListener(NULL)
, m_pfnSelector(NULL)
, _Counter(0)
, _noBlack(false)
, _noCreateBlack(false)
, _hasProcess(false)
, _isOpenExitDialog(false)
{
	setVisible(false);
}

GUIBase::~GUIBase(void)
{
}

void GUIBase::onEnter()
{
	CCLayer::onEnter();

	setTouchPriority(kGUIBasePriority);
	setTouchMode(kCCTouchesOneByOne);
	// 设置close回调
	if (m_pCcbNode)
	{
		CCNode* pNode = m_pCcbNode->getChildByTag(TAG_CLOSE);
		if (pNode)
		{
			CCNode* pTemp = pNode->getChildByTag(TAG_CLOSE);
			if (pTemp)
			{
				CCMenuItemImage *pItem = (CCMenuItemImage *)pTemp;
				pItem->setTarget(this, menu_selector(GUIBase::closeButtonCallback));
			}
		}
	}
}

void GUIBase::cleanup(void)
{
    CCLayer::cleanup();
}

void GUIBase::setTouchPriority(int priority)
{
	CCLayer::setTouchPriority(priority);

	// RESET ALL CCMenu priority
	resetPriority(m_pCcbNode ? m_pCcbNode : this, priority);
}

void GUIBase::resetPriority(CCNode* pNode, int priority)
{
	CCArray* pChildren = pNode->getChildren();

	CCObject* pObj = NULL;
	CCARRAY_FOREACH(pChildren, pObj)
	{
		CCNode* pChild = (CCNode*)pObj;
		if (pChild->getChildrenCount() > 0)
		{
			resetPriority(pChild, priority);
		}

		// CCMenu
		CCMenu* pMenu = dynamic_cast<CCMenu*>(pChild);
		if (pMenu)
		{
			pMenu->setTouchPriority(priority  - 1);
			continue;
		}

		// CCScrollView
		CCScrollView* pCCScrollView = dynamic_cast<CCScrollView*>(pChild);
		if (pCCScrollView)
		{
			pCCScrollView->setTouchPriority(priority  - 1);
			continue;
		}

		// CCControlButton
		CCControlButton* pCCControlButton = dynamic_cast<CCControlButton*>(pChild);
		if (pCCControlButton)
		{
			pCCControlButton->setTouchPriority(priority  - 1);
			continue;
		}
	}
}

/**
 * load ccb node
 */
void GUIBase::loadCCB(CCNode* pParent, const char* pCCBFileName)
{

	CcbBase::loadCCB(pCCBFileName);

    // remove base node
	CCNode* node = NULL;

	node = m_pCcbNode->getChildByTag(TAG_BASE_NODE);
	if (node) 
	{
		m_pCcbNode = node;
	}

	node = m_pCcbNode->getChildByTag(TAG_BASE_NODE);
	if (node) 
	{
		m_pCcbNode = node;
	}
    CCAssert(m_pCcbNode, "GUIBase::loadCCB II");
}

bool GUIBase::isTouchInside(CCTouch* touch)
{
	CCPoint touchLocation = touch->getLocation(); // Get the touch position
	CCNode* pNode = (m_pCcbNode ? m_pCcbNode : this)->getChildByTag(TAG_BG);
	if (pNode)
	{
		touchLocation = pNode->getParent()->convertToNodeSpace(touchLocation);
		CCRect bBox = pNode->boundingBox();
		return bBox.containsPoint(touchLocation);
	}
	return false;
}


bool GUIBase::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
	if (! m_bVisible || !m_bEnabled)
	{
		return false;
	}

	if (getBlack() || isTouchInside(pTouch))
	{
		return true;
	}
	else
	{
		if (m_bAutoClose)
		{
			doClose();
			return true;
		}
		else
		{
			return m_bFocus;
		}
	}
}

void GUIBase::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
}

void GUIBase::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
}

void GUIBase::closeButtonCallback(CCObject* pSender)
{
//	BUTTON_SOUND;
//	CCLOG("GUIBase::CloseButtonCallback");
	doClose();
}

void GUIBase::doOpen()
{
	doOpen(NULL, -1);
}

void GUIBase::doOpen(BaseDownCMD* pCmd)
{
	doOpen(pCmd, -1);
}

void GUIBase::doOpen(BaseDownCMD* pCmd, int index)
{
	if (m_bVisible) return;

	onFocus(_noBlack);

	if (m_pParent)
	{
		GUIBase* pParent = dynamic_cast<GUIBase*>(m_pParent);
		if (pParent && pParent->isVisible())
		{
			pParent->offFocus(_noCreateBlack);
		}
	}
	

	setKeypadEnabled(true);
	setVisible(true);
	setTouchEnabled(true);
	processCMD(pCmd, index);
}

void GUIBase::doClose()
{
	if (!m_bVisible) return;
	setVisible(false);

	if (m_pListener && m_pfnSelector) {
		(m_pListener->*m_pfnSelector)(this);
	}

	offFocus(_noBlack);
	GUIBase* pParent = dynamic_cast<GUIBase*>(m_pParent);
	if (pParent && pParent->isVisible())
	{
		pParent->onFocus(_noCreateBlack);
	}

	setTouchEnabled(false);
	doClean();
}

void GUIBase::doClean()
{
	if (!USING_CACHE)
	{
		// 清空自己
		removeFromParent();
	}
}

void GUIBase::processCMD(BaseDownCMD* pCmd, int index)
{
}

void GUIBase::updateCMD(BaseDownCMD* pCmd, int index)
{
}

// 获得焦点
void GUIBase::onFocus(bool noCreateBlack)
{
	if (noCreateBlack)
	{
		return;
	}

	_Counter++;
	if (_Counter == 1)
	{
		setBlack(false);
	}
}

// 失去焦点
void GUIBase::offFocus(bool noCreateBlack)
{
	if (noCreateBlack)
	{
		return;
	}

	_Counter--;
	if (_Counter < 1)
	{
		setBlack(true);
	}
}

void GUIBase::setTarget(CCObject* target, cocos2d::SEL_MenuHandler selector)
{
	m_pListener = target;
	m_pfnSelector = selector;
}

void GUIBase::setNoBlack(bool noBlack)
{ 
	_noBlack = noBlack;
	setBlack(!noBlack);
}

void GUIBase::setBlack(bool isBlack)
{
	CCNode* pModel = getChildByTag(kTagBlack);
	if (isBlack)
	{
		if (pModel == NULL)
		{
			CCLayerColor* pBlack = CCLayerColor::create(ccc4(0, 0, 0, 0));
			addChild(pBlack,getChildrenCount(), kTagBlack);
		}
	}
	else
	{
		if (pModel)
		{
			removeChildByTag(kTagBlack);
		}
	}

	if (m_pCcbNode)
	{
		resetTouch(m_pCcbNode, !isBlack);
	}
}

bool GUIBase::getBlack()
{
	return getChildByTag(kTagBlack) != NULL;
}

void GUIBase::resetTouch(CCNode* pNode, bool enabled)
{
	CCArray* pChildren = pNode->getChildren();

	CCObject* pObj = NULL;
	CCARRAY_FOREACH(pChildren, pObj)
	{
		CCNode* pChild = (CCNode*)pObj;
		if (pChild->getChildrenCount() > 0)
		{
			resetTouch(pChild, enabled);
		}

		// CCMenu
		CCLayer* pModul = dynamic_cast<CCLayer*>(pChild);
		if (pModul)
		{
			pModul->setTouchEnabled(enabled);
		}
	}
}

void GUIBase::setLoading(bool loading)
{
	GameBaseScene* parent = dynamic_cast<GameBaseScene*>(m_pParent);
	if (parent)
	{
		/*if (loading)
		{
		parent->openLoading();
		}
		else
		{
		parent->closeLoading();
		}*/
	}
	else
	{
		CCLOG("XXX setLoading is wrong~~~");
	}
// 	GUIBase* pModel = (GUIBase*)getChildByTag(kTagLoading);
// 	if (loading)
// 	{
// 		if (pModel == NULL)
// 		{
// 			GUIBase* pLoading = GUIBase::create();
// 			pLoading->loadCCB(this, "scene/loading_UI1.ccbi");
// 			addChild(pLoading, kOrderGUIHigh, kTagLoading);
// 			pLoading->setNoBlack();
// 			pLoading->doOpen();
// 		}
// 	}
// 	else
// 	{
// 		if (pModel)
// 		{
// 			pModel->doClose();
// 		}
// 	}
// 
// 	if (m_pCcbNode)
// 	{
// 		resetTouch(m_pCcbNode, !loading);
// 	}
}

void GUIBase::keyBackClicked()
{
	CCDirector::sharedDirector()->end();
	/*GameBaseScene* scene = dynamic_cast<GameBaseScene*>(CCDirector::sharedDirector()->getRunningScene());
	if(scene)
		scene->openExitDialog();*/
}

void GUIBase::exitCallback(CCObject* pSender)
{
	
}
