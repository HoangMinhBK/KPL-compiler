/* 
 * @copyright (c) 2008, Hedspi, Hanoi University of Technology
 * @author Huu-Duc Nguyen
 * @version 1.0
 */

#include <stdlib.h>

#include "reader.h"
#include "scanner.h"
#include "parser.h"
#include "error.h"

Token *currentToken;
Token *lookAhead;

void scan(void) {
  Token* tmp = currentToken;
  currentToken = lookAhead;
  lookAhead = getValidToken();
  free(tmp);
}

void eat(TokenType tokenType) {
  if (lookAhead->tokenType == tokenType) {
    printToken(lookAhead);
    scan();
  } else missingToken(tokenType, lookAhead->lineNo, lookAhead->colNo);
}
//01
void compileProgram(void) {
  assert("Parsing a Program ....");
  eat(KW_PROGRAM);
  eat(TK_IDENT);
  eat(SB_SEMICOLON);
  compileBlock();
  eat(SB_PERIOD);
  assert("Program parsed!");
}

//02 03
void compileBlock(void) {
  assert("Parsing a Block ....");
  if (lookAhead->tokenType == KW_CONST) {
    eat(KW_CONST);
    compileConstDecl();
    compileConstDecls();
    compileBlock2();
  } 
  else compileBlock2();
  assert("Block parsed!");
}

//04 05
void compileBlock2(void) {
  if (lookAhead->tokenType == KW_TYPE) {
    eat(KW_TYPE);
    compileTypeDecl();
    compileTypeDecls();
    compileBlock3();
  } 
  else compileBlock3();
}

//06 07
void compileBlock3(void) {
  if (lookAhead->tokenType == KW_VAR) {
    eat(KW_VAR);
    compileVarDecl();
    compileVarDecls();
    compileBlock4();
  } 
  else compileBlock4();
}

//08
void compileBlock4(void) {
  compileSubDecls();
  compileBlock5();
}

//09
void compileBlock5(void) {
  eat(KW_BEGIN);
  compileStatements();
  eat(KW_END);
}

//10 11
void compileConstDecls(void) {
if (lookAhead->tokenType == TK_IDENT){ 
  compileConstDecl();
  compileConstDecls();
}
}
 
//12
void compileConstDecl(void) {
  eat(TK_IDENT);
  eat(SB_EQ);
  compileConstant();
  eat(SB_SEMICOLON);
}

//13 14
void compileTypeDecls(void) {
  // TODO
  if (lookAhead->tokenType == TK_IDENT){
    compileTypeDecl();
    compileTypeDecls();
  }
}

//15
void compileTypeDecl(void) {
  // TODO
  eat(TK_IDENT);
  eat(SB_EQ);
  compileType();
  eat(SB_SEMICOLON);
}

//16 17
void compileVarDecls(void) {
  // TODO
  if (lookAhead->tokenType == TK_IDENT){
    compileVarDecl();
    compileVarDecls();
  }
}

//18
void compileVarDecl(void) {
  // TODO
  eat(TK_IDENT);
  eat(SB_COLON);
  compileType();
  eat(SB_SEMICOLON);
}

//19 20 21
void compileSubDecls(void) {
  assert("Parsing subtoutines ....");
  // TODO
  switch (lookAhead->tokenType){
    case KW_FUNCTION:
      compileFuncDecl();
      compileSubDecls();
      break;
    case KW_PROCEDURE:
      compileProcDecl();
      compileSubDecls();
      break;
    default:
      break;
    }
  assert("Subtoutines parsed ....");
}

//22 
void compileFuncDecl(void) {
  assert("Parsing a function ....");
  // TODO
  eat(KW_FUNCTION);
  eat(TK_IDENT);
  compileParams();
  eat(SB_COLON);
  compileBasicType();
  eat(SB_SEMICOLON);
  compileBlock();
  eat(SB_SEMICOLON);
  assert("Function parsed ....");
}

