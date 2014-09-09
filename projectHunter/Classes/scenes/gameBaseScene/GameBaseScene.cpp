#include "GameBaseScene.h"

GameBaseScene::GameBaseScene()
{
}

GameBaseScene::~GameBaseScene()
{
}

void GameBaseScene::onExit()
{
	TestScene::onExit();
}

bool GameBaseScene::init()
{
	return true;
}

void GameBaseScene::update(float dt)
{
	
}

#include "GameBaseSceneGUI.hpp"
//#ifdef ENABLE_CHEAT
//	#include "GameBaseSceneCheat.hpp"
//#endif