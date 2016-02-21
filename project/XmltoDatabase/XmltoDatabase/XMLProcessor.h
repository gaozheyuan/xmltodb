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
	string returnHashString(xml_node node);
	xml_document doc;
	xml_node node;
	DBConnect dbConnect;
	bool read_xml_document(string fileName);
	bool get_root_node(string firstChild);
	bool process_node(bool hasReferTable, xml_node* currentNode,string referTableName="", string referColumn="",string referPrimaryKey="");
	bool getExistingHashtoDBNameInfo();
	XMLProcessor();
	~XMLProcessor();
};