//23
void compileProcDecl(void) {
  assert("Parsing a procedure ....");
  // TODO
  eat(KW_PROCEDURE);
  eat(TK_IDENT);
  compileParams();
  eat(SB_SEMICOLON);
  compileBlock();
  eat(SB_SEMICOLON);
  assert("Procedure parsed ....");
}

//36 37 38
void compileUnsignedConstant(void) {
  // TODO
  switch(lookAhead->tokenType){
    case TK_NUMBER:
      eat(TK_NUMBER);
      break;
    case TK_IDENT:
      eat(TK_IDENT);
      break;
    case TK_CHAR:
      eat(TK_CHAR);
      break;
    default:
      error(ERR_INVALIDCONSTANT, lookAhead->lineNo, lookAhead->colNo);
      break;
    }
}

//40 41 42 43
void compileConstant(void) {
  // TODO
  switch(lookAhead->tokenType){
    case SB_PLUS:
      eat(SB_PLUS);
      compileConstant2();
      break;
    case SB_MINUS:
      eat(SB_MINUS);
      compileConstant2();
      break;
    case TK_CHAR:
      eat(TK_CHAR);
      break;
    default:
      compileConstant2();
      break;
    }
}

//44 45
void compileConstant2(void) {
  // TODO
  switch (lookAhead->tokenType){
    case TK_IDENT:
      eat(TK_IDENT);
      break;
    case TK_NUMBER:
      eat(TK_NUMBER);
      break;
    default:
      error(ERR_INVALIDCONSTANT, lookAhead->lineNo, lookAhead->colNo);
      break;
    }
}

//30 31 32 33
void compileType(void) {
  // TODO
  switch(lookAhead->tokenType){
    case KW_INTEGER:
      eat(KW_INTEGER);
      break;
    case KW_CHAR:
      eat(KW_CHAR);
      break;
    case TK_IDENT:
      eat(TK_IDENT);
      break;
    case KW_ARRAY:
      eat(KW_ARRAY);
      eat(SB_LSEL);
      eat(TK_NUMBER);
      eat(SB_RSEL);
      eat(KW_OF);
      compileType();
      break;
    default:
      error(ERR_INVALIDTYPE, lookAhead->lineNo, lookAhead->colNo);
      break;
    }
}

//34 35
void compileBasicType(void) {
  // TODO
  switch(lookAhead->tokenType){
    case KW_INTEGER:
      eat(KW_INTEGER);
      break;
    case KW_CHAR:
      eat(KW_CHAR);
      break;
    default:
      error(ERR_INVALIDBASICTYPE, lookAhead->lineNo, lookAhead->colNo);
      break;
    }
}

//24 25
//FOLLOW(Params) = {SB_COLON, SB_SEMICOLON}
void compileParams(void) {
  // TODO
  switch(lookAhead->tokenType){
    case SB_LPAR:
      eat(SB_LPAR);
      compileParam();
      compileParams2();
      eat(SB_RPAR);
      break;
    // check the FOLLOW set
    case SB_COLON:
    case SB_SEMICOLON:
      break;
    default:
      error(ERR_INVALIDPARAM, lookAhead->lineNo, lookAhead->colNo);
      break;
    }
}

//26 27
void compileParams2(void) {
  // TODO
  switch (lookAhead->tokenType) {
  case SB_SEMICOLON:
      eat(SB_SEMICOLON);
      compileParam();
      compileParams2();
      break;
  case SB_RPAR:
      break;
  default:
      error(ERR_INVALIDPARAM, lookAhead->lineNo, lookAhead->colNo);
      break;
  }
}

//28 29
void compileParam(void) {
  // TODO
  switch(lookAhead->tokenType){
    case TK_IDENT:
      eat(TK_IDENT);
      eat(SB_COLON);
      compileBasicType();
      break;
    case KW_VAR:
      eat(KW_VAR);
      eat(TK_IDENT);
      eat(SB_COLON);
      compileBasicType();
      break;
    default:
      error(ERR_INVALIDPARAM, lookAhead->lineNo, lookAhead->colNo);
      break;
    }
}

//46
void compileStatements(void) {
  // TODO
  compileStatement();
  compileStatements2();
}

