
///-------------------------------------------------------------
// FirstSceneHUD.h
// UI界面相关
///-------------------------------------------------------------

enum 
{
	kTagGUIGamePan = 100500,		// logo
};

void MainGameScene::openGamePan()
{
	if (getGamePanUI())
	{
		GUIBase* pGUIBase = dynamic_cast<GUIBase*>(getChildByTag(kTagGUIGamePan));
		pGUIBase->doClose();
	}
	else
	{
		GamePan* pSceneUI = GamePan::create();
		pSceneUI->loadCCB(this, "scene/gamepan.ccbi");
		addChild(pSceneUI, kOrderGUINormal, kTagGUIGamePan);
		pSceneUI->doOpen();
	}
}

CCNode* MainGameScene::getGamePanUI()
{
	CCNode* pNode = getChildByTag(kTagGUIGamePan);
	if (pNode)
	{
		GamePan* pModel = dynamic_cast<GamePan*>(pNode);
		return pModel;
	}
	return NULL;
}
