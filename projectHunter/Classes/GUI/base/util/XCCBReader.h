#ifndef __X_CCBREADER_H__
#define __X_CCBREADER_H__

#include "cocos2d.h"
#include "cocos-ext.h"

/**
 * XCCBReader
 */
class XCCBReader : public cocos2d::extension::CCBReader
{
public:
	XCCBReader(cocos2d::extension::CCNodeLoaderLibrary *pCCNodeLoaderLibrary, 
		cocos2d::extension::CCBMemberVariableAssigner *pCCBMemberVariableAssigner = NULL, 
		cocos2d::extension::CCBSelectorResolver *pCCBSelectorResolver = NULL, 
		cocos2d::extension::CCNodeLoaderListener *pCCNodeLoaderListener = NULL);

	void pause();
	void resume();
	void restart();
};

#endif // __X_CCBREADER_H__