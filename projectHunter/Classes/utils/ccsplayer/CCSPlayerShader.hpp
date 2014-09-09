
//特效部分
void CCSPlayer::removeEffectState(int m_type)
{
	m_effectState[m_type] = 0;
	if(m_type != ANIM_PLAYER_EFFECT_ACTORWAVECLIP)
		reloadShader(-1);
}
//初始化
void CCSPlayer::initShaderVertex(const char *vert, const char *frag)
{
	CCGLProgram* pProgram = shaderNode::loadShaderVertex(vert, frag);
	this->getCCSNode()->setShaderProgram(pProgram);
	//pProgram->release();
}
void CCSPlayer::initShaderVertex(CCGLProgram *shader)
{
	this->getCCSNode()->setShaderProgram(shader);
	//shader->release();
}
void CCSPlayer::reloadShader(int curType)
{
	if(isShaderProgramNeedReload())
	{
		initShaderVertex(shaderNode::getAnimPlayerShader(m_effectState, curType, effectInfo));
		copyCurEffectInfo2Cache();
	}
}
//检测是否和上一次一致
bool CCSPlayer::isShaderProgramNeedReload()
{
	int needReload = false;
	for(int i = 0 ; i < ANIM_PLAYER_EFFECT_MAX; i++)
	{
		if(m_effectStateCache[i] != m_effectState[i])
			needReload = true;
	}

	return needReload;
}
void CCSPlayer::copyCurEffectInfo2Cache()
{
	for(int i = 0 ; i < ANIM_PLAYER_EFFECT_MAX; i++)
	{
		m_effectStateCache[i] = m_effectState[i];
	}

}
bool CCSPlayer::isRemoveSoulAndSound()
{
#ifdef ENABLE_CHEAT
	if(Cheat::sharedCheat()->isRemoveSoulAndSound)
		return true;
#endif

	return false;
}


//////////////////////////////////////////////////////////////////////////shader muilt color effect///////////////////////////
void CCSPlayer::enableEffectMuiltColor(ccColor4F color)
{

	enableEffectMuiltColor(color, 1.0);
}

void CCSPlayer::enableEffectMuiltColor(ccColor4F color, float curDieJiaAlpha)
{

	if (isRemoveSoulAndSound() || (m_effectState[ANIM_PLAYER_EFFECT_MUILT_COLOR_CCS]))
		return;


	m_effectState[ANIM_PLAYER_EFFECT_MUILT_COLOR_CCS] = 1;

	reloadShader(ANIM_PLAYER_EFFECT_MUILT_COLOR_CCS);
	m_effectMultiColorValue = color;
	m_effectMultiColorLocation = glGetUniformLocation( getCCSNode()->getShaderProgram()->getProgram(), "DieJiaColor");
	shaderNode::setUniformLocationWith4f(getCCSNode()->getShaderProgram(), "DieJiaColor", color.r, color.g, color.b, color.a); 
	shaderNode::setUniformLocationWith1f(getCCSNode()->getShaderProgram(), "DieJiaCurAlpha", curDieJiaAlpha); 
	effectInfo.DieJiaColorValue = color;
	effectInfo.DieJiaCurAlpha = curDieJiaAlpha;
}
void CCSPlayer::disableEffectMuiltColor()
{
	shaderNode::setUniformLocationWith4f(getCCSNode()->getShaderProgram(), "DieJiaColor", 0, 0, 0, 0); 
	effectInfo.DieJiaColorValue = ccc4f(0,0,0,0);
	effectInfo.DieJiaCurAlpha = 1.0;
	removeEffectState(ANIM_PLAYER_EFFECT_MUILT_COLOR_CCS);
}

void CCSPlayer::setMuiltColor(ccColor4F color)
{
	m_effectMultiColorValue = color;


	//getCCSNode()->getShaderProgram()->setUniformLocationWith4f(m_effectMultiColorLocation, color.r, color.g, color.b, color.a);
}


void CCSPlayer::enableEffectGray()
{
	if(isRemoveSoulAndSound() || (m_effectState[ANIM_PLAYER_EFFECT_GARY] == 1 ))
		return;


	m_effectState[ANIM_PLAYER_EFFECT_GARY] = 1;
	reloadShader(ANIM_PLAYER_EFFECT_GARY);
}
void CCSPlayer::disableEffectGray()
{
	removeEffectState(ANIM_PLAYER_EFFECT_GARY);
}




