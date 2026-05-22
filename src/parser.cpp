#include "parser.h"

#include <stdexcept>
#include <vector>

#include "ast.h"
#include "tokenizer.h"

Parser::Parser(const std::vector<Token>& t) : tokens(t), pos(0) {
}

Token Parser::currentToken() {
    return tokens[pos];
}

void Parser::eat(TokenType type) {
    if (currentToken().type == type) {
        pos++;
    } else {
        throw std::runtime_error(
            "Unexpected Token:: The Expression Contains an Invalid Token");
    }
}

ASTNode* Parser::parse() {
    return expr();
}

ASTNode* Parser::expr() {
    ASTNode* node = term();
    while (currentToken().type == PLUS || currentToken().type == MINUS) {
        Token op = currentToken();
        if (op.type == PLUS) {
            eat(PLUS);
        } else {
            eat(MINUS);
        }

        node = new BinOpNode(node, op.value, term());
    }
    return node;
}

ASTNode* Parser::term() {
    ASTNode* node = factor();

    while (currentToken().type == MUL || currentToken().type == DIV) {
        Token op = currentToken();

        if (op.type == MUL) {
            eat(MUL);
        } else {
            eat(DIV);
        }
        node = new BinOpNode(node, op.value, factor());
    }

    return node;
}

ASTNode* Parser::factor() {
    Token token = currentToken();
    if (token.type == NUMBER) {
        eat(NUMBER);
        return new NumberNode(std::stod(token.value));
    }

    if (token.type == LPAREN) {
        eat(LPAREN);
        ASTNode* node = expr();
        eat(RPAREN);
        return node;
    }
    throw std::runtime_error("Invalid syntax");
}
