#include "$CMD_NAME$.h"
#include "libGCExtensions/wxSqlite3/sqlite3.h" 
#include "../../utils/Utils.h"

$CMD_STATIC$

sqlite3* $CMD_NAME$::pDB = NULL;//数据库指针 
int $CMD_NAME$::result	=	0;
char* $CMD_NAME$::errMsg = NULL;
vector<$CMD_NAME$*> $CMD_NAME$::resultVector ;
CCArray* $CMD_NAME$::resultArray = NULL;
CCDictionary* $CMD_NAME$::resultDictionary = NULL;

void $CMD_NAME$::open() 
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
	CCAssert( result == SQLITE_OK , "打开数据库失败($CMD_NAME$)");
	
	sqlite3_key(pDB, "1234", 4);
	
	resultArray = CCArray::create();
	resultArray->retain();

	resultDictionary = CCDictionary::create();
	resultDictionary->retain();
}

void $CMD_NAME$::close() 
{
	releaseVector();

	CC_SAFE_RELEASE(resultArray);

	CC_SAFE_RELEASE(resultDictionary);

	sqlite3_close(pDB);
}

void $CMD_NAME$::releaseVector()
{
	for (unsigned int i = 0;i < resultVector.size();i ++)
	{
		(($CMD_NAME$*)resultVector.at(i))->release();
	}
	resultVector.clear();
}

void $CMD_NAME$::ExcuteQuery(string sqlstr)
{
	releaseVector();//Utils::Unicode2Utf8(Utils::Acsi2WideByte(sqlstr)).c_str()
	sqlite3_exec( pDB, sqlstr.c_str() , loadRecord, NULL, &errMsg ); 
}

int $CMD_NAME$::loadRecord( void * para, int n_column, char ** column_value, char ** column_name ) 
{ 
	$CMD_NAME$* npc = new $CMD_NAME$(column_value) ;
	resultVector.push_back(npc) ;
	resultArray->addObject(npc);
	resultDictionary->setObject(npc, npc->ID);
	return 0; 
} 

vector<$CMD_NAME$*> $CMD_NAME$::getQueryResult() 
{
	return resultVector;
} 

CCArray* $CMD_NAME$::getQueryResultArray()
{
	return resultArray;
}

CCDictionary* $CMD_NAME$::getQueryResultDictionary()
{
	return resultDictionary;
}

void $CMD_NAME$::ExcuteQueryNoneResult(string sqlstr)
{
	releaseVector();
	sqlite3_exec( pDB, sqlstr.c_str() , NULL, NULL, &errMsg ); 
}

$CMD_NAME$::$CMD_NAME$(char ** column_value)
{
	$CMD_CREATE$
}

$CMD_NAME$::$CMD_NAME$()
{

}

$CMD_NAME$::~$CMD_NAME$()
{

}
