#ifndef __APPMACROS_H__
#define __APPMACROS_H__

#include "cocos2d.h"
USING_NS_CC;
using namespace std;

#if COCOS2D_DEBUG > 0

	// #include "vld.h"
    //#define _DEBUG1
    #ifdef _DEBUG1
        #define NEW_DBG new(_NORMAL_BLOCK, __FILE__, __LINE__)
        #define _CRTDBG_MAP_ALLOC
        #include<crtdbg.h>
        #define new NEW_DBG
    #endif // _DEBUG1


    //#define DEBUG_PVP
    // #define DEBUG_PVP_ROOM
	// #define DEBUG_TASK
	// #define DEBUG_STATISTICS
#endif // COCOS2D_DEBUG

// 检查NULL字符串
#define CHECK_STRING(a) (a == NULL) ? "null" : a

#define COPY_VECTOR_RETAIN(a, b) \
for (int k = 0; k < (int)b.size(); k++) \
{ \
	b.at(k)->retain(); \
	a.push_back(b.at(k));\
}

#define RELEASE_VECTOR(a) \
for (int k = 0; k < (int)a.size(); k++) \
{ \
	a.at(k)->release(); \
} \
a.clear();

#define RELEASE_MAP(A, B, c) \
{ \
	map<A, B>::iterator iter100; \
	for (iter100 = c.begin(); iter100 != c.end(); iter100++) \
	{ \
		delete iter100->second; \
	} \
	c.clear(); \
}

/* For demonstrating using one design resolution to match different resources,
   or one resource to match different design resolutions.

   [Situation 1] Using one design resolution to match different resources.
     Please look into Appdelegate::applicationDidFinishLaunching.
     We check current device frame size to decide which resource need to be selected.
     So if you want to test this situation which said in title '[Situation 1]',
     you should change ios simulator to different device(e.g. iphone, iphone-retina3.5, iphone-retina4.0, ipad, ipad-retina),
     or change the window size in "proj.XXX/main.cpp" by "CCEGLView::setFrameSize" if you are using win32 or linux plaform
     and modify "proj.mac/AppController.mm" by changing the window rectangle.

   [Situation 2] Using one resource to match different design resolutions.
     The coordinates in your codes is based on your current design resolution rather than resource size.
     Therefore, your design resolution could be very large and your resource size could be small.
     To test this, just define the marco 'TARGET_DESIGN_RESOLUTION_SIZE' to 'DESIGN_RESOLUTION_2048X1536'
     and open iphone simulator or create a window of 480x320 size.

   [Note] Normally, developer just need to define one design resolution(e.g. 960x640) with one or more resources.
 */

#define DESIGN_RESOLUTION_480X320    0
#define DESIGN_RESOLUTION_960X640    1
#define DESIGN_RESOLUTION_1024X768   2
#define DESIGN_RESOLUTION_2048X1536  3

/* If you want to switch design resolution, change next line */
#define TARGET_DESIGN_RESOLUTION_SIZE  DESIGN_RESOLUTION_960X640

typedef struct tagResource
{
    cocos2d::CCSize size;
    char directory[100];
}Resource;

//#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS) || (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
static Resource iphone4Resource  =  { cocos2d::CCSizeMake(960, 640),   "resources-iphone" };
static Resource iphone5Resource  =  { cocos2d::CCSizeMake(1136, 640),   "resources-ipad" };
static Resource S3Resource =  { cocos2d::CCSizeMake(1280, 720),  "resources-large"   };
//static Resource mediumResource =  { cocos2d::CCSizeMake(1024, 768),  "resources-ipad"   };
//static Resource largeResource  =  { cocos2d::CCSizeMake(2048, 1536), "resources-ipadhd" };
//#endif

//#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//static Resource smallestResource  =  { cocos2d::CCSizeMake(480, 320),   "resources-small" };
//static Resource smallResource  =  { cocos2d::CCSizeMake(800, 480),   "resources-medium" };

//static Resource largeResource  =  { cocos2d::CCSizeMake(2048, 1536), "resources-xlarge" };
//#endif

#if (TARGET_DESIGN_RESOLUTION_SIZE == DESIGN_RESOLUTION_480X320)
static cocos2d::CCSize designResolutionSize = cocos2d::CCSizeMake(480, 320);
#elif (TARGET_DESIGN_RESOLUTION_SIZE == DESIGN_RESOLUTION_960X640)
static cocos2d::CCSize designResolutionSize = cocos2d::CCSizeMake(640,1136);
#elif (TARGET_DESIGN_RESOLUTION_SIZE == DESIGN_RESOLUTION_1024X768)
static cocos2d::CCSize designResolutionSize = cocos2d::CCSizeMake(1024, 768);
#elif (TARGET_DESIGN_RESOLUTION_SIZE == DESIGN_RESOLUTION_2048X1536)
static cocos2d::CCSize designResolutionSize = cocos2d::CCSizeMake(2048, 1356);
#else
#error unknown target design resolution!
#endif

// The font size 24 is designed for small resolution, so we should change it to fit for current design resolution
#define TITLE_FONT_SIZE  (cocos2d::CCEGLView::sharedOpenGLView()->getDesignResolutionSize().width / iphone4Resource.size.width * 24)


//*********************方向控制部分
//使用8方向按键控制方向
//#define USE_CONTROL_8_DIRECTION


//使用摇杆控制方向
#define USE_CONTROL_JOYSTICK 
//摇杆和上，下，左，右角度分别小于57.29...度（1.0）时才开始朝该方向移动， 比如如果在右上方 80度位置，则更倾向于只向上移动，无向右移动倾向
#define USE_MOVE_ALWAYS_IN_LINE
//以摇杆的施力长度转化为速度进行移动
//#define USE_CONTROL_JOYSTICK_VELOCITY 
//使用攻击摇杆
//#define USE_CONTROL_ATTACKSTICK


//使用摇杆跟随的方式控制方//#define USE_CONTROL_FOLLOW_TOUCH
//*********************方向控制部分



// android effect only support ogg
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#define EFFECT_FILE        "effect2.ogg"
#elif( CC_TARGET_PLATFORM == CC_PLATFORM_MARMALADE)
#define EFFECT_FILE        "effect1.raw"
#else
#define EFFECT_FILE        "effect1.wav"
#endif // CC_PLATFORM_ANDROID

#define ACTOR_TRIGGER_LINK_MAX_SUPPORT		5
//#define GetCurMilliSecond(m) struct cc_timeval now; \
//	CCTime::gettimeofdayCocos2d(&now, NULL); \
//	m = (now.tv_sec * 1000 + now.tv_usec / 1000);

#endif /* __APPMACROS_H__ */
