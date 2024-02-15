#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstdio>
#include <string>

#include "compiler.h"
#include "token.h"
#include "testTree.h"
#include "staticSemantics.h"

using namespace std;

getStack stackID;
static int VarCntr = 0;
static int LabelCntr = 0;
static char Name[20];

enum nameType {
    VAR,
    LABEL
};

static char *newName(nameType what) {
    if (what == VAR) {
        sprintf(Name, "T%d", VarCntr++);
    } else if (what == LABEL) {
        sprintf(Name, "L%d", LabelCntr++);
    }
    return Name;
}

void recGen(node *n, int *temp, ofstream* output) {
    int stack_t;
    string label1;
    string label2;
    string argR;
    stack_t = verify(n, temp, output);

    switch (n -> nonTerminal) {
        //TODO: think this is partially causing the LOAD issue
        case INn:
            //staticSemantics(n -> c1, temp, output);
            argR = newName(VAR);
            *output << "READ " << argR << endl;
            break;

        case OUTn:
            staticSemantics(n -> c1, temp, output);
            argR = newName(VAR);
            *output << "STORE " << argR << endl;
            *output << "WRITE " << argR << endl;
            break;
        
        case EXPn:
            if (n -> tk1.tokenType != EMPTY_tk) {
                staticSemantics(n -> c2, temp, output);
                argR = newName(VAR);
                *output << "STORE " << argR << endl;
                staticSemantics(n -> c1, temp, output);
            

            if (n -> tk1.tokenType == MULTI_tk) {
                *output << "MULT " << argR << endl;
            } else if (n -> tk1.tokenType == DIVIDE_tk) {
                *output << "DIV " << argR << endl;
            } 
            } else {
                staticSemantics(n -> c1, temp, output);
            }
            break;

        case IFn:
            //call child 3
            staticSemantics(n -> c3, temp, output);
            argR = newName(VAR);
            *output << "STORE " << argR << endl;
            
            //call child 1
            staticSemantics(n -> c1, temp, output);
            *output << "SUB " << argR << endl;
            label1 = newName(LABEL);

            //lookup child 2 token
            switch (n -> c2 ->tk1.tokenType) {
                case GREATER_tk:
                    *output << (n -> c2 -> tk2.tokenType == EQUAL_tk ? "BRNEG " : "BRZNEG ") << label1 << endl;
                    break;
                case LESSER_tk:
                    *output << (n -> c2 -> tk2.tokenType == EQUAL_tk ? "BRPOS " : "BRZPOS ") << label1 << endl;
                    break;
                case EQUAL_tk:
                    *output << (n -> c2 -> tk2.tokenType == EQUAL_tk ? "BRZERO " : "BRNEG " + string("BRPOS ")) << label1 << endl;
                    break;
                default:
                    *output << (n -> c2 -> tk2.tokenType == EQUAL_tk ? "BRZERO " : "BRNEG " + string("BRPOS ")) << label1 << endl;
            }

            //call child 4
            staticSemantics(n -> c4, temp, output);
            *output << "BR " << label1 << endl;
            *output << label2 << ": NOOP" << endl;
            break;

        case Mn:
            if (n -> tk1.tokenType != EMPTY_tk) {
                staticSemantics(n -> c1, temp, output);
                argR = newName(VAR);
                *output << "STORE " << argR << endl;

                staticSemantics(n -> c1, temp, output);

                //case A token and 2 children
                if (n -> tk1.tokenType == PLUS_tk) {
                    *output << "ADD " << argR << endl;
                }
            } else {
                staticSemantics(n -> c1, temp, output);
            }
            break;

        case Nn:
            if (n -> tk1.tokenType != EMPTY_tk) {
                staticSemantics(n -> c2, temp, output);
                argR = newName(VAR);
                *output << "STORE " << argR << endl;
                staticSemantics(n -> c1, temp, output);

                if (n -> tk1.tokenType == MINUS_tk) {
                    *output << "SUB " << argR << endl;
                } else if (n -> c1 -> nonTerminal == Nn) {
                    *output << "MULT -1" << endl;
                }
            } else {
                staticSemantics(n -> c1, temp, output);
            }
            break;

        //TODO: this also seems to be contributing to LOAD issue
        case Rn:
            if (n -> c1 == nullptr) {
                 if (n -> tk1.tokenType == IDENTIFIER_tk) {
                     argR = newName(VAR);
                     *output << "LOAD " << n -> tk1.tokenInstance << endl;
                     *output << "PUSH\nSTACKW 0" << endl;
                 } else if (n -> tk1.tokenType == INTEGER_tk) {
                     argR = newName(VAR);
                     *output << "LOAD " << n -> tk1.tokenInstance << endl;
                     *output << "PUSH\nSTACKW 0" << endl;
                 }
             } else {
                 staticSemantics(n -> c1, temp, output);
             }
             break;

        case LOOPn:
            label1 = newName(LABEL);
            *output << label1 << ": NOOP" << endl;
            staticSemantics(n -> c3, temp, output);
            argR = newName(VAR);
            *output << "STORE " << argR << endl;

            staticSemantics(n -> c1, temp, output);
            *output << "SUB " << argR << endl;
            label2 = newName(LABEL);

            switch(n-> c2 -> tk1.tokenType) {
                case GREATER_tk:
                    *output << (n -> c2 -> tk2.tokenType == EQUAL_tk ? "BRNEG " : "BRZNEG ") << label2 << endl;
                    break;
                case LESSER_tk:
                    *output << (n -> c2 -> tk2.tokenType == EQUAL_tk ? "BRPOS " : "BRZPOS ") << label2 << endl;
                    break;
                case EQUAL_tk:
                    *output << (n -> c2 -> tk2.tokenType == EQUAL_tk ? "BRZERO " : "BRNEG " + string("BRPOS ")) << label2 << endl;
                    break;
                default:
                    *output << (n -> c2 -> tk2.tokenType == EQUAL_tk ? "BRZERO " : "BRNEG " + string("BRPOS ")) << label2 << endl;
            }
            staticSemantics(n -> c4, temp, output);
            *output << "BR " << label1 << endl;
            *output << label2 << ": NOOP" << endl;
            break;

        case ASSIGNn:
            staticSemantics(n -> c1, temp, output);
            *output << "STACKW " << stack_t << endl;
            break;
        
        default:
            staticSemantics(n -> c1, temp, output);
            staticSemantics(n -> c2, temp, output);
            staticSemantics(n -> c3, temp, output);
            staticSemantics(n -> c4, temp, output);
            break;
    }
}

