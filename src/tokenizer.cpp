#include "tokenizer.h"

#include <cctype>
#include <cstddef>
#include <vector>

std::vector<Token> tokenize(const std::string& input) {
    std::vector<Token> tokens;

    std::size_t i = 0;
    while (i < input.size()) {
        char c = input[i];
        if (isspace(c)) {
            i++;
            continue;
        }

        if (isdigit(c) || c == '.') {
            std::string number;
            while (i < input.size() && (isdigit(input[i]) || input[i] == '.')) {
                number += input[i];
                i++;
            }

            tokens.push_back({NUMBER, number});
            continue;
        }

        switch (c) {
            case '+': tokens.push_back({PLUS, "+"}); break;
            case '-': tokens.push_back({MINUS, "-"}); break;
            case '*': tokens.push_back({MUL, "*"}); break;
            case '/': tokens.push_back({DIV, "/"}); break;
            case '(': tokens.push_back({LPAREN, "("}); break;
            case ')': tokens.push_back({RPAREN, ")"}); break;
        }
        i++;
    }
    tokens.push_back({END, ""});
    return tokens;
}
