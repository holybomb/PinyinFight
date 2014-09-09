public:

	int		m_effectState[ANIM_PLAYER_EFFECT_MAX];
	int		m_effectStateCache[ANIM_PLAYER_EFFECT_MAX];
	bool	isShaderProgramNeedReload();
	void	copyCurEffectInfo2Cache();
	AnimEffectInfo effectInfo;//������������Ч����ʱ�洢֮ǰ�Ĳ���
	void	removeEffectState(int m_type);

	//�Ƿ�ʹ����Ļ����ϵ����Ϊ�ڼ�CCAction��Чʱ����Щ3D��Ч��������animPlayerΪ��Զ���������Ļ�����꣬�����������layer�ġ���ʱ��ֵΪtrue
	bool		useScreenCoordinate;

	bool		isInitShader;

	void	initShaderVertex(const char *vert, const char *frag);
	void	initShaderVertex(CCGLProgram *shader);
	void	reloadShader(int curType);

	bool	isRemoveSoulAndSound();

	///////////////////shader muilt color effect///////////////////////////
	ccColor4F	m_effectMultiColorValue;
	GLuint		m_effectMultiColorLocation; 
	void enableEffectMuiltColor(ccColor4F color);
	void enableEffectMuiltColor(ccColor4F color, float curDieJiaAlpha);
	void disableEffectMuiltColor();
	void setMuiltColor(ccColor4F color);


	///////////////////shader gray effect///////////////////////////
	void enableEffectGray();
	void disableEffectGray();


	///////////////////shader blur effect///////////////////////////
	void enableEffectBlur();
	void disableEffectBlur();


	///////////////////effect Actor Clip////////////////////////////
	#define WATER_WAVE_HIGH_POS_Y	60
	#define WATER_WAVE_HIGH_SCALE	0.8

	#define CLIP_ANIM_NONE			0
	#define CLIP_ANIM_DOWN			1
	#define CLIP_ANIM_DAOYING		2

	int m_clipAnimType;
	//bool isOpenActorWaveClipDown;
	//bool isOpenActorWaveClipDaoYing;
	float isOpenActorWaveClipY;
	void enableEffectActorWaveClip(int clipAnimType, float deepHeight);
	void disableEffectActorWaveClip();


	//////////////////////////////
	GLuint m_effectSandboxParamLocation1;
	GLuint m_effectSandboxParamValue1;
	GLuint m_effectSandboxParamLocation2;
	GLuint m_effectSandboxParamValue2;
	GLuint m_effectSandboxParamLocation3;
	GLuint m_effectSandboxParamValue3;
	void enableEffectSandbox();
	void disableEffectSandbox();


	void updateFx();
