#include "Npc.h"

USING_NS_CC;

Npc::Npc(void)
{
	
}

Npc::~Npc(void)
{
}

void Npc::doOpen()
{
	Actor::doOpen();
}


void Npc::ActorInit()
{
	throw std::logic_error("The method or operation is not implemented.");
}

void Npc::ActorThink()
{
	throw std::logic_error("The method or operation is not implemented.");
}

void Npc::ActorAttack()
{
	throw std::logic_error("The method or operation is not implemented.");
}

void Npc::ActorHurt()
{
	throw std::logic_error("The method or operation is not implemented.");
}

void Npc::ActorTurnEnd()
{
	throw std::logic_error("The method or operation is not implemented.");
}
