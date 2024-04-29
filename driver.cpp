//*****************************************************************************
// purpose: Driver for Project Part 2 (parser for TIPS)
// version: Spring 2024
//*****************************************************************************
#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>
#include <iostream>
#include "lexer.h"
#include "parser.h"
#include "parse_nodes.h"
#include <string.h>

using namespace std;

extern "C" {
    // Instantiate global variables
    extern FILE *yyin;     // input stream
    extern FILE *yyout;    // output stream
    extern int   yyleng;   // length of current lexeme
    extern char *yytext;   // text of current lexeme
    extern int   yylineno; // line number for current lexeme
    extern int   yylex();  // the generated lexical analyzer
    extern int   yylex_destroy(); // deletes memory allocated by yylex
}

// Needed global variables
extern int nextToken; // token returned from yylex

//*****************************************************************************
// The main processing loop
//
int main(int argc, char* argv[]) {

    bool printTree = false; // shall we print the parse tree?
    bool printSymbolTable = false;

    // Process any command-line switches
    for(int i = 1; i < argc; i++) {
        // -p flag: if requested, print while parsing
        if(strcmp(argv[i], "-p") == 0) {
            printParse = !printParse;
        }
        // -t flag: if requested, print parse tree
        if(strcmp(argv[i], "-t") == 0) {
            printTree = !printTree;
        }
        // -d flag: if requested, print while deleting parse tree
        if(strcmp(argv[i], "-d") == 0) {
            printDelete = !printDelete;
        }
        // -s flag: if requested, print the symbol table
        if(strcmp(argv[i], "-s") == 0){
            printSymbolTable = !printSymbolTable;
        }
    }
    // Set the input stream
    if (argc > 1) {
        printf("INFO: Using the %s file for input\n", argv[1]);
        yyin = fopen(argv[1], "r"); 
    }
    else {
        printf("INFO: Using the sample.pas file for input\n");
        yyin = fopen("sample.pas", "r");
    }
  
    if (!yyin) {
        printf("ERROR: input file not found\n");
        printf("INFO: Using stdin for input\n");
        yyin = stdin;
    }

    // Set the output stream
    yyout = stdout;

    // Get the first token
    nextToken = yylex();

    // Fire up the parser!
    ProgNode* root = nullptr;
    try {
        // Process the expected production
        //program();  
        // do{
        //     root = program(); // Build bottom-up to <expression>
        // } while (nextToken != TOK_EOF);
        root = program(); // Build bottom-up to <expression>

        if (nextToken != TOK_EOF)
            throw "end of file expected, but there is more here!";
        
    } catch (char const *errmsg) {
        cout << endl << "***ERROR:" << endl;
        cout << "On line number " << yylineno << ", near |" << yytext << "|, error type ";
        cout << errmsg << endl;
        return EXIT_FAILURE;
    }

    if (yyin)
    fclose(yyin);

    yylex_destroy();

    // Tell the world about our success!!
    if(printParse) cout << endl << "=== GO BULLDOGS! Your parse was successful! ===" << endl;

    // Print the tree
    if(printTree){
        cout << endl << "*** Print the Tree ***" << endl;
        cout << *root << endl;
    }
    // Print out the symbol table
    if(printSymbolTable) {
        cout << endl << "*** User Defined Symbols ***" << endl;
    // for (auto it = symbolTable.begin(); it != symbolTable.end(); ++it) {
    //     cout << *it << endl;
    // }
        for(auto it = symbolTableN.begin(); it != symbolTableN.end(); ++it){
            cout << it->first << " " << it->second << endl;
            //cout << it->first << " " << it->second << endl;
        }
    }
    // for(auto it = symbolTableN2.begin(); it != symbolTableN2.end(); ++it){
    //     cout << it->first.first << endl;
    //     //cout << it->first.first << " " << it->first.second << " " << it->second << endl;
    // }
    // Interpret the parse tree
    cout << endl << "*** Interpret the Tree ***" << endl;
    root->interpret();
    // Clean up
    if(printDelete) cout << endl << "*** Delete the Tree ***" << endl;
    if(!symbolTable.empty()) symbolTable.clear();
    if(!symbolTableN.empty()) symbolTableN.clear();
    if(!symbolTableN2.empty()) symbolTableN2.clear();
    if(root!=NULL) {delete root;root = NULL;}

    //cout << root << endl;

    //cout << printParse << " " << printTree << " " << printDelete << endl;

    return EXIT_SUCCESS;
}


