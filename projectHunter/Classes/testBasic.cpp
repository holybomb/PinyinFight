#include "testBasic.h"
#include "utils/ccsplayer/CCSPlayer.h"

TestScene::TestScene()
	: ccbNode(NULL)
{
    CCScene::init();
}

TestScene::~TestScene()
{
    CC_SAFE_RELEASE_NULL(ccbNode);
}

void TestScene::onEnter()
{
    CCScene::onEnter();
}

void TestScene::runThisTest()
{
	runThisTest(true);
}

void TestScene::runThisTest(bool needClearCmd)
{
    // default transition effect
    CCDirector::sharedDirector()->setDepthTest(false);
 	CCTransitionScene* transition = CCTransitionFade::create(1.0f, this, ccBLACK);
	if(!CCDirector::sharedDirector()->getRunningScene())
		CCDirector::sharedDirector()->replaceScene(transition);
	else
		CCDirector::sharedDirector()->runWithScene(transition);
}

#ifdef DEBUG_ANIM_RECT
DebugRect::DebugRect()
{

}

void DebugRect::draw()
{

	for(int i = 0; i < rect.size(); i++)
	{
		ccDrawColor4B(255, 255, 255, 255);
		if(rect.at(i).type == 1)
			ccDrawColor4B(0, 255, 0, 255);
		else if(rect.at(i).type == 2)
			ccDrawColor4B(255, 255, 255, 255);
		else if(rect.at(i).type == 3)
			ccDrawColor4B(255, 255, 0, 255);
		else if(rect.at(i).type == 0)
			ccDrawColor4B(128, 128, 128, 255);
		else if(rect.at(i).type == 299 || rect.at(i).type == 199)
			ccDrawColor4B(185, 20, 185, 255);

		ccDrawRect(ccp(rect.at(i).rect.getMinX() + rect.at(i).layerPos.x, rect.at(i).rect.getMinY() + rect.at(i).layerPos.y), ccp(rect.at(i).rect.getMaxX() + rect.at(i).layerPos.x, rect.at(i).rect.getMaxY() + + rect.at(i).layerPos.y));

	}
}
#endif // DEBUG_ANIM_RECT
