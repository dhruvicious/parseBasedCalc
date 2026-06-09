#ifndef CURRENCY_FETCHER_H
#define CURRENCY_FETCHER_H

#include <string>
#include <unordered_map>

std::unordered_map<std::string, double> fetchCurrencyRates();

#endif
