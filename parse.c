#include "parse.h"

static Token token;
static char stmtFlag;			//used for parsing stmtLists

//parsing with a production
void useProduction(const char* production) {
  printf("Use Production %s\n",production);
}

//initialize the token
void init() {
  token = scan();
}

//return the current token
Token currentToken() {
  if (token.type==0) 
    token = scan();
  return token;
}

//consume the current token
void consumeTerminal() {
  printf("Consume \"%s\"\n", token.value);
  token.value = "";
  token.type = 0;
  token.len = 0;
}

//returns a string representation of a TOKENTYPE
char *getType(int t) {
  switch (t) {
    case 1:  return "SEMICOLON";  break;
    case 2:  return "LPARAN";  break;
    case 3:  return "RPARAN";  break;
    case 4:  return "LBRACE";  break;
    case 5:  return "RBRACE";  break;
    case 6:  return "LT";  break;
    case 7:  return "GT";  break;
    case 8:  return "EQ";  break;
    case 9:  return "ASSIGN";  break;
    case 10:  return "PLUS";  break;
    case 11:  return "MINUS";  break;
    case 12:  return "MULT";  break;
    case 13:  return "DIVIDE";  break;
    case 14:  return "WHILE";  break;
    case 15:  return "IF";  break;
    case 16:  return "ELSE";  break;
    case 17:  return "INT";  break;
    case 18:  return "FLOAT";  break;
    case 19:  return "ID";  break;
    case 20:  return "ICONST";  break;
    case 21:  return "FCONST";  break;
    case 22:  return "ERROR";  break;
    case 23:  return "END";  break;
    default:  printf("Error:  Invalid token type %d\n", t);  break;
  }
  return NULL;
}

//parses a terminal token
void parseTerminal(int type) {
  if ((currentToken()).type==type) {
    consumeTerminal();
    return;
  }
  else {
    printf("\nERROR:  expecting %s but get %s\n", getType(type), getType((currentToken()).type));
    exit(1); 
  }
}

//parses an expression
void parseExp() {
  useProduction("exp ::= expA expB");
  parseExpA();
  parseExpB();
}

//parses a type-a expression
void parseExpA() {
  useProduction("expA ::= expC expD");
  parseExpC();
  parseExpD();
}

//parses a type-b expression
void parseExpB() {
  Token temp = currentToken();
  if (temp.type==6) {				//less than
    useProduction("expB ::= < expA expB");
    parseTerminal(6);
    parseExpA();
    parseExpB();
    return;
  }
  if (temp.type==7) {				//greater than
    useProduction("expB ::= > expA expB");
    parseTerminal(7);
    parseExpA();
    parseExpB();
    return;
  }
  if (temp.type==8) {				//equal to
    useProduction("expB ::= == expA expB");
    parseTerminal(8);
    parseExpA();
    parseExpB();
  }
  if (temp.type==10) {				//addition
    useProduction("expB ::= + expA expB");
    parseTerminal(10);
    parseExpA();
    parseExpB();
    return;
  }
  if (temp.type==11) {				//subtraction
    useProduction("expB ::= - expA expB");
    parseTerminal(11);
    parseExpA();
    parseExpB();
    return;
  }
  else {					//end of expB
    useProduction("expB ::= epsilon");
    return;
  }
}

//parses a type-d expression
void parseExpD() {
  Token temp = currentToken();
  if (temp.type==12) {				//multiplication
    useProduction("expD ::= * expC expD");
    parseTerminal(12);
    parseExpC();
    parseExpD();
    return;
  }
  if (temp.type==13) {				//division
    useProduction("expD ::= / expC expD");
    parseTerminal(13);
    parseExpC();
    parseExpD();
    return;
  }
  else {					//end of expD
    useProduction("expD ::= epsilon");
    return;
  }
}

