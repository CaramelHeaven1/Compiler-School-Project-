/*********************************************************************
Name: Caleb Hickman-Thomas
Course: CSE 4714
Project: Part 2 
Purpose of File: Recursive descenet parser
*********************************************************************/
#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdlib.h>
#include <iostream>
#include "lexer.h"
#include "parser.h"
#include "parse_tree_nodes.h"
 
using namespace std;

int nextToken = 0;            // hold nextToken returned by lex
extern bool printParse;
// Which tree level are we currently in?  
static int level = 0;
string idName, idType;

// Feel free to use a different data structure for the symbol table (list of
// variables declared in the program) but you will have to adjust the code in
// main() to print out the symbol table after a successful parse
 // Symbol Table

//*****************************************************************************
// Indent to reveal tree structure
string psp(void) { // Stands for p-space, but I want the name short
  string str("");
  for(int i = 0; i < level; i++)
    str += "|  ";
  return str;
}
//*****************************************************************************
//Get the next lexeme(word in sentence)
int lex() 
{
  nextToken = yylex();
  return nextToken;
}

//*****************************************************************************
// Report what we found
void output(string what) {
  std::cout << psp() << "found |" << yytext << "| " << what << endl;
}
//*****************************************************************************
// Forward declarations of FIRST_OF functions.  These check whether the current 
// token is in the FIRST set of a production rule.
bool first_of_program(void);
bool first_of_block(void);
bool first_of_factor(void);
bool first_of_statement(void);

BlockNode* block();
StatementNode* statement();
ExprNode* expression();
Simple_ExprNode* simpleexpression();
TermNode* term();
FactorNode* factor();
//*****************************************************************************
// Parses strings in the language generated by the rule:
// <program> → TOK_PROGRAM TOK_IDENT TOK_SEMICOLON <block>
ProgramNode* program() 
{
    if (!first_of_program()) // Check for PROGRAM
        throw "3: 'PROGRAM' expected";
    
    if(printParse) {output("PROGRAM");}

    ProgramNode* newProgramNode = nullptr;

    if(printParse) {std::cout << psp() << "enter <program>" << endl;}
    ++level;

    // TODO: finish this parsing function...
    if(first_of_program())
    {
      lex();
      if(nextToken == TOK_IDENT)
      {
        if(printParse) {output("IDENTIFIER");}
        lex();
      }

      if(nextToken == TOK_SEMICOLON)
      {
        if(printParse) {output("SEMICOLON");}
        lex();
      }

      //if next token is the begin or var token proceed to the block parse function. 
      if(nextToken == TOK_BEGIN || nextToken == TOK_VAR)
      {
        if(printParse) {output("BLOCK");}
        newProgramNode = new ProgramNode(level, block());
      }

    }

    --level;
    if(printParse) {std::cout << psp() << "exit <program>" << endl;}
    return newProgramNode;
}

