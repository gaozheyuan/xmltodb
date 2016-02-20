#include <string>
#include <algorithm>
#include <iostream>
using namespace std;
#pragma once
class DBStringProcessor
{
public:
	DBStringProcessor();
	~DBStringProcessor();
	static string getMainKeyString(string tableName);
	static string getForeignKeyString(string referTableName, string referColumn);
	static string getOriginalTrueTableName(string originTableName);
	static string getLowerCaseString(string originalString);
};

