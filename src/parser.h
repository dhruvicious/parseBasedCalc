#ifndef PARSER_H
#define PARSER_H

#include <cstddef>
#include <functional>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "ast.h"
#include "tokenizer.h"

class ParseError : public std::runtime_error {
   public:
    std::size_t index;
    ParseError(const std::string& msg, std::size_t idx) :
        std::runtime_error(msg), index(idx) {}
};

class Parser {
   private:
    std::vector<Token> tokens;
    std::size_t        pos;

    std::unordered_map<std::string, double>                        constants;
    std::unordered_map<std::string, std::function<double(double)>> functions;

    const Token& currentToken() const;

    void eat(TokenType type);

    std::unique_ptr<ASTNode> expr();
    std::unique_ptr<ASTNode> term();
    std::unique_ptr<ASTNode> unary();
    std::unique_ptr<ASTNode> power();
    std::unique_ptr<ASTNode> primary();

   public:
    Parser(const std::vector<Token>& t);
    std::unique_ptr<ASTNode> parse();
};

#endif
