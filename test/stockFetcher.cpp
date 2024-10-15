#include "stockFetcher.h"
#include <iostream>
#include <vector>

// Test Data for stock
std::vector<StockData> getTestStockData() {
    std::vector<StockData> testData = {
        {150.0, 155.0, 158.0, 149.5, "2024-10-15T09:00:00"},
        {155.0, 160.0, 162.0, 154.5, "2024-10-15T10:00:00"},
        {160.0, 158.0, 161.0, 157.0, "2024-10-15T11:00:00"},
        {158.0, 157.0, 160.0, 156.0, "2024-10-15T12:00:00"},
        {157.0, 159.0, 161.0, 156.5, "2024-10-15T13:00:00"},
        {159.0, 162.0, 164.0, 158.5, "2024-10-15T14:00:00"},
        {162.0, 161.0, 163.0, 159.0, "2024-10-15T15:00:00"}
    };
    return testData;
}

// Function to fetch stock data (using test data for now)
std::vector<StockData> StockFetcher::fetchStockData(const std::string& ticker, const std::string& startDate, const std::string& endDate) const {
    // For now, return the test data
    return getTestStockData();
}

double StockFetcher::fetchLatestPrice(const std::string& ticker) const {
    std::vector<StockData> data = getTestStockData();
    return data.back().close;  // Return the latest close price
}
