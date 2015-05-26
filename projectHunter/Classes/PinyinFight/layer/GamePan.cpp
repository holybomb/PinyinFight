#include "GamePan.h"
#include "../../utils/Utils.h"
#include "../../Database/DBManager.h"
#include "../../utils/TextManager.h"
#include "../../utils/textConvert.h"
#include "../../shaderProgram/shaderNode.h"
#include "../../GUI/base/check_box/XCheckBox.h"
#include "../scenes/MainGameScene.h"
#include "../actor/Npc.h"
#include "../actor/Player.h"

USING_NS_CC;

#define DEBUG_LOG false
#define ITEM_SPACE 6
#define MAX_LINE 3
#define MAX_ROW 8
#define SELECT_MAX 4

#define MOVE_STEP 38.8f
#define MAX_DISTANCE MOVE_STEP*4

#define TIME_OUT 60
#define NPC_ATTACK 0
#define PLAYER_ATTACK 1
enum
{
	kTagGamePanField = 1,
	kTagGamePanPublic = 2,
	kTagGamePanOpp,
	kTagGamePanSelf,
	kTagGamePanWordPan,
	kTagGamePanConfirmPad,
	kTagGamePanResultPad,
	kTagGamePanBg,
};
	enum
	{
		kTagGamePanBgBarL,
		kTagGamePanBgBarM,
		kTagGamePanBgBarR,
		kTagGamePanBgTimer,
	};
	enum
	{
		kTagGamePanResultPadText,
		kTagGamePanResultPadMenu,
	};
	enum
	{
		kTagGamePanConfirmPadOkBtn,
		kTagGamePanConfirmPadCancelBtn,
	};
	enum
	{
		kTagGamePanFieldLeft = 1,
		kTagGamePanFieldMiddle,
		kTagGamePanFieldRight,
	};
	enum 
	{
		kTagGamePanWordPanTabBase,
		kTagGamePanWordPanItemBase,
	};
		enum 
		{
			kTagGamePanWordPanTabBaseItem,
		};
		enum 
		{
			kTagGamePanWordPanItemTitle,
		};
std::string GamePan::_answerStr = "";
GamePan::GamePan(void)
{
	setFocus(true);
	cc_timeval psv; 
	CCTime::gettimeofdayCocos2d( &psv, NULL );    // 计算时间种子 
	unsigned int tsrans = psv.tv_sec * 1000 + psv.tv_usec / 1000;    // 初始化随机数  
	srand( tsrans );
}

GamePan::~GamePan(void)
{
}

void GamePan::doOpen()
{
	GUIBase::doOpen();
	isCheckWin=false;
	_isPlayer = true;
	addNpc(DBManager::sharedDBManager()->getActor(1));
	addPlayer(DBManager::sharedDBManager()->getActor(2));
	initWordList();
	refreshWorldPad();
	showHideConfirmPad(false);
	_curTime = TIME_OUT;

	CCNode* pBaseNode = m_pCcbNode->getChildByTag(kTagGamePanBg);
	CCLabelBMFont* timerTxt = dynamic_cast<CCLabelBMFont*>(pBaseNode->getChildByTag(kTagGamePanBgTimer));
	int timeStr = (int)_curTime;
	timerTxt->setString(Utils::itos(timeStr).c_str());

	changeSide();
}
void GamePan::refreshLetter()
{
	vector<string> arr = Utils::split(_pinyinStr);
	string pubLetter = arr.at(0);
	CCNode* pBaseNode = m_pCcbNode->getChildByTag(kTagGamePanPublic);
	CCSprite* pItem = dynamic_cast<CCSprite*>(pBaseNode->getChildByTag(0)->getChildByTag(0));
	pItem->initWithSpriteFrameName((pubLetter+".png").c_str());
	pBaseNode = m_pCcbNode->getChildByTag(kTagGamePanField);
	for(int i =kTagGamePanFieldLeft;i<=kTagGamePanFieldRight;i++)
	{
		string pubLetter = arr.at(i);
		CCSprite* pItem = dynamic_cast<CCSprite*>(pBaseNode->getChildByTag(i)->getChildByTag(0));
		CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName((pubLetter+".png").c_str());
		if(frame)
			pItem->initWithSpriteFrame(frame);
		else
		{
			//CClog((pubLetter+".png no found").c_str());
			pItem->setVisible(false);
		}
	}
}

