#include "XMLProcessor.h"



XMLProcessor::XMLProcessor()
{
}


XMLProcessor::~XMLProcessor()
{
}


bool XMLProcessor::read_xml_document(string fileName)
{
	if (!doc.load_file(fileName.c_str()))
		return false;
	return true;
}

bool XMLProcessor::get_root_node(string firstChild)
{
	node = doc.child(firstChild.c_str());
	return true;
}

bool XMLProcessor::process_node(bool hasReferTable, xml_node* currentNode, string referTableName, string referColumn, string referPrimaryKey)
{
	vector<xml_node> childnodesVecs;
	vector<pair<string, string>> columnNameandValues;
	string trueTableName;
	for (xml_node_iterator it = currentNode->begin(); it != currentNode->end(); ++it)
	{
		if(it->first_child().name()=="")
		{
			string column_name = DBStringProcessor::getOriginalTrueTableName(it->name());
			cout << "name: " << it->name() << endl;
			string column_value = it->child_value();
			columnNameandValues.push_back(make_pair(column_name, column_value));
		}
		else
		{
			cout << it->name() << endl;
			childnodesVecs.push_back(*it);
		}
	}
	vector<string> columnVec;
	if (columnNameandValues.size() > 0)
	{
		for (vector<pair<string, string>>::iterator it = columnNameandValues.begin(); it != columnNameandValues.end(); it++)
		{
			columnVec.push_back(it->first);
		}
	}
	string originalTrueTableName = DBStringProcessor::getOriginalTrueTableName(currentNode->name());
	string hashTableValue = returnHashString(*currentNode);
	if (nodeToDBName[hashTableValue]=="")//if there is no corresponding table in database
	{
		countSameNode[originalTrueTableName]++;//true name of table plus 1
		char str[10];
		_itoa_s(countSameNode[originalTrueTableName], str, 10);
		cout << originalTrueTableName + "_" + string(str) << endl;
		trueTableName =DBStringProcessor::getLowerCaseString(originalTrueTableName + "_" + string(str));
		nodeToDBName[hashTableValue] = trueTableName;
		cout << "Table " + trueTableName << " has not been created in database" << endl;
		if (hasReferTable)
		{
			dbConnect.createTable(hasReferTable, trueTableName, columnVec, referTableName,DBStringProcessor::getMainKeyString(referTableName));
		}
		else
		{
			dbConnect.createTable(hasReferTable, trueTableName, columnVec);
		}
	}
	else
	{
		cout << "Table " + trueTableName + " has already been created in database"<<endl;
	}
	trueTableName = nodeToDBName[hashTableValue];
	if (hasReferTable)
	{
		string referColumn = DBStringProcessor::getMainKeyString(referTableName);
		dbConnect.insertIntoTable(hasReferTable, "", trueTableName, columnNameandValues, referTableName, referColumn, referPrimaryKey);
	}
	else
	{	
		dbConnect.insertIntoTable(hasReferTable, "", trueTableName, columnNameandValues);
	}

	for (int index = 0; index < childnodesVecs.size(); index++)
	{
		string childTableName = DBStringProcessor::getOriginalTrueTableName(childnodesVecs[index].name());
		process_node(true, &childnodesVecs[index],trueTableName,DBStringProcessor::getMainKeyString(trueTableName),"1");
	}
	return true;
}

string XMLProcessor::returnHashString(xml_node node)
{
	if (node.parent().name() == "")
		return node.name();
	else
		return returnHashString(node.parent()) + "-" + node.name();
}