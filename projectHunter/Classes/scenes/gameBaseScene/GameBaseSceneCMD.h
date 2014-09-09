///-------------------------------------------------------------
// GameBaseSceneCMD.h
// 公共网络部分相关
///-------------------------------------------------------------
public:
	virtual void initCMD();
	virtual void destroyCMD();
	virtual void sendCMD();
	virtual bool processCMD(BaseDownCMD* pCmd);
	virtual void processCacheCMD(BaseDownCMD* pCmd);

	LONG_TYPE m_Ping;

	void CMD_LoginToServerCallback(CCObject* pSender);
	
	// 计算客户端ping值
	void CMD_PingCallback(CCObject* pSender);

	// 推送当前玩家的金钱 & 元宝
	void CMD_PushCurMoneyCallback(CCObject* pSender);
	// 推送当前玩家的经验
	void CMD_PushCurExpCallback(CCObject* pSender);

	/**
	 * 调用：监听
	 * 功能：升级
	 */
	void CMD_ShowLevelUpCallback(CCObject* pSender);

	/**
	 * 调用：监听
	 * 功能：服务器公告！
	 */
	void CMD_TOWN_ServerSayCallback(CCObject* pSender);

	/**
	 * 调用：监听
	 * 功能：客户端显示提示
	 */
	void CMD_ShowMessageCallback(CCObject* pSender);

	/**
	 * 调用：监听
	 * 功能：聊天信息的更新
	 */
	void CMD_TOWN_OppoPlayerSayCallback(CCObject* pSender);

	/**
	 * 调用：标准
	 * 功能：通过UUID返回玩家的状态
	 */
	void CMD_TOWN_InquiryPlayerCallback(CCObject* pSender);

	/**
	 * 调用：监听
	 * 功能：好友列表状态的更新
	 */
	void CMD_FRIEND_reFlashCallback(CCObject* pSender);