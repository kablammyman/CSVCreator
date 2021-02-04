#include "stdafx.h"
#include "CSVFields.h"

#include "Resource.h"
#include "CSVHandler.h"
#include "StringUtils.h"
#include <string>
#include <vector>
#include <fstream>
#include <Commctrl.h> //for listview


#define INPUT_BUFFER_SIZE 5000
using namespace std;
	QueryIDResults idResults;
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

	HINSTANCE mainInst; 
	typedef pair <string, string> CSVDataPair;
	vector<string> editTextVec;
	vector<string> templateTextVec;
	vector<string> csvHeaderVec; 
	vector<string> CSV; 
	string internalDelim = "|";

	int X = 5, Y = 10;
	int textAreaWidth = winWidth/2;
	int textAreaHeight = winHeight;
	bool testData = true;
	int textHeight= 20;
	int longInput = 1000;
	int ButtonY = Y+textAreaHeight+textHeight+80;
	string numEntriesText = "Entries: ";
string GetNumEntriesText()
{
	string ret = numEntriesText;
	ret += to_string(CSV.size());
	return ret;
}
void CreateIndivInputWindow(HWND hWnd)
{
	indivInputGroupHandle = CreateDialog(mainInst, MAKEINTRESOURCE(IDD_QUICK_QUERY_BOX), hWnd, InputIndvFieldsBox);

	if (indivInputGroupHandle == NULL)
	{
		int x = GetLastError();
		printf("error: %d", x);
	}
	ShowWindow(indivInputGroupHandle, SW_SHOW);
}
void UpdateListView( )
{
	// RECT rcClient;                       // The parent window's client area.
  // GetClientRect(hDlg, &rcClient);
  // listView= CreateListView(hDlg, colNames,X, ButtonY + 30, (longInput+longInput/2), textAreaHeight, longInput);
	
	if(listView != NULL)
	{
		DestroyWindow(listView);
		listView = NULL;
	}
	int width = (longInput+longInput/2);
	int colWidth = 100;
	if(csvHeaderVec.size() > 0)
		colWidth =  width/csvHeaderVec.size();
	listView= CreateListView(mainWindowHandle, csvHeaderVec,X, ButtonY + 50, width,textAreaHeight,colWidth);
}
void InitMainWindows(HWND hDlg)
{
	
	string defualtCSVHeader = "NAME,ADDRESS,CITY,STATE,ZIP,PADDRESS,PTOWN,CNTY_NAME,COUNTY_STATE,APN,ACRES";
	string defaultInput = "Parcel Detail for Parcel #  001-011-01\r\n"
"Location\r\n"
"Property Location\r\n"
"591 COUNTY RD M101\r\n"
"\r\n"
"\r\n"
"\r\n"
"Town\r\n"
"EUREKA\r\n"
"District\r\n"
"1.0 - Eureka Town District\r\n"
"Subdivision\r\n"
"   Lot    Block 117\r\n"
"Property Name\r\n"
"\r\n" 
"\r\n" 
"Ownership\r\n"
"Assessed Owner Name\r\n"
"SILVER VIKING CORPORATION\r\n"
"\r\n"
"Mailing Address\r\n"
"\r\n" 
"\r\n"
"545 SO. VALLEY VIEW DR., #100\r\n" 
"ST. GEORGE, UT 84770\r\n"
"Legal Owner Name\r\n"
"SILVER VIKING CORPORATION\r\n"
"Vesting Doc #, Date\r\n"
"174382  02/14/2000   Year / Book / Page 00 / 334 / 41\r\n"
"Map Document #s";
	
	string defualtTemplate = "Parcel Detail for Parcel #  [APN]\r\n"
"Location\r\n"
"Property Location\r\n"
"[PADDRESS]\r\n"
"\r\n"
"\r\n"
"\r\n"
"//Town\r\n"
"[PTOWN]\r\n"
"//District\r\n"
"1.0 - Eureka Town District\r\n"
"Subdivision\r\n"
"   Lot    Block 117\r\n"
"Property Name\r\n"
"\r\n" 
"\r\n" 
"//Ownership\r\n"
"Assessed Owner Name\r\n"
"[NAME]\r\n"
"\r\n"
"Mailing Address\r\n"
"\r\n" 
"\r\n"
"[ADDRESS]\r\n"
"[CITY], [STATE] [ZIP]\r\n"
"Legal Owner Name\r\n"
"SILVER VIKING CORPORATION\r\n"
"Vesting Doc #, Date\r\n"
"174382  02/14/2000   Year / Book / Page 00 / 334 / 41\r\n"
"Map Document #s";
	
	mainWindowHandle = hDlg;
	if(testData)
	{
		csvHeaderVec.clear();
		csvHeaderVec = StringUtils::Tokenize(defualtCSVHeader,","); 
	}
	else
	{
		defualtCSVHeader.clear();
		defualtTemplate.clear();
		defaultInput.clear();
	}
	int buttonHeight = 25;
	int xoffset = 8*19;
	csvHeader = CreateWindow(TEXT("STATIC"), TEXT("CSV Header Tempalte"), WS_CHILD | WS_VISIBLE ,X + 10, Y,textAreaWidth ,textHeight , hDlg, (HMENU)IDC_CSV_HEADER, NULL, NULL);
	csvHeaderText = CreateWindow(TEXT("Edit"), TEXT(defualtCSVHeader.c_str()), WS_CHILD | WS_VISIBLE | WS_BORDER, X + (xoffset), Y, longInput, textHeight, hDlg, (HMENU)IDC_CSV_HEADER_TEXT, NULL, NULL);
	CreateWindow(TEXT("BUTTON"), TEXT("set header"), WS_VISIBLE | WS_CHILD,  X + (xoffset)+longInput+10,Y, 100, buttonHeight, hDlg, (HMENU)IDC_EXECUTE_CSV_HEADER_BUTTON, NULL, NULL);

	int textBoxPos = Y+textHeight*4;
	CreateWindow(TEXT("STATIC"), TEXT("Data for csv entry"), WS_CHILD | WS_VISIBLE ,X + 10, textBoxPos - 50 ,textAreaWidth,20 , hDlg, NULL, NULL, NULL);
	editArea = CreateWindow(TEXT("EDIT"), TEXT(defaultInput.c_str()), WS_CHILD | WS_VISIBLE | ES_LEFT | ES_WANTRETURN | ES_AUTOVSCROLL | ES_AUTOHSCROLL | ES_MULTILINE  | WS_VSCROLL | WS_HSCROLL, 
	X, textBoxPos,textAreaWidth ,textAreaHeight , hDlg,  // use WM_SIZE and MoveWindow() to size
	(HMENU)IDC_EDIT_TEXT_WINDOW, GetModuleHandle(NULL), NULL );

	CreateWindow(TEXT("STATIC"), TEXT("template on how to parse data (set [tags] + header at same time!)"), WS_CHILD | WS_VISIBLE ,textAreaWidth + 10, textBoxPos - 50 ,textAreaWidth,20 , hDlg, NULL, NULL, NULL);
	templateArea = CreateWindow(TEXT("EDIT"), TEXT(defualtTemplate.c_str()), WS_CHILD | WS_VISIBLE | ES_LEFT | ES_WANTRETURN | ES_AUTOVSCROLL | ES_AUTOHSCROLL | ES_MULTILINE  | WS_VSCROLL | WS_HSCROLL, 
	textAreaWidth + 10, textBoxPos,textAreaWidth ,textAreaHeight , hDlg,
	(HMENU)IDC_TEMPLATE_WINDOW, GetModuleHandle(NULL), NULL );
	
	numEntriesView = CreateWindow(TEXT("STATIC"), TEXT(GetNumEntriesText().c_str()), WS_CHILD | WS_VISIBLE ,X + 10+ (textAreaWidth * 2.5), Y+(textHeight*2),100 ,25 , hDlg, (HMENU)IDC_CSV_HEADER, NULL, NULL);
	
	
	CreateWindow(TEXT("BUTTON"), TEXT("submit"), WS_VISIBLE | WS_CHILD, X,ButtonY, 100, buttonHeight, hDlg, (HMENU)IDC_EXECUTE_CSV_INPUT_BUTTON, NULL, NULL);


	
	UpdateListView();

}
/*LRESULT CALLBACK Edit_Prc(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam)
{
  if(msg==WM_CHAR&&wParam==1)
  {
	  SendMessage(hwnd,EM_SETSEL,0,-1); 
	  return 1;
  }
  else 
	  return CallWindowProc((void*)WPA,hwnd,msg,wParam,lParam);
}*/

