#include "stdafx.h"
#include "Erase.h"
#ifdef ERASE_EXPORTS
#define ERASE_API __declspec(dllexport)
#else
#define ERASE_API __declspec(dllimport)
#endif

extern "C" ERASE_API BaseTool* GetTool(LPWSTR szToolName)  //returns object of Erase class and initializes szToolName 
{
	wsprintf(szToolName, L"Erase");
	return new Erase;
}