//47 48
//FOLLOW(Statements2) = KW_END
void compileStatements2(void) {
  // TODO
  switch (lookAhead->tokenType) {
  case SB_SEMICOLON:
      eat(SB_SEMICOLON);
      compileStatement();
      compileStatements2();
      break;
  // Check FOLLOW set
  case KW_END:
      break;
  // error
  default:
      error(ERR_INVALIDSTATEMENT, lookAhead->lineNo, lookAhead->colNo);
      break;
  }
}

//49 50 51 52 53 54 55
void compileStatement(void) {
  switch (lookAhead->tokenType) {
  case TK_IDENT:
    compileAssignSt();
    break;
  case KW_CALL:
    compileCallSt();
    break;
  case KW_BEGIN:
    compileGroupSt();
    break;
  case KW_IF:
    compileIfSt();
    break;
  case KW_WHILE:
    compileWhileSt();
    break;
  case KW_FOR:
    compileForSt();
    break;
    // EmptySt needs to check FOLLOW tokens
  case SB_SEMICOLON:
  case KW_END:
  case KW_ELSE:
    break;
    // Error occurs
  default:
    error(ERR_INVALIDSTATEMENT, lookAhead->lineNo, lookAhead->colNo);
    break;
  }
}

//56
void compileAssignSt(void) {
  assert("Parsing an assign statement ....");
  // TODO
  eat(TK_IDENT);
  if (lookAhead->tokenType == SB_LSEL) {
      compileIndexes();
  }
  eat(SB_ASSIGN);
  compileExpression();
  assert("Assign statement parsed ....");
}
//57
void compileCallSt(void) {
  assert("Parsing a call statement ....");
  // TODO
  eat(KW_CALL);
  if (lookAhead->tokenType == KW_CALL)
    error(ERR_INVALIDSTATEMENT, lookAhead->lineNo, lookAhead->colNo);
  eat(TK_IDENT);
  if (lookAhead->tokenType == SB_SEMICOLON)
    eat(SB_SEMICOLON);
  compileArguments();
  assert("Call statement parsed ....");
}

//58
void compileGroupSt(void) {
  assert("Parsing a group statement ....");
  // TODO
  eat(KW_BEGIN);
  compileStatements();
  eat(KW_END);
  assert("Group statement parsed ....");
}

//59
void compileIfSt(void) {
  assert("Parsing an if statement ....");
  eat(KW_IF);
  compileCondition();
  eat(KW_THEN);
  compileStatement();
  if (lookAhead->tokenType == KW_ELSE) 
    compileElseSt();
  assert("If statement parsed ....");
}

//60 61
void compileElseSt(void) {
  eat(KW_ELSE);
  compileStatement();
}

//62
void compileWhileSt(void) {
  assert("Parsing a while statement ....");
  // TODO
  eat(KW_WHILE);
  compileCondition();
  eat(KW_DO);
  compileStatement();
  assert("While statement pased ....");
}

//63
void compileForSt(void) {
  assert("Parsing a for statement ....");
  // TODO
  eat(KW_FOR);
  eat(TK_IDENT);
  eat(SB_ASSIGN);
  compileExpression();
  eat(KW_TO);
  compileExpression();
  eat(KW_DO);
  compileStatement();
  assert("For statement parsed ....");
}

//64 65
/* FOLLOW(Term2) = FOLLOW(Term) = (First(Expression3) - epsilon) U Follow(Expression3) = 
{SB_COMMA,SB_SEMICOLON, KW_END, KW_TO, KW_THEN, KW_DO, SB_RPAR, SB_RSEL, SB_EQ, SB_NEQ, SB_LT, 
SB_LE, SB_GT, SB_GE, KW_ELSE, SB_PLUS, SB_MINUS}

FOLLOW(FunctionAppilcation) = FOLLOW(Factor) = (FIRST(Term2) - epsilon) U FOLLOW(Term2) =
{SB_COMMA,SB_SEMICOLON, KW_END, KW_TO, KW_THEN, KW_DO, SB_RPAR, SB_RSEL, SB_EQ, SB_NEQ, SB_LT, 
SB_LE, SB_GT, SB_GE, KW_ELSE, SB_PLUS, SB_MINUS, SB_TIMES, SB_SLASH}

FOLLOW(Arguments) = FOLLOW(CallSt) U FOLLOW(FunctionApplication) = 
{SB_COMMA,SB_SEMICOLON, KW_END, KW_TO, KW_THEN, KW_DO, SB_RPAR, SB_RSEL, SB_EQ, SB_NEQ, SB_LT,
 SB_LE, SB_GT, SB_GE, KW_ELSE, SB_PLUS, SB_MINUS, SB_TIMES, SB_SLASH}

*/

