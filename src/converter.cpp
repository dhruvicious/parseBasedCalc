#include "converter.h"

#include <iostream>
#include <stdexcept>

namespace {
    struct UnitData {
        std::string category;
        double      rate;
    };

    std::unordered_map<std::string, UnitData> units = {
        {"kg", {"mass", 1.0}},        {"g", {"mass", 0.001}},
        {"mg", {"mass", 0.000001}},   {"lbs", {"mass", 0.453592}},
        {"oz", {"mass", 0.0283495}},

        {"m", {"length", 1.0}},       {"km", {"length", 1000.0}},
        {"cm", {"length", 0.01}},     {"mm", {"length", 0.001}},
        {"ft", {"length", 0.3048}},   {"in", {"length", 0.0254}},
        {"yd", {"length", 0.9144}},   {"mi", {"length", 1609.34}},

        {"s", {"time", 1.0}},         {"min", {"time", 60.0}},
        {"hr", {"time", 3600.0}},     {"day", {"time", 86400.0}},

        {"L", {"volume", 1.0}},       {"mL", {"volume", 0.001}},
        {"gal", {"volume", 3.78541}}, {"qt", {"volume", 0.946353}},
        {"pt", {"volume", 0.473176}}};
}  // namespace

void updateConversionRates(
    const std::unordered_map<std::string, double>& newRates,
    const std::string&                             category) {
    for (const auto& pair : newRates) {
        units[pair.first] = {category, 1.0 / pair.second};
    }
}

bool extractConversion(std::vector<Token>& tokens, std::string& toUnit) {
    int targetIdx = -1;
    if (tokens.empty()) return false;
    for (int i = tokens.size() - 1; i >= 0; --i) {
        if (tokens[i].type == IDENTIFIER
            && (tokens[i].value == "to" || tokens[i].value == "in")) {
            targetIdx = i;
            break;
        }
    }

    if (targetIdx == -1 || targetIdx + 1 >= (int) tokens.size() - 1) {
        for (const auto& token : tokens) {
            if (token.type == IDENTIFIER
                && units.find(token.value) != units.end()) {
                toUnit = token.value;
                break;
            }
        }
        if (toUnit.empty()) return false;

    } else {
        toUnit = tokens[targetIdx + 1].value;
        tokens.erase(
            tokens.begin() + targetIdx, tokens.begin() + targetIdx + 2);
    }

    if (units.find(toUnit) == units.end()) {
        throw std::runtime_error("Unknown target unit: " + toUnit);
    }

    std::string targetCategory = units[toUnit].category;

    std::vector<Token> expandedTokens;
    for (const auto& token : tokens) {
        if (token.type == IDENTIFIER
            && units.find(token.value) != units.end()) {
            std::string currentUnit = token.value;

            if (units[currentUnit].category != targetCategory) {
                throw std::runtime_error("Dimension mismatch");
            }

            if (!expandedTokens.empty()
                && expandedTokens.back().type == NUMBER) {
                Token prevNum = expandedTokens.back();
                expandedTokens.pop_back();

                expandedTokens.push_back({LPAREN, "("});
                expandedTokens.push_back(prevNum);
                expandedTokens.push_back({MUL, "*"});
                expandedTokens.push_back(
                    {NUMBER, std::to_string(units[currentUnit].rate)});
                expandedTokens.push_back({RPAREN, ")"});
            } else {
                expandedTokens.push_back({MUL, "*"});
                expandedTokens.push_back(
                    {NUMBER, std::to_string(units[currentUnit].rate)});
            }
        } else {
            expandedTokens.push_back(token);
        }
    }
    tokens = expandedTokens;
    return true;
}

void printConversionResult(double baseResult, const std::string& toUnit) {
    double finalResult = baseResult / units[toUnit].rate;
    std::cout << "= " << finalResult << " " << toUnit << '\n';
}

void printAvailableUnits() {
    std::cout << "Available Units for Conversion:\n";
    std::unordered_map<std::string, std::vector<std::string>> groupedUnits;
    for (const auto& pair : units)
        groupedUnits[pair.second.category].push_back(pair.first);

    for (const auto& group : groupedUnits) {
        std::cout << "  [" << group.first << "]: ";
        for (size_t i = 0; i < group.second.size(); ++i) {
            std::cout << group.second[i]
                      << (i < group.second.size() - 1 ? ", " : "");
        }
        std::cout << '\n';
    }
    std::cout << "Usage Example: 10 mi to km  (or just type '10 mi + 5 km')\n";
}
