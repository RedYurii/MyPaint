// Erase.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "Erase.h"


void Erase::Work(MyCOORD crd, HDC hDC, int thick, COLORREF color) //implementation of virtual function work to allow erasing selected area (filling it with white brush)
{

	HBRUSH hBr = CreateSolidBrush(RGB(255, 255, 255));
	HPEN hPen = CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
	HPEN hOldPen = (HPEN)SelectObject(hDC, hPen);
	HBRUSH hOldBr = (HBRUSH)SelectObject(hDC, hBr);
	Rectangle(hDC, crd.mxInit, crd.myInit, crd.mxFinal, crd.myFinal);
	SelectObject(hDC, hOldBr);
	SelectObject(hDC, hOldPen);
	DeleteObject(hPen);
	DeleteObject(hBr);
}
