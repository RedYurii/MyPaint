#include "COORD.h"
#ifndef _BASETOOL_
#define _BASETOOL_

class BaseTool
{
public:
	virtual void Work(MyCOORD, HDC, int, COLORREF)=0;
};

#endif // !_BASETOOL_
