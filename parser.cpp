/**********************************************
Name: Dylan Tribble             NetID: dat307
Course: CSE 4714              
Programming Environment: VS Code on Fedora
Purpose of File: Contains code for parsing tips
                syntax and semantics.
**********************************************/

/* POSSIBLE ERROR CODES //
"2: identifier expected"
"3: 'PROGRAM' expected"
"4: ')' expected"
"5: ':' expected"
"9: '(' expected"
"10: error in type"     
            variable declaration other than INTEGER or REAL
"13: 'END' expected"
"14: ';' expected"
"17: 'BEGIN' expected"
"18: error in declaration part OR 17: 'BEGIN' expected"
            <block> does not start with VAR or BEGIN
"51: ':=' expected"
"52: 'THEN' expected"
"101: identifier declared twice"
"104: identifier not declared"
"134: illegal type of operand(s)"
            WRITE does not contain an identifier or string literal
"144: illegal type of expression"
            illegal first of expression

"900: illegal type of statement"
            illegal first of statement
"901: illegal type of simple expression"
            illegal first of simple expression
"902: illegal type of term"
            illegal first of term
"903: illegal type of factor"
            illegal first of factor
"999: an error has occurred"
            any other error
// POSSIBLE ERROR CODES */

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdlib.h>
#include <iostream>
#include "parser.h"
#include "parse_nodes.h"
 
using namespace std;

extern "C"
{
  extern int   yylex();      // the generated lexical analyzer
  extern char *yytext;       // text of current lexeme
}

int nextToken = 0;            // hold nextToken returned by lex
bool printParse = false;           // shall we print while parsing?

// Which tree level are we currently in?  
static int level = 0;
static int prevToken = 0;
bool isInWhile = false;
bool isInIf = false;
string exprId = "NULL";


// Feel free to use a different data structure for the symbol table (list of
// variables declared in the program) but you will have to adjust the code in
// main() to print out the symbol table after a successful parse
set<string> symbolTable; // Symbol Table
symbolTableT symbolTableN; // Symbol Table with types
symbolTableT2 symbolTableN2; // Symbol Table with types and values

