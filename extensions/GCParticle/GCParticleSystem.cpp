#include "GCParticleSystem.h"


NS_CC_EXT_BEGIN
using namespace std;

GCParticleSystem::GCParticleSystem():mEmitter(NULL)
{
	mParentMap.clear();
	mVector.clear();
}

GCParticleSystem::~GCParticleSystem()
{

}

GCParticleSystem *GCParticleSystem::create(string filePath, string fileName)
{
	GCParticleSystem *pRet = new GCParticleSystem();
	if (pRet && pRet->initWithFile(filePath, fileName))
	{
		pRet->autorelease();
		return pRet;
	}
	CC_SAFE_DELETE(pRet);	
	return NULL;
}

//初始化函数,init成功返回true,否则返回false
bool GCParticleSystem::initWithFile(string filePath, string fileName)
{
	 CCAssert((fileName.length() > 0) , "Invalid filename for GCParticleSystem");

#ifdef Hunter
	 string strPath = CCFileUtils::sharedFileUtils()->MD5fullPathForFilename((filePath + fileName).c_str());
#else
	 string strPath = CCFileUtils::sharedFileUtils()->fullPathForFilename((filePath + fileName).c_str());
#endif // Hunter
	 mEmitter = new CCNode();
	 mEmitter->setPosition(ccp(0, 0));
	 this->addChild(mEmitter, 1000);

	 if(fileName.find(".gpm") != string::npos)
	 {
		 //读取GPM文件
		 return loadGPMFiles(filePath, fileName);
	 } else if(fileName.find(".parpro") != string::npos)
	 {
		 //读取PAR文件
		 return loadPARFiles(strPath, fileName);
	 }

	 return false;
}


bool GCParticleSystem::loadGPMFiles(std::string filePath, std::string fileName)
{
	/*
	//读取二进制文件
	FILE *fp = fopen(strPath.c_str(), "rb");
	if(!fp) {
		return false;
	}
	fseek(fp, 0, SEEK_END );
	size_t size = ftell(fp);
	fseek(fp, 0, SEEK_SET );

	//读取GPM的版本号
	//陈述，需要补全

	//版本号字符串
	unsigned char strLen = 0;
	fread(&strLen ,sizeof(unsigned char), 1, fp);
	char *vsp = new char[strLen + 1];
	fread(vsp,strLen*sizeof(char),1,fp);
	vsp[strLen]= '\0';
	string vns(vsp);
	delete []vsp;
		
	//版本号int索引
	int tmp = getVersionNum(vns); 
	int version = tmp;
	CCAssert((version == CUR_VERSION) , "Cur GMP did not match the newest version");
	
	//开始读取二进制文件
	 //陈述，需要补全

	bool result = false;
	switch(version)
	{
	case VERSION_001:
		result = loadGPMFilesV1(fp);
		break;

	}
	fclose(fp);
	return result;
	*/

	unsigned long nSize = 0;
	unsigned char* pBuffer = CCFileUtils::sharedFileUtils()->getFileData((filePath + fileName).c_str(), "rb", &nSize);
	int off = 0;
	//读取GPM的版本号
	char strLen = GPM_getByteValue((char*)pBuffer,off);
	string verStr = GPM_getStrValue((char*)pBuffer, off, strLen);
	//版本号int索引
	int version = getVersionNum(verStr);
	CCAssert((version == CUR_VERSION) , "Cur GMP did not match the newest version");

	bool result = false;
	switch(version)
	{
	case VERSION_001:
		result = loadGPMFilesV1((char*)pBuffer, off, fileName);
		break;

	}

	delete[] pBuffer;
	return result;
}