void InitIndividualFields(HWND hDlg)
{
	int baseX = 10;
	int baseY = 10,stepY = 32;
	int W = 150;
	int H = 10;
	int labelWidth = 100;
	int editSize = stepY - 2;

	
	indivInputGroup = CreateWindowEx(WS_EX_TRANSPARENT, TEXT("Button"), TEXT("Individual Input"), WS_CHILD | WS_VISIBLE | BS_GROUPBOX, 10,  10, 500, 300, hDlg, (HMENU)11, NULL, NULL);
	
	int numFields = csvHeaderVec.size();

	if(fieldName != nullptr)
	{
		delete[] fieldName;
		fieldName = nullptr;
	}
	if(fieldText != nullptr)
	{
		delete[] fieldText;
		fieldText = nullptr;
	}

	 fieldName = new HWND[numFields];
	 fieldText = new HWND[numFields];

	for(size_t i = 0; i < numFields; i++)
	{
		fieldName[i] = CreateWindow(TEXT("BUTTON"), TEXT(csvHeaderVec[i].c_str()), WS_CHILD | WS_VISIBLE , baseX, baseY + (stepY * (i + 1)), labelWidth, editSize, indivInputGroup, (HMENU)IDC_INDIVIDUAL_FIELD_LABEL_START+i, NULL, NULL);
		fieldText[i] = CreateWindow(TEXT("Edit"), TEXT(""), WS_CHILD | WS_VISIBLE | WS_BORDER, baseX + labelWidth, baseY + (stepY * (i + 1)), 300, editSize, indivInputGroup, (HMENU)IDC_INDIVIDUAL_FIELD_TEXT_START+i, NULL, NULL);
	}
	
}
void RemoveBlankLines(vector<string> &data)
{
	for(size_t i = data.size()-1; i > 0; i--)
		if(data[i] == "")
			data.erase(data.begin() + i);
}

