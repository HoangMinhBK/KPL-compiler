/* Scanner
 * @copyright (c) 2008, Hedspi, Hanoi University of Technology
 * @author Huu-Duc Nguyen
 * @version 1.0
 */
/*
  Solution by Nguyen Luu Hoang Minh
  ID: 20194798
  Class: ICT 02 - K64
  Hanoi University of Science and Technology
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "reader.h"
#include "charcode.h"
#include "token.h"
#include "error.h"

extern int lineNo;
extern int colNo;
extern int currentChar;

extern CharCode charCodes[];
bool isBackSlash = false;

/***************************************************************/

void skipBlank() {
  // TODO
  while ((currentChar != -1) && (charCodes[currentChar] == CHAR_SPACE)){
    readChar();
  }
}

void skipComment(int choice) {
  // TODO
  // To determine the comment is single-lined or multi-lined
  // Single-lined comment:
  while(1){ // Inside comment
    readChar(); // Get next character

    //single-lined comment
    if (charCodes[choice] == CHAR_LPAR){
      if (((char) currentChar == '\n')) { // unclosed single-comment 
          error(ERR_ENDOFCOMMENT, lineNo, colNo);
      }
      else if (charCodes[currentChar] == CHAR_TIMES){ // *
        readChar();
        if (charCodes[currentChar] == CHAR_RPAR){ // End of comment *)
          readChar();  // Finish reading a comment, scan next character
          return;
        }
        else if (currentChar == -1){ // End of file
            error(ERR_ENDOFCOMMENT, lineNo, colNo);
          }
      }
      else if (currentChar == -1){ // End of file
        error(ERR_ENDOFCOMMENT, lineNo, colNo);
      }
    }
    
    // multi-lined comment
    else if (charCodes[choice] == CHAR_SLASH){
      if (charCodes[currentChar] == CHAR_TIMES){ // *
        readChar();
        if (charCodes[currentChar] == CHAR_SLASH){ // End of comment *)
          readChar();  // Finish reading a comment, scan next character
          return;
        }
        else if (currentChar == -1){ // End of file
            error(ERR_ENDOFCOMMENT, lineNo, colNo);
          }
      }
      else if (currentChar == -1){ // End of file
        error(ERR_ENDOFCOMMENT, lineNo, colNo);
      }
    }
  }
}

Token *readIdentKeyword(void) //Keyword and identifiers are case-insensitive
{
  // TODO
  Token *token;
  // let a token be a 
  token = makeToken(TK_IDENT, lineNo, colNo);
  int count = 0;
  while ((charCodes[currentChar] == CHAR_DIGIT) || (charCodes[currentChar] == CHAR_LETTER)|| currentChar == 95){
    if (currentChar >= 97 && (currentChar <= 122)){ // a to z 
      currentChar -=32; // if a char is letter, make it uppercase (a -> A)
    }
    token->string[count] = currentChar; // Add current character to identifier
    count++; // crease index of token->sting array
    readChar(); // get next character
  }
  token->string[count] = '\0'; //end of string


  if (count > MAX_IDENT_LEN)
    error(ERR_IDENTTOOLONG, token->lineNo, token->colNo);
  else if (count <= MAX_IDENT_LEN){  
    // check if this string is a keyword or not
    if (checkKeyword(token->string)!= TK_NONE){
      token->tokenType = checkKeyword(token->string);
    } 
  }
  return token;
}

Token* readNumber(void) {
  // TODO
  Token *token;
  token = makeToken(TK_NUMBER, lineNo, colNo);
  int count = 0;
  while ((currentChar != -1) && (charCodes[currentChar]) == CHAR_DIGIT){
    token->string[count] = currentChar;
    count ++;
    readChar();
  }
  token->string[count] = '\0';

  // An identifier cannot start with a number
  //underscore - lowercase letter - uppercase letter
  if (currentChar == 95 || (currentChar >= 65 && currentChar <=90) || (currentChar >=97 && currentChar <=122)){
    printf("%d-%d:Invalid identifier!", token->lineNo, token->colNo);
    exit(0);
  }

  // check if the string is a valid number
  return token;
}

