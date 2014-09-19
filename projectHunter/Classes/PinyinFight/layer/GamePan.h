#include "../../GUI/base/GUIBase.h"
#include "Database/base/DB_Actor.h"
#ifndef __GAMEPAN_H__
#define __GAMEPAN_H__


/**
 * GamePanΩÁ√Ê
 */
class GamePan : public GUIBase
{
public:
	GamePan(void);
	virtual ~GamePan(void);

	CREATE_FUNC(GamePan);

	virtual void doOpen();
	void refreshWorldPad();
	void initWordList();

	void wordSelectCallbackCCControl(CCObject * pSender, cocos2d::extension::CCControlEvent pCCControlEvent);
	void refreshLetter();
	void update(float delta);
	void updateNpc(float delta);
	void showHideConfirmPad(bool isShow);
	void ConfirmBtnCallback(CCObject* pSender);
	void Attack(int direct);
	void nextPuzzle();

	void createNextPuzzle();
	void updateTimer(float delta);
	void resetGamePan();
	bool checkAnswer(std::string pAnswer = "");
	void checkWhoWin();
	void changeSide();
	void showResult();
	void hideResult();
	void ResultBtnCallBack(CCObject* pSender);

	void addNpc(DB_Actor* pActor);
	void addPlayer(DB_Actor* pActor);
	bool _isPlayer;
	static std::string _answerStr;
private:
	float _curTime;
	CCArray* _wordList;
	std::string _pinyinStr;
	std::string  _targetStr;
	std::string answer;
	bool isCheckWin;
private:
};

#endif // __GAMEPAN_H__