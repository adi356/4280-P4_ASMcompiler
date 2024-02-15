#include <istream>
#include <iostream>
#include <string> 

#include "testScanner.h"
#include "scanner.h" 
#include "token.h" 

using namespace std;

string token_types[] = {
	//Keywords
    "XOPEN -> KEYWORD",
	"XCLOSE -> KEYWORD",
	"XLOOP -> KEYWORD",
	"XDATA -> KEYWORD",
	"XEXIT -> KEYWORD",
	"XIN -> KEYWORD",
	"XOUT -> KEYWORD",
	"XCOND -> KEYWORD",
	"XTHEN -> KEYWORD",
	"XLET -> KEYWORD", 
	"XFUNC -> KEYWORD",
	
	"EOF", 	
	"IDENTIFIER",
	"INTEGER",
    "COMMENT",
	
    //Operands
	"EQUAL -> OPERAND",
    "LESSER -> OPERAND",
    "GREATER -> OPERAND",
    "TILDE -> OPERAND",
  	"PLUS -> OPERAND",
    "MINUS -> OPERAND",
    "MULTI -> OPERAND",
    "DIVIDE -> OPERAND",
    "PERCENT -> OPERAND",
    "GREATERGREATER -> OPERAND",
    "LESSLESS -> OPERAND",

	//Delimiters
    "COLON -> DELIMITER",
    "SEMICOLON -> DELIMITER",
	"PERIOD -> DELIMITER", 
	"LEFT_PARENTHESIS -> DELIMITER",
    "RIGHT_PARENTHESIS -> DELIMITER",
	"COMMA -> DELIMITER",
	"LEFT_BRACE -> DELIMITER",
    "RIGHT_BRACE -> DELIMITER",
    "LEFT_BRACKET -> DELIMITER",
    "RIGHT_BRACKET -> DELIMITER",
};

void testScanner(istream &FILE) {  
	Token token; 
	int end = 0; 

	lookahead(FILE);

	while (!end) { 
		token = scanner(FILE); 
	if (token.tokenType == EOF_tk) { 
		end = 1; 
	} 
	printToken(token); 
	}
}	

void printToken(Token token) {
        cout << "Token ID: " << token_types[token.tokenType] << "\n" << "Line Number: " << token.line_number << "\n" << "Token Instance: " << token.tokenInstance << "\n" << " ---- " << endl;
}