#include "DBConnect.h"
DBConnect::DBConnect()
{
}
DBConnect::~DBConnect()
{
}
bool DBConnect::enableConnection(string hostName, string userName, string password)
{
	try
	{
		driver = get_driver_instance();
		con = driver->connect(SQLString(hostName), SQLString(userName), SQLString(password));
		/* Connect to the MySQL test database */
		con->setSchema(dbSchema);
	}
	catch (SQLException &exception)
	{
		cerr << "Unable to establish connection to database, please check the parameters!" << endl;
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}
bool DBConnect::createTable(bool hasReferTable, string tableName, vector<string> columns, string referTableName, string referColumn)
{
	try
	{
		stmt = con->createStatement();
		string statement =
			"CREATE TABLE "
			+ tableName
			+ "("
			+ DBStringProcessor::getMainKeyString(tableName)
			+ " int NOT NULL AUTO_INCREMENT";
		if (hasReferTable)
		{
			statement = statement+","
				+ DBStringProcessor::getForeignKeyString(referTableName, referColumn)
				+ " int";
		}
		for (int i = 0; i < columns.size(); i++)
		{
			statement += ",";
			statement += columns[i];
			statement += " TEXT";
		}
		statement = statement
			+ ","
			+ "PRIMARY KEY("
			+ DBStringProcessor::getMainKeyString(tableName)
			+ ")";
		if (hasReferTable)
		{
			statement = statement
				+ ","
				+ statement = statement
				+ ", FOREIGN KEY("
				+ DBStringProcessor::getForeignKeyString(referTableName, referColumn)
				+ ") REFERENCES "
				+ referTableName
				+ "("
				+ referColumn
				+ ")";
		}
		statement = statement + ");";
		cout << statement << endl;
		stmt->execute(statement.c_str());
	}
	catch (SQLException &exception)
	{
		cerr << "Create table " + tableName + " failed!" << endl;
		stmt->close();
		return EXIT_FAILURE;
	}
	stmt->close();
	cout << "Create table " + tableName + " success!" << endl;
	return EXIT_SUCCESS;
}
bool DBConnect::existTable(string tableName)
{
	try
	{
		stmt = con->createStatement();
		string statement = "show tables like '" + tableName + "'";
		res = stmt->executeQuery(statement.c_str());
		if (res->rowsCount() > 0)
			return true;
		else
			return false;
	}
	catch (SQLException &exception)
	{
		cerr << "Finding table " + tableName + " failed!";
		stmt->close();
		return EXIT_FAILURE;
	}
	stmt->close();
	return EXIT_SUCCESS;
}
void DBConnect::setDbSchema(string dbSchema)
{
	this->dbSchema = dbSchema;
}
string DBConnect::getDbSchema()
{
	return dbSchema;
}
bool DBConnect::insertIntoTable(bool hasReferTable, string tableSchema, string tableName, vector<pair<string, string>> columnAndValue, string referTable, string referColumn , string referKeyValue )
{
	try
	{
		stmt = con->createStatement();
		string statement =
			"INSERT INTO "
			+ tableName
			+ "(";
		for (int i = 0; i < columnAndValue.size(); i++)
		{
			statement = statement + columnAndValue[i].first;
			if (i < columnAndValue.size() - 1)
				statement = statement + ",";
		}
		if (hasReferTable)
		{
			if (columnAndValue.size() != 0)
				statement = statement + ",";
			statement= statement+DBStringProcessor::getForeignKeyString(referTable, referColumn);
		}

		statement= statement +") VALUES ( ";
		for (int i = 0; i < columnAndValue.size(); i++)
		{
			statement = statement + "'"+columnAndValue[i].second+"'";
			if (i < columnAndValue.size() - 1)
				statement = statement + ",";
		}
		if (hasReferTable)
		{
			if (columnAndValue.size() != 0)
				statement = statement + ",";
			statement = statement +"'"+ referKeyValue+"'";
		}
		statement = statement + ");";
		cout << statement << endl;
		stmt->execute(statement.c_str());
	}
	catch (SQLException &exception)
	{
		cerr << "Insert into table " + tableName + " failed!" << endl;
		cerr << exception.what() << endl;
		stmt->close();
		return EXIT_FAILURE;
	}
	stmt->close();
	cout << "Insert into table " + tableName + " success!" << endl;
	return EXIT_SUCCESS;
}
bool DBConnect::createSchema(string schemaName)
{
	stmt = con->createStatement();
	string statement = "CREATE SCHEMA " + schemaName + ";";
	stmt->execute(statement.c_str());
}
bool DBConnect::createTableOfHashNametoTrueName()
{
	vector<string> column;
	column.push_back("hashname");
	column.push_back("truetablename");
	column.push_back("tableindex");
	return createTable(false, HASHNAMETOTRUETABLENAME, column);
}
bool DBConnect::insertTableOfHashNametoTrueName(string hashname, string truename, string nameindex)
{
	vector<pair<string, string>> hashVec;
	hashVec.push_back(make_pair(HASHNAME, hashname));
	hashVec.push_back(make_pair(TRUETABLENAME, truename));
	hashVec.push_back(make_pair(NAMEINDEX, nameindex));
	return insertIntoTable(false, "testvs", HASHNAMETOTRUETABLENAME, hashVec);
}