#include "CSVReader.h"
#include "StringUtils.h"
#include <fstream>

using namespace std;


vector<string> results;
HWND mainWindowHandle;
HWND indivInputGroupHandle;
HWND indivInputGroup;

HWND *fieldName;
HWND *fieldText;

HWND editArea;
HWND templateArea;

HWND csvHeader;
HWND csvHeaderText;
HWND listView;
HWND numEntriesView;
vector<CSVHeaderItem>CSVList;
HINSTANCE mainInst; 
typedef pair <string, string> CSVDataPair;
vector<string> editTextVec;
vector<string> templateTextVec;
vector<string> csvHeaderVec; 
vector<string> CSV; 
string internalDelim = "|";

bool FillCSVHeaders(string path,string delim = "\t")
{
	csvHeaderVec.clear();
	
	ifstream myfile (path);
	string line;

	if (!myfile.is_open())
		return false;
	
	bool headerLine = true;

	while ( getline (myfile,line) )
	{
		if(headerLine)
		{
			csvHeaderVec = StringUtils::Tokenize(line,delim);
		}
		else
		{
		}
	}
	myfile.close();

	for (int i = 0; i < csvHeaderVec.size(); i++)
	{
		CSVHeaderItem temp;
		temp.csvHeaderName = csvHeaderVec[i];
		temp.enabled = false;
		temp.csvIndex = i;
		CSVList.push_back(temp);
	}
	return true;
}

void InitMainWindows(HWND hDlg)
{
		
	mainWindowHandle = hDlg;
	
	defualtCSVHeader.clear();
	defualtTemplate.clear();
	defaultInput.clear();
	
	hWnd = CreateWindowEx(NULL,BUTTON,"Checkbox",BS_CHECKBOX,xPos,yPos,width,height,parent,NULL,NULL,NULL)

	HWND hwndChannelList = CreateWindow(
        L"COMBOBOX",  // Predefined class; Unicode assumed 
        L"",      // Button text 
        WS_VISIBLE | WS_CHILD | CBS_DROPDOWNLIST | BS_DEFSPLITBUTTON | CBS_DROPDOWN | CBS_HASSTRINGS | WS_VSCROLL,  // Styles WS_VSCROLL | BS_DEFSPLITBUTTON WS_DISABLED | 
        10,         // x position 
        80,         // y position 
        100,        // Button width
        200,        // Button height
        hWnd,     // Parent window
        (HMENU)IDC_CHANNEL_COUT_BUTTON,       //menu.
        (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),
        NULL);   


	int buttonHeight = 25;
	csvHeader = CreateWindow(TEXT("STATIC"), TEXT("CSV Header Tempalte"), WS_CHILD | WS_VISIBLE ,X + 10, Y,textAreaWidth ,textHeeight , hDlg, (HMENU)IDC_CSV_HEADER, NULL, NULL);
	csvHeaderText = CreateWindow(TEXT("Edit"), TEXT(defualtCSVHeader.c_str()), WS_CHILD | WS_VISIBLE | WS_BORDER, X + (8*19), Y, longInput, textHeeight, hDlg, (HMENU)IDC_CSV_HEADER_TEXT, NULL, NULL);
	CreateWindow(TEXT("BUTTON"), TEXT("set header"), WS_VISIBLE | WS_CHILD,  X + (8*19)+longInput+10,Y, 100, buttonHeight, hDlg, (HMENU)IDC_EXECUTE_CSV_HEADER_BUTTON, NULL, NULL);


	editArea = CreateWindow(TEXT("EDIT"), TEXT(defaultInput.c_str()), WS_CHILD | WS_VISIBLE | ES_LEFT | ES_WANTRETURN | ES_AUTOVSCROLL | ES_AUTOHSCROLL | ES_MULTILINE  | WS_VSCROLL | WS_HSCROLL, X, Y+textHeeight*2,textAreaWidth ,textAreaHeight , hDlg,  // use WM_SIZE and MoveWindow() to size
	(HMENU)IDC_EDIT_TEXT_WINDOW, GetModuleHandle(NULL), NULL );

	templateArea = CreateWindow(TEXT("EDIT"), TEXT(defualtTemplate.c_str()), WS_CHILD | WS_VISIBLE | ES_LEFT | ES_WANTRETURN | ES_AUTOVSCROLL | ES_AUTOHSCROLL | ES_MULTILINE  | WS_VSCROLL | WS_HSCROLL, textAreaWidth + 10, Y+textHeeight*2,textAreaWidth ,textAreaHeight , hDlg,
	(HMENU)IDC_TEMPLATE_WINDOW, GetModuleHandle(NULL), NULL );
	
	numEntriesView = CreateWindow(TEXT("STATIC"), TEXT(GetNumEntriesText().c_str()), WS_CHILD | WS_VISIBLE ,X + 10+ (textAreaWidth * 2.5), Y+(textHeeight*2),100 ,25 , hDlg, (HMENU)IDC_CSV_HEADER, NULL, NULL);
	
	
	CreateWindow(TEXT("BUTTON"), TEXT("submit"), WS_VISIBLE | WS_CHILD, X,ButtonY, 100, buttonHeight, hDlg, (HMENU)IDC_EXECUTE_CSV_INPUT_BUTTON, NULL, NULL);



}