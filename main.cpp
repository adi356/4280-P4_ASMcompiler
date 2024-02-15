#include <iostream> 

#include <fstream> 
#include <iomanip>
#include "token.h"
#include "scanner.h"
#include "testTree.h" 
#include "parser.h"
#include "staticSemantics.h"  
#include "compiler.h"

using namespace std; 

int main(int argc, char *argv[]) {
	ifstream inFile; 
	istream *input_stream;  
	string file; 
	ofstream outFile;
	
	if (argc > 2) { 
		cout << "Too many arguments" << endl; 
		return argc; 
	}

	if (argc == 2) {
     string inputExtension = ".f23";
	 file = argv[1];
	 if (file.substr(file.size() - inputExtension.size()) != inputExtension) {
		file += inputExtension;
	 }
	 inFile.open(file);

	 if (!inFile.is_open()) {
		cout << "ERROR: File could not be opened" << endl;
		return 1;
	 }
	 input_stream = &inFile;
	} else {
		const string output = "a";
		file = output;
		input_stream = &std::cin;
	}

	//parsing
	node *root = parser(*input_stream); 
	if (inFile.is_open()) {
		inFile.close();
	}
	
	//file output
	file = file.substr(0, file.size() - 4) + ".asm";
	outFile.open(file);

	if (!outFile.is_open()) {
		cout << "ERROR: could not open .asm file" << endl;
		return 1;
	}

	//compiler
	compiler(root, &outFile);
	outFile.close();
	
	cout << "SUCCESS: .asm file has been created: " << file << endl;

	return 0; 
}

