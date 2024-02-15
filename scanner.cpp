#include "token.h" 
#include "scanner.h"
#include "testTree.h" 
#include <iostream>
#include <fstream> 
#include <stdlib.h> 
#include <string> 

using namespace std;

/* 
Chose to modify my FSA table from P1 to work better when calling things in parser
*/ 

const int max_keywords = 11; 
const char *keywords[] = { 
	"xopen", "xclose", "xloop", "xdata", "xexit", "xin", "xout", "xcond", "xthen", "xlet", "xfunc"}; 
const int max_length = 8; 
int lineNum = 1; 
character next_ch; 
character filter(istream &FILE) { 
	character input; 
	char ch = FILE.get();
 
	int col = getFSAColumn(ch);
	
	while (col == COMMENT) { 
		do { 
			ch = FILE.get(); 
			if (ch == '\n') { 
				lineNum++; 
			}
		} while (ch != '$'); 
		
		ch = FILE.get(); 
		col = getFSAColumn(ch); 
	} 

	if (col >= LOWERCASE && col <= END) { 
		input.value = ch; 
		input.FSAColumn = col; 
		input.lineNum = lineNum; 

		if (ch == '\n') { 
			lineNum++; 
		} 
		
		return input; 
	} else {
		lexicalError(lineNum); 
		exit(col); 
	}
}


void lookahead(istream &FILE) { 
	next_ch = filter(FILE); 
} 

Token new_token; 

Token scanner(istream &FILE) {
	// initiala state  
	states curr_state = s1; 
	
	states next_state; 

	// store token instance
	string instance = "";
		
	while (curr_state != 1001) { 
		next_state = static_cast<states>(FSA_Table[curr_state][next_ch.FSAColumn]); 
		if (next_state == -1) {
			lexicalError(next_ch.lineNum); 
		}
		
		// double characters
		if (curr_state == 5 && next_ch.value == '>') {
            		next_ch = filter(FILE); 
            		instance = ">>"; 
            		next_state = FINAL; 
        	}

		if (curr_state == 4 && next_ch.value == '<') {
            		next_ch = filter(FILE); 
            		instance = "<<"; 
            		next_state = FINAL; 
        	}


		// final state 
		if (next_state == FINAL) { 
			new_token.tokenType = static_cast<tokenID>(curr_state + max_keywords);
			new_token.tokenInstance = instance; 
			new_token.line_number = lineNum;
			
			if (curr_state == s2) { 
				for (int i = 0; i < max_keywords; i++) { 
					if (keywords[i] == new_token.tokenInstance) { 
						new_token.tokenType = static_cast<tokenID>(i); 
						break;
					}
				}
			}
			return new_token; 
		} else { 
			if (next_ch.FSAColumn != WS) { 
				instance += next_ch.value;
			} 

			curr_state = next_state; 
			next_ch = filter(FILE); 
		
			if (instance.length() == max_length) { 
				new_token.tokenType = static_cast<tokenID>(curr_state + max_keywords);
				new_token.tokenInstance = instance; 
				new_token.line_number = next_ch.lineNum; 
				return new_token; 
			} 
		}
	}
	
	return new_token;

}

int getFSAColumn(char ch) {
	if (islower(ch)) { 
		return LOWERCASE;
	}
	if (isupper(ch)) { 
		return UPPERCASE;
	} 
	if (isdigit(ch)) { 
		return INTEGER;
	}
	if (isspace(ch)) { 
		return WS; 
	} 
	if (!islower(ch) && !isupper(ch) && !isdigit(ch) && !isspace(ch)) { 
		switch(ch) { 
			case '=': 
				return EQUAL; 
			case '<': 
				return LESS;
			case '>': 
				return GREATER; 
			case '~':
				return TILDE;
			case ':': 
				return COLON; 
			case ';': 
				return SEMICOLON;
			case '+': 
				return PLUS;
			case '-': 
				return MINUS; 
			case '*': 
				return MULTIPLY;
			case '/': 
				return DIVISION;
			case '%': 
				return PERCENT; 
			case '.': 
				return PERIOD;
			case '(': 
				return LEFT_PARENTHESIS;
			case ')': 
				return RIGHT_PARENTHESIS;
			case ',':
				return COMMA;
			case '{': 
				return LEFT_BRACE;
			case '}': 
				return RIGHT_BRACE; 
			case '[': 
				return LEFT_BRACKET;
			case ']':
				return RIGHT_BRACKET;
			case EOF: 
				return END; 
			case '$': 
				return COMMENT;
			default: 
				return -1;
			}
	}
	return -1;
}

void lexicalError(int lineNum) { 
	cerr << "LEXICAL ERROR at line: " << lineNum << endl; 
} 