//BLOCK FUNCTION 
BlockNode* block()
{
  //checking start of block.
  if (!first_of_block()) 
      throw "18: error in declaration part OR 17: 'BEGIN' expected";
  
  BlockNode* newBlockNode = nullptr;

  if(printParse) {std::cout << psp() << "enter <block>" << endl;}
  ++level;

  //VARIABLE PARSING (adding variable to the symbol table)
  if(nextToken == TOK_VAR)
  {
    lex();
    while(nextToken != TOK_BEGIN)
    {
      if(nextToken == TOK_IDENT)
      {
        if(printParse) {output("IDENTIFIER");}
        idName = yytext;
        lex();
      }
      if(nextToken == TOK_COLON)
      {
        if(printParse) {output("COLON");}
        lex();
        if(nextToken == TOK_REAL)
        {
          if(printParse) {output("TYPE");}
          idType = "REAL";
          lex();
        }
        else if(nextToken == TOK_INTEGER)
        {
          if(printParse) {output("TYPE");}
          idType = "INTEGER";
          lex();
        }
        else
        {
          //type error
          throw "10: error in type" ;
        }
        }
        else
        { 
          //colon error
          throw "5: ':' expected";
        }
      
      if(nextToken == TOK_SEMICOLON)
      {
        if(printParse) {output("SEMICOLON");}
        if(printParse) {std::cout<< psp() << "-- idName: |"<<idName<<"| idType: |"<<idType<<"| --"<<endl;}
        if(!query(idName))
        {
          symbolTable.insert(pair<string, float>(string(idName), 0.0));
          lex();
        }
        else
        {
          //identifier error
          throw "101: identifier declared twice";
        } 
      }
      else
      {
        //semicolon error
        throw "14: ';' expected";
      }
     
    }

  }
  
  //BEGIN ENCOUNTERED GO TO STATMENT FUNCTION
  if(nextToken == TOK_BEGIN)
  {
    
    newBlockNode = new BlockNode(level, comp_stmnt());
    
    //AFTER LEAVING STATMENT WHEN SEMICOLON ENOUNTERED LOOK FOR END 
    while(nextToken == TOK_SEMICOLON)
    {
      if(printParse) {output("SEMICOLON");}
      lex();
      statement();
    }
    if(nextToken == TOK_END)
    {
      if(printParse) {output("END");}
      lex();
    }
  }
  else
  {
    //begin error
    throw "17: 'BEGIN' expected";
  }

  --level;
  if(printParse) {std::cout << psp() << "exit <block>" << endl;}
  return newBlockNode;
}

//STATEMENT FUNCTIONS
StatementNode* statement()
{
  StatementNode* newStatementNode = nullptr;
  if(first_of_statement())
  {
    if(printParse) {output("STATEMENT");}
  }

  switch(nextToken)
  {
    case(TOK_IDENT) : {newStatementNode = assign_stmnt(); break;}
    case(TOK_IF) : {newStatementNode = if_stmnt(); break;}
    case(TOK_WHILE) : {newStatementNode = while_stmnt(); break;}
    case(TOK_READ) : {newStatementNode = read_stmnt(); break;}
    case(TOK_WRITE): { newStatementNode = write_stmnt(); break;}
    case(TOK_BEGIN): { newStatementNode = comp_stmnt(); break;}
  }
  return newStatementNode;
}

//WRITE STATEMENT
writeStmtNode* write_stmnt()
{
  if(printParse) {std::cout << psp() << "enter <write>" << endl;}
    ++level;
    writeStmtNode* writeptr = new writeStmtNode(level);
    lex();

    if(nextToken == TOK_OPENPAREN)
    {
      if(printParse) {output("OPENPAREN");}
      lex();

      if(nextToken == TOK_IDENT)
      {
        if(printParse) {output("WRITE");}
        if(printParse) {std::cout << psp() << yytext << endl;}
        writeptr->type = nextToken;
        writeptr->ident = string(yytext);
        lex();
      }
      else if(nextToken == TOK_STRINGLIT)
      {
        if(printParse) {output("WRITE");}
        if(printParse) {std::cout << psp() << yytext << endl;}
        writeptr->type = nextToken;
        writeptr->ident = string(yytext);
        lex();
      }
      else
      { 
        throw "134: illegal type of operand(s)";
      }

      if(nextToken == TOK_CLOSEPAREN)
      {
        if(printParse) {output("CLOSEPAREN");}
        lex();
      }
      else
      {
        throw "9: '(' expected";
      } 
    }
    else
    { 
      throw "4: ')' expected";
    }
      
    --level;
    if(printParse) {std::cout << psp() << "exit <write>" << endl;}
    return writeptr;
}

