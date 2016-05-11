#include "../src/lexer.h"

int main(int argc, char *argv[]) {
  printf("=== Kaleidoscope Lexer Tests ===\n");
  while (true) {
    int type = gettok();
    switch(type) {
      case tok_eof:
        printf("EOF\n");
        return 0;
      case tok_def:
        printf("keyword: define\n");
        break;
      case tok_extern:
        printf("keyword: extern\n");
        break;
      case tok_identifier:
        printf("identifier: %s\n", IdentifierStr.c_str());
        break;
      case tok_number:
        printf("number: %lf\n", NumVal);
        break;
      default:
        printf("unknown: %c\n", type);
        break;
    }
  }
  return 0;
}

