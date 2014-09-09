#include "CCSPlayerPool.h"
#include "defines.h"
USING_NS_CC;
USING_NS_CC_EXT;

#define DEBUG_CCSPLAYER_RECT false

CCSPlayerPool::CCSPlayerPool()
{
	b_isCreateNewWhenFull = true;
}

CCSPlayerPool::~CCSPlayerPool()
{
	releasePool();
}

CCSPlayerPool *CCSPlayerPool::create(const char* fileName, CCObject* target, SEL_CCS_CB selector)
{
	return CCSPlayerPool::create(fileName, DEFAULT_POOL_MAX, target, selector);
}

CCSPlayerPool *CCSPlayerPool::create(const char* fileName, int defaultMax, CCObject* target, SEL_CCS_CB selector)
{
	return CCSPlayerPool::create(fileName, defaultMax, target, selector, true);
}

CCSPlayerPool *CCSPlayerPool::create(const char* fileName, int defaultMax, CCObject* target, SEL_CCS_CB selector, bool isCreateNewWhenFull)
{
	CCSPlayerPool *pRet = new CCSPlayerPool();
	if(pRet)
	{
		pRet->createPool(fileName, defaultMax, target, selector, isCreateNewWhenFull);
		pRet->autorelease();
	}
	return pRet;
}


void CCSPlayerPool::createPool(const char* fileName, int defaultMax, CCObject* target, SEL_CCS_CB selector, bool isCreateNewWhenFull)
{
	CCSPlayer::addArmatureFileInfo(CCSPlayer::getCocostudioPath(fileName).c_str());

	for(int i = 0 ; i < defaultMax; i++)
	{
		m_pool.push_back(create_PoolElement(fileName, i, target, selector));
	}

	poolTarget = target;
	poolSelector = selector;
	poolFileName = fileName;

	b_isCreateNewWhenFull = isCreateNewWhenFull;
}

PoolElement CCSPlayerPool::create_PoolElement(const char* fileName, int index, CCObject* target, SEL_CCS_CB selector)
{
	PoolElement dg;
	dg.ID = index;
	dg.isFree = true;
	dg.ccs_player = CCSPlayer::create(fileName);
	dg.ccs_player->retain();
	dg.ccs_player->setTarget(target, selector);
	return dg;
}

CCSPlayer *CCSPlayerPool::getFreeCCSPlayer()
{
	for(int i = 0; i < m_pool.size(); i++)
	{
		if(m_pool.at(i).isFree)
		{
			m_pool.at(i).isFree = false;
			return m_pool.at(i).ccs_player;
		}
	}

	if(m_pool.size() > 0 && b_isCreateNewWhenFull)
	{
		CCSPlayer::addArmatureFileInfo(CCSPlayer::getCocostudioPath(poolFileName.c_str()).c_str());
		PoolElement dg = create_PoolElement(poolFileName.c_str(), m_pool.size(), poolTarget, poolSelector);
		dg.isFree = false;
		m_pool.push_back(dg);
		return dg.ccs_player;
	}

	return NULL;
}

bool CCSPlayerPool::isCanGetFreeCCSPlayer()//查看当前还能否获得新的ccsplayer,主要是在b_isCreateNewWhenFull = false的时候检查还能否再分配只用
{
	if(b_isCreateNewWhenFull)
		return true;

	for(int i = 0; i < m_pool.size(); i++)
	{
		if(m_pool.at(i).isFree)
		{
			return true;
		}
	}
	return false;
}

void CCSPlayerPool::resetCCSPlayer2Free(CCSPlayer *ccsPlayer)
{
	for(int i = 0 ; i < m_pool.size(); i++)
	{
		if(m_pool.at(i).ccs_player == ccsPlayer)
		{
			m_pool.at(i).isFree = true;
			m_pool.at(i).ccs_player->reset();
			m_pool.at(i).ccs_player->removeFromParent();
			break;
		}
	}
}

void CCSPlayerPool::releasePool()
{
	for(int i = 0 ; i < m_pool.size(); i++)
	{
		m_pool.at(i).ccs_player->release();
	}
	m_pool.clear();

	poolTarget = NULL;
	poolSelector = NULL;
}