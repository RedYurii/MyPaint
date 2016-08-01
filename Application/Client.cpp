#include "stdafx.h"
#include "Client.h"

int Client::mnCount = 0;

Client::Client() 
{
	++mnCount;
	mpDwg = new Drawing;
	miBTind = -1;
}

Client::~Client()
{
	if(mnCount-- == 1)  //because destrutor of client class in this application is called explicitly ref count was used
	{
		vector<BaseTool*>::iterator iter;
		for(iter = vcBT.begin(); iter != vcBT.end(); ++iter)
			delete  vcBT[iter - vcBT.begin()];
		vcBT.clear();
		delete mpDwg;
	}
}

void Client::Scan( HMENU hMenu)  //this function scans the directory in which MyPaint.exe is located and loads dynamic libraries into memory
{
	WCHAR szPath[MAX_SYMBOLS];   //storage for path name
	GetModuleFileName(NULL, szPath, MAX_SYMBOLS);   //getting full path to executable module
	PathRemoveFileSpec(szPath);					//clip .exe name from path
	WIN32_FIND_DATA wf; 
	HANDLE hFile = FindFirstFile(wcscat(szPath,L"\\*.dll*"), &wf); //finding first .dll file in that directory 
	
	if (hFile != INVALID_HANDLE_VALUE) 
	{
		WCHAR szToolName[MAX_SYMBOLS];
		BOOL bOk = TRUE;
		while (bOk)		//in this loop all .dlls in the folder are loaded and checked whether they have required function (GetShape), which is needed for creating objets of BaseTool derived classes
		{
			HMODULE hMod = LoadLibrary(wf.cFileName);	
			typedef BaseTool* (*GetToolT)(LPWSTR);
			GetToolT pGT_Func = (GetToolT)GetProcAddress(hMod, "GetTool"); //GetTool function returns new object of derived class and initializes szToolName  
			if (pGT_Func == NULL)
			{
				FreeLibrary(hMod);
			}
			else
			{
				vcBT.push_back(pGT_Func(szToolName));   //add new tool to container of available tools
				InsertMenu(hMenu, -1, MF_BYPOSITION | MF_STRING, (UINT_PTR)hMenu, szToolName); //add new menu item with the name of avaliable tool 
			}
			bOk = FindNextFile(hFile, &wf);
		}
		FindClose(hFile);
	}	
}

void Client::StartDraw(HDC hDC, int x, int y, int thick, COLORREF color)  //start draw initializes initial coordinates of an element and adds it to container
{
	if (miBTind >= 0) //if any tool is selected
	{
		Element el(miBTind, thick, color);   //create object of Element class
		el.GetCurCoord(INIT, x, y);			//init starting coordinates
		mpDwg->AddElement(el);				//add it to container
	}
}

void Client::StopDraw(HDC hDC, int x, int y)  //finish initializing the element with final coordinates and draw it 
{
	if (miBTind >= 0) //if any tool is selected
	{
		Element el (mpDwg->GetLastElement());  //get last element
		el.GetCurCoord(FINAL, x, y);  //replace it's invlaid final coordinates with correct one
		mpDwg->RemoveElement();    //remvoe last element
		mpDwg->AddElement(el);		//add valid element 
		el.Work(hDC, vcBT);			//draw element
	}
}


void Client::ReDraw(HDC hDC)  //redraw the painting
{
	mpDwg->ReDraw(hDC, vcBT);  
}

void Client::Undo() //undo last action  (remove element from drawing )
{
	mpDwg->RemoveElement();
}

void Client::ClearDrawing()  //clear the drawing
{
	mpDwg->ClearDrawing();
}

void Client::SetBTInd(int ind)  //set index of currently selected tool
{
	miBTind = ind;
}

int Client::GetDwgSize()  //get size (number of elements) of drawign. needed for creating a file 
{
	return mpDwg->GetSize();
}

Element Client::GetElement(int i) //get numbered element 
{
	return mpDwg->GetElement(i);
}

void Client::LoadElement(Element el)   //adds element (acquired from a file) to the drawing 
{
	mpDwg->AddElement(el);
}


