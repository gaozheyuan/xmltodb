#include <stdlib.h>
#include <iostream>
#include <vector>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include "DBStringProcessor.h"
#include "mysql_connection.h"
#pragma once
using namespace sql;
using namespace std;
class DBConnect
{
private:
	sql::Driver *driver;
	sql::Connection *con;
	sql::Statement *stmt;
	sql::ResultSet *res;
	string dbSchema;
public:
	bool enableConnection(string hostName, string userName, string password);
	bool createTable(bool hasReferTable,string tableName, vector<string> columns,string referTableName="",string referColumn="");
	bool existTable(string tableschema,string tableName);
	void setDbSchema(string dbSchema);
	string getDbSchema();
	DBConnect();
	~DBConnect();
};