//*****************************************************************************
// Indent to reveal tree structure
string psp(void) { // Stands for p-space, but I want the name short
  string str("");
  for(int i = 0; i < level; i++)
    str += "|  ";
  return str;
}
//*****************************************************************************
// Convert token to string
int lex() 
{
  char* token_str;  // point to the logical name of the token

  nextToken = yylex();
  if( nextToken == TOK_EOF ) {
    yytext[0] = 'E'; yytext[1] = 'O'; yytext[2] = 'F'; yytext[3] = 0;   
  }

  // Determine the logical name of the lexeme
  switch( nextToken )
  {
    //keywords
    case TOK_PROGRAM:     token_str = (char*)"TOK_PROGRAM"; break;
    case TOK_BEGIN:     token_str = (char*)"TOK_BEGIN"; break;
    case TOK_BREAK:     token_str = (char*)"TOK_BREAK"; break;
    case TOK_CONTINUE:      token_str = (char*)"TOK_CONTINUE"; break;
    case TOK_DOWNTO:      token_str = (char*)"TOK_DOWNTO"; break;
    case TOK_END:      token_str = (char*)"TOK_END"; break;
    case TOK_FOR:      token_str = (char*)"TOK_FOR"; break;
    case TOK_IF:      token_str = (char*)"TOK_IF"; break;
    case TOK_ELSE:      token_str = (char*)"TOK_ELSE"; break;
    case TOK_LET:      token_str = (char*)"TOK_LET"; break;
    case TOK_READ:      token_str = (char*)"TOK_READ"; break;
    case TOK_THEN:      token_str = (char*)"TOK_THEN"; break;
    case TOK_TO:      token_str = (char*)"TOK_TO"; break;
    case TOK_VAR:      token_str = (char*)"TOK_VAR"; break;
    case TOK_WHILE:      token_str = (char*)"TOK_WHILE"; break;
    case TOK_WRITE:     token_str = (char*)"TOK_WRITE"; break;

    //data types
    case TOK_INTEGER:     token_str = (char*)"TOK_INTEGER"; break;
    case TOK_REAL:     token_str = (char*)"TOK_REAL"; break;

    //punctuation
    case TOK_SEMICOLON:      token_str = (char*)"TOK_SEMICOLON"; break;
    case TOK_COLON:     token_str = (char*)"TOK_COLON"; break;
    case TOK_OPENPAREN:      token_str = (char*)"TOK_OPENPAREN"; break;
    case TOK_CLOSEPAREN:      token_str = (char*)"TOK_CLOSEPAREN"; break;

    //operators
    case TOK_PLUS:      token_str = (char*)"TOK_PLUS"; break;
    case TOK_MINUS:      token_str = (char*)"TOK_MINUS"; break;
    case TOK_MULTIPLY:      token_str = (char*)"TOK_MULTIPLY"; break;
    case TOK_DIVIDE:      token_str = (char*)"TOK_DIVIDE"; break;
    case TOK_ASSIGN:      token_str = (char*)"TOK_ASSIGN"; break;
    case TOK_EQUALTO:      token_str = (char*)"TOK_EQUALTO"; break;
    case TOK_LESSTHAN:      token_str = (char*)"TOK_LESSTHAN"; break;
    case TOK_GREATERTHAN:      token_str = (char*)"TOK_GREATERTHAN"; break;
    case TOK_NOTEQUALTO:      token_str = (char*)"TOK_NOTEQUALTO"; break;
    case TOK_MOD:     token_str = (char*)"TOK_MOD"; break;
    case TOK_NOT:      token_str = (char*)"TOK_NOT"; break;
    case TOK_OR:      token_str = (char*)"TOK_OR"; break;
    case TOK_AND:      token_str = (char*)"TOK_AND"; break;

    //abstracts
    case TOK_IDENT:     token_str = (char*)"TOK_IDENT"; break;
    case TOK_INTLIT:      token_str = (char*)"TOK_INTLIT"; break;
    case TOK_FLOATLIT:      token_str = (char*)"TOK_FLOATLIT"; break;
    case TOK_STRINGLIT:      token_str = (char*)"TOK_STRINGLIT"; break;
    
    //other-tokens
    case TOK_UNKNOWN:      token_str = (char*)"TOK_UNKNOWN"; break;
    case TOK_EOF:      token_str = (char*)"TOK_EOF"; break;

    default:      token_str = (char*)"=== unmapped token name ===";
  }

  //cout << psp() << "next lexeme: |" << yytext << "|, token: " << token_str << endl;
  return nextToken;
}
//*****************************************************************************
// Report what we found
void output(string what) {
  cout << psp() << "found |" << yytext << "| " << what << endl;
}
//*****************************************************************************
// Forward declarations of FIRST_OF functions.  These check whether the current 
// token is in the FIRST set of a production rule.


//*****************************************************************************
// Parses strings in the language generated by the rule:
// <program> → TOK_PROGRAM TOK_IDENT TOK_SEMICOLON <block>
ProgNode* program() 
{
  //<program> -> TOK_PROGRAM TOK_IDENT ; <block>
  if (!first_of_program()) // Check for PROGRAM
        throw "3: 'PROGRAM' expected";
  if(printParse){output(checkToken());
  cout << psp() << "enter <program>" << endl;}
  else { checkToken(); }
  ++level;
  for (int i = 0; i < 2; i++)
  {
    lex();
    if(i==1 && nextToken!=TOK_SEMICOLON) throw "14: ';' expected";
    if(printParse) output(checkToken());
    else checkToken();
  }
  lex();
  ProgNode* newProg = new ProgNode(level, block());

  --level;
  if(printParse) cout << psp() << "exit <program>" << endl;
  return newProg;
}
bool first_of_program(void) {
    return nextToken == TOK_PROGRAM;
}
bool first_of_block(){
  //also follows for compound stmt, must start with BEGIN
  return nextToken == TOK_VAR || nextToken == TOK_BEGIN;
}
bool first_of_stmt(){
  /*
  <compound> = TOK_BEGIN <stmt>; TOK_END
  <assignment> = TOK_IDENT := <expr>
  <if> = TOK_IF <expr> TOK_THEN <stmt> [TOK_ELSE <stmt>]
  <while> = TOK_WHILE <expr> <stmt>
  <read> = TOK_READ ( TOK_IDENT )
  <write> = TOK_WRITE ( [TOK_IDENT | TOK_STRINGLIT] )
  */
  return (nextToken == TOK_IF || nextToken == TOK_WHILE || nextToken == TOK_IDENT 
          || nextToken == TOK_BEGIN || nextToken == TOK_READ || nextToken == TOK_WRITE
          );
}
bool first_of_expression(){
  return first_of_smpl_expr();
}
bool first_of_smpl_expr(){
  return first_of_term();
}
bool first_of_term(){
  return isFactor();
}
bool isFactor(){
  return (nextToken==TOK_INTLIT || nextToken==TOK_FLOATLIT || nextToken==TOK_IDENT
          || nextToken==TOK_OPENPAREN || nextToken==TOK_NOT || nextToken == TOK_MINUS
          );
}

