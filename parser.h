/**********************************************
Name: Dylan Tribble             NetID: dat307
Course: CSE 4714              
Programming Environment: VS Code on Fedora
Purpose of File: Header file that instantiates the global variables 
				used by flex and declares the functions used in the parser.
**********************************************/


#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <set>
#include <map>
#include <iostream>
#include "lexer.h"
#include "parse_nodes.h"

using namespace std;

// Holds the symbols in the interpreted program
typedef map<string, float> symbolTableT;
typedef map<pair<string, string>, float> symbolTableT2;
extern symbolTableT symbolTableN;
extern symbolTableT2 symbolTableN2;
extern set<string> symbolTable;

extern int nextToken;        // next token returned by lexer
extern bool printParse;      // shall we print while parsing?

extern "C" {
	// Instantiate global variables used by flex
	extern int   yylex();      // the generated lexical analyzer
	extern char* yytext;       // text of current lexeme
}

int lex(); // function to get the next token

// First_of functions
bool first_of_program();
bool first_of_block();
bool first_of_stmt();
bool first_of_assignment();
bool first_of_expression();
bool first_of_smpl_expr();
bool first_of_term();
bool isFactor();

// Function declarations

FactorNode* factor();
TermNode* term();
SimpleNode* simpleExpression();
ExprNode* expression();
ProgNode* program();
BlockNode* block();

StmtNode* stmt();
CompNode* compd_stmt();
AssignNode* assignment();
ReadNode* read();
WriteNode* write();
WhileNode* whileStmt();
IfNode* ifStmt();

void writeToSymbolTable();
string checkToken();
bool idExists();

#endif /* PARSER_H */