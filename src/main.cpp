#include <cmath>
#include <cstdlib>
#include <exception>
#include <iostream>
#include <memory>
#include <readline/history.h>
#include <readline/readline.h>
#include <string>

#include "ast.h"
#include "parser.h"
#include "tokenizer.h"

int main() {
    while (true) {
        char* line = readline("calc > ");
        if (!line) break;
        std::string input(line);
        std::free(line);
        if (input == "exit") break;
        if (input.empty()) continue;
        if (!input.empty()) add_history(input.c_str());

        try {
            auto   tokens = tokenize(input);
            Parser parser(tokens);

            std::unique_ptr<ASTNode> tree   = parser.parse();
            double                   result = tree->evaluate();

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