/*
bool GCParticleSystem::loadGPMFilesV1(FILE* fp)
{
	int counter = 0;
	fread(&counter ,sizeof(int), 1, fp);
	for (int i = 0; i < counter; i++)
	{
		int maxParticles = 0;
		fread(&maxParticles ,sizeof(int), 1, fp);
		CCParticleSystem *emitter = CCParticleSystemQuad::createWithTotalParticles(maxParticles);
		if (!emitter)
		{
			return false;
		}
		float angle = 0.0f;
		float angleVar = 0.0f;
		float duration = 0.0f;
		ccBlendFunc func;
		ccColor4F startColor;
		ccColor4F startColorVar;
		ccColor4F endColor;
		ccColor4F endColorVar;
		float startSize = 0.0f;
		float startSizeVar = 0.0f;
		float endSize = 0.0f;
		float endSizeVar = 0.0f;
		int nSpinVarSame = 0;
		bool bSpinVarSame = false;
		float sleepDuration = 0.0f;
		float sleepDurationVar = 0.0f;
		float sourcePositionX = 0.0f;
		float sourcePositionY = 0.0f;
		float sourcePositionVarX = 0.0f;
		float sourcePositionVarY = 0.0f;
		float startSpin = 0.0f;
		float startSpinVar = 0.0f;
		float endSpin = 0.0f;
		float endSpinVar = 0.0f;
		int emitterMode = 0;
		float gravityX = 0.0f;
		float gravityY = 0.0f;
		float speed = 0.0f;
		float speedVar = 0.0f;
		float radialAccel = 0.0f;
		float radialAccelVar = 0.0f;
		float tangentialAccel = 0.0f;
		float tangentialAccelVar = 0.0f;
		float startRadius = 0.0f;
		float startRadiusVar = 0.0f;
		float endRadius = 0.0f;
		float endRadiusVar = 0.0f;
		float rotatePerSecond = 0.0f;
		float rotatePerSecondVar = 0.0f;
		float life = 0.0f;
		float lifeVar = 0.0f;
		float scaleX = 0;
		float scaleY = 0;
		int positionType = 0;
		float emissionRate = 0.0f;
		float visibleRate = 0.0f;
		float advance = 0;
		float delay = 0.0f;
		int asLength = 1;

		fread(&angle ,sizeof(float), 1, fp);
		fread(&angleVar ,sizeof(float), 1, fp);
		fread(&duration ,sizeof(float), 1, fp);
		fread(&func.src ,sizeof(int), 1, fp);
		fread(&func.dst ,sizeof(int), 1, fp);
		fread(&startColor.r ,sizeof(float), 1, fp);
		fread(&startColor.g ,sizeof(float), 1, fp);
		fread(&startColor.b ,sizeof(float), 1, fp);
		fread(&startColor.a ,sizeof(float), 1, fp);
		fread(&startColorVar.r ,sizeof(float), 1, fp);
		fread(&startColorVar.g ,sizeof(float), 1, fp);
		fread(&startColorVar.b ,sizeof(float), 1, fp);
		fread(&startColorVar.a ,sizeof(float), 1, fp);
		fread(&endColor.r ,sizeof(float), 1, fp);
		fread(&endColor.g ,sizeof(float), 1, fp);
		fread(&endColor.b ,sizeof(float), 1, fp);
		fread(&endColor.a ,sizeof(float), 1, fp);
		fread(&endColorVar.r ,sizeof(float), 1, fp);
		fread(&endColorVar.g ,sizeof(float), 1, fp);
		fread(&endColorVar.b ,sizeof(float), 1, fp);
		fread(&endColorVar.a ,sizeof(float), 1, fp);
		fread(&startSize ,sizeof(float), 1, fp);
		fread(&startSizeVar ,sizeof(float), 1, fp);
		fread(&endSize ,sizeof(float), 1, fp);
		fread(&endSizeVar ,sizeof(float), 1, fp);
		fread(&nSpinVarSame ,sizeof(int), 1, fp);
		bSpinVarSame = nSpinVarSame > 0;
		fread(&sleepDuration ,sizeof(float), 1, fp);
		fread(&sleepDurationVar ,sizeof(float), 1, fp);
		fread(&sourcePositionX ,sizeof(float), 1, fp);
		fread(&sourcePositionY ,sizeof(float), 1, fp);
		fread(&sourcePositionVarX ,sizeof(float), 1, fp);
		fread(&sourcePositionVarY ,sizeof(float), 1, fp);
		fread(&startSpin ,sizeof(float), 1, fp);
		fread(&startSpinVar ,sizeof(float), 1, fp);
		fread(&endSpin ,sizeof(float), 1, fp);
		fread(&endSpinVar ,sizeof(float), 1, fp);
		fread(&emitterMode ,sizeof(int), 1, fp);
		fread(&gravityX ,sizeof(float), 1, fp);
		fread(&gravityY ,sizeof(float), 1, fp);
		fread(&speed ,sizeof(float), 1, fp);
		fread(&speedVar ,sizeof(float), 1, fp);
		fread(&radialAccel ,sizeof(float), 1, fp);
		fread(&radialAccelVar ,sizeof(float), 1, fp);
		fread(&tangentialAccel ,sizeof(float), 1, fp);
		fread(&tangentialAccelVar ,sizeof(float), 1, fp);
		fread(&startRadius ,sizeof(float), 1, fp);
		fread(&startRadiusVar ,sizeof(float), 1, fp);
		fread(&endRadius ,sizeof(float), 1, fp);
		fread(&endRadiusVar ,sizeof(float), 1, fp);
		fread(&rotatePerSecond ,sizeof(float), 1, fp);
		fread(&rotatePerSecondVar ,sizeof(float), 1, fp);
		fread(&life ,sizeof(float), 1, fp);
		fread(&lifeVar ,sizeof(float), 1, fp);
		fread(&scaleX ,sizeof(float), 1, fp);
		fread(&scaleY ,sizeof(float), 1, fp);
		fread(&positionType ,sizeof(int), 1, fp);
		fread(&emissionRate ,sizeof(float), 1, fp);
		fread(&visibleRate ,sizeof(float), 1, fp);
		fread(&advance, sizeof(float), 1, fp);
		fread(&delay, sizeof(float), 1, fp);
		fread(&asLength, sizeof(int), 1, fp);
		float *asArray = new float[asLength * 3];
		for (int i = 0; i < asLength * 3 ; i++)
		{
			fread(&asArray[i], sizeof(float), 1, fp);
		}

		setEmitterAttr
		(
			emitter,
			angle, angleVar, duration, func, startColor, startColorVar, endColor, endColorVar, 
			startSize, startSizeVar, endSize, endSizeVar, bSpinVarSame, sleepDuration, sleepDurationVar, 
			sourcePositionX, sourcePositionY, sourcePositionVarX, sourcePositionVarY, startSpin, startSpinVar, endSpin, endSpinVar, 
			emitterMode, gravityX, gravityY, speed, speedVar, radialAccel, radialAccelVar, tangentialAccel, tangentialAccelVar, 
			startRadius, startRadiusVar, endRadius, endRadiusVar, rotatePerSecond, rotatePerSecondVar, life, lifeVar, scaleX, scaleY, 
			positionType, emissionRate, visibleRate, advance, delay, asLength, asArray
		);
		delete[] asArray;

		// texture
		CCImage *image = NULL;
		int dataLen = 0;
		fread(&dataLen,sizeof(int),1,fp);
		unsigned char *deflated = new unsigned char[dataLen];
		fread(deflated,sizeof(unsigned char),dataLen,fp);	
		unsigned char strLen = 0;
		fread(&strLen ,sizeof(unsigned char), 1, fp);
		char *ca = new char[strLen + 1];
		fread(ca,strLen*sizeof(char),1,fp);
		ca[strLen]= '\0';
		string textureName(ca);	
		delete []ca;
		ca = NULL;
		image = new CCImage();
		bool isOK = image->initWithImageData(deflated, dataLen);
		delete []deflated;
		if (isOK)
		{
			emitter->setTexture(CCTextureCache::sharedTextureCache()->addUIImage(image, textureName.c_str()));
		}
		image->release();

		// selfKey
		fread(&strLen ,sizeof(unsigned char), 1, fp);
		ca = new char[strLen + 1];
		fread(ca,strLen*sizeof(char),1,fp);
		ca[strLen]= '\0';
		string selfKey(ca);	
		delete []ca;
		ca = NULL;

		// parentKey
		fread(&strLen ,sizeof(unsigned char), 1, fp);
		ca = new char[strLen + 1];
		fread(ca,strLen*sizeof(char),1,fp);
		ca[strLen]= '\0';
		string parentKey(ca);	
		delete []ca;
		ca = NULL;

		//flag
		unsigned int flag = 0;
		fread(&flag,sizeof(unsigned int),1,fp);
		size_t t = ftell(fp);
		if (flag + sizeof(unsigned int) != t)
		{
			return false;
		}

		GCParticleItem item;
		item.emitter = emitter;
		item.parentKey = parentKey;
		item.selfKey = selfKey;
		mVector.push_back(item);

		mParentMap.insert(pair<string, CCParticleSystem *>(selfKey, emitter));
	}
	addToParent();
	return true;
}
*/

