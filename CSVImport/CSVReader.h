#pragma once
#include <Windows.h>

#include <string>
#include <vector>
#include <map>
class CSVMerger
{
	struct CSVHeaderItem
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
	HWND mergeButton;

	bool isLoaded = false;
	std::string internalDelim = "\t";
	std::string newFilePath;

	std::vector<CSVHeaderItem>CSVList;
	std::vector<std::string> csvHeaderVec; 
	std::vector<std::string> CSVEntryData;

	//all the NEW csv header values, and where to get the data from the old csv
	std::map<std::string, std::vector<int>> CSVDataIndexes;

	std::string GetDataFromCSVLine(std::string csvLine, std::vector<int> indexes);
	public:
		bool IsDataLoaded() { return isLoaded; };
		CSVMerger(std::string path, std::string newFile, std::string delim= "");
		void InitMainWindow(HWND hDlg);
		bool SaveCSV(std::string path);
		bool LoadCSV(std::string path,char delim= '\t');
		void OutpuNewCSV(std::string path);
		BOOL CheckInput(WPARAM wParam, std::string &output);
};