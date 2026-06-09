#include "currency_fetcher.h"

#include <cstddef>
#include <curl/curl.h>
#include <exception>
#include <iostream>
#include <memory>
#include <new>
#include <string>
#include <unordered_map>

#include "include/json.hpp"

using json            = nlohmann::json;
const std::string URL = "https://api.frankfurter.dev/v1/latest?from=USD";

static std::size_t WriteCallback(void* contents, size_t size, size_t nmemb,
                                 std::string* userp) noexcept {
    size_t totalSize = size * nmemb;
    if (userp && contents) {
        try {
            userp->append(static_cast<const char*>(contents), totalSize);
        } catch (const std::bad_alloc&) {
            std::cerr << "Critical: Memory allocation failed during curl write "
                         "callback. \n";
            return 0;
        }
    }
    return totalSize;
}

std::unordered_map<std::string, double> fetchCurrencyRates() {
    std::unordered_map<std::string, double> rates;
    rates["USD"] = 1.0;
    std::unique_ptr<CURL, void (*)(CURL*)> curl(curl_easy_init(), [](CURL* c) {
        if (c) curl_easy_cleanup(c);
    });

    if (!curl) {
        std::cerr << "Warning: Failed to initialize curl. Using "
                     "fallback/Hardcoded Rates.\n";
        return rates;
    }

    std::string readBuffer;

    curl_easy_setopt(curl.get(), CURLOPT_URL, URL.c_str());
    curl_easy_setopt(curl.get(), CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl.get(), CURLOPT_WRITEDATA, &readBuffer);
    curl_easy_setopt(curl.get(), CURLOPT_TIMEOUT, 5L);  
    CURLcode res = curl_easy_perform(curl.get());

    if (res == CURLE_OK) {
        try {
            json response = json::parse(readBuffer);

            for (const auto& element : response["rates"].items()) {
                rates[element.key()] = element.value();
            }
        } catch (const json::parse_error& e) {
            std::cerr << "Warning: Failed to parse currency rates (" << e.what()
                      << ").\n";
        } catch (const std::exception& e) {
            std::cerr << "Warning: Unexpected error processing ucrrency data ("
                      << e.what() << ").\n";
        }
    } else {
        std::cerr << "Warning: Network request failed("
                  << curl_easy_strerror(res) << "). Are you offline?\n";
    }
    return rates;
}