bool GCParticleSystem::loadGPMFilesV1(char* buffer, int off, string fileName)
{
	int counter = GPM_getIntValue(buffer, off);
	for (int i = 0; i < counter; i++)
	{
		int maxParticles = GPM_getIntValue(buffer, off);
		CCParticleSystem *emitter = CCParticleSystemQuad::createWithTotalParticles(maxParticles);
		if (!emitter)
		{
			return false;
		}
	
		float angle = 0.0f;
		float angleVar = 0.0f;
		float duration = 0.0f;
		ccBlendFunc func;
		ccColor4F startColor;
		ccColor4F startColorVar;
		ccColor4F endColor;
		ccColor4F endColorVar;
		float startSize = 0.0f;
		float startSizeVar = 0.0f;
		float endSize = 0.0f;
		float endSizeVar = 0.0f;
		int nSpinVarSame = 0;
		bool bSpinVarSame = false;
		float sleepDuration = 0.0f;
		float sleepDurationVar = 0.0f;
		float sourcePositionX = 0.0f;
		float sourcePositionY = 0.0f;
		float sourcePositionVarX = 0.0f;
		float sourcePositionVarY = 0.0f;
		float startSpin = 0.0f;
		float startSpinVar = 0.0f;
		float endSpin = 0.0f;
		float endSpinVar = 0.0f;
		int emitterMode = 0;
		float gravityX = 0.0f;
		float gravityY = 0.0f;
		float speed = 0.0f;
		float speedVar = 0.0f;
		float radialAccel = 0.0f;
		float radialAccelVar = 0.0f;
		float tangentialAccel = 0.0f;
		float tangentialAccelVar = 0.0f;
		float startRadius = 0.0f;
		float startRadiusVar = 0.0f;
		float endRadius = 0.0f;
		float endRadiusVar = 0.0f;
		float rotatePerSecond = 0.0f;
		float rotatePerSecondVar = 0.0f;
		float life = 0.0f;
		float lifeVar = 0.0f;
		float scaleX = 0;
		float scaleY = 0;
		int positionType = 0;
		float emissionRate = 0.0f;
		float visibleRate = 0.0f;
		float advance = 0;
		float delay = 0.0f;
		int asLength = 1;

		angle = GPM_getFloatValue(buffer, off);
		angleVar = GPM_getFloatValue(buffer, off);
		duration = GPM_getFloatValue(buffer, off);
		func.src = GPM_getIntValue(buffer, off);
		func.dst = GPM_getIntValue(buffer, off);
		startColor.r = GPM_getFloatValue(buffer, off);
		startColor.g = GPM_getFloatValue(buffer, off);
		startColor.b = GPM_getFloatValue(buffer, off);
		startColor.a = GPM_getFloatValue(buffer, off);
		startColorVar.r = GPM_getFloatValue(buffer, off);
		startColorVar.g = GPM_getFloatValue(buffer, off);
		startColorVar.b = GPM_getFloatValue(buffer, off);
		startColorVar.a = GPM_getFloatValue(buffer, off);
		endColor.r = GPM_getFloatValue(buffer, off);
		endColor.g = GPM_getFloatValue(buffer, off);
		endColor.b = GPM_getFloatValue(buffer, off);
		endColor.a = GPM_getFloatValue(buffer, off);
		endColorVar.r = GPM_getFloatValue(buffer, off);
		endColorVar.g = GPM_getFloatValue(buffer, off);
		endColorVar.b = GPM_getFloatValue(buffer, off);
		endColorVar.a = GPM_getFloatValue(buffer, off);
		startSize = GPM_getFloatValue(buffer, off);
		startSizeVar = GPM_getFloatValue(buffer, off);
		endSize = GPM_getFloatValue(buffer, off);
		endSizeVar = GPM_getFloatValue(buffer, off);
		nSpinVarSame = GPM_getIntValue(buffer, off);
		bSpinVarSame = nSpinVarSame > 0;
		sleepDuration = GPM_getFloatValue(buffer, off);
		sleepDurationVar = GPM_getFloatValue(buffer, off);
		sourcePositionX = GPM_getFloatValue(buffer, off);
		sourcePositionY = GPM_getFloatValue(buffer, off);
		sourcePositionVarX = GPM_getFloatValue(buffer, off);
		sourcePositionVarY = GPM_getFloatValue(buffer, off);
		startSpin = GPM_getFloatValue(buffer, off);
		startSpinVar = GPM_getFloatValue(buffer, off);
		endSpin = GPM_getFloatValue(buffer, off);
		endSpinVar = GPM_getFloatValue(buffer, off);
		emitterMode = GPM_getIntValue(buffer, off);
		gravityX = GPM_getFloatValue(buffer, off);
		gravityY = GPM_getFloatValue(buffer, off);
		speed = GPM_getFloatValue(buffer, off);
		speedVar = GPM_getFloatValue(buffer, off);
		radialAccel = GPM_getFloatValue(buffer, off);
		radialAccelVar = GPM_getFloatValue(buffer, off);
		tangentialAccel = GPM_getFloatValue(buffer, off);
		tangentialAccelVar = GPM_getFloatValue(buffer, off);
		startRadius = GPM_getFloatValue(buffer, off);
		startRadiusVar = GPM_getFloatValue(buffer, off);
		endRadius = GPM_getFloatValue(buffer, off);
		endRadiusVar = GPM_getFloatValue(buffer, off);
		rotatePerSecond = GPM_getFloatValue(buffer, off);
		rotatePerSecondVar = GPM_getFloatValue(buffer, off);
		life = GPM_getFloatValue(buffer, off);
		lifeVar = GPM_getFloatValue(buffer, off);
		scaleX = GPM_getFloatValue(buffer, off);
		scaleY = GPM_getFloatValue(buffer, off);
		positionType = GPM_getIntValue(buffer, off);
		emissionRate = GPM_getFloatValue(buffer, off);
		visibleRate = GPM_getFloatValue(buffer, off);
		advance = GPM_getFloatValue(buffer, off);
		delay = GPM_getFloatValue(buffer, off);
		asLength = GPM_getIntValue(buffer, off);
		float *asArray = new float[asLength * 3];
		for (int i = 0; i < asLength * 3 ; i++)
		{
			asArray[i] = GPM_getFloatValue(buffer, off);
		}

		setEmitterAttr
			(
			emitter,
			angle, angleVar, duration, func, startColor, startColorVar, endColor, endColorVar, 
			startSize, startSizeVar, endSize, endSizeVar, bSpinVarSame, sleepDuration, sleepDurationVar, 
			sourcePositionX, sourcePositionY, sourcePositionVarX, sourcePositionVarY, startSpin, startSpinVar, endSpin, endSpinVar, 
			emitterMode, gravityX, gravityY, speed, speedVar, radialAccel, radialAccelVar, tangentialAccel, tangentialAccelVar, 
			startRadius, startRadiusVar, endRadius, endRadiusVar, rotatePerSecond, rotatePerSecondVar, life, lifeVar, scaleX, scaleY, 
			positionType, emissionRate, visibleRate, advance, delay, asLength, asArray
			);
		delete[] asArray;
		
		// texture
		CCImage *image = NULL;
		int dataLen = GPM_getIntValue(buffer, off);
		char *deflated = new char[dataLen];
		memcpy(deflated,&buffer[off],sizeof(char)*dataLen);
		off += sizeof(char)*dataLen;
		char strLen  = GPM_getByteValue(buffer, off);
		string textureName = GPM_getStrValue(buffer, off, strLen);
		image = new CCImage();
		bool isOK = image->initWithImageData(deflated, dataLen);
		delete []deflated;
		if (isOK)
		{
			size_t pos = fileName.find(".gpm");
			string key = fileName.substr(0, pos);
			key = key + "_" + textureName;
			emitter->setTexture(CCTextureCache::sharedTextureCache()->addUIImage(image, key.c_str()));
		}
		image->release();

		
		// selfKey
		strLen  = GPM_getByteValue(buffer, off);
		string selfKey = GPM_getStrValue(buffer, off, strLen);
		
		// parentKey
		strLen  = GPM_getByteValue(buffer, off);
		string parentKey = GPM_getStrValue(buffer, off, strLen);
		
		//flag
		int flag = GPM_getIntValue(buffer, off);
		if (flag + sizeof(int) != off)
		{
			return false;
		}

		GCParticleItem item;
		item.emitter = emitter;
		item.parentKey = parentKey;
		item.selfKey = selfKey;
		mVector.push_back(item);

		mParentMap.insert(pair<string, CCParticleSystem *>(selfKey, emitter));

	}
	addToParent();
	return true;
}

