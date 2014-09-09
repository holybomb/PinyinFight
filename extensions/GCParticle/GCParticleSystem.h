#ifndef _GCPARTICLE_SYSTEM_H__
#define _GCPARTICLE_SYSTEM_H__

#include "cocos2d.h"
#include "ExtensionMacros.h"
#include "support/tinyxml2/tinyxml2.h"
NS_CC_EXT_BEGIN

//д�뵽GPM��PAR�ļ���İ汾���ַ�����ÿ��������ʱ��Ҫɾ������궨�壬ע�͵��ͺ���,���Կ�����������
//#define GCPARTICLE_SYSTEM_VER	"V000"
#define GCPARTICLE_SYSTEM_VER	"V001"

//�汾��int����
#define VERSION_000			0
#define VERSION_001			1


#define CUR_VERSION			VERSION_001

typedef struct {

	std::string selfKey;
	std::string parentKey;
	CCParticleSystem *emitter;

}GCParticleItem;

class CC_EX_DLL GCParticleSystem : public CCNode
{
public:
	GCParticleSystem();
	~GCParticleSystem();

	// ��������
	static GCParticleSystem *create(std::string filePath, std::string fileName);

	CCNode *getEmitter() {return mEmitter;};
private:
	// ��ʼ������,init�ɹ�����true,���򷵻�false
	bool initWithFile(std::string filePath, std::string fileName);

	// ��ȡ�������ļ�,��ȡ�ɹ�����true, ���򷵻�false
	bool loadGPMFiles(std::string filePath, std::string fileName);
	// version 001�汾�������ļ��Ķ�ȡ����
	// bool loadGPMFilesV1(FILE* fp);
	bool loadGPMFilesV1(char* buffer, int off, std::string fileName);

	// ��ȡ�����ļ�,��ȡ�ɹ�����true,���򷵻�false
	bool loadPARFiles(std::string strPath, std::string fileName);
	// version 001�汾�����ļ��Ķ�ȡ����
	bool loadPARFilesV1(tinyxml2::XMLElement* root, std::string strPath, std::string fileName);

	//�Ӷ�ȡ����string�л�õ�ǰ�汾��
	int	 getVersionNum(std::string vns);

	void addToParent();
	void setEmitterAttr
	(
		CCParticleSystem *emitter,
		float angle,
		float angleVar,
		float duration,
		ccBlendFunc func,
		ccColor4F startColor,
		ccColor4F startColorVar,
		ccColor4F endColor,
		ccColor4F endColorVar,
		float startSize,
		float startSizeVar,
		float endSize,
		float endSizeVar,
		bool bSpinVarSame,
		float sleepDuration,
		float sleepDurationVar,
		float sourcePositionX,
		float sourcePositionY,
		float sourcePositionVarX,
		float sourcePositionVarY,
		float startSpin,
		float startSpinVar,
		float endSpin,
		float endSpinVar,
		int emitterMode,
		float gravityX,
		float gravityY,
		float speed,
		float speedVar,
		float radialAccel,
		float radialAccelVar,
		float tangentialAccel,
		float tangentialAccelVar,
		float startRadius,
		float startRadiusVar,
		float endRadius,
		float endRadiusVar,
		float rotatePerSecond,
		float rotatePerSecondVar,
		float life,
		float lifeVar,
		float scaleX,
		float scaleY,
		int positionType,
		float emissionRate,
		float visibleRate,
		float advance,
		float delay,
		int asLength, 
		float* asArray
	);

	void resetAllNode();

	tinyxml2::XMLElement* PAR_getFloatValue(tinyxml2::XMLElement *child, float &val);
	tinyxml2::XMLElement* PAR_getIntValue(tinyxml2::XMLElement *child, int &val);
	tinyxml2::XMLElement* PAR_getStrValue(tinyxml2::XMLElement *child, std::string &val);

	float GPM_getFloatValue(char* src, int &src_off);
	std::string GPM_getStrValue(char* src, int &src_off, char strLen);
	int GPM_getIntValue(char* src, int &src_off);
	char GPM_getByteValue(char* src, int &src_off);

	CCNode *mEmitter;
	std::vector<GCParticleItem> mVector;
	std::map<std::string,CCParticleSystem *> mParentMap;
};

NS_CC_EXT_END
#endif