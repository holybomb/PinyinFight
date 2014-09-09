#include "DB_Word.h" 
#include "libGCExtensions/wxSqlite3/sqlite3.h" 
#include "../../utils/Utils.h" 
 
string DB_Word::dbsrcFileName = "db/word.db"; 
string DB_Word::dbdesFileName = "word.db"; 
string DB_Word::dbTableName = "sword"; 
 
sqlite3* DB_Word::pDB = NULL;//数据库指针 
int DB_Word::result	=	0; 
char* DB_Word::errMsg = NULL; 
vector<DB_Word*> DB_Word::resultVector ; 
CCArray* DB_Word::resultArray = NULL; 
CCDictionary* DB_Word::resultDictionary = NULL; 
 
void DB_Word::open() 
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
	CCAssert( result == SQLITE_OK , "打开数据库失败(DB_Word)"); 
	 
	sqlite3_key(pDB, "1234", 4); 
	 
	resultArray = CCArray::create(); 
	resultArray->retain(); 
	 
	resultDictionary = CCDictionary::create(); 
	resultDictionary->retain(); 
} 
 
void DB_Word::close() 
{ 
	releaseVector(); 
	 
	CC_SAFE_RELEASE(resultArray); 
	 
	CC_SAFE_RELEASE(resultDictionary); 
	 
	sqlite3_close(pDB); 
} 
 
void DB_Word::releaseVector() 
{ 
	for (unsigned int i = 0;i < resultVector.size();i ++) 
	{ 
		((DB_Word*)resultVector.at(i))->release(); 
	} 
	resultVector.clear(); 
} 
 
void DB_Word::ExcuteQuery(string sqlstr) 
{ 
	releaseVector();//Utils::Unicode2Utf8(Utils::Acsi2WideByte(sqlstr)).c_str() 
	sqlite3_exec( pDB, sqlstr.c_str() , loadRecord, NULL, &errMsg ); 
} 
 
int DB_Word::loadRecord( void * para, int n_column, char ** column_value, char ** column_name ) 
{ 
	DB_Word* npc = new DB_Word(column_value) ; 
	resultVector.push_back(npc) ; 
	resultArray->addObject(npc); 
	resultDictionary->setObject(npc, npc->ID); 
	return 0; 
} 
 
vector<DB_Word*> DB_Word::getQueryResult() 
{ 
	return resultVector; 
} 
 
CCArray* DB_Word::getQueryResultArray() 
{ 
	return resultArray; 
} 
 
CCDictionary* DB_Word::getQueryResultDictionary() 
{ 
	return resultDictionary; 
} 
 
void DB_Word::ExcuteQueryNoneResult(string sqlstr) 
{ 
	releaseVector(); 
	sqlite3_exec( pDB, sqlstr.c_str() , NULL, NULL, &errMsg ); 
} 
 
DB_Word::DB_Word(char ** column_value) 
{ 
	ID = atoi(column_value[DB_Word_ID]); 
	sch = string(column_value[DB_Word_sch]); 
} 
 
DB_Word::DB_Word() 
{ 
	 
} 
 
DB_Word::~DB_Word() 
{ 
	 
} 
