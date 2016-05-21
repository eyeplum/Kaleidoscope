#include "parser.h"
#include <cstdio>
#include <utility>
#include <map>

// Helper functions

static int currentToken;
static int getNextToken() {
  return currentToken = gettok();
}

std::unique_ptr<ExprAST> LogError(const char *errorMessage) {
  fprintf(stderr, "Error: %s\n", errorMessage);
  return nullptr;
}

std::unique_ptr<PrototypeAST> LogErrorP(const char * errorMessage) {
  LogError(errorMessage);
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

static std::unique_ptr<PrototypeAST> parsePrototype() {
  if (currentToken != tok_identifier) {
    return LogErrorP("Expect function name in prototype");
  }

  std::string functionName = IdentifierStr;
  getNextToken();

  if (currentToken != '(') {
    return LogErrorP("Expect '(' in prototype");
  }

  std::vector<std::string> argNames;
  while (getNextToken() == tok_identifier) {
    argNames.push_back(IdentifierStr);
  }

  if (currentToken != ')') {
    return LogErrorP("Expect ')' in prototype");
  }

  getNextToken();

  return llvm::make_unique<PrototypeAST>(functionName, std::move(argNames));
}

static std::unique_ptr<FunctionAST> parseDefinition() {
  getNextToken();
  auto prototype = parsePrototype();
  if (!prototype) {
    return nullptr;
  }

  if (auto expression = parseExpression()) {
    return llvm::make_unique<FunctionAST>(std::move(prototype), std::move(expression));
  }

  return nullptr;
}

static std::unique_ptr<PrototypeAST> parseExtern() {
  getNextToken();
  return parsePrototype();
}

static std::unique_ptr<FunctionAST> parseTopLevelExpression() {
  if (auto expression = parseExpression()) {
    auto anonymousPrototype = llvm::make_unique<PrototypeAST>("", std::vector<std::string>());
    return llvm::make_unique<FunctionAST>(std::move(anonymousPrototype), std::move(expression));
  }

  return nullptr;
}


// function precedence

static std::map<char, int> BinaryOperationPrecedence;

static int getTokenPrecedence() {
  if (!isascii(currentToken)) {
    return -1;
  }

  int result = BinaryOperationPrecedence[currentToken];
  if (result <= 0) {
    return -1;
  }
  return result;
}


// main loop

static void handleDefinition() {
  if (parseDefinition()) {
    fprintf(stderr, "Parsed a function definition.\n");
  } else {
    getNextToken();
  }
}

static void handleExtern() {
  if (parseExtern()) {
    fprintf(stderr, "Parsed an extern.\n");
  } else {
    getNextToken();
  }
}

static void handleTopLevelExpression() {
  if (parseTopLevelExpression()) {
    fprintf(stderr, "Parsed a top level expression.\n");
  } else {
    getNextToken();
  }
}

static void mainLoop() {
  while (true) {
    fprintf(stderr, "ready > ");
    switch (currentToken) {
      case tok_eof:
        fprintf(stderr, "\nGoodbye.\n");
        return;
      case ';':
        getNextToken();
        break;
      case tok_def:
        handleDefinition();
        break;
      case tok_extern:
        handleExtern();
        break;
      default:
        handleTopLevelExpression();
        break;
    }
  }
}


// main func and status

int main(int argc, char *argv[]) {
  // Insert operation precedence
  BinaryOperationPrecedence['<'] = 10;
  BinaryOperationPrecedence['+'] = 20;
  BinaryOperationPrecedence['-'] = 20;
  BinaryOperationPrecedence['*'] = 40;

  // print initial hint
  fprintf(stderr, "ready > ");
  getNextToken();

  // start main loop
  mainLoop();
  
  return 0;
}

