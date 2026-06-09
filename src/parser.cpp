#include "parser.h"

#include <cmath>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

#include "ast.h"
#include "tokenizer.h"

Parser::Parser(const std::vector<Token>& t) : tokens(t), pos(0) {
    constants["pi"]  = 3.14159265358979323846;
    constants["e"]   = 2.71828182845904523536;
    constants["phi"] = 1.6180339887;

    functions["sin"]  = [](double x) { return std::sin(x); };
    functions["cos"]  = [](double x) { return std::cos(x); };
    functions["tan"]  = [](double x) { return std::tan(x); };
    functions["sqrt"] = [](double x) { return std::sqrt(x); };
    functions["log"]  = [](double x) { return std::log10(x); };
    functions["ln"]   = [](double x) { return std::log(x); };
}

const Token& Parser::currentToken() const {
    if (pos >= tokens.size()) {
        // Fallback index in case of catastrophic token sequence failure
        std::size_t idx = tokens.empty() ? 0 : tokens.back().index;
        throw ParseError("Unexpected end of input", idx);
    }

    return tokens[pos];
}

void Parser::eat(TokenType type) {
    if (currentToken().type == type) {
        pos++;
    } else {
        throw ParseError(
            "Unexpected Token: Expected a different token but found '"
                + currentToken().value + "'",
            currentToken().index);
    }
}

std::unique_ptr<ASTNode> Parser::parse() {
    auto node = expr();
    if (currentToken().type != END) {
        throw ParseError("Unexpected token at end of expression: '"
                             + currentToken().value + "'",
                         currentToken().index);
    }
    return node;
}

std::unique_ptr<ASTNode> Parser::expr() {
    auto node = term();
    while (currentToken().type == PLUS || currentToken().type == MINUS) {
        Token op = currentToken();
        eat(op.type);

        if (op.type == PLUS) {
            node = std::make_unique<BinOpNode>(
                std::move(node), term(),
                [](double a, double b) { return a + b; }, "+");
        } else {
            node = std::make_unique<BinOpNode>(
                std::move(node), term(),
                [](double a, double b) { return a - b; }, "-");
        }
    }
    return node;
}

std::unique_ptr<ASTNode> Parser::term() {
    auto node = unary();
    while (currentToken().type == MUL || currentToken().type == DIV) {
        Token op = currentToken();
        eat(op.type);

        if (op.type == MUL) {
            node = std::make_unique<BinOpNode>(
                std::move(node), unary(),
                [](double a, double b) { return a * b; }, "*");
        } else {
            node = std::make_unique<BinOpNode>(
                std::move(node), unary(),
                [](double a, double b) {
                    if (b == 0.0) {
                        // Math evaluation error, standard runtime_error is fine
                        // here
                        throw std::runtime_error("Division by zero");
                    }
                    return a / b;
                },
                "/");
        }
    }
    return node;
}

std::unique_ptr<ASTNode> Parser::unary() {
    Token token = currentToken();

    if (token.type == MINUS) {
        eat(MINUS);
        return std::make_unique<UnaryOpNode>(
            unary(), [](double a) { return -a; }, "-");
    }

    if (token.type == PLUS) {
        eat(PLUS);
        return unary();
    }

    return power();
}

std::unique_ptr<ASTNode> Parser::power() {
    auto node = primary();

    if (currentToken().type == POW) {
        eat(POW);
        node = std::make_unique<BinOpNode>(
            std::move(node), unary(),
            [](double a, double b) -> double { return std::pow(a, b); }, "^");
    }
    return node;
}

std::unique_ptr<ASTNode> Parser::primary() {
    Token token = currentToken();

    if (token.type == NUMBER) {
        eat(NUMBER);
        return std::make_unique<NumberNode>(std::stod(token.value));
    }

    if (token.type == IDENTIFIER) {
        eat(IDENTIFIER);
        std::string name = token.value;

        if (constants.find(name) != constants.end()) {
            return std::make_unique<NumberNode>(constants[name]);
        }

        if (functions.find(name) != functions.end()) {
            eat(LPAREN);
            auto argNode = expr();
            eat(RPAREN);

            return std::make_unique<UnaryOpNode>(
                std::move(argNode), functions[name], name);
        }

        throw ParseError("Unknown Identifier: '" + name
                             + "' is not part of the calculator's syntax",
                         token.index);
    }

    if (token.type == LPAREN) {
        eat(LPAREN);
        auto node = expr();
        eat(RPAREN);
        return node;
    }

    // The ultimate syntax failure fallback!
    throw ParseError(
        "Invalid syntax: unexpected token '" + token.value + "'", token.index);
}
