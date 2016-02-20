#include "DBStringProcessor.h"



DBStringProcessor::DBStringProcessor()
{
}


DBStringProcessor::~DBStringProcessor()
{
}

string DBStringProcessor::getMainKeyString(string tableName)
{
	return getLowerCaseString(tableName + "_ID");
}

string DBStringProcessor::getForeignKeyString(string referTableName, string referColumn)
{
	return getLowerCaseString(referTableName + "_" + referColumn + "_key");
}

string DBStringProcessor::getOriginalTrueTableName(string originTableName)
{
	return getLowerCaseString(originTableName.substr(originTableName.find(':') + 1, string::npos));
}

string DBStringProcessor::getLowerCaseString(string originalString)
{
	string lowerString;
	lowerString.resize(originalString.size());
	transform(originalString.begin(), originalString.end(), lowerString.begin(), ::tolower);
	return lowerString;
}