ExprNode* expression(){
  //<expr> -> <simpleExpr> {(=|<|>|<>) <simpleExpr>}
  if(!first_of_expression()) throw ("144: illegal type of expression");
  if(printParse) {
    output("EXPRESSION");
    cout << psp() << "enter <expression>" << endl;
  }
  ++level;
  ExprNode* newExpr = new ExprNode(level-1, simpleExpression());
  while(nextToken==TOK_EQUALTO||nextToken==TOK_LESSTHAN||nextToken==TOK_GREATERTHAN||nextToken==TOK_NOTEQUALTO){
    if(printParse) {
      output(checkToken());
      cout << psp() << yytext << endl;
    } else {
      checkToken();
    }
    newExpr->ops.push_back(yytext);
    newExpr->opToks.push_back(nextToken);
    lex();
    newExpr->simples.push_back(simpleExpression());
  }
  --level;
  if(printParse) {
    cout << psp() << "exit <expression>" <<endl;
    if(nextToken==TOK_CLOSEPAREN) { 
      output(checkToken());lex();
    }
  } else {
    if(nextToken==TOK_CLOSEPAREN) { checkToken(); lex(); }
  }
  return newExpr;
}
SimpleNode* simpleExpression(){
  //<simpleExpr> -> <term> {(*|/|AND) <term>}
  if(!first_of_smpl_expr()) throw "901: illegal type of simple expression";
  if(printParse) {  
    output("SIMPLE_EXP");
    cout << psp() << "enter <simple_exp>" << endl;
  }
  ++level;
  SimpleNode* newSimple = new SimpleNode(level-1, term());
  while(nextToken==TOK_PLUS||nextToken==TOK_MINUS||nextToken==TOK_OR) {
    if(printParse) {  
      output(checkToken());
      cout << psp() << yytext << endl;
    } else {
      checkToken();
      }
    newSimple->ops.push_back(yytext);
    newSimple->opToks.push_back(nextToken);
    lex();
    newSimple->terms.push_back(term());
  }
  --level;
  if(printParse) cout << psp() << "exit <simple_exp>" <<endl;
  return newSimple;
}
TermNode* term(){
  //<term> -> <factor> {(*|/|AND) <factor>}
  if(!first_of_term()) throw "902: illegal type of term";
  if(printParse) {
    output("TERM");
    cout << psp() << "enter <term>" << endl;
  }
  ++level;
  TermNode* newTerm = new TermNode(level-1, factor());
  while(nextToken==TOK_MULTIPLY||nextToken==TOK_DIVIDE||nextToken==TOK_AND) {
    if(printParse) {
      output(checkToken());
      cout << psp() << yytext << endl;
    } else {
      checkToken();
    }
    newTerm->ops.push_back(yytext);
    newTerm->opToks.push_back(nextToken);
    lex();
    newTerm->factors.push_back(factor());
  }
  --level;
  if(printParse) cout << psp() << "exit <term>" <<endl;
  return newTerm;
}
FactorNode* factor(){
  //<factor> -> <int_lit> | <float_lit> | <identifier> 
  //  | ( <expression> ) | NOT <factor> | - <factor>
  int prevToken;
  if(!isFactor()) throw "903: illegal type of factor";
  if(printParse) {output("FACTOR");
  cout << psp() << "enter <factor>" <<endl;}

  ++level;
  FactorNode* newFactor = nullptr;
  prevToken=nextToken;
  switch (nextToken)
  {
  case TOK_OPENPAREN:
    if(printParse){output(checkToken());
    cout << psp() << yytext << endl;}
    else { checkToken(); }
    if(nextToken==TOK_IDENT && symbolTable.count(yytext)==0) {throw "104: identifier not declared"; break;}
    lex();
    newFactor = new NestedNode(level-1, expression());
    if(nextToken==TOK_CLOSEPAREN) { 
      if(printParse){output(checkToken());}
      else { checkToken(); }
      lex();
    } //else throw "X: <expr> does not end with )";
    break;
  case TOK_NOT:
    if(printParse){output(checkToken());
    cout << psp() << yytext << endl;}
    else { checkToken(); }
    lex();
    newFactor = new NotNode(level-1, factor());
    break;
  case TOK_MINUS:
    if(printParse){output(checkToken());
    cout << psp() << yytext << endl;}
    else { checkToken(); }
    lex();
    newFactor = new NegNode(level-1, factor());
    break;
  case TOK_INTLIT:
    if(printParse){output(checkToken());
    cout << psp() << yytext << endl;}
    else { checkToken(); }
    newFactor = new IntNode(level-1, atoi(yytext));
    lex();
    break;
  case TOK_FLOATLIT:
    if(printParse){output(checkToken());
    cout << psp() << yytext << endl;}
    else { checkToken(); }
    newFactor = new FloatNode(level-1, atof(yytext));
    lex();
    break;
  case TOK_IDENT:
    if(printParse){output(checkToken());
    cout << psp() << yytext << endl;}
    else { checkToken(); }
    newFactor = new IdNode(level-1, yytext);
    if(nextToken==TOK_IDENT && symbolTable.count(yytext)==0) throw "104: identifier not declared";
    lex();
    break;
  default: //INTLIT, FLOATLIT, IDENT
    throw "X: Generic error in <factor>";
    break;
  }
  --level;
  if(printParse) cout << psp() << "exit <factor>" <<endl;
  return newFactor;
}

