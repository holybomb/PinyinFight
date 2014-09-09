///-------------------------------------------------------------
// GameBaseSceneCMD.hpp
// 公共网络部分相关
///-------------------------------------------------------------

#include "../../homeScene/cmd_cache/CMDCacheManager.h"
#include "../../wrapper/wrapper.h"
#include "../../Actor/Hero.h"
#include "../../Database/DBManager.h"
#include "../../homeScene/HomeScene.h"
#include "../../GUI/home_scene/SceneUI.h"
#include "../../GUI/home_scene/chat/Chat.h"
#include "../../GUI/home_scene/scene_ui/Friend.h"

#define DEBUG_LOG true

void GameBaseScene::initCMD()
{
}

void GameBaseScene::destroyCMD()
{
}

void GameBaseScene::sendCMD()
{
}

bool GameBaseScene::processCMD(BaseDownCMD* pCmd)
{
	bool process = true;
	switch (pCmd->getCMDID())
	{
		CASE_CMD(CMD_LoginToServer)
		CASE_CMD(CMD_Ping)
		CASE_CMD(CMD_PushCurMoney)
		CASE_CMD(CMD_PushCurExp)
		CASE_CMD(CMD_ShowLevelUp)
		CASE_CMD(CMD_ShowMessage)
		CASE_CMD(CMD_TOWN_OppoPlayerSay)
		CASE_CMD(CMD_TOWN_InquiryPlayer)
		CASE_CMD(CMD_FRIEND_reFlash)

	default:
		process = false;
		break;
	}
	return process;
}

void GameBaseScene::processCacheCMD(BaseDownCMD* pCmd)
{
}

void GameBaseScene::CMD_LoginToServerCallback(CCObject* pSender)
{
	CMD_LoginToServer_down* pCmd = (CMD_LoginToServer_down*)pSender;
	if (DEBUG_LOG) CCLOG("GameBaseScene ||| receive CMD_LoginToServer /////isOK is %d", pCmd->isOK);
	if (pCmd->isOK == CMD_LoginToServer_down::CMD_LoginToServer_LoginFailed)
	{
		// 重登陆失败
		openNetErrorScene();
	}
}

// 计算客户端ping值
void GameBaseScene::CMD_PingCallback(CCObject* pSender)
{
// 	CMD_Ping_down* pCmd = (CMD_Ping_down*)pSender;
// 	if (DEBUG_LOG) CCLOG("GameBaseSceneCMD ||| receive CMD_Ping ///// ping is %d", pCmd->ping);
// 
// 	m_Ping = pCmd->ping;
// 
// 	CmdSendManager::instance()->send_CMD_Ping(pCmd->time);
// 	if(m_hero)
// 	{
// 		m_hero->m_HeroPing = m_Ping;
// 	}
}

///-------------------------------------------------------------

void GameBaseScene::CMD_PushCurMoneyCallback(CCObject* pSender)
{
	CMD_PushCurMoney_down* pCmd = (CMD_PushCurMoney_down*)pSender;

	if (DEBUG_LOG) CCLOG("GameBaseSceneCMD ||| receive CMD_PushCurMoney ///// ServerMoney is %d, RewardMoney is %d", pCmd->ServerMoney, pCmd->RewardMoney);
	// refresh UI
	HomeScene* pHomeScene = dynamic_cast<HomeScene*>(this);
	if (pHomeScene)
	{
		m_hero->setMoney(pCmd);
		pHomeScene->refreshUI(pSender);
	}
}

void GameBaseScene::CMD_PushCurExpCallback(CCObject* pSender)
{
	CMD_PushCurExp_down* pCmd = (CMD_PushCurExp_down*)pSender;

	if (DEBUG_LOG) CCLOG("GameBaseSceneCMD ||| receive CMD_PushCurExp ///// ServerExp is %d, RewardExp is %d", pCmd->ServerExp, pCmd->RewardExp);

	m_hero->getPlayerInfo()->Experience = pCmd->ServerExp;
}

///-------------------------------------------------------------

void GameBaseScene::CMD_ShowLevelUpCallback(CCObject* pSender)
{
	if (DEBUG_LOG) CCLOG("GameBaseSceneCMD ||| receive CMD_ShowLevelUp");
	GameScene* game =dynamic_cast<GameScene*>(CCDirector::sharedDirector()->getRunningScene());
	if(game&& game->getStatistics())
		return;
	openLevelUp();
}

///-------------------------------------------------------------

void GameBaseScene::CMD_TOWN_ServerSayCallback(CCObject* pSender)
{
	CMD_TOWN_ServerSay_down* pCmd = (CMD_TOWN_ServerSay_down*)pSender;
	if (DEBUG_LOG) CCLOG("GameBaseSceneCMD ||| receive CMD_TOWN_ServerSay ///// %s", pCmd->ServerSay.c_str());
	// TODO
}

