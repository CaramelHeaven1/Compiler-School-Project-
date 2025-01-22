/*********************************************************************
Name: Caleb Hickman-Thomas
Course: CSE 4714
Project: Part 4
Purpose of File: tree nodes cpp file
*********************************************************************/

#include "parse_tree_nodes.h"
#include "parser.h"

bool printDelete = false;  


#define EPSILON 0.001

static bool truth(float F) 
{
return !((EPSILON > F) && (F > -EPSILON));
}


static void indent(int level) 
{
  for (int i = 0; i < level; i++)
  {
    cout << ("| ");
  }
}

symbolTableT symbolTable; // Symbol Table

//symbol table search
bool query(string temp)
{
  symbolTableT::iterator it;
  it = symbolTable.find(temp);
  return !(it == symbolTable.end());
}


ostream& operator<<(ostream& os, FactorNode& fn) 
{
  os << endl; indent(fn._level); os << "(factor (";
  fn.printTo(os);
  os << endl; indent(fn._level); os << "factor) ";
	return os;
}
float FactorNode::interpret() 
{
  return this->interpret();
}
FactorNode::~FactorNode() {}

//integer literal node
IntLitNode::IntLitNode(int level, int value) 
{
  _level = level - 1 ;
  int_literal = value;
}
IntLitNode::~IntLitNode() 
{
  if(printDelete)
    {
      cout << "Deleting FactorNode:IntLitNode " << endl;
    }
}
void IntLitNode::printTo(ostream& os) 
{
	os << " INTLIT: " << int_literal << " ) ";
}
float IntLitNode::interpret()
{
  return int_literal;
}

//float literal node 
FloatLitNode::FloatLitNode(int level, float value) 
{
  _level = level - 1 ;
  float_literal = value;
}
FloatLitNode::~FloatLitNode() 
{
  if(printDelete)
  {
    std::cout << "Deleting FactorNode:FloatLitNode " << endl;
  }
}
void FloatLitNode::printTo(ostream& os) 
{
	os << " FLOATLIT: " << float_literal << " ) "; 
}
float FloatLitNode::interpret()
{
  return float_literal;
}

//identifier node 
IdNode::IdNode(int level, string name) 
{
  _level = level - 1 ;
  id = new string(name);
}
IdNode::~IdNode() 
{
  if(printDelete)
  {
    cout << "Deleting FactorNode:IdNode " << endl;
  }
	delete id;
	id = nullptr;
}
void IdNode::printTo(ostream& os) 
{
	os << " IDENT: " << *id << " ) ";
}
float IdNode::interpret()
{
  symbolTableT::iterator variable = symbolTable.find(*id);
  return variable->second;
}

//not node 
NotNode::NotNode(int level, FactorNode* factorNode) 
{
  _level = level - 1 ;
  factorNodePtr = factorNode;
}
NotNode::~NotNode() 
{
  if(printDelete)
  {
    cout << "Deleting FactorNode:NotNode " << endl;
  }
  delete factorNodePtr;
  factorNodePtr = nullptr;
}
void NotNode::printTo(ostream& os) 
{
	os << "NOT " << *factorNodePtr << ") "; 
}
float NotNode::interpret()
{

  if(truth(factorNodePtr->interpret())){
    return 0.0;
  }else{
    return 1.0;
  }
}

//minus node
MinusNode::MinusNode(int level, FactorNode* factorNode) 
{
  _level = level - 1 ;
  factorNodePtr = factorNode;
}
MinusNode::~MinusNode() 
{
  if(printDelete)
  {
    cout << "Deleting FactorNode:MinusNode " << endl;
  }
  delete factorNodePtr;
  factorNodePtr = nullptr;
}
void MinusNode::printTo(ostream& os) 
{
	os << "- " << *factorNodePtr << ") ";
}
float MinusNode::interpret()
{
  return -(factorNodePtr->interpret());
}

//nested expression
NestedExprNode::NestedExprNode(int level, ExprNode* en) 
{
  _level = level - 1 ;
	exprPtr = en;
}
void NestedExprNode::printTo(ostream& os) 
{
	os << *exprPtr << ") ";
}
NestedExprNode::~NestedExprNode() 
{
  if(printDelete)
  {
    cout << "Deleting FactorNode:NestedExprNode " << endl;
  }
  delete exprPtr;
  exprPtr = nullptr;
}
float NestedExprNode::interpret()
{
  return exprPtr->interpret();
}




