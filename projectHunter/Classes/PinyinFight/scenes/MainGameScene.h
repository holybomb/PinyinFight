#ifndef __MAINGAMESCENE_H__
#define __MAINGAMESCENE_H__

#include "../../scenes/gameBaseScene/GameBaseScene.h"

class MainGameScene : public GameBaseScene
{
public:
	MainGameScene();
	virtual ~MainGameScene();
	CREATE_SCENE_FUNC(MainGameScene);
    virtual void prepareThisScene();

	virtual void onEnter();

#include "MainGameSceneGUI.h"
};

#endif // __MAINGAMESCENE_H__
