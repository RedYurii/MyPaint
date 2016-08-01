#include "stdafx.h"
#include "Line.h"
#ifdef LINE_EXPORTS
#define LINE_API __declspec(dllexport)
#else
#define LINE_API __declspec(dllimport)
#endif

extern "C" LINE_API BaseTool* GetTool(LPWSTR szToolName)  //returns object of Line class and initializes szToolName 
{
	wsprintf(szToolName, L"Draw line");
	return new Line;
}