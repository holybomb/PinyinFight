#ifndef __DB_Text_H__ 
#define __DB_Text_H__ 
 
#include "cocos2d.h" 
 
USING_NS_CC; 
using namespace std; 
 
struct sqlite3; 
class DB_Text : public CCObject 
{ 
	public: 
		DB_Text(char ** column_value); 
		DB_Text(); 
		virtual ~DB_Text(); 
		 
		static sqlite3 *pDB ;//数据库指针 
		static vector<DB_Text*> resultVector ; 
		static CCArray* resultArray; 
		static CCDictionary* resultDictionary; 
		 
		static int result;//sqlite3_exec返回值 
		static char * errMsg;//错误信息 
		 
		static void open() ; 
		static void ExcuteQuery(string sqlstr) ; 
		static int loadRecord( void * para, int n_column, char ** column_value, char ** column_name ) ; 
		 
		static vector<DB_Text*> getQueryResult() ; 
		static CCArray* getQueryResultArray() ; 
		static CCDictionary* getQueryResultDictionary() ; 
		 
		static void ExcuteQueryNoneResult(string sqlstr) ; 
		static void close() ; 
		static void releaseVector(); 
		 
	public: 
		static string dbsrcFileName ; 
		static string dbdesFileName ; 
		static string dbTableName ; 
		static const int DB_Text_ID = 0 ; 
		static const int DB_Text_sch = 1 ; 
		 
	public: 
		int ID ; 
		string sch ; 
}; 
 
#endif // __DB_Text_H__ 