// Arguments ::= SB_LPAR Arguments1 SB_RPAR
// Arguments1 ::= Expression Arguments2
// Arguments1 ::= eps
// Arguments2::= SB_COMMA Expression Arguments2
// Arguments2::= eps
void compileArguments1(void) {
  switch(lookAhead->tokenType){
    case TK_NUMBER:
      compileExpression();
      compileArguments2();
      break;
    case TK_CHAR:
      compileExpression();
      compileArguments2();
      break;
    case TK_IDENT:
      compileExpression();
      compileArguments2();
      break;
    case SB_PLUS:
      compileExpression();
      compileArguments2();
      break;
    case SB_MINUS:
      compileExpression();
      compileArguments2();
      break;
    default:
      break;
    }
}

void compileArguments(void) {
  // TODO
  switch (lookAhead->tokenType) {
  case SB_LPAR:
    eat(SB_LPAR);
    compileArguments1();
    eat(SB_RPAR);
    break;
  // check the FOLLOW set
  case SB_COMMA:
  case SB_SEMICOLON:
  case KW_END:
  case KW_TO:
  case KW_THEN:
  case KW_DO:
  case SB_RPAR:
  case SB_RSEL:
  case SB_EQ:
  case SB_NEQ:
  case SB_LT:
  case SB_LE:
  case SB_GT:
  case SB_GE:
  case KW_ELSE:
  case SB_PLUS:
  case SB_MINUS:
  case SB_TIMES:
  case SB_SLASH:
    break;
  default:
    error(ERR_INVALIDARGUMENTS, lookAhead->lineNo, lookAhead->colNo);
    break;
  }
}

//66
//FOLLOW(Arguments2) = SB_RPAR
void compileArguments2(void) {
  // TODO
  switch(lookAhead->tokenType){
    case SB_COMMA:
      eat(SB_COMMA);
      compileExpression();
      compileArguments2();
      break;
    // check FOLLOW set
    case SB_RPAR:
      break;
    //error
    default:
      error(ERR_INVALIDARGUMENTS, lookAhead->lineNo, lookAhead->colNo);
      break;
    }
}

//68
void compileCondition(void) {
  // TODO
  compileExpression();
  compileCondition2();
}

//69 70 71 72 73 74
//FOLLOW(Condition2) = FOLLOW(Condtion) = {KW_THEN, KW_DO}
void compileCondition2(void) {
  // TODO
  switch(lookAhead->tokenType){
    case SB_EQ:
      eat(SB_EQ);
      compileExpression();
      break;
    case SB_NEQ:
      eat(SB_NEQ);
      compileExpression();
      break;
    case SB_LE:
      eat(SB_LE);
      compileExpression();
      break;
    case SB_LT:
      eat(SB_LT);
      compileExpression();
      break;
    case SB_GE:
      eat(SB_GE);
      compileExpression();
      break;
    case SB_GT:
      eat(SB_GT);
      compileExpression();
      break;
    // check FOLLOW set
    case KW_THEN:
    case KW_DO:
      break;
    default:
      error(ERR_INVALIDCOMPARATOR, lookAhead->lineNo, lookAhead->colNo);
      break;
    }
}

