//**************************************************************************
 
// Replace with appropriate header comment......

//**************************************************************************

#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <set>
#include <iostream>
#include "parse_tree_nodes.h"

using namespace std;

extern int nextToken;        // next token returned by lexer

extern "C" {
	// Instantiate global variables used by flex
	extern int   yylex();      // the generated lexical analyzer
	extern char* yytext;       // text of current lexeme
}

int lex();

// Function declarations
ProgramNode* program();
BlockNode* block();
StatementNode* statement();
ExprNode* expression();
Simple_ExprNode* simpleexpression();
TermNode* term();
FactorNode*  factor();

writeStmtNode* write_stmnt();
compoundStmtNode* comp_stmnt();
readstmtNode* read_stmnt();
ifstmtNode* if_stmnt();
assignNode* assign_stmnt();
whileNode* while_stmnt();
#endif /* PARSER_H */
