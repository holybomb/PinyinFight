#ifndef __GUI_BASE_H__
#define __GUI_BASE_H__

#include "CcbBase.h"

enum
{
	kOrderGUILow = 100000,
	kOrderGUINormal,
	kOrderGUIHigh,
};

enum
{
	kGUIPriorityHighest = cocos2d::kCCMenuHandlerPriority - 10000,
	kGUIPriorityHigh = cocos2d::kCCMenuHandlerPriority - 300,
	kGUIPriorityNormal = cocos2d::kCCMenuHandlerPriority - 200,
	kGUIPriorityLow = cocos2d::kCCMenuHandlerPriority - 100,
	kGUIBasePriority = cocos2d::kCCMenuHandlerPriority - 1,
	kGUIPriorityDefault = cocos2d::kCCMenuHandlerPriority + 1,
};

enum GUIScreenType
{
	kGUIScreenFull,
	kGUIScreenLeft,
	kGUIScreenRight,
};

/**
 * GUI base
 */
class BaseDownCMD;
class GUIBase : public CcbBase
{
    CC_SYNTHESIZE(bool, m_bFocus, Focus);
	CC_SYNTHESIZE(bool, m_bEnabled, Enabled);
	CC_SYNTHESIZE(bool, m_bLoop, Loop);
	CC_SYNTHESIZE(bool, m_bAutoClose, AutoClose);

public:
	// base node tag
	static const int TAG_BASE_NODE = 0;
		// close btn
		static const int TAG_CLOSE = 0;
		static const int TAG_BG = 1;

public:
    GUIBase(void);
    virtual ~GUIBase(void);

	CREATE_FUNC(GUIBase);

    virtual void onEnter();
    virtual void cleanup(void);
    
	virtual void setTouchPriority(int priority);
    
    /**
     * load ccb node
     */
    void loadCCB(CCNode* pParent, const char* pCCBFileName);
    
    virtual bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    virtual void ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    virtual void ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);

	virtual bool isTouchInside(cocos2d::CCTouch* touch);

	virtual void closeButtonCallback(CCObject* pSender);

	virtual void doOpen();
	virtual void doOpen(BaseDownCMD* pCmd);
	virtual void doOpen(BaseDownCMD* pCmd, int index);
	virtual void doClose();
	virtual void doClean();

    virtual void processCMD(BaseDownCMD* pCmd, int index); // ��ʼ��������Ϣ
	virtual void updateCMD(BaseDownCMD* pCmd, int index);  // ������Ϣ

	void setTarget(CCObject* target, cocos2d::SEL_MenuHandler selector);

	void setBlack(bool isBlack);
	bool getBlack();
	virtual void keyBackClicked();//Android ���ؼ�

	void setLoading(bool loading);

protected:
	virtual void onFocus(bool noCreateBlack);		// ��ý���
	virtual void offFocus(bool noCreateBlack);		// ʧȥ����

	inline void setScreenType(GUIScreenType type)
	{
		_GUIScreenType = type;
	}

public:
	inline GUIScreenType getScreenType()
	{
		return _GUIScreenType;
	}

	void setNoBlack(bool noBlack = true);
	inline void setNoCreateBlack() { _noCreateBlack = true; };

private:
	void resetPriority(CCNode* pNode, int priority);
	void resetTouch(CCNode* pNode, bool enabled);
	void exitCallback(CCObject* pSender);
protected:
	bool _hasProcess;		// ����ظ�������Ӧ����

private:
	GUIScreenType _GUIScreenType;

	CCObject* m_pListener;
	cocos2d::SEL_MenuHandler m_pfnSelector;
	int _Counter; // ������
	bool _noBlack;			// �Լ��޺���
	bool _noCreateBlack;	// ����ɺ���
	bool _isOpenExitDialog;
	GUIBase* pExitConfirm;
};

#endif // __GUI_BASE_H__