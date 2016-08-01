// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define _CRT_SECURE_NO_WARNINGS

#define WIN32_LEAN_AND_MEAN   
#define MAX_SYMBOLS 100
// Windows Header Files:
#include <windows.h>

// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <vector>
#include <crtdbg.h>
#include <Shlwapi.h>
#include <commdlg.h>

using namespace std;
// TODO: reference additional headers your program requires here

enum MODE { INIT, FINAL };