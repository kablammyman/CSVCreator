#pragma once
#include <string>
#include <vector>
#include "SQLiteUtils.h"
#include "DatabaseController.h"

class SQLHandler
{
public:
	std::string dbPath;
	std::vector<std::string> tables;
	std::vector<std::string> cols;
	DatabaseController ctlr;

	SQLHandler(std::string path);
	void GetAllColsInTable(std::string table);
	void InsertData(std::string table,std::vector<std::string> & dbFields,std::vector<std::string> & csvValues);
	void Load(std::string path);
	void Clear();
};