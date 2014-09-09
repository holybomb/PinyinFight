#include "TipGame.h"
#include "../base/GUIConfig.h"

USING_NS_CC;

static const int TIP_NUM = 3;
static const int OFFSET_Y = 30;

static const float TIME_1 = 0.3f;
static const float TIME_2 = 1.5f + TIME_1;
static const float TIME_3 = 0.5f + TIME_2;

enum
{
	ALPHA_1 = 255,
	ALPHA_2 = 255,
	ALPHA_3 = 255,
};

enum
{
	kTagTipTemplate = GUIBase::TAG_BG + 1,
	kTagTip,
};

enum
{
	kTagTipBg1,
	kTagTipBg2,
	kTagTipMsg,
};

TipGame::TipGame(void)
{
}

TipGame::~TipGame(void)
{
	list<TipMsg*>::iterator iter;
	for (iter = _tipList.begin(); iter != _tipList.end(); iter++)
	{
		delete (*iter);
	}
	_tipList.clear();
}

void TipGame::doOpen(const string& str)
{
	GUIBase::doOpen();
	setNoBlack();

	scheduleUpdate();

	putTip(str);
}

void TipGame::putTip(const string& str)
{
	CCNode* pTemplate = m_pCcbNode->getChildByTag(kTagTipTemplate);
	pTemplate->setVisible(false);
	CCSprite* pTemplateBg1 = dynamic_cast<CCSprite*>(pTemplate->getChildByTag(kTagTipBg1));
	CCSprite* pTemplateBg2 = dynamic_cast<CCSprite*>(pTemplate->getChildByTag(kTagTipBg2));
	CCLabelTTF* pTemplateMsg = dynamic_cast<CCLabelTTF*>(pTemplate->getChildByTag(kTagTipMsg));

	CCNode* pNode = CCNode::create();
	Utils::copyCCNode(pNode, pTemplate);
	m_pCcbNode->addChild(pNode, m_pCcbNode->getChildrenCount(), kTagTip);

	/// icon bg
	CCSprite* pItemBg1 = CCSprite::create();
	Utils::copyCCSprite(pItemBg1, pTemplateBg1);
	pNode->addChild(pItemBg1, pNode->getChildrenCount(), kTagTipBg1);

	CCSprite* pItemBg2 = CCSprite::create();
	Utils::copyCCSprite(pItemBg2, pTemplateBg2);
	pNode->addChild(pItemBg2, pNode->getChildrenCount(), kTagTipBg2);

	// str
	CCLabelTTF* pItemMsg = Utils::copyCCLabelTTF(pTemplateMsg);
	pItemMsg->setString(str.c_str());
	pNode->addChild(pItemMsg, pNode->getChildrenCount(), kTagTipMsg);

	// shadow
	ccFontDefinition shadowTextDef;
	shadowTextDef.m_fontSize = pItemMsg->getFontSize();
	shadowTextDef.m_fontName = pItemMsg->getFontName();
	shadowTextDef.m_shadow.m_shadowEnabled = true;
	shadowTextDef.m_shadow.m_shadowOffset = CCSizeMake(0, -3.0);
	shadowTextDef.m_shadow.m_shadowOpacity = 1.0;
	shadowTextDef.m_shadow.m_shadowBlur = 1.0;
	shadowTextDef.m_fontFillColor = pItemMsg->getColor();

	pItemMsg->setTextDefinition(&shadowTextDef);

	TipMsg* pTipMsg = new TipMsg();
	pTipMsg->m_pNode = pNode;
	pTipMsg->_alpha = 0;
	pTipMsg->_alpha_max = ALPHA_1;
	pTipMsg->_time = 0;
	pTipMsg->_tipMsgState = kStateAppear;

	setAlpha(pTipMsg);

	_tipList.push_back(pTipMsg);
	if (_tipList.size() > 3)
	{
		removeTip();
	}

	int i = 0;
	list<TipMsg*>::iterator iter;
	int count = _tipList.size();
	for (iter = _tipList.begin(); iter != _tipList.end(); iter++)
	{
		// 旧的往上移动
		if (i == count - 1)
		{
			break;
		}

		int y = (*iter)->m_pNode->getPositionY() + OFFSET_Y;
		(*iter)->m_pNode->setPositionY(y);

		switch (i)
		{
		case 0:
			(*iter)->_alpha_max = ALPHA_3;
			break;

		case 1:
			(*iter)->_alpha_max = ALPHA_2;
			break;

		default:
			break;
		}
		i++;

		(*iter)->_alpha = MIN((*iter)->_alpha, (*iter)->_alpha_max);

		if ((*iter)->_tipMsgState == kStateAppear)
		{
			(*iter)->_tipMsgState = kStateShow;
			(*iter)->_time = MAX((*iter)->_time, TIME_1);
			(*iter)->_alpha = (*iter)->_alpha_max;
		}

		setAlpha(*iter);
	}
}

void TipGame::removeTip()
{
	TipMsg* unused = _tipList.front();
	unused->m_pNode->removeFromParent();
	delete unused;
	_tipList.pop_front();
}

void TipGame::update(float delta)
{
	list<TipMsg*> _removeList;
	list<TipMsg*>::iterator iter;
	for (iter = _tipList.begin(); iter != _tipList.end(); iter++)
	{
		TipMsg* pTipMsg = (*iter);
		pTipMsg->_time += delta;
		switch (pTipMsg->_tipMsgState)
		{
		case kStateAppear:
			{
				// 出现
				pTipMsg->_alpha = pTipMsg->_time * pTipMsg->_alpha_max / TIME_1;

				if (pTipMsg->_time >= TIME_1)
				{
					pTipMsg->_alpha = pTipMsg->_alpha_max;
					pTipMsg->_tipMsgState = kStateShow;
				}

				setAlpha(pTipMsg);
			}
			break;

		case kStateShow:
			{
				// 正常显示
				if (pTipMsg->_time >= TIME_2)
				{
					pTipMsg->_tipMsgState = kStateDisappear;
				}
			}
			break;

		case kStateDisappear:
			{
				// 消失
				pTipMsg->_alpha = (TIME_3 - pTipMsg->_time)* pTipMsg->_alpha_max / (TIME_3 - TIME_2);

				if (pTipMsg->_time >= TIME_3)
				{
					pTipMsg->_alpha = 0;
					pTipMsg->_tipMsgState = kStateInvalid;
				}

				setAlpha(pTipMsg);
			}
			break;

		case kStateInvalid:
			_removeList.push_back(pTipMsg);
			break;

		default:
			break;
		}
	}

	// check remove
	if (!_removeList.empty())
	{
		list<TipMsg*>::iterator iter;
		for (iter = _removeList.begin(); iter != _removeList.end(); iter++)
		{
			_tipList.remove(*iter);
			delete *iter;
		}
		_removeList.clear();
	}

	if (_tipList.empty())
	{
		doClose();
	}
}

void TipGame::setAlpha(TipMsg* pTipMsg)
{
	CCSprite* pBg1 = dynamic_cast<CCSprite*>(pTipMsg->m_pNode->getChildByTag(kTagTipBg1));
	pBg1->setOpacity(pTipMsg->_alpha);

	CCSprite* pBg2 = dynamic_cast<CCSprite*>(pTipMsg->m_pNode->getChildByTag(kTagTipBg2));
	pBg2->setOpacity(pTipMsg->_alpha);

	CCLabelTTF* pMsg = dynamic_cast<CCLabelTTF*>(pTipMsg->m_pNode->getChildByTag(kTagTipMsg));
	pMsg->setOpacity(pTipMsg->_alpha);
}