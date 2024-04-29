/********************************************************************* 
  Name: Dylan Tribble             NetID: dat307 
  Course: CSE 4714              Assignment: Part 2 
  Programming Environment: VSCode
  Purpose of File: Contains the rules responsible for recognizing 
                    the different symbols in tips. 
*********************************************************************/
%option yylineno
%option noyywrap
%{
#include "lexer.h"

int lineNumber = 1;
%}

%%
[\n] {lineNumber++; }

{} /*Rules for keywords*/
PROGRAM {return TOK_PROGRAM; }
BEGIN {return TOK_BEGIN; }
BREAK {return TOK_BREAK; }
CONTINUE {return TOK_CONTINUE; }
DOWNTO {return TOK_DOWNTO; }
ELSE {return TOK_ELSE; }
END {return TOK_END; }
FOR {return TOK_FOR; }
IF {return TOK_IF; }
LET {return TOK_LET; }
READ {return TOK_READ; }
THEN {return TOK_THEN; }
TO {return TOK_TO; }
VAR {return TOK_VAR; }
WHILE {return TOK_WHILE; }
WRITE {return TOK_WRITE; }

{} /*Rules for data types*/
{} //REAL|INTEGER {return TOK_TYPE; }
INTEGER {return TOK_INTEGER; }
REAL {return TOK_REAL; }

{} /*Rules for punctuation*/

\; {return TOK_SEMICOLON; }
\: {return TOK_COLON; }
\( {return TOK_OPENPAREN; }
\) {return TOK_CLOSEPAREN;}

{} /*Rules for operators*/
\+ {return TOK_PLUS; }
\- {return TOK_MINUS; }
\* {return TOK_MULTIPLY; }
\/ {return TOK_DIVIDE; }
\:\= {return TOK_ASSIGN; }
\= {return TOK_EQUALTO; }
\< {return TOK_LESSTHAN; }
\> {return TOK_GREATERTHAN; }
\<\> {return TOK_NOTEQUALTO; }
MOD {return TOK_MOD; }
NOT {return TOK_NOT; }
OR {return TOK_OR; }
AND {return TOK_AND; }

{} /*Rules for abstracts (indetifiers, strings and numbers) */

[A-Z][A-Z0-9]{0,8} {return TOK_IDENT; } /* match identifiers*/
[0-9]{1,} {return TOK_INTLIT; } /* match integet literals*/
[0-9]{1,}\.[0-9]{1,} {return TOK_FLOATLIT; } /* match float literals */
\'[^'\n]{0,80}\' {return TOK_STRINGLIT; } /* match string literals */


\{[^}\n]*\}     /* Ignore one-line comments */

[ \t\r]+          /* eat up whitespace */

 /* Invalid SRING LITERALS & IDENTIFIERS*/
[A-Z][A-Z0-9]{8,}  {return TOK_UNKNOWN; }
\'[^'\n]{80,}\' {return TOK_UNKNOWN; }

 /* Found an unknown character */
.         { return TOK_UNKNOWN; }

 /* Recognize end of file */

<<EOF>>   { return TOK_EOF; }

