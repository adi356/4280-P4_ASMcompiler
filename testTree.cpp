#include "token.h"
#include "testTree.h"
#include "parser.h"
#include "staticSemantics.h"
#include "compiler.h"


#include <iostream> 
#include <string> 

using namespace std; 


string nonTerminal_names[] = {
        "PROGRAM", "VARS", "VARLIST", "EXP", "M", "N", "R", "STATS", "MSTAT", "STAT", "BLOCK", "IN", "OUT", "IF", "LOOP", "ASSIGN", "R0",
};

string token_types[] = {
        "XOPEN", "XCLOSED", "XLOOP", "XDATA", "XEXIT",
        "XIN", "XOUTD", "XCOND", "XTHEN", "XLET", "XFUNC",
        "EOF", "IDENTIFIER", "INTEGER", "EQUAL", "LESS",
        "GREATER", "TILDE", "COLON", "SEMICOLON", "PLUS",
        "MINUS", "MULTIPLY", "DIVIDE", "PERCENT", "PERIOD",
        "LEFT_PARENTHESIS", "RIGHT_PARENTHESIS", "COMMA",
        "LEFT_BRACE", "RIGHT_BRACE", "LEFT_BRACKET",
        "RIGHT_BRACKET", "GREATERGREATER", "LESSLESS",
        "COMMENT", "EMPTY_TOKEN"
};

node* createNode(node_t nonTerminal) {
	node* new_node = new node;
	new_node -> nonTerminal = nonTerminal; 
	
	Token token1 = {EMPTY_tk, "", 0}; 
	Token token2 = {EMPTY_tk, "", 0}; 
	new_node -> tk1 = token1; 
	new_node -> tk2 = token2; 

	new_node -> c1 = nullptr;
	new_node -> c2 = nullptr;
	new_node -> c3 = nullptr;
	new_node -> c4 = nullptr;

	return new_node; 
} 

// start at root and visit each node before child node 
void traversePreorder(node *n, int depth) {
	// go to node if not null
    if (n != nullptr) {
		visitNode(n, depth);  
		// recursively visit child nodes
		traversePreorder(n->c1, depth+1);
		traversePreorder(n->c2, depth+1);
		traversePreorder(n->c3, depth+1);
		traversePreorder(n->c4, depth+1);
	}
}

// Visits nodes and prints parse tree
// TODO: need to figure out how to connect similar levels of tree
void visitNode(node *n, int depth) { 
	cout << string(depth * 2, ' ') << "├─" << nonTerminal_names[n -> nonTerminal]; 
	
	if (n->tk1.tokenType != EMPTY_tk) {
   		 cout << " {" << token_types[n->tk1.tokenType] << ", " << n->tk1.tokenInstance << ", " << n->tk1.line_number << "}";
	} 
 
	if (n->tk2.tokenType != EMPTY_tk) {
    		cout << " {" << token_types[n->tk2.tokenType] << ", " << n->tk2.tokenInstance << ", " << n->tk2.line_number << "}";
	}
    cout << endl;
}
