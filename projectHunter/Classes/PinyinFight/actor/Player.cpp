#include "Player.h"

USING_NS_CC;

Player::Player(void)
{
	
}

Player::~Player(void)
{
}

void Player::doOpen()
{
	Actor::doOpen();
}


void Player::ActorInit()
{
	throw std::logic_error("The method or operation is not implemented.");
}

void Player::ActorThink()
{
	throw std::logic_error("The method or operation is not implemented.");
}

void Player::ActorAttack()
{
	throw std::logic_error("The method or operation is not implemented.");
}

void Player::ActorHurt()
{
	throw std::logic_error("The method or operation is not implemented.");
}

void Player::ActorTurnEnd()
{
	throw std::logic_error("The method or operation is not implemented.");
}
