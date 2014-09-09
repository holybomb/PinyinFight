#ifndef __TIP_GAME_H__
#define __TIP_GAME_H__

#include "../base/GUIBase.h"

enum kTipMsgState
{
	kStateAppear,
	kStateShow,
	kStateDisappear,
	kStateInvalid,
};

struct TipMsg
{
	kTipMsgState _tipMsgState;
	cocos2d::CCNode* m_pNode;
	int _alpha_max;
	int _alpha;
	float _time;
};

/**
 * ”Œœ∑Ã· æ
 */
class TipGame : public GUIBase
{
public:
	TipGame(void);
	virtual ~TipGame(void);

	CREATE_FUNC(TipGame);

	virtual void update(float delta);

	void doOpen(const std::string& str);

	void putTip(const std::string& str);

private:
	void removeTip();
	void setAlpha(TipMsg* pTipMsg);

private:
	std::list<TipMsg*> _tipList;
};

#endif // __TIP_GAME_H__