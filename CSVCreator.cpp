// CSVCreator.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "CSVCreator.h"
#include "CSVFields.h"
#include <string>
#include <Commctrl.h> //for listview
#include <Commdlg.h>// for file open dialog (include Windows.h)
#include <WinUser.h>

#define WINDOW_WIDTH 1600
#define WINDOW_HEIGHT 850


using namespace std;
HINSTANCE hInst;                                // current instance
WNDPROC lpfnEditWndProc; 


WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
//------------------------------------------------------------------------------------
// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);

BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
//LRESULT CALLBACK ListViewProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
std::string BrowseToFile(HWND hWnd, LPCTSTR filename,bool save);


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_CSVCREATOR, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }
	

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_CSVCREATOR));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
		//this is cut, copy, paste, select all short cuts
		if (msg.message == WM_KEYDOWN)
		{
			HWND hFocused = GetFocus();

			if(msg.wParam == 'A' && GetKeyState(VK_CONTROL) < 0)
			{
				
				//wchar_t className[6];
				//GetClassNameW(hFocused, className, 6);
				//if (hFocused && !_wcsicmp(className, L"edit"))
				if (hFocused == editArea || hFocused == templateArea)
					SendMessage(hFocused, EM_SETSEL, 0, -1);
			}
			else if(msg.wParam == VK_DELETE)
			{
				if (hFocused == listView)
				{
					// Get the first selected item
					int iPos = ListView_GetNextItem(listView, -1, LVNI_SELECTED);

					if (iPos != -1)
					{
						char path[MAX_PATH];
						ListView_GetItemText(listView, iPos, 0, path, MAX_PATH);
					
						//delete the selected entry
						//gotta do this math since the list is represented in the reverse order than the array
						size_t index = (CSV.size() - iPos)-1;
						CSV.erase(CSV.begin() +  index);

						ListView_DeleteAllItems(listView);
						FillListViewItems(listView, CSV);
					}
				}
			}
		}
    }
	


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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_CSVCREATOR));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_CSVCREATOR);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, WINDOW_WIDTH, WINDOW_HEIGHT, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }
   InitMainWindows(hWnd);
   //  Change the window procedure for both edit windows 
	//  to the subclass procedure. 
   //subclass window...didnt help me cuz of the edit windows, but this i good info for the future
   /*
    WNDPROC OrignMainWinProcProc = reinterpret_cast<WNDPROC>(SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(ListViewProc)));
    if (OrignMainWinProcProc == NULL)
    {
        exit(0);
    }

    // Store the original, default window procedure of the button as the 
    // button control's user data.
    SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(WndProc));
	*/
   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
	case WM_CREATE: 
		

	break;
	
	
	case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
			string output;

			if (CheckInput(wParam, output) == TRUE)
			{
				ListView_DeleteAllItems(listView);
				FillListViewItems(listView, CSV);
			}
			

            // Parse the menu selections:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
			 case IDM_SAVE:
                SaveCSV( BrowseToFile(hWnd,"C:\\",true));
                break;
			 case IDM_OPEN:
				 //this will do something eventually
				LoadCSV( BrowseToFile(hWnd,"C:\\",false));
				 ListView_DeleteAllItems(listView);
				FillListViewItems(listView, CSV);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Add any drawing code that uses hdc here...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}
/*
//unused, but good iunfo for the future
LRESULT CALLBACK ListViewProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
	case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
			string output;

			if (CheckInput(wParam, output) == TRUE)
			{
				ListView_DeleteAllItems(listView);
				FillListViewItems(listView, CSV);
			}
	}
	break;
	case WM_KEYDOWN:
			switch (wParam) 
			{
				case VK_DELETE:
				{
					// Get the first selected item
					int iPos = ListView_GetNextItem(listView, -1, LVNI_SELECTED);

					if (iPos != -1)
					{
						char path[MAX_PATH];
						ListView_GetItemText(listView, iPos, 0, path, MAX_PATH);
					
						//delete the selected entry
						CSV.erase(CSV.begin() + iPos);
						ListView_DeleteAllItems(listView);
						FillListViewItems(listView, CSV);
					}
				}
				case VK_LEFT:
				{
					HWND curWindow = GetActiveWindow();
					if(curWindow == editArea)
					{
						MessageBox(hWnd, _T("edit area") , _T("Key Pressed"), MB_OK);
					}
					else if(curWindow == templateArea)
					{
						MessageBox(hWnd, _T("template area") , _T("Key Pressed"), MB_OK);
					}
				}
					break;
			}
		break;
	}
	//  Call the original window procedure for default processing. 
     return CallWindowProc(WndProc, hWnd, message, wParam, lParam); 
}*/
// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}


std::string BrowseToFile(HWND hWnd, LPCTSTR filename,bool save = false)
{
    OPENFILENAME ofn;       // common dialog box structure
	char szFile[MAX_PATH];       // buffer for file name
	HWND hwnd;              // owner window
	HANDLE hf;              // file handle

	// Initialize OPENFILENAME
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hWnd;
	ofn.lpstrFile = szFile;
	// Set lpstrFile[0] to '\0' so that GetOpenFileName does not 
	// use the contents of szFile to initialize itself.
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = "Text\0*.csv\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST;// | OFN_FILEMUSTEXIST;

	// Display the Open dialog box. 
	BOOL ret;
	if(!save)
		ret = GetOpenFileName(&ofn);
	else
		ret = GetSaveFileName(&ofn);

	if (ret) 
	{
		//return std::string(ofn.lpstrFile); 
		 std::string temp = ofn.lpstrFile;
		 return temp;
	}
	
	return "";
}