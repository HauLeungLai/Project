// stockFetcher.h
#ifndef STOCKFETCHER_H
#define STOCKFETCHER_H

#include <string>
#include <vector>

// Structure to hold stock data
struct StockData {
    double open;
    double close;
    double high;
    double low;
    std::string date;
};

// Class declaration
class StockFetcher {
public:
    StockFetcher() = default;
    
    std::vector<StockData> fetchStockData(const std::string& ticker, const std::string& startDate, const std::string& endDate) const;

    double fetchLatestPrice(const std::string& ticker) const;  // Ensure only one argument
};

#endif
