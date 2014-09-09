#ifndef __CCS_PLAYERPOOL_H__
#define __CCS_PLAYERPOOL_H__

#include "cocos2d.h"
#include "cocos-ext.h"

#include "CCSPlayer.h"

#define DEFAULT_POOL_MAX	5

typedef struct {
	int				ID;//����
	bool			isFree;//�Ƿ����
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
	bool isCanGetFreeCCSPlayer();//�鿴��ǰ���ܷ����µ�ccsplayer,��Ҫ����b_isCreateNewWhenFull = false��ʱ���黹�ܷ��ٷ���ֻ��
	void resetCCSPlayer2Free(CCSPlayer *ccsPlayer);

	void releasePool();

	std::string		poolFileName;
	CCObject*		poolTarget;
	SEL_CCS_CB		poolSelector;
};

#endif /* __CCS_PLAYER_H__ */
