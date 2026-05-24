#include "tokenizer.h"

#include <cctype>
#include <cstddef>
#include <stdexcept>
#include <string>
#include <vector>

const char* tokenTypeName(TokenType type) {
    switch (type) {
        case NUMBER: return "NUMBER";
        case IDENTIFIER: return "IDENTIFIER";
        case PLUS: return "PLUS";
        case MINUS: return "MINUS";
        case MUL: return "MUL";
        case DIV: return "DIV";
        case POW: return "POW";
        case LPAREN: return "LPAREN";
        case RPAREN: return "RPAREN";
        case END: return "END";
    }

    return "UNKNOWN";
}

std::vector<Token> tokenize(const std::string& input) {
    std::vector<Token> tokens;

    std::size_t i = 0;
    while (i < input.size()) {
        char          c  = input[i];
        unsigned char uc = static_cast<unsigned char>(c);

        if (std::isspace(uc)) {
            i++;
            continue;
        }

        if (std::isalpha(uc)) {
            std::string ident;
            while (i < input.size()) {
                unsigned char identChar = static_cast<unsigned char>(input[i]);
                if (!std::isalnum(identChar) && input[i] != '_') {
                    break;
                }
                ident += input[i++];
            }
            tokens.push_back({IDENTIFIER, ident});
            continue;
        }

        if (std::isdigit(uc) || c == '.') {
            std::string number;
            bool        seenDecimal = false;
            bool        seenDigit   = false;

            while (i < input.size()) {
                unsigned char numberChar = static_cast<unsigned char>(input[i]);

                if (std::isdigit(numberChar)) {
                    seenDigit = true;
                    number += input[i++];
                    continue;
                }

                if (input[i] == '.') {
                    if (seenDecimal) {
                        throw std::runtime_error("Invalid number: " + number
                                                 + ".");
                    }
                    seenDecimal = true;
                    number += input[i++];
                    continue;
                }

                break;
            }

            if (!seenDigit) {
                throw std::runtime_error("Invalid number: " + number);
            }

            tokens.push_back({NUMBER, number});
            continue;
        }

        switch (c) {
            case '+': tokens.push_back({PLUS, "+"}); break;
            case '-': tokens.push_back({MINUS, "-"}); break;
            case '*': tokens.push_back({MUL, "*"}); break;
            case '/': tokens.push_back({DIV, "/"}); break;
            case '^': tokens.push_back({POW, "^"}); break;
            case '(': tokens.push_back({LPAREN, "("}); break;
            case ')': tokens.push_back({RPAREN, ")"}); break;
            default:
                throw std::runtime_error("Unexpected character: "
                                         + std::string(1, c));
        }
        i++;
    }
    tokens.push_back({END, ""});
    return tokens;
}
