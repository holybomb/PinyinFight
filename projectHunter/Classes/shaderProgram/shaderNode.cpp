#include "shaderNode.h"

const GLchar * AnimPlayerFragmentMainHead =
#include "AnimPlayerFragmentMainHead.h"
const GLchar * AnimPlayerFragmentMainMiddle =
#include "AnimPlayerFragmentMainMiddle.h"
const GLchar * AnimPlayerFragmentMainEnd =
#include "AnimPlayerFragmentMainEnd.h"
const GLchar * fragDengBaiDong =
#include "fragDengBaiDong.h"
const GLchar * fragDieJia =
#include "fragDieJia.h"
const GLchar * fragDieJiaCCS =
#include "fragDieJiaCCS.h"
const GLchar * fragGaussianBlur =
#include "fragGaussianBlur.h"
const GLchar * fragGlow =
#include "fragGlow.h"
const GLchar * fragSeJie =
#include "fragSeJie.h"
const GLchar * fragGray =
#include "fragGray.h"
const GLchar * fragBianse =
#include "fragBianse.h"
const GLchar * fragHue =
#include "fragHue.h"
const GLchar * fragBrightBian =
#include "fragBrightBian.h"
const GLchar * fragHSDY_HUXI =
#include "fragHSDY_HUXI.h"
const GLchar * fragOneColor =
#include "fragOneColor.h"
const GLchar * fragShuiCao =
#include "fragShuiCao.h"
const GLchar * fragShuiDouDong =
#include "fragShuiDouDong.h"
const GLchar * fragWave =
#include "fragWave.h"
const GLchar * fragWaWaBaiDong =
#include "fragWaWaBaiDong.h"
const GLchar * fragBaoJiShadow =
#include "fragBaoJiShadow.h"
const GLchar * fragGrassWave = 
#include "fragGrassWave.h"
const GLchar * fragYunPingTai =
#include "fragYunPingTai.h"

shaderNode::shaderNode()
{
	
}
shaderNode::~shaderNode()
{

}


CCGLProgram *shaderNode::loadShaderVertex(std::string vert, std::string frag)
{
	const GLchar * vertexSource = NULL;
	const GLchar * fragmentSource = NULL;

	if(vert.length() > 0)	
		vertexSource= (GLchar*) CCString::createWithContentsOfFile(CCFileUtils::sharedFileUtils()->fullPathForFilename(vert.c_str()).c_str())->getCString();
	else
		vertexSource = ccPositionTextureColor_vert;

	if(frag.length() > 0)
		fragmentSource = (GLchar*) CCString::createWithContentsOfFile(CCFileUtils::sharedFileUtils()->fullPathForFilename(frag.c_str()).c_str())->getCString();
	else
		fragmentSource = ccPositionTextureColor_frag;


	return loadShaderVertex((GLchar *)vertexSource, (GLchar *)fragmentSource);

}
CCGLProgram *shaderNode::loadShaderVertex(std::string vert, GLchar *frag)
{
	const GLchar * vertexSource = NULL;

	if(vert.length() > 0)	
		vertexSource= (GLchar*) CCString::createWithContentsOfFile(CCFileUtils::sharedFileUtils()->fullPathForFilename(vert.c_str()).c_str())->getCString();
	else
		vertexSource = ccPositionTextureColor_vert;


	return loadShaderVertex((GLchar *)vertexSource, frag);
}
CCGLProgram *shaderNode::loadShaderVertex(GLchar *vert, GLchar *frag)
{
	CCGLProgram *shader = new CCGLProgram();
	shader->autorelease();
	

	shader->initWithVertexShaderByteArray(vert, frag);

	shader->addAttribute(kCCAttributeNamePosition, kCCVertexAttrib_Position);

	//这个就是顶点里面的v_fragmentColor, 所有CCNodeRGBA在绘制之前都会set一下这个顶点，用于做颜色叠加的底色
	//详细参见 CCAtlasNode里面的draw函数
	//所以需要半透明的顶点特效，都需要在gl_fragColor = result * v_fragmentColor一下同底色叠加且设置一下透明色
	shader->addAttribute(kCCAttributeNameColor, kCCVertexAttrib_Color);  
	shader->addAttribute(kCCAttributeNameTexCoord, kCCVertexAttrib_TexCoords);

	shader->link();

	shader->updateUniforms();

	return shader;
}
#define STRING_NULL		std::string("")
CCGLProgram *shaderNode::getNullShader()
{
	return CCShaderCache::sharedShaderCache()->programForKey(kCCShader_PositionTextureColor);
	//return shaderNode::loadShaderVertex(STRING_NULL, STRING_NULL);
}

