#include "evaluator.h"

#include <stdexcept>

double evaluate(ASTNode* node) {
    NumberNode* num = dynamic_cast<NumberNode*>(node);

    if (num) return num->value;

    BinOpNode* bin = dynamic_cast<BinOpNode*>(node);

    if (!bin) throw std::runtime_error("Invalid AST");

    double left  = evaluate(bin->left);
    double right = evaluate(bin->right);

    if (bin->op == "+") return left + right;

    if (bin->op == "-") return left - right;

    if (bin->op == "*") return left * right;

    if (bin->op == "/") return left / right;

    throw std::runtime_error("Unknown operator");
}
