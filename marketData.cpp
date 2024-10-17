#include "marketData.h"
#include <cstdlib>  // For rand()
#include <ctime>    // For seeding random

MarketData::MarketData(const std::string& apiKey) : stockFetcher(apiKey) {}

std::vector<StockData> MarketData::fetchStockData(const std::string& ticker) {

return stockFetcher.fetchStockData(ticker, "2024-10-01", "2024-10-30", "daily");

}
