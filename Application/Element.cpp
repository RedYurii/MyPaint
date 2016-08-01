#include "stdafx.h"
#include "Element.h"


Element::Element(int ind, int thick, COLORREF color) : iElInd(ind), mnThick(thick), mColor(color)
{
}

Element::Element(const Element& Right) : iElInd(Right.iElInd),  mnThick(Right.mnThick), mColor(Right.mColor), mCrd(Right.mCrd)  //copy constructor
{
}

Element::Element()
{
}

Element::~Element()
{
}

void Element::Work(HDC hDC, vector<BaseTool*> vcTool)     //call virtual function from dll depending on which tool is selected 
{
	SelectObject(hDC, GetStockObject(NULL_BRUSH));
	vcTool[iElInd]->Work(mCrd, hDC, mnThick, mColor);
}

void Element::GetCurCoord(MODE mode, int x, int y)    //get current position of the mouse
{
	if (mode == INIT)				//MODE represents whether starting of final coordinatas of the shape needs to be initialized
	{
		mCrd.mxInit = x;
		mCrd.myInit= y;
	}
	else
	{
		mCrd.mxFinal=x;
		mCrd.myFinal=y;
	}
	
}
