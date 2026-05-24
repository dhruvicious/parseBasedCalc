#ifndef AST_H
#define AST_H

#include <functional>
#include <memory>
#include <ostream>
#include <string>
#include <utility>

inline void printAstIndent(std::ostream& out, int indent) {
    for (int i = 0; i < indent; i++) {
        out << ' ';
    }
}

class ASTNode {
   public:
    virtual ~ASTNode()                                        = default;
    virtual double evaluate() const                           = 0;
    virtual void   print(std::ostream& out, int indent) const = 0;
};

class NumberNode : public ASTNode {
    double value;

   public:
    explicit NumberNode(double val) : value(val) {};
    double evaluate() const override { return value; }

    void print(std::ostream& out, int indent) const override {
        printAstIndent(out, indent);
        out << "Number(" << value << ")\n";
    }
};

class BinOpNode : public ASTNode {
   public:
    std::unique_ptr<ASTNode>              left;
    std::unique_ptr<ASTNode>              right;
    std::function<double(double, double)> operation;
    std::string                           opName;

    BinOpNode(std::unique_ptr<ASTNode> l, std::unique_ptr<ASTNode> r,
              std::function<double(double, double)> op, std::string name) :
        left(std::move(l)),
        right(std::move(r)),
        operation(std::move(op)),
        opName(std::move(name)) {}

    double evaluate() const override {
        return operation(left->evaluate(), right->evaluate());
    }

    void print(std::ostream& out, int indent) const override {
        printAstIndent(out, indent);
        out << "BinOp(" << opName << ")\n";
        left->print(out, indent + 2);
        right->print(out, indent + 2);
    }
};

class UnaryOpNode : public ASTNode {
    std::unique_ptr<ASTNode>      operand;
    std::function<double(double)> operation;
    std::string                   opName;

   public:
    UnaryOpNode(std::unique_ptr<ASTNode> op, std::function<double(double)> func,
                std::string name) :
        operand(std::move(op)),
        operation(std::move(func)),
        opName(std::move(name)) {}

    double evaluate() const override { return operation(operand->evaluate()); }

    void print(std::ostream& out, int indent) const override {
        printAstIndent(out, indent);
        out << "UnaryOp(" << opName << ")\n";
        operand->print(out, indent + 2);
    }
};

#endif
