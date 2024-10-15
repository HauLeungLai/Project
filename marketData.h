#ifndef MARKETDATA_H
#define MARKETDATA_H

#include "stock.h"
#include "stockFetcher.h"  // Include StockFetcher to fetch real data
#include <map>
#include <vector>

class MarketData {
private:
    StockFetcher stockFetcher;  // Use StockFetcher for real-time data

public:
    MarketData(const std::string& apiKey);

    // Retrieve stock data for a given ticker
    std::vector<StockData> fetchStockData(const std::string& ticker);
};

#endif