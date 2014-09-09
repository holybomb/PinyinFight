#include "Actor.h"

USING_NS_CC;

Actor::Actor(void)
{
	
}

Actor::~Actor(void)
{
}

void Actor::doOpen()
{
	GUIBase::doOpen();
	scheduleUpdate();
}

void Actor::update(float delta)
{
	switch (_state)
	{
	case kStateInit:
		ActorInit();
		break;
	case kStateThink:
		ActorThink();
		break;
	case kStateAttack:
		ActorAttack();
		break;
	case kStateWait:
		ActorWait();
		break;
	case kStateHurt:
		ActorHurt();
		break;
	case kStateTurnEnd:
		ActorTurnEnd();
		break;
	default:
		break;
	}
}

void Actor::ActorInit()
{
	throw std::logic_error("The method or operation is not implemented.");
}

void Actor::ActorThink()
{
	throw std::logic_error("The method or operation is not implemented.");
}

void Actor::ActorAttack()
{
	throw std::logic_error("The method or operation is not implemented.");
}

void Actor::ActorHurt()
{
	throw std::logic_error("The method or operation is not implemented.");
}

void Actor::ActorTurnEnd()
{
	throw std::logic_error("The method or operation is not implemented.");
}

void Actor::ActorWait()
{
	throw std::logic_error("The method or operation is not implemented.");
}
