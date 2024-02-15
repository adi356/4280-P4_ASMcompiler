#include <cstdio>
#include <cstdlib>
#include <vector>
#include <iostream>
#include <fstream>
#include <string>

#include "token.h"
#include "staticSemantics.h"
#include "testTree.h"
#include "compiler.h"

using namespace std;

getStack::~getStack() {
    while (size>0) {
        pop();
    }
}

void getStack::push(const Token& tk) {
    if (size == MAX_SIZE) {
        std::cout << "ERROR: STACK OVERFLOW" << std::endl;
        std::exit(size);
    }

    getID* id = new getID(tk);
    id -> next = top;
    top = id;
    size++;
}

void getStack::pop() {
    if (size > 0) {
        getID* tempID = top;
        top = tempID -> next;
        delete tempID;
        size--;
    } else {
        cout << "EMPTY STACK" << endl;
        exit(size);
    }
}

int getStack::find(const Token &tk, int error) {
    getID* currentID = top;

    for (int distance = 0; distance < size; distance++) {
        if (tk.tokenInstance == currentID -> tk.tokenInstance) {
            if (error == 1) {
                cout << "ERROR: STATIC SEMANTICS -> varaiable '" << tk.tokenInstance << "' originally defined on line: " << currentID -> tk.line_number << "( " << tk.line_number << ")" << endl;
                exit(currentID -> tk.line_number);
            } else if (error == 2) {
                return distance;
            }
        }
        currentID = currentID -> next;
    }

    if (error == 2) {
        cout << "ERROR: STATIC SEMANTICS -> variable '" << tk.tokenInstance << "' originally defined on line: " << tk.line_number << endl;
        std::exit(tk.line_number);
    }
    return -1;
}

void getStack::printStack() const {
    int distance;
    getID* currentID = top;
    cout << "Stack: " << endl;

    for (distance = 0; distance < size; distance++) {
        cout << currentID ->tk.tokenInstance << " " << distance << endl;
        currentID = currentID -> next;
    }
}
