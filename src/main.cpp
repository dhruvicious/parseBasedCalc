#include <exception>
#include <iostream>
#include <readline/history.h>
#include <readline/readline.h>
#include <string>

#include "ast.h"
#include "evaluator.h"
#include "parser.h"
#include "tokenizer.h"

int main() {
    std::string input;
    while (true) {
        char* line = readline("calc > ");
        if (!line) break;
        std::string input(line);
        free(line);
        if (input == "exit") break;
        if (!input.empty()) add_history(input.c_str());

        try {
            auto   tokens = tokenize(input);
            Parser parser(tokens);

            ASTNode* tree   = parser.parse();
            double   result = evaluate(tree);

            std::cout << "= " << result << '\n';
            delete tree;
        } catch (const std::exception& e) {
            std::cout << "Error: " << e.what() << '\n';
        }
    }

    return 0;
}