//COMPOUND STATEMENT
compoundStmtNode* comp_stmnt()
{
      if(printParse) {output("BEGIN");}
      lex();
      if(printParse) {std::cout << psp() << "enter <compound_stmt>" << endl;}
      ++level;
      compoundStmtNode* compPtr = new compoundStmtNode(level);

      if(first_of_statement())
      {
        compPtr->statements.push_back(statement());
      }
      while(nextToken == TOK_SEMICOLON)
      {
        if(printParse) {output("SEMICOLON");}
        lex();
        compPtr->statements.push_back(statement());
        //recurse to if another statment found
        if(first_of_statement())
        {
          if(printParse) {output("STATEMENT");}
          compPtr->statements.push_back(statement());
        } 
      }
      if(nextToken == TOK_END)
      {
        --level;
        if(printParse) {output("END");}
        if(printParse) {std::cout << psp() << "exit <compound_stmt>" << endl;}
        lex();
      }
      else
      {
        //end error
        throw "13: 'END' expected";
      }
  return compPtr;
}

//READ STATEMENT
readstmtNode* read_stmnt()
{
  if(printParse) {std::cout << psp() << "enter <read>" << endl;}
    ++level;
    readstmtNode* readptr = nullptr;
    lex();

    if(nextToken == TOK_OPENPAREN)
    {
      if(printParse) {output("OPENPAREN");}
      lex();
    }
    else
    {
      throw "4: ')' expected";
    }

    if(nextToken == TOK_IDENT)
    {
      if(printParse) {output("IDENTIFIER");}
      if(printParse) {std::cout << psp() << yytext << endl;}
      readptr = new readstmtNode(level);
      readptr->read = yytext;
      lex();
        
      if(nextToken == TOK_CLOSEPAREN)
      {
      if(printParse) {output("CLOSEPAREN");}
      lex();
      }
      else
      {
        throw "9: '(' expected";
      }
    }
    else
    {
      "2: identifier expected";
    }
     
    --level;
    if(printParse) {std::cout << psp() << "exit <read>" << endl;}
    return readptr;
}

//IF STATEMENT
ifstmtNode* if_stmnt()
{
  if(printParse) {std::cout << psp() << "enter <if>" << endl;}
      ++level;
      ifstmtNode* ifptr = new ifstmtNode(level);
      lex();

      if(first_of_factor())
      {
        if(printParse) {output("EXPRESSION");}
        ifptr->expression = expression();
      }

      if(nextToken == TOK_THEN)
      {
        --level;
        if(printParse) {output("THEN");}
        if(printParse) {std::cout << psp() << "enter <then>" << endl;}
        lex();
        ++level;
      }
      else
      {
        throw "52: 'THEN' expected";
      }
      if(first_of_statement())
      {
        ifptr->then_stmt = statement();
        --level;
        if(printParse) {std::cout << psp() << "exit <then>" << endl;}
        ++level;
      }
      if(nextToken == TOK_ELSE)
      {
        --level;
        if(printParse) {output("ELSE");}
        if(printParse) {std::cout << psp() << "enter <else>" << endl;}
        ++level;
        lex();

        if(first_of_statement())
        {
          ifptr->else_stmt = statement();
          --level;
          if(printParse) {std::cout << psp() << "exit <else>" << endl;}
          ++level;
        }
          
      }
    --level;
    if(printParse) {std::cout << psp() << "exit <if>" << endl;}

    return ifptr;
}

//ASSIGN STATEMENT
assignNode* assign_stmnt()
{
      assignNode* assignptr = nullptr;
      //output("STATEMENT");
      if(printParse) {std::cout << psp() << "enter <assignment>" << endl;}
      ++level;

      if(nextToken == TOK_IDENT)
      {
        if(printParse) {output("IDENTIFIER");}
        if(printParse) {std::cout << psp() << yytext << endl;}
        assignptr = new assignNode(level);
        assignptr->identifier = yytext;
        lex();
      }
      else{
        throw "2: indentifier expected";
      }

      if(nextToken == TOK_ASSIGN)
      {
        if(printParse) {output("ASSIGN");}
        lex();
        if(printParse) {output("EXPRESSION");}
        assignptr->exprPtr = expression();
      }
      else
      {
        throw "51: ':=' expected";
      }
      --level;
      if(printParse) {std::cout << psp() << "exit <assignment>" << endl;}
    return assignptr;
}

