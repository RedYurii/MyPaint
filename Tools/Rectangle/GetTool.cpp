#include "stdafx.h"
#ifdef RECTANGULAR_EXPORTS
#define RECTANGULAR_API __declspec(dllexport)
#else
#define RECTANGULAR_API __declspec(dllimport)
#endif
#include "Rectangular.h"

extern "C" RECTANGULAR_API BaseTool* GetTool(LPWSTR szToolName)  //returns object of Rectangular class and initializes szToolName 
{
	wsprintf(szToolName, L"Draw rectangle");
	return new Rectangular;
}