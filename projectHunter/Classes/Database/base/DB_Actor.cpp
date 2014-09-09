#include "DB_Actor.h" 
#include "libGCExtensions/wxSqlite3/sqlite3.h" 
#include "../../utils/Utils.h" 
 
string DB_Actor::dbsrcFileName = "db/actor.db"; 
string DB_Actor::dbdesFileName = "actor.db"; 
string DB_Actor::dbTableName = "sactor"; 
 
sqlite3* DB_Actor::pDB = NULL;//数据库指针 
int DB_Actor::result	=	0; 
char* DB_Actor::errMsg = NULL; 
vector<DB_Actor*> DB_Actor::resultVector ; 
CCArray* DB_Actor::resultArray = NULL; 
CCDictionary* DB_Actor::resultDictionary = NULL; 
 
void DB_Actor::open() 
{ 
	#ifdef WIN32 
	string despath = CCFileUtils::sharedFileUtils()->fullPathForFilename(dbsrcFileName.c_str()) ; 
	#else 
	string despath = CCFileUtils::sharedFileUtils()->getWritablePath()+dbdesFileName; 
	 
	if (!Utils::isFileExist(despath)) 
	{ 
		string srcpath = CCFileUtils::sharedFileUtils()->fullPathForFilename(dbsrcFileName.c_str()) ; 
		Utils::FileCopy( srcpath, despath); 
	} 
	#endif 
	 
	result = sqlite3_open( despath.c_str() , &pDB); 
	CCAssert( result == SQLITE_OK , "打开数据库失败(DB_Actor)"); 
	 
	sqlite3_key(pDB, "1234", 4); 
	 
	resultArray = CCArray::create(); 
	resultArray->retain(); 
	 
	resultDictionary = CCDictionary::create(); 
	resultDictionary->retain(); 
} 
 
void DB_Actor::close() 
{ 
	releaseVector(); 
	 
	CC_SAFE_RELEASE(resultArray); 
	 
	CC_SAFE_RELEASE(resultDictionary); 
	 
	sqlite3_close(pDB); 
} 
 
void DB_Actor::releaseVector() 
{ 
	for (unsigned int i = 0;i < resultVector.size();i ++) 
	{ 
		((DB_Actor*)resultVector.at(i))->release(); 
	} 
	resultVector.clear(); 
} 
 
void DB_Actor::ExcuteQuery(string sqlstr) 
{ 
	releaseVector();//Utils::Unicode2Utf8(Utils::Acsi2WideByte(sqlstr)).c_str() 
	sqlite3_exec( pDB, sqlstr.c_str() , loadRecord, NULL, &errMsg ); 
} 
 
int DB_Actor::loadRecord( void * para, int n_column, char ** column_value, char ** column_name ) 
{ 
	DB_Actor* npc = new DB_Actor(column_value) ; 
	resultVector.push_back(npc) ; 
	resultArray->addObject(npc); 
	resultDictionary->setObject(npc, npc->ID); 
	return 0; 
} 
 
vector<DB_Actor*> DB_Actor::getQueryResult() 
{ 
	return resultVector; 
} 
 
CCArray* DB_Actor::getQueryResultArray() 
{ 
	return resultArray; 
} 
 
CCDictionary* DB_Actor::getQueryResultDictionary() 
{ 
	return resultDictionary; 
} 
 
void DB_Actor::ExcuteQueryNoneResult(string sqlstr) 
{ 
	releaseVector(); 
	sqlite3_exec( pDB, sqlstr.c_str() , NULL, NULL, &errMsg ); 
} 
 
DB_Actor::DB_Actor(char ** column_value) 
{ 
	ID = atoi(column_value[DB_Actor_ID]); 
	name = string(column_value[DB_Actor_name]); 
	resource = string(column_value[DB_Actor_resource]); 
	isPlayer = atoi(column_value[DB_Actor_isPlayer]); 
} 
 
DB_Actor::DB_Actor() 
{ 
	 
} 
 
DB_Actor::~DB_Actor() 
{ 
	 
} 
