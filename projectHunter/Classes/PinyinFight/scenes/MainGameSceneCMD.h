///-------------------------------------------------------------
// FirstSceneCMD.h
// 网络部分相关
///-------------------------------------------------------------
public:
	virtual void sendCMD();
	virtual bool processCMD(BaseDownCMD* pCmd);

	void CMD_Http_RequestVersionCallback(CCObject* pSender);
