#include "stdafx.h"
#include "SQLHandler.h"

using namespace std;

SQLHandler::SQLHandler(string path)
{
	ctlr.OpenDB(path);
	ctlr.GetAllTablesInDB(tables);
}

void SQLHandler::GetAllColsInTable(std::string table)
{
	ctlr.GetAllColsInTable(table, cols);
}

void SQLHandler::InsertData(string table, std::vector<std::string> & dbFields,std::vector<std::string> & csvValues)
{
	//bool InsertNewDataEntry(string table, dbDataPair data, string &output);
	//bool 
	string output;
	vector<DatabaseController::dbDataPair> data;

	for (size_t i = 0; i < dbFields.size(); i++)
	{
		DatabaseController::dbDataPair temp = make_pair( dbFields[i],csvValues[i]);
		data.push_back(temp);
	}

	ctlr.InsertNewDataEntry(table,data, output);
}