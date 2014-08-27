/****************************************************************************
Copyright (c) 2010-2012 cocos2d-x.org
Copyright (c) 2008-2010 Ricardo Quesada
Copyright (c) 2011      Zynga Inc.

http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/

// ideas taken from:
//     . The ocean spray in your face [Jeff Lander]
//        http://www.double.co.nz/dust/col0798.pdf
//     . Building an Advanced Particle System [John van der Burg]
//        http://www.gamasutra.com/features/20000623/vanderburg_01.htm
//   . LOVE game engine
//        http://love2d.org/
//
//
// Radius mode support, from 71 squared
//        http://particledesigner.71squared.com/
//
// IMPORTANT: Particle Designer is supported by cocos2d, but
// 'Radius Mode' in Particle Designer uses a fixed emit rate of 30 hz. Since that can't be guaranteed in cocos2d,
//  cocos2d uses a another approach, but the results are almost identical. 
//

#include "CCParticleSystem.h"
#include "CCParticleBatchNode.h"
#include "ccTypes.h"
#include "textures/CCTextureCache.h"
#include "textures/CCTextureAtlas.h"
#include "support/base64.h"
#include "support/CCPointExtension.h"
#include "platform/CCFileUtils.h"
#include "platform/CCImage.h"
#include "platform/platform.h"
#include "support/zip_support/ZipUtils.h"
#include "CCDirector.h"
#include "support/CCProfiling.h"
// opengl
#include "CCGL.h"

#include <string>

using namespace std;


NS_CC_BEGIN

// ideas taken from:
//     . The ocean spray in your face [Jeff Lander]
//        http://www.double.co.nz/dust/col0798.pdf
//     . Building an Advanced Particle System [John van der Burg]
//        http://www.gamasutra.com/features/20000623/vanderburg_01.htm
//   . LOVE game engine
//        http://love2d.org/
//
//
// Radius mode support, from 71 squared
//        http://particledesigner.71squared.com/
//
// IMPORTANT: Particle Designer is supported by cocos2d, but
// 'Radius Mode' in Particle Designer uses a fixed emit rate of 30 hz. Since that can't be guaranteed in cocos2d,
//  cocos2d uses a another approach, but the results are almost identical. 
//

CCParticleSystem::CCParticleSystem()
: m_sPlistFile("")
, m_fElapsed(0)
, m_pParticles(NULL)
, m_fEmitCounter(0)
, m_uParticleIdx(0)
, m_pBatchNode(NULL)
, m_uAtlasIndex(0)
, m_bTransformSystemDirty(false)
, m_uAllocatedParticles(0)
, m_bIsActive(true)
, m_uParticleCount(0)
, m_fDuration(0)
, m_tSourcePosition(CCPointZero)
, m_tPosVar(CCPointZero)
, m_fLife(0)
, m_fLifeVar(0)
, m_fAngle(0)
, m_fAngleVar(0)
, m_fStartSize(0)
, m_fStartSizeVar(0)
, m_fEndSize(0)
, m_fEndSizeVar(0)
, m_fStartSpin(0)
, m_fStartSpinVar(0)
, m_fEndSpin(0)
, m_fEndSpinVar(0)
, m_fEmissionRate(0)
, m_uTotalParticles(0)
, m_pTexture(NULL)
, m_bOpacityModifyRGB(false)
, m_bIsBlendAdditive(false)
, m_ePositionType(kCCPositionTypeFree)
, m_bIsAutoRemoveOnFinish(false)
, m_nEmitterMode(kCCParticleModeGravity)
#ifdef Hunter
, m_nASLength(0)
, m_fSpinVarSame(false)
, sleepLastTime(0)
, m_fSleepDuration(0)
, m_fVisibleRate(0)
, m_fAdvance(0)
, m_fDelay(0)
#endif // Hunter
{
    modeA.gravity = CCPointZero;
    modeA.speed = 0;
    modeA.speedVar = 0;
    modeA.tangentialAccel = 0;
    modeA.tangentialAccelVar = 0;
    modeA.radialAccel = 0;
    modeA.radialAccelVar = 0;
    modeA.rotationIsDir = false;
    modeB.startRadius = 0;
    modeB.startRadiusVar = 0;
    modeB.endRadius = 0;
    modeB.endRadiusVar = 0;            
    modeB.rotatePerSecond = 0;
    modeB.rotatePerSecondVar = 0;
    m_tBlendFunc.src = CC_BLEND_SRC;
    m_tBlendFunc.dst = CC_BLEND_DST;
	
}
// implementation CCParticleSystem

CCParticleSystem * CCParticleSystem::create(const char *plistFile)
{
    CCParticleSystem *pRet = new CCParticleSystem();
    if (pRet && pRet->initWithFile(plistFile))
    {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return pRet;
}

CCParticleSystem* CCParticleSystem::createWithTotalParticles(unsigned int numberOfParticles)
{
    CCParticleSystem *pRet = new CCParticleSystem();
    if (pRet && pRet->initWithTotalParticles(numberOfParticles))
    {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return pRet;
}

bool CCParticleSystem::init()
{
    return initWithTotalParticles(150);
}

bool CCParticleSystem::initWithFile(const char *plistFile)
{
    bool bRet = false;
    m_sPlistFile = CCFileUtils::sharedFileUtils()->fullPathForFilename(plistFile);
    CCDictionary *dict = CCDictionary::createWithContentsOfFileThreadSafe(m_sPlistFile.c_str());

    CCAssert( dict != NULL, "Particles: file not found");
    
    // XXX compute path from a path, should define a function somewhere to do it
    string listFilePath = plistFile;
    if (listFilePath.find('/') != string::npos)
    {
        listFilePath = listFilePath.substr(0, listFilePath.rfind('/') + 1);
        bRet = this->initWithDictionary(dict, listFilePath.c_str());
    }
    else
    {
        bRet = this->initWithDictionary(dict, "");
    }
    
    dict->release();

    return bRet;
}

bool CCParticleSystem::initWithDictionary(CCDictionary *dictionary)
{
    return initWithDictionary(dictionary, "");
}

bool CCParticleSystem::initWithDictionary(CCDictionary *dictionary, const char *dirname)
{
    bool bRet = false;
    unsigned char *buffer = NULL;
    unsigned char *deflated = NULL;
    CCImage *image = NULL;
    do 
    {
        int maxParticles = dictionary->valueForKey("maxParticles")->intValue();
        // self, not super
        if(this->initWithTotalParticles(maxParticles))
        {
            // angle
            m_fAngle = dictionary->valueForKey("angle")->floatValue();
            m_fAngleVar = dictionary->valueForKey("angleVariance")->floatValue();

            // duration
            m_fDuration = dictionary->valueForKey("duration")->floatValue();

            // blend function 
            m_tBlendFunc.src = dictionary->valueForKey("blendFuncSource")->intValue();
            m_tBlendFunc.dst = dictionary->valueForKey("blendFuncDestination")->intValue();

            // color
            m_tStartColor.r = dictionary->valueForKey("startColorRed")->floatValue();
            m_tStartColor.g = dictionary->valueForKey("startColorGreen")->floatValue();
            m_tStartColor.b = dictionary->valueForKey("startColorBlue")->floatValue();
            m_tStartColor.a = dictionary->valueForKey("startColorAlpha")->floatValue();

            m_tStartColorVar.r = dictionary->valueForKey("startColorVarianceRed")->floatValue();
            m_tStartColorVar.g = dictionary->valueForKey("startColorVarianceGreen")->floatValue();
            m_tStartColorVar.b = dictionary->valueForKey("startColorVarianceBlue")->floatValue();
            m_tStartColorVar.a = dictionary->valueForKey("startColorVarianceAlpha")->floatValue();

            m_tEndColor.r = dictionary->valueForKey("finishColorRed")->floatValue();
            m_tEndColor.g = dictionary->valueForKey("finishColorGreen")->floatValue();
            m_tEndColor.b = dictionary->valueForKey("finishColorBlue")->floatValue();
            m_tEndColor.a = dictionary->valueForKey("finishColorAlpha")->floatValue();

            m_tEndColorVar.r = dictionary->valueForKey("finishColorVarianceRed")->floatValue();
            m_tEndColorVar.g = dictionary->valueForKey("finishColorVarianceGreen")->floatValue();
            m_tEndColorVar.b = dictionary->valueForKey("finishColorVarianceBlue")->floatValue();
            m_tEndColorVar.a = dictionary->valueForKey("finishColorVarianceAlpha")->floatValue();

            // particle size
            m_fStartSize = dictionary->valueForKey("startParticleSize")->floatValue();
            m_fStartSizeVar = dictionary->valueForKey("startParticleSizeVariance")->floatValue();
            m_fEndSize = dictionary->valueForKey("finishParticleSize")->floatValue();
            m_fEndSizeVar = dictionary->valueForKey("finishParticleSizeVariance")->floatValue();

#ifdef Hunter
			m_fSpinVarSame = dictionary->valueForKey("ParticleSpinVarRandomSame")->intValue() > 0;
			m_fSleepDuration = dictionary->valueForKey("ParticleSleepDuration")->floatValue();
			m_fSleepDurationVar = dictionary->valueForKey("ParticleSleepDurationVar")->floatValue();
#endif // Hunter

            // position
            float x = dictionary->valueForKey("sourcePositionx")->floatValue();
            float y = dictionary->valueForKey("sourcePositiony")->floatValue();
#ifdef Hunter
			this->setSourcePosition(ccp(x,y));
#else
            this->setPosition( ccp(x,y) );            
#endif // Hunter
            m_tPosVar.x = dictionary->valueForKey("sourcePositionVariancex")->floatValue();
            m_tPosVar.y = dictionary->valueForKey("sourcePositionVariancey")->floatValue();

            // Spinning
            m_fStartSpin = dictionary->valueForKey("rotationStart")->floatValue();
            m_fStartSpinVar = dictionary->valueForKey("rotationStartVariance")->floatValue();
            m_fEndSpin= dictionary->valueForKey("rotationEnd")->floatValue();
            m_fEndSpinVar= dictionary->valueForKey("rotationEndVariance")->floatValue();

            m_nEmitterMode = dictionary->valueForKey("emitterType")->intValue();

            // Mode A: Gravity + tangential accel + radial accel
            if( m_nEmitterMode == kCCParticleModeGravity ) 
            {
                // gravity
                modeA.gravity.x = dictionary->valueForKey("gravityx")->floatValue();
                modeA.gravity.y = dictionary->valueForKey("gravityy")->floatValue();

                // speed
                modeA.speed = dictionary->valueForKey("speed")->floatValue();
                modeA.speedVar = dictionary->valueForKey("speedVariance")->floatValue();

                // radial acceleration
                modeA.radialAccel = dictionary->valueForKey("radialAcceleration")->floatValue();
                modeA.radialAccelVar = dictionary->valueForKey("radialAccelVariance")->floatValue();

                // tangential acceleration
                modeA.tangentialAccel = dictionary->valueForKey("tangentialAcceleration")->floatValue();
                modeA.tangentialAccelVar = dictionary->valueForKey("tangentialAccelVariance")->floatValue();
                
                // rotation is dir
                modeA.rotationIsDir = dictionary->valueForKey("rotationIsDir")->boolValue();
            }

            // or Mode B: radius movement
            else if( m_nEmitterMode == kCCParticleModeRadius ) 
            {
                modeB.startRadius = dictionary->valueForKey("maxRadius")->floatValue();
                modeB.startRadiusVar = dictionary->valueForKey("maxRadiusVariance")->floatValue();
                modeB.endRadius = dictionary->valueForKey("minRadius")->floatValue();
                modeB.endRadiusVar = 0.0f;
                modeB.rotatePerSecond = dictionary->valueForKey("rotatePerSecond")->floatValue();
                modeB.rotatePerSecondVar = dictionary->valueForKey("rotatePerSecondVariance")->floatValue();

            } else {
                CCAssert( false, "Invalid emitterType in config file");
                CC_BREAK_IF(true);
            }

            // life span
            m_fLife = dictionary->valueForKey("particleLifespan")->floatValue();
            m_fLifeVar = dictionary->valueForKey("particleLifespanVariance")->floatValue();

#ifdef Hunter
			float scaleX = dictionary->valueForKey("ScaleX")->floatValue();
			float scaleY = dictionary->valueForKey("ScaleY")->floatValue();
			
			this->setScaleX(scaleX == 0 ? 1 : scaleX);
			this->setScaleY(scaleY == 0 ? 1 : scaleY);

			m_ePositionType = (tCCPositionType)dictionary->valueForKey("positionType")->intValue();
            // emission Rate
            m_fEmissionRate = dictionary->valueForKey("emissionRate")->floatValue();//m_uTotalParticles / m_fLife;
#else
            m_fEmissionRate = m_uTotalParticles / m_fLife;
#endif // Hunter

            //don't get the internal texture if a batchNode is used
            if (!m_pBatchNode)
            {
                // Set a compatible default for the alpha transfer
                m_bOpacityModifyRGB = false;

                // texture        
                // Try to get the texture from the cache
                std::string textureName = dictionary->valueForKey("textureFileName")->getCString();
                
                size_t rPos = textureName.rfind('/');
               
                if (rPos != string::npos)
                {
                    string textureDir = textureName.substr(0, rPos + 1);
                    
                    if (dirname != NULL && textureDir != dirname)
                    {
                        textureName = textureName.substr(rPos+1);
                        textureName = string(dirname) + textureName;
                    }
                }
                else
                {
                    if (dirname != NULL)
                    {
                        textureName = string(dirname) + textureName;
                    }
                }
                
                CCTexture2D *tex = NULL;
                
                if (textureName.length() > 0)
                {
                    // set not pop-up message box when load image failed
                    bool bNotify = CCFileUtils::sharedFileUtils()->isPopupNotify();
                    CCFileUtils::sharedFileUtils()->setPopupNotify(false);
                    tex = CCTextureCache::sharedTextureCache()->addImage(textureName.c_str());
                    // reset the value of UIImage notify
                    CCFileUtils::sharedFileUtils()->setPopupNotify(bNotify);
                }
                
                if (tex)
                {
                    setTexture(tex);
                }
                else
                {                        
                    const char *textureData = dictionary->valueForKey("textureImageData")->getCString();
                    CCAssert(textureData, "");
                    
                    int dataLen = strlen(textureData);
                    if(dataLen != 0)
                    {
                        // if it fails, try to get it from the base64-gzipped data    
                        int decodeLen = base64Decode((unsigned char*)textureData, (unsigned int)dataLen, &buffer);
                        CCAssert( buffer != NULL, "CCParticleSystem: error decoding textureImageData");
                        CC_BREAK_IF(!buffer);
                        
                        int deflatedLen = ZipUtils::ccInflateMemory(buffer, decodeLen, &deflated);
                        CCAssert( deflated != NULL, "CCParticleSystem: error ungzipping textureImageData");
                        CC_BREAK_IF(!deflated);
                        
                        // For android, we should retain it in VolatileTexture::addCCImage which invoked in CCTextureCache::sharedTextureCache()->addUIImage()
                        image = new CCImage();
                        bool isOK = image->initWithImageData(deflated, deflatedLen);
                        CCAssert(isOK, "CCParticleSystem: error init image with Data");
                        CC_BREAK_IF(!isOK);
                        
                        setTexture(CCTextureCache::sharedTextureCache()->addUIImage(image, textureName.c_str()));

                        image->release();
                    }
                }
                CCAssert( this->m_pTexture != NULL, "CCParticleSystem: error loading the texture");
            }
            bRet = true;
        }
    } while (0);
    CC_SAFE_DELETE_ARRAY(buffer);
    CC_SAFE_DELETE_ARRAY(deflated);
    return bRet;
}

bool CCParticleSystem::initWithTotalParticles(unsigned int numberOfParticles)
{
    m_uTotalParticles = numberOfParticles;

    CC_SAFE_FREE(m_pParticles);
    
    m_pParticles = (tCCParticle*)calloc(m_uTotalParticles, sizeof(tCCParticle));

    if( ! m_pParticles )
    {
        CCLOG("Particle system: not enough memory");
        this->release();
        return false;
    }
    m_uAllocatedParticles = numberOfParticles;

    if (m_pBatchNode)
    {
        for (unsigned int i = 0; i < m_uTotalParticles; i++)
        {
            m_pParticles[i].atlasIndex=i;
        }
    }
    // default, active
    m_bIsActive = true;

    // default blend function
    m_tBlendFunc.src = CC_BLEND_SRC;
    m_tBlendFunc.dst = CC_BLEND_DST;

    // default movement type;
    m_ePositionType = kCCPositionTypeFree;

    // by default be in mode A:
    m_nEmitterMode = kCCParticleModeGravity;

    // default: modulate
    // XXX: not used
    //    colorModulate = YES;

    m_bIsAutoRemoveOnFinish = false;

    // Optimization: compile updateParticle method
    //updateParticleSel = @selector(updateQuadWithParticle:newPosition:);
    //updateParticleImp = (CC_UPDATE_PARTICLE_IMP) [self methodForSelector:updateParticleSel];
    //for batchNode
    m_bTransformSystemDirty = false;
    // update after action in run!
    this->scheduleUpdateWithPriority(1);
//	preWarm(2);
    return true;
}

CCParticleSystem::~CCParticleSystem()
{
    // Since the scheduler retains the "target (in this case the ParticleSystem)
	// it is not needed to call "unscheduleUpdate" here. In fact, it will be called in "cleanup"
    //unscheduleUpdate();
    CC_SAFE_FREE(m_pParticles);
    CC_SAFE_RELEASE(m_pTexture);
}

bool CCParticleSystem::addParticle()
{
    if (this->isFull())
    {
        return false;
    }

    tCCParticle * particle = &m_pParticles[ m_uParticleCount ];
    this->initParticle(particle);
    ++m_uParticleCount;

    return true;
}

void CCParticleSystem::initParticle(tCCParticle* particle)
{
    // timeToLive
    // no negative life. prevent division by 0
    particle->timeToLive = m_fLife + m_fLifeVar * CCRANDOM_MINUS1_1();
    particle->timeToLive = MAX(0, particle->timeToLive);

    // position
    particle->pos.x = m_tSourcePosition.x + m_tPosVar.x * CCRANDOM_MINUS1_1();

    particle->pos.y = m_tSourcePosition.y + m_tPosVar.y * CCRANDOM_MINUS1_1();


    // Color

#ifdef Hunter
	particle->elapsed = 0;
	particle->alphaStageIdx = 1;
	particle->frameCounter = 0;
	// alpha单独计算
	vector<alphaStage>::iterator currIt = asVector.begin();
	vector<alphaStage>::iterator endIt = --asVector.end();
	endIt->tPoint = particle->timeToLive;

	ccColor4F start;
	start.r = clampf(m_tStartColor.r + m_tStartColorVar.r * CCRANDOM_MINUS1_1(), 0, 1);
	start.g = clampf(m_tStartColor.g + m_tStartColorVar.g * CCRANDOM_MINUS1_1(), 0, 1);
	start.b = clampf(m_tStartColor.b + m_tStartColorVar.b * CCRANDOM_MINUS1_1(), 0, 1);
	start.a = clampf(currIt->alpha + currIt->alphaVar * CCRANDOM_MINUS1_1(), 0, 1);
	particle->color = start;
	ccColor4F end;
	end.r = clampf(m_tEndColor.r + m_tEndColorVar.r * CCRANDOM_MINUS1_1(), 0, 1);
	end.g = clampf(m_tEndColor.g + m_tEndColorVar.g * CCRANDOM_MINUS1_1(), 0, 1);
	end.b = clampf(m_tEndColor.b + m_tEndColorVar.b * CCRANDOM_MINUS1_1(), 0, 1);
	particle->deltaColor.r = (end.r - start.r) / particle->timeToLive;
	particle->deltaColor.g = (end.g - start.g) / particle->timeToLive;
	particle->deltaColor.b = (end.b - start.b) / particle->timeToLive;

	bool bfirst = false;
	while(true)
	{
		vector<alphaStage>::iterator it = currIt;
		vector<alphaStage>::iterator nextIt = ++it;
		if (nextIt == asVector.end())
		{
			break;
		}

		float nextAlpha = nextIt->alpha;
		nextAlpha = clampf(nextAlpha + nextIt->alphaVar * CCRANDOM_MINUS1_1(), 0, 1);
		nextIt->alpha = nextAlpha;
		float interval = nextIt->tPoint - currIt->tPoint;
		nextIt->deltaAlpha = (nextAlpha - currIt->alpha)/interval;
		if (!bfirst)
		{
			bfirst = true;
			particle->deltaColor.a = nextIt->deltaAlpha;
		}
		currIt = nextIt;
	}
	
#else
    ccColor4F start;
    start.r = clampf(m_tStartColor.r + m_tStartColorVar.r * CCRANDOM_MINUS1_1(), 0, 1);
    start.g = clampf(m_tStartColor.g + m_tStartColorVar.g * CCRANDOM_MINUS1_1(), 0, 1);
    start.b = clampf(m_tStartColor.b + m_tStartColorVar.b * CCRANDOM_MINUS1_1(), 0, 1);
    start.a = clampf(m_tStartColor.a + m_tStartColorVar.a * CCRANDOM_MINUS1_1(), 0, 1);

    ccColor4F end;
    end.r = clampf(m_tEndColor.r + m_tEndColorVar.r * CCRANDOM_MINUS1_1(), 0, 1);
    end.g = clampf(m_tEndColor.g + m_tEndColorVar.g * CCRANDOM_MINUS1_1(), 0, 1);
    end.b = clampf(m_tEndColor.b + m_tEndColorVar.b * CCRANDOM_MINUS1_1(), 0, 1);
    end.a = clampf(m_tEndColor.a + m_tEndColorVar.a * CCRANDOM_MINUS1_1(), 0, 1);

    particle->color = start;
    particle->deltaColor.r = (end.r - start.r) / particle->timeToLive;
    particle->deltaColor.g = (end.g - start.g) / particle->timeToLive;
    particle->deltaColor.b = (end.b - start.b) / particle->timeToLive;
    particle->deltaColor.a = (end.a - start.a) / particle->timeToLive;
#endif // Huntrt


    // size
    float startS = m_fStartSize + m_fStartSizeVar * CCRANDOM_MINUS1_1();
    startS = MAX(0, startS); // No negative value

    particle->size = startS;

    if( m_fEndSize == kCCParticleStartSizeEqualToEndSize )
    {
        particle->deltaSize = 0;
    }
    else
    {
        float endS = m_fEndSize + m_fEndSizeVar * CCRANDOM_MINUS1_1();
        endS = MAX(0, endS); // No negative values
        particle->deltaSize = (endS - startS) / particle->timeToLive;
    }

    // rotation
#ifdef Hunter
	if(m_fSpinVarSame)
	{
		float randomSpin = CCRANDOM_MINUS1_1();
		float startA = m_fStartSpin + m_fStartSpinVar * randomSpin;
		float endA = m_fEndSpin + m_fEndSpinVar * randomSpin;
		particle->rotation = startA;
		particle->deltaRotation = (endA - startA) / particle->timeToLive;
	} else 
	{
		float startA = m_fStartSpin + m_fStartSpinVar * CCRANDOM_MINUS1_1();
		float endA = m_fEndSpin + m_fEndSpinVar * CCRANDOM_MINUS1_1();
		particle->rotation = startA;
		particle->deltaRotation = (endA - startA) / particle->timeToLive;
	}
#else
    float startA = m_fStartSpin + m_fStartSpinVar * CCRANDOM_MINUS1_1();
    float endA = m_fEndSpin + m_fEndSpinVar * CCRANDOM_MINUS1_1();
    particle->rotation = startA;
    particle->deltaRotation = (endA - startA) / particle->timeToLive;
#endif // Hunter

    // position
    if( m_ePositionType == kCCPositionTypeFree )
    {
        particle->startPos = this->convertToWorldSpace(CCPointZero);
    }
    else if ( m_ePositionType == kCCPositionTypeRelative )
    {
        particle->startPos = m_obPosition;
    }

    // direction
    float a = CC_DEGREES_TO_RADIANS( m_fAngle + m_fAngleVar * CCRANDOM_MINUS1_1() );    

    // Mode Gravity: A
    if (m_nEmitterMode == kCCParticleModeGravity) 
    {
        CCPoint v(cosf( a ), sinf( a ));
        float s = modeA.speed + modeA.speedVar * CCRANDOM_MINUS1_1();

        // direction
        particle->modeA.dir = ccpMult( v, s );

        // radial accel
        particle->modeA.radialAccel = modeA.radialAccel + modeA.radialAccelVar * CCRANDOM_MINUS1_1();
 

        // tangential accel
        particle->modeA.tangentialAccel = modeA.tangentialAccel + modeA.tangentialAccelVar * CCRANDOM_MINUS1_1();

        // rotation is dir
        if(modeA.rotationIsDir)
            particle->rotation = -CC_RADIANS_TO_DEGREES(ccpToAngle(particle->modeA.dir));
    }

    // Mode Radius: B
    else 
    {
        // Set the default diameter of the particle from the source position
        float startRadius = modeB.startRadius + modeB.startRadiusVar * CCRANDOM_MINUS1_1();
        float endRadius = modeB.endRadius + modeB.endRadiusVar * CCRANDOM_MINUS1_1();

        particle->modeB.radius = startRadius;

        if(modeB.endRadius == kCCParticleStartRadiusEqualToEndRadius)
        {
            particle->modeB.deltaRadius = 0;
        }
        else
        {
            particle->modeB.deltaRadius = (endRadius - startRadius) / particle->timeToLive;
        }

        particle->modeB.angle = a;
        particle->modeB.degreesPerSecond = CC_DEGREES_TO_RADIANS(modeB.rotatePerSecond + modeB.rotatePerSecondVar * CCRANDOM_MINUS1_1());
    }    
}

void CCParticleSystem::stopSystem()
{
    m_bIsActive = false;
    m_fElapsed = m_fDuration;
    m_fEmitCounter = 0;
}

void CCParticleSystem::resetSystem()
{
#ifdef Hunter
	if (m_fDelay != 0.0f)
	{
		m_bIsActive = false;
		scheduleOnce(schedule_selector(CCParticleSystem::delayOver), m_fDelay);
	} 
	else
	{
		delayOver(0);
	}
#else
    m_bIsActive = true;
#endif //Hunter
    m_fElapsed = 0;
    for (m_uParticleIdx = 0; m_uParticleIdx < m_uParticleCount; ++m_uParticleIdx)
    {
        tCCParticle *p = &m_pParticles[m_uParticleIdx];
        p->timeToLive = 0;
    }


}
bool CCParticleSystem::isFull()
{
    return (m_uParticleCount == m_uTotalParticles);
}
#ifdef Hunter
typedef long long				s64;
static s64 currentTimeMillis()
{
	struct cc_timeval ptv;
	CCTime::gettimeofdayCocos2d(&ptv, NULL);
	s64 tmp = ptv.tv_sec;
	s64 tmp2 = 1000;
	tmp *= tmp2;
	tmp += ptv.tv_usec / tmp2;
	return tmp;
}
#endif // Hunter

#define kParticleFPS		60.0
void CCParticleSystem::preWarm(int cycle)
{
	//if(true)
	//	return;
	//int cycleParCounter = cycle / m_fEmissionRate;

	//if(cycleParCounter < m_uTotalParticles)
	//	cycleParCounter = m_uTotalParticles;

	for(int i = 0; i < cycle * 60; i++)
	{
		this->update(0);
	}
}


// ParticleSystem - MainLoop
void CCParticleSystem::update(float dt)
{
	
#ifdef Hunter
	dt = 1.0 / kParticleFPS; // 为了粒子的均匀发射

	s64 curTime = currentTimeMillis();
	if((m_fSleepDuration != 0 || m_fSleepDurationVar != 0) && curTime - sleepLastTime < m_fSleepDuration + sleepDurationCurVar )
	{
		return ;
	}

	if(!isUseBufferOffset())
	{
		CCPoint curPos = getPosition();

		CCPoint cp = convertToWorldSpace(curPos);

		CCSize s = CCDirector::sharedDirector()->getWinSize();
		if(cp.x + ( m_tSourcePosition.x + m_tPosVar.x ) + s.width / 2< 0 || cp.x - (m_tSourcePosition.x + m_tPosVar.x) -  s.width/2> s.width )
		{
			//CCLog("Anim %d fram %d cp (%f, %f) ---------point x %f y %f  width %d, height %d-----",curAnim, curFrame,curPos.x,curPos.y, cp.x,cp.y,w,h);
			return;
		}
	}
#endif // Hunter

    CC_PROFILER_START_CATEGORY(kCCProfilerCategoryParticles , "CCParticleSystem - update");

    if (m_bIsActive && m_fEmissionRate)
    {
        float rate = 1.0f / m_fEmissionRate;
        //issue #1201, prevent bursts of particles, due to too high emitCounter
        if (m_uParticleCount < m_uTotalParticles)
        {
            m_fEmitCounter += dt;
        }
        
        while (m_uParticleCount < m_uTotalParticles && m_fEmitCounter > rate) 
        {
            this->addParticle();
            m_fEmitCounter -= rate;
        }

        m_fElapsed += dt;
        if (m_fDuration != -1 && m_fDuration < m_fElapsed)
        {
            this->stopSystem();
        }
    }

    m_uParticleIdx = 0;

    CCPoint currentPosition = CCPointZero;
    if (m_ePositionType == kCCPositionTypeFree)
    {
        currentPosition = this->convertToWorldSpace(CCPointZero);
    }
    else if (m_ePositionType == kCCPositionTypeRelative)
    {
        currentPosition = m_obPosition;
    }

    if (m_bVisible)
    {
#ifdef Hunter
// XXX 提出临时变量
        tCCParticle *p;
        CCPoint tmp, radial, tangential, newPos;
        float newy;
#endif // Hunter
        while (m_uParticleIdx < m_uParticleCount)
        {
            p = &m_pParticles[m_uParticleIdx];

            // life
            p->timeToLive -= dt;

            if (p->timeToLive > 0) 
            {
                // Mode A: gravity, direction, tangential accel & radial accel
                if (m_nEmitterMode == kCCParticleModeGravity) 
                {
                    

                    radial = CCPointZero;
                    // radial acceleration
                    if (p->pos.x || p->pos.y)
                    {
                        radial = ccpNormalize(p->pos);
                    }
                    tangential = radial;
                    radial = ccpMult(radial, p->modeA.radialAccel);

                    // tangential acceleration
                    newy = tangential.x;
                    tangential.x = -tangential.y;
                    tangential.y = newy;
                    tangential = ccpMult(tangential, p->modeA.tangentialAccel);

                    // (gravity + radial + tangential) * dt
                    tmp = ccpAdd( ccpAdd( radial, tangential), modeA.gravity);
                    tmp = ccpMult( tmp, dt);
                    p->modeA.dir = ccpAdd( p->modeA.dir, tmp);
                    tmp = ccpMult(p->modeA.dir, dt);
                    p->pos = ccpAdd( p->pos, tmp );
                }

                // Mode B: radius movement
                else 
                {                
                    // Update the angle and radius of the particle.
                    p->modeB.angle += p->modeB.degreesPerSecond * dt;
                    p->modeB.radius += p->modeB.deltaRadius * dt;

                    p->pos.x = - cosf(p->modeB.angle) * p->modeB.radius;
                    p->pos.y = - sinf(p->modeB.angle) * p->modeB.radius;
                }

                // color
#ifdef Hunter	
				p->elapsed += dt;				
				updeteDeltaAlpha(p);
#endif // Hunter
                p->color.r += (p->deltaColor.r * dt);
                p->color.g += (p->deltaColor.g * dt);
                p->color.b += (p->deltaColor.b * dt);
                p->color.a += (p->deltaColor.a * dt);

                // size
                p->size += (p->deltaSize * dt);
                p->size = MAX( 0, p->size );

                // angle
                p->rotation += (p->deltaRotation * dt);

                //
                // update values in quad
                //

                

                if (m_ePositionType == kCCPositionTypeFree || m_ePositionType == kCCPositionTypeRelative) 
                {
                    CCPoint diff = ccpSub( currentPosition, p->startPos );
                    newPos = ccpSub(p->pos, diff);
                } 
                else
                {
                    newPos = p->pos;
                }

                // translate newPos to correct position, since matrix transform isn't performed in batchnode
                // don't update the particle with the new position information, it will interfere with the radius and tangential calculations
                if (m_pBatchNode)
                {
                    newPos.x+=m_obPosition.x;
                    newPos.y+=m_obPosition.y;
                }

                updateQuadWithParticle(p, newPos);
                //updateParticleImp(self, updateParticleSel, p, newPos);

                // update particle counter
                ++m_uParticleIdx;
            } 
            else 
            {

#ifdef Hunter
				if(m_fSleepDuration != 0 || m_fSleepDurationVar != 0)
				{
					sleepLastTime = curTime;
					sleepDurationCurVar = m_fSleepDurationVar * CCRANDOM_MINUS1_1();
				}
#endif // Hunter
                // life < 0
                int currentIndex = p->atlasIndex;
                if( m_uParticleIdx != m_uParticleCount-1 )
                {
                    m_pParticles[m_uParticleIdx] = m_pParticles[m_uParticleCount-1];
                }
                if (m_pBatchNode)
                {
                    //disable the switched particle
                    m_pBatchNode->disableParticle(m_uAtlasIndex+currentIndex);

                    //switch indexes
                    m_pParticles[m_uParticleCount-1].atlasIndex = currentIndex;
                }


                --m_uParticleCount;

                if( m_uParticleCount == 0 && m_bIsAutoRemoveOnFinish )
                {
                    this->unscheduleUpdate();
                    m_pParent->removeChild(this, true);
                    return;
                }
            }
        } //while
        m_bTransformSystemDirty = false;
    }
    if (! m_pBatchNode)
    {
        postStep();
    }

    CC_PROFILER_STOP_CATEGORY(kCCProfilerCategoryParticles , "CCParticleSystem - update");
}

void CCParticleSystem::updateWithNoTime(void)
{
    this->update(0.0f);
}

void CCParticleSystem::updateQuadWithParticle(tCCParticle* particle, const CCPoint& newPosition)
{
    CC_UNUSED_PARAM(particle);
    CC_UNUSED_PARAM(newPosition);
    // should be overridden
}

void CCParticleSystem::postStep()
{
    // should be overridden
}

// ParticleSystem - CCTexture protocol
void CCParticleSystem::setTexture(CCTexture2D* var)
{
    if (m_pTexture != var)
    {
        CC_SAFE_RETAIN(var);
        CC_SAFE_RELEASE(m_pTexture);
        m_pTexture = var;
        updateBlendFunc();
    }
}

void CCParticleSystem::updateBlendFunc()
{
    CCAssert(! m_pBatchNode, "Can't change blending functions when the particle is being batched");

    if(m_pTexture)
    {
        bool premultiplied = m_pTexture->hasPremultipliedAlpha();
        
        m_bOpacityModifyRGB = false;
        
        if( m_pTexture && ( m_tBlendFunc.src == CC_BLEND_SRC && m_tBlendFunc.dst == CC_BLEND_DST ) )
        {
            if( premultiplied )
            {
                m_bOpacityModifyRGB = true;
            }
            else
            {
                m_tBlendFunc.src = GL_SRC_ALPHA;
                m_tBlendFunc.dst = GL_ONE_MINUS_SRC_ALPHA;
            }
        }
    }
}

CCTexture2D * CCParticleSystem::getTexture()
{
    return m_pTexture;
}

// ParticleSystem - Additive Blending
void CCParticleSystem::setBlendAdditive(bool additive)
{
    if( additive )
    {
        m_tBlendFunc.src = GL_SRC_ALPHA;
        m_tBlendFunc.dst = GL_ONE;
    }
    else
    {
        if( m_pTexture && ! m_pTexture->hasPremultipliedAlpha() )
        {
            m_tBlendFunc.src = GL_SRC_ALPHA;
            m_tBlendFunc.dst = GL_ONE_MINUS_SRC_ALPHA;
        } 
        else 
        {
            m_tBlendFunc.src = CC_BLEND_SRC;
            m_tBlendFunc.dst = CC_BLEND_DST;
        }
    }
}

bool CCParticleSystem::isBlendAdditive()
{
    return( m_tBlendFunc.src == GL_SRC_ALPHA && m_tBlendFunc.dst == GL_ONE);
}

// ParticleSystem - Properties of Gravity Mode 
void CCParticleSystem::setTangentialAccel(float t)
{
    CCAssert( m_nEmitterMode == kCCParticleModeGravity, "Particle Mode should be Gravity");
    modeA.tangentialAccel = t;
}

float CCParticleSystem::getTangentialAccel()
{
    CCAssert( m_nEmitterMode == kCCParticleModeGravity, "Particle Mode should be Gravity");
    return modeA.tangentialAccel;
}

void CCParticleSystem::setTangentialAccelVar(float t)
{
    CCAssert( m_nEmitterMode == kCCParticleModeGravity, "Particle Mode should be Gravity");
    modeA.tangentialAccelVar = t;
}

float CCParticleSystem::getTangentialAccelVar()
{
    CCAssert( m_nEmitterMode == kCCParticleModeGravity, "Particle Mode should be Gravity");
    return modeA.tangentialAccelVar;
}    

void CCParticleSystem::setRadialAccel(float t)
{
    CCAssert( m_nEmitterMode == kCCParticleModeGravity, "Particle Mode should be Gravity");
    modeA.radialAccel = t;
}

float CCParticleSystem::getRadialAccel()
{
    CCAssert( m_nEmitterMode == kCCParticleModeGravity, "Particle Mode should be Gravity");
    return modeA.radialAccel;
}

void CCParticleSystem::setRadialAccelVar(float t)
{
    CCAssert( m_nEmitterMode == kCCParticleModeGravity, "Particle Mode should be Gravity");
    modeA.radialAccelVar = t;
}

float CCParticleSystem::getRadialAccelVar()
{
    CCAssert( m_nEmitterMode == kCCParticleModeGravity, "Particle Mode should be Gravity");
    return modeA.radialAccelVar;
}

void CCParticleSystem::setRotationIsDir(bool t)
{
    CCAssert( m_nEmitterMode == kCCParticleModeGravity, "Particle Mode should be Gravity");
    modeA.rotationIsDir = t;
}

bool CCParticleSystem::getRotationIsDir()
{
    CCAssert( m_nEmitterMode == kCCParticleModeGravity, "Particle Mode should be Gravity");
    return modeA.rotationIsDir;
}

void CCParticleSystem::setGravity(const CCPoint& g)
{
    CCAssert( m_nEmitterMode == kCCParticleModeGravity, "Particle Mode should be Gravity");
    modeA.gravity = g;
}

const CCPoint& CCParticleSystem::getGravity()
{
    CCAssert( m_nEmitterMode == kCCParticleModeGravity, "Particle Mode should be Gravity");
    return modeA.gravity;
}

void CCParticleSystem::setSpeed(float speed)
{
    CCAssert( m_nEmitterMode == kCCParticleModeGravity, "Particle Mode should be Gravity");
    modeA.speed = speed;
}

float CCParticleSystem::getSpeed()
{
    CCAssert( m_nEmitterMode == kCCParticleModeGravity, "Particle Mode should be Gravity");
    return modeA.speed;
}

void CCParticleSystem::setSpeedVar(float speedVar)
{
    CCAssert( m_nEmitterMode == kCCParticleModeGravity, "Particle Mode should be Gravity");
    modeA.speedVar = speedVar;
}

float CCParticleSystem::getSpeedVar()
{
    CCAssert( m_nEmitterMode == kCCParticleModeGravity, "Particle Mode should be Gravity");
    return modeA.speedVar;
}

// ParticleSystem - Properties of Radius Mode
void CCParticleSystem::setStartRadius(float startRadius)
{
    CCAssert( m_nEmitterMode == kCCParticleModeRadius, "Particle Mode should be Radius");
    modeB.startRadius = startRadius;
}

float CCParticleSystem::getStartRadius()
{
    CCAssert( m_nEmitterMode == kCCParticleModeRadius, "Particle Mode should be Radius");
    return modeB.startRadius;
}

void CCParticleSystem::setStartRadiusVar(float startRadiusVar)
{
    CCAssert( m_nEmitterMode == kCCParticleModeRadius, "Particle Mode should be Radius");
    modeB.startRadiusVar = startRadiusVar;
}

float CCParticleSystem::getStartRadiusVar()
{
    CCAssert( m_nEmitterMode == kCCParticleModeRadius, "Particle Mode should be Radius");
    return modeB.startRadiusVar;
}

void CCParticleSystem::setEndRadius(float endRadius)
{
    CCAssert( m_nEmitterMode == kCCParticleModeRadius, "Particle Mode should be Radius");
    modeB.endRadius = endRadius;
}

float CCParticleSystem::getEndRadius()
{
    CCAssert( m_nEmitterMode == kCCParticleModeRadius, "Particle Mode should be Radius");
    return modeB.endRadius;
}

void CCParticleSystem::setEndRadiusVar(float endRadiusVar)
{
    CCAssert( m_nEmitterMode == kCCParticleModeRadius, "Particle Mode should be Radius");
    modeB.endRadiusVar = endRadiusVar;
}

float CCParticleSystem::getEndRadiusVar()
{
    CCAssert( m_nEmitterMode == kCCParticleModeRadius, "Particle Mode should be Radius");
    return modeB.endRadiusVar;
}

void CCParticleSystem::setRotatePerSecond(float degrees)
{
    CCAssert( m_nEmitterMode == kCCParticleModeRadius, "Particle Mode should be Radius");
    modeB.rotatePerSecond = degrees;
}

float CCParticleSystem::getRotatePerSecond()
{
    CCAssert( m_nEmitterMode == kCCParticleModeRadius, "Particle Mode should be Radius");
    return modeB.rotatePerSecond;
}

void CCParticleSystem::setRotatePerSecondVar(float degrees)
{
    CCAssert( m_nEmitterMode == kCCParticleModeRadius, "Particle Mode should be Radius");
    modeB.rotatePerSecondVar = degrees;
}

float CCParticleSystem::getRotatePerSecondVar()
{
    CCAssert( m_nEmitterMode == kCCParticleModeRadius, "Particle Mode should be Radius");
    return modeB.rotatePerSecondVar;
}

bool CCParticleSystem::isActive()
{
    return m_bIsActive;
}

unsigned int CCParticleSystem::getParticleCount()
{
    return m_uParticleCount;
}

float CCParticleSystem::getDuration()
{
    return m_fDuration;
}

void CCParticleSystem::setDuration(float var)
{
    m_fDuration = var;
}

const CCPoint& CCParticleSystem::getSourcePosition()
{
    return m_tSourcePosition;
}

void CCParticleSystem::setSourcePosition(const CCPoint& var)
{
    m_tSourcePosition = var;
}

const CCPoint& CCParticleSystem::getPosVar()
{
    return m_tPosVar;
}

void CCParticleSystem::setPosVar(const CCPoint& var)
{
    m_tPosVar = var;
}

float CCParticleSystem::getLife()
{
    return m_fLife;
}

void CCParticleSystem::setLife(float var)
{
    m_fLife = var;
}

float CCParticleSystem::getLifeVar()
{
    return m_fLifeVar;
}

void CCParticleSystem::setLifeVar(float var)
{
    m_fLifeVar = var;
}

float CCParticleSystem::getAngle()
{
    return m_fAngle;
}

void CCParticleSystem::setAngle(float var)
{
    m_fAngle = var;
}

float CCParticleSystem::getAngleVar()
{
    return m_fAngleVar;
}

void CCParticleSystem::setAngleVar(float var)
{
    m_fAngleVar = var;
}

float CCParticleSystem::getStartSize()
{
    return m_fStartSize;
}

void CCParticleSystem::setStartSize(float var)
{
    m_fStartSize = var;
}

float CCParticleSystem::getStartSizeVar()
{
    return m_fStartSizeVar;
}

void CCParticleSystem::setStartSizeVar(float var)
{
    m_fStartSizeVar = var;
}

float CCParticleSystem::getEndSize()
{
    return m_fEndSize;
}

void CCParticleSystem::setEndSize(float var)
{
    m_fEndSize = var;
}

float CCParticleSystem::getEndSizeVar()
{
    return m_fEndSizeVar;
}

void CCParticleSystem::setEndSizeVar(float var)
{
    m_fEndSizeVar = var;
}

const ccColor4F& CCParticleSystem::getStartColor()
{
    return m_tStartColor;
}

void CCParticleSystem::setStartColor(const ccColor4F& var)
{
    m_tStartColor = var;
}

const ccColor4F& CCParticleSystem::getStartColorVar()
{
    return m_tStartColorVar;
}

void CCParticleSystem::setStartColorVar(const ccColor4F& var)
{
    m_tStartColorVar = var;
}

const ccColor4F& CCParticleSystem::getEndColor()
{
    return m_tEndColor;
}

void CCParticleSystem::setEndColor(const ccColor4F& var)
{
    m_tEndColor = var;
}

const ccColor4F& CCParticleSystem::getEndColorVar()
{
    return m_tEndColorVar;
}

void CCParticleSystem::setEndColorVar(const ccColor4F& var)
{
    m_tEndColorVar = var;
}

float CCParticleSystem::getStartSpin()
{
    return m_fStartSpin;
}

void CCParticleSystem::setStartSpin(float var)
{
    m_fStartSpin = var;
}

float CCParticleSystem::getStartSpinVar()
{
    return m_fStartSpinVar;
}

void CCParticleSystem::setStartSpinVar(float var)
{
    m_fStartSpinVar = var;
}

#ifdef Hunter
float CCParticleSystem::getSleepDuration()
{
	return m_fSleepDuration;
}

void CCParticleSystem::setSleepDuration(float var)
{
	m_fSleepDuration = var;
}

float CCParticleSystem::getSleepDurationVar()
{
	return m_fSleepDurationVar;
}

void CCParticleSystem::setSleepDurationVar(float var)
{
	m_fSleepDurationVar = var;
}

float CCParticleSystem::getVisibleRate()
{
	return m_fVisibleRate;
}

void CCParticleSystem::setVisibleRate(float var)
{
	m_fVisibleRate = var;
}

float CCParticleSystem::getAdvance()
{
	return m_fAdvance;
}

void CCParticleSystem::setAdvance(float var)
{
	m_fAdvance = var;
}

float CCParticleSystem::getDelay()
{
	return m_fDelay;
}

void CCParticleSystem::setDelay(float var)
{
	m_fDelay = var;
}

void CCParticleSystem::delayOver(float dt)
{
	float r = CCRANDOM_0_1();
	if (m_fVisibleRate < r)
	{
		m_bVisible = false;
		m_bIsActive = false;
	}
	else
	{
		m_bVisible = true;
		m_bIsActive = true;
		preWarm(m_fAdvance);
	}
	unschedule(schedule_selector(CCParticleSystem::delayOver));
}

#endif // Hunter
float CCParticleSystem::getEndSpin()
{
    return m_fEndSpin;
}

void CCParticleSystem::setEndSpin(float var)
{
    m_fEndSpin = var;
}
float CCParticleSystem::getEndSpinVar()
{
    return m_fEndSpinVar;
}

void CCParticleSystem::setEndSpinVar(float var)
{
    m_fEndSpinVar = var;
}
#ifdef Hunter
bool CCParticleSystem::getSpinVarSame()
{
	return m_fSpinVarSame;
}

void CCParticleSystem::setSpinVarSame(bool var)
{
	m_fSpinVarSame = var;
}

void CCParticleSystem::updeteDeltaAlpha(tCCParticle* p)
{
	p->frameCounter++;

	if (p->frameCounter % 10 != 0)
	{
		return;
	}
	
	if (p->alphaStageIdx == m_nASLength - 1)
	{
		return;
	}
	alphaStage as = asVector[p->alphaStageIdx];
	if (p->elapsed > as.tPoint)
	{
		p->alphaStageIdx++;
		as = asVector[p->alphaStageIdx];
		p->deltaColor.a = as.deltaAlpha;
	} 
}

void CCParticleSystem::setAlphaStatge(float *array, int length)
{
	asVector.clear();
	for (int i=0; i<length; i++)
	{
		alphaStage stage;
		stage.alpha = array[i*3 + 0] / 255.0f;
		stage.alphaVar = array[i*3 + 1] / 255.0f;
		stage.tPoint = array[i*3 + 2];
		asVector.push_back(stage);
	}
	m_nASLength = asVector.size();
}

#endif // Hunter
float CCParticleSystem::getEmissionRate()
{
    return m_fEmissionRate;
}

void CCParticleSystem::setEmissionRate(float var)
{
    m_fEmissionRate = var;
}

unsigned int CCParticleSystem::getTotalParticles()
{
    return m_uTotalParticles;
}

void CCParticleSystem::setTotalParticles(unsigned int var)
{
    CCAssert( var <= m_uAllocatedParticles, "Particle: resizing particle array only supported for quads");
    m_uTotalParticles = var;
}

ccBlendFunc CCParticleSystem::getBlendFunc()
{
    return m_tBlendFunc;
}

void CCParticleSystem::setBlendFunc(ccBlendFunc blendFunc)
{
    if( m_tBlendFunc.src != blendFunc.src || m_tBlendFunc.dst != blendFunc.dst ) {
        m_tBlendFunc = blendFunc;
        this->updateBlendFunc();
    }
}

bool CCParticleSystem::getOpacityModifyRGB()
{
    return m_bOpacityModifyRGB;
}

void CCParticleSystem::setOpacityModifyRGB(bool bOpacityModifyRGB)
{
    m_bOpacityModifyRGB = bOpacityModifyRGB;
}

tCCPositionType CCParticleSystem::getPositionType()
{
    return m_ePositionType;
}

void CCParticleSystem::setPositionType(tCCPositionType var)
{
    m_ePositionType = var;
}

bool CCParticleSystem::isAutoRemoveOnFinish()
{
    return m_bIsAutoRemoveOnFinish;
}

void CCParticleSystem::setAutoRemoveOnFinish(bool var)
{
    m_bIsAutoRemoveOnFinish = var;
}

int CCParticleSystem::getEmitterMode()
{
    return m_nEmitterMode;
}

void CCParticleSystem::setEmitterMode(int var)
{
    m_nEmitterMode = var;
}


// ParticleSystem - methods for batchNode rendering

CCParticleBatchNode* CCParticleSystem::getBatchNode(void)
{
    return m_pBatchNode;
}

void CCParticleSystem::setBatchNode(CCParticleBatchNode* batchNode)
{
    if( m_pBatchNode != batchNode ) {

        m_pBatchNode = batchNode; // weak reference

        if( batchNode ) {
            //each particle needs a unique index
            for (unsigned int i = 0; i < m_uTotalParticles; i++)
            {
                m_pParticles[i].atlasIndex=i;
            }
        }
    }
}

//don't use a transform matrix, this is faster
void CCParticleSystem::setScale(float s)
{
    m_bTransformSystemDirty = true;
    CCNode::setScale(s);
}

void CCParticleSystem::setRotation(float newRotation)
{
    m_bTransformSystemDirty = true;
    CCNode::setRotation(newRotation);
}

void CCParticleSystem::setScaleX(float newScaleX)
{
    m_bTransformSystemDirty = true;
    CCNode::setScaleX(newScaleX);
}

void CCParticleSystem::setScaleY(float newScaleY)
{
    m_bTransformSystemDirty = true;
    CCNode::setScaleY(newScaleY);
}


NS_CC_END

