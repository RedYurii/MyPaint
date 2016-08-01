#include "stdafx.h"
#ifdef ELLIPSE_EXPORTS
#define ELLIPTICAL_API __declspec(dllexport)
#else
#define ELLIPTICAL_API __declspec(dllimport)
#endif
#include "Elliptical.h"

extern "C" ELLIPTICAL_API BaseTool* GetTool(LPWSTR szToolName)  //returns object of Elliptical class and initializes szToolName 
{
	wsprintf(szToolName, L"Draw ellipse");
	return new Elliptical;
}