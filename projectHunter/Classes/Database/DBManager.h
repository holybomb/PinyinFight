///-------------------------------------------------------------
// DBManager.cpp
// DB������
///-------------------------------------------------------------

#ifndef __DB_MANAGER_H__
#define __DB_MANAGER_H__

#include "cocos2d.h"
#include "base/XTipMsg.h"
#include "base/DB_Text.h"


enum kDBType
{
	
	kDBTypeTipMsg,				// ϵͳ��ʾ
	kDBTypeText,				// �ı�
	kDBTypeWord,				// ����
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
	 * ���ϵͳ��Ϣ
	 */
	XTipMsg* getTipMsg(int ID);

	/************************************************************************/
	/* ��ȡ�ı�                                                             */
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
