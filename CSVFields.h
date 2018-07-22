#pragma once

#include <string>
#include <vector>
#include <Windows.h>

#define NUM_CHECKS	5
#define MAX_LOADSTRING 100

#define buttonW  100
#define buttonH  40
#define BASE_X 30 
#define BASE_Y 20
#define winWidth 1000
#define winHeight 300

#define FIELD_WIDTH 500

struct QueryIDResults
{
	std::string CategoryID;
	std::string WebsiteID;
	std::string SubWebsiteID;
	std::vector<std::string> ModelID;
	std::vector<std::string> GalleryID;
	void Clear(void)
	{
		CategoryID.clear();
		WebsiteID.clear();
		SubWebsiteID.clear();
		ModelID.clear();
		GalleryID.clear();
	}
};

extern HINSTANCE mainInst;        

extern HWND mainWindowHandle;
extern HWND indivInputGroupHandle;
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
