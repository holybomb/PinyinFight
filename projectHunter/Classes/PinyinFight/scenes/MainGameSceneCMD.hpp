///-------------------------------------------------------------
// FirstSenceCMD.hpp
// 网络部分相关
///-------------------------------------------------------------

#include "../../Online/CmdSendManager.h"
#include "../../Online/BaseDownCMD.h"
#include "../../Online/CMD/CmdMaker.h"

#define DEBUG_LOG true

void MainGameScene::sendCMD()
{
	GameBaseScene::sendCMD();
}

bool MainGameScene::processCMD(BaseDownCMD* pCmd)
{
	bool process = true;
	switch (pCmd->getCMDID())
	{
		CASE_CMD(CMD_Http_RequestVersion);

	default:
		process = GameBaseScene::processCMD(pCmd);
		break;
	}
	return process;
}

void MainGameScene::CMD_Http_RequestVersionCallback(CCObject* pSender)
{
	CMD_Http_RequestVersion_down* pCmd = (CMD_Http_RequestVersion_down*)pSender;
	/*
	AssetsUpdate* model = dynamic_cast<AssetsUpdate*>(getAssetsUpdate());
	if (model)
	{
	model->updateCmd(pCmd);
	}*/
}