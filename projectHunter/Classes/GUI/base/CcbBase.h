#ifndef __CCB_BASE_H__
#define __CCB_BASE_H__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "../../defines.h"

/**
 * ccb base
 */
class XCCBReader;
class CcbBase : public cocos2d::CCLayer
{
    // CCB node
    CC_SYNTHESIZE_READONLY(CCNode*, m_pCcbNode, CcbNode);
    
public:
    CcbBase(void);
    virtual ~CcbBase(void);

	CREATE_FUNC(CcbBase);

	virtual void update(float delta);
	virtual void visit(void);

	bool needOrder;

	float getDuration();
    /**
     * load ccb node
     */
    void loadCCB(const char* pCCBFileName);

	void runAnimation(int index, float skipTime = 0, bool loop = false);
	void runAnimation(const char* anim, float skipTime = 0);

	void setAnimationCompletedCallback(CCObject *target, cocos2d::SEL_CallFunc callbackFunc);

	cocos2d::extension::CCBAnimationManager* getAnimationManager();

	void setNeedRemove(bool remove);

private:
	void skip(cocos2d::CCNode* node, float skipTime);
	void animationCompletedCallback();

private:
	XCCBReader* m_pCCBReader;
	LONG_TYPE _startTime;
	int _index;
	float _skipTime;
	bool _needCallBack;
	bool _needRemove; // 非循环动画结束后是否删掉
};

#endif // __CCB_BASE_H__