///-------------------------------------------------------------
// GameBaseSceneGUI.h
// UI界面相关
///-------------------------------------------------------------

#include "../../GUI/base/GUIConfig.h"
#include "../../GUI/public/TipGame.h"

enum 
{
	kTagGUIBase	= 100000,			// Base
	kTagGUITipGame,					// 游戏提示
};

void GameBaseScene::loadGUI()
{
}



void GameBaseScene::openTipGame(const string& str)
{
	if (getChildByTag(kTagGUITipGame))
	{
		TipGame* pGUIBase = dynamic_cast<TipGame*>(getChildByTag(kTagGUITipGame));
		pGUIBase->putTip(str);
	}
	else
	{
		TipGame* pGUIBase = TipGame::create();
		pGUIBase->loadCCB(this, "scene/tishi_youxi.ccbi");
		addChild(pGUIBase, kOrderGUIHigh+100000, kTagGUITipGame);
		pGUIBase->doOpen(str);
	}
}
