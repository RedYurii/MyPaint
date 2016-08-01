// Line.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "Line.h"

void Line::Work(MyCOORD crd, HDC hDC, int thick, COLORREF color)  //implementation of virtual function work to allow drawing lines
{
	HPEN hPen = CreatePen(PS_SOLID, thick, color);
	HPEN hOldPen = (HPEN)SelectObject(hDC, hPen);
	MoveToEx(hDC, crd.mxInit, crd.myInit, NULL);
	LineTo(hDC, crd.mxFinal, crd.myFinal);
	SelectObject(hDC, hOldPen);
	DeleteObject(hPen);
}
