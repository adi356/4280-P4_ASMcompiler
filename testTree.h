#ifndef TESTTREE_H
#define TESTTREE_H

#include "token.h"
#include "staticSemantics.h"

#pragma once

using namespace std; 

enum node_t { 
	PROGRAMn, 
	VARSn, 
	VARLISTn, 
	EXPn, 
	Mn, 
	Nn, 
	Rn, 
	STATSn, 
	MSTATn, 
	STATn, 
	BLOCKn, 
	INn, 
	OUTn, 
	IFn, 
	LOOPn, 
	ASSIGNn, 
	R0n,
};

class getStack;

typedef struct node { 
	node_t nonTerminal; 
	Token tk1;		
	Token tk2; 
	struct node *c1; 
	struct node *c2; 
	struct node *c3;
    struct node *c4;
} node; 

extern string token_types[]; 
node *createNode(node_t);
void visitNode(node *n, int depth);
void traversePreorder(node *n, int depth); 

#endif
