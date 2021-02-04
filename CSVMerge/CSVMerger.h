#pragma once
#include <Windows.h>

#include <string>
#include <vector>
#include <map>
class CSVMerger
{
	struct OrigCSVHeaderItem
	{
		std::string csvHeaderName;
		//int dropDownValue;
		int csvIndex;
		bool enabled;
		HWND checkBox;
		HWND inputField;

	};
	struct NewCSVHeaderItem
	{
		std::string csvHeaderName;
		std::vector<int> csvIndexOrigin;
		int dataCombineType;//will we just concat strings, or dop something else
	};
	HINSTANCE mainInst;        
	HWND mainWindowHandle;
	HWND mergeButton;

	bool isLoaded = false;
	std::string internalDelim = "\t";
	std::string newFilePath;

	std::vector<OrigCSVHeaderItem>OrigCSVDataStructList;
	std::vector<NewCSVHeaderItem>NewCSVDataStructList;

	std::vector<std::string> csvHeaderVec; 
	std::vector<std::string> CSVEntryData;

	std::string GetDataFromCSVLine(std::string csvLine, std::vector<int> indexes);
	public:
		bool IsDataLoaded() { return isLoaded; };
		CSVMerger(std::string path, std::string newFile, std::string delim= "");
		void InitMainWindow(HWND hDlg);
		bool SaveCSV(std::string path);
		bool LoadCSV(std::string path,char delim= ',');
		void OutpuNewCSV(std::string path);
		BOOL CheckInput(WPARAM wParam, std::string &output);
};