BlockNode* block(){
  //<block> = [TOK_VAR (TOK_IDENT : (REAL|INTEGER);)+] <compound>
  if(!first_of_block) throw "18: error in declaration part OR 17: 'BEGIN' expected";
  if(printParse){output("BLOCK");
  cout << psp() << "enter <block>" << endl;}
  else { checkToken(); }
  ++level;
  if(nextToken==TOK_VAR){
    writeToSymbolTable();
  }
  BlockNode* newBlock = new BlockNode(level, compd_stmt());
  --level;
  if(printParse) cout << psp() << "exit <block>" << endl;
  return newBlock;
}
CompNode* compd_stmt(){
  //<compound> = TOK_BEGIN (<stmt>[;])+ TOK_END
  if(!first_of_stmt()) throw "17: 'BEGIN' expected";
  if(printParse){output(checkToken());
  cout << psp() << "enter <compound_stmt>" << endl;}
  else {checkToken();}
  lex();
  CompNode* newComp = new CompNode(level, stmt());
  //if(nextToken==TOK_SEMICOLON) output(checkToken());
  while(nextToken==TOK_SEMICOLON) {
    ++level;
    if(printParse) output(checkToken());
    else checkToken();
    --level;
    lex();
    newComp->stmts.push_back(stmt());

    //if(nextToken==TOK_SEMICOLON) output(checkToken());
    }
  //"13: 'END' expected" incorporate this exception
  if(printParse) output(checkToken());
  else checkToken();
  prevToken=nextToken;
  lex();
  if(printParse) cout << psp() << "exit <compound_stmt>" << endl;
  return newComp;
}
StmtNode* stmt(){
  //<stmt> -> (compound|if|while|read|write|assignment)
  ++level;
  if(printParse) output("STATEMENT");
  StmtNode* newStmt = nullptr;
  switch(nextToken){
    case TOK_BEGIN:
      newStmt = compd_stmt();
      //if(nextToken==TOK_SEMICOLON) output(checkToken())
      break;
    case TOK_READ:
      newStmt = read();
      //if(nextToken==TOK_SEMICOLON) output(checkToken());
      break;
    case TOK_WRITE:
      newStmt = write();
      //if(nextToken==TOK_SEMICOLON) output(checkToken());
      break;
    case TOK_IF:
      newStmt = ifStmt();
      //if(nextToken==TOK_SEMICOLON) output(checkToken());
      break;
    case TOK_WHILE:
      newStmt = whileStmt();
      //if(nextToken==TOK_SEMICOLON) output(checkToken());
      break;
    case TOK_IDENT:
      newStmt = assignment();
      //if(nextToken==TOK_SEMICOLON) output(checkToken());
      break;
    default:
      throw "X: Generic error in <stmt>";
      break;
  }
  --level;
  return newStmt;
}
ReadNode* read(){
  //<read> = TOK_READ ( TOK_IDENT )
  if(!first_of_stmt()) throw "900: illegal type of statement";
  if(printParse) cout << psp() << "enter <read>" << endl;
  ++level;
  lex(); //goes to next token after READ
  
  if(nextToken!=TOK_OPENPAREN) throw "9: '(' expected";
  if(printParse) output(checkToken());
  else checkToken();
  lex(); //goes to next token after (

  if(nextToken!=TOK_IDENT) "2: identifier expected";
  //if(!idExists()) throw "X: IDENTIFIER NOT DECLARED";
  if(printParse){output(checkToken());
  cout << psp() << yytext << endl;}
  else { checkToken(); }
  ReadNode* newRead = new ReadNode(level, yytext);
  newRead->type = "READ"; //TODO: add correct Type
  newRead->id = yytext;
  lex(); //goes to token before )

  if(nextToken!=TOK_CLOSEPAREN) throw "4: ')' expected";
  if(printParse) output(checkToken());
  else checkToken();
  lex(); //goes to token after )

  --level;
  if(printParse) cout << psp() << "exit <read>" << endl;
  return newRead;
}
WriteNode* write(){
  //<write> = TOK_WRITE ( (TOK_IDENT | TOK_STRINGLIT) )
  if(!first_of_stmt()) throw "900: illegal type of statement";
  if(printParse) cout << psp() << "enter <write>" << endl;
  ++level;
  lex(); //goes to next token after WRITE
  if(nextToken!=TOK_OPENPAREN) throw "9: '(' expected";
  if(printParse) output(checkToken());
  else checkToken();
  lex(); //goes to next token after (

  if(nextToken!=TOK_IDENT && nextToken!=TOK_STRINGLIT) throw "134: illegal type of operand(s)";
  if(printParse) output("WRITE");
  // if(nextToken==TOK_IDENT) {
  //   if(!idExists()) throw "X: IDENTIFIER NOT DECLARED";
  // }
  if(printParse) cout << psp() << yytext <<endl;
  WriteNode* newWrite = new WriteNode(level, yytext);
  //newWrite->id = yytext;
  lex(); //goes to next token after string,etc.

  if(nextToken!=TOK_CLOSEPAREN) throw "4: ')' expected";
  if(printParse) output(checkToken());
  else checkToken();
  lex(); //goes to next token after )
  --level;
  if(printParse) cout << psp() << "exit <write>" << endl;
  return newWrite;
}
AssignNode* assignment(){
  //<assignment> = TOK_IDENT := <expr>
  if(!first_of_stmt()) throw "900: illegal type of statement";
  if(printParse) cout << psp() << "enter <assignment>" <<endl;
  ++level;
  AssignNode* newAssign = new AssignNode(level);
  if(nextToken!=TOK_IDENT) throw "2: identifier expected";
  if(printParse){output(checkToken());
  cout << psp() << yytext << endl;}
  else { checkToken(); }
  newAssign->id = yytext;
  //if(!idExists()) throw "X: IDENTIFIER NOT DECLARED";
  lex();

  if(nextToken!=TOK_ASSIGN) throw "51: ':=' expected";
  if(printParse) output(checkToken());
  else checkToken();
  lex();
  newAssign->expr = expression();
  --level;
  if(printParse) cout << psp() << "exit <assignment>" << endl;
  return newAssign;
}
WhileNode* whileStmt(){
  //<while> = TOK_WHILE <expr> <stmt>
  if(!first_of_stmt()) throw "900: illegal type of statement";
  isInWhile = true;
  if(printParse) cout << psp() << "enter <while>" << endl;
  WhileNode* newWhile = new WhileNode(level);
  ++level;
  lex();
  newWhile->cond = expression();
  --level;
  newWhile->stmt = stmt();
  ++level;
  --level;
  if(printParse) cout << psp() << "exit <while>" << endl;
  isInWhile = false;
  return newWhile;
}
IfNode* ifStmt(){
  //<if> = TOK_IF <expr> TOK_THEN <stmt> [TOK_ELSE <stmt>]
  if(!first_of_stmt()) throw "900: illegal type of statement";
  isInIf = true;
  if(printParse) cout << psp() << "enter <if>" << endl;
  IfNode* newIf = new IfNode(level);
  ++level;
  lex();
  newIf->cond = expression();
  --level;
  if(nextToken != TOK_THEN) throw "X: FILL THIS IN";
  if(printParse) {output(checkToken());
  cout << psp() << "enter <then>" << endl;}
  else { checkToken(); }
  lex();
  newIf->then = stmt();
  if(printParse) cout << psp() << "exit <then>" <<endl;
  if(nextToken == TOK_ELSE){
    if(printParse) {output(checkToken());
    cout << psp() << "enter <else>" << endl;}
    else { checkToken(); }
    lex();
    newIf->other = stmt();
    if(printParse) cout << psp() << "exit <else>" <<endl;
  }
  if(printParse) cout << psp() << "exit <if>" << endl;
  isInIf = false;
  return newIf;
}

