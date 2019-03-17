#include "stdafx.h"
#include "CSVImporter.h"
#include "StringUtils.h"
#include "DateTime.h"


#include <fstream>
#include <iostream>
using namespace std;

CSVImporter::CSVImporter(string path,string delim)
{
	LoadCSV(path, delim);
}

void CSVImporter::LoadCSV(string path,string delim)
{
	if (delim.empty())
		delim = internalDelim;
	else
		internalDelim = delim;

	csvHeaderVec.clear();

	csvPath = path;

	ifstream myfile (path);
	string line;

	if (!myfile.is_open())
		return;
	
	bool headerLine = true;

	while ( getline (myfile,line) )
	{
		if(headerLine)
		{
			csvHeaderVec = StringUtils::Tokenize(line,delim);
			headerLine = false;
		}
		else
		{
			CSVEntryData.push_back(line);
		}
	}
	myfile.close();

	isLoaded = true;
}

//get and cobine data from this csv row 
string CSVImporter::GetDataFromCSVLine(string csvLine, vector<int> indexes)
{
	string ret;
	vector<string> tokens = StringUtils::Tokenize(csvLine, internalDelim);
	for (size_t i = 0; i < indexes.size(); i++)
	{
		ret += tokens[indexes[i]] + " ";
	}
	ret.pop_back();
	return ret;
}

void CSVImporter::GetDataFromCSVLine(string csvLine, vector<int> indexes, vector<string>&retData)
{
	vector<string> tokens = StringUtils::Tokenize(csvLine, internalDelim);
	for (size_t i = 0; i < indexes.size(); i++)
	{
		retData.push_back( tokens[indexes[i]]);
	}
}

void CSVImporter::GetDataFromCSVLine(string csvLine, vector<string>&retData)
{
	vector<string> tokens = StringUtils::Tokenize2(csvLine, internalDelim);
	//count the delmiters to check if we have some empty fields...empty fields throw off the indexing
	int numDelims = 0;
	size_t found = 0;
	while(found < csvLine.size())
	{
		found = csvLine.find(internalDelim, found + 1);
		if (found != string::npos)
			numDelims++;
	}
	//got some empty data....
	if (tokens.size() != numDelims + 1)
	{

	}

	//when the data given has some blank fields at the end, fill in the gaps
	if (tokens.size() < NewCSVDataStructList.size())
	{
		for (size_t i = 0; i < NewCSVDataStructList.size() - tokens.size(); i++)
			tokens.push_back("");
	}
	
	

	for (size_t i = 0; i < NewCSVDataStructList.size(); i++)
	{
		for (size_t j = 0; j < NewCSVDataStructList[i].csvIndexOrigin.size(); j++)
		{
			int curDataIndex = NewCSVDataStructList[i].csvIndexOrigin[j];
			string curToken;
			if (NewCSVDataStructList[i].isDate)
			{
				DateTime date(tokens[curDataIndex]);
				curToken = date.ToString();
			}
			else
			{
				if (curDataIndex < tokens.size())
					curToken = tokens[curDataIndex];
				else
					curToken = "";
			}
			retData.push_back( curToken);
		}
		
	}
}

bool CSVImporter::VectorContains(vector<string> & vec,string word)
{
	for (size_t i = 0; i < vec.size(); i++)
	{
		if (vec[i] == word)
			return true;
	}
	return false;
}

//find all the data that will be under a header. in most cases, this will be 1 entry
//but there are times where 2+ feilds in the old csv will be combined into 1  in the new csv
vector<int> CSVImporter::CollectIndexesForDataEntry(vector<string> & vec,string newFieldName)
{
	vector<int> ret;
	for (size_t i = 0; i < vec.size(); i++)
	{
		if (vec[i] == newFieldName)
			ret.push_back(i);
	}
	return ret;
}

void CSVImporter::FillCSVDataVec(vector<int> &usedCSVHeader )
{
	NewCSVDataStructList.clear();
	

	for (size_t i = 0; i < usedCSVHeader.size(); i++)
	{				
		NewCSVHeaderItem newItem;
		newItem.csvHeaderName =  csvHeaderVec[usedCSVHeader[i]];
		newItem.csvIndexOrigin.push_back(usedCSVHeader[i]);

		NewCSVDataStructList.push_back(newItem);
	}
}

void CSVImporter::AddNewCSVHeaderItem(string name,int index,bool _isDate, int dataComboType)
{
	NewCSVHeaderItem *newItem = nullptr;
	//check to see if this entry is already in the list
	for (size_t i = 0; i < NewCSVDataStructList.size(); i++)
	{
		if (name == NewCSVDataStructList[i].csvHeaderName)
			newItem = &NewCSVDataStructList[i];
	}
	if (newItem == nullptr)
	{
		NewCSVHeaderItem nextItem;
		NewCSVDataStructList.push_back(nextItem);
		newItem = &NewCSVDataStructList.back();
	}

	newItem->csvHeaderName = name;
	newItem->csvIndexOrigin.push_back(index);
	newItem->dataCombineType = dataComboType;
	newItem->isDate = _isDate;
}

void CSVImporter::AddNewCSVHeaderItem(NewCSVHeaderItem newItem )
{
	NewCSVDataStructList.push_back(newItem);	
}

void CSVImporter::ClearHeaders()
{
	NewCSVDataStructList.clear();
}