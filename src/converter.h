#ifndef CONVERTER_H
#define CONVERTER_H

#include <string>
#include <unordered_map>
#include <vector>

#include "tokenizer.h"

void updateConversionRates(
    const std::unordered_map<std::string, double>& newRates,
    const std::string&                             category);

bool extractConversion(std::vector<Token>& tokens, std::string& toUnit);

void printConversionResult(double baseResult, const std::string& toUnit);

void printAvailableUnits();

#endif
