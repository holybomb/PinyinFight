#include "XTipMsg.h" 
#include "libGCExtensions/wxSqlite3/sqlite3.h" 
#include "../../utils/Utils.h" 
 
string XTipMsg::dbsrcFileName = "db/tipmsg.db"; 
string XTipMsg::dbdesFileName = "tipmsg.db"; 
string XTipMsg::dbTableName = "tipmsg"; 
 
sqlite3* XTipMsg::pDB = NULL;//数据库指针 
int XTipMsg::result	=	0; 
char* XTipMsg::errMsg = NULL; 
vector<XTipMsg*> XTipMsg::resultVector ; 
CCArray* XTipMsg::resultArray = NULL; 
CCDictionary* XTipMsg::resultDictionary = NULL; 
 
void XTipMsg::open() 
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
	CCAssert( result == SQLITE_OK , "打开数据库失败(XTipMsg)"); 
	 
	sqlite3_key(pDB, "1234", 4); 
	 
	resultArray = CCArray::create(); 
	resultArray->retain(); 
	 
	resultDictionary = CCDictionary::create(); 
	resultDictionary->retain(); 
} 
 
void XTipMsg::close() 
{ 
	releaseVector(); 
	 
	CC_SAFE_RELEASE(resultArray); 
	 
	CC_SAFE_RELEASE(resultDictionary); 
	 
	sqlite3_close(pDB); 
} 
 
void XTipMsg::releaseVector() 
{ 
	for (unsigned int i = 0;i < resultVector.size();i ++) 
	{ 
		((XTipMsg*)resultVector.at(i))->release(); 
	} 
	resultVector.clear(); 
} 
 
void XTipMsg::ExcuteQuery(string sqlstr) 
{ 
	releaseVector();//Utils::Unicode2Utf8(Utils::Acsi2WideByte(sqlstr)).c_str() 
	sqlite3_exec( pDB, sqlstr.c_str() , loadRecord, NULL, &errMsg ); 
} 
 
int XTipMsg::loadRecord( void * para, int n_column, char ** column_value, char ** column_name ) 
{ 
	XTipMsg* npc = new XTipMsg(column_value) ; 
	resultVector.push_back(npc) ; 
	resultArray->addObject(npc); 
	resultDictionary->setObject(npc, npc->ID); 
	return 0; 
} 
 
vector<XTipMsg*> XTipMsg::getQueryResult() 
{ 
	return resultVector; 
} 
 
CCArray* XTipMsg::getQueryResultArray() 
{ 
	return resultArray; 
} 
 
CCDictionary* XTipMsg::getQueryResultDictionary() 
{ 
	return resultDictionary; 
} 
 
void XTipMsg::ExcuteQueryNoneResult(string sqlstr) 
{ 
	releaseVector(); 
	sqlite3_exec( pDB, sqlstr.c_str() , NULL, NULL, &errMsg ); 
} 
 
XTipMsg::XTipMsg(char ** column_value) 
{ 
	ID = atoi(column_value[XTipMsg_ID]); 
	skey = string(column_value[XTipMsg_skey]); 
	Stype = atoi(column_value[XTipMsg_Stype]); 
	TipMsg = string(column_value[XTipMsg_TipMsg]); 
} 
 
XTipMsg::XTipMsg() 
{ 
	 
} 
 
XTipMsg::~XTipMsg() 
{ 
	 
} 