//parses a type-c expression
void parseExpC() {
  Token temp = currentToken();
  if (temp.type==19) {			//identifiers
    useProduction("expC ::= ID");
    parseTerminal(19);
    return;
  }
  if (temp.type==20) {			//integer constants
    useProduction("expC ::= ICONST");
    parseTerminal(20);
    return;
  }
  if (temp.type==21) {			//float constants
    useProduction("expC ::= FCONST");
    parseTerminal(21);
    return;
  }
  if (temp.type==11) {			//negative expressions
    useProduction("expC ::= -exp");
    parseTerminal(11);
    parseExp();
    return;
  }
  if (temp.type==2) {			//expressions enclosed in paranthesis
    useProduction("expC ::= (exp)");
    parseTerminal(2);
    parseExp();
    parseTerminal(3);
    return;
  }
  else {
    printf("\nERROR:  Invalid production:  expC ::= %s\n", getType(temp.type));
    exit(1);
  }
}

//parses a 'type' (used in type declarations)
void parseType() {
  Token temp = currentToken();
  if (temp.type==17) {			//integer type
    useProduction("type ::= INT");
    parseTerminal(17);
    return;
  }
  if (temp.type==18) {			//float type
    useProduction("type ::= FLOAT");
    parseTerminal(18);
    return;
  }
  else {
    printf("\nERROR:  Invalid production:  type ::= %s\n", getType(temp.type));
    exit(1);
  }  
}

//parses a type declaration
void parseTypeDecl() {
  useProduction("typeDecl ::= type ID");
  parseType();
  Token temp = currentToken();
  if (temp.type==19) {
    parseTerminal(19);
    return;
  }
  else {
    printf("\nERROR:  Invalid production:  ID ::= %s\n", getType(temp.type));
    exit(1);
  }    
}

//parses a statement block
void parseStmtBlock() {
  Token temp = currentToken();
  if (temp.type==4) {
    useProduction("stmtBlock ::= LBRACE stmt RBRACE");
    parseTerminal(4);
    parseStmtList();
    parseTerminal(5);
    return;
  }
}

//parses a list of statements
void parseStmtList() {
  useProduction("stmtList ::= stmt stmtList");
  parseStmt();
  if (!stmtFlag) {
    parseStmtList();
    return;
  }
  else {  
    useProduction("stmtList ::= epsilon");
    return;
  }
}

//parses a statement
void parseStmt() {
  Token temp = currentToken();
  stmtFlag = 0;
  if (temp.type==17 || temp.type==18) {		//type declaration
    useProduction("stmt ::= typeDecl SEMICOLON");
    parseTypeDecl();
    parseTerminal(1);
    return;
  }
  if (temp.type==19) {				//id = exp;
    useProduction("stmt ::= ID ASSIGN exp SEMICOLON");
    parseTerminal(19);
    parseTerminal(9);
    parseExp();
    parseTerminal(1);
    return;
  }
  if (temp.type==14) {				//while loops
    useProduction("stmt ::= WHILE LPARAN exp RPARAN stmt");
    parseTerminal(14);
    parseTerminal(2);
    parseExp();
    parseTerminal(3);
    parseStmt();
    return;
  }

  //skipping return statements for now
  if (temp.type==15) {				//if statements
    useProduction("stmt ::= IF LPARAN exp RPARAN stmt stmtA");
    parseTerminal(15);
    parseTerminal(2);
    parseExp();
    parseTerminal(3);
    parseStmt();
    parseStmtA();
    return;    
  }
  if (temp.type==4) {				//statement blocks
    useProduction("stmt ::= stmtBlock");
    parseStmtBlock();
    return;
  }
  else {
    useProduction("stmt ::= epsilon"); 
    stmtFlag = 1;
    return;
  }
}

void parseStmtA() {
  Token temp = currentToken();
  if (temp.type==16) {				//else statements
    parseTerminal(16);
    parseStmt();
  }
  else {
    useProduction("stmtA ::= epsilon");
  }
}

void parseFuncDefn() {
  useProduction("funcdefn ::= type ID LPARAN RPARAN stmtBlock");
  parseType();
  parseTerminal(19);
  parseTerminal(2);
  parseTerminal(3);
  parseStmtBlock();
  return;
}

void parseProgram() {
  useProduction("program ::= funcdefn");
  parseFuncDefn();
  return;
}