bool GCParticleSystem::loadPARFiles(string strPath, string fileName)
{
	//读取工程文件
	tinyxml2::XMLDocument doc;  
	doc.LoadFile(strPath.c_str());  

	//从file文件中获取版本号
	//陈述，需要补全
 	tinyxml2::XMLElement *root = doc.RootElement(); 
 	const tinyxml2::XMLAttribute *attributeOfRoot = root->FirstAttribute();

	int tmp = getVersionNum(attributeOfRoot->Value()); 
	int version = tmp;

	bool result = false;

	switch(version)
	{
	case VERSION_001:
		result = loadPARFilesV1(root, strPath, fileName);
		break;

	}
	return result;
}

bool GCParticleSystem::loadPARFilesV1(tinyxml2::XMLElement* root, string strPath, string fileName)
{
	//读取 VERSION 001 版本的工程文件
	//陈述，需要补全

	tinyxml2::XMLElement *item = root->FirstChildElement("Item");
	while (item)
	{
		string selfKey;
		string parentKey;
		CCParticleSystem *emitter;
		tinyxml2::XMLElement *child = item->FirstChildElement(); 
		while(child)  
		{  
			int maxParticles = 0;
			child = PAR_getIntValue(child,maxParticles);
			emitter = CCParticleSystemQuad::createWithTotalParticles(maxParticles);
			if (!emitter)
			{
				return false;
			}
			float angle = 0.0f;
			float angleVar = 0.0f;
			float duration = 0.0f;
			ccBlendFunc func;
			int src = 0;
			int dst = 0;
			ccColor4F startColor;
			ccColor4F startColorVar;
			ccColor4F endColor;
			ccColor4F endColorVar;
			float startSize = 0.0f;
			float startSizeVar = 0.0f;
			float endSize = 0.0f;
			float endSizeVar = 0.0f;
			int nSpinVarSame = 0;
			bool bSpinVarSame = false;
			float sleepDuration = 0.0f;
			float sleepDurationVar = 0.0f;
			float sourcePositionX = 0.0f;
			float sourcePositionY = 0.0f;
			float sourcePositionVarX = 0.0f;
			float sourcePositionVarY = 0.0f;
			float startSpin = 0.0f;
			float startSpinVar = 0.0f;
			float endSpin = 0.0f;
			float endSpinVar = 0.0f;
			int emitterMode = 0;
			float gravityX = 0.0f;
			float gravityY = 0.0f;
			float speed = 0.0f;
			float speedVar = 0.0f;
			float radialAccel = 0.0f;
			float radialAccelVar = 0.0f;
			float tangentialAccel = 0.0f;
			float tangentialAccelVar = 0.0f;
			float startRadius = 0.0f;
			float startRadiusVar = 0.0f;
			float endRadius = 0.0f;
			float endRadiusVar = 0.0f;
			float rotatePerSecond = 0.0f;
			float rotatePerSecondVar = 0.0f;
			float life = 0.0f;
			float lifeVar = 0.0f;
			float scaleX = 0;
			float scaleY = 0;
			int positionType = 0;
			float emissionRate = 0.0f;
			float visibleRate = 0.0f;
			float advance = 0;
			float delay = 0.0f;
			int asLength = 0;
			float *asArray;

			child = PAR_getFloatValue(child, angle);
			child = PAR_getFloatValue(child, angleVar);
			child = PAR_getFloatValue(child, duration);
			child = PAR_getIntValue(child, src);
			child = PAR_getIntValue(child, dst);
			func.src = src;
			func.dst = dst;
			child = PAR_getFloatValue(child, startColor.r);
			child = PAR_getFloatValue(child, startColor.g);
			child = PAR_getFloatValue(child, startColor.b);
			child = PAR_getFloatValue(child, startColor.a);
			child = PAR_getFloatValue(child, startColorVar.r);
			child = PAR_getFloatValue(child, startColorVar.g);
			child = PAR_getFloatValue(child, startColorVar.b);
			child = PAR_getFloatValue(child, startColorVar.a);
			child = PAR_getFloatValue(child, endColor.r);
			child = PAR_getFloatValue(child, endColor.g);
			child = PAR_getFloatValue(child, endColor.b);
			child = PAR_getFloatValue(child, endColor.a);
			child = PAR_getFloatValue(child, endColorVar.r);
			child = PAR_getFloatValue(child, endColorVar.g);
			child = PAR_getFloatValue(child, endColorVar.b);
			child = PAR_getFloatValue(child, endColorVar.a);
			child = PAR_getFloatValue(child, startSize);
			child = PAR_getFloatValue(child, startSizeVar);
			child = PAR_getFloatValue(child, endSize);
			child = PAR_getFloatValue(child, endSizeVar);
			child = PAR_getIntValue(child, nSpinVarSame);
			bSpinVarSame = nSpinVarSame > 0;
			child = PAR_getFloatValue(child, sleepDuration);
			child = PAR_getFloatValue(child, sleepDurationVar);
			child = PAR_getFloatValue(child, sourcePositionX);
			child = PAR_getFloatValue(child, sourcePositionY);
			child = PAR_getFloatValue(child, sourcePositionVarX);
			child = PAR_getFloatValue(child, sourcePositionVarY);
			child = PAR_getFloatValue(child, startSpin);
			child = PAR_getFloatValue(child, startSpinVar);
			child = PAR_getFloatValue(child, endSpin);
			child = PAR_getFloatValue(child, endSpinVar);
			child = PAR_getIntValue(child, emitterMode);
			child = PAR_getFloatValue(child, gravityX);
			child = PAR_getFloatValue(child, gravityY);
			child = PAR_getFloatValue(child, speed);
			child = PAR_getFloatValue(child, speedVar);
			child = PAR_getFloatValue(child, radialAccel);
			child = PAR_getFloatValue(child, radialAccelVar);
			child = PAR_getFloatValue(child, tangentialAccel);
			child = PAR_getFloatValue(child, tangentialAccelVar);
			child = PAR_getFloatValue(child, startRadius);
			child = PAR_getFloatValue(child, startRadiusVar);
			child = PAR_getFloatValue(child, endRadius);
			child = PAR_getFloatValue(child, endRadiusVar);
			child = PAR_getFloatValue(child, rotatePerSecond);
			child = PAR_getFloatValue(child, rotatePerSecondVar);
			child = PAR_getFloatValue(child, life);
			child = PAR_getFloatValue(child, lifeVar);
			child = PAR_getFloatValue(child, scaleX);
			child = PAR_getFloatValue(child, scaleY);
			child = PAR_getIntValue(child, positionType);
			child = PAR_getFloatValue(child, emissionRate);
			child = PAR_getFloatValue(child, visibleRate);
			child = PAR_getFloatValue(child, advance);
			child = PAR_getFloatValue(child, delay);
			child = PAR_getIntValue(child, asLength);
			asArray = new float[asLength * 3];
			tinyxml2::XMLElement *stage = child->FirstChildElement(); 
			int stageIdx = 0;
			while (stage)
			{

				tinyxml2::XMLElement *stageItme = stage->FirstChildElement(); 
				int itemIdx = 0;
				while(stageItme)
				{
					stageItme = PAR_getFloatValue(stageItme, asArray[stageIdx *3 + itemIdx]);
					CCLog(" %f",asArray[stageIdx *3 + itemIdx]);
					itemIdx++;
				}
				stageIdx++;
				stage = stage->NextSiblingElement(); 
			}
			child = child->NextSiblingElement();

			setEmitterAttr
				(
				emitter,
				angle, angleVar, duration, func, startColor, startColorVar, endColor, endColorVar, 
				startSize, startSizeVar, endSize, endSizeVar, bSpinVarSame, sleepDuration, sleepDurationVar, 
				sourcePositionX, sourcePositionY, sourcePositionVarX, sourcePositionVarY, startSpin, startSpinVar, endSpin, endSpinVar, 
				emitterMode, gravityX, gravityY, speed, speedVar, radialAccel, radialAccelVar, tangentialAccel, tangentialAccelVar, 
				startRadius, startRadiusVar, endRadius, endRadiusVar, rotatePerSecond, rotatePerSecondVar, life, lifeVar, scaleX, scaleY, 
				positionType, emissionRate, visibleRate, advance, delay, asLength, asArray
				);

			delete[] asArray;

			// texture
			CCImage *image = NULL;
			string textureName;
			child = PAR_getStrValue(child,textureName);
			image = new CCImage();
			size_t pos = strPath.find(fileName);
			if (pos != string::npos)
			{
				strPath.replace(pos,fileName.length(),textureName);
			}
			bool isOK = image->initWithImageFile(strPath.c_str());
			if (isOK)
			{
				emitter->setTexture(CCTextureCache::sharedTextureCache()->addUIImage(image, textureName.c_str()));
			}
			image->release();

			// selfKey
			child = PAR_getStrValue(child,selfKey);
			// parentKey	
			child = PAR_getStrValue(child,parentKey);
			int visible = 0;
			child = PAR_getIntValue(child,visible);
		}  

		GCParticleItem particleItem;
		particleItem.emitter = emitter;
		particleItem.parentKey = parentKey;
		particleItem.selfKey = selfKey;
		mVector.push_back(particleItem);

		mParentMap.insert(pair<string, CCParticleSystem *>(selfKey, emitter));

		item = item->NextSiblingElement();  
	}
	addToParent();
	return true;
}
//
//  从读取到的string中获得当前版本号
//
int	GCParticleSystem::getVersionNum(string vns)
{
	int vn = -1;

	size_t start_pos = vns.find('V');
	if(start_pos == string::npos)
		return -1;
	vns.replace(start_pos, 1, string("0"));

	vn = atoi(vns.c_str());

	return vn;
}