TermNode::TermNode(int level, FactorNode* factorNodePtr) 
{
  _level = level - 1 ;
}
ostream& operator<<(ostream& os, TermNode& tn) 
{
  os << endl; indent(tn._level); os << "(term ";
	os << *(tn.firstFactor);

	int length = tn.restFactorOps.size();
	for (int i = 0; i < length; ++i) 
  {
		int op = tn.restFactorOps[i];
    if (op == TOK_MULTIPLY) 
    {
      os << endl; indent(tn._level); os << "* ";
    } 
    else if(op == TOK_DIVIDE) 
    {
      os << endl; indent(tn._level); os << "/ ";
    }
    else
    {
      os << endl; indent(tn._level); os << "AND ";
    }
		os << *(tn.restFactors[i]);
	}
  os << endl; indent(tn._level); os << "term) ";
	return os;
}
TermNode::~TermNode() 
{
  if(printDelete)
  {
    cout << "Deleting TermNode " << endl;
  }
	delete firstFactor;
	firstFactor = nullptr;

	int length = restFactorOps.size();
	for (int i = 0; i < length; ++i) 
  {
		delete restFactors[i];
		restFactors[i] = nullptr;
	}
}
float TermNode::interpret()
{
  float returnValue = firstFactor->interpret();
  float nextValue;
  int length = restFactorOps.size();
  for (int i = 0; i < length; ++i) {
    // get the value of the next Factor
    nextValue = restFactors[i]->interpret();
    
    // perform the operation (* or /) that separates the Factors
    switch(restFactorOps[i]) 
    {
      case TOK_MULTIPLY:
        returnValue = returnValue * nextValue;
      break;
      case TOK_DIVIDE:
        returnValue = returnValue / nextValue;
      break;
      case TOK_AND:
        //truth = returnValue && nextValue;
      break;
    }
  }
  return returnValue;
}





Simple_ExprNode::Simple_ExprNode(int level, TermNode* termNodePtr) 
{
  _level = level - 1 ;
}
ostream& operator<<(ostream& os, Simple_ExprNode& en) 
{
  os << endl; indent(en._level); os << "(simple_exp ";
	os << *(en.firstTerm);

	int length = en.restTermOps.size();
	for (int i = 0; i < length; ++i) 
  {
		int op = en.restTermOps[i];
    if (op == TOK_PLUS) 
    {
      os << endl; indent(en._level); os << "+ ";
    }
    else if( op == TOK_MINUS)
    {
      os << endl; indent(en._level); os << "- ";
    } 
    else 
    {
      os << endl; indent(en._level); os << "OR ";
    }
		os << *(en.restTerms[i]);
	}
  os << endl; indent(en._level); os << "simple_exp) ";
	return os;
}
Simple_ExprNode::~Simple_ExprNode() {
  if(printDelete)
  {
    cout << "Deleting SimpleExpNode " << endl;
  }
	delete firstTerm;
	firstTerm = nullptr;

	int length = restTermOps.size();
	for (int i = 0; i < length; ++i) 
  {
		delete restTerms[i];
		restTerms[i] = nullptr;
	}
}
float Simple_ExprNode::interpret() 
{

 // get the value of the first Factor
  float returnValue = firstTerm->interpret();
  float nextValue;
  int length = restTermOps.size();
  for (int i = 0; i < length; ++i) 
  {
    // get the value of the next Term
    nextValue = restTerms[i]->interpret();
    
    // perform the operation (+ or -) that separates the Terms
    switch(restTermOps[i]) {
      case TOK_PLUS:
        returnValue = returnValue + nextValue;
      break;
      case TOK_MINUS:
        returnValue = returnValue - nextValue;
      break;
      case TOK_OR:
        if(truth(returnValue) || truth(nextValue))
        {
          returnValue = 1.0;
        }else
        {
          returnValue = 0.0;
        }
      break;
    }
  }
  return returnValue;
}


