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
public:
	struct OrigCSVHeaderItem
	{
		std::string csvHeaderName;
		//int dropDownValue;
		int csvIndex;
		bool enabled;
		HWND checkBox;
		HWND inputField;
	};

	HINSTANCE mainInst;        
	HWND mainWindowHandle;
	//HWND dataWindowHandle;
	HWND importButton;
	HWND browseToCSV;
	HWND browseToSQL;
	HWND processButton;
	HWND dbTableDropDown;

	CSVImporter *csvImporter;
	SQLHandler *sqlHandler;

	std::vector<OrigCSVHeaderItem>OrigCSVDataStructList;

	CSVImporterWIndow();

	void InitMainWindow(HWND hDlg);
	void InitSQLTableWindow(HWND hDlg);
	void InitDataWindow(HWND nDlg);

	bool SaveCSV(std::string path);
	bool LoadCSV(std::string path,char delim= '\t');
	
	void OutpuNewCSV(std::string path);
	void ImportCSVDataToDB();
	
	BOOL CheckCsvFileInput();
	BOOL CheckSqlFileInput();
	void ProcessInputFiles();
	void FillInAllData();
	BOOL CheckInput();

};