#include <cmath>
#include <cstdlib>
#include <exception>
#include <iostream>
#include <memory>
#include <readline/history.h>
#include <readline/readline.h>
#include <string>
#include <vector>

#include "ast.h"
#include "parser.h"
#include "tokenizer.h"

namespace {

    void printUsage(const char* programName) {
        std::cout << "Usage: " << programName << " [--tokens|-t] [--ast|-a]\n";
    }

    void printTokens(const std::vector<Token>& tokens) {
        std::cout << "Tokens:\n";
        for (const auto& token : tokens) {
            std::cout << "  " << tokenTypeName(token.type);
            if (!token.value.empty()) {
                std::cout << "(\"" << token.value << "\")";
            }
            std::cout << '\n';
        }
    }

}  // namespace

int main(int argc, char* argv[]) {
    bool showTokens = false;
    bool showAst    = false;

    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        if (arg == "--tokens" || arg == "-t") {
            showTokens = true;
        } else if (arg == "--ast" || arg == "-a") {
            showAst = true;
        } else if (arg == "--help" || arg == "-h") {
            printUsage(argv[0]);
            return 0;
        } else {
            std::cout << "Unknown option: " << arg << '\n';
            printUsage(argv[0]);
            return 1;
        }
    }

    while (true) {
        char* line = readline("calc > ");
        if (!line) break;
        std::string input(line);
        std::free(line);
        if (input == "exit") break;
        if (input.empty()) continue;
        if (!input.empty()) add_history(input.c_str());

        try {
            auto tokens = tokenize(input);

            if (showTokens) {
                printTokens(tokens);
            }

            Parser parser(tokens);

            std::unique_ptr<ASTNode> tree = parser.parse();

            if (showAst) {
                std::cout << "AST:\n";
                tree->print(std::cout, 2);
            }

            double result = tree->evaluate();

            if (std::abs(result) < 1e-10) {
                result = 0.0;
            }

            std::cout << "= " << result << '\n';
        } catch (const std::exception& e) {
            std::cout << "Error: " << e.what() << '\n';
        }
    }

    return 0;
}
