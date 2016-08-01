#include "BaseTool.h"
#include "COORD.h"

#ifndef _ELEMENT_
#define _ELEMENT_


class Element   //this class stores info about element of a drawing (primitive, filled area etc.) and method for drawing particular element
{
	int iElInd;   //index of tool required for an element
	int mnThick;	//pen thickness
	COLORREF mColor;   //color of shape/tool
	MyCOORD mCrd;		//coordinates of the element 
public:
	Element(int, int, COLORREF);	
	Element(const Element & Right);   
	Element();
	~Element();
	void Work(HDC, vector<BaseTool*> vcTool);   //call virtual function from dll depending on which tool is selected 
	void GetCurCoord(MODE,int, int);       //get current position of the mouse
};

#endif // !_ELEMENT_

