#include "pugixml.hpp"
#include "XMLProcessor.h"
#include "DBStringProcessor.h"
#include <iostream>
int main()
{
	XMLProcessor proc;
	proc.dbConnect.setDbSchema("testvs");
	proc.dbConnect.enableConnection("tcp://127.0.0.1:3306", "root", "82213053");
	proc.read_xml_document("C:/Users/Zheyuan Gao/Desktop/sample data and codes/scriptforextractingdstable/20151128/re_extract/04.xml");
	proc.get_root_node("ccdr:CCDRMessage");
	proc.process_node(false, &proc.node);
}
