#include "XCCBReader.h"

USING_NS_CC;
USING_NS_CC_EXT;

XCCBReader::XCCBReader(CCNodeLoaderLibrary *pCCNodeLoaderLibrary, 
	CCBMemberVariableAssigner *pCCBMemberVariableAssigner,
	CCBSelectorResolver *pCCBSelectorResolver,
	CCNodeLoaderListener *pCCNodeLoaderListener)
	 : CCBReader(pCCNodeLoaderLibrary, pCCBMemberVariableAssigner, pCCBSelectorResolver, pCCNodeLoaderListener)
{
}

void XCCBReader::pause()
{
	CCBAnimationManager* mActionManager = getAnimationManager();
	mActionManager->getRootNode()->pauseSchedulerAndActions();

	CCDictionary* pDic = mActionManager->getNodeSequences();
	CCDictElement* pElement = NULL;
	CCDICT_FOREACH(pDic, pElement)
	{
		CCNode *node = (CCNode*)pElement->getIntKey();
		node->pauseSchedulerAndActions();
	}
}

void XCCBReader::resume()
{
	CCBAnimationManager* mActionManager = getAnimationManager();
	mActionManager->getRootNode()->resumeSchedulerAndActions();

	CCDictionary* pDic = mActionManager->getNodeSequences();
	CCDictElement* pElement = NULL;
	CCDICT_FOREACH(pDic, pElement)
	{
		CCNode *node = (CCNode*)pElement->getIntKey();
		node->resumeSchedulerAndActions();
	}
}

void XCCBReader::restart()
{
	CCBAnimationManager* mActionManager = getAnimationManager();
	if (mActionManager->getAutoPlaySequenceId() != -1 && !jsControlled)
	{
		// Auto play animations
		mActionManager->runAnimationsForSequenceIdTweenDuration(mActionManager->getAutoPlaySequenceId(), 0);
	}
}


