#ifndef PARSER_H
#define PARSER_H

#include <vector>

#include "ast.h"
#include "tokenizer.h"

class Parser {
   private:
    std::vector<Token> tokens;
    int                pos;

    Token currentToken();

    void eat(TokenType type);

    ASTNode* expr();
    ASTNode* term();
    ASTNode* factor();

   public:
    Parser(const std::vector<Token>& t);
    ASTNode* parse();
};

#endif
