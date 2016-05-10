#include <string>
#include <memory>
#include <vector>
#include <utility>
#include "parser.h"

using namespace std;


// AST definitions

class ExprAST {
public:
  virtual ~ExprAST() {}
};


class NumberExprAST : public ExprAST {
  double value;

public:
  NumberExprAST(double value) : value(value) {}
};


class VariableExprAST : public ExprAST {
  string name;

public:
  VariableExprAST(const string &name) : name(name) {}
};


class BinaryExprAST : public ExprAST {
  char operand;
  unique_ptr<ExprAST> lhs, rhs;
  
public:
  BinaryExprAST(char operand, unique_ptr<ExprAST> lhs, unique_ptr<ExprAST> rhs)
    : operand(operand), lhs(move(lhs)), rhs(move(rhs)) {}
};


class CallExprAST : public ExprAST {
  string callee;
  vector<unique_ptr<ExprAST> > args;

public:
  CallExprAST(const string &callee, vector<unique_ptr<ExprAST> > args)
    : callee(callee), args(move(args)) {}
};


class PrototypeAST : public ExprAST {
  string name;
  vector<string> argnames;

public:
  PrototypeAST(const string &name, vector<string> argnames)
    : name(name), argnames(move(argnames)) {}
};


class FunctionAST : public ExprAST {
  unique_ptr<PrototypeAST> prototype;
  unique_ptr<ExprAST> body;

public:
  FunctionAST(unique_ptr<PrototypeAST> prototype, unique_ptr<ExprAST> body)
    : prototype(move(prototype)), body(move(body)) {}
};


// Helper functions

static int currentToken;
static int getNextToken() {
  return currentToken = gettok();
}


// Parser

