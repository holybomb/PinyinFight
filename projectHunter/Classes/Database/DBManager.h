///-------------------------------------------------------------
// DBManager.cpp
// DB管理器
///-------------------------------------------------------------

#ifndef __DB_MANAGER_H__
#define __DB_MANAGER_H__

#include "cocos2d.h"
#include "base/XTipMsg.h"
#include "base/DB_Text.h"


enum kDBType
{
	
	kDBTypeTipMsg,				// 系统提示
	kDBTypeText,				// 文本
	kDBTypeWord,				// 成语
	kDBTypeMax,
};

class DBManager : public cocos2d::CCObject
{
public:
	DBManager();
	virtual ~DBManager();

	static DBManager *sharedDBManager();
	void purgeCachedData();

	/**
	 * 获得系统消息
	 */
	XTipMsg* getTipMsg(int ID);

	/************************************************************************/
	/* 获取文本                                                             */
	/************************************************************************/
	string getString(int ID);

	std::string getWord(int ID);
	std::string getRandomWord();
	std::string getRandomWordByLetter(string Letter,string pWord);
	std::string checkWordExist(std::string pWord);
private:
	cocos2d::CCDictionary* m_pDBCache;				// DB Cache
	cocos2d::CCDictionary* m_pDBCacheState;				// DB Cache
};

#endif // __DB_MANAGER_H__
