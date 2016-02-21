#include "pugixml.hpp"
#include "XMLProcessor.h"
#include "DBStringProcessor.h"
#include <iostream>
int main(int argc, char *argv[])
{
	XMLProcessor proc;
	string tableaddress, username, password, schema;
	
	proc.dbConnect.enableConnection("tcp://127.0.0.1:3306", "root", "82213053","tns");
	proc.dbConnect.initialize();
	proc.getExistingHashtoDBNameInfo();
	proc.read_xml_document("C:/Users/Zheyuan Gao/Desktop/sample data and codes/scriptforextractinglabtesttable/ltdata/13.xml");
	proc.get_root_node("tns:CCDRMessage");
	proc.process_node(false, &proc.node);
}
