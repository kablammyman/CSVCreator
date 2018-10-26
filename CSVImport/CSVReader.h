#pragma once
#include <Windows.h>

#include <string>
#include <vector>

struct CSVHeaderItem
{
	std::string csvHeaderName;
	std::string csvName;
	int dropDownValue;
	int csvIndex;
	bool enabled;

	void Clear(void)
	{
		csvHeaderName.clear();
	}
};

extern vector<CSVHeaderItem>CSVList;
extern HINSTANCE mainInst;        

extern HWND mainWindowHandle;

extern HWND indivInputGroup;

extern HWND *fieldName;
extern HWND *fieldText;

extern HWND csvHeader;
extern HWND csvHeaderText;

extern HWND editArea;
extern HWND templateArea;

extern HWND listView;
extern HWND numEntriesView;

extern std::vector<std::string> editTextVec;
extern std::vector<std::string> templateTextVec;
extern std::vector<std::string> csvHeaderVec; 
extern std::vector<std::string> CSV; 

	
INT_PTR CALLBACK InputIndvFieldsBox(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
void InitMainWindows(HWND hDlg);
BOOL CheckInput(WPARAM wParam, std::string &output);
HWND CreateListView(HWND hwndParent, std::vector<std::string> colNames,int x, int y, int w, int h, int size);
BOOL FillListViewItems(HWND hWndListView, std::vector<std::string> items);
bool SaveCSV(std::string path);
bool LoadCSV(std::string path,char delim= '\t');