void compiler(node *program, ofstream* output) {
    if (program == nullptr || program -> nonTerminal != PROGRAMn) {
        cerr << "COMPILER ERROR: invalid parse tree" << endl;
        exit(program -> nonTerminal);
    }

    staticSemantics(program, nullptr, output);

    *output << "STOP" << endl;
    for (int i = 0; i < VarCntr; i++) {
        *output << "T" << i << " 0" << endl;
    }
}

void staticSemantics(node *n, int *temp, ofstream* output) {
    if (n != nullptr) {
        if (n -> nonTerminal == PROGRAMn) {
            temp = new int(0);

            recGen(n, temp, output);

            for (int i = 0; i < *temp; i++) {
                stackID.pop();
                *output << "POP " << endl;
            }
            delete temp;
        } else {
            recGen(n, temp, output);
        }
    }
}

int verify(node *n, int *temp, ofstream* output) {
    int stack_t = -1;
    if (n -> nonTerminal == VARLISTn) {
        if (n -> tk1.tokenType == IDENTIFIER_tk) {
            stack_t = stackID.find(n -> tk1, 1);
            if (*temp > 0) {
                stack_t = stackID.find(n -> tk1, 1);
            }

            stackID.push(n -> tk1);
            *output << "PUSH" << endl;

            (*temp)++;

            return stack_t;
        }
    } else if (n -> nonTerminal == EXPn || n -> nonTerminal == Mn || n -> nonTerminal == Nn ||
    n -> nonTerminal == Rn || n -> nonTerminal == INn || n -> nonTerminal == ASSIGNn || n -> nonTerminal == OUTn) {
        if (n -> tk1.tokenType == IDENTIFIER_tk) {
            stack_t = stackID.find(n -> tk1, 2);
            return stack_t;
        }
    }
    return stack_t;
}