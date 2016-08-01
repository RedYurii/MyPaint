// MyPaint application was created using win32 API. Additional external dependencies were used (shlwapi.dll)
//

#include "stdafx.h"
#include "MyPaint.h"
#include "Client.h"

#define MAX_LOADSTRING 100
#define MAX_CAPTION_SIZE 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_MYPAINT, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MYPAINT));

    MSG msg;


    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
	if (_CrtDumpMemoryLeaks())
		MessageBox(msg.hwnd, L"Memory Leaks", L"MemLeak Check", MB_OK);
    return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MYPAINT));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName = NULL;
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}


BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}


void DrawBoxOutline(HWND hwnd,int  ptxBeg, int  ptyBeg, int ptxEnd, int ptyEnd) //fucntion draws rectanle around shape that is currently drawn 
{
	HDC hdc;
	hdc = GetDC(hwnd);
	SetROP2(hdc, R2_NOT);
	SelectObject(hdc, GetStockObject(NULL_BRUSH));
	Rectangle(hdc, ptxBeg, ptyBeg, ptxEnd, ptyEnd);
	MoveToEx(hdc, ptxBeg, ptyBeg, NULL);
	LineTo(hdc, ptxEnd, ptyEnd);

	ReleaseDC(hwnd, hdc);
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static Client cl;  //object of Client class
	static BOOL  fBlocking, fValidBox;   //flags used for blocking mouse capture 
	HDC hdc;
	RECT rect;
	static HMENU hMenu, hToolsMenu, hFileMenu, hEditMenu,  hPenMenu, hWidthMenu, hColorMenu; //menu handles
	static COLORREF ShapeColor;   
	static int nInit_X, nInit_Y, nFinal_X, nFinal_Y, nThick = 4;  
	switch (message)
	{
	case WM_CREATE:  //in this sequence main menu and submenus are initialized
	{
		hMenu = CreateMenu();
		hFileMenu = CreatePopupMenu();
		AppendMenu(hMenu,  MF_BYPOSITION | MF_POPUP, (UINT_PTR)hFileMenu, L"File");  
		AppendMenu(hFileMenu, MF_BYPOSITION, (UINT_PTR)hFileMenu, L"Open");
		AppendMenu(hFileMenu, MF_BYPOSITION, (UINT_PTR)hFileMenu, L"Save");
		AppendMenu(hFileMenu, MF_SEPARATOR | MF_BYPOSITION, 0, NULL);
		AppendMenu(hFileMenu, MF_BYPOSITION, (UINT_PTR)hFileMenu, L"Exit");
		hEditMenu = CreatePopupMenu();
		AppendMenu(hMenu, MF_BYPOSITION | MF_POPUP, (UINT_PTR)hEditMenu, L"Edit");
		AppendMenu(hEditMenu, MF_BYPOSITION, (UINT_PTR)hFileMenu, L"Undo");
		AppendMenu(hEditMenu, MF_BYPOSITION, (UINT_PTR)hFileMenu, L"Clear");
		hToolsMenu = CreatePopupMenu();
		AppendMenu(hMenu, MF_BYPOSITION | MF_POPUP , (UINT_PTR)hToolsMenu, L"Tools");
		cl.Scan(hToolsMenu);  //call scan menu to get avaliable tools 
		hPenMenu = CreatePopupMenu();
		AppendMenu(hMenu, MF_BYPOSITION | MF_POPUP , (UINT_PTR)hPenMenu, L"Pen properties");
		hWidthMenu = CreatePopupMenu();
		InsertMenu(hPenMenu, 0, MF_BYPOSITION | MF_POPUP | MF_DISABLED, (UINT_PTR)hWidthMenu, L"Width");
		for (int i = 0; i < 10; ++i)
		{
			WCHAR szBuff[MAX_SYMBOLS];
			wsprintf(szBuff, L"%d", i + 1);
			InsertMenu(hWidthMenu, i, MF_BYPOSITION, (UINT_PTR)hWidthMenu, szBuff);
		}
		hColorMenu = CreatePopupMenu();
		InsertMenu(hPenMenu, 1, MF_BYPOSITION | MF_POPUP | MF_DISABLED, (UINT_PTR)hColorMenu, L"Color");
		InsertMenu(hColorMenu,-1, MF_BYPOSITION , (UINT_PTR)hColorMenu, L"Black");
		InsertMenu(hColorMenu, -1, MF_BYPOSITION , (UINT_PTR)hColorMenu, L"Red");
		InsertMenu(hColorMenu, -1, MF_BYPOSITION, (UINT_PTR)hColorMenu, L"Green");
		InsertMenu(hColorMenu, -1, MF_BYPOSITION, (UINT_PTR)hColorMenu, L"Blue");
		InsertMenu(hColorMenu, -1, MF_BYPOSITION, (UINT_PTR)hColorMenu, L"Yellow");
		InsertMenu(hColorMenu, -1, MF_BYPOSITION, (UINT_PTR)hColorMenu, L"Brown");
		InsertMenu(hColorMenu, -1, MF_BYPOSITION, (UINT_PTR)hColorMenu, L"White");
		InsertMenu(hColorMenu, -1, MF_BYPOSITION, (UINT_PTR)hColorMenu, L"Custom");
		
		MENUINFO mi;
		memset(&mi, 0, sizeof(mi));
		mi.cbSize = sizeof(mi);
		mi.fMask = MIM_STYLE;
		mi.dwStyle = MNS_NOTIFYBYPOS;
		SetMenuInfo(hMenu, &mi);
		SetMenu(hWnd, hMenu);
		CheckMenuItem(hWidthMenu, 3, MF_BYPOSITION | MF_CHECKED);
		CheckMenuItem(hColorMenu, 0, MF_BYPOSITION | MF_CHECKED);
	}
		break;
	case WM_LBUTTONDOWN:    // LMB is pressed
		{
			nInit_X = LOWORD(lParam);
			nInit_Y = HIWORD(lParam);
			nFinal_X = nInit_X;
			nFinal_Y = nInit_Y;
			hdc = GetDC(hWnd);
			DrawBoxOutline(hWnd, nInit_X, nInit_Y, nInit_X, nInit_Y);
			cl.StartDraw(hdc, nInit_X, nInit_Y, nThick, ShapeColor);
			SetCapture(hWnd);  
			SetCursor(LoadCursor(NULL, IDC_CROSS));
			fBlocking = TRUE;  
			ReleaseDC(hWnd, hdc);
		}
		break;
	case WM_LBUTTONUP:  // LMB is released
		{
			hdc = GetDC(hWnd);
			DrawBoxOutline(hWnd, nInit_X, nInit_Y, nFinal_X, nFinal_Y);
			nFinal_X = LOWORD(lParam);
			nFinal_Y = HIWORD(lParam);
			ReleaseCapture();
			SetCursor(LoadCursor(NULL, IDC_ARROW));
			fBlocking = FALSE;
			fValidBox = TRUE;
			cl.StopDraw(hdc, nFinal_X, nFinal_Y); 
			InvalidateRect(hWnd, NULL, TRUE);
			ReleaseDC(hWnd, hdc);
		}
		break;
	case WM_MOUSEMOVE:      
		if (MK_LBUTTON & wParam)  // LMB is pressed and moving
		{
			SetCursor(LoadCursor(NULL, IDC_CROSS));
			DrawBoxOutline(hWnd, nInit_X, nInit_Y, nFinal_X, nFinal_Y);
			nFinal_X = LOWORD(lParam);
			nFinal_Y = HIWORD(lParam);
			DrawBoxOutline(hWnd, nInit_X, nInit_Y, nFinal_X, nFinal_Y);
		}
		break;
	case WM_MENUCOMMAND:
	{
		HMENU menu = (HMENU)lParam;
		int idx = wParam;
		
		if (menu == hToolsMenu)   //parse tools menu 
		{
			static int nPrevItem = -1;
			EnableMenuItem(hPenMenu, 0, MF_BYPOSITION | MF_ENABLED);  //enable pen and  color menu which are grayed at the start of application
			EnableMenuItem(hPenMenu, 1, MF_BYPOSITION | MF_ENABLED);
			cl.SetBTInd(idx);		//set index of selected tool
			CheckMenuItem(hToolsMenu, idx, MF_BYPOSITION| MF_CHECKED);       //check selected item
			CheckMenuItem(hToolsMenu,nPrevItem, MF_BYPOSITION | MF_UNCHECKED);   //uncheck previous item
			nPrevItem = idx;
		}
		else if (menu == hFileMenu)  //file selection menu
		{
			switch(idx)
			{
			case 0:   // open file selected
			{
				cl.ClearDrawing();
				FILE* f = fopen("MyDrawing.mdwg", "rb");  //simple solution with saving to only one particular file
				if (f == NULL)
				{
					MessageBox(hWnd, L"File does not exist", L"Error", MB_OK);
					break;
				}
				fseek(f, 0, SEEK_END);
				int size = ftell(f);
				size /= sizeof(Element);
				fseek(f, 0, SEEK_SET);
				for (int i = 0; i < size; ++i)
				{
					Element elBuff;
					fread(&elBuff, sizeof(Element), 1, f);
					cl.LoadElement(elBuff);
				}
				fclose(f);
				GetClientRect(hWnd, &rect);
				fBlocking = FALSE;
				fValidBox = TRUE;
				InvalidateRect(hWnd, &rect, TRUE);

			}
				break;
			case 1:		// save file selected
			{
				FILE* f = fopen("MyDrawing.mdwg", "wb");
				int size = cl.GetDwgSize();
				for (int i = 0; i < size; ++i)
				{
					fwrite(&cl.GetElement(i), sizeof(Element), 1, f);
				}
				fclose(f);
			}
				break;
			case 3:   //exit selected
				DestroyWindow(hWnd);
				break;
			}
		}
		else if (menu == hEditMenu)  //edit menu 
		{
			switch (idx)
			{
			case 0:     // undo selected
				cl.Undo();
				GetClientRect(hWnd, &rect);
				InvalidateRect(hWnd, &rect, TRUE);
				break;
			case 1:     // clear selected
				cl.ClearDrawing();
				GetClientRect(hWnd, &rect);
				InvalidateRect(hWnd, &rect, TRUE);
				break;
			}
		}
		else if (menu == hWidthMenu)   //pen width selection
		{
			static int nPrevItem = 3;
			switch(idx)
			{
			case 0:
			case 1:
			case 2:
			case 3:
			case 4:
			case 5:
			case 6:
			case 7:
			case 8:
			case 9:
				nThick =idx+1;
				CheckMenuItem(hWidthMenu, idx, MF_BYPOSITION | MF_CHECKED);
				CheckMenuItem(hWidthMenu, nPrevItem, MF_BYPOSITION | MF_UNCHECKED);
				nPrevItem = idx;
				break;
			}
		}
		else if (menu == hColorMenu)  //color selection menu
		{
			static int nPrevItem = 0;
			CheckMenuItem(hColorMenu, idx, MF_BYPOSITION | MF_CHECKED);
			CheckMenuItem(hColorMenu, nPrevItem, MF_BYPOSITION | MF_UNCHECKED);
			nPrevItem = idx;
			switch (idx)
			{
			case 0:				
				ShapeColor = RGB(0, 0, 0);   //black 
				break;
			case 1:
				ShapeColor = RGB(255, 0, 0);  // red
				break;
			case 2:
				ShapeColor = RGB(0,255, 0);	// green
				break;
			case 3:
				ShapeColor = RGB(0, 0, 255);   //blue
				break;
			case 4:
				ShapeColor = RGB(255,255, 0);   //yellow
				break;
			case 5:
				ShapeColor = RGB(102, 51, 0);   //brown
				break;
			case 6:
				ShapeColor = RGB(255, 255, 255);   //White
				break;
			case 7:						//custom color selection using common dialog window
				static CHOOSECOLOR cc;
				static COLORREF    crCustColors[16];

				cc.lStructSize = sizeof(CHOOSECOLOR);
				cc.hwndOwner = NULL;
				cc.hInstance = NULL;
				cc.rgbResult = RGB(0x80, 0x80, 0x80);
				cc.lpCustColors = crCustColors;
				cc.Flags = CC_RGBINIT | CC_FULLOPEN;
				cc.lCustData = 0;
				cc.lpfnHook = NULL;
				cc.lpTemplateName = NULL;

				ChooseColor(&cc);
				ShapeColor = cc.rgbResult;
				break;
			}
		}
	}
	break;
    case WM_PAINT:
		PAINTSTRUCT ps;
		hdc = BeginPaint(hWnd, &ps);

		if (fValidBox)  //when user finished drawing one element
		{
			cl.ReDraw(hdc);  //redraw 
		}
		if (fBlocking)  //user is currently drawing and element
		{
			SetROP2(hdc, R2_NOT);
			SelectObject(hdc, GetStockObject(NULL_BRUSH));
			DrawBoxOutline(hWnd, nInit_X, nInit_Y, nFinal_X, nFinal_Y);
		}
		EndPaint(hWnd, &ps);
        break;
    case WM_DESTROY:
		{
			cl.~Client();      //explicit call of client destructor
			PostQuitMessage(0);
		}
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}


