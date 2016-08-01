#include "stdafx.h"
#include "Drawing.h"

void Drawing::AddElement(Element el)  //adds new element to container
{
	vcEl.push_back(el); 
}

void Drawing::RemoveElement()   //removes an element
{
	if(vcEl.size() != 0)   //protection from going out of vector range
		vcEl.pop_back();
}

Element Drawing::GetElement(int i)   //get specific element 
{
	return vcEl[i];
}

Element Drawing::GetLastElement()  //get last element 
{
	return vcEl.back();
}

void Drawing::ReDraw(HDC hDC, vector<BaseTool*> vcTool)  //redraw all elemets of the drawing
{
	vector<Element>::iterator iter;
	for (iter = vcEl.begin(); iter != vcEl.end(); ++iter)
	{
		vcEl[iter - vcEl.begin()].Work(hDC, vcTool);           
	}
}

void Drawing::ClearDrawing()  //clear drawing
{
	vcEl.clear();
}

int Drawing::GetSize()   //get size of elements container
{
	return vcEl.size();
}


