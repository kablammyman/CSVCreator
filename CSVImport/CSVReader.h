#pragma once
#include <Windows.h>

#include <string>
#include <vector>

struct CSVHeaderItem
{
	std::string csvHeaderName;
	std::string newCsvName; //the new header field to put data into
	int dropDownValue;
	int csvIndex;
	bool enabled;
	HWND checkBox;
	HWND inputField;
	void Clear(void)
	{
		csvHeaderName.clear();
		newCsvName.clear();
	}
};

extern std::vector<CSVHeaderItem>CSVList;
extern HINSTANCE mainInst;        

extern HWND mainWindowHandle;


extern std::vector<std::string> csvHeaderVec; 
extern std::vector<std::string> CSV;
bool FillCSVHeaders(std::string path, std::vector<std::string> &csvHeaderVec, std::string delim= "\t");
void InitMainWindow(HWND hDlg, std::vector<std::string> &csvHeader);
bool SaveCSV(std::string path);
bool LoadCSV(std::string path,char delim= '\t');