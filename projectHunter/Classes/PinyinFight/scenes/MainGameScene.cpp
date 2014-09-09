#include "MainGameScene.h"
#include "../../utils/Utils.h"
#include "../layer/GamePan.h"
USING_NS_CC;
using namespace std;

MainGameScene::MainGameScene()
{
}

MainGameScene::~MainGameScene()
{
	CCLOG("~MainGameScene");
}
void MainGameScene::prepareThisScene()
{

	//runThisTest();//Ê¹ÓÃ´øÍøÂç
}

void MainGameScene::onEnter()
{
	GameBaseScene::onEnter();
	openGamePan();
}

#include "MainGameSceneGUI.hpp"