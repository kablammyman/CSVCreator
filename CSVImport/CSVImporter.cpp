#include "stdafx.h"
#include "CSVImporter.h"
#include "StringUtils.h"


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

	/*for (int i = 0; i < csvHeaderVec.size(); i++)
	{
		CSVHeaderItem temp;
		temp.csvHeaderName = csvHeaderVec[i];
		temp.enabled = false;
		temp.csvIndex = i;
		CSVList.push_back(temp);
	}*/
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

void CSVImporter::OutpuNewCSV(string path,vector<string> &usedCSVHeader )
{
	string outputDelim = ";";// internalDelim;
	string line;
	NewCSVDataStructList.clear();
	
	string headerString;//this willbe printed inside the csv file

	for (size_t i = 0; i < usedCSVHeader.size(); i++)
	{
		string newName = usedCSVHeader[i];
		headerString += newName + outputDelim;
				
		NewCSVHeaderItem newItem;
		newItem.csvHeaderName = newName;
		newItem.csvIndexOrigin.push_back(i);

		NewCSVDataStructList.push_back(newItem);
	}

	if (headerString.empty())
	{
		MessageBox(NULL, _T("you didnt specify what thigns you wanted"),_T("error"),MB_OK|MB_SYSTEMMODAL);
		return;
	}
	headerString.pop_back();
	ofstream myfile (path);

	myfile << headerString << endl;
	//next start filling out the data

	for (size_t i = 0; i < CSVEntryData.size(); i++)
	{
		string newLine;
		for (size_t j = 0; j < NewCSVDataStructList.size(); j++)
		{
			newLine += GetDataFromCSVLine(CSVEntryData[i], NewCSVDataStructList[j].csvIndexOrigin) + outputDelim;
		}
		newLine.pop_back();
		myfile << newLine << endl;
	}

	
	myfile.close();
}