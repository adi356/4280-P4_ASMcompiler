#ifndef PARSER_H
#define PARSER_H

#include <iostream> 
#include <fstream>  
#include <cstdlib> 

#include "scanner.h" 
#include "testTree.h"

using namespace std; 

node* parser(istream &file);
node *program(istream &file); 
node *vars(istream &file); 
node *varList(istream &file); 
node *exp(istream &file); 
node *M(istream &file); 
node *N(istream &file); 
node *R(istream &file); 
node *stats(istream &file); 
node *mStat(istream &file); 
node *stat(istream &file);
node *block(istream &file); 
node *in(istream &file); 
node *out(istream &file); 
node *If(istream &file); 
node *loop(istream &file); 
node *assign(istream &file); 
node *R0(istream &file); 
void error();

#endif 