vector<CSVDataPair> FindTokensInsideInput(vector<string> &input,vector<string> &templateList)
{
	vector<CSVDataPair> retTokens;
	if(csvHeaderVec.empty() )
	{
		MessageBox(mainWindowHandle,_T("You have not set a header!"), _T("empty header") , MB_OK);
		return retTokens;
	}
	if(templateList.empty() )
	{
		MessageBox(mainWindowHandle,_T("Your template is blank!"), _T("empty template") ,  MB_OK);
		return retTokens;
	}
	for(size_t i = 0; i < input.size(); i++)
	{
		//make sure we dont overrun the bounds of the template
		if(i > templateList.size()-1 )
			break;
		//this is a line we ignore...its commented out so to speak
		if(templateList[i][0] == '/' && templateList[i][1] == '/')
			continue;
		size_t templateIndex = 0, inputIndex = 0;
		while(templateIndex < templateList[i].size())
		{
			size_t numCharsToToken = templateList[i].find('[',templateIndex);
			if (numCharsToToken == string::npos)
				break;

			//move the where to start getting the input. this only needs to be done at start of first tag
			//after that, it depends on the input gathered...this is where input and tempalte index diverge
			
			//if(inputIndex == 0)
			inputIndex += numCharsToToken - templateIndex;
			templateIndex = numCharsToToken;

			string tag = StringUtils::CopyCharsBetweenTags(templateList[i], '[',']',templateIndex);
			
			//gotta use tempalte structe to tell how the data will be.
			//so [tag1] [tag2],[tag3]
			//means that start index at tag1. copy until space. start copy of tag 2. copy until comma. copy tag 3 until \n
			//this wont work well if tag1 has spaces within it.
			char delim;
			size_t nextIndex = templateList[i].find(']',templateIndex+1);
			
			if (nextIndex <  templateList[i].size()-1)
			{
				delim = templateList[i][nextIndex+1];
			}
			else 	
				delim = '\n';
		
			string value = StringUtils::CopyUntilChar(input[i],inputIndex,delim);
			
			
			if(!tag.empty())
				retTokens.push_back(make_pair(tag,value));
			
			templateIndex = nextIndex+1;

			//move index by size of the token we collected
			//plus the num of chars from cur template token to next
			inputIndex += value.size();
		}
		
	}
	
	return retTokens;
}
string CreateCSVLine(vector<CSVDataPair> &input, vector<string> &header,string delim)
{
	string line = "";
	bool hasToken = false;
	for(size_t i = 0; i < header.size(); i++)
	{
		hasToken = false;
		for(size_t j = 0; j <input.size(); j++)
		{
			if(input[j].first == header[i])
			{
				line += input[j].second + delim;
				hasToken = true;
				break;
			}
		}
		if(!hasToken)
		{
			line += " ";
			line += delim;
		}
	}
	return line;
}
BOOL CheckInput(WPARAM wParam, std::string &output)
{
	if (wParam == IDC_EXECUTE_CSV_INPUT_BUTTON)
	{
		char editBuffer[INPUT_BUFFER_SIZE];
		int numEditChars = GetDlgItemText(mainWindowHandle, IDC_EDIT_TEXT_WINDOW, editBuffer, INPUT_BUFFER_SIZE);
		

		if (numEditChars == 0)
		{
			int x = GetLastError();
			printf("error: %d", x);
		}

		editTextVec.clear();
		editTextVec = StringUtils::Tokenize2(editBuffer,"\r\n");
		//RemoveBlankLines(editText);

		vector<CSVDataPair> csvLineData = FindTokensInsideInput(editTextVec,templateTextVec);
		
		if(csvLineData.empty())
			return (INT_PTR)TRUE;

		string newLine = CreateCSVLine(csvLineData, csvHeaderVec,internalDelim);
		CSV.push_back(newLine);
	
		SetWindowText(numEntriesView,  GetNumEntriesText().c_str());
		return (INT_PTR)TRUE;
	}
	if(wParam == IDC_EXECUTE_CSV_HEADER_BUTTON)
	{
		char templateBuffer[INPUT_BUFFER_SIZE];
		char csvHeaderBuffer[MAX_PATH];

		int numTemplateChars = GetDlgItemText(mainWindowHandle, IDC_TEMPLATE_WINDOW, templateBuffer, INPUT_BUFFER_SIZE);
		int numHeaderChars = GetDlgItemText(mainWindowHandle, IDC_CSV_HEADER_TEXT, csvHeaderBuffer, MAX_PATH); 
		
		if (numTemplateChars == 0 || numHeaderChars == 0)
		{
			int x = GetLastError();
			printf("error: %d", x);
		}

		csvHeaderVec.clear();
		csvHeaderVec = StringUtils::Tokenize(csvHeaderBuffer,","); 

		templateTextVec.clear();
		templateTextVec = StringUtils::Tokenize2(templateBuffer,"\r\n");
		
		UpdateListView();

		return (INT_PTR)TRUE;
	}
	
	return FALSE;
}
// Message handler for eitehr selecting or deleting record...window dims are in resource.rc
INT_PTR CALLBACK InputIndvFieldsBox(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:

		InitMainWindows(hDlg);
		return (INT_PTR)TRUE;

	case WM_COMMAND:
	{
		std::string output;
		CheckInput(wParam, output);
	}
		break;
	case WM_PAINT:
		if (indivInputGroupHandle != NULL)
			ShowWindow(indivInputGroupHandle, SW_SHOW);
		break;
	case WM_CLOSE:
		EndDialog(hDlg, 0);
		break;
	}
	return (INT_PTR)FALSE;
}

