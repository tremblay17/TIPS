/**********************************************
Name: Dylan Tribble             NetID: dat307
Course: CSE 4714              
Programming Environment: VS Code on Fedora
Purpose of File: Header file declaring the necessary functions/variables 
                for generating the parse tree in tips.
**********************************************/

#ifndef PARSE_NODES_H
#define PARSE_NODES_H

#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <set>
#include "lexer.h"

using namespace std;

extern bool printDelete;      // shall we print deleting the tree?

class FactorNode;
class TermNode;
class SimpleNode;
class ExprNode;

class ProgNode;
class BlockNode;
class StmtNode;
class CompNode;
class WriteNode;

class FactorNode {
    public:
        int _level;
        virtual ~FactorNode();
        virtual void print(ostream& os) = 0;
        virtual float interpret() = 0;
};
ostream& operator<<(ostream& os, FactorNode& fn);
class IntNode : public FactorNode {
    public:
        int intFactor;
        IntNode(int level, int value);
        ~IntNode();
        void print(ostream& os);
        float interpret();
};
class FloatNode : public FactorNode {
    public:
        float floatFactor;
        FloatNode(int level, float value);
        ~FloatNode();
        void print(ostream& os);
        float interpret();
};
class StringNode : public FactorNode {
    public:
        string value;
        StringNode(int level, string value);
        ~StringNode();
        void print(ostream& os);
        float interpret();
};
class IdNode : public FactorNode {
    public:
        string idFactor;
        IdNode(int level, string value);
        ~IdNode();
        void print(ostream& os);
        float interpret();
};
class NegNode : public FactorNode {
    public:
        FactorNode* factor;
        NegNode(int level, FactorNode* factor);
        ~NegNode();
        void print(ostream& os);
        float interpret();
};
class NotNode : public FactorNode {
    public:
        FactorNode* factor;
        NotNode(int level, FactorNode* factor);
        ~NotNode();
        void print(ostream& os);
        float interpret();
};
class NestedNode : public FactorNode {
    public:
        ExprNode* expr;
        NestedNode(int level, ExprNode* expr);
        ~NestedNode();
        void print(ostream& os);
        float interpret();
};

class TermNode {
    public:
        int _level;
        FactorNode* factor;
        vector<FactorNode*> factors = {};
        vector<string> ops = {};
        vector<int> opToks = {};
        TermNode(int level, FactorNode* factor);
        ~TermNode();
        void print(ostream& os);
        float interpret();
};
ostream& operator<<(ostream& os, TermNode& tn);

class SimpleNode {
    public:
        int _level;
        TermNode* term;
        vector<TermNode*> terms = {};
        vector<string> ops = {};
        vector<int> opToks = {};
        SimpleNode(int level, TermNode* term);
        ~SimpleNode();
        void print(ostream& os);
        float interpret();
};
ostream& operator<<(ostream& os, SimpleNode& sn);

class ExprNode {
    public:
        int _level;
        SimpleNode* simple;
        vector<SimpleNode*> simples = {};
        vector<string> ops = {};
        vector<int> opToks = {};
        ExprNode(int level, SimpleNode* simple);
        ~ExprNode();
        void print(ostream& os);
        float interpret();
};
ostream& operator<<(ostream& os, ExprNode& en);

class ProgNode{
    public:
        int _level;
        BlockNode* block;
        ProgNode(int level, BlockNode* block);
        ~ProgNode();
        void print(ostream& os);
        void interpret();
};
ostream& operator<<(ostream& os, ProgNode& pn);

class BlockNode{
    public:
        int _level;
        CompNode* compStmt;
        //StmtNode* firstStmt;
        BlockNode(int level, CompNode* compStmt);
        ~BlockNode();
        void print(ostream& os);
        void interpret();
};
ostream& operator<<(ostream& os, BlockNode& bn);

class StmtNode{
    public:
        int _level;
        virtual ~StmtNode();
        virtual void print(ostream& os) = 0;
        virtual void interpret() = 0;
};
ostream& operator<<(ostream& os, StmtNode& sn);
class CompNode : public StmtNode {
    public:
        StmtNode* stmt;
        vector<StmtNode*> stmts = {};
        CompNode(int level, StmtNode* stmt);
        ~CompNode();
        void print(ostream& os);
        void interpret();
};
class WriteNode : public StmtNode {
    public:
        string id;
        //WriteNode* wn;
        WriteNode(int level);
        WriteNode(int level, string id);
        //WriteNode(int level, WriteNode* wn);
        ~WriteNode();
        void print(ostream& os);
        void interpret();
};
class ReadNode : public StmtNode {
    public:
        string id;
        string type;
        ReadNode(int level);
        ReadNode(int level, string id);
        ~ReadNode();
        void print(ostream& os);
        void interpret();
};
class AssignNode : public StmtNode {
    public:
        string id; //lhs
        ExprNode* expr; //rhs
        AssignNode(int level);
        ~AssignNode();
        void print(ostream& os);
        void interpret();
};
class IfNode : public StmtNode {
    public:
        ExprNode* cond;
        StmtNode* then;
        StmtNode* other;
        IfNode(int level);
        ~IfNode();
        void print(ostream& os);
        void interpret();
};
class WhileNode : public StmtNode {
    public:
        ExprNode* cond;
        StmtNode* stmt;
        WhileNode(int level);
        ~WhileNode();
        void print(ostream& os);
        void interpret();
};



#endif // PARSE_TREE_NODES_H