void GCParticleSystem::addToParent()
{
	for (vector<GCParticleItem>::iterator it = mVector.begin(); it != mVector.end(); ++it)
	{
		string parentKey = it->parentKey;
		CCParticleSystem *currNode = it->emitter;
		if (parentKey.compare("RootNode") == 0)
		{
			mEmitter->addChild(currNode);
		} 
		else
		{
			map<string,CCParticleSystem *>::iterator  pIt = mParentMap.find(parentKey);
			CCParticleSystem *parent = pIt->second;
			parent->addChild(currNode);
		}
		currNode->resetSystem();
	}
	mParentMap.clear();
}

void GCParticleSystem::setEmitterAttr
(
	CCParticleSystem *emitter,
	float angle, float angleVar, float duration, ccBlendFunc func, ccColor4F startColor, ccColor4F startColorVar, ccColor4F endColor, ccColor4F endColorVar, 
	float startSize, float startSizeVar, float endSize, float endSizeVar, bool bSpinVarSame, float sleepDuration, float sleepDurationVar, 
	float sourcePositionX, float sourcePositionY, float sourcePositionVarX, float sourcePositionVarY, float startSpin, float startSpinVar, float endSpin, float endSpinVar, 
	int emitterMode, float gravityX, float gravityY, float speed, float speedVar, float radialAccel, float radialAccelVar, float tangentialAccel, float tangentialAccelVar, 
	float startRadius, float startRadiusVar, float endRadius, float endRadiusVar, float rotatePerSecond, float rotatePerSecondVar, float life, float lifeVar, float scaleX, float scaleY, 
	int positionType, float emissionRate, float visibleRate, float advance, float delay, int asLength, float* asArray
)
{
	emitter->setAngle(angle);
	emitter->setAngleVar(angleVar);
	emitter->setDuration(duration);
	emitter->setBlendFunc(func);
	emitter->setStartColor(startColor);
	emitter->setStartColorVar(startColorVar);
	emitter->setEndColor(endColor);
	emitter->setEndColorVar(endColorVar);
	emitter->setStartSize(startSize);
	emitter->setStartSizeVar(startSizeVar);
	emitter->setEndSize(endSize);
	emitter->setEndSizeVar(endSizeVar);
#ifdef Hunter
	emitter->setSpinVarSame(bSpinVarSame);
	emitter->setSleepDuration(sleepDuration);
	emitter->setSleepDurationVar(sleepDurationVar);
	emitter->setSourcePosition(ccp(sourcePositionX,sourcePositionY));
#else
	emitter->setPosition( ccp(sourcePositionX,sourcePositionY) );            
#endif // Hunter
	emitter->setPosVar(ccp(sourcePositionVarX,sourcePositionVarY));
	emitter->setStartSpin(startSpin);
	emitter->setStartSpinVar(startSpinVar);
	emitter->setEndSpin(endSpin);
	emitter->setEndSpinVar(endSpinVar);
	emitter->setEmitterMode(emitterMode);
	if( emitterMode == kCCParticleModeGravity ) 
	{
		emitter->setGravity(ccp(gravityX,gravityY));
		emitter->setSpeed(speed);
		emitter->setSpeedVar(speedVar);
		emitter->setRadialAccel(radialAccel);
		emitter->setRadialAccelVar(radialAccelVar);
		emitter->setTangentialAccel(tangentialAccel);
		emitter->setTangentialAccelVar(tangentialAccelVar);
	}else if (emitterMode == kCCParticleModeRadius)
	{
		emitter->setStartRadius(startRadius);
		emitter->setStartRadiusVar(startRadiusVar);
		emitter->setEndRadius(endRadius);
		emitter->setEndRadiusVar(endRadiusVar);
		emitter->setRotatePerSecond(rotatePerSecond);
		emitter->setRotatePerSecondVar(rotatePerSecondVar);
	}
	emitter->setLife(life);
	emitter->setLifeVar(lifeVar);
#ifdef Hunter
	emitter->setScaleX(scaleX == 0 ? 1 : scaleX);
	emitter->setScaleX(scaleY == 0 ? 1 : scaleY);
	emitter->setEmissionRate(emissionRate);
	emitter->setVisibleRate(visibleRate);
	emitter->setAdvance(advance);
	emitter->setDelay(delay);
	emitter->setAlphaStatge(asArray,asLength);
#else
	emitter->setEmissionRate(m_uTotalParticles / m_fLife);      
#endif // Hunter
	emitter->setPositionType((tCCPositionType)positionType);
}

