#ifndef AST_H
#define AST_H

#include <string>

class ASTNode {
   public:
    virtual ~ASTNode() = default;
};

class NumberNode : public ASTNode {
   public:
    double value;
    NumberNode(double val) : value(val) {};
};

class BinOpNode : public ASTNode {
   public:
    ASTNode*    left;
    std::string op;
    ASTNode*    right;

    BinOpNode(ASTNode* l, std::string& o, ASTNode* r) :
        left(l), op(o), right(r) {}

    ~BinOpNode() {
        delete left;
        delete right;
    }
};

#endif
