#include <stdlib.h>
#include <iostream>
#include <vector>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include "DBStringProcessor.h"
#include "mysql_connection.h"
#define HASHNAMETOTRUETABLENAME "hashnametotruename"
#define HASHNAME "hashname"
#define TRUETABLENAME "truetablename"
#define NAMEINDEX "nameindex"
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
	//set up connection to mysql database
	bool enableConnection(string hostName, string userName, string password);
	//create schema in database
	bool createSchema(string schemaName);
	//create table in database
	bool createTable(bool hasReferTable,string tableName, vector<string> columns,string referTableName="",string referColumn="");
	//judge whether there is a table acoor
	bool existTable(string tableName);
	bool insertIntoTable(bool hasReferTable,string tableSchema, string tableName, vector<pair<string, string>> columnAndValue,string referTable="",string referColumn="",string referKeyValue="");
	void setDbSchema(string dbSchema);
	bool createTableOfHashNametoTrueName();
	bool insertTableOfHashNametoTrueName(string hashname, string truename, string nameindex);
	string getDbSchema();
	DBConnect();
	~DBConnect();
};

