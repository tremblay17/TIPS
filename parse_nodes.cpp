/**********************************************
Name: Dylan Tribble             NetID: dat307
Course: CSE 4714              
Programming Environment: VS Code on Fedora
Purpose of File: Contains the source code for generating 
                the parse tree in tips.
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
#include "parser.h"
#include "parse_nodes.h"
#include <cmath>

extern set<string> symbolTable;
extern symbolTableT symbolTableN;
extern symbolTableT2 symbolTableN2;
extern int nextToken;

bool printDelete = false;   // shall we print deleting the tree?
// ---------------------------------------------------------------------
// Indent according to tree level
static void indent(int level) {
  for (int i = 0; i < level; i++)
    cout << ("|  ");
}
static bool idExists(string id){
    if(symbolTable.count(id)!=0) return true;
    else return false;
}

// ---------------------------------------------------------------------
// FACTOR NODE
FactorNode::~FactorNode() {
  if (printDelete) {
    indent(_level+1);
    cout << "Deleting a FactorNode " << endl;
  }
}
ostream& operator<<(ostream& os, FactorNode& fn) {
    os << endl; indent(fn._level); os << "(factor ";
    fn.print(os);
    os << endl; indent(fn._level); os << "factor) ";
    return os;
}
// INT NODE
IntNode::IntNode(int level, int value) : intFactor(value) {
    _level = level;
}
void IntNode::print(ostream& os) {
    os << "( INTLIT: " << intFactor << " ) ";
}
IntNode::~IntNode() {
    if (printDelete) {
        indent(_level+1);
        cout << "Deleting an IntNode " << endl;
         
    }
}
float IntNode::interpret() {
    //cout << intFactor << endl;
    return intFactor;
}
// FLOAT NODE
FloatNode::FloatNode(int level, float value) : floatFactor(value) {
    _level = level;
}
void FloatNode::print(ostream& os) {
    os << "( FLOATLIT: " << floatFactor << " ) ";
}
FloatNode::~FloatNode() {
    if (printDelete) {
        indent(_level+1);
        cout << "Deleting a FloatNode " << endl;
         
    }
}
float FloatNode::interpret() {
    //cout << floatFactor << endl;
    return floatFactor;
}
// IDENTIFIER NODE
IdNode::IdNode(int level, string value) : idFactor(value) {
    _level = level;
}
void IdNode::print(ostream& os) {
    os << "( IDENT: " << idFactor << " ) ";
}
IdNode::~IdNode() {
    if (printDelete) {
        indent(_level+1);
        cout << "Deleting an IdNode " << endl;
         
    }
}
//Return the value of the identifier from symbolTableN
float IdNode::interpret() {
    //cout << idFactor;
    for(auto it = symbolTableN.begin(); it != symbolTableN.end(); ++it){
        if(it->first == idFactor){
            //cout << it->second << endl;
            return it->second;
        }
    }
    return 1;
}
// NEGATIVE NODE
NegNode::NegNode(int level, FactorNode* factor) : factor(factor) {
    _level = level;
}
void NegNode::print(ostream& os) {
    os << "(- " << *factor << ") ";
}
NegNode::~NegNode() {
    if (printDelete) {
        indent(_level+1);
        cout << "Deleting a NegNode " << endl;
        if(factor!=NULL) {delete factor;
        factor = nullptr;}
         
    }
}
//Shouldn't output each factor
float NegNode::interpret() {
    //cout << "- " << *factor << endl;
    if((int)factor->interpret() == 0){
        return 0;
    }
    else {
        return -factor->interpret();
    }
}
// NOT NODE
NotNode::NotNode(int level, FactorNode* factor) : factor(factor) {
    _level = level;
}
void NotNode::print(ostream& os) {
    os << "(NOT " << *factor << ") ";
}
NotNode::~NotNode() {
    if (printDelete) {
        indent(_level+1);
        cout << "Deleting a NotNode " << endl;
        if(factor!=NULL) {delete factor;
        factor = nullptr;}
         
    }
}
//TODO: IMPLEMENT NOT
float NotNode::interpret() {
    //cout << "NOT " << *factor << endl;
    //return NOT factor->interpret(); 
    if((int)factor->interpret() == 1){
        return 0;
    }
    else {
        return 1;
    }
}
// NESTED EXPRESSION
NestedNode::NestedNode(int level, ExprNode* expr) : expr(expr) {
    _level = level;
}
void NestedNode::print(ostream& os) {
    os << "( " << *expr << ") ";
}
NestedNode::~NestedNode() {
    if (printDelete) {
        indent(_level+1);
        cout << "Deleting a NestedNode " << endl;
        if(expr!=NULL) {delete expr;
        expr = nullptr;}
         
    }
}
//Shouldn't output each expression
float NestedNode::interpret() {
    //cout << "(" << expr->interpret() << ")" << endl;
    return expr->interpret();
}
// TERM NODE
TermNode::~TermNode() {
    if (printDelete) {
        indent(_level+1);
        cout << "Deleting a TermNode " << endl;
        if(factor!=NULL) {delete factor;
        factor = nullptr;}
        for (auto f : factors) {
            if(f!=NULL) {delete f;
            f = nullptr;}
        }
        if(!factors.empty()) factors.clear();
        if(!ops.empty()) ops.clear();
        if(!opToks.empty()) opToks.clear();
         
    }
}
TermNode::TermNode(int level, FactorNode* factor) : factor(factor) {
    _level = level;
}
ostream& operator<<(ostream& os, TermNode& tn) {
    os << endl; indent(tn._level); os << "(term ";
    //sn.print(os);
    os << *(tn.factor);
    int i = 0;
    for (auto f : tn.factors) {
        os << endl; indent(tn._level); //os << tn.ops.at(i) << " ";
        switch (tn.opToks.at(i))
        {
        case TOK_MULTIPLY:
            os << /* tn.opToks.at(i) << */ "* ";
            break;
        case TOK_DIVIDE:
            os << /* tn.opToks.at(i) << */ "/ ";
            break;
        default:
            os << /* tn.opToks.at(i) << */ "AND ";
            break;
        }
        os << *f;
        i++;
    }
    os << endl; indent(tn._level); os << "term) ";
    return os;
}
float TermNode::interpret() {
    float returnF = factor->interpret();
    for(int i = 0; i < opToks.size(); i++){
        //float nextF = factors.at(i)->interpret();
        switch (opToks.at(i))
        {
        case TOK_MULTIPLY:
            returnF *= factors.at(i)->interpret();
            break;
        case TOK_DIVIDE:
            returnF /= factors.at(i)->interpret();
            break;
        default:
            if((int)returnF == 1 && (int)factors.at(i)->interpret() == 1){
                returnF = 1;
            }
            else {
                returnF = 0;
            }
            //returnF = (int)returnF && (int)nextF;
            break;
        }
    }
    return returnF;
}
// SIMPLE EXPRESSION NODE
SimpleNode::~SimpleNode() {
    if (printDelete) {
        indent(_level+1);
        cout << "Deleting a SimpleExprNode " << endl;
        if(term!=NULL) {delete term;
        term = nullptr;}
        for (auto t : terms) {
            if(t!=NULL) {delete t;
            t = nullptr;}
        }
        if(!terms.empty()) terms.clear();
        if(!ops.empty()) ops.clear();
        if(!opToks.empty()) opToks.clear();
         
    }
}
SimpleNode::SimpleNode(int level, TermNode* term) : term(term) {
    _level = level;
}
ostream& operator<<(ostream& os, SimpleNode& sn) {
    os << endl; indent(sn._level); os << "(simple_exp ";
    //sn.print(os);
    os << *(sn.term);
    int i = 0;
    for (auto t : sn.terms) {
        os << endl; indent(sn._level); //os << sn.ops.at(i) << " ";
        switch (sn.opToks.at(i))
        {
        case TOK_PLUS:
            os << /* sn.opToks.at(i) << */ "+ ";
            break;
        case TOK_MINUS:
            os << /* sn.opToks.at(i) << */ "- ";
            break;
        default:
            os << /* sn.opToks.at(i) << */ "OR ";
            break;
        }
        os << *t;
        i++;
    }
    os << endl; indent(sn._level); os << "simple_exp) ";
    return os;
}
float SimpleNode::interpret() {
    float returnT = term->interpret();
    for(int i = 0; i < opToks.size(); i++){
        //float nextT = terms.at(i)->interpret();
        switch (opToks.at(i))
        {
        case TOK_PLUS:
            returnT += terms.at(i)->interpret();
            break;
        case TOK_MINUS:
            returnT -= terms.at(i)->interpret();
            break;
        default:
            if((int)returnT == 1 || (int)terms.at(i)->interpret() == 1){
                returnT = 1;
            }
            else {
                returnT = 0;
            }
            //returnT = (int)returnT || (int)terms.at(i)->interpret();
            break;
        }
    }
    return returnT;
}
// EXPRESSION NODE
ExprNode::~ExprNode() {
    if (printDelete) {
        indent(_level+1);
        cout << "Deleting an ExprNode" << endl;
        if(simple!=NULL) {delete simple;
        simple = nullptr;}
        for (auto s : simples) {
            if(s!=NULL) {delete s;
            s = nullptr;}
        }
        if(!simples.empty()) simples.clear();
        if(!ops.empty()) ops.clear();
        if(!opToks.empty()) opToks.clear();
         
    }
}
ExprNode::ExprNode(int level, SimpleNode* simple) : simple(simple) {
    _level = level;
}
ostream& operator<<(ostream& os, ExprNode& en) {
    os << endl; indent(en._level); os << "(expression ";
    //en.print(os);
    os << *(en.simple);
    int i = 0;
    for (auto s : en.simples) {
        os << endl; indent(en._level); //os << en.ops.at(i) << " ";
        switch (en.opToks.at(i))
        {
        case TOK_EQUALTO:
            os << /* en.opToks.at(i) << */ "= ";
            break;
        case TOK_GREATERTHAN:
            os << /* en.opToks.at(i) << */ "> ";
            break;
        case TOK_LESSTHAN:
            os << /* en.opToks.at(i) << */ "< ";
            break;
        default:
            os << /* en.opToks.at(i) << */ "<> ";
            break;
        }
        os << *s;
        i++;
    }
    os << endl; indent(en._level); os << "expression) ";
    return os;
}
float ExprNode::interpret() {
    float returnS = simple->interpret();
    for(int i = 0; i < opToks.size(); i++){
        //float nextS = simples.at(i)->interpret();
        switch (opToks.at(i))
        {
        case TOK_EQUALTO:
            if((returnS - simples.at(i)->interpret()) <= __FLT_EPSILON__){
                returnS = 1;
            }
            else {
                returnS = 0;
            }
            break;
        case TOK_GREATERTHAN:
            if(returnS > simples.at(i)->interpret()){
                returnS = 1;
            }
            else {
                returnS = 0;
            }
            break;
        case TOK_LESSTHAN:
            if(returnS < simples.at(i)->interpret()){
                returnS = 1;
            }
            else {
                returnS = 0;
            }
            break;
        case TOK_NOTEQUALTO:
            if((returnS - simples.at(i)->interpret()) > __FLT_EPSILON__){
                returnS = 1;
            }
            else {
                returnS = 0;
            }
            break;
        default:
            break;
        }
    }
    return returnS;
}
// PROGRAM NODE
ProgNode::ProgNode(int level, BlockNode* block) : block(block) {
    _level = level;
}
ProgNode::~ProgNode() {
    if (printDelete) {
        indent(_level);
        cout << "Deleting a ProgramNode " << endl;
        if(block!=NULL) {delete block;
        block = nullptr;}
         
    }
}
void ProgNode::print(ostream& os) {
    os << *block;
}
ostream& operator<<(ostream& os, ProgNode& pn) {
    os << endl; os << "(program ";
    pn.print(os);
    os << endl; os << "program) " << endl;
    return os;
}
void ProgNode::interpret() {
    block->interpret();
    return void();
}
// BLOCK NODE
BlockNode::BlockNode(int level, CompNode* compStmt) : compStmt(compStmt) {
    _level = level;
}
BlockNode::~BlockNode() {
    if (printDelete) {
        indent(_level);
        cout << "Deleting a BlockNode " << endl;
        if(compStmt!=NULL) {delete compStmt;
        compStmt = nullptr;}
         
    }
}
void BlockNode::print(ostream& os) {
    os << *compStmt;
}
ostream& operator<<(ostream& os, BlockNode& bn) {
    os << endl; indent(bn._level-1); os << "(block ";
    bn.print(os);
    os << endl; indent(bn._level-1); os << "block) ";
    return os;
}
void BlockNode::interpret() {
    compStmt->interpret();
    return void();
}
// STATEMENT
StmtNode::~StmtNode() {
    if (printDelete) {
        indent(_level);
        cout << "Deleting a StatementNode " << endl;
    }
}
void StmtNode::interpret() {
    this->interpret();
}
ostream& operator<<(ostream& os, StmtNode& sn) {
    sn.print(os);
    return os;
}
// COMPOUND STATEMENT
CompNode::CompNode(int level, StmtNode* stmt) : stmt(stmt) {
    _level = level;
    stmts.push_back(stmt);
}
CompNode::~CompNode() {
    if (printDelete) {
        indent(_level+1);
        cout << "Deleting a CompoundNode " << endl;
        if(stmt!=NULL) {delete stmt;
        stmt = nullptr;}
        for (auto s : stmts) {
            if(s!=NULL) {delete s;
            s = nullptr;}
        }
        if(!stmts.empty()) stmts.clear();
         
    }
}
void CompNode::print(ostream& os) {
    os << endl; indent(_level);
    os << "(compound_stmt ";
    //os << *stmt;
    for(int i = 0; i < stmts.size(); i++){
        os << *stmts.at(i);
    }
    os << endl; indent(_level);
    os << "compound_stmt) ";
}
void CompNode::interpret() {
    for(int i = 0; i < stmts.size(); i++){
        stmts.at(i)->interpret();
    }
    return void();
}
// WRITE STATEMENT
WriteNode::WriteNode(int level) {
    _level = level;
}
WriteNode::WriteNode(int level, string id) : id(id) {
    _level = level;
}
WriteNode::~WriteNode() {
    if (printDelete) {
        indent(_level);
        cout << "Deleting a WriteNode " << endl;
    }
}
void WriteNode::print(ostream& os) {
    // if(nextToken == TOK_IDENT){
    //     if(!idExists(id)) throw "X: IDENTIFIER DOES NOT EXIST";
    // }
    os << endl; indent(_level-1);
    os << "(write_stmt ( " << id << " ) " << endl;
    indent(_level-1); 
    os << "write_stmt) ";
    _level--;
}
void WriteNode::interpret() { 
    bool isVar = false;
    auto varFound = symbolTableN.find(id);

    // if(varFound != symbolTableN.end()){
    //     cout << it->second << endl;
    //     isVar = true;
    // }

    for (auto it = symbolTableN.begin(); it != symbolTableN.end(); it++)
    {
        if(it->first == id){
            //cout << it->first << endl;
            cout << it->second << endl;
            isVar = true;
            break;
            //return it->second;
        }
    }
    if(isVar==false) cout << id << endl;
    return void();
    //cout << id << endl;
}
// READ STATEMENT
ReadNode::ReadNode(int level) {
    _level = level;
}
ReadNode::ReadNode(int level, string id) : id(id) {
    _level = level;
}
ReadNode::~ReadNode() {
    if (printDelete) {
        indent(_level);
        cout << "Deleting a ReadNode " << endl;
    }
}
void ReadNode::print(ostream& os) {
    // if(nextToken == TOK_IDENT){
    //     if(!idExists(id)) throw "X: IDENTIFIER DOES NOT EXIST";
    // }
    os << endl; indent(_level-1);
    os << "(read_stmt ( " << id << " ) ";
    os << endl; indent(_level-1);
    os << "read_stmt) ";
}
void ReadNode::interpret() {
    float idVal = 9;
    //auto it = symbolTableN2.begin();
    //auto end = symbolTableN2.end();
    auto varFound = symbolTableN.find(id);
    auto it = symbolTableN.begin();
    auto end = symbolTableN.end();

    //Wait for user input
    /* TODO */
    cin >> idVal;
    //Store the value of idVal with the identifier | Assign value to id
    /* TODO */

    // if(varFound != symbolTableN.end()){
    //     cout << varFound->second << endl;
    //     varFound->second = idVal;
    // }

    for (it; it != end; it++)
    {
        if(it->first == id){
            // if(typeid(idVal) != typeid(float) || typeid(idVal) != typeid(int)){
            //     cout << "ERROR: INVALID INPUT" << endl;
            //     return 999;
            //     break;
            // }
            it->second = idVal;
            cout << it->second << endl;
            //return (it->second);
            break;
        }
    }
    return void();
    //cout << id << endl;
}
// ASSIGNMENT STATEMENT
AssignNode::AssignNode(int level) : expr(nullptr){
    _level = level;
}
AssignNode::~AssignNode() {
    if (printDelete) {
        indent(_level);
        cout << "Deleting an AssignmentNode " << endl;
        if(this!=NULL) {delete expr;
        expr = nullptr;}
    }
}
void AssignNode::print(ostream& os) {
    os << endl; indent(_level-1);
    os << "(assignment_stmt ( ";
    os << id << " := ) ";
    os << *expr << endl;
    indent(_level-1);
    os << "assignment_stmt) ";
    _level--;
}
void AssignNode::interpret() {
    string idVal = "";
    //auto it = symbolTableN2.begin();
    //auto end = symbolTableN2.end();
    auto varFound = symbolTableN.find(id);
    auto it = symbolTableN.begin();
    auto end = symbolTableN.end();

    //Evaluate expression
    //Store value with id
    // if(varFound != symbolTableN.end()){
    //     //cout << varFound->second << endl;
    //     varFound->second = expr->interpret();
    // }

    for (it; it != end; it++)
    {
        if(it->first == id){
            //cout << "val " << id << " " << it->second << endl;
            it->second = expr->interpret();
            //cout << "val " << id << " " << it->second << endl;
            //return it->second;

            break;
        }
    }
    //cout << id << " := ";
    return void();
}
// IF STATEMENT
IfNode::IfNode(int level) : cond(nullptr), then(nullptr), other(nullptr) {
    _level = level;
}
IfNode::~IfNode() {
    if (printDelete) {
        indent(_level);
        cout << "Deleting an IfNode " << endl;
        if(cond!=NULL) {delete cond;
        cond = nullptr;}
        if(then!=NULL) {delete then;
        then = nullptr;}
        if(other!=NULL) {delete other;
        other = nullptr;}
    }
}
void IfNode::print(ostream& os) {
    os << endl; indent(_level);
    if(other == nullptr) {
        os << "(if_stmt " << *cond;
        os << endl; indent(_level);
        os << "(then " << *then;
        os << endl; indent(_level);
        os << "then) ";
        os << endl; indent(_level);
        os << "if_stmt) ";
    }else{
        os << "(if_stmt " << *cond; 
        os << endl; indent(_level);
        os << "(then " << *then;
        os << endl; indent(_level);
        os << "then) ";
        os << endl; indent(_level);
        os << "(else " << *other; 
        os << endl; indent(_level);
        os << "else) ";
        os << endl; indent(_level);
        os << "if_stmt) ";
    }
}
void IfNode::interpret() {
    //Check if the condition is true
    //Evaluate the then statement
    //Evaluate the else statement if it exists

    //cout << "IF " << endl;
    if(cond->interpret()==1){
        //cout << " THEN " << endl;
        then->interpret();
    }
    else {
        //cout << " ELSE " << endl;
        if(other!=nullptr){
            other->interpret();
        }
    }
    return void();
}
// WHILE STATEMENT
WhileNode::WhileNode(int level) : cond(nullptr), stmt(nullptr) {
    _level = level;
}
WhileNode::~WhileNode() {
    if (printDelete) {
        indent(_level);
        cout << "Deleting a WhileNode " << endl;
        if(cond!=NULL) {delete cond;
        cond = nullptr;}
        if(stmt!=NULL) {delete stmt;
        stmt = nullptr;}
    }
}
void WhileNode::print(ostream& os) {
    os << endl; indent(_level);
    os << "(while_stmt "; 
    // if(nextToken == TOK_IDENT){
    //     if(!idExists(id)) throw "X: IDENTIFIER DOES NOT EXIST";
    // }
    os << *cond;
    os << *stmt; 
    os << endl; indent(_level);
    os << "while_stmt) ";
}
void WhileNode::interpret() {
    //Check if the condition is true
    //If true, execute the statement until the condition is false
    while(cond->interpret()==1){
        //cout << " WHILE " << endl;
        stmt->interpret();
    }

    return void();
}
