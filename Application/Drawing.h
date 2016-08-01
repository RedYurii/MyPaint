#include "stdafx.h"
#include "Element.h"
#ifndef _DRAWWING_
#define _DRAWWING_

class Drawing        //this class represent a drawing. it contains elements of the drawing and couple of methods to work with container
{
	vector<Element> vcEl;
public:
	void AddElement(Element);   //adds new element to container
	void RemoveElement();      //removes an element
	Element GetElement(int);   //get specific element 
	Element GetLastElement();	//get last element 
	void ReDraw(HDC, vector<BaseTool*> vcTool);  //redraw painting 
	void ClearDrawing();				//clear drawing 
	int GetSize();			//get size of container
};

#endif // !_DRAWWING_