#ifndef __CCS_PLAYER_H__
#define __CCS_PLAYER_H__

#include "cocos2d.h"
#include "cocos-ext.h"

#include "../../shaderProgram/AnimPlayerEffectDefine.h"

enum RectType
{
	kRectTypeCollide,	// 碰撞框
	kRectTypeAttack,	// 攻击框
	kRectTypeBoundBox,	// 全局框
};

struct CCSPlayerPlayPara
{
	std::string animationName;
	int animationIndex;
	int durationTo;
	int durationTween;
	int loop;
	int tweenEasing;
};

class CCSPlayer;
typedef void (cocos2d::CCObject::*SEL_CCS_CB)(CCSPlayer*, const char*, cocos2d::extension::MovementEventType);
#define ccs_cb_selector(_SELECTOR) (SEL_CCS_CB)(&_SELECTOR)

class CCSPlayer : public cocos2d::CCSprite
{
public:
	CCSPlayer();
	~CCSPlayer();

	void reset();

	//CCSPlayer(CCSPlayer *obj);

	/**
    * load ccs node
    */
    static void addArmatureFileInfo(const char* pCCSFileName);
	static void addArmatureFileInfo(const char *imagePath, const char *plistPath, const char *configFilePath);
	static void purge();

	static CCSPlayer* create(const char* pAniName);
	bool initWithAniName(const char* pAniName);

	virtual void draw();

	inline cocos2d::extension::CCArmature* getCCSNode() const { return _ccsNode; };

	inline virtual void play(const char *animationName, int durationTo = -1, int durationTween = -1, int loop = -1, int tweenEasing = cocos2d::extension::TWEEN_EASING_MAX);
	inline virtual void playWithIndex(int animationIndex, int durationTo = -1, int durationTween = -1, int loop = -1, int tweenEasing = cocos2d::extension::TWEEN_EASING_MAX);

	cocos2d::CCRect getRect(RectType rectType, bool isSelf = false);

	void setTarget(CCObject* target, SEL_CCS_CB selector);

	// 动画回调
	void animationEvent(cocos2d::extension::CCArmature *armature, cocos2d::extension::MovementEventType movementType, const char *movementID);
	
	// 重播放
	void replay();
	void pause();
	void resume();

	void setFlipX(bool flipX);
	void setFlipY(bool flipY);
	inline bool isFlipX() const { return m_bFlipX; };
	inline bool isFlipY() const { return m_bFlipY; };

	inline GLubyte getOpacity() { return _opacity; };
	inline void setOpacity(GLubyte opacity);
	float getCurrentFrame();

	inline void setShowRect(bool show) { _showRect = show; };
	inline int GetAnim() { return _CCSPlayerPlayPara.animationIndex;  };
	inline std::string GetAnimName() { return _CCSPlayerPlayPara.animationName; };

	static std::string getCocostudioPath(const char *spriteName);//不能删，主要是ccs load机制的问题

	// 换装
	void changeAvatar(std::string& typeName);
protected:
	cocos2d::extension::CCArmature* _ccsNode;
private:

	CCObject*	m_pListener;
	SEL_CCS_CB	m_pfnSelector;

	CCSPlayerPlayPara _CCSPlayerPlayPara;

	bool m_bFlipX;
	bool m_bFlipY;

	GLubyte _opacity;

	bool _showRect;

	void beforeDraw();
	void afterDraw();
	void changeAvatar(std::string& typeName, cocos2d::extension::CCBone* bone);

	#include "CCSPlayerShader.h"
};

#endif /* __CCS_PLAYER_H__ */
