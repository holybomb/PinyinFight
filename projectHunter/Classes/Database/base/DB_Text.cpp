#include "DB_Text.h" 
#include "libGCExtensions/wxSqlite3/sqlite3.h" 
#include "../../utils/Utils.h" 
 
string DB_Text::dbsrcFileName = "db/text.db"; 
string DB_Text::dbdesFileName = "text.db"; 
string DB_Text::dbTableName = "stext"; 
 
sqlite3* DB_Text::pDB = NULL;//数据库指针 
int DB_Text::result	=	0; 
char* DB_Text::errMsg = NULL; 
vector<DB_Text*> DB_Text::resultVector ; 
CCArray* DB_Text::resultArray = NULL; 
CCDictionary* DB_Text::resultDictionary = NULL; 
 
void DB_Text::open() 
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
	CCAssert( result == SQLITE_OK , "打开数据库失败(DB_Text)"); 
	 
	sqlite3_key(pDB, "1234", 4); 
	 
	resultArray = CCArray::create(); 
	resultArray->retain(); 
	 
	resultDictionary = CCDictionary::create(); 
	resultDictionary->retain(); 
} 
 
void DB_Text::close() 
{ 
	releaseVector(); 
	 
	CC_SAFE_RELEASE(resultArray); 
	 
	CC_SAFE_RELEASE(resultDictionary); 
	 
	sqlite3_close(pDB); 
} 
 
void DB_Text::releaseVector() 
{ 
	for (unsigned int i = 0;i < resultVector.size();i ++) 
	{ 
		((DB_Text*)resultVector.at(i))->release(); 
	} 
	resultVector.clear(); 
} 
 
void DB_Text::ExcuteQuery(string sqlstr) 
{ 
	releaseVector();//Utils::Unicode2Utf8(Utils::Acsi2WideByte(sqlstr)).c_str() 
	sqlite3_exec( pDB, sqlstr.c_str() , loadRecord, NULL, &errMsg ); 
} 
 
int DB_Text::loadRecord( void * para, int n_column, char ** column_value, char ** column_name ) 
{ 
	DB_Text* npc = new DB_Text(column_value) ; 
	resultVector.push_back(npc) ; 
	resultArray->addObject(npc); 
	resultDictionary->setObject(npc, npc->ID); 
	return 0; 
} 
 
vector<DB_Text*> DB_Text::getQueryResult() 
{ 
	return resultVector; 
} 
 
CCArray* DB_Text::getQueryResultArray() 
{ 
	return resultArray; 
} 
 
CCDictionary* DB_Text::getQueryResultDictionary() 
{ 
	return resultDictionary; 
} 
 
void DB_Text::ExcuteQueryNoneResult(string sqlstr) 
{ 
	releaseVector(); 
	sqlite3_exec( pDB, sqlstr.c_str() , NULL, NULL, &errMsg ); 
} 
 
DB_Text::DB_Text(char ** column_value) 
{ 
	ID = atoi(column_value[DB_Text_ID]); 
	sch = string(column_value[DB_Text_sch]); 
} 
 
DB_Text::DB_Text() 
{ 
	 
} 
 
DB_Text::~DB_Text() 
{ 
	 
} 
