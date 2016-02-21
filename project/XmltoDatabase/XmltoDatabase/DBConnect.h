#include <stdlib.h>
#include <iostream>
#include <vector>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include "DBStringProcessor.h"
#include "mysql_connection.h"
#include "MACROS.h"
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
public:
	DBConnect();
	~DBConnect();
	//set up connection to mysql database
	bool enableConnection(string hostName, string userName, string password,string schemaName);
	//Do the initialization work
	bool initialize();
	//create schema in database
	bool createSchema(string schemaName);
	//create table in database
	bool createTable(bool hasReferTable,string tableName, vector<string> columns,string referTableName="",string referColumn="");
	//judge whether there is a schema in db
	bool existSchema(string schemaName);
	//judge whether there is a table in db
	bool existTable(string tableName);
	//insert data into table
	bool insertIntoTable(bool hasReferTable, string tableName, vector<pair<string, string>> columnAndValue,string referTable="",string referColumn="",string referKeyValue="");
	//create table and return mainKey
	bool insertIntoTableandReturnMainKey(bool hasReferTable, string tableName, vector<pair<string, string>> columnAndValue,string &mainKeyValue, string referTable = "", string referColumn = "", string referKeyValue = "");
	//create table of hash name to true name mapping
	bool createTableOfHashNametoTrueName();
	//insert table of hash name to true name mapping
	bool insertTableOfHashNametoTrueName(string hashname, string truename, string nameindex);
	//look up result from a specific table by querying existing known column name
	ResultSet* querybyExistingColumnName(string tableName, vector<pair<string, string>> columnAndValue);
	//find all column names in a database
	vector<string> findAllColumnsInDatabase(string tableName);
	//add new columns into a table
	bool addNewColumnsIntoTable(vector<string> newColumnVecs,string tableName);
};

