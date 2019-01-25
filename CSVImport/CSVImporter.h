#pragma once
#include <Windows.h>

#include <string>
#include <vector>

class CSVImporter
{
	std::string csvPath;

	bool isLoaded = false;
	std::string internalDelim = "\t";
	std::vector<int> CollectIndexesForDataEntry(std::vector<std::string> & vec, std::string newFieldName);
	
	bool VectorContains(std::vector<std::string> & vec, std::string word);

public:
	struct NewCSVHeaderItem
	{
		std::string csvHeaderName;
		std::vector<int> csvIndexOrigin;
		int dataCombineType;//will we just concat strings, or dop something else
		bool isDate;
	};

	CSVImporter(std::string path, std::string delim= "");
	void LoadCSV(std::string path, std::string delim= "");

	std::string GetInternalDelim() {return internalDelim;};
	std::vector<NewCSVHeaderItem>NewCSVDataStructList;

	std::vector<std::string> csvHeaderVec; 
	std::vector<std::string> CSVEntryData;
	void FillCSVDataVec(std::vector<int> &usedCSVHeader);
	void AddNewCSVHeaderItem(NewCSVHeaderItem newItem);
	void AddNewCSVHeaderItem(std::string name, int index, bool _isDate, int dataComboType);
	std::string GetDataFromCSVLine(std::string csvLine, std::vector<int> indexes);
	void GetDataFromCSVLine(std::string csvLine, std::vector<int> indexes, std::vector<std::string>&retData);
	void GetDataFromCSVLine(std::string csvLine, std::vector<std::string>&retData);
	bool IsDataLoaded() { return isLoaded; };
	void ClearHeaders();
};