//	parse.h

#include <stdio.h>
#include <stdlib.h>
#include "scan.h"


void useProduction(const char *production);
void init();
Token currentToken();
char *getType(int t);

void parseTerminal(int type);
void parseExp();
void parseExpA();
void parseExpB();
void parseExpC();
void parseExpD();

void parseType();
void parseTypeDecl();
void parseStmt();
void parseStmtA();
void parseStmtBlock();
void parseStmtList();

void parseFuncDefn();
void parseProgram();
