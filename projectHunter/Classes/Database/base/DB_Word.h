#ifndef __DB_Word_H__ 
#define __DB_Word_H__ 
 
#include "cocos2d.h" 
 
USING_NS_CC; 
using namespace std; 
 
struct sqlite3; 
class DB_Word : public CCObject 
{ 
	public: 
		DB_Word(char ** column_value); 
		DB_Word(); 
		virtual ~DB_Word(); 
		 
		static sqlite3 *pDB ;//数据库指针 
		static vector<DB_Word*> resultVector ; 
		static CCArray* resultArray; 
		static CCDictionary* resultDictionary; 
		 
		static int result;//sqlite3_exec返回值 
		static char * errMsg;//错误信息 
		 
		static void open() ; 
		static void ExcuteQuery(string sqlstr) ; 
		static int loadRecord( void * para, int n_column, char ** column_value, char ** column_name ) ; 
		 
		static vector<DB_Word*> getQueryResult() ; 
		static CCArray* getQueryResultArray() ; 
		static CCDictionary* getQueryResultDictionary() ; 
		 
		static void ExcuteQueryNoneResult(string sqlstr) ; 
		static void close() ; 
		static void releaseVector(); 
		 
	public: 
		static string dbsrcFileName ; 
		static string dbdesFileName ; 
		static string dbTableName ; 
		static const int DB_Word_ID = 0 ; 
		static const int DB_Word_sch = 1 ; 
		 
	public: 
		int ID ; 
		string sch ; 
}; 
 
#endif // __DB_Word_H__ 
