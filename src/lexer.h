#ifndef LEXER_H
#define LEXER_H

#include <string>

enum Token {
  tok_eof = -1,

  tok_def = -2,
  tok_extern = -3,

  tok_identifier = -4,
  
  tok_number = -5,
};

std::string IdentifierStr;
double NumVal;

int gettok();

#endif

