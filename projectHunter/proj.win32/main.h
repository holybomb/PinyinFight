#ifndef __MAIN_H__
#define __MAIN_H__

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
//#define _DEBUG1

#ifdef _DEBUG1
#define NEW_DBG new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define _CRTDBG_MAP_ALLOC
#include<crtdbg.h>
#endif

// Windows Header Files:
#include <windows.h>
#include <tchar.h>

// C RunTime Header Files
#include "CCStdC.h"

#endif    // __MAIN_H__
