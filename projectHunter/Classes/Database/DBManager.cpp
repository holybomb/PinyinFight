#include "DBManager.h"
#include "../utils/Utils.h"
#include "base/DB_Word.h"
#include "utils/textConvert.h"

USING_NS_CC;

#define GET_DB_MAP(A, b) \
if (m_pDBCache->objectForKey(b) == NULL) \
{ \
	A::open(); \
	char str[100] = {0}; \
	sprintf(str,"select * from %s", A::dbTableName.c_str()); \
	A::ExcuteQuery(str); \
	m_pDBCache->setObject(A::getQueryResultDictionary(), b); \
	A::close(); \
}

#define GET_DB_ARRAY(A, b) \
if (m_pDBCache->objectForKey(b) == NULL) \
{ \
	A::open(); \
	char str[100] = {0}; \
	sprintf(str,"select * from %s", A::dbTableName.c_str()); \
	A::ExcuteQuery(str); \
	m_pDBCache->setObject(A::getQueryResultArray(), b); \
	A::close(); \
}

#define GET_DB_ARRAY_TABLENAME_KEY_INT(A, b, tableName) \
	if (m_pDBCache->objectForKey(b) == NULL) \
{ \
	A::open(); \
	char str[100] = {0}; \
	sprintf(str,"select * from %s", tableName.c_str()); \
	A::ExcuteQuery(str); \
	m_pDBCache->setObject(A::getQueryResultArray(), b); \
	A::close(); \
}

#define GET_DB_ARRAY_TABLENAME(A, b, tableName) \
	if (m_pDBCacheState->objectForKey(b) == NULL) \
{ \
	A::open(); \
	char str[100] = {0}; \
	sprintf(str,"select * from %s", tableName); \
	A::ExcuteQuery(str); \
	m_pDBCacheState->setObject(A::getQueryResultArray(), b); \
	A::close(); \
}

static const int PUBLIC_ERR_ID = 9;

DBManager::DBManager()
	: m_pDBCache(NULL)
{
	m_pDBCache = CCDictionary::create();
	m_pDBCache->retain();

	m_pDBCacheState = CCDictionary::create();
	m_pDBCacheState->retain();
	
}

DBManager::~DBManager()
{
	purgeCachedData();
	CC_SAFE_RELEASE_NULL(m_pDBCache);
	CC_SAFE_RELEASE_NULL(m_pDBCacheState);
}

DBManager* DBManager::sharedDBManager()
{
	static DBManager* s_pDBManager = NULL;
	if (s_pDBManager == NULL)
	{
		s_pDBManager = new DBManager();
	}
	return s_pDBManager;
}

void DBManager::purgeCachedData()
{
	m_pDBCache->removeAllObjects();
	m_pDBCacheState->removeAllObjects();
}

/**
 * 获得系统消息
 */
XTipMsg* DBManager::getTipMsg(int ID)
{
	GET_DB_MAP(XTipMsg, kDBTypeTipMsg);

	CCDictionary* dic = (CCDictionary*)m_pDBCache->objectForKey(kDBTypeTipMsg);
	CCObject* pModel = dic->objectForKey(ID);
	if (pModel == NULL)
	{
		pModel = dic->objectForKey(PUBLIC_ERR_ID);
	}
	return dynamic_cast<XTipMsg*>(pModel);
}


