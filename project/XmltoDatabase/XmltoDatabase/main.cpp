#include "pugixml.hpp"
#include "XMLProcessor.h"
#include "DBStringProcessor.h"
#include <iostream>
int main(int argc, char *argv[])
{
	XMLProcessor proc;
	string tableaddress, username, password, schema;
	proc.dbConnect.enableConnection("tcp://127.0.0.1:3306", "root", "123456","ccdr");
	proc.dbConnect.initialize();
	proc.getExistingHashtoDBNameInfo();
	proc.read_xml_document("D:/04.xml");
	proc.process_node(false, &proc.node);
}
