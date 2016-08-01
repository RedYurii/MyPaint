// Rectangular.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "Rectangular.h"

void Rectangular::Work(MyCOORD crd, HDC hDC, int thick, COLORREF color) //implementation of virtual function work to allow drawing rectangles
{
	HPEN hPen = CreatePen(PS_SOLID, thick, color);
	HPEN hOldPen = (HPEN)SelectObject(hDC, hPen);
	Rectangle(hDC, crd.mxInit, crd.myInit, crd.mxFinal, crd.myFinal);
	SelectObject(hDC, hOldPen);
	DeleteObject(hPen);
}