void GamePan::refreshWorldPad()
{
	CCNode* pBaseNode = m_pCcbNode->getChildByTag(kTagGamePanWordPan);
	CCNode* tabNode = pBaseNode->getChildByTag(kTagGamePanWordPanTabBase);
	CCNode* itemNode = pBaseNode->getChildByTag(kTagGamePanWordPanItemBase);
	CCLabelTTF* itemTemp = dynamic_cast<CCLabelTTF*>(itemNode->getChildByTag(kTagGamePanWordPanItemTitle));
	CCControlButton* tabItem = dynamic_cast<CCControlButton*>(tabNode->getChildByTag(kTagGamePanWordPanTabBaseItem));
	float itemWidth = tabItem->getContentSize().width+ITEM_SPACE;
	float itemHeight = tabItem->getContentSize().height+ITEM_SPACE;
	for(int i =0;i<MAX_LINE;i++)
	{
		for(int j =0;j<MAX_ROW;j++)
		{
			CCControlButton* item = CCControlButton::create();
			Utils::copyCCControlButton(item,tabItem);
			item->setPosition(ccp(tabItem->getPositionX()+j*itemWidth,tabItem->getPositionY()-i*itemHeight));
			item->setTag(j+i*MAX_ROW);
			tabNode->addChild(item);

			CCLabelTTF* title = Utils::copyCCLabelTTF(itemTemp);
			title->setPosition(item->getPosition());
			title->setTag(j+i*MAX_ROW);
			itemNode->addChild(title);
			CCString* str = (CCString*)_wordList->randomObject();
			title->setString(str->getCString());
			_wordList->removeObject(str);
		}
	}

	for(int i =0;i<MAX_LINE;i++)
	{
		for(int j =0;j<MAX_ROW;j++)
		{
			CCControlButton* item = dynamic_cast<CCControlButton*>(tabNode->getChildByTag(j+i*MAX_ROW));
			XCheckBox* pCheckBox = XCheckBox::create(item);
			pCheckBox->setTag(item->getTag());
			pCheckBox->setToggle(false);
			pCheckBox->setTarget(this, cccontrol_selector(GamePan::wordSelectCallbackCCControl));
			tabNode->addChild(pCheckBox);
			item->removeFromParent();
		}
	}
	itemTemp->removeFromParent();
	tabItem->removeFromParent();
	refreshLetter();
}
void GamePan::resetGamePan()
{
	CCNode* pBaseNode = m_pCcbNode->getChildByTag(kTagGamePanWordPan);
	CCNode* tabNode = pBaseNode->getChildByTag(kTagGamePanWordPanTabBase);
	CCNode* itemNode = pBaseNode->getChildByTag(kTagGamePanWordPanItemBase);
	for(int i =0;i<MAX_LINE;i++)
	{
		for(int j =0;j<MAX_ROW;j++)
		{
			XCheckBox* pCheckBox = dynamic_cast<XCheckBox*>(tabNode->getChildByTag(j+i*MAX_ROW));
			if(pCheckBox)
				pCheckBox->setToggle(false);

			CCLabelTTF* title = dynamic_cast<CCLabelTTF*>(itemNode->getChildByTag(j+i*MAX_ROW));
			if(title)
			{
				CCString* str = (CCString*)_wordList->randomObject();
				title->setString(str->getCString());
				_wordList->removeObject(str);
			}
		}
	}
	refreshLetter();
}
void GamePan::initWordList()
{
	_wordList = CCArray::create();
	_wordList->retain();
	//if(_pinyinStr.size()>0)
	//	_targetStr = DBManager::sharedDBManager()->getRandomWordByLetter(_pinyinStr,_targetStr);
	//else
		_targetStr = DBManager::sharedDBManager()->getRandomWord();//GET_STRING(TEXT_JUQING_SPLASH_BEGIN2);
	//CCLog("Target is %s",_targetStr.c_str());
	
	std::string wordList = _targetStr;//DBManager::sharedDBManager()->getRandomWord();//GET_STRING(TEXT_JUQING_SPLASH_BEGIN2);
	int maxWord = (MAX_LINE*MAX_ROW)/4+((MAX_LINE*MAX_ROW)%4 ==0 ? 0:1);
	for (int i =1;i<maxWord;i++)
	{
		string temp = DBManager::sharedDBManager()->getRandomWord();//GET_STRING(50+i);
		if(temp.compare(_targetStr) == 0)
		{
			i--;
		}
		else
		{
			wordList += temp;
		}
	}
	int length = MAX_LINE*MAX_ROW;//wordList->count();
	int srcLength = wordList.size();
	CCLog("%s",wordList.c_str());
	if(_pinyinStr.empty())
	{
		std::string pinyinStr = Utils::getPinyinStr(wordList);//GET_STRING(TEXT_JUQING_SPLASH_BEGIN2);
		vector<string> arrTotal= Utils::split(Utils::getPinyinLetter(pinyinStr));
		vector<string>::iterator iter;
		vector<string> arr;
		for (iter = arrTotal.begin(); iter != arrTotal.end(); iter++)
		{
			if((*iter).compare("ch") == 0
				|| (*iter).compare("zh") == 0
				|| (*iter).compare("sh") == 0)
			{
			}
			else
			{
				arr.push_back((*iter));
			}
		}
		string doublePinyin[] ={"ch","sh","zh"}; 
		string singlePinyin[] ={"b","c","d","f","g","h","j","k","l","m","n","p","q","r","s","t","w","x","y","z"}; 
		_pinyinStr = doublePinyin[(int)(CCRANDOM_0_1()*2)];
		for(int i =0;i<3;i++)
		{
			_pinyinStr+= ","+arr.at((int)(CCRANDOM_0_1()*arr.size()));
		}
		CCLog("Target is %s,%s,%s",_targetStr.c_str(),_pinyinStr.c_str(),pinyinStr.c_str());
	}
	for(int i = 0;i<srcLength;i=i+3)
	{
		CCString* c = CCString::create(wordList.substr(i,3));
		c->retain();
		_wordList->addObject(c);
	}
}
void GamePan::wordSelectCallbackCCControl(CCObject * pSender, CCControlEvent pCCControlEvent)
{
	MainGameScene* _scene = dynamic_cast<MainGameScene*>(CCDirector::sharedDirector()->getRunningScene());
	XCheckBox* pCheckBox = dynamic_cast<XCheckBox*>(pSender);
	GamePan* pNode = dynamic_cast<GamePan*>(_scene->getGamePanUI());
	if(!pNode->_isPlayer)
	{
		if(pCheckBox->getToggle())
		{	
			pCheckBox->setToggle(false);
			return;
		}
	}
	vector<string> arr = Utils::split(_answerStr);
	if(arr.size()>=SELECT_MAX)
	{
		if(pCheckBox->getToggle())
		{	
			pCheckBox->setToggle(false);
			return;
		}
	}
	int tag = pCheckBox->getTag();
	if(pCheckBox->getToggle())
	{
		if(_answerStr.size()>0)
			_answerStr+=",";
		_answerStr += Utils::itos(tag);	
	}
	else
	{
		vector<string> arr = Utils::split(_answerStr);
		_answerStr = "";
		for(int i = 0 ;i <arr.size();i++)
		{
			int id = atoi(arr.at(i).c_str());
			if(id == tag)
			{
				continue;;
			}
			else
			{
				if(_answerStr.size()>0)
					_answerStr+=",";
				_answerStr += Utils::itos(id);	
			}
		}
	}
	arr = Utils::split(_answerStr);
	if(_scene)
	{
		
		if(arr.size()>=SELECT_MAX)
		{
			pNode->nextPuzzle();
			//pNode->showHideConfirmPad(true);
		}
		//pNode->update(0);
	}
}

