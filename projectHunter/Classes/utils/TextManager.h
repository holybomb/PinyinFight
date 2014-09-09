#ifndef __TEXT_MANAGER_H__
#define __TEXT_MANAGER_H__

#include "../Database/DBManager.h"
#include "../Database/base/DB_Text_const.h"

#define GET_STRING(a) (DBManager::sharedDBManager()->getString(a))
#define GET_WORD(a) (DBManager::sharedDBManager()->getWord(a))

#endif // __TEXT_MANAGER_H__