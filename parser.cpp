#include <iostream> 
#include <string> 
#include <iomanip>
#include <fstream>

#include "parser.h"
#include "scanner.h"
#include "token.h" 
#include "testTree.h"
#include "testScanner.h"

using namespace std; 

static Token token;

node* parser(istream &file) { 
	node *root;

	cout << "Starting parsing..." << endl;  

	lookahead(file); 
	token = scanner(file); 
	root = program(file);
	

	if (token.tokenType != EOF_tk) { 
		cout << "ERROR: PARSER - extra token after EOF" << endl; 
	} 
	return root; 
} 

// <program>  -> <vars> xopen <stats> xclose
node *program(istream &file) { 
	node *n = createNode(PROGRAMn);
	n-> c1 = vars(file); 
	if (token.tokenInstance == "xopen") {
		token = scanner(file); 
		n -> c2 = stats(file);

		if (token.tokenInstance == "xclose") { 
			token = scanner(file); 
			return n; 
		}
		cout << "<program>" << endl;
		error(); 
	}
	cout << "<program>" << endl; 	
	error(); 
	return NULL; 	 
} 
	
// <vars> -> empty | xdata <varList> 
node *vars(istream &file) { 
	if (token.tokenInstance == "xdata") {
		node *n = createNode(VARSn); 
		token = scanner(file); 
		n -> c1 = varList(file); 
		return n; 
	} else { 
		return NULL; // empty 
	}
} 

// <varList> -> identifier : integer ; | identifier : integer <varList> 
node *varList(istream &file) { 
	if (token.tokenType == IDENTIFIER_tk) {
		node *n = createNode(VARLISTn); 
		n-> tk1 = token; 
		token = scanner(file); 

		if (token.tokenType == COLON_tk) { 
			token = scanner(file); 
			
			if (token.tokenType == INTEGER_tk) { 
				n-> tk2 = token; 
				token = scanner(file); 

				if (token.tokenType == SEMICOLON_tk) { 
					token = scanner(file); 
					return n; 
				} else { 
					n-> c1 = varList(file); 
					return n; 
				}
			}
		}
	}
	cout << "<varList>" << endl; 
	error(); 
	return NULL;	
} 

// <exp> -> <M> / <exp> | <M> * <exp> | <M>
node *exp(istream &file) { 
	node *n = createNode(EXPn);
	n -> c1 = M(file);  

	if (token.tokenType == DIVIDE_tk) { 
		n -> tk1 = token; 
		token = scanner(file); 
		n -> c2 = exp(file); 
		return n; 
	} else if (token.tokenType == MULTI_tk) { 
		n -> tk1 = token; 
		token = scanner(file); 
		n -> c2 = exp(file);
		return n;  
	} 
	return n; 
}

// <M> -> <N> + <M> | <N> 
node *M(istream &file) { 
	node *n = createNode(Mn); 
	n -> c1 = N(file); 

	if (token.tokenType == PLUS_tk) { 
		n-> tk1 = token; 
		token = scanner(file); 
		n-> c2 = M(file); 
		return n; 
	} 
	return n; 
} 

// <N> -> <R> - <N> | ~ <N> | <R>
node *N(istream &file) { 
	node *n = createNode(Nn); 

	if (token.tokenType == TILDE_tk) { 
		n-> tk1 = token; 
		token = scanner(file);
		n -> c1 = N(file);  
		return n; 
	} else { 
		n-> c1 = R(file); 
	
		if (token.tokenType == MINUS_tk) { 
			n-> tk1 = token; 
			token = scanner(file); 
			n-> c2 = N(file); 
			return n; 
		} else { 
			return n; 
		} 
	}
	return NULL; 
} 


// <R> -> (<exp>) | identifier | integer
node *R(istream &file) { 
	node *n = createNode(Rn);
	 
	if (token.tokenInstance == "(") { 
		token = scanner(file); 
		n-> c1 = exp(file); 

		if (token.tokenInstance == ")") { 
			token = scanner(file);
		}
	} 
	 
	else if (token.tokenType == IDENTIFIER_tk){ 
		n -> tk1 = token; 
		token = scanner(file); 
	} 
	else if (token.tokenType == INTEGER_tk) { 
		n -> tk1 = token; 
		token = scanner(file); 
	} else { 
		cout << "<R>" << endl;
		error(); 
	} 
	return n;  
} 


// <stats> -> <stat> <mStat> 
node *stats(istream &file) { 
	node *n = createNode(STATSn); 
	n -> c1 = stat(file); 
	n -> c2 = mStat(file); 
	return n; 
} 

// <mStat> -> empty | <stat> <mStat> 
node *mStat(istream &file) { 
	node *n = createNode(MSTATn);
	
	// check if current token can start a <stat> 
	if (token.tokenType == XIN_tk || token.tokenType == XOUT_tk || token.tokenType == XLET_tk || token.tokenType == XLOOP_tk || token.tokenType == XCOND_tk || token.tokenType == LEFT_BRACE_tk) {
		n -> c1 = stat(file); 
		n -> c2 = mStat(file);
		return n;
	} 
	return n; 
}
// <stat> -> <in>|<out>|<block>|<if>|<loop>|<assign>
node *stat(istream &file) { 
	node *n = createNode(STATn);
	 
	if (token.tokenType == XIN_tk) { 
		n -> c1 = in(file);
		return n;
	} else if (token.tokenType == XOUT_tk) { 
		n-> c1 = out(file); 
		return n;
	} else if (token.tokenType == LEFT_BRACE_tk) { 
		n -> c1 = block(file); 
		return n;
	} else if (token.tokenType == XCOND_tk) { 
		n -> c1 = If(file); 
		return n;
	} else if (token.tokenType == XLOOP_tk) { 
		n -> c1 = loop(file); 
		return n; 
	} else if (token.tokenType == XLET_tk) { 
		n -> c1 = assign(file); 
		return n;
	} 
	cout << "<stat>" << endl;	
	error();  
	return NULL; 
} 