void GamePan::update(float delta)
{
	CCNode* pBaseNode = m_pCcbNode->getChildByTag(kTagGamePanWordPan);
	CCNode* tabNode = pBaseNode->getChildByTag(kTagGamePanWordPanTabBase);
	CCNode* itemNode = pBaseNode->getChildByTag(kTagGamePanWordPanItemBase);
	vector<string> arr = Utils::split(_answerStr);
	
	string answer;
	for(int i = 0 ;i <arr.size();i++)
	{
		int id = atoi((arr.at(i).c_str()));
		CCLabelTTF* text = dynamic_cast<CCLabelTTF*>(itemNode->getChildByTag(id));
		answer += text->getString();
	}
	//CCLog(("###############   "+answer).c_str());
}
void GamePan::ConfirmBtnCallback(CCObject* pSender)
{
	int tag = ((CCNode*)pSender)->getTag();
	switch (tag)
	{
	case kTagGamePanConfirmPadOkBtn:
		{
			nextPuzzle();

		}
		break;
	case kTagGamePanConfirmPadCancelBtn:
		showHideConfirmPad(false);
		break;
	}
}
void GamePan::showHideConfirmPad(bool isShow)
{
	CCLayerColor* pBaseNode = dynamic_cast<CCLayerColor*>(m_pCcbNode->getChildByTag(kTagGamePanConfirmPad));
	pBaseNode->setVisible(isShow);
	CCLabelTTF* pText = dynamic_cast<CCLabelTTF*>(pBaseNode->getChildByTag(1));
	pText->setVisible(!_isPlayer);
	CCMenu* pMenu = dynamic_cast<CCMenu*>(pBaseNode->getChildByTag(0));
	pMenu->setVisible(isShow && _isPlayer);
	pMenu->setTouchPriority(-1000);
	if(pMenu)
	{
		CCMenuItemImage* btn = dynamic_cast<CCMenuItemImage*>(pMenu->getChildByTag(kTagGamePanConfirmPadOkBtn));
		btn->setTarget(this,menu_selector(GamePan::ConfirmBtnCallback));
		btn = dynamic_cast<CCMenuItemImage*>(pMenu->getChildByTag(kTagGamePanConfirmPadCancelBtn));
		btn->setTarget(this,menu_selector(GamePan::ConfirmBtnCallback));
	}
}
bool GamePan::checkAnswer(std::string pAnswer)
{
	if(pAnswer.empty())
	{
		CCNode* pBaseNode = m_pCcbNode->getChildByTag(kTagGamePanWordPan);
		CCNode* itemNode = pBaseNode->getChildByTag(kTagGamePanWordPanItemBase);
		vector<string> arr = Utils::split(_answerStr);

		answer = "";
		for(int i = 0 ;i <arr.size();i++)
		{
			int id = atoi((arr.at(i).c_str()));
			CCLabelTTF* text = dynamic_cast<CCLabelTTF*>(itemNode->getChildByTag(id));
			answer += text->getString();
		}
	}
	else
	{
		answer = pAnswer;
	}
	string dbResult = DBManager::sharedDBManager()->checkWordExist(answer);
	if(!dbResult.empty())
	{
		MainGameScene* scene = dynamic_cast<MainGameScene*>(CCDirector::sharedDirector()->getRunningScene());
		if(scene)
			scene->openTipGame(dbResult+Utils::getPinyinLetter(Utils::getPinyinStr(dbResult)));
		return true;
	}
	else
		return false;
	
}
void GamePan::nextPuzzle()
{
	if(checkAnswer())
	{
		Attack(_isPlayer?PLAYER_ATTACK:NPC_ATTACK);
	}
	else
	{
		Utils::openTip(TIP_SELECT_WRONG);
	}
	showHideConfirmPad(false);
}
void GamePan::checkWhoWin()
{
	if(isCheckWin)
		return;
	if(!isCheckWin)
		isCheckWin = true;
	CCNode* fieldNode = m_pCcbNode->getChildByTag(kTagGamePanField);
	bool isWin = false;
	for(int i =kTagGamePanFieldLeft ;i<=kTagGamePanFieldRight;i++)
	{
		CCNode* bar = fieldNode->getChildByTag(i);
		float dis = bar->getPositionY();
		if(dis>=MAX_DISTANCE||dis<= -MAX_DISTANCE)
			isWin = true;
		else
		{
			isWin = false;
			break;
		}
	}
	if(isWin)
	{
		showResult();
	}
	else
	{
		createNextPuzzle();
		_isPlayer = !_isPlayer;
		changeSide();
	}
}

