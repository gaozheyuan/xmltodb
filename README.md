Please add mysql c++ connector dll to C:\Windows\SysWOW64  and C:\Windows\System32
# xmltodb
In this program I implemented a program which realized the function of transfering XML file data into Mysql database.
Quick Start:

int main(int argc, char *argv[])
{
	XMLProcessor proc;
	proc.dbConnect.enableConnection("tcp://127.0.0.1:3306", "root", "82213053","ccdr");//Connect to the mysql database
	proc.dbConnect.initialize();//do the initialization works for dbConnect
	proc.getExistingHashtoDBNameInfo();//extract whether there exists
	proc.read_xml_document("C:/Users/Zheyuan Gao/Desktop/sample data and codes/scriptforextractingdstable/20151128/re_extract/07.xml");//specify the xml file to be processed
	proc.get_root_node("ccdr:CCDRMessage");//specify the root node name
	proc.process_node(false, &proc.node);
}



Here is a simple of processing 
Here are some notes and explanation on the functions of different files.

DBConnect class: Responsible for maintaining the connection between the program and the Mysql
database, and also manages the function directly interacting between the program and the database.

DBStringProcessor class: Responsible for processing 
string data which helps us procesing some specific target which transforms raw string data to fit 
some specific target string.

XMLProcessor is responsible for reading the xml files and perform operation on that
and calls DBConnect to interact with the Mysql database