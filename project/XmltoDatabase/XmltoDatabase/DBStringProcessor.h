#include <string>
using namespace std;
#pragma once
class DBStringProcessor
{
public:
	DBStringProcessor();
	~DBStringProcessor();
	static string getMainKeyString(string tableName);
	static string getForeignKeyString(string referTableName, string referColumn);
};