// <block> -> { <vars> <stats> } 
node *block(istream &file) { 
	if (token.tokenType == LEFT_BRACE_tk) {
		node *n = createNode(BLOCKn);
		token = scanner(file); 

		n -> c1 = vars(file); 
		n -> c2 = stats(file); 
		
		if (token.tokenType == RIGHT_BRACE_tk) { 
			token = scanner(file); 
			return n; 
		}
		cout << "<block>" << endl;
		error(); 
	} 
	cout << "<block>" << endl;
	error(); 
	return NULL;
} 
	

// <in> -> xin >> identifier; 
node *in(istream &file) { 
	node *n = createNode(INn); 
	
	if (token.tokenType == XIN_tk) { 	
		token = scanner(file); 
	
		if (token.tokenInstance == ">>") { 
			token = scanner(file); 
		
			if (token.tokenType == IDENTIFIER_tk) { 
				n-> tk1 = token; 
				token = scanner(file); 
				
				if (token.tokenType == SEMICOLON_tk) { 
					token = scanner(file); 
					return n; 
				}
			}
		}
	}
	cout << "<in>" << endl;
	error(); 
	return NULL;
} 

// <out> -> xout << <exp>; 
node *out(istream &file) { 
	node *n = createNode(OUTn); 
	
	if (token.tokenType == XOUT_tk) { 
		token = scanner(file); 

		if (token.tokenInstance == "<<") { 
			token = scanner(file); 
			n -> c1 = exp(file); 
		
			if (token.tokenType == SEMICOLON_tk) { 
				token = scanner(file); 
				return n; 
			} 
		}
	} 
	cout << "<out>" << endl;
	error(); 
	return NULL; 
}

// <if> -> xcond [ <exp> <RO>  <exp> ] <stat> 
node *If(istream &file) { 
	node *n = createNode(IFn); 

	if (token.tokenType == XCOND_tk) { 
		token = scanner(file); 

		if (token.tokenInstance == "[") {
			token = scanner(file); 
			n -> c1 = exp(file); 
			n -> c2 = R0(file); 
			n -> c3 = exp(file); 
		
			if (token.tokenInstance == "]") { 
				token = scanner(file); 
				n -> c4 = stat(file); 
				return n; 
			} 
		}
	}
	cout << "<if>" << endl;
	error(); 
	return NULL; 
} 

// <loop> -> xloop [ <exp> <RO> <exp> ] <stat> 
node *loop(istream &file) { 
	node *n = createNode(LOOPn); 

	if (token.tokenType == XLOOP_tk) { 
		token = scanner(file); 
		
		if (token.tokenInstance == "[") {
                        token = scanner(file);
                        n -> c1 = exp(file);
                        n -> c2 = R0(file);
                        n -> c3 = exp(file);

                        if (token.tokenInstance == "]") {
                                token = scanner(file);
                                n -> c4 = stat(file);
                                return n;
                        }
		}
    }
	cout << "<loop>" << endl;
	error(); 
        return NULL;
}


// <assign> -> xlet identifier <exp>; 
node *assign(istream &file) { 
	node *n = createNode(ASSIGNn); 

	if (token.tokenType == XLET_tk) { 
		n -> tk1 = token; 
		token = scanner(file); 
		
		if (token.tokenType == IDENTIFIER_tk) { 
			n -> tk2 = token; 
			token = scanner(file); 
			n -> c1 = exp(file); 
		
			if (token.tokenType == SEMICOLON_tk) { 
				token = scanner(file); 
				return n; 
			}
		}
	}
	cout << "<assign>" << endl;
	error(); 
	return NULL; 	
}

// <R0> -> <<(one token) | >> (one token) | < | > | = | %
node *R0(istream &file) { 
	node *n = createNode(R0n);
	if (token.tokenInstance == "<<") { 
		n-> tk1 = token; 
		token = scanner(file); 
		return n; 
	} else if (token.tokenInstance == ">>") { 
		n-> tk1 = token;
        token = scanner(file);
        return n;
	} else if (token.tokenType == LESSER_tk) { 
		n-> tk1 = token;
        token = scanner(file);
        return n;
	} else if (token.tokenType == GREATER_tk) { 
		n-> tk1 = token;
        token = scanner(file);
        return n;
	} else if (token.tokenType == EQUAL_tk) { 
		n-> tk1 = token;
        token = scanner(file);
        return n;
	} else if (token.tokenType == PERCENT_tk) { 	
		n-> tk1 = token;
        token = scanner(file);
        return n;
	} 
	cout << "<R0>" << endl;
	error(); 
	return NULL;
}

void error() {
   cout << "ERROR: Unexpected token - " << token_types[token.tokenType] << endl; 
   exit(token.tokenType);
}
