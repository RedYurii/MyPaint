#include "COORD.h"
#ifndef _BASETOOL_
#define _BASETOOL_

class BaseTool  //abstract class BaseTool is needed to make application flexible in the way that it could be extended with new tools (with new dlls) in the future without recompiling 
{
public:
	virtual void Work(MyCOORD, HDC, int, COLORREF)=0;   //pure virtual function (it's implementation will be inside child classes loaded from dlls)
};

#endif // !_BASETOOL_