std::string DBManager::getRandomWordByLetter(string Letter,string pWord)
{
	GET_DB_MAP(DB_Word, kDBTypeWord);
	CCDictionary* dic = (CCDictionary*)m_pDBCache->objectForKey(kDBTypeWord);
	CCArray* arr = CCArray::create();
	int wordPos = -1;
	for(int i =0;i<dic->allKeys()->count();i++)
	{
		int key = (dynamic_cast<CCInteger*>(dic->allKeys()->objectAtIndex(i)))->getValue();
		DB_Word* word = dynamic_cast<DB_Word*>(dic->objectForKey(key));
		std::string targetStr;
		if(word)
			targetStr =	(word)->sch; 
		if(targetStr.size()>0)
		{
			string pp = targetStr;
			char p[1000];
			int i;
			for( i=0;i<pp.length();i++)
				p[i] = pp[i];
			p[i] = '\0';
			printf("%s\n",p);
			cout<<p;
			std::string pinyinStr = textConvert::getPinYin(p);//GET_STRING(TEXT_JUQING_SPLASH_BEGIN2);
			std::string _pinyinStr = Utils::getPinyinLetter(pinyinStr);
			if(Letter.compare(_pinyinStr) == 0)
			{
				arr->addObject(CCString::create(p));
				if(pWord.compare(targetStr) == 0)
					wordPos = arr->count()-1;
			}
			CCLog("%d	%s	%s",key,targetStr.c_str(),_pinyinStr.c_str());
		}
	}
	if(arr->count()>0)
	{
		if(arr->count()>1 && wordPos>=0)
			arr->removeObjectAtIndex(wordPos);
		return (dynamic_cast<CCString*>(arr->randomObject()))->m_sString;
	}
	else
		return "";
}
std::string DBManager::checkWordExist(std::string pWord)
{
	string isExistStr = "";
	GET_DB_MAP(DB_Word, kDBTypeWord);
	CCDictionary* dic = (CCDictionary*)m_pDBCache->objectForKey(kDBTypeWord);
	CCDictElement* pObject = NULL;
	CCDICT_FOREACH(dic,pObject)
	{
		DB_Word* word = dynamic_cast<DB_Word*>(pObject->getObject());
		bool isExist = false;
		CCArray* _dbWordArr = CCArray::create();
		int srcLength = word->sch.size();
		for(int i = 0;i<srcLength;i=i+3)
		{
			CCString* c = CCString::create(word->sch.substr(i,3));
			c->retain();
			_dbWordArr->addObject(c);
		}
		srcLength = pWord.size();
		for(int i = 0;i<srcLength;i=i+3)
		{
			CCString* c = CCString::create(pWord.substr(i,3));
			bool isHaveWord = false;
			for(int j =0;j<_dbWordArr->count();j++)
			{
				CCString* s = dynamic_cast<CCString*>(_dbWordArr->objectAtIndex(j));
				CCLog("%s and %s",c->getCString(),s->getCString());
				if(s->m_sString.compare(c->m_sString) == 0)
				{
					isHaveWord = true;
					break;
				}
				else
				{
					isHaveWord = false;
				}
			}
			if(isHaveWord)
				isExist = true;
			else
			{
				isExist = false;
				break;
			}
		}
		if(isExist)
		{
			isExistStr = word->sch;
			break;
		}
		/*if(word->sch.compare(pWord)== 0)
		{
			isExist = true;
			break;
		}*/
	}
	return  isExistStr ;
}
std::string DBManager::getRandomWord()
{
	GET_DB_MAP(DB_Word, kDBTypeWord);
	CCDictionary* dic = (CCDictionary*)m_pDBCache->objectForKey(kDBTypeWord);
	int count = dic->count();
	DB_Word* word = dynamic_cast<DB_Word*>(dic->randomObject());
	if(word)
		return (word)->sch; 
	else
		return "";
}


string DBManager::getWord(int ID)
{
	GET_DB_MAP(DB_Word, kDBTypeWord);
	CCDictionary* dic = (CCDictionary*)m_pDBCache->objectForKey(kDBTypeWord);
	return (dynamic_cast<DB_Word*>(dic->objectForKey(ID)))->sch; 
}

string DBManager::getString(int ID)
{
	GET_DB_MAP(DB_Text, kDBTypeText);
	CCDictionary* dic = (CCDictionary*)m_pDBCache->objectForKey(kDBTypeText);
	return (dynamic_cast<DB_Text*>(dic->objectForKey(ID)))->sch; 
}
