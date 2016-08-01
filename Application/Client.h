#include "BaseTool.h"
#include "Drawing.h"

#ifndef _CLIENT_
#define _CLIENT_

class Client   //With this class all 
{
	Drawing* mpDwg;			//pointer to object of Drawing class
	int miBTind;             //index of currently selected tool
	vector<BaseTool*> vcBT;    //set of tools avaliable for user
	static int mnCount;       //ref count
public:
	Client();
	~Client();
	void Scan(HMENU);   //scan application directory and acquire tools to work with painting
	void StartDraw(HDC, int x, int y, int thick, COLORREF color);  //initialize new lement with coordinates pen thickness and color
	void StopDraw(HDC, int x, int y);  //finish initializing the element with final coordinates and draw it 
	void ReDraw(HDC);   //redraw the painting
	void Undo();  //undo last action  (remove element from drawing )
	void ClearDrawing();   //clear the drawing
	void SetBTInd(int);   //set index of currently selected tool
	int GetDwgSize();		//get size (number of elements) of drawign. needed for creating a file 
	Element GetElement(int);   //get numbered element 
	void LoadElement(Element);  //adds element (acquired from a file) to the drawing 
};

#endif // !_CLIENT_
