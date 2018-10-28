#include "stdafx.h"
#include "CSVReader.h"
#include "StringUtils.h"
#include "Resource.h"

#include <fstream>

using namespace std;


HWND mainWindowHandle;
vector<CSVHeaderItem>CSVList;
HINSTANCE mainInst; 

vector<string> CSV; 
string internalDelim = "|";

typedef pair <string, string> CSVDataPair;


bool FillCSVHeaders(string path,vector<string> &csvHeaderVec, string delim)
{
	csvHeaderVec.clear();
	
	ifstream myfile (path);
	string line;

	if (!myfile.is_open())
		return false;
	
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
			CSV.push_back(line);
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
	return true;
}

void InitMainWindow(HWND hDlg, vector<string> &csvHeader)
{	
	mainWindowHandle = hDlg;
	int xPos = 25,yPos = 10;
	int checkSize = 25;
	int spacing = 35;
	int width = 50, height = 25;
	for (int i = 0; i < csvHeader.size(); i++)
	{
		CSVHeaderItem newItem;

		newItem.checkBox = CreateWindow(TEXT("BUTTON"), TEXT(csvHeader[i].c_str()), WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, xPos, yPos+(spacing*(i+1)), width+(csvHeader[i].size()*8), checkSize, hDlg, (HMENU)CHECKBOX_INDEX+i, NULL, NULL);
		newItem.inputField = CreateWindow(TEXT("Edit"), TEXT(""), WS_CHILD | WS_VISIBLE | WS_BORDER ,xPos +  width+(csvHeader[i].size()*8)+10, yPos+(spacing*(i+1)),300 ,height , hDlg, (HMENU)INPUTFILED_INDEX+i, NULL, NULL);
		newItem.csvHeaderName = csvHeader[i];
		newItem.csvIndex = i;
		CSVList.push_back(newItem);
		//newItem.dropDown = CreateWindow(TEXT("COMBOBOX"),TEXT("csv hedaer"), WS_VISIBLE | WS_CHILD | CBS_DROPDOWNLIST |  CBS_DROPDOWN | CBS_HASSTRINGS | WS_VSCROLL,
		//	xPos + checkSize,yPos+(spacing*(i+1)),width,height,hDlg,(HMENU)DROPDOWN_INDEX+i, NULL, NULL);
		

	}
		
	//numEntriesView = CreateWindow(TEXT("STATIC"), TEXT(GetNumEntriesText().c_str()), WS_CHILD | WS_VISIBLE ,X + 10+ (textAreaWidth * 2.5), Y+(textHeeight*2),100 ,25 , hDlg, (HMENU)IDC_CSV_HEADER, NULL, NULL);
	//



}