//WHILE STATEMENT
whileNode* while_stmnt()
{
    whileNode* whileptr = nullptr;

    if(printParse) {std::cout << psp() << "enter <while>" << endl;}
    ++level;
    lex();

    whileptr = new whileNode(level);

    if(first_of_factor())
    {
      if(printParse) {output("EXPRESSION");}
      whileptr->expression = expression();
    }

    if(first_of_statement())
    {
      whileptr->statement = statement();
    }

    --level;
    if(printParse) {std::cout << psp() << "exit <while>" << endl;}
    return whileptr;
}

//EXPRESSION FUNCTION
ExprNode* expression()
{
  ExprNode* newExprNode = new ExprNode(level, nullptr);
  if (!first_of_factor()) 
      throw "144: illegal type of expression";
    
  if(printParse) cout << psp() << "enter <expression>" << endl;
  ++level;
  newExprNode->firstSimpleExpr = simpleexpression();
  if( nextToken == TOK_EQUALTO){
    if(printParse) output("EQUALTO");
    if(printParse) cout << psp() << yytext << endl;
    newExprNode->simpleExpOp = nextToken;
    lex();
    newExprNode->secondSimpleExpr = simpleexpression();
  }
  else if( nextToken == TOK_LESSTHAN){
    if(printParse) output("LESSTHAN");
    if(printParse) cout << psp() << yytext << endl;
    newExprNode->simpleExpOp = nextToken;
    lex();
    newExprNode->secondSimpleExpr = simpleexpression();
  }
  else if( nextToken == TOK_GREATERTHAN){
    if(printParse) output("GREATERTHAN");
    if(printParse) cout << psp() << yytext << endl;
    newExprNode->simpleExpOp = nextToken;
    lex();
    newExprNode->secondSimpleExpr = simpleexpression();
  }
  else if( nextToken == TOK_NOTEQUALTO){
    if(printParse) output("NOTEQUALTO");
    if(printParse) cout << psp() << yytext << endl;
    newExprNode->simpleExpOp = nextToken;
    lex();
    newExprNode->secondSimpleExpr = simpleexpression();
  }
  
  --level;
  if(printParse) cout << psp() << "exit <expression>" << endl;
  return newExprNode;
}

//SIMPLE EXPRESSION
Simple_ExprNode* simpleexpression(){
  if (!first_of_factor()) //
      throw "901: illegal type of simple expression";


  if(printParse) output("SIMPLE_EXP");
  if(printParse) cout << psp() << "enter <simple_exp>" << endl;
  ++level;
  Simple_ExprNode* newSimpleExprNode = new Simple_ExprNode(level, nullptr);
  
  newSimpleExprNode->firstTerm = term();
  while(nextToken == TOK_PLUS || nextToken == TOK_MINUS || nextToken == TOK_OR){
    if(nextToken == TOK_PLUS){
      if(printParse) output("PLUS");
      if(printParse) cout << psp() << yytext << endl;
      newSimpleExprNode->restTermOps.push_back(nextToken);
      lex();
      newSimpleExprNode->restTerms.push_back(term());
    }
    if(nextToken == TOK_MINUS){
      if(printParse) output("MINUS");
      if(printParse) cout << psp() << yytext << endl;
      newSimpleExprNode->restTermOps.push_back(nextToken);
      lex();
      newSimpleExprNode->restTerms.push_back(term());
    }
    if(nextToken == TOK_OR){
      if(printParse) output("OR");
      newSimpleExprNode->restTermOps.push_back(nextToken);
      lex();
      newSimpleExprNode->restTerms.push_back(term());
    }
  }
  --level;
  if(printParse) cout << psp() << "exit <simple_exp>" << endl;
  return newSimpleExprNode;
}

