#ifndef __XTipMsg_H__ 
#define __XTipMsg_H__ 
 
#include "cocos2d.h" 
 
USING_NS_CC; 
using namespace std; 
 
struct sqlite3; 
class XTipMsg : public CCObject 
{ 
	public: 
		XTipMsg(char ** column_value); 
		XTipMsg(); 
		virtual ~XTipMsg(); 
		 
		static sqlite3 *pDB ;//数据库指针 
		static vector<XTipMsg*> resultVector ; 
		static CCArray* resultArray; 
		static CCDictionary* resultDictionary; 
		 
		static int result;//sqlite3_exec返回值 
		static char * errMsg;//错误信息 
		 
		static void open() ; 
		static void ExcuteQuery(string sqlstr) ; 
		static int loadRecord( void * para, int n_column, char ** column_value, char ** column_name ) ; 
		 
		static vector<XTipMsg*> getQueryResult() ; 
		static CCArray* getQueryResultArray() ; 
		static CCDictionary* getQueryResultDictionary() ; 
		 
		static void ExcuteQueryNoneResult(string sqlstr) ; 
		static void close() ; 
		static void releaseVector(); 
		 
	public: 
		static string dbsrcFileName ; 
		static string dbdesFileName ; 
		static string dbTableName ; 
		static const int XTipMsg_ID = 0 ; 
		static const int XTipMsg_skey = 1 ; 
		static const int XTipMsg_Stype = 2 ; 
		static const int XTipMsg_TipMsg = 3 ; 
		 
	public: 
		int ID ; 
		string skey ; 
		int Stype ; 
		string TipMsg ; 
}; 
 
#endif // __XTipMsg_H__ 
