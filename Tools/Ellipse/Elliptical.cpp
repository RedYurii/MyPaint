#include "stdafx.h"
#include "Elliptical.h"



void Elliptical::Work(MyCOORD crd, HDC hDC, int thick, COLORREF color) //implementation of virtual function work to allow drawing ellipses
{
	HPEN hPen = CreatePen(PS_SOLID, thick,color);
	HPEN hOldPen = (HPEN)SelectObject(hDC, hPen);
	Ellipse(hDC, crd.mxInit, crd.myInit, crd.mxFinal, crd.myFinal);
	SelectObject(hDC, hOldPen);
	DeleteObject(hPen);
	
}