//TERM 
TermNode* term(){
  if (!first_of_factor()) //
      throw "902: illegal type of term";
    
  if(first_of_factor){
    
    if(printParse) output("TERM");
    if(printParse){
      cout << psp() << "enter <term>" << endl;
    }
    ++level;


    TermNode* newTermNode = new TermNode(level, nullptr);
    newTermNode->firstFactor = factor();
    
    while(nextToken == TOK_MULTIPLY||nextToken == TOK_DIVIDE||nextToken == TOK_AND){
      
      if(nextToken == TOK_MULTIPLY){
        if(printParse) output("MULTIPLY");
        if(printParse) cout << psp() << yytext << endl;
        newTermNode->restFactorOps.push_back(nextToken);
        lex();
        newTermNode->restFactors.push_back(factor());
      }
      else if(nextToken == TOK_DIVIDE){
        if(printParse) output("DIVIDE");
        if(printParse) cout << psp() << yytext << endl;
        newTermNode->restFactorOps.push_back(nextToken);
        lex();
        newTermNode->restFactors.push_back(factor());
      }
      else if(nextToken == TOK_AND){
        if(printParse) output("AND");
        if(printParse) cout << psp() << yytext << endl;
        newTermNode->restFactorOps.push_back(nextToken);
        lex();
        newTermNode->restFactors.push_back(factor());
      }
    }
    --level;
    if(printParse){
      cout << psp() << "exit <term>" << endl;
    }
    return newTermNode;
  }
}

//FACTOR
FactorNode* factor()
{
  if (!first_of_factor()) //
      throw "903: illegal type of factor";
  
  if(printParse) output("FACTOR");
  if(printParse)
  {
    cout << psp() << "enter <factor>" << endl;
  }
  ++level;

  FactorNode* newFactorNode = nullptr;

  if(nextToken == TOK_INTLIT){
    if(printParse) output("INTLIT");
    if(printParse) cout<<psp()<<yytext<<endl;
    newFactorNode = new IntLitNode(level, atoi(yytext));
    lex();
    }
  else if(nextToken == TOK_FLOATLIT){
    if(printParse) output("FLOATLIT");
    if(printParse) cout<<psp()<<yytext<<endl;
    newFactorNode = new FloatLitNode(level, atof(yytext));
    lex();
    }
  else if(nextToken == TOK_IDENT){
    if(printParse) output("IDENTIFIER");
    if(printParse) cout << psp() << yytext << endl;
    if(!query(yytext)){
      throw "104: identifier not declared";
    }
    newFactorNode = new IdNode(level, string(yytext));
    lex();
    }
  else if(nextToken == TOK_OPENPAREN){
    if(printParse) output("OPENPAREN");
    if(printParse) cout << psp() << yytext << endl;
    lex();
    if(printParse) output("EXPRESSION");

    newFactorNode = new NestedExprNode(level, expression());
    if(printParse) output("CLOSEPAREN");
    lex();
    }
  else if(nextToken == TOK_NOT){
    if(printParse) output("NOT");
    if(printParse) cout << psp() << yytext << endl;
    lex();
    newFactorNode = new NotNode(level, factor());
    }
  else if(nextToken == TOK_MINUS){
    if(printParse) output("MINUS");
    if(printParse) cout << psp() << yytext << endl;
    lex();
    newFactorNode = new MinusNode(level, factor());    
  }

  --level;
  if(printParse){
    cout << psp() << "exit <factor>" << endl;
  }
  return newFactorNode;
}


//first of statements
bool first_of_program(void) {
  return nextToken == TOK_PROGRAM;
}

bool first_of_block(void) {
  return nextToken == TOK_VAR || nextToken == TOK_BEGIN;
}

bool first_of_factor(void) 
{
  return nextToken == TOK_INTLIT || nextToken == TOK_FLOATLIT || nextToken == TOK_IDENT || nextToken == TOK_OPENPAREN || nextToken == TOK_NOT || nextToken == TOK_MINUS;
}

bool first_of_statement(void) 
{
  return nextToken == TOK_IDENT ||nextToken == TOK_BEGIN || nextToken == TOK_IF || nextToken == TOK_WHILE || nextToken == TOK_READ || nextToken == TOK_WRITE;
}


