#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <string>
#include <vector>

enum TokenType { NUMBER, PLUS, MINUS, MUL, DIV, LPAREN, RPAREN, END };

struct Token {
    TokenType   type;
    std::string value;
};

std::vector<Token> tokenize(const std::string& input);

#endif
