#ifndef AST_H
#define AST_H

#include <functional>
#include <memory>
#include <utility>

class ASTNode {
   public:
    virtual ~ASTNode()              = default;
    virtual double evaluate() const = 0;
};

class NumberNode : public ASTNode {
    double value;

   public:
    explicit NumberNode(double val) : value(val) {};
    double evaluate() const override { return value; }
};

class BinOpNode : public ASTNode {
   public:
    std::unique_ptr<ASTNode>              left;
    std::unique_ptr<ASTNode>              right;
    std::function<double(double, double)> operation;

    BinOpNode(std::unique_ptr<ASTNode> l, std::unique_ptr<ASTNode> r,
              std::function<double(double, double)> op) :
        left(std::move(l)), right(std::move(r)), operation(std::move(op)) {}

    double evaluate() const override {
        return operation(left->evaluate(), right->evaluate());
    }
};

class UnaryOpNode : public ASTNode {
    std::unique_ptr<ASTNode>      operand;
    std::function<double(double)> operation;

   public:
    UnaryOpNode(std::unique_ptr<ASTNode>      op,
                std::function<double(double)> func) :
        operand(std::move(op)), operation(std::move(func)) {}

    double evaluate() const override { return operation(operand->evaluate()); }
};

#endif
