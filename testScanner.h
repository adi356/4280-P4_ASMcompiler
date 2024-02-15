#ifndef TEST_SCANNER_H 
#define TEST_SCANNER_H

#include <iostream> 
#include "token.h" 

using namespace std;

void testScanner(istream &);
void lookahead(ifstream &);
void printToken(Token tkn);
Token scanner(ifstream &);

#endif