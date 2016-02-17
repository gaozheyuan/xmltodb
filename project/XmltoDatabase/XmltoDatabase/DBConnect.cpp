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
			+ " int NOT NULL AUTO_INCREMENT,";
		if (hasReferTable)
		{
			statement = statement
				+ DBStringProcessor::getForeignKeyString(referTableName, referColumn)
				+ " int,";
		}
		for (int i = 0; i < columns.size(); i++)
		{
			statement += columns[i];
			statement += " varchar(255),";
		}
		statement = statement
			+ "PRIMARY KEY("
			+ DBStringProcessor::getMainKeyString(tableName)
			+ ")";
		if (hasReferTable)
		{
			statement = statement
				+ ","
				+ statement = statement
				+ "FOREIGN KEY("
				+ DBStringProcessor::getForeignKeyString(referTableName, referColumn)
				+ ") REFERENCES "
				+ referTableName
				+ "("
				+ referColumn
				+ ")";
		}
		statement = statement + ");";
		stmt->execute(statement.c_str());
	}
	catch (SQLException &exception)
	{
		cerr << "Create table " + tableName + " failed!" << endl;
		return EXIT_FAILURE;
	}
}
cout << "Create table " + tableName + " success!" << endl;
return EXIT_SUCCESS;
}

bool DBConnect::existTable(string tableschema, string tableName)
{
	try
	{
		string subStr = tableName.substr(tableName.find(':') + 1, string::npos);
		stmt = con->createStatement();
		string statement = "show tables like '" + subStr + "'";
		res = stmt->executeQuery(statement.c_str());
		if (res->rowsCount() > 0)
			return true;
		else
			return false;
	}
	catch (SQLException &exception)
	{
		cerr << "Finding table " + tableName + " failed!";
		return EXIT_FAILURE;
	}
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