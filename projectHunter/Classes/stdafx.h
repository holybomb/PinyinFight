// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "defines.h"


#if defined( WIN32 )

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#define _WIN32_DCOM

// 	#include <windows.h>
// 	#include <Vfw.h>
// 	#include <Mmreg.h>
#include <math.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>
#include <memory.h>
#include <string.h>
#include <sys/types.h>
// 	#include <sys/stat.h>
// 	#include <fcntl.h>
#include <io.h>
// 	#include <limits.h>
// 	#include <signal.h>
#include <time.h>
// 	#include "Commctrl.h"
#include <wchar.h>
#include <tchar.h>
#include <iostream>
#endif

#if defined CC_TARGET_OS_IPHONE || defined ANDROID
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <time.h>
#include <sys/time.h>
#include <wchar.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
//#include <malloc/malloc.h>
#include "math.h"
#endif