void writeToSymbolTable(){
  string type;
  string varName;
  bool isDuplicate;
  while(nextToken!=TOK_BEGIN){
    lex();
    switch (nextToken)
    {
    case TOK_IDENT:
      if(printParse) output(checkToken());
      else checkToken();
      varName = yytext;
      /* if(symbolTable.count(varName)==0) {
        symbolTable.insert(yytext);
        symbolTableN.insert(pair<string, string>(yytext, type));
        symbolTableN2.insert(pair<pair<string, string>, string>(pair<string, string>(varName, type), "NULL"));
        cout << "New Var" << endl;
      }
      else{
        isDuplicate=true;
      } */
      break;
    case TOK_REAL:
      if(printParse) output("TYPE");
      type = checkToken();
      break;
    case TOK_INTEGER:
      if(printParse) output("TYPE");
      type = checkToken();
      break;
    case TOK_SEMICOLON:
      if(symbolTable.count(varName)==0) {
        symbolTable.insert(varName);
        symbolTableN.insert(pair<string, float>(varName, 1.0));
        symbolTableN2.insert(pair<pair<string, string>, float>(pair<string, string>(varName, type), 1.0));
      }
      else{
        isDuplicate=true;
      }

      if(printParse) {output(checkToken());
        cout << psp() << "-- idName: |" << varName << "| idType: |" << type << "| --"<< endl;
      } else {
        checkToken();
      }
      if(isDuplicate) throw ("101: identifier declared twice");
      break;
    case TOK_BEGIN:
      break;
    case TOK_COLON:
      if(printParse) output(checkToken());
      else checkToken();
      break;
    default:
      if(type!="INTEGER" || type!="REAL") throw "10: error in type";
      else throw "X: error in writeToSymbolTable";
      break;
    }
  }
}

