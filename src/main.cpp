#include <cstdlib>
#include <curl/curl.h>
#include <iomanip>
#include <iostream>
#include <memory>
#include <readline/history.h>
#include <readline/readline.h>
#include <string>
#include <vector>

#include "ast.h"
#include "converter.h"
#include "currency_fetcher.h"
#include "parser.h"
#include "tokenizer.h"

namespace {
    void printUsage(const char* programName) {
        std::cout << "Usage: " << programName
                  << " [--tokens|-t] [--ast|-a] [--convert|-c]\n";
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
    bool showTokens  = false;
    bool showAst     = false;
    bool convertMode = false;

    curl_global_init(CURL_GLOBAL_DEFAULT);

    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        if (arg == "--tokens" || arg == "-t")
            showTokens = true;
        else if (arg == "--ast" || arg == "-a")
            showAst = true;
        else if (arg == "--convert" || arg == "-c")
            convertMode = true;
        else if (arg == "--help" || arg == "-h") {
            printUsage(argv[0]);
            return 0;
        } else {
            std::cout << "Unknown option: " << arg << '\n';
            printUsage(argv[0]);
            return 1;
        }
    }

    if (convertMode) {
        std::cout << "Fetching live currency rates...\n";
        auto liveRates = fetchCurrencyRates();

        updateConversionRates(liveRates, "currency");

        std::cout << "Successfully loaded " << liveRates.size()
                  << " live currencies.\n\n";

        printAvailableUnits();
        std::cout << "\n";
    }

    while (true) {
        const char* prompt = convertMode ? "convert > " : "calc > ";
        char*       line   = readline(prompt);

        if (!line) break;
        std::string input(line);
        std::free(line);

        if (input == "exit") break;
        if (input.empty()) continue;
        add_history(input.c_str());

        try {
            auto tokens = tokenize(input);

            std::string toUnit;
            bool        isConversion = false;

            if (convertMode) {
                isConversion = extractConversion(tokens, toUnit);
            }

            if (showTokens) printTokens(tokens);

            Parser                   parser(tokens);
            std::unique_ptr<ASTNode> tree = parser.parse();

            if (showAst) {
                std::cout << "AST:\n";
                tree->print(std::cout, 2);
            }

            double result = tree->evaluate();

            if (isConversion) {
                printConversionResult(result, toUnit);
            } else {
                std::cout << "= " << std::setprecision(10) << result << '\n';
            }

        } catch (const ParseError& e) {
            std::cout << input << '\n';
            std::cout << std::string(e.index, ' ') << "^\n";
            std::cout << "Error: " << e.what() << '\n';
        } catch (const std::runtime_error& e) {
            std::cout << "Conversion Error: " << e.what() << '\n';
        } catch (const std::exception& e) {
            std::cout << "Unexpected Error: " << e.what() << '\n';
        }
    }
    curl_global_cleanup();
    return 0;
}
