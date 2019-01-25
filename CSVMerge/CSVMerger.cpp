#include "stdafx.h"
#include "CSVReader.h"
#include "StringUtils.h"
#include "Resource.h"

#include <fstream>
#include <iostream>
using namespace std;

CSVMerger::CSVMerger(string path,string newPath,string delim)
{
	if (delim.empty())
		delim = internalDelim;
	newFilePath = newPath;

	csvHeaderVec.clear();
	
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

void CSVMerger::InitMainWindow(HWND hDlg)
{	
	mainWindowHandle = hDlg;
	int xPos = 25,yPos = 50;
	int checkSize = 25;
	int spacing = 35;
	int width = 50, height = 25;
	//64 is 8 chars x 8pixels per char
	CreateWindow(TEXT("STATIC"), TEXT("CSV Path"), WS_CHILD | WS_VISIBLE, xPos, 10, width+64, 20, hDlg, (HMENU)(CHECKBOX_INDEX-3), NULL, NULL);
	CreateWindow(TEXT("Edit"), TEXT(""), WS_CHILD | WS_VISIBLE | WS_BORDER ,xPos +74,10,600 ,height , hDlg, (HMENU)(INPUTFILED_INDEX-3), NULL, NULL);

	CreateWindow(TEXT("STATIC"), TEXT("SQLite DB path"), WS_CHILD | WS_VISIBLE, xPos, height*2, 64, checkSize, hDlg, (HMENU)(CHECKBOX_INDEX-2), NULL, NULL);
	CreateWindow(TEXT("Edit"), TEXT(""), WS_CHILD | WS_VISIBLE | WS_BORDER ,xPos + 74, height*2,600 ,height , hDlg, (HMENU)(INPUTFILED_INDEX-2), NULL, NULL);

	//CreateWindow(TEXT("TEXT"), TEXT("SQLite DB path"), WS_CHILD | WS_VISIBLE, xPos, yPos-(spacing*(i+1)), width+(csvHeaderVec[i].size()*8), checkSize, hDlg, (HMENU)(CHECKBOX_INDEX-1), NULL, NULL);
	//CreateWindow(TEXT("Edit"), TEXT(""), WS_CHILD | WS_VISIBLE | WS_BORDER ,xPos +  width+(csvHeaderVec[i].size()*8)+10, yPos+(spacing*(i+1)),300 ,height , hDlg, (HMENU)(INPUTFILED_INDEX+i), NULL, NULL);

	CreateWindow(TEXT("TEXT"), TEXT("CSV Hedaer"), WS_CHILD | WS_VISIBLE, xPos, 30, 80, checkSize, hDlg, (HMENU)(CHECKBOX_INDEX-1), NULL, NULL);
	

	for (size_t i = 0; i < csvHeaderVec.size(); i++)
	{
		OrigCSVHeaderItem newItem;
		newItem.checkBox = CreateWindow(TEXT("BUTTON"), TEXT(csvHeaderVec[i].c_str()), WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, xPos, yPos+(spacing*(i+1)), width+(csvHeaderVec[i].size()*8), checkSize, hDlg, (HMENU)(CHECKBOX_INDEX+i), NULL, NULL);
		newItem.inputField = CreateWindow(TEXT("Edit"), TEXT(""), WS_CHILD | WS_VISIBLE | WS_BORDER ,xPos +  width+(csvHeaderVec[i].size()*8)+10, yPos+(spacing*(i+1)),300 ,height , hDlg, (HMENU)(INPUTFILED_INDEX+i), NULL, NULL);
		newItem.csvHeaderName = csvHeaderVec[i];
		newItem.csvIndex = i;
		//if (i == 0)
		//	SendMessage(newItem.checkBox, BM_SETCHECK, BST_CHECKED, 0);

		OrigCSVDataStructList.push_back(newItem);
		//newItem.dropDown = CreateWindow(TEXT("COMBOBOX"),TEXT("csv hedaer"), WS_VISIBLE | WS_CHILD | CBS_DROPDOWNLIST |  CBS_DROPDOWN | CBS_HASSTRINGS | WS_VSCROLL,
		//	xPos + checkSize,yPos+(spacing*(i+1)),width,height,hDlg,(HMENU)DROPDOWN_INDEX+i, NULL, NULL);
		//SetDlgItemText(mainWindowHandle, (INPUTFILED_INDEX+i), "This is a string");
	}
		
	mergeButton = CreateWindow(TEXT("BUTTON"), TEXT("MERGE!"), WS_VISIBLE | WS_CHILD, xPos,yPos+(spacing*(csvHeaderVec.size()+3)), 100, 35, hDlg, (HMENU)IDC_MERGE_CSV_BUTTON, NULL, NULL);
}

//get and cobine data from this csv row 
string CSVMerger::GetDataFromCSVLine(string csvLine, vector<int> indexes)
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

bool VectorContains(vector<string> & vec,string word)
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
vector<int> CollectIndexesForDataEntry(vector<string> & vec,string newFieldName)
{
	vector<int> ret;
	for (size_t i = 0; i < vec.size(); i++)
	{
		if (vec[i] == newFieldName)
			ret.push_back(i);
	}
	return ret;
}

void CSVMerger::OutpuNewCSV(string path)
{
	string outputDelim = ";";// internalDelim;
	string line;
	//first lets create the new header
	int headerNameMaxSize = 100;
	char* csvHeaderBuffer = new char[headerNameMaxSize];
	NewCSVDataStructList.clear();
	
	string headerString;//this willbe printed inside the csv file

	for (size_t i = 0; i < OrigCSVDataStructList.size(); i++)
	{
		UINT t = SendMessage(OrigCSVDataStructList[i].checkBox, BM_GETCHECK, 0, 0);
		if (t == 1)//item is checked
		{
			int curInputFiledIndex = (INPUTFILED_INDEX + i);
			int len = GetWindowTextLength(GetDlgItem(mainWindowHandle, curInputFiledIndex));
			int numHeaderChars = GetDlgItemText(mainWindowHandle,curInputFiledIndex , csvHeaderBuffer, headerNameMaxSize);
			if (numHeaderChars > 0)
			{
				string newName = csvHeaderBuffer;
				headerString += newName + outputDelim;
				
				NewCSVHeaderItem newItem;
				newItem.csvHeaderName = newName;
				newItem.csvIndexOrigin.push_back(i);

				NewCSVDataStructList.push_back(newItem);
				OrigCSVDataStructList[i].enabled = true;
			}
			csvHeaderBuffer[0] = '\0';
		}
		else
		{
			OrigCSVDataStructList[i].enabled = false;
		}
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
	map<string, vector<int>>::iterator it;
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
/*
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
*/
BOOL CSVMerger::CheckInput(WPARAM wParam, string &output)
{
	if (wParam == IDC_MERGE_CSV_BUTTON)
	{
		OutpuNewCSV(newFilePath);
		return (INT_PTR)TRUE;
	}
	
	
	return FALSE;
}