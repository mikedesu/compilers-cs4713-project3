#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef 
enum {
  SEMICOLON = 1, LPARAN, RPARAN, LBRACE, RBRACE, LT, GT, 
  ASSIGN, EQ, PLUS, MINUS, MULT, DIVIDE, WHILE, IF, ELSE, INT, FLOAT, 
ID, 
  ICONST, FCONST, ERROR, END
} TOKEN_TYPE;

typedef struct {
  TOKEN_TYPE type;  
  char *value;  
  unsigned len; 
} Token;


void nextchar();
void putback();
int issingle(char c);
TOKEN_TYPE gettype(char c);
int ischarvalid(char c);
Token scan();