CCGLProgram *shaderNode::getGrayShader()
{
	return shaderNode::loadShaderVertex(STRING_NULL, std::string("math/basic/AnimationGrayLevel.fsh"));
}


CCGLProgram *shaderNode::getOneColorShader()
{
	return shaderNode::loadShaderVertex(STRING_NULL, std::string("math/basic/AnimationOneColor.fsh"));
}

CCGLProgram *shaderNode::getMultiColorShader()
{
	return shaderNode::loadShaderVertex(STRING_NULL, std::string("math/basic/AnimationDiejia.fsh"));
}


CCGLProgram *shaderNode::getBlurShader()
{
	return shaderNode::loadShaderVertex(STRING_NULL, std::string("math/basic/GaussianBlur.fsh"));
}

CCGLProgram *shaderNode::getSejieShader()
{
	return shaderNode::loadShaderVertex(STRING_NULL, std::string("math/basic/AnimationSeJie.fsh"));
}

CCGLProgram *shaderNode::getGlowShader()
{
	return shaderNode::loadShaderVertex(STRING_NULL, std::string("math/basic/AnimationGlow.fsh"));
}

CCGLProgram *shaderNode::getWaveShader()
{
	return shaderNode::loadShaderVertex(STRING_NULL, std::string("math/wave.fsh"));
}

CCGLProgram *shaderNode::getRadialBlurShader()
{
	return shaderNode::loadShaderVertex(std::string("math/layer_effect/radial_Blur_vectex.vsh"), std::string("math/layer_effect/radial_Blur.fsh"));
}

CCGLProgram *shaderNode::getRadialBlurScaleInShader()
{
	return shaderNode::loadShaderVertex(std::string("math/layer_effect/radialBlurScaleIn.vsh"), std::string("math/layer_effect/radialBlurScaleIn.fsh"));
}

CCGLProgram *shaderNode::getRadialBlurScaleOutShader()
{
	return shaderNode::loadShaderVertex(std::string("math/layer_effect/radialBlurScaleIn.vsh"), std::string("math/layer_effect/radialBlurScaleOut.fsh"));
}

CCGLProgram *shaderNode::getRadialBlurScaleInKeepShader()
{
	return shaderNode::loadShaderVertex(std::string("math/radialBlurScaleIn.vsh"), std::string("math/radialBlurScaleInKeep.fsh"));
}

CCGLProgram *shaderNode::getPolarShader()
{
	return shaderNode::loadShaderVertex(std::string("math/layer_effect/polar.vsh"), std::string("math/layer_effect/polar.fsh"));
}

CCGLProgram *shaderNode::getGaussianBlurShader()
{
	return shaderNode::loadShaderVertex(STRING_NULL, std::string("math/basic/GaussianBlur.fsh"));
}

CCGLProgram *shaderNode::getHuXiShader()
{
	return shaderNode::loadShaderVertex(STRING_NULL, std::string("math/layer_effect/HSDY_hx.fsh")); 
}

CCGLProgram *shaderNode::getWaveHorizontalShader()
{
	return shaderNode::loadShaderVertex(STRING_NULL, std::string("math/layer_effect/waveHorizonal.fsh"));
}

CCGLProgram *shaderNode::getBaiDongShader()
{
	return shaderNode::loadShaderVertex(std::string("math/BaiDong.vsh"), std::string("math/BaiDong.fsh"));
}
CCGLProgram *shaderNode::getScreenSplitHorizontal()
{
	return shaderNode::loadShaderVertex(std::string("math/layer_effect/screenSplitHorizontal.vsh"), std::string("math/layer_effect/screenSplitHorizontal.fsh"));
}

CCGLProgram *shaderNode::getSandboxShader()
{
	return shaderNode::loadShaderVertex(std::string("math/sandbox/sandbox.vsh"), std::string("math/sandbox/sandbox.fsh"));
}


CCGLProgram *shaderNode::getAnimPlayerShader( int *m_effectState, int curType, AnimEffectInfo effectInfo)
{

	GLchar *code = new GLchar[65535];

	sprintf(code, "%s", AnimPlayerFragmentMainHead);

	//先处理移动coord的
	for(int i = 0; i < ANIM_PLAYER_EFFECT_MAX; i++)
	{
		if(m_effectState[i] > 0)
		{
			switch(i)
			{
			case ANIM_PLAYER_EFFECT_WAVE:
				strcat(code, fragWave);
				break;
			case ANIM_PLAYER_EFFECT_SHUI_CAO:
				strcat(code, fragShuiCao);
				break;
			case ANIM_PLAYER_EFFECT_BAIDONG:
				strcat(code, fragWaWaBaiDong);
				break;
			case ANIM_PLAYER_EFFECT_DENGBAIDONG:
				strcat(code, fragDengBaiDong);
				break;
			case ANIM_PLAYER_EFFECT_HU_XI:
				strcat(code, fragHSDY_HUXI);
				break;
			case ANIM_PLAYER_EFFECT_YUNPINGTAI:
				strcat(code, fragYunPingTai);
				break;
			case ANIM_PLAYER_EFFECT_GRASS_WAVE:
				strcat(code, fragGrassWave);
				break;

			}
		}
		
	}

	strcat(code, AnimPlayerFragmentMainMiddle);
	//再处理位移和着色一起的
	for(int i = 0; i < ANIM_PLAYER_EFFECT_MAX; i++)
	{
		if(m_effectState[i] > 0)
		{
			switch(i)
			{
			case ANIM_PLAYER_EFFECT_BLUR:
				strcat(code, fragGaussianBlur);
				break;
			case ANIM_PLAYER_EFFECT_SHUI_DOU_DONG:
				strcat(code, fragShuiDouDong);
				break;
			}
		}

	}

	//最后处理着色
	for(int i = 0; i < ANIM_PLAYER_EFFECT_MAX; i++)
	{
		if(m_effectState[i] > 0)
		{
			switch(i)
			{
			case ANIM_PLAYER_EFFECT_MUILT_COLOR:
				strcat(code, fragDieJia);
				break;
			case ANIM_PLAYER_EFFECT_MUILT_COLOR_CCS:
				strcat(code, fragDieJiaCCS);
				break;
			case ANIM_PLAYER_EFFECT_SEJIE:
				strcat(code, fragSeJie);
				break;
			case ANIM_PLAYER_EFFECT_GLOW:
				strcat(code, fragGlow);
				break;
			case ANIM_PLAYER_EFFECT_ONE_COLOR:
				strcat(code, fragOneColor);
				break;
			case ANIM_PLAYER_EFFECT_GARY:
				strcat(code, fragGray);
				break;
			case ANIM_PLAYER_EFFECT_BIANSE:
				strcat(code, fragBianse);
				break;
			case ANIM_PLAYER_EFFECT_HUE:
				strcat(code, fragHue);
				break;
			case ANIM_PLAYER_EFFECT_BRIGHT_BIAN:
				strcat(code, fragBrightBian);
				break;
			case ANIM_PLAYER_EFFECT_BAOJI_SHADOW:
				strcat(code, fragBaoJiShadow);
				break;
			}
		}

	}

	strcat(code, AnimPlayerFragmentMainEnd);

//	CCLOG("%s",code);
	CCGLProgram *program = loadShaderVertex(std::string("math/AnimPlayerShader.vsh"), code);

	delete[] code;

	for(int i = 0 ; i < ANIM_PLAYER_EFFECT_MAX; i++)
	{
		if(m_effectState[i] > 0 && i != curType)
		{
			switch(i)
			{
			case ANIM_PLAYER_EFFECT_MUILT_COLOR_CCS:
			case ANIM_PLAYER_EFFECT_MUILT_COLOR:
				shaderNode::setUniformLocationWith4f(program, "DieJiaColor", effectInfo.DieJiaColorValue.r, effectInfo.DieJiaColorValue.g, effectInfo.DieJiaColorValue.b, effectInfo.DieJiaColorValue.a); 
				shaderNode::setUniformLocationWith1f(program, "DieJiaCurAlpha", effectInfo.DieJiaCurAlpha); 
				break;
			case ANIM_PLAYER_EFFECT_ONE_COLOR:
				shaderNode::setUniformLocationWith3f(program, "oneColor", effectInfo.oneColorValue.r, effectInfo.oneColorValue.g, effectInfo.oneColorValue.b); 
				break;
			case ANIM_PLAYER_EFFECT_SEJIE:
				{
					shaderNode::setUniformLocationWith3f(program, "SeJieInPut", effectInfo.Sejie_min_in, effectInfo.Sejie_max_in, effectInfo.Sejie_inGamma); 
					shaderNode::setUniformLocationWith2f(program, "SeJieOutPut",  effectInfo.Sejie_min_out, effectInfo.Sejie_max_out);
				}
				break;
			case ANIM_PLAYER_EFFECT_HU_XI:
				{
					shaderNode::setUniformLocationWith1f(program, "u_huxiWaveWidth", effectInfo.HuXi_Width);
				}
				break;
			case ANIM_PLAYER_EFFECT_YUNPINGTAI:
				{
					shaderNode::setUniformLocationWith1f(program, "u_YunPingTaiWaveWidth", effectInfo.HuXi_Width);
				}
				break;
			case ANIM_PLAYER_EFFECT_HUE:
				{
					shaderNode::setUniformLocationWith1f(program, "u_hue", effectInfo.Hue);
				}
				break;
			case ANIM_PLAYER_EFFECT_BIANSE:
				{
					shaderNode::setUniformLocationWith3f(program, "actorBianSe", effectInfo.BianSe_r,effectInfo.BianSe_g,effectInfo.BianSe_b);
				}
				break;
			case ANIM_PLAYER_EFFECT_BRIGHT_BIAN:
				{
					shaderNode::setUniformLocationWith3f(program, "actorBianSe", effectInfo.BianSe_r,effectInfo.BianSe_g,effectInfo.BianSe_b);
				}
				break;
			case ANIM_PLAYER_EFFECT_SHUI_CAO:
				{
					shaderNode::setUniformLocationWith1f(program, "u_ShuiCaoRandom", effectInfo.ShuiCao_Random);
					shaderNode::setUniformLocationWith1f(program, "u_ShuiCaoWaveWidth", effectInfo.ShuiCao_WaveWidth );
					shaderNode::setUniformLocationWith1f(program, "u_ShuiCaoFreq", effectInfo.ShuiCao_Freq);
				}
				break;
			case ANIM_PLAYER_EFFECT_WAVE:
				{
					shaderNode::setUniformLocationWith1f(program, "u_waveRandom", 0);
					shaderNode::setUniformLocationWith1f(program, "u_waveWaveWidth", effectInfo.Wave_Width);
					shaderNode::setUniformLocationWith1f(program, "u_wavefreq", effectInfo.Wave_Freq);
				}
				break;
			case ANIM_PLAYER_EFFECT_DENGBAIDONG:
				{
					shaderNode::setUniformLocationWith1f(program, "u_DengBaiDongRandom", effectInfo.DengBaiDong_Random);
					shaderNode::setUniformLocationWith1f(program, "u_DengBaiDongFreq", effectInfo.DengBaiDong_Freq);
				}
				break;
			case ANIM_PLAYER_EFFECT_BLUR:
				{
					shaderNode::setUniformLocationWith2f(program, "GaussianBlurSize", effectInfo.GaussianBlurSize.x, effectInfo.GaussianBlurSize.y); 
					shaderNode::setUniformLocationWith4f(program, "GaussianSubstract", 0, 0, 0, 0); 
				}
				break;
			case ANIM_PLAYER_EFFECT_BAOJI_SHADOW:
				{
					shaderNode::setUniformLocationWith1f(program, "u_BaoJiShadowAlpha", effectInfo.BaoJiShadowAlpha); 
				}
				break;
			}
		}
	}

	return program;
}

CCGLProgram *shaderNode::getGUIShader()
{
	CCGLProgram *program = shaderNode::loadShaderVertex(std::string("math/GUIShader.vsh"), std::string("math/GUIShader.fsh"));
	return program;
}


CCGLProgram *shaderNode::setUniformLocationWith1i(CCGLProgram *program, std::string varName, int t_value)
{
	program->use();
	GLuint location = glGetUniformLocation( program->getProgram(), varName.c_str());
	program->setUniformLocationWith1i(location, t_value);
	return program;
}
CCGLProgram *shaderNode::setUniformLocationWith1f(CCGLProgram *program, std::string varName, float t_value)
{
	program->use();
	GLuint location = glGetUniformLocation( program->getProgram(), varName.c_str());
	program->setUniformLocationWith1f(location, t_value);
	return program;
}
CCGLProgram *shaderNode::setUniformLocationWith2f(CCGLProgram *program, std::string varName, float v1, float v2)
{
	program->use();
	GLuint location = glGetUniformLocation( program->getProgram(), varName.c_str());
	program->setUniformLocationWith2f(location, v1, v2);
	return program;
}
CCGLProgram *shaderNode::setUniformLocationWith3f(CCGLProgram *program, std::string varName, float v1, float v2, float v3)
{
	program->use();
	GLuint location = glGetUniformLocation( program->getProgram(), varName.c_str());
	program->setUniformLocationWith3f(location, v1, v2, v3);
	return program;
}
CCGLProgram *shaderNode::setUniformLocationWith4f(CCGLProgram *program, std::string varName, float v1, float v2, float v3, float v4)
{
	program->use();
	GLuint location = glGetUniformLocation( program->getProgram(), varName.c_str());
	program->setUniformLocationWith4f(location, v1, v2, v3, v4);
	return program;
}