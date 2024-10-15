#include "stockFetcher.h"
#include <curl/curl.h>
#include "json.hpp"
#include <sstream>
#include <stdexcept>
#include <iostream>  // For debugging

// Write callback for handling the HTTP response
size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* data) {
    size_t totalSize = size * nmemb;
    data->append((char*)contents, totalSize);
    return totalSize;
}

StockFetcher::~StockFetcher() {
    curl_global_cleanup();
}

std::string StockFetcher::makeApiCall(const std::string& url) const {
    CURL* curl = curl_easy_init();
    std::string readBuffer;

    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

        CURLcode res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);

        if (res != CURLE_OK) {
            throw std::runtime_error("Failed to fetch data: " + std::string(curl_easy_strerror(res)));
        }
    } else {
        throw std::runtime_error("Failed to initialize CURL");
    }

    return readBuffer;
}

double StockFetcher::fetchLatestPrice(const std::string& ticker) const {
    std::stringstream url;
    url << "https://api.tiingo.com/tiingo/daily/" << ticker << "/prices?token=" << apiKey;

    std::string response = makeApiCall(url.str());

    // Debugging: Print the JSON response to inspect it
    std::cout << "API Response: " << response << std::endl;

    try {
        auto jsonData = nlohmann::json::parse(response);

        // Check if the response is an array or object
        if (!jsonData.is_array() || jsonData.empty()) {
            throw std::runtime_error("Unexpected JSON structure for ticker: " + ticker);
        }

        return jsonData[0]["close"].get<double>();  // This assumes the correct structure
    } catch (const nlohmann::json::exception& e) {
        throw std::runtime_error("Failed to parse JSON response: " + std::string(e.what()));
    }
}
