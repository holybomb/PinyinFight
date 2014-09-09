#include "Actor.h"
#ifndef __NPC_H__
#define __NPC_H__
/**
 * NpcΩÁ√Ê
 */
class Npc : public Actor
{
public:
	Npc(void);
	virtual ~Npc(void);

	CREATE_FUNC(Npc);

	virtual void doOpen();


	virtual void ActorInit();
	virtual void ActorThink();
	virtual void ActorAttack();
	virtual void ActorHurt();
	virtual void ActorTurnEnd();
private:
};

#endif // __NPC_H__