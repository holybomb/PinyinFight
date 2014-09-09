#include "CcbBase.h"
#include "util/XCCBReader.h"
#include "../../CSystem/CSystem.h"

USING_NS_CC;
USING_NS_CC_EXT;

#define  USING_CACHE false
enum 
{
	kTagBlack = 1000,
	kTagLoading,
};

CcbBase::CcbBase(void)
	: m_pCcbNode(NULL)
	, m_pCCBReader(NULL)
	, _needCallBack(false)
	, needOrder(false)
	, _needRemove(false)
{
}

CcbBase::~CcbBase(void)
{
	CC_SAFE_RELEASE_NULL(m_pCCBReader);
}

void CcbBase::update(float delta)
{
	if (_needCallBack)
	{
		_needCallBack = false;

		CCArray *arr = m_pCCBReader->getAnimationManager()->getSequences();

		CCBSequence* curSeq = NULL;
		CCObject *pElement = NULL;
		CCARRAY_FOREACH(arr, pElement)
		{
			CCBSequence *seq = (CCBSequence*)pElement;
			if (seq->getSequenceId() == _index)
			{
				curSeq = seq;
				break;
			}
		}

		if (curSeq)
		{
			m_pCCBReader->getAnimationManager()->runAnimationsForSequenceIdTweenDuration(_index, 0);
			setAnimationCompletedCallback(this, callfunc_selector(CcbBase::animationCompletedCallback));

			float dt = (CSystem::currentTimeMillis() - _startTime) / 1000.0f;
			dt = dt + _skipTime - curSeq->getDuration();

			while (dt > curSeq->getDuration())
			{
				dt -= curSeq->getDuration();
			}

			// dt可能为负数 暂时忽略不计
			if(dt > 0)
				skip(m_pCcbNode, dt);

			_skipTime = dt > 0 ? dt : 0;
			_startTime = CSystem::currentTimeMillis();
			// CCLOG("m_pCcbNode:%d skipTime:%f",m_pCcbNode, dt);
		}
	}
}

void CcbBase::visit()
{
	// quick return if not visible. children won't be drawn.
	if (!m_bVisible)
	{
		return;
	}
	kmGLPushMatrix();

	if (m_pGrid && m_pGrid->isActive())
	{
		m_pGrid->beforeDraw();
	}

	this->transform();

	CCNode* pNode = NULL;
	unsigned int i = 0;
#ifdef Hunter
	beforeDraw();
#endif // Hunter
	if(m_pChildren && m_pChildren->count() > 0)
	{
		sortAllChildren();
		// draw children zOrder < 0
		ccArray *arrayData = m_pChildren->data;
		for( ; i < arrayData->num; i++ )
		{
			pNode = (CCNode*) arrayData->arr[i];

			if (pNode == m_pCcbNode)
			{
				if(!isUseBufferOffset())
				{
					CCPoint curPos = m_pCcbNode->getPosition();
					CCPoint cp = m_pCcbNode->convertToWorldSpace(curPos);

					CCSize s = CCDirector::sharedDirector()->getWinSize();
					if(cp.x + s.width / 2< 0 || cp.x -  s.width/2> s.width)
					{
						// CCLog("cp (%f, %f) ---------point x %f y %f ----" ,cp.x,cp.y, curPos.x,curPos.y, );
						continue;
					}
				}
			}
			
			if ( pNode && pNode->getZOrder() < 0 ) 
			{
				pNode->visit();
			}
			else
			{
				break;
			}
		}
		// self draw
		this->draw();

		for( ; i < arrayData->num; i++ )
		{
			pNode = (CCNode*) arrayData->arr[i];
			if (pNode)
			{
				pNode->visit();
			}
		}        
	}
	else
	{
		this->draw();
	}
#ifdef Hunter
	afterDraw();
#endif // Hunter
	// reset for next frame
	m_uOrderOfArrival = 0;

	if (m_pGrid && m_pGrid->isActive())
	{
		m_pGrid->afterDraw(this);
	}

	kmGLPopMatrix();
}


float CcbBase::getDuration()
{
	float dur = 0;
	CCArray *arr = m_pCCBReader->getAnimationManager()->getSequences();
	CCObject *pElement = NULL;
	CCARRAY_FOREACH(arr, pElement)
	{
		CCBSequence *seq = (CCBSequence*)pElement;
		if (seq->getDuration() > dur)
		{
			dur = seq->getDuration();
		}
	}
	return dur;
}

/**
 * load ccb node
 */
void CcbBase::loadCCB(const char* pCCBFileName)
{
    CCNodeLoaderLibrary* pCcNodeLoaderLibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
	pCcNodeLoaderLibrary->registerDefaultCCNodeLoaders();

	/* Create an autorelease CCBReader. */
	m_pCCBReader = new XCCBReader(pCcNodeLoaderLibrary);
    
	/* Read a ccbi file. */
	m_pCcbNode = m_pCCBReader->readNodeGraphFromFile(pCCBFileName);

	addChild(m_pCcbNode);
}

void CcbBase::runAnimation(int index, float skipTime, bool loop)
{
	if (loop)
	{
		setAnimationCompletedCallback(this, callfunc_selector(CcbBase::animationCompletedCallback));
		_startTime = CSystem::currentTimeMillis();
		_index = index;
		_skipTime = skipTime;
		scheduleUpdate();
	}

	m_pCCBReader->getAnimationManager()->runAnimationsForSequenceIdTweenDuration(index, 0);

	if(skipTime != 0)
		skip(m_pCcbNode, skipTime);
}

void CcbBase::runAnimation(const char* anim, float skipTime)
{
	m_pCCBReader->getAnimationManager()->runAnimationsForSequenceNamedTweenDuration(anim, 0);

	//CCLOG("m_pCcbNode:%d skipTime:%f",m_pCcbNode, skipTime);
	// CCLOG("m_pCcbNode:%d", m_pCcbNode);

	if(skipTime > 0)
		skip(m_pCcbNode, skipTime);
}

void CcbBase::setAnimationCompletedCallback(CCObject *target, SEL_CallFunc callbackFunc)
{
	m_pCCBReader->getAnimationManager()->setAnimationCompletedCallback(target, callbackFunc);
}

CCBAnimationManager* CcbBase::getAnimationManager()
{
	return m_pCCBReader->getAnimationManager();
}

void CcbBase::skip(CCNode* node, float skipTime)
{
	CCDirector::sharedDirector()->getActionManager()->skip(node, 0);
	CCDirector::sharedDirector()->getActionManager()->skip(node, skipTime);

	CCArray* arr = node->getChildren();
	CCObject* obj = NULL;
	CCARRAY_FOREACH(arr, obj)
	{
		skip((CCNode*)obj, skipTime);
	}
}

void CcbBase::animationCompletedCallback()
{
	_needCallBack = true;

	if (_needRemove)
	{
		removeFromParent();
	}
}

void CcbBase::setNeedRemove(bool remove)
{
	_needRemove = remove;
	if (remove)
	{
		setAnimationCompletedCallback(this, callfunc_selector(CcbBase::animationCompletedCallback));
	}
}
