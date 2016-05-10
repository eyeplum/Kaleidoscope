#include "parser.h"
#include <cstdio>
#include <string>
#include <memory>
#include <vector>
#include <utility>
#include <llvm/ADT/STLExtras.h>


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
  std::string name;

public:
  VariableExprAST(const std::string &name) : name(name) {}
};


class BinaryExprAST : public ExprAST {
  char operand;
  std::unique_ptr<ExprAST> lhs, rhs;
  
public:
  BinaryExprAST(char operand, std::unique_ptr<ExprAST> lhs, std::unique_ptr<ExprAST> rhs)
    : operand(operand), lhs(std::move(lhs)), rhs(std::move(rhs)) {}
};


class CallExprAST : public ExprAST {
  std::string callee;
  std::vector<std::unique_ptr<ExprAST> > args;

public:
  CallExprAST(const std::string &callee, std::vector<std::unique_ptr<ExprAST> > args)
    : callee(callee), args(std::move(args)) {}
};


class PrototypeAST : public ExprAST {
  std::string name;
  std::vector<std::string> argnames;

public:
  PrototypeAST(const std::string &name, std::vector<std::string> argnames)
    : name(name), argnames(std::move(argnames)) {}
};


class FunctionAST : public ExprAST {
  std::unique_ptr<PrototypeAST> prototype;
  std::unique_ptr<ExprAST> body;

public:
  FunctionAST(std::unique_ptr<PrototypeAST> prototype, std::unique_ptr<ExprAST> body)
    : prototype(std::move(prototype)), body(std::move(body)) {}
};


// Helper functions

static int currentToken;
static int getNextToken() {
  return currentToken = gettok();
}

std::unique_ptr<ExprAST> LogError(const char *errorMessage) {
  fprintf(stderr, "Error: %s\n", errorMessage);
  return nullptr;
}


// Parser

static std::unique_ptr<ExprAST> parseNumberExpr() {
  auto result = llvm::make_unique<NumberExprAST>(NumVal);
  getNextToken();
  return std::move(result);
}

