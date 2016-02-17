#include "DBStringProcessor.h"



DBStringProcessor::DBStringProcessor()
{
}


DBStringProcessor::~DBStringProcessor()
{
}

string DBStringProcessor::getMainKeyString(string tableName)
{
	return tableName + "_ID";
}

string DBStringProcessor::getForeignKeyString(string referTableName, string referColumn)
{
	return referTableName + "_" + referColumn + "_Key";
}