ExprNode::ExprNode(int level, Simple_ExprNode* simpleExprNodePtr) 
{
  _level = level;
}
ostream& operator<<(ostream& os, ExprNode& en) 
{
  os << endl; indent(en._level); os << "(expression ";
	os << *(en.firstSimpleExpr);
	int op = en.simpleExpOp;
    if (op == TOK_EQUALTO) 
    {
      os << endl; indent(en._level); os << "= ";
    } 
    else if(op == TOK_GREATERTHAN)
    {
      os << endl; indent(en._level); os << "> ";
    }
    else if(op == TOK_LESSTHAN)
    {
      os << endl; indent(en._level); os << "< ";
    }
    else
    {
      os << endl; indent(en._level); os << "<> ";
    }
	  if(en.secondSimpleExpr == nullptr)
    {
      os << *(en.firstSimpleExpr);
    }
    else
    {
      os << *(en.secondSimpleExpr);
    }
    os << endl; indent(en._level); os << "expression) ";
	return os;
}

ExprNode::~ExprNode() 
{
  if(printDelete)
  {
    cout << "Deleting ExpressionNode " << endl;
  }
	delete firstSimpleExpr;
	firstSimpleExpr = nullptr;
  delete secondSimpleExpr;
	secondSimpleExpr = nullptr;
}
float ExprNode::interpret()
{
 
  float first = firstSimpleExpr->interpret();
  if (secondSimpleExpr != nullptr)
  {
    float second = secondSimpleExpr->interpret();
    switch(simpleExpOp)
    {
      case TOK_EQUALTO:
        if(abs(first - second) <= EPSILON)
        {
          return 1.0; // true
        }
        else
        {
          return 0.0; // false
        }
        break;
        case TOK_NOTEQUALTO:
        if(abs(first - second) > EPSILON)
        {
          return 1.0; // true
        }
        else
        {
          return 0.0; // false
        }
        break;
        case TOK_LESSTHAN:
        if(first < second)
          return 1.0; // true
        else
          return 0.0; // false  
      break;
      case TOK_GREATERTHAN:
        if(first > second)
        {
          return 1.0; // true
        }else
        {
          return 0.0; // false
        }
        break;
    }
  }
  return first;
}




ProgramNode::ProgramNode(int level, BlockNode* BlockNodePtr) 
{
  _level = level - 1 ;
  firstBlock = BlockNodePtr;  
}
ostream& operator<<(ostream& os, ProgramNode& tn) 
{
  os << endl; indent(tn._level); os << "(program ";
	os << *(tn.firstBlock);
  os << endl; indent(tn._level); os << "program) ";
	return os;
}
ProgramNode::~ProgramNode() 
{
  if(printDelete)
  {
    cout << "Deleting ProgramNode " << endl;
  }
	delete firstBlock;
	firstBlock = nullptr;
}
void ProgramNode::interpret()
{
  firstBlock->interpret();
}




//BLOCK NODE
BlockNode::BlockNode(int level, compoundStmtNode* CompoundNodePtr) 
{
  _level = level - 1 ;
  firstCompound = CompoundNodePtr;
}
ostream& operator<<(ostream& os, BlockNode& bn) 
{
  os << endl; indent(bn._level); 
  os << "(block ";
	os << *(bn.firstCompound);
  os << endl; indent(bn._level);
  os << "block) ";
	return os;
}
BlockNode::~BlockNode() 
{
  if(printDelete)
  {
    cout << "Deleting BlockNode " << endl;
  }
	delete firstCompound;
	firstCompound = nullptr;
}
void BlockNode::interpret()
{
  firstCompound->interpret();  
}




compoundStmtNode::compoundStmtNode(int level) 
{
  _level = level - 1 ;
}
compoundStmtNode::~compoundStmtNode() 
{
  if(printDelete)
    cout << "Deleting StatementNode:CompoundStmtNode" << endl;

  for(int i = 0;i < statements.size();i++)
  {
    delete statements[i];
  }
}
void compoundStmtNode::printTo(ostream& os) 
{
  os << endl; indent(_level);
  os << "(compound_stmt ";

  int length = statements.size();
  for(int i = 0;i < length;i++)
  {
    os << *(statements[i]);
  }
  os << endl; indent(_level);
  os << "compound_stmt)";
}
void compoundStmtNode::interpret()
{
  for(int i = 0; i< statements.size();i++)
  {
    statements[i]->interpret();
  }
}

