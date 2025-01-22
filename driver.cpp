/*********************************************************************
Name: Caleb Hickman-Thomas
Course: CSE 4714
Project: Part 4
Purpose of File: Driver for tips
*********************************************************************/

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <cstring>
#include "lexer.h"
#include "parser.h"
#include "parse_tree_nodes.h"


using namespace std;

bool printTree = false; 
bool printSymbolTable = false;
bool printParse = false; 

extern "C"
{
  // Instantiate global variables
  extern FILE *yyin;        // input stream
  extern FILE *yyout;       // output stream
  extern int   yyleng;      // length of current lexeme
  extern char *yytext;      // text of current lexeme
  extern int   yylineno;    // line number for current lexeme
  extern int   yylex();     // the generated lexical analyzer
  extern int   yylex_destroy(); // deletes memory allocated by yylex
}

//*****************************************************************************
// The main processing loop
//
int main(int argc, char* argv[])
{
  // Process any command-line switches
  for(int i = 1; i < argc; i++) {
    // -p flag: if requested, print while parsing
    if(strcmp(argv[i], "-p") == 0) {
      printParse = true;
    }
    // -t flag: if requested, print parse tree
    if(strcmp(argv[i], "-t") == 0) {
      printTree = true; 
    }
    // -s flag: if requested, print symbol table
    if(strcmp(argv[i], "-s") == 0) {
      printSymbolTable = true;
    }
    // -d flag: if requested, print while deleting parse tree
    if(strcmp(argv[i], "-d") == 0) {
      printDelete = true;
    }
  }
  // Set the input stream
  if (argc > 1) {
    printf("INFO: Using the %s file for input\n", argv[argc-1]);
    yyin = fopen(argv[argc-1], "r"); 
  }

  ProgramNode* root = nullptr;

  lex();  // prime the pump (get first token)
  
  try
  {
    do{root = program();} 
    while(nextToken != TOK_EOF);
  }
  catch(char const *errmsg) 
  {
    cout << endl << "***ERROR:" << endl;
    cout << "On line number " << yylineno << ", near |" << yytext << "|, error type ";
    cout << errmsg << endl;
    return EXIT_FAILURE;
  }
  if (yyin)
  fclose(yyin);
  yylex_destroy();
  
  cout << "*** Interpret the Tree ***" << endl;
  root->interpret();


  if(printTree) 
  {
    cout << endl << "*** Print the Tree ***" << endl;
    cout << *root << endl << endl;
  }
  
  if(printSymbolTable)
  {
    cout << "*** Print the Symbol Table ***" << endl;
    symbolTableT::iterator it;
    for(it = symbolTable.begin(); it != symbolTable.end(); ++it )
      cout << setw(8) << it->first << ": " << it->second << endl;
  }
 
  if(printDelete) 
  {
    cout << "*** Delete the Tree ***" << endl;
  }
  delete root;
  root = nullptr;
    
  return(EXIT_SUCCESS);
}