Token* readConstChar(void) {
  // TODO
  Token *token;
  token = makeToken(TK_CHAR, lineNo, colNo);
  readChar();
  if (currentChar >= 32 && currentChar <= 126){ //Decimal code of printable characters  (32 -> 126)
    token->string[0] = currentChar; // Store the character 
    token->string[1] = '\0'; // End of string
    readChar();
    if (currentChar == 92){
      readChar();
      if (charCodes[currentChar] == CHAR_SINGLEQUOTE)
      {
        readChar(); // get next character
        return token;
      }
      else{
        readChar(); // get next character
        error(ERR_INVALIDCHARCONSTANT, token->lineNo, token->colNo);
        return NULL;
      }
    }
    else{
      error(ERR_INVALIDSYMBOL, token->lineNo, token->colNo);
      return NULL;
    }
  }
  else{
    error(ERR_INVALIDCHARCONSTANT, token->lineNo, token->colNo);
    return NULL;
  }
}

Token* getToken() {
  Token *token;
  token = makeToken(TK_NONE, 0, 0); // Default initialization
  int ln, cn;
  int choice; // used to determine single-lined or multi-lined comment (* or /*

  if (currentChar == -1) 
    return makeToken(TK_EOF, lineNo, colNo);

  switch (charCodes[currentChar]) {
  case CHAR_SPACE: skipBlank(); return getToken();
  case CHAR_LETTER:
    return readIdentKeyword();
  case CHAR_DIGIT:
    return readNumber();
  case CHAR_PLUS: 
    token = makeToken(SB_PLUS, lineNo, colNo);
    readChar(); 
    return token;
    // ....
    // TODO
  case CHAR_LPAR:
    choice = currentChar;
    ln = lineNo;
    cn = colNo;
    readChar(); 
    if (charCodes[currentChar] == CHAR_PERIOD){
      return makeToken(SB_LSEL, ln, cn);
    }
    else if (charCodes[currentChar] == CHAR_TIMES){
      skipComment(choice);
      return getToken();
    }
    else
      return makeToken(SB_LPAR, ln, cn);
  case CHAR_RPAR:
    token = makeToken(SB_RPAR, lineNo, colNo);
    readChar();
    return token;
  case CHAR_SINGLEQUOTE:
      if (isBackSlash == true){
        isBackSlash = false;
        return readConstChar();
      }
      error(ERR_INVALIDSYMBOL, lineNo, colNo);
  case CHAR_LT:
    ln = lineNo;
    cn = colNo;
    readChar();
    if (charCodes[currentChar] == CHAR_EQ){
      return makeToken(SB_LE, ln, cn);
    }
    else
      return makeToken(SB_LT, ln, cn);
  case CHAR_GT:
    ln = lineNo;
    cn = colNo;
    readChar();
    if (charCodes[currentChar] == CHAR_EQ){
      readChar();
      return makeToken(SB_GE, ln, cn);
    }
    else{
      readChar();
      return makeToken(SB_GT, ln, cn);
    }
  case CHAR_EXCLAIMATION:
    ln = lineNo;
    cn = colNo;
    readChar();
    if (charCodes[currentChar] == CHAR_EQ){
      readChar();
      return makeToken(SB_NEQ, ln, cn);
    }
    else{
      readChar();
      error(ERR_INVALIDSYMBOL, ln, cn);
    }
  case CHAR_PERIOD:
    ln = lineNo;
    cn = colNo;
    readChar();
    if (charCodes[currentChar] == CHAR_RPAR){
      token = makeToken(SB_RSEL, ln, cn);
      readChar();
      return token;
    }
    else{
      token = makeToken(SB_PERIOD, ln, cn);
      readChar();
      return token;
    }
  case CHAR_MINUS:
    token = makeToken(SB_MINUS, lineNo, colNo);
    readChar();
    return token;
  case CHAR_TIMES:
    token = makeToken(SB_TIMES, lineNo, colNo);
    readChar();
    return token;
  case CHAR_EQ:
    token = makeToken(SB_EQ, lineNo, colNo);
    readChar();
    return token;
  case CHAR_COMMA:
    token = makeToken(SB_COMMA, lineNo, colNo);
    readChar();
    return token;
  case CHAR_SEMICOLON:;
    token = makeToken(SB_SEMICOLON, lineNo, colNo);
    readChar();
    return token;
  case CHAR_SLASH:
    choice = currentChar;
    ln = lineNo;
    cn = colNo;
    readChar();
    if (charCodes[currentChar] == CHAR_TIMES){
       skipComment(choice);
       return getToken();
    }
    else
      return makeToken(SB_SLASH, ln, cn);
  case CHAR_COLON:
    ln = lineNo;
    cn = colNo;
    readChar();
    if ((currentChar != -1) && (charCodes[currentChar] == CHAR_EQ)){
      readChar();
      return makeToken(SB_ASSIGN, ln, cn); // :=
    }
    else{
      readChar(); 
      return makeToken(SB_COLON, ln, cn);
    }
    // ....
  default:
    if (currentChar == 92){
      readChar();
      if (charCodes[currentChar] == CHAR_SINGLEQUOTE)
        isBackSlash = true;
      return getToken();
    }
    token = makeToken(TK_NONE, lineNo, colNo);
    error(ERR_INVALIDSYMBOL, lineNo, colNo);
    readChar();
    return token;
  }
}


