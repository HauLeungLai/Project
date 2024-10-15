#ifndef STOCKFETCHER_H
#define STOCKFETCHER_H

#include <string>
#include <vector>

struct StockData {
    double open;
    double close;
    double high;
    double low;
    std::string date;
};

class StockFetcher {
private:
    std::string apiKey;

public:
    StockFetcher(const std::string& key) : apiKey(key) {}
    ~StockFetcher();

    // Fetch stock data for a given ticker
    std::vector<StockData> fetchStockData(const std::string& ticker, const std::string& startDate, const std::string& endDate) const;

    // Fetch the latest price for a stock
    double fetchLatestPrice(const std::string& ticker) const;

private:
    std::string makeApiCall(const std::string& url) const;
};

#endif
