#ifndef COMPILER_H
#define COMPILER_H

#include <fstream>
#include <iostream>

#include "token.h"
#include "testTree.h"
#include "staticSemantics.h"

using namespace std;

void recGen(node *n, int* temp, ofstream* output);
void compiler(node *program, ofstream* output);
void staticSemantics(node *n, int* temp, ofstream* output);
int verify(node *n, int* temp, ofstream* output);

#endif