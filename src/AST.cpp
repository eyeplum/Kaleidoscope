#include "AST.h"
#include <cstdio>

std::unique_ptr<llvm::Module> *currentModule;
llvm::IRBuilder<> builder(llvm::getGlobalContext());
std::map<std::string, llvm::Value *> namedValues;

llvm::Value *ErrorV(const char *string) {
  fprintf(stderr, "Error: %s\n", string);
  return nullptr;
}