/******************************************************************/

void printToken(Token *token) {

  printf("%d-%d:", token->lineNo, token->colNo);

  switch (token->tokenType) {
  case TK_NONE: printf("TK_NONE\n"); break;
  case TK_IDENT: printf("TK_IDENT(%s)\n", token->string); break;
  case TK_NUMBER: printf("TK_NUMBER(%s)\n", token->string); break;
  case TK_CHAR: printf("TK_CHAR(\'%s\')\n", token->string); break;
  case TK_EOF: printf("TK_EOF\n"); break;

  case KW_PROGRAM: printf("KW_PROGRAM\n"); break;
  case KW_CONST: printf("KW_CONST\n"); break;
  case KW_TYPE: printf("KW_TYPE\n"); break;
  case KW_VAR: printf("KW_VAR\n"); break;
  case KW_INTEGER: printf("KW_INTEGER\n"); break;
  case KW_CHAR: printf("KW_CHAR\n"); break;
  case KW_ARRAY: printf("KW_ARRAY\n"); break;
  case KW_OF: printf("KW_OF\n"); break;
  case KW_FUNCTION: printf("KW_FUNCTION\n"); break;
  case KW_PROCEDURE: printf("KW_PROCEDURE\n"); break;
  case KW_BEGIN: printf("KW_BEGIN\n"); break;
  case KW_END: printf("KW_END\n"); break;
  case KW_CALL: printf("KW_CALL\n"); break;
  case KW_IF: printf("KW_IF\n"); break;
  case KW_THEN: printf("KW_THEN\n"); break;
  case KW_ELSE: printf("KW_ELSE\n"); break;
  case KW_WHILE: printf("KW_WHILE\n"); break;
  case KW_DO: printf("KW_DO\n"); break;
  case KW_FOR: printf("KW_FOR\n"); break;
  case KW_TO: printf("KW_TO\n"); break;

  case SB_SEMICOLON: printf("SB_SEMICOLON\n"); break;
  case SB_COLON: printf("SB_COLON\n"); break;
  case SB_PERIOD: printf("SB_PERIOD\n"); break;
  case SB_COMMA: printf("SB_COMMA\n"); break;
  case SB_ASSIGN: printf("SB_ASSIGN\n"); break;
  case SB_EQ: printf("SB_EQ\n"); break;
  case SB_NEQ: printf("SB_NEQ\n"); break;
  case SB_LT: printf("SB_LT\n"); break;
  case SB_LE: printf("SB_LE\n"); break;
  case SB_GT: printf("SB_GT\n"); break;
  case SB_GE: printf("SB_GE\n"); break;
  case SB_PLUS: printf("SB_PLUS\n"); break;
  case SB_MINUS: printf("SB_MINUS\n"); break;
  case SB_TIMES: printf("SB_TIMES\n"); break;
  case SB_SLASH: printf("SB_SLASH\n"); break;
  case SB_LPAR: printf("SB_LPAR\n"); break;
  case SB_RPAR: printf("SB_RPAR\n"); break;
  case SB_LSEL: printf("SB_LSEL\n"); break;
  case SB_RSEL: printf("SB_RSEL\n"); break;
  }
}

int scan(char *fileName) {
  Token *token;

  if (openInputStream(fileName) == IO_ERROR)
    return IO_ERROR;

  token = getToken();
  while (token->tokenType != TK_EOF) {
    printToken(token);
    free(token);
    token = getToken();
  }

  free(token);
  closeInputStream();
  return IO_SUCCESS;
}

/******************************************************************/

int main(int argc, char *argv[]) {
  if (argc <= 1) {
    printf("scanner: no input file.\n");
    return -1;
  }

  if (scan(argv[1]) == IO_ERROR) {
    printf("Can\'t read input file!\n");
    return -1;
  }
    
  return 0;
}



