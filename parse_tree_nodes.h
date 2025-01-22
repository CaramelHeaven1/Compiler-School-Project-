/*********************************************************************
Name: Caleb Hickman-Thomas
Course: CSE 4714
Project: Part 4
Purpose of File: tree nodes header file 
*********************************************************************/


#ifndef PARSE_TREE_NODES_H
#define PARSE_TREE_NODES_H

#include <iostream>
#include <vector>
#include <string>
#include "lexer.h"
#include <map>


using namespace std;

extern bool printDelete; 


typedef map<string, float> symbolTableT;
extern symbolTableT symbolTable;

static bool truth(float F); 
bool query(string temp);

class ProgramNode;
class BlockNode;
class StatementNode;
class ExprNode; 
class Simple_ExprNode; 
class TermNode;
class FactorNode;
class compoundStmtNode;

class ProgramNode {
public:
  int _level = 0;      
  BlockNode* firstBlock = nullptr;

  void interpret();
  ProgramNode(int level, BlockNode* blockNodePtr);
  ~ProgramNode();
};
ostream& operator<<(ostream&, ProgramNode&); 

class BlockNode {
public:
  int _level = 0;              
  compoundStmtNode* firstCompound = nullptr;


  void interpret();
  BlockNode(int level, compoundStmtNode* compoundNodePtr);
  ~BlockNode();
};
ostream& operator<<(ostream&, BlockNode&);

class StatementNode {
public:
  int _level = 0;   

  vector<StatementNode*> statements;

  virtual void interpret() = 0;          
  virtual void printTo(ostream &os) = 0; 
  virtual ~StatementNode();             
};
ostream& operator<<(ostream&, StatementNode&); 



class compoundStmtNode : public StatementNode{
public:
  int _level = 0;   
  vector<StatementNode*> statements;

  void interpret();
  compoundStmtNode(int level);
  ~compoundStmtNode();
  void printTo(ostream & os);
};



class assignNode : public StatementNode {
public:
    string identifier;
    ExprNode* exprPtr;

    void interpret();
    assignNode(int level);
    ~assignNode();
    void printTo(ostream & os);
};


class ifstmtNode : public StatementNode {
public:
    string id;

    ExprNode* expression = nullptr;
    StatementNode* then_stmt = nullptr;
    StatementNode* else_stmt = nullptr;
    ifstmtNode(int level);
    ~ifstmtNode();
    void interpret();
    void printTo(ostream & os);
};


class whileNode : public StatementNode {
  public:
    string* id = nullptr;

    ExprNode* expression = nullptr;
    StatementNode* statement = nullptr;

    whileNode(int level);
    ~whileNode();
    void interpret();
    void printTo(ostream & os);
};

class readstmtNode : public StatementNode {
  public:
    string read;

    readstmtNode(int level);
    ~readstmtNode();
    void interpret();
    void printTo(ostream & os);
};


class writeStmtNode : public StatementNode {
  public:
    int type = 0;
    string ident; 

    writeStmtNode(int level);
    ~writeStmtNode();
    void printTo(ostream & os);
    void interpret();
};


//ExprNode
class ExprNode {
public:
  int _level = 0;          
  Simple_ExprNode* firstSimpleExpr = nullptr;
  int simpleExpOp; 
  Simple_ExprNode* secondSimpleExpr = nullptr;

  ExprNode(int level, Simple_ExprNode* simpleExprNodePtr);
  ~ExprNode();
  float interpret();
};
ostream& operator<<(ostream&, ExprNode&); 

// Simple_ExprNode
class Simple_ExprNode {
public:
  int _level = 0;          
  TermNode* firstTerm = nullptr;
  vector<int> restTermOps;
  vector<TermNode*> restTerms;

  Simple_ExprNode(int level, TermNode* termNodePtr);
  ~Simple_ExprNode();
  float interpret();
};
ostream& operator<<(ostream&, Simple_ExprNode&); 

// TERM NODE
class TermNode {
public:
  int _level = 0;              
  FactorNode* firstFactor = nullptr;
  vector<int> restFactorOps;  
  vector<FactorNode*> restFactors;

  TermNode(int level, FactorNode* factorNodePtr);
  ~TermNode();
  float interpret();
};
ostream& operator<<(ostream&, TermNode&); 


//factor node 
class FactorNode {
public:
  int _level = 0;                       

  
  virtual void printTo(ostream &os) = 0; 
  virtual float interpret() = 0; 
  virtual ~FactorNode();               
	                                       
};
ostream& operator<<(ostream&, FactorNode&); 


//class <int_lit>
class IntLitNode : public FactorNode {
public:
    int int_literal = 0;

    IntLitNode(int level, int value);
    ~IntLitNode();
    void printTo(ostream & os);
    float interpret();
};


//class float lit 
class FloatLitNode : public FactorNode {
public:
    float float_literal = 0.0;

    FloatLitNode(int level, float value);
    ~FloatLitNode();
    void printTo(ostream & os);
    float interpret();
};

//class Identifier Node
class IdNode : public FactorNode {
public:
    string* id = nullptr;

    IdNode(int level, string name);
    ~IdNode();
    void printTo(ostream & os);
    float interpret();
};

//class not node 
class NotNode : public FactorNode {
public:

  FactorNode* factorNodePtr = nullptr;
  NotNode(int level, FactorNode* factorNode);
  ~NotNode();
  void printTo(ostream & os);
  float interpret();
};


//minus node 
class MinusNode : public FactorNode {
public:
    
  FactorNode* factorNodePtr = nullptr;
  MinusNode(int level, FactorNode* factorNode);
  ~MinusNode();
  void printTo(ostream & os);
  float interpret();
};


class NestedExprNode : public FactorNode {
public:
    ExprNode* exprPtr = nullptr;

    NestedExprNode(int level, ExprNode* en);
    void printTo(ostream & os);
    ~NestedExprNode();
    float interpret();
};

#endif
