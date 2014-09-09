#include "Actor.h"
#ifndef __PLAYER_H__
#define __PLAYER_H__
/**
 * PlayerΩÁ√Ê
 */
class Player : public Actor
{
public:
	Player(void);
	virtual ~Player(void);

	CREATE_FUNC(Player);

	virtual void doOpen();
	virtual void ActorInit();
	virtual void ActorThink();
	virtual void ActorAttack();
	virtual void ActorHurt();
	virtual void ActorTurnEnd();
private:
};

#endif // __PLAYER_H__