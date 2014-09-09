//
//  XCheckBox.h
//

#ifndef __X_CHECK_BOX_H__
#define __X_CHECK_BOX_H__

#include "cocos-ext.h"

class  XCheckBox : public cocos2d::extension::CCControlButton
{
public:
	XCheckBox();
	virtual ~XCheckBox();

	static XCheckBox* create();
	static XCheckBox* create(cocos2d::extension::CCControlButton* pBtn);

	virtual void ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);

	void setTarget(CCObject* target, cocos2d::extension::SEL_CCControlHandler selector);

	void toggle();

	void setToggle(bool toggle);
	inline bool getToggle() { return m_bToggle; }

private:
	bool m_bToggle;
};

#endif // __X_CHECK_BOX_H__