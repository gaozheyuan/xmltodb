#include "DBConnect.h"
DBConnect::DBConnect()
{
}
DBConnect::~DBConnect()
{
}
bool DBConnect::enableConnection(string hostName, string userName, string password, string schemaName)
{
	try
	{
		driver = get_driver_instance();
		con = driver->connect(SQLString(hostName), SQLString(userName), SQLString(password));
		/* Connect to the MySQL test database */
		if (!existSchema(schemaName))
			createSchema(schemaName);
		con->setSchema(schemaName);
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
			statement = statement + ","
				+ DBStringProcessor::getForeignKeyString(referTableName)
				+ " int";
		}
		for (int i = 0; i < columns.size(); i++)
		{
			statement += ",";
			statement += columns[i];
			statement += " LONGTEXT";
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
				+ DBStringProcessor::getForeignKeyString(referTableName)
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
		cerr << exception.what() << endl;
		stmt->close();
		return EXIT_FAILURE;
	}
	stmt->close();
	cout << "Create table " + tableName + " success!" << endl;
	return EXIT_SUCCESS;
}
bool DBConnect::initialize()
{
	if(!existTable(HASHNAMETOTRUETABLENAME))
		return createTableOfHashNametoTrueName();
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
bool DBConnect::insertIntoTable(bool hasReferTable, string tableName, vector<pair<string, string>> columnAndValue, string referTable, string referColumn, string referKeyValue)
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
			statement = statement + DBStringProcessor::getForeignKeyString(referTable);
		}

		statement = statement + ") VALUES ( ";
		for (int i = 0; i < columnAndValue.size(); i++)
		{
			statement = statement + "'" + columnAndValue[i].second + "'";
			if (i < columnAndValue.size() - 1)
				statement = statement + ",";
		}
		if (hasReferTable)
		{
			if (columnAndValue.size() != 0)
				statement = statement + ",";
			statement = statement + "'" + referKeyValue + "'";
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
bool DBConnect::insertIntoTableandReturnMainKey(bool hasReferTable, string tableName, vector<pair<string, string>> columnAndValue, string &mainKeyValue, string referTable, string referColumn, string referKeyValue)
{
	string statement;
	try
	{
		stmt = con->createStatement();
		statement =
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
			statement = statement + DBStringProcessor::getForeignKeyString(referTable);
		}

		statement = statement + ") VALUES ( ";
		for (int i = 0; i < columnAndValue.size(); i++)
		{
			statement = statement + "'" + columnAndValue[i].second + "'";
			if (i < columnAndValue.size() - 1)
				statement = statement + ",";
		}
		if (hasReferTable)
		{
			if (columnAndValue.size() != 0)
				statement = statement + ",";
			statement = statement + "'" + referKeyValue + "'";
		}
		statement = statement + ");";
		cout << statement << endl;
		stmt->execute(statement);
		ResultSet* resultSet = stmt->executeQuery("select last_insert_id();");
		while (resultSet->next())
		{
			mainKeyValue = resultSet->getString("last_insert_id()");
		}
	}
	catch (SQLException &exception)
	{
		cerr << "Insert into table " + tableName + " failed!" << endl;
		cerr << exception.getErrorCode() << endl;
		if (exception.getErrorCode() == COLUMNMISSINGCODE)
		{
			vector<string> existingColumns = findAllColumnsInDatabase(tableName);
			vector<string> missingColumns;
			for (int index = 0; index < columnAndValue.size(); index++)
			{
				vector<string>::iterator it=find(existingColumns.begin(), existingColumns.end(), columnAndValue[index].first);
				if (it == existingColumns.end())
					missingColumns.push_back(columnAndValue[index].first);
			}
			addNewColumnsIntoTable(missingColumns, tableName);
			stmt = con->createStatement();
			cout << statement << endl;
			try
			{
				stmt->execute(statement);
				ResultSet* resultSet = stmt->executeQuery("select last_insert_id();");
				while (resultSet->next())
				{
					mainKeyValue = resultSet->getString("last_insert_id()");
				}
			}
			catch (SQLException &e)
			{
				cerr << e.what();
			}
			stmt->close();
			return EXIT_SUCCESS;
		}
		stmt->close();
		return EXIT_FAILURE;
	}
	stmt->close();
	cout << "Insert into table " + tableName + " success!" << endl;
	return EXIT_SUCCESS;
}
bool DBConnect::createSchema(string schemaName)
{
	try
	{
		stmt = con->createStatement();
		string statement = "CREATE SCHEMA " + schemaName + ";";
		stmt->execute(statement.c_str());
	}
	catch (SQLException &exception)
	{
		cerr << "Create schema " + schemaName + " failed!" << endl;
		stmt->close();
		return EXIT_FAILURE;
	}
	stmt->close();
	cout << "Create schema " + schemaName + " success!" << endl;
	return EXIT_SUCCESS;
}
bool DBConnect::existSchema(string schemaName)
{
	try
	{
		stmt = con->createStatement();
		string statement = "show schemas like '" + schemaName + "'";
		res = stmt->executeQuery(statement.c_str());
		if (res->rowsCount() > 0)
			return true;
		else
			return false;
	}
	catch (SQLException &exception)
	{
		cerr << "Finding schema " + schemaName + " failed!" << endl;
		stmt->close();
		return EXIT_FAILURE;
	}
	stmt->close();
	return EXIT_SUCCESS;
}
bool DBConnect::createTableOfHashNametoTrueName()
{
	vector<string> column;
	column.push_back(HASHNAME);
	column.push_back(TRUETABLENAME);
	column.push_back(NAMEINDEX);
	return createTable(false, HASHNAMETOTRUETABLENAME, column);
}
bool DBConnect::insertTableOfHashNametoTrueName(string hashname, string truename, string nameindex)
{
	vector<pair<string, string>> hashVec;
	hashVec.push_back(make_pair(HASHNAME, hashname));
	hashVec.push_back(make_pair(TRUETABLENAME, truename));
	hashVec.push_back(make_pair(NAMEINDEX, nameindex));
	return insertIntoTable(false,HASHNAMETOTRUETABLENAME, hashVec);
}
ResultSet* DBConnect::querybyExistingColumnName(string tableName, vector<pair<string, string>> columnAndValue)
{
	try
	{
		stmt = con->createStatement();
		string statement = "SELECT * FROM " + tableName;
		for (int index = 0; index < columnAndValue.size(); index++)
		{
			if (index == 0)
				statement = statement + " WHERE ";
			statement = statement + columnAndValue[index].first + "=" + "'" + columnAndValue[index].second + "'";
			if (index < columnAndValue.size() - 1)
				statement = statement + " and ";
		}
		statement = statement + ";";
		ResultSet* result=stmt->executeQuery(statement.c_str());
		return result;
	}
	catch (SQLException &exception)
	{
		stmt->close();
	}
	stmt->close();
}
vector<string> DBConnect::findAllColumnsInDatabase(string tableName)
{
	vector<string> result;
	try
	{
		stmt = con->createStatement();
		string statement = "select column_name from information_schema.columns where table_name = '" + tableName + "';";
		ResultSet* resultset = stmt->executeQuery(statement.c_str());
		while (resultset->next())
		{
			result.push_back(resultset->getString("column_name"));
		}
	}
	catch (SQLException &exception)
	{
		stmt->close();
	}
	stmt->close();
	return result;
}
bool DBConnect::addNewColumnsIntoTable(vector<string> newColumnVecs, string tableName)
{
	try
	{
		stmt = con->createStatement();
		string statement = "ALTER TABLE "
			+ tableName;
		for (int index = 0; index < newColumnVecs.size(); index++)
		{
			statement = statement + " ADD COLUMN " + newColumnVecs[index] + " LONGTEXT";
			if (index < newColumnVecs.size() - 1)
				statement = statement + ",";
		}
		statement = statement + ";";
		stmt->execute(statement.c_str());
	}
	catch (SQLException &exception)
	{
		stmt->close();
		return EXIT_FAILURE;
	}
	stmt->close();
	return EXIT_SUCCESS;
}