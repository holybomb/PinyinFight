#ifndef _TEST_BASIC_H_
#define _TEST_BASIC_H_

#include "cocos2d.h"
#include "utils/VisibleRect.h"
#include "AppMacros.h"
/**
 * define a create function for scene
 * @__TYPE__ class type to add create(), such as TestScene
 */
#define CREATE_SCENE_FUNC(__TYPE__) \
static __TYPE__* create() \
{ \
    __TYPE__ *pRet = new __TYPE__(); \
    if (pRet && pRet->init()) \
    { \
        pRet->prepareThisScene(); \
        pRet->autorelease(); \
        return pRet; \
    } \
    else \
    { \
        delete pRet; \
        pRet = NULL; \
        return NULL; \
    } \
}

class TestScene : public CCScene
{
public: 
    TestScene();
    virtual ~TestScene();
    virtual void onEnter();

    virtual void runThisTest();
	virtual void runThisTest(bool needClearCmd);
    virtual void prepareThisScene() = 0;

public:
	CCNode*      ccbNode;
};

#ifdef DEBUG_ANIM_RECT
typedef struct {
	CCRect	rect;
	char	type;
	CCPoint	layerPos;
}debugRect;

class DebugRect : public CCNode
{
public: 
	DebugRect();

    std::vector<debugRect> rect;
    
	void draw();
};
#endif // DEBUG_ANIM_RECT

#endif // _TEST_BASIC_H_
