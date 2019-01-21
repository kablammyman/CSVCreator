#include "stdafx.h"
#include "CSVImporterWIndow.h"
#include "Resource.h"

using namespace std;

CSVImporterWIndow::CSVImporterWIndow()
{
	csvImporter = nullptr;
	sqlHandler = nullptr;
}
string CSVImporterWIndow::GetSelectedValueFromDropdown(HWND dropdown)
{
	//case CBN_SELCHANGE:
	int idx_row;
	char strText[255] = {0};
	idx_row = SendMessage(dropdown , CB_GETCURSEL, 0, 0 );
	SendMessage(dropdown,CB_GETLBTEXT, idx_row,(LPARAM)strText);
	return strText;
}

void CSVImporterWIndow::InitMainWindow(HWND hDlg)
{
	mainWindowHandle = hDlg;
	
	string testPath = "D:\\source\\CSVCreator\\testData.csv";
	string testPath2 = "D:\\source\\CSVCreator\\EurekaParcels.db";

	//64 is 8 chars x 8pixels per char
	CreateWindow(TEXT("STATIC"), TEXT("CSV Path"), WS_CHILD | WS_VISIBLE, xPos, 10, width + 64, 20, hDlg, (HMENU)(BROWSE_TO_CSV_HEADER), NULL, NULL);
	CreateWindow(TEXT("Edit"), TEXT(testPath.c_str()), WS_CHILD | WS_VISIBLE | WS_BORDER, xPos + 74, 10, 600, height, hDlg, (HMENU)(BROWSE_TO_CSV_FILE_PATH), NULL, NULL);
	browseToCSV = CreateWindow(TEXT("BUTTON"), TEXT("open csv"), WS_VISIBLE | WS_CHILD, xPos + 676, 10, 100, 35, hDlg, (HMENU)IDC_BROWSE_TO_CSV_BUTTON, NULL, NULL);

	int sqlButtonLevel = height * 2;
	CreateWindow(TEXT("STATIC"), TEXT("DB path"), WS_CHILD | WS_VISIBLE, xPos, sqlButtonLevel, 64, checkSize, hDlg, (HMENU)(BROWSE_TO_SQL_HEADER), NULL, NULL);
	CreateWindow(TEXT("Edit"), TEXT(testPath2.c_str()), WS_CHILD | WS_VISIBLE | WS_BORDER, xPos + 74, sqlButtonLevel, 600, height, hDlg, (HMENU)(BROWSE_TO_SQL_FILE_PATH), NULL, NULL);
	browseToSQL = CreateWindow(TEXT("BUTTON"), TEXT("open sql"), WS_VISIBLE | WS_CHILD, xPos + 676, sqlButtonLevel, 100, 35, hDlg, (HMENU)IDC_BROWSE_TO_SQL_BUTTON, NULL, NULL);

	int processButtonLevel = height * 4;
	processButton = CreateWindow(TEXT("BUTTON"), TEXT("process"), WS_VISIBLE | WS_CHILD, xPos, processButtonLevel, 100, 35, hDlg, (HMENU)IDC_PROCESS_BUTTON, NULL, NULL);

	
}
//need to know what table in the db we will add info to
void CSVImporterWIndow::InitSQLTableWindow(HWND hDlg)
{
	int sqlButtonLevel = height * 6;
	CreateWindow(TEXT("STATIC"), TEXT("Which Table"), WS_CHILD | WS_VISIBLE, xPos, sqlButtonLevel, 64, checkSize, hDlg, (HMENU)(PICK_DATABASE_TABLE_TEXT), NULL, NULL);
	dbTableDropDown = CreateWindow(TEXT("COMBOBOX"), TEXT(""), WS_VISIBLE | WS_CHILD | CBS_DROPDOWNLIST |  CBS_DROPDOWN | CBS_HASSTRINGS | WS_VSCROLL, 
		xPos + 74, sqlButtonLevel, 300, height*10, hDlg, (HMENU)(PICK_DATABASE_TABLE), NULL, NULL);

	for (size_t j = 0; j < sqlHandler->tables.size(); j++)
	{
		SendMessage(dbTableDropDown, CB_ADDSTRING, 0, (LPARAM) sqlHandler->tables[j].c_str());
	}
}
void CSVImporterWIndow::InitDataWindow(HWND hDlg)
{
	CreateWindow(TEXT("STATIC"), TEXT("CSV Hedaer"), WS_CHILD | WS_VISIBLE, xPos, 30, 80, checkSize, hDlg, (HMENU)(CHECKBOX_INDEX - 1), NULL, NULL);
	for (size_t i = 0; i < csvImporter->csvHeaderVec.size(); i++)
	{
		OrigCSVHeaderItem newItem;
		newItem.checkBox = CreateWindow(TEXT("BUTTON"), TEXT(csvImporter->csvHeaderVec[i].c_str()), WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, xPos, yPos+(spacing*(i+1)), width+(csvImporter->csvHeaderVec[i].size()*8), checkSize, hDlg, (HMENU)(CHECKBOX_INDEX+i), NULL, NULL);
		
		//NOTICE THE HEIGHT OF THIS THING! IF ITS LITTLE THE DROP DOWN ISNT SHOWN!
		newItem.inputField = CreateWindow(TEXT("COMBOBOX"), TEXT(""), WS_VISIBLE | WS_CHILD | CBS_DROPDOWNLIST |  CBS_DROPDOWN | CBS_HASSTRINGS | WS_VSCROLL,
			xPos +  width+(csvImporter->csvHeaderVec[i].size()*8)+10, yPos+(spacing*(i+1)),300 ,height*10*(csvImporter->csvHeaderVec.size()) , hDlg, (HMENU)(INPUTFILED_INDEX+i), NULL, NULL);

		newItem.csvHeaderName = csvImporter->csvHeaderVec[i];
		newItem.csvIndex = i;
		//if (i == 0)
		//	SendMessage(newItem.checkBox, BM_SETCHECK, BST_CHECKED, 0);

		OrigCSVDataStructList.push_back(newItem);
		for (size_t j = 0; j < sqlHandler->cols.size(); j++)
		{
			SendMessage(newItem.inputField, CB_ADDSTRING, 0, (LPARAM) sqlHandler->cols[j].c_str());
		}
	}
		
	importButton = CreateWindow(TEXT("BUTTON"), TEXT("go!"), WS_VISIBLE | WS_CHILD, xPos,yPos+(spacing*(csvImporter->csvHeaderVec.size()+3)), 100, 35, hDlg, (HMENU)IDC_IMPORT_CSV_BUTTON, NULL, NULL);
}