void GamePan::hideResult()
{
	CCNode* pBaseNode = m_pCcbNode->getChildByTag(kTagGamePanResultPad);
	pBaseNode->setVisible(false);
}
void GamePan::showResult()
{
	CCNode* pBaseNode = m_pCcbNode->getChildByTag(kTagGamePanResultPad);
	pBaseNode->setVisible(true);
	CCLabelTTF* title = dynamic_cast<CCLabelTTF*>(pBaseNode->getChildByTag(kTagGamePanResultPadText));
	if(_isPlayer)
		title->setString("PLAYER WIN");
	else
		title->setString("NPC WIN");
	CCNode* pMenu = pBaseNode->getChildByTag(kTagGamePanResultPadMenu);
	if(pMenu)
	{
		CCMenuItemImage* btn = dynamic_cast<CCMenuItemImage*>(pMenu->getChildByTag(0));
		btn->setTarget(this,menu_selector(GamePan::ResultBtnCallBack));
	}
}
void GamePan::ResultBtnCallBack(CCObject* pSender)
{
	_isPlayer = true;
	_pinyinStr = "";
	_answerStr = "";
	createNextPuzzle();
	changeSide();
	hideResult();
	CCNode* fieldNode = m_pCcbNode->getChildByTag(kTagGamePanField);
	for(int i =kTagGamePanFieldLeft ;i<=kTagGamePanFieldRight;i++)
	{
		CCNode* bar = fieldNode->getChildByTag(i);
		bar->setPositionY(0);
	}
}
void GamePan::changeSide()
{
	_curTime = TIME_OUT;
	if(_isPlayer)
	{
		showHideConfirmPad(false);
		unschedule(schedule_selector(GamePan::updateNpc));
	}
	else
	{
		showHideConfirmPad(true);
		schedule(schedule_selector(GamePan::updateNpc),2.0f*(1+CCRANDOM_0_1()*3.0f));
	}
	schedule(schedule_selector(GamePan::updateTimer),1.0f);
}
void GamePan::updateNpc(float delta)
{
	string temp = DBManager::sharedDBManager()->getRandomWord();
	if(checkAnswer(temp))
	{
		Attack(NPC_ATTACK);
	}
	else
	{
		Utils::openTip(TIP_SELECT_WRONG);
	}
	//bool isAnswer = (CCRANDOM_0_1()*1000)>700;
	//if(isAnswer)
	//{
	//	bool isAttack = (CCRANDOM_0_1()*1000)>500;
	//	if(isAttack)
	//		Attack(NPC_ATTACK);
	//	else
	//		Utils::openTip(TIP_SELECT_WRONG);
	//}
}