//////////////////////////////////////////////////////////////////////////Blur code
//set Blur部分代码
void CCSPlayer::enableEffectBlur()
{
	if(isRemoveSoulAndSound() || (m_effectState[ANIM_PLAYER_EFFECT_BLUR]))
		return;


	m_effectState[ANIM_PLAYER_EFFECT_BLUR] = 1;
	reloadShader(ANIM_PLAYER_EFFECT_BLUR);

	//CHECK_GL_ERROR_DEBUG();
	CCPoint blur_;
	//float	blurRadialStrength;//径向模糊的半径参数
	GLfloat    sub_[4];
	//CCSize s = sprite->getTexture()->getContentSizeInPixels();
	blur_ = ccp(1/1024.0f, 1/1024.0f);
	sub_[0] = sub_[1] = sub_[2] = sub_[3] = 0;
	blur_ = ccpMult(blur_,1.1f);

	shaderNode::setUniformLocationWith2f(getCCSNode()->getShaderProgram(), "GaussianBlurSize", blur_.x, blur_.y); 
	shaderNode::setUniformLocationWith4f(getCCSNode()->getShaderProgram(), "GaussianSubstract", sub_[0], sub_[1], sub_[2], sub_[3]); 
	effectInfo.GaussianBlurSize = blur_;
}
void CCSPlayer::disableEffectBlur()
{
	removeEffectState(ANIM_PLAYER_EFFECT_BLUR);
}


void CCSPlayer::enableEffectActorWaveClip(int clipAnimType, float deepHeight)
{
	if(isRemoveSoulAndSound() || (m_effectState[ANIM_PLAYER_EFFECT_ACTORWAVECLIP]))
		return;

	m_effectState[ANIM_PLAYER_EFFECT_ACTORWAVECLIP] = 1;
	m_clipAnimType = clipAnimType;
	isOpenActorWaveClipY = deepHeight;
}
void CCSPlayer::disableEffectActorWaveClip()
{
	removeEffectState(ANIM_PLAYER_EFFECT_ACTORWAVECLIP);
	m_clipAnimType = CLIP_ANIM_NONE;
	isOpenActorWaveClipY = 0;
}



void CCSPlayer::enableEffectSandbox()
{
	initShaderVertex(shaderNode::getSandboxShader());

	//shaderNode::setUniformLocationWith1f(getShaderProgram(), "u_BaiDongRandom", 5 * CCRANDOM_MINUS1_1());

}
void CCSPlayer::disableEffectSandbox()
{
	initShaderVertex(shaderNode::getNullShader());
}

void CCSPlayer::updateFx()
{

	if(this->getCCSNode()->getShaderProgram())
		CC_INCREMENT_SHADER(1);

	//enableEffectMuiltColor(ccc4f(1.0f,0,0,0.5f));
	if (!m_effectState[ANIM_PLAYER_EFFECT_WAVE] && !m_effectState[ANIM_PLAYER_EFFECT_MUILT_COLOR_CCS]
	&& !m_effectState[ANIM_PLAYER_EFFECT_SHUI_CAO] && !m_effectState[ANIM_PLAYER_EFFECT_HU_XI] && !m_effectState[ANIM_PLAYER_EFFECT_YUNPINGTAI]
	&& !m_effectState[ANIM_PLAYER_EFFECT_BAIDONG] && !m_effectState[ANIM_PLAYER_EFFECT_BAOJI_SHADOW] && !m_effectState[ANIM_PLAYER_EFFECT_DENGBAIDONG] && !m_effectState[ANIM_PLAYER_EFFECT_SHUI_DOU_DONG])
		return;


	ccGLEnableVertexAttribs(kCCVertexAttribFlag_PosColorTex );
	getCCSNode()->getShaderProgram()->use();
	getCCSNode()->getShaderProgram()->setUniformsForBuiltins();

	if (m_effectState[ANIM_PLAYER_EFFECT_MUILT_COLOR_CCS])
	{

		getCCSNode()->getShaderProgram()->setUniformLocationWith4f(m_effectMultiColorLocation, m_effectMultiColorValue.r, m_effectMultiColorValue.g, m_effectMultiColorValue.b, m_effectMultiColorValue.a);

		ccGLBindTexture2D( getTexture()->getName());
	} 





}

