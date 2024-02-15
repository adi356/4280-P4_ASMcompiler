#ifndef TOKEN_H
#define TOKEN_H 

#include <iostream> 
#include <string> 

using namespace std; 



enum tokenID {
	//KWtk
	XOPEN_tk, 
	XCLOSE_tk, 
	XLOOP_tk, 
	XDATA_tk, 
	XEXIT_tk, 
	XIN_tk, 
	XOUT_tk, 
	XCOND_tk,
	XTHEN_tk, 
	XLET_tk,
	XFUNC_tk,

	// IDtk, INTtk
	EOF_tk,
	IDENTIFIER_tk,
	INTEGER_tk,

	// OPtk		
	EQUAL_tk,
	LESSER_tk,
	GREATER_tk,
	TILDE_tk, 
	COLON_tk,
    SEMICOLON_tk,
	PLUS_tk, 
	MINUS_tk, 
	MULTI_tk, 
	DIVIDE_tk, 
	PERCENT_tk, 
	PERIOD_tk, 
	COMMA_tk, 
	LEFT_PARENTHESIS_tk,
	RIGHT_PARENTHESIS_tk, 
	LEFT_BRACE_tk, 
	RIGHT_BRACE_tk, 
	LEFT_BRACKET_tk,
	RIGHT_BRACKET_tk,
	GREATERGREATER_tk,
	LESSERLESSER_tk,
	COMMENT_tk,
	EMPTY_tk,
}; 

typedef struct Token { 
	tokenID tokenType; 
	string tokenInstance; 
	int line_number; 
} Token;

#endif

