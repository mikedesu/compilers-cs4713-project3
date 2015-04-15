#include "scan.h"

static char current_char;		//next char to be scanned


//gets next char from stdin
void nextchar() {
  current_char = getchar();
}

//puts a char back onto stdin
void putback() {
  ungetc(current_char, stdin);
}

//returns true if c is one of +-/*(){};
int issingle(char c) {
  if (c=='+'||c=='-'||c=='/'||c=='*'||c=='('||c==')'||c=='{'||c=='}'||c=='<'||c=='>'||c==';'||c=='=')
    return(1);
  return(0);
}

//used to determine type of single-character tokens (only works if issingle(c))
TOKEN_TYPE gettype(char c) {
  TOKEN_TYPE type;
  switch (c) {
    case '+':  type=PLUS;  break;
    case '-':  type=MINUS;  break;
    case '*':  type=MULT;  break;
    case '/':  type=DIVIDE;  break;
    case '(':  type=LPARAN;  break;
    case ')':  type=RPARAN;  break;
    case '{':  type=LBRACE;  break;
    case '}':  type=RBRACE;  break;
    case '<':  type=LT;  break;
    case '>':  type=GT;  break;
    case ';':  type=SEMICOLON;  break;
    case '=':  nextchar();
               if (current_char=='=')
                 type=ASSIGN;
               else {
                 putback();
                 type=EQ;
               }
               break;
    default:  break;
  }
  return type;
}


//used to determine if a character following an expected int/float is a valid terminating character
int ischarvalid(char c) { 
  if(isspace(c)||c=='+'||c=='-'||c=='/'||c=='*'||c==')'||c==';')
    return(1);
  return(0);
}


Token scan() {
  Token t;
  t.len = 0;
  nextchar();
  while (current_char!=EOF) {

    //white-space cases
    while(isspace(current_char)) {
      nextchar();
    }

    //single-character-termination cases (including ==)
    if(issingle(current_char)) {
      t.type = gettype(current_char);
      switch(t.type) {
        case 1:   t.value=";";   break;
        case 2:   t.value="(";   break; 
        case 3:   t.value=")";   break; 
        case 4:   t.value="{";   break; 
        case 5:   t.value="}";   break; 
        case 6:   t.value="<";   break; 
        case 7:   t.value=">";   break; 
        case 8:   t.value="==";  t.len++;  break;
        case 9:   t.value="=";   break;
        case 10:  t.value="+";   break; 
        case 11:  t.value="-";   break; 
        case 12:  t.value="*";   break; 
        case 13:  t.value="/";   break; 
        default:  break;
      }
      t.len++;
      return(t);
    }

    //integer and floating-point constant cases (max # of digits 100)
    if (isdigit(current_char)) {
      char temp[100];
      int i = 0;
      int isfloat = 0;
      temp[i++]=current_char;
      nextchar(); 
      while (isdigit(current_char)) {
        temp[i++]=current_char;
        nextchar();
      }
      if (current_char=='.') {
        isfloat=1;
        temp[i++]=current_char;
        nextchar();
        while (isdigit(current_char)) {
          temp[i++]=current_char;
          nextchar();
        }
      }
      if ( ischarvalid(current_char)) {
        putback();
        temp[i]='\0';
        t.value = (char *)malloc(strlen(temp));
        strcpy(t.value, temp);
        t.len = strlen(t.value);
        if (isfloat) 
          t.type = FCONST;
        else
          t.type = ICONST;
        return(t);
      }
      else {
        t.type = ERROR;
        while(!(isspace(current_char)||current_char==';')) {
          temp[i++] = current_char;
          nextchar();
        }
        putback();
        temp[i] = '\0';
        t.value = (char *)malloc(strlen(temp));
        strcpy(t.value, temp);
        t.len = strlen(t.value);
        return(t);
      }
    }


    //keywords & identifier cases
    if (isalpha(current_char)) {
      char temp[100];
      int i = 0;
      while (isalnum(current_char)) {
        temp[i++] = current_char;
        nextchar();
      }
      putback();
      temp[i] = '\0';
      t.type = ID;
      if (!strcmp(temp, "if")) 
        t.type = IF;
      else if (!strcmp(temp, "else")) 
        t.type = ELSE;
      else if (!strcmp(temp, "int")) 
        t.type = INT;
      else if (!strcmp(temp, "float"))
        t.type = FLOAT;
      else if (!strcmp(temp, "while"))
        t.type = WHILE;
      t.value = (char *)malloc(strlen(temp));
      strcpy(t.value, temp);
      t.len = strlen(t.value);
      return(t);
      
    }

    if (current_char==EOF) 
      break;

    //else -- error
    t.type = ERROR;
    t.value = (char *)malloc(sizeof(char));
    t.value[0] = current_char;
    t.len = 1;
    return(t);

  }
  if (current_char==EOF) {
    t.type = END;
    t.value = "";
    t.len = 1;
  }
  return(t);
}