HWND CreateListView(HWND hwndParent, vector<string> colNames,int x, int y, int w, int h, int size)
{
	INITCOMMONCONTROLSEX icex;           // Structure for control initialization.
	icex.dwICC = ICC_LISTVIEW_CLASSES;
	InitCommonControlsEx(&icex);
	LV_COLUMN lvC;      // List View Column structure
	char szText[MAX_PATH];

	// Create the list-view window in report view with label editing enabled.
	HWND hWndListView = CreateWindow(WC_LISTVIEW,
		"Current CSV",
		WS_VISIBLE | WS_CHILD | LVS_REPORT | LVS_EDITLABELS | WS_BORDER | LVS_SHOWSELALWAYS,
		x, y,w,h,
		hwndParent,
		(HMENU)IDM_LIST_VIEW_RESULTS,
		mainInst,
		NULL);
	// Add the columns.
	lvC.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	lvC.fmt = LVCFMT_LEFT;  // left align the column
	lvC.pszText = szText;
	for (int index = 0; index < colNames.size(); index++)
	{
		lvC.cx = size;            // width of the column, in pixels
		lvC.iSubItem = index;
		lvC.pszText = (LPSTR)colNames[index].c_str();
		ListView_InsertColumn(hWndListView, index, &lvC);
	}
	SendMessage(hWndListView, LVM_SETEXTENDEDLISTVIEWSTYLE, LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);
	return (hWndListView);
}

BOOL FillListViewItems(HWND hWndListView, vector<string> items)
{
	for (size_t index = 0; index < items.size(); index++)
	{
		LVITEM lvI;

		// Initialize LVITEM members that are common to all items.
		lvI.mask = LVIF_TEXT | LVIF_PARAM; //// Initialize LV_ITEM members that are common to all items.
		lvI.stateMask = 0;
		lvI.state = 0;
		// structures

		vector<string> curElement = StringUtils::Tokenize(items[index],internalDelim);
		
		lvI.mask=LVIF_TEXT;   // Text Style
		lvI.cchTextMax = 256; // Max size of test
		lvI.iItem=0;          // choose item  
		lvI.iSubItem=0;       // Put in first coluom
		lvI.pszText= (LPSTR)curElement[0].c_str(); // Text to display 

		SendMessage(hWndListView,LVM_INSERTITEM,0,(LPARAM)&lvI); // Send info to the Listview

		for(size_t i=1;i<curElement.size();i++) // Add SubItems in a loop
		{
		   lvI.iSubItem=i;
		   lvI.pszText=(LPSTR)curElement[i].c_str();
		   SendMessage(hWndListView,LVM_SETITEM,0,(LPARAM)&lvI); // Enter text to SubItems
		}
	}
	
	return TRUE;
}
bool SaveCSV(string path)
{
	CSVHandler csv;
	
	csv.CreateCSVFile(path, csvHeaderVec, ',',true);
	map<string, string> dict;

	for (size_t i = 0; i < csvHeaderVec.size(); i++)
		dict[csvHeaderVec[i]] = "temp";

	for(size_t i = 0; i < CSV.size(); i++)
	{
		vector<string> tokens = StringUtils::Tokenize(CSV[i], '|');

		for (int j = 0; j < tokens.size(); j++)
		{
			dict[csvHeaderVec[j]] = tokens[j];
		}

		csv.WriteCSVEntry(dict);
	}

	return true;
}

//delim is a dafu;lt parm with the tab char. we can override it to read in other things like commas or something
bool LoadCSV(string path, char delim)
{
	CSVHandler csv;
	if(!csv.ReadCSVFile(path,delim))
		return false;
		
	SetDlgItemText(mainWindowHandle, IDC_CSV_HEADER_TEXT, csv.GetCsvHeaderString().c_str());
	csvHeaderVec.clear();
	csvHeaderVec = csv.GetCSVHeader();

	UpdateListView();
		
	SetDlgItemText(numEntriesView, IDC_NUM_ENTRIES_VIEW, to_string(csv.GetCSVLength()).c_str());
	CSV.clear();
	for (size_t i = 0; i < csv.GetCSVLength(); i++)
	{
		vector<string> tokens = csv.GetCSVLineAsVector(i);
		string line;
		for (size_t j = 0; j < tokens.size(); j++)
		{
			line += tokens[j] + internalDelim;
		}
		CSV.push_back(line);
	}
	
	
	return false;
}