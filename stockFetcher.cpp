#include "stockFetcher.h"
#include <curl/curl.h>
#include "json.hpp"
#include <sstream>
#include <stdexcept>
#include <ctime>
#include <iomanip>

StockFetcher::StockFetcher(const std::string& key) : apiKey(key) {
    curl_global_init(CURL_GLOBAL_DEFAULT);
}

StockFetcher::~StockFetcher() {
    curl_global_cleanup();
}

// Write callback for handling the HTTP response
size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* data) {
    size_t totalSize = size * nmemb;
    data->append((char*)contents, totalSize);
    return totalSize;
}

// Fetch stock data using API
std::vector<StockData> StockFetcher::fetchStockData(const std::string& ticker, const std::string& startDate, const std::string& endDate, const std::string& timeframe) const {
    CURL* curl;
    CURLcode res;
    std::string readBuffer;
    std::stringstream url;


    url << "https://api.tiingo.com/tiingo/daily/" << ticker << "/prices?startDate=" << startDate << "&endDate=" << endDate << "&token=" << apiKey;

    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.str().c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
    }

    std::vector<StockData> stockDataList;

    if (res != CURLE_OK) {
        throw std::runtime_error("Error fetching stock data from Tiingo.");
    }

    try {
        auto jsonData = nlohmann::json::parse(readBuffer);
        for (const auto& entry : jsonData) {
            StockData data;
            data.open = entry["open"].get<double>();
            data.close = entry["close"].get<double>();
            data.high = entry["high"].get<double>();
            data.low = entry["low"].get<double>();
            data.date = entry["date"].get<std::string>();
            stockDataList.push_back(data);
        }
    } catch (const nlohmann::json::exception& e) {
        throw std::runtime_error("Failed to parse JSON response: " + std::string(e.what()));
    }

    return stockDataList;
}

// Fetch the latest price of a stock using API
double StockFetcher::fetchLatestPrice(const std::string& ticker) const {
    CURL* curl;
    CURLcode res;
    std::string readBuffer;
    std::stringstream url;

    url << "https://api.tiingo.com/tiingo/daily/" << ticker << "/prices?token=" << apiKey;

    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.str().c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
    }

    if (res != CURLE_OK) {
        throw std::runtime_error("Error fetching latest stock price from Tiingo.");
    }

    try {
        auto jsonData = nlohmann::json::parse(readBuffer);
        if (!jsonData.empty()) {
            return jsonData[0]["close"].get<double>();
        }
        throw std::runtime_error("No data returned for ticker.");
    } catch (const nlohmann::json::exception& e) {
        throw std::runtime_error("Failed to parse JSON response: " + std::string(e.what()));
    }
}
