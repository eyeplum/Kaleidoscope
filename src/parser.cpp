#include "parser.h"
#include <cstdio>
#include <string>
#include <memory>
#include <vector>
#include <utility>
#include <map>
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

static std::unique_ptr<ExprAST> parsePrimaryExpr();
static int getTokenPrecedence();

static std::unique_ptr<ExprAST> parseNumberExpr() {
  auto result = llvm::make_unique<NumberExprAST>(NumVal);
  getNextToken();
  return std::move(result);
}

static std::unique_ptr<ExprAST> parseBinaryOperationRhs(int expressionPrecedence, std::unique_ptr<ExprAST> lhs) {
  while (true) {
    int tokenPrecedence = getTokenPrecedence();
    if (tokenPrecedence < expressionPrecedence) {
      return lhs;
    }

    int binaryOperation = currentToken;
    getNextToken();

    auto rhs = parsePrimaryExpr();
    if (!rhs) {
      return nullptr;
    }
    
    int nextOperationPrecedence = getTokenPrecedence();
    if (tokenPrecedence < nextOperationPrecedence) {
      rhs = parseBinaryOperationRhs(tokenPrecedence + 1, std::move(rhs));
      if (!rhs) {
        return nullptr;
      }
    }
    
    lhs = llvm::make_unique<BinaryExprAST>(binaryOperation, std::move(lhs), std::move(rhs));
  }
}

static std::unique_ptr<ExprAST> parseExpression() {
  auto lhs = parsePrimaryExpr();
  if (!lhs) {
    return nullptr;
  }

  return parseBinaryOperationRhs(0, std::move(lhs));
}

static std::unique_ptr<ExprAST> parseParenthesisExpr() {
  getNextToken();

  auto expression = parseExpression();
  
  if (!expression) {
    return nullptr;
  }

  if (currentToken != ')') {
    return LogError("Expected ')'");
  }

  getNextToken();

  return expression;
}

static std::unique_ptr<ExprAST> parseIdentifierExpr() {
  std::string identifier = IdentifierStr;

  getNextToken();

  // If not followed by '('
  // return the AST as a variable expression
  if (currentToken != '(') {
    return llvm::make_unique<VariableExprAST>(identifier);
  }

  getNextToken();
  std::vector<std::unique_ptr<ExprAST> > args;
  if (currentToken != ')') {
    while (true) {
      if (auto expression = parseExpression()) {
        args.push_back(std::move(expression));
      } else {
        return nullptr;
      }

      if (currentToken == ')') {
        break;
      } 

      if (currentToken != ',') {
        return LogError("Expect ')' or ',' in argument list");
      }

      getNextToken();
    }
  }
  getNextToken();

  return llvm::make_unique<CallExprAST>(identifier, std::move(args));
}

static std::unique_ptr<ExprAST> parsePrimaryExpr() {
  switch (currentToken) {
    case tok_number:
      return parseNumberExpr();
    case tok_identifier:
      return parseIdentifierExpr();
    case '(':
      return parseParenthesisExpr();
    default:
      return LogError("unknown token when expecting an expression");
  }
}


// main func and status

static std::map<char, int> BinaryOperationPrecedence;

static int getTokenPrecedence() {
  if (!isascii(currentToken)) {
    return -1;
  }

  int result = BinaryOperationPrecedence[currentToken];
  if (result < 0) {
    return -1;
  }
  return result;
}

int main(int argc, char *argv[]) {
  // Insert operation precedence
  BinaryOperationPrecedence['<'] = 10;
  BinaryOperationPrecedence['+'] = 20;
  BinaryOperationPrecedence['-'] = 20;
  BinaryOperationPrecedence['*'] = 30;

  // ...
  
  return 0;
}

