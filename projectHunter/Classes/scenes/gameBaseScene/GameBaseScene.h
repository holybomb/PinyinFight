#ifndef __GAME_BASE_SCENE_H__
#define __GAME_BASE_SCENE_H__

#include "../../testBasic.h"

class GameBaseScene : public TestScene
{
public:
	GameBaseScene();
	virtual ~GameBaseScene();
	virtual bool init();
	void update(float dt);
	virtual void onExit();
#include "GameBaseSceneGUI.h"
// #ifdef ENABLE_CHEAT
// 	#include "GameBaseSceneCheat.h"
// #endif
};

#endif // __GAME_BASE_SCENE_H__

