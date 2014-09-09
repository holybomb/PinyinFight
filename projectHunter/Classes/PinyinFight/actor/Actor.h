#include "../../GUI/base/GUIBase.h"
#ifndef __ACTOR_H__
#define __ACTOR_H__
enum
{
	kStateInit,
	kStateThink,
	kStateAttack,
	kStateWait,
	kStateHurt,
	kStateTurnEnd,
};

/**
 * ActorΩÁ√Ê
 */
class Actor : public GUIBase
{
public:
	Actor(void);
	virtual ~Actor(void);

	CREATE_FUNC(Actor);

	virtual void doOpen();
	void update(float delta);
	virtual void ActorInit();
	virtual void ActorThink();
	virtual void ActorAttack();
	virtual void ActorHurt();
	virtual void ActorTurnEnd();
	virtual void ActorWait();
private:
	int _state;
private:
};

#endif // __ACTOR_H__