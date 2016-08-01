// Fill.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "Fill.h"

void Fill::Work(MyCOORD crd, HDC hDC, int thick, COLORREF color) //implementation of virtual function work to allow filling closed areas with colored brush
{
	HBRUSH hBr = CreateSolidBrush(color);
	HBRUSH hOldBr = (HBRUSH)SelectObject(hDC, hBr);
	COLORREF bgColor = GetPixel(hDC, crd.mxInit, crd.myInit);
	ExtFloodFill(hDC, crd.mxInit, crd.myInit, bgColor, FLOODFILLSURFACE);
	SelectObject(hDC, hOldBr);
	DeleteObject(hBr);
}