//75 76 77
void compileExpression(void) {
  assert("Parsing an expression");
  // TODO
  switch(lookAhead->tokenType){
    case SB_PLUS:
      eat(SB_PLUS);
      compileExpression2();
      break;
    case SB_MINUS:
      eat(SB_MINUS);
      compileExpression2();
      break;
    default:
      compileExpression2();
      break;
    }
  assert("Expression parsed");
}

//78
void compileExpression2(void) {
  // TODO
  compileTerm();
  compileExpression3();
}

//79 80 81
void compileExpression3(void) {
  // TODO
  switch(lookAhead->tokenType){
    case SB_PLUS:
      eat(SB_PLUS);
      compileTerm();
      compileExpression3();
      break;
    case SB_MINUS:
      eat(SB_MINUS);
      compileTerm();
      compileExpression3();
      break;
    // Check FOLLOW set
    case SB_COMMA:
    case SB_SEMICOLON:
    case KW_END:
    case KW_TO:
    case KW_THEN:
    case KW_DO:
    case SB_RPAR:
    case SB_RSEL:
    case SB_EQ:
    case SB_NEQ:
    case SB_LT:
    case SB_LE:
    case SB_GT:
    case SB_GE:
    case KW_ELSE:
      break;
    // error
    default:
      error(ERR_INVALIDEXPRESSION, lookAhead->lineNo, lookAhead->colNo);
      break;
      }
}

//82
void compileTerm(void) {
  // TODO
  compileFactor();
  compileTerm2();
}

//83 84 85
/* FOLLOW(Term2) = FOLLOW(Term) = (First(Expression3) - epsilon) U Follow(Expression3) = 
{SB_COMMA,SB_SEMICOLON, KW_END, KW_TO, KW_THEN, KW_DO, SB_RPAR, SB_RSEL, SB_EQ, SB_NEQ, SB_LT, 
SB_LE, SB_GT, SB_GE, KW_ELSE, SB_PLUS, SB_MINUS} */
void compileTerm2(void) {
  // TODO
  switch (lookAhead->tokenType) {
  case SB_TIMES:
      eat(SB_TIMES);
      compileFactor();
      compileTerm2();
      break;
  case SB_SLASH:
      eat(SB_SLASH);
      compileFactor();
      compileTerm2();
      break;
    //follow set
  case SB_COMMA:
  case SB_SEMICOLON:
  case KW_END:
  case KW_TO:
  case KW_THEN:
  case KW_DO:
  case SB_RPAR:
  case SB_RSEL:
  case SB_EQ:
  case SB_NEQ:
  case SB_LT:
  case SB_LE:
  case SB_GT:
  case SB_GE:
  case KW_ELSE:
  case SB_PLUS:
  case SB_MINUS:
    break;
  // error
  default:
    error(ERR_INVALIDTERM, lookAhead->lineNo, lookAhead->colNo);
    break;
    }
}

//86 87 88 89 90
void compileFactor(void) {
  // TODO
  switch(lookAhead->tokenType){
    case TK_NUMBER:
      eat(TK_NUMBER);
      break;
    case TK_CHAR:
      eat(TK_CHAR);
      break;
    case TK_IDENT:
      eat(TK_IDENT);
      switch(lookAhead->tokenType){
        case SB_LSEL:
          compileIndexes();
          break;
        case SB_LPAR:
          compileArguments();
          break;
        default:
          break;
        }
      break;
    case SB_LPAR:
      eat(SB_LPAR);
      compileExpression();
      eat(SB_RPAR);
      break;
    default:
      error(ERR_INVALIDFACTOR, lookAhead->lineNo, lookAhead->colNo);
      break;
    }
}

//91

//93 94
void compileIndexes(void) {
  // TODO
  if (lookAhead->tokenType == SB_LSEL){
    eat(SB_LSEL);
    compileExpression();
    eat(SB_RSEL);
    compileIndexes();
  }
}

int compile(char *fileName) {
  if (openInputStream(fileName) == IO_ERROR)
    return IO_ERROR;

  currentToken = NULL;
  lookAhead = getValidToken();

  compileProgram();

  free(currentToken);
  free(lookAhead);
  closeInputStream();
  return IO_SUCCESS;

}