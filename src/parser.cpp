#include <string>
#include <memory>
#include <vector>

using namespace std;


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
}


class BinaryExprAST : public ExprAST {
  char operand;
  unique_ptr<ExprAST> lhs, rhs;
  
public:
  BinaryExprAST(char operand, unique_ptr<ExprAST> lhs, unique_ptr<ExprAST> rhs)
    : operand(operand), lhs(move(lhs)), rhs(move(rhs)) {}
}


class CallExprAST : public ExprAST {
  string callee;
  vector<unique_ptr<ExprAST> > args;

public:
  CallExprAST(const string &callee, vector<unique_ptr<ExprAST> > args)
    : callee(callee), args(move(args)) {}
}