bool idExists(){
  // /cout << symbolTable.count(yytext);
  if(symbolTable.count(yytext)!=0) return true;
  else return false;
}

string checkToken(){
  switch (nextToken)
  {
    case TOK_BEGIN: return "BEGIN";
    case TOK_BREAK: return "BREAK";
    case TOK_CONTINUE: return "CONTINUE";
    case TOK_DOWNTO: return "DOWNTO";
    case TOK_ELSE: return "ELSE";
    case TOK_END: return "END";
    case TOK_FOR: return "FOR";
    case TOK_IF: return "IF";
    case TOK_LET: return "LET";
    case TOK_PROGRAM: return "PROGRAM";
    case TOK_READ: return "READ";
    case TOK_THEN: return "THEN";
    case TOK_TO: return "TO";
    case TOK_VAR: return "VAR";
    case TOK_WHILE: return "WHILE";
    case TOK_WRITE: return "WRITE";
    //add cases for punctuation
    case TOK_SEMICOLON: return "SEMICOLON";
    case TOK_COLON: return "COLON";
    case TOK_OPENPAREN: return "OPENPAREN";
    case TOK_CLOSEPAREN: return "CLOSEPAREN";
    //add cases for operators
    case TOK_PLUS: return "PLUS";
    case TOK_MINUS: return "MINUS";
    case TOK_MULTIPLY: return "MULTIPLY";
    case TOK_DIVIDE: return "DIVIDE";
    case TOK_MOD: return "MODULO";
    case TOK_ASSIGN: return "ASSIGN";
    case TOK_EQUALTO: return "EQUALTO";
    case TOK_NOTEQUALTO: return "NOTEQUALTO";
    case TOK_LESSTHAN: return "LESSTHAN";
    case TOK_GREATERTHAN: return "GREATERTHAN";
    //add cases for IDENTIFIERS, INTEGER LITERALS, FLOAT LITERALS, STRING LITERALS
    case TOK_IDENT: return "IDENTIFIER";
    case TOK_INTLIT: return "INTLIT";
    case TOK_FLOATLIT: return "FLOATLIT";
    case TOK_STRINGLIT: return "STRINGLIT";
    
    case TOK_NOT: return "NOT";
    case TOK_REAL: return "REAL";
    case TOK_INTEGER: return "INTEGER";
    default: return "UNKNOWN TOKEN";
  }
}
