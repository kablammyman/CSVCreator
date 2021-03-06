// CSVImport take a csv and import it into an sqlite DB
//

#include "stdafx.h"
#include "CSVImport.h"
#include "CSVImporter.h"
#include "CSVImporterWindow.h"

#include <string>


using namespace std;

CSVImporterWIndow csvMergeTest;

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
    LoadStringW(hInstance, IDC_CSVIMPORT, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_CSVIMPORT));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_CSVIMPORT));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_CSVIMPORT);
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

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }
   
   /*if (!csvMergeTest.IsDataLoaded())
   {
	   MessageBox(NULL, _T("couldnt open test data"),_T("error"),MB_OK|MB_SYSTEMMODAL);
	   exit(0);
   }*/
   csvMergeTest.InitMainWindow(hWnd);
   
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
    case WM_COMMAND:
        {
            /*if (CheckInput(wParam, output) == TRUE)
			{
				ListView_DeleteAllItems(listView);
				FillListViewItems(listView, CSV);
			}*/
			if(LOWORD(wParam) == IDC_BROWSE_TO_CSV_BUTTON)
			{
				//use file browser
				string csvFile = BrowseToFile(hWnd,"C:\\");
				//set the string to what the file broser gave us so we can see it on screen
				SetDlgItemText(hWnd, BROWSE_TO_CSV_FILE_PATH, TEXT(csvFile.c_str()));
			}
			if(LOWORD(wParam) == IDC_BROWSE_TO_SQL_BUTTON)
			{
				string sqlFile = BrowseToFile(hWnd,"C:\\");https://www.youtube.com/watch?v=Pt-YFyLGme0
				//set the string to what the file broser gave us so we can see it on screen
				SetDlgItemText(hWnd, BROWSE_TO_SQL_FILE_PATH, TEXT(sqlFile.c_str()));
			}

			if (wParam == IDC_IMPORT_CSV_BUTTON)
			{
				/*if (csvMergeTest.CheckInput())
				{
					//string newPath = BrowseToFile(hWnd,"C:\\",true);
					//csvMergeTest.OutpuNewCSV(newPath);
				}*/
				
				//MessageBox(NULL, _T("cant save yet, finish your work!"),_T("error"),MB_OK|MB_SYSTEMMODAL);
				csvMergeTest.ImportCSVDataToDB();
				MessageBox(NULL, _T("Add done!"),_T("import"),MB_OK|MB_SYSTEMMODAL);
				return (INT_PTR)TRUE;
			}
			if (wParam == IDC_PROCESS_BUTTON)
			{
				if (!csvMergeTest.CheckCsvFileInput())
				{
					 MessageBox(NULL, _T("can only open csv or txt files!"),_T("error"),MB_OK|MB_SYSTEMMODAL);
				}
				if (!csvMergeTest.CheckSqlFileInput())
				{
					 MessageBox(NULL, _T("this is an invalid sqlite db file"),_T("error"),MB_OK|MB_SYSTEMMODAL);
				}
				else
				{
					csvMergeTest.ProcessInputFiles();
				}
				return (INT_PTR)TRUE;
			}
			if (HIWORD(wParam) == CBN_SELCHANGE && LOWORD(wParam) == PICK_DATABASE_TABLE)
			{
				//&& 
				csvMergeTest.FillInAllData();
			}

			int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
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

std::string BrowseToFile(HWND hWnd, LPCTSTR filename,bool save)
{
    OPENFILENAME ofn;       // common dialog box structure
	char szFile[MAX_PATH];       // buffer for file name
	//HWND hwnd;              // owner window
	//HANDLE hf;              // file handle

	// Initialize OPENFILENAME
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hWnd;
	ofn.lpstrFile = szFile;
	// Set lpstrFile[0] to '\0' so that GetOpenFileName does not 
	// use the contents of szFile to initialize itself.
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(szFile);
//	ofn.lpstrFilter = "Text\0*.csv\0*.db\0";
	ofn.lpstrFilter = "*.*\0";
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