/*DBStringProcessor is the class which is responsible for processing 
string data which helps us procesing some specific target which transforms raw string data to fit 
some specific target string.
*/
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
	//Return the main key column name of table
	static string getMainKeyString(string tableName);
	//Return the foreign key column name of a refering table
	static string getForeignKeyString(string referTableName);
	//Return the true name of a Table
	static string getOriginalTrueTableName(string originTableName);
	//Return the string which converts all the upper case letter to corresponding lower case letter
	static string getLowerCaseString(string originalString);
};

