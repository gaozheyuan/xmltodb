#include<stdio.h>
#include<iostream>
#include<string>
#include"DBConnect.h"
#include "pugixml.hpp"
#include <vector>
using namespace std;
using namespace pugi;
#pragma once
class XMLProcessor
{
public:
	xml_document doc;
	xml_node node;
	DBConnect dbConnect;
	bool read_xml_document(string fileName);
	bool get_root_node(string firstChild);
	bool process_node(bool hasParent,xml_node* currentNode,xml_node* parentNode=0, int parentPrimaryKey=0);
	XMLProcessor();
	~XMLProcessor();
};

