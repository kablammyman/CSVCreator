#pragma once
#include <Windows.h>

#include <string>
#include <vector>
#include "SQLHandler.h"
#include "CSVImporter.h"


class CSVImporterWIndow
{
	int xPos = 25, yPos = 220;
	int checkSize = 25;
	int spacing = 35;
	int width = 50, height = 25;
	std::string csvFilePath, dbFilePath;
	std::string GetSelectedValueFromDropdown(HWND dropdown);
	std::string tableName;
	std::string delim;
public:

	//each csv entry will get one of these structs...the struct tells us what options the user selected\want
	struct OrigCSVHeaderItem
	{
		std::string csvHeaderName;
		int csvIndex;
		bool enabled;//when processing, did the user want ot use this by slecting an item on the drop down?
		HWND isDateCheckBox;//is this csv field a date? if so, we must make sure we convert it by using the DateTime class (in sqlite utils)
		HWND dropDownMenu;
	};

	HINSTANCE mainInst;        
	HWND mainWindowHandle;
	//HWND dataWindowHandle;
	HWND importButton;
	HWND browseToCSV;
	HWND browseToSQL;
	HWND processButton;
	HWND dbTableDropDown;
	HWND delimterType;

	CSVImporter *csvImporter;
	SQLHandler *sqlHandler;

	std::vector<OrigCSVHeaderItem>OrigCSVDataStructList;
	void ClearOrigCSVDataStructList();
	CSVImporterWIndow();

	void InitMainWindow(HWND hDlg);
	void InitSQLTableWindow(HWND hDlg);
	void InitDataWindow(HWND nDlg);

	void ImportCSVDataToDB();
	
	BOOL CheckCsvFileInput();
	BOOL CheckSqlFileInput();
	void ProcessInputFiles();
	void FillInAllData();
	BOOL CheckInput();

};