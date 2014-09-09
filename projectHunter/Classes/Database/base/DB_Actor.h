#ifndef __DB_Actor_H__ 
#define __DB_Actor_H__ 
 
#include "cocos2d.h" 
 
USING_NS_CC; 
using namespace std; 
 
struct sqlite3; 
class DB_Actor : public CCObject 
{ 
	public: 
		DB_Actor(char ** column_value); 
		DB_Actor(); 
		virtual ~DB_Actor(); 
		 
		static sqlite3 *pDB ;//数据库指针 
		static vector<DB_Actor*> resultVector ; 
		static CCArray* resultArray; 
		static CCDictionary* resultDictionary; 
		 
		static int result;//sqlite3_exec返回值 
		static char * errMsg;//错误信息 
		 
		static void open() ; 
		static void ExcuteQuery(string sqlstr) ; 
		static int loadRecord( void * para, int n_column, char ** column_value, char ** column_name ) ; 
		 
		static vector<DB_Actor*> getQueryResult() ; 
		static CCArray* getQueryResultArray() ; 
		static CCDictionary* getQueryResultDictionary() ; 
		 
		static void ExcuteQueryNoneResult(string sqlstr) ; 
		static void close() ; 
		static void releaseVector(); 
		 
	public: 
		static string dbsrcFileName ; 
		static string dbdesFileName ; 
		static string dbTableName ; 
		static const int DB_Actor_ID = 0 ; 
		static const int DB_Actor_name = 1 ; 
		static const int DB_Actor_resource = 2 ; 
		static const int DB_Actor_isPlayer = 3 ; 
		 
	public: 
		int ID ; 
		string name ; 
		string resource ; 
		int isPlayer ; 
}; 
 
#endif // __DB_Actor_H__ 