void GCParticleSystem::resetAllNode()
{
	for (vector<GCParticleItem>::iterator it = mVector.begin(); it != mVector.end(); ++it)
	{
		CCParticleSystem *currNode = it->emitter;
		currNode->resetSystem();
	}
}

tinyxml2::XMLElement* GCParticleSystem::PAR_getFloatValue(tinyxml2::XMLElement *child, float &val)
{
	const char* content = child->GetText();
	val = atof(content); 
	child = child->NextSiblingElement(); 
	return child;
}

tinyxml2::XMLElement* GCParticleSystem::PAR_getIntValue(tinyxml2::XMLElement *child, int &val)
{
	const char* content = child->GetText();
	val = atoi(content); 
	child = child->NextSiblingElement(); 
	return child;
}

tinyxml2::XMLElement* GCParticleSystem::PAR_getStrValue(tinyxml2::XMLElement *child, string &val)
{
	const char* content = child->GetText();
	val = string(content);
	child = child->NextSiblingElement(); 
	return child;
}

float GCParticleSystem::GPM_getFloatValue(char* src, int &src_off)
{
	size_t length = sizeof(float);
	float f = 0;
	memcpy((void*) &f,&src[src_off],length);
	src_off += length;
	return f;
}

string GCParticleSystem::GPM_getStrValue(char* src, int &src_off, char strLen)
{
	size_t length = strLen * sizeof(char);
	char *dest = new char[strLen + 1];
	memset(dest,0,length);
	memcpy(dest,&src[src_off],length);
	dest[strLen]= '\0';
	string result(dest);
	delete []dest;
	src_off += length;
	return result;
}

int GCParticleSystem::GPM_getIntValue(char* src, int &src_off)
{
	int n = ((src[src_off] & 0xFF) | ((src[src_off+1] & 0xFF) << 8) | ((src[src_off+2] & 0xFF) << 16) | ((src[src_off+3] & 0xFF) << 24));
	src_off += sizeof(int);
	return n;
}

char GCParticleSystem::GPM_getByteValue(char* src, int &src_off)
{
	char n = src[src_off];
	src_off += sizeof(char);
	return n;
}

NS_CC_EXT_END