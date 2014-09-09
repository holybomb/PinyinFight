#ifndef __CCS_PLAYERPOOL_H__
#define __CCS_PLAYERPOOL_H__

#include "cocos2d.h"
#include "cocos-ext.h"

#include "CCSPlayer.h"

#define DEFAULT_POOL_MAX	5

typedef struct {
	int				ID;//索引
	bool			isFree;//是否空闲
	CCSPlayer	    *ccs_player;
} PoolElement;
class CCSPlayerPool : public CCObject
{
public:
	CCSPlayerPool();
	~CCSPlayerPool();

	std::vector<PoolElement> m_pool;
	bool b_isCreateNewWhenFull;

	static CCSPlayerPool *create(const char* fileName, CCObject* target, SEL_CCS_CB selector);
	static CCSPlayerPool *create(const char* fileName, int defaultMax, CCObject* target, SEL_CCS_CB selector);
	static CCSPlayerPool *create(const char* fileName, int defaultMax, CCObject* target, SEL_CCS_CB selector, bool isCreateNewWhenFull);

	void createPool(const char* fileName, int defaultMax, CCObject* target, SEL_CCS_CB selector, bool isCreateNewWhenFull);
	PoolElement create_PoolElement(const char* fileName, int index, CCObject* target, SEL_CCS_CB selector);
	CCSPlayer *getFreeCCSPlayer();
	bool isCanGetFreeCCSPlayer();//查看当前还能否获得新的ccsplayer,主要是在b_isCreateNewWhenFull = false的时候检查还能否再分配只用
	void resetCCSPlayer2Free(CCSPlayer *ccsPlayer);

	void releasePool();

	std::string		poolFileName;
	CCObject*		poolTarget;
	SEL_CCS_CB		poolSelector;
};

#endif /* __CCS_PLAYER_H__ */
