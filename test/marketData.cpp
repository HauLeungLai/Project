#include "marketData.h"
#include <cstdlib>  // For rand()
#include <ctime>    // For seeding random

MarketData::MarketData(const std::string& apiKey) : stockFetcher(apiKey) {}

std::vector<StockData> MarketData::fetchStockData(const std::string& ticker) {
    // Use StockFetcher to get real-time stock data from the API
    return stockFetcher.fetchStockData(ticker);
}
