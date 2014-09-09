#include "AppDelegate.h"

#include <vector>
#include <string>

//#include "HelloWorldScene.h"
#include "AppMacros.h"
#include "cocos2d.h"
//#include "scenes/first_scene/FirstScene.h"
//#include "DataStatistics/StatisticsRunner.h"
#include "PinyinFight/scenes/MainGameScene.h"

USING_NS_CC;
using namespace std;

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    CCDirector* pDirector = CCDirector::sharedDirector();
    CCEGLView* pEGLView = CCEGLView::sharedOpenGLView();

    pDirector->setOpenGLView(pEGLView);

	CCSize frameSize = pEGLView->getFrameSize();
	bool dd = frameSize.width / frameSize.height > designResolutionSize.width / designResolutionSize.height;
	ResolutionPolicy rp = dd ? kResolutionFixedWidth : kResolutionFixedHeight;
	
    // Set the design resolution
    pEGLView->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, kResolutionShowAll);

    vector<string> searchPath;

    // In this demo, we select resource according to the frame's height.
    // If the resource size is different from design resolution size, you need to set contentScaleFactor.
    // We use the ratio of resource's height to the height of design resolution,
    // this can make sure that the resource's height could fit for the height of design resolution.

    // if the frame's height is larger than the height of medium resource size, select large resource.
	/*if (frameSize.height > mediumResource.size.height)
	{
        searchPath.push_back(largeResource.directory);

        pDirector->setContentScaleFactor(MIN(largeResource.size.height/designResolutionSize.height, largeResource.size.width/designResolutionSize.width));
	}
    // if the frame's height is larger than the height of small resource size, select medium resource.
    else if (frameSize.height > smallResource.size.height)
    {
        searchPath.push_back(mediumResource.directory);
        
        pDirector->setContentScaleFactor(MIN(mediumResource.size.height/designResolutionSize.height, mediumResource.size.width/designResolutionSize.width));
    }
    else if (frameSize.height > smallestResource.size.height)
    {
        searchPath.push_back(smallResource.directory);
        
        pDirector->setContentScaleFactor(MIN(smallResource.size.height/designResolutionSize.height, smallResource.size.width/designResolutionSize.width));
    }
    // if the frame's height is smaller than the height of medium resource size, select small resource.
	else
    {
        searchPath.push_back(smallestResource.directory);

        pDirector->setContentScaleFactor(MIN(smallestResource.size.height/designResolutionSize.height, smallestResource.size.width/designResolutionSize.width));
    }*/

    /*
	if (frameSize.width > iphone5Resource.size.width)
	{
		searchPath.push_back(S3Resource.directory);

		pDirector->setContentScaleFactor(MIN(S3Resource.size.height/designResolutionSize.height, S3Resource.size.width/designResolutionSize.width));

	} else if(frameSize.width > iphone4Resource.size.width)
	{
		searchPath.push_back(iphone5Resource.directory);
		pDirector->setContentScaleFactor(MIN(iphone5Resource.size.height/designResolutionSize.height, iphone5Resource.size.width/designResolutionSize.width));
	} else 
	{
		searchPath.push_back(iphone4Resource.directory);
		pDirector->setContentScaleFactor(1);
	}
	*/
    
    // set searching path
    CCFileUtils::sharedFileUtils()->setSearchResolutionsOrder(searchPath);
    CCFileUtils::sharedFileUtils()->setSearchPaths(searchPath);
	
#if COCOS2D_DEBUG > 0
    // turn on display FPS
    pDirector->setDisplayStats(true);
#endif
    
    // set FPS. the default value is 1.0/60 if you don't call this
    pDirector->setAnimationInterval(1.0 / 60);

    // run
	// SCCScene *pScene = HelloWorld::scene();
	CCScene *pScene = MainGameScene::create();
	//CCScene *pScene = FirstScene::create();
    pDirector->runWithScene(pScene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    CCDirector::sharedDirector()->stopAnimation();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    CCDirector::sharedDirector()->startAnimation();
}
