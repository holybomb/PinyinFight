#ifndef __$CMD_NAME$_H__
#define __$CMD_NAME$_H__

#include "cocos2d.h"

USING_NS_CC;
using namespace std;

struct sqlite3;
class $CMD_NAME$ : public CCObject
{
public:    
    $CMD_NAME$(char ** column_value);
	$CMD_NAME$();
	virtual ~$CMD_NAME$();
	
	static sqlite3 *pDB ;//数据库指针 
	static vector<$CMD_NAME$*> resultVector ;
	static CCArray* resultArray;
	static CCDictionary* resultDictionary;

	static int result;//sqlite3_exec返回值 
	static char * errMsg;//错误信息 

	static void open() ;
	static void ExcuteQuery(string sqlstr) ;
	static int loadRecord( void * para, int n_column, char ** column_value, char ** column_name ) ;

	static vector<$CMD_NAME$*> getQueryResult() ;
	static CCArray* getQueryResultArray() ; 
	static CCDictionary* getQueryResultDictionary() ; 

	static void ExcuteQueryNoneResult(string sqlstr) ;
	static void close() ;
	static void releaseVector();
	
public:
	$CMD_STATIC$
	
public:
	$CMD_BODY$
};

#endif // __$CMD_NAME$_H__
