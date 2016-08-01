#include "stdafx.h"
#include "Fill.h"

#ifdef FILL_EXPORTS
#define FILL_API __declspec(dllexport)
#else
#define FILL_API __declspec(dllimport)
#endif

extern "C" FILL_API BaseTool* GetTool(LPWSTR szToolName) //returns object of Fill class and initializes szToolName 
{
	wsprintf(szToolName, L"Fill"); 
	return new Fill;
}