void GamePan::Attack(int direct)
{
	CCNode* fieldNode = m_pCcbNode->getChildByTag(kTagGamePanField);
	for(int i =0;i<fieldNode->getChildrenCount();i++)
	{
		fieldNode->getChildByTag(i);
	}
	
	std::string pinyinStr = Utils::getPinyinStr(answer);//GET_STRING(TEXT_JUQING_SPLASH_BEGIN2);
	std::string answerPinyin = Utils::getPinyinLetter(pinyinStr);
	vector<string> answerArr = Utils::split(answerPinyin);
	vector<string> pinyinArr = Utils::split(_pinyinStr);
	for(int i =0;i<pinyinArr.size();i++)
	{
		string letter = pinyinArr.at(i);
		int pos = -1;
		for(int j =0;j<answerArr.size();j++)
		{
			string tmp = answerArr.at(j);
			if(tmp.compare(letter) == 0)
			{
				pos = i;
			}
		}
		if(i>0 && pos>0)
		{
			CCNode* bar = fieldNode->getChildByTag(i);
			CCMoveTo* moveTo = NULL;
			switch (direct)
			{
			case NPC_ATTACK:
				{
					moveTo = CCMoveTo::create(0.3f,ccp(bar->getPositionX(),bar->getPositionY()-MOVE_STEP));
				}
				break;
			case PLAYER_ATTACK:
				{
					moveTo = CCMoveTo::create(0.3f,ccp(bar->getPositionX(),bar->getPositionY()+MOVE_STEP));
				}
				break;
			}
			bool isMoveAble = true;
			float dis = bar->getPositionY();
			if(dis>=MAX_DISTANCE)
			{
				isMoveAble = false;
			}
			else if(dis<= -MAX_DISTANCE)
			{
				isMoveAble = false;
			}
			if(isMoveAble)
			{
				bar->runAction(moveTo);
			}
		}
	}
	this->runAction(CCSequence::create(CCDelayTime::create(0.35f),CCCallFunc::create(this,callfunc_selector(GamePan::checkWhoWin)),NULL));
	
}