StatementNode::~StatementNode() {}
ostream& operator<<(ostream& os, StatementNode& fn) 
{
  fn.printTo(os);
	return os;
}
void StatementNode::interpret()
{
  this->interpret();
}
writeStmtNode::writeStmtNode(int level) 
{
  _level = level-1;
}
writeStmtNode::~writeStmtNode() 
{
  if(printDelete) 
    cout << "Deleting StatementNode:WriteStmtNode "<< endl;
}
void writeStmtNode::printTo(ostream& os) 
{
	os << endl; indent(_level);
  os << "(write_stmt ";
  if (!ident.empty())
  {
    os << "( "<< ident << " )";
  }
  os << endl; indent(_level);
  os << "write_stmt)";
}
void writeStmtNode::interpret()
{
  switch (type)
  {
    case TOK_STRINGLIT:
      cout<<ident.substr(1, ident.length() - 2) << endl;
    break;
    case TOK_IDENT:
      symbolTableT::iterator variable = symbolTable.find(ident);
      cout << variable->second << endl;
    break;
  }
}





readstmtNode::readstmtNode(int level) 
{
  _level = level - 1 ;
}
readstmtNode::~readstmtNode() 
{
  if(printDelete) 
    cout << "Deleting StatementNode:ReadStmtNode" << endl;
}
void readstmtNode::printTo(ostream& os) 
{
  os << endl; indent(_level);
  os << "(read_stmt";
  if (!read.empty())
  {
    os << " ( " << read << " )";
  }
  os << endl; indent(_level);
  os << "read_stmt)";
}
void readstmtNode::interpret()
{
  symbolTableT::iterator variable = symbolTable.find(read);
  cin >> variable->second;
}





assignNode::assignNode(int level) 
{
  _level = level - 1 ;
}
assignNode::~assignNode() 
{
  if(printDelete) 
    cout << "Deleting StatementNode:AssignmentStmtNode " << endl;
	delete exprPtr;
  exprPtr = nullptr;
}
void assignNode::printTo(ostream& os) 
{
	os << endl; indent(_level); 
  os << "(assignment_stmt ";
  os << "( " << identifier << " := )";
  os << *exprPtr;
  os << endl; indent(_level);
  os << "assignment_stmt) ";
}
void assignNode::interpret(){

  if(query(identifier) == true)
  { 
    symbolTableT::iterator variable = symbolTable.find(identifier);
    variable->second = exprPtr->interpret();
  }
  else
  {
    cout<<"She aint here..."<<endl;
  }
}





ifstmtNode::ifstmtNode(int level) 
{
  _level = level - 1 ;
}
ifstmtNode::~ifstmtNode() 
{
  if(printDelete) 
    cout << "Deleting StatementNode:IfStmtNode " << endl;
  delete expression;
  expression = nullptr;
  delete then_stmt;
  then_stmt = nullptr;
  delete else_stmt;
  else_stmt = nullptr;
}
void ifstmtNode::printTo(ostream& os) 
{
	os << endl; indent(_level); os << "(if_stmt";
  os << *expression; os << endl; indent(_level);
  if (then_stmt != nullptr)
  {
    os << "(then"; os << *then_stmt ;
    os << endl; indent(_level);
    os << "then)"; os << endl; indent(_level);
  }
  if (else_stmt != nullptr)
  {
    os << "(else"; os << *else_stmt;
    os << endl; indent(_level);
    os << "else)"; os << endl; indent(_level);
  }
  os << "if_stmt)";
}
void ifstmtNode::interpret()
{
  if(truth(expression->interpret()))
  {
    then_stmt->interpret();
  }
  else
  {
    if (else_stmt !=nullptr)
    {
      else_stmt->interpret();
    }  
  }
}





whileNode* while_stmnt();whileNode::whileNode(int level) 
{
  _level = level-1;
}
whileNode::~whileNode() 
{
  if(printDelete) 
    cout << "Deleting StatementNode:WhileStmtNode " << endl;
  delete expression;
  expression = nullptr;
  delete statement;
  statement = nullptr;
}
void whileNode::printTo(ostream& os) 
{
	os << endl; indent(_level);
  os << "(while_stmt";
  os  << *expression;
  os << *statement;
  os << endl; indent(_level);
  os << "while_stmt)";
}
void whileNode::interpret()
{
  while(truth(expression->interpret()))
    {
      statement->interpret();
    }
}


