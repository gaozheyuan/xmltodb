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

bool XMLProcessor::process_node(bool hasParent, xml_node* currentNode, xml_node* parentNode, int parentPrimaryKey)
{
	vector<xml_node> childnodesVecs;
	vector<pair<string, string>> columnNameandValues;
	for (xml_node_iterator it = currentNode->begin(); it != currentNode->end(); ++it)
	{
		cout << "name: " << it->name() << endl;
		cout << "value: " << it->child_value() << endl;
		if (it->begin() == it->end())
		{
			columnNameandValues.push_back(make_pair(it->name(), it->child_value()));
		}
		else
		{
			childnodesVecs.push_back(*it);
		}
	}
	vector<string> columnVec;
	cout << columnVec.size();
	if (columnNameandValues.size() > 0)
	{
		for (vector<pair<string, string>>::iterator it = columnNameandValues.begin(); it != columnNameandValues.end(); it++)
		{
			columnVec.push_back(it->first);
		}
	}
	if (!dbConnect.existTable(dbConnect.getDbSchema(),currentNode->name()))
	{
		string strCurrentName = string(currentNode->name());
		string strParentName = string(parentNode->name());
		string subCurrentName = strCurrentName.substr(strCurrentName.find(":") + 1, string::npos);
		string subParentName = strParentName.substr(strParentName.find(":") + 1, string::npos);
		dbConnect.createTable(hasParent, subCurrentName, columnVec,subParentName);
	}
	return true;
}
