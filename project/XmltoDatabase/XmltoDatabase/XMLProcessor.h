/*XMLProcessor is responsible for reading the xml files and perform operation on that
and calls DBConnect to interact with the Mysql database
*/
#include<stdio.h>
#include<iostream>
#include<string>
#include <iostream>
#include"DBConnect.h"
#include"DBStringProcessor.h"
#include "pugixml.hpp"
#include <vector>
#include <map>
using namespace std;
using namespace pugi;
#pragma once
class XMLProcessor
{
public:
	map<string, string> nodeToDBName;
	map<string, int> countSameNode;
	xml_document doc;
	xml_node node;
	DBConnect dbConnect;
	//return the combined string from the root node to the current node
	string returnHashString(xml_node node);
	//specify the filename and read the xml document from the file
	bool read_xml_document(string fileName);
	//get the root node from its name firstChild
	bool get_root_node(string firstChild);
	//function which processing the current Node and iteratively inserts the data into the databaseS
	bool process_node(bool hasReferTable, xml_node* currentNode,string referTableName="", string referColumn="",string referPrimaryKey="");
	//find all the mappings from mysql database which stores the mapping relationship
	bool getExistingHashtoDBNameInfo();
	XMLProcessor();
	~XMLProcessor();
};

