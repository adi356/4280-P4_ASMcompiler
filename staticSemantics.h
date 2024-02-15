#ifndef STATICSEMANTICS_H
#define STATICSEMANTICS_H

#include <fstream>

#include "token.h"
#include "testTree.h"

#pragma once

using namespace std;

class getID {
    public:
        Token tk;
        getID* next;
        getID(const Token& token) : tk(token), next(nullptr) {}
};

class getStack {
    private:
        static const int MAX_SIZE = 100;
        int size;
        getID *top;

    public:
        getStack(): size(0), top(nullptr) {}
        ~getStack();
        void push(const Token& tk);
        void pop();
        int find(const Token &tk, int error);
        void printStack() const;
};

#endif