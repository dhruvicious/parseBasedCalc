#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <string>
#include <vector>

enum TokenType {
    NUMBER,
    IDENTIFIER,
    PLUS,
    MINUS,
    MUL,
    DIV,
    POW,
    LPAREN,
    RPAREN,
    END
};

struct Token {
    TokenType   type;
    std::string value;
};

const char*        tokenTypeName(TokenType type);
std::vector<Token> tokenize(const std::string& input);

#endif