void GamePan::createNextPuzzle()
{
	_answerStr = "";
	initWordList();
	resetGamePan();
	isCheckWin = false;
}

void GamePan::updateTimer(float delta)
{
	_curTime-=1;
	if(_curTime<0)
	{
		unschedule(schedule_selector(GamePan::updateTimer));
		_isPlayer = !_isPlayer;
		changeSide();
	}
	CCNode* pBaseNode = m_pCcbNode->getChildByTag(kTagGamePanBg);
	CCLabelBMFont* timerTxt = dynamic_cast<CCLabelBMFont*>(pBaseNode->getChildByTag(kTagGamePanBgTimer));
	int timeStr = (int)_curTime;
	timerTxt->setString(Utils::itos(timeStr).c_str());
}

void GamePan::addNpc(DB_Actor* pActor)
{
	CCNode* pBaseNode = m_pCcbNode->getChildByTag(kTagGamePanOpp)->getChildByTag(0);
	pBaseNode->removeAllChildren();
	Npc* npc = Npc::create();
	npc->loadCCB(pBaseNode,("scene/actor/"+pActor->resource).c_str());
	pBaseNode->addChild(npc);
	npc->doOpen();
	npc->setPosition(CCPointZero-(npc->getContentSize()/2));
}

void GamePan::addPlayer(DB_Actor* pActor)
{
	CCNode* pBaseNode = m_pCcbNode->getChildByTag(kTagGamePanSelf)->getChildByTag(0);
	pBaseNode->removeAllChildren();
	Player* player = Player::create();
	player->loadCCB(pBaseNode,("scene/actor/"+pActor->resource).c_str());
	pBaseNode->addChild(player);
	player->doOpen();
	player->setPosition(CCPointZero-(player->getContentSize()/2));
}
