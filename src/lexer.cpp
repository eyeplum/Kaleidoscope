#include "lexer.h"
#include <cctype>
#include <cstdlib>

std::string IdentifierStr;
double NumVal;

// Is number component
static bool isnumcomp(char character) {
  return isdigit(character) || character == '.';
}

// Get token from stdin
int gettok() {
  static char LastChar = ' ';
  
  // Skip whitespaces
  while (isspace(LastChar)) {
    LastChar = getchar();
  }

  // Handle alphabetic chars
  if (isalpha(LastChar)) {
    IdentifierStr = LastChar;

    // Accumulate token string
    while (isalnum((LastChar = getchar()))) {
      IdentifierStr += LastChar;
    }

    // Return token type
    if (IdentifierStr == "def") {
      return tok_def;
    }
    if (IdentifierStr == "extern") {
      return tok_extern;
    }
    
    return tok_identifier;
  }

  // Handle numbers
  if (isnumcomp(LastChar)) {
    std::string numberString;
    
    do {
      numberString += LastChar;
      LastChar = getchar();
    } while (isnumcomp(LastChar));

    NumVal = strtod(numberString.c_str(), 0);
    return tok_number;
  }

  // Handle comments
  if (LastChar == '#') {
    do {
      LastChar = getchar();
    } while (LastChar != EOF && LastChar != '\n' && LastChar != '\r');

    if (LastChar != EOF) {
      gettok();
    }
  }

  // Handle EOF
  if (LastChar == EOF) {
    return tok_eof;
  } else {
    char result = LastChar;
    LastChar = getchar();
    return result;
  }
}

