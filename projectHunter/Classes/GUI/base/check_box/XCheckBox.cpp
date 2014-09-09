//
//  XCheckBox.h
//

#include "XCheckBox.h"
#include "../../../utils/Utils.h"

USING_NS_CC;
USING_NS_CC_EXT;

XCheckBox::XCheckBox()
	: m_bToggle(false)
{
}

XCheckBox::~XCheckBox()
{
}

XCheckBox* XCheckBox::create()
{
	XCheckBox *pRet = new XCheckBox();
	pRet->init();
	pRet->autorelease();
	return pRet;
}

XCheckBox* XCheckBox::create(CCControlButton* pBtn)
{
	XCheckBox *pRet = new XCheckBox();
	pRet->init();
	pRet->autorelease();

	Utils::copyCCControlButton(pRet, pBtn);

	return pRet;
}

void XCheckBox::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
	m_isPushed = false;

	toggle();

	if (isTouchInside(pTouch))
	{
		sendActionsForControlEvents(CCControlEventTouchUpInside);        
	}
	else
	{
		sendActionsForControlEvents(CCControlEventTouchUpOutside);        
	}
}

void XCheckBox::setTarget(CCObject* target, SEL_CCControlHandler selector)
{
	addTargetWithActionForControlEvents(this, selector, CCControlEventTouchUpInside);
}

void XCheckBox::toggle()
{
	setToggle(!m_bToggle);
}

void XCheckBox::setToggle(bool toggle)
{
	m_bToggle = toggle;
	setHighlighted(toggle);
}