void GameBaseScene::CMD_ShowMessageCallback(CCObject* pSender)
{
	string str;
	CMD_ShowMessage_down* pCmd = (CMD_ShowMessage_down*)pSender;
	if (DEBUG_LOG) CCLOG("GameBaseSceneCMD ||| receive CMD_ShowMessage ///// %d", pCmd->msgID);
	if (pCmd->msgID == 0)
	{
		// 自定义类型
		str = pCmd->CustomMsg;
	}
	else
	{
		XTipMsg* pModel = DBManager::sharedDBManager()->getTipMsg(pCmd->msgID);
		if (pModel)
		{
			pCmd->msgType = pModel->Stype;
			str = pModel->TipMsg;
		}
		else
		{
			pCmd->msgType = CMD_ShowMessage_down::CMD_ShowMessage_TYPE_DIALOG;
			str = "XTipMsg err";
		}
	}

	switch (pCmd->msgType)
	{
	case CMD_ShowMessage_down::CMD_ShowMessage_TYPE_TIP_SYSTEM:
		openTipSystem(str);
		break;


	case CMD_ShowMessage_down::CMD_ShowMessage_TYPE_DIALOG:
		{
			// 系统提示弹版
			ConfirmDialog* pDialog = ConfirmDialog::create();
			pDialog->loadCCB(this, "scene/dialog1.ccbi");
			pDialog->setTipText(str.c_str());
			addChild(pDialog, kOrderGUIHigh);
			pDialog->doOpen();
		}

		break;

	case CMD_ShowMessage_down::CMD_ShowMessage_TYPE_TIP_GAME:
		openTipGame(str);
		break;

	default:
		break;
	}
}

///-------------------------------------------------------------

void GameBaseScene::CMD_TOWN_OppoPlayerSayCallback(CCObject* pSender)
{
	CMD_TOWN_OppoPlayerSay_down* pCmd = (CMD_TOWN_OppoPlayerSay_down*)pSender;
	if (DEBUG_LOG) CCLOG("GameBaseSceneCMD ||| receive CMD_TOWN_OppoPlayerSay ///// %d", pCmd->count);
	Chat::putList(pCmd);

	// refresh UI
	HomeScene* pHomeScene = dynamic_cast<HomeScene*>(this);
	if (pHomeScene)
	{
		SceneUI* su = pHomeScene->getSceneUI();
		if (su)
		{
			su->refreshChatTip();
			su->refreshChat();
		}
	}
}

void GameBaseScene::CMD_TOWN_InquiryPlayerCallback(CCObject* pSender)
{
	CMD_TOWN_InquiryPlayer_down* pCmd = (CMD_TOWN_InquiryPlayer_down*)pSender;
	if (DEBUG_LOG) CCLOG("HomeSceneCMD ||| receive CMD_TOWN_InquiryPlayer ///// %d", pCmd->stateType);
	
	// refresh UI
	switch (pCmd->stateType)
	{
	case CMD_TOWN_InquiryPlayer_down::CMD_TOWN_InquiryPlayer_Player_STATE_NONE:
		Utils::openTip(TIP_CHAT_PLAYER_NO);
		break;
	case CMD_TOWN_InquiryPlayer_down::CMD_TOWN_InquiryPlayer_Player_STATE_OFFLINE:
		Utils::openTip(TIP_CHAT_PLAYER_OFFLINE);
		break;
	case CMD_TOWN_InquiryPlayer_down::CMD_TOWN_InquiryPlayer_Player_STATE_ONLINE:
		break;
	case CMD_TOWN_InquiryPlayer_down::CMD_TOWN_InquiryPlayer_Player_STATE_BLACK:
		Utils::openTip(TIP_CHAT_PLAYER_BLACK);
		break;
	default:
		break;
	}
	HomeScene* pHomeScene = dynamic_cast<HomeScene*>(this);
	if (pHomeScene)
	{
		Chat* pChat = pHomeScene->getSceneUI()->getChat();
		if (pChat)
		{
			pChat->setPrivateName(pCmd->stateType == CMD_TOWN_InquiryPlayer_down::CMD_TOWN_InquiryPlayer_Player_STATE_ONLINE);
		}
	}
}
///-------------------------------------------------------------

///-----------------------------start 好友--------------------------------
/**
 * 调用：监听
 * 功能：刷新好友列表
 */
void GameBaseScene::CMD_FRIEND_reFlashCallback(CCObject* pSender)
{
	CMD_FRIEND_reFlash_down* pCmd = (CMD_FRIEND_reFlash_down*)pSender;
	if (DEBUG_LOG) CCLOG("GameBaseSceneCMD ||| receive CMD_FRIEND_reFlashCallback ///// %d", pCmd->dType);

	Friend::refreshList(pCmd);

	// refresh UI
	HomeScene* pHomeScene = dynamic_cast<HomeScene*>(this);
	if (pHomeScene)
	{
		SceneUI* pSceneUI = pHomeScene->getSceneUI();
		if (pSceneUI)
		{
			Friend* pNode = pSceneUI->getFriend();
			if (pNode)
			{
				pNode->refreshListUI();
			}
		}
	}
}

///-----------------------------end 好友--------------------------------