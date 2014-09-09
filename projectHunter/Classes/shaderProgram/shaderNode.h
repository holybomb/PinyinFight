#ifndef __SHADER_NODE_H__
#define __SHADER_NODE_H__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "AnimPlayerEffectDefine.h"
USING_NS_CC_EXT;
USING_NS_CC;
using namespace std;
class shaderNode : public CCObject
{
public:
	shaderNode();
	~shaderNode();

	//load shader顶点
	static CCGLProgram *loadShaderVertex(std::string vert, std::string frag);
	static CCGLProgram *loadShaderVertex(GLchar *vert, GLchar *frag);
	static CCGLProgram *loadShaderVertex(std::string vert, GLchar *frag);
	static CCGLProgram *setUniformLocationWith1i(CCGLProgram *program, std::string varName, int t_value);
	static CCGLProgram *setUniformLocationWith1f(CCGLProgram *program, std::string varName, float t_value);
	static CCGLProgram *setUniformLocationWith2f(CCGLProgram *program, std::string varName, float v1, float v2);
	static CCGLProgram *setUniformLocationWith3f(CCGLProgram *program, std::string varName, float v1, float v2, float v3);
	static CCGLProgram *setUniformLocationWith4f(CCGLProgram *program, std::string varName, float v1, float v2, float v3, float v4);

	static CCGLProgram *getNullShader();

	static CCGLProgram *getGrayShader();
	static CCGLProgram *getOneColorShader();
	static CCGLProgram *getMultiColorShader();
	static CCGLProgram *getBlurShader();
	static CCGLProgram *getSejieShader();
	static CCGLProgram *getGlowShader();
	static CCGLProgram *getWaveShader();
	static CCGLProgram *getRadialBlurShader();
	static CCGLProgram *getRadialBlurScaleInShader();
	static CCGLProgram *getRadialBlurScaleInKeepShader();
	static CCGLProgram *getRadialBlurScaleOutShader();
	static CCGLProgram *getPolarShader();
	static CCGLProgram *getGaussianBlurShader();
	static CCGLProgram *getHuXiShader();
	static CCGLProgram *getWaveHorizontalShader();
	static CCGLProgram *getBaiDongShader();
	static CCGLProgram *getAnimPlayerShader(int *m_effectState, int curType, AnimEffectInfo effectInfo);
	static CCGLProgram *getGUIShader();
	static CCGLProgram *getScreenSplitHorizontal();
	static CCGLProgram *getSandboxShader();


};
#endif