void CSVImporterWIndow::ImportCSVDataToDB()
{
	vector<int> csvFields;
	vector<string> csvData;
	vector<string> dbFields;

	for (size_t i = 0; i < OrigCSVDataStructList.size(); i++)
	{
		//clear used flag
		OrigCSVDataStructList[i].enabled = false;
		UINT t = SendMessage(OrigCSVDataStructList[i].checkBox, BM_GETCHECK, 0, 0);
		if (t == 1)//item is checked
		{
			//get DB col to insert data into
			string curDropdownValue = GetSelectedValueFromDropdown(OrigCSVDataStructList[i].inputField);
			if (!curDropdownValue.empty())
			{
				dbFields.push_back(curDropdownValue);
				OrigCSVDataStructList[i].enabled = true;
				csvFields.push_back(i);
			}
		}
	}
	//use teh indexes of the csv  to get the actual data
	csvImporter->FillCSVDataVec(csvFields);

	//now go thru each csv line
	for (size_t i = 0; i < csvImporter->CSVEntryData.size(); i++)
	{
		csvData.clear();
		for (size_t j = 0; j < csvImporter->NewCSVDataStructList.size(); j++)
		{
			csvImporter->GetDataFromCSVLine(csvImporter->CSVEntryData[i], csvImporter->NewCSVDataStructList[j].csvIndexOrigin,csvData);
		}

		sqlHandler->InsertData(tableName,dbFields,csvData);
	}

	
}

void CSVImporterWIndow::OutpuNewCSV(string path)
{

	int headerNameMaxSize = 100;
	char* csvHeaderBuffer = new char[headerNameMaxSize];

	vector<string> usedCSVHeaders;

	string headerString;//this willbe printed inside the csv file

	for (size_t i = 0; i < OrigCSVDataStructList.size(); i++)
	{
		UINT t = SendMessage(OrigCSVDataStructList[i].checkBox, BM_GETCHECK, 0, 0);
		if (t == 1)//item is checked
		{
			int curInputFiledIndex = (INPUTFILED_INDEX + i);
			int len = GetWindowTextLength(GetDlgItem(mainWindowHandle, curInputFiledIndex));
			int numHeaderChars = GetDlgItemText(mainWindowHandle,curInputFiledIndex , csvHeaderBuffer, headerNameMaxSize);
			if (numHeaderChars > 0)
			{
				string newName = csvHeaderBuffer;
				usedCSVHeaders.push_back(newName);
				OrigCSVDataStructList[i].enabled = true;
			}
			csvHeaderBuffer[0] = '\0';
		}
		else
		{
			OrigCSVDataStructList[i].enabled = false;
		}
	}
	if (headerString.empty())
	{
		MessageBox(NULL, _T("you didnt specify what thigns you wanted"),_T("error"),MB_OK|MB_SYSTEMMODAL);
		return;
	}
	csvImporter->OutpuNewCSV(path,usedCSVHeaders);
}

BOOL CSVImporterWIndow::CheckCsvFileInput()
{
	char buffer[MAX_PATH];
	GetDlgItemText(mainWindowHandle, BROWSE_TO_CSV_FILE_PATH, buffer, MAX_PATH);
	//fill string with data in text box (incase they typed in a path)
	csvFilePath = (char *)buffer;

	if (csvFilePath.empty())
		return FALSE;

	if (csvFilePath.find(".csv") != csvFilePath.size() - 4)
		return FALSE;

	return TRUE;
}

BOOL CSVImporterWIndow::CheckSqlFileInput()
{
	char buffer[MAX_PATH];
	GetDlgItemText(mainWindowHandle, BROWSE_TO_SQL_FILE_PATH, buffer, MAX_PATH);
	//fill string with data in text box (incase they typed in a path)
	dbFilePath = (char *)buffer;

	if (dbFilePath.empty())
		return FALSE;
	
	if (dbFilePath.find(".db") != dbFilePath.size() - 3)
		return FALSE;

	return TRUE;
}

void CSVImporterWIndow::ProcessInputFiles()
{
	//dont check for valid files in here since we do that in other methods
	
	if (csvImporter == nullptr)
		csvImporter = new CSVImporter(csvFilePath);
	else
		csvImporter->LoadCSV(csvFilePath);

	if (sqlHandler == nullptr)
		sqlHandler = new SQLHandler(dbFilePath);
	else
		sqlHandler->ctlr.OpenDB(dbFilePath);

	InitSQLTableWindow(mainWindowHandle);
}

void CSVImporterWIndow::FillInAllData()
{
	tableName = GetSelectedValueFromDropdown(dbTableDropDown);
	sqlHandler->GetAllColsInTable(tableName);
	InitDataWindow(mainWindowHandle);
}
BOOL CSVImporterWIndow::CheckInput()
{
	//make sure we have a csv, a db file and some sort of data filled out
	return FALSE;
}