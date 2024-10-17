#include <iostream>
#include <sstream>
#include <cassert>
#include <stdexcept>
#include <memory>
#include "financialAsset.h"
#include "stock.h"
#include "optionStock.h"
#include "portfolio.h"
#include "stockFetcher.h"
#include "user.h"
#include "marketData.h"

#define TEST(name) void name()
#define ASSERT(condition) if (!(condition)) { throw std::runtime_error(#condition); }

TEST(test_financial_assets) {
    Stock apple("AAPL", "Apple Inc.", 150.0);
    ASSERT(apple.getSymbol() == "AAPL");
    ASSERT(apple.getCompanyName() == "Apple Inc.");
    ASSERT(apple.getCurrentPrice() == 150.0);
    ASSERT(apple.getAssetType() == "Stock");

    apple.updatePrice(160.0);
    ASSERT(apple.getCurrentPrice() == 160.0);

    OptionStock googleOption("GOOGL", "Alphabet Inc.", 2800.0, 3000.0, "2023-12-15");
    ASSERT(googleOption.getSymbol() == "GOOGL");
    ASSERT(googleOption.getCompanyName() == "Alphabet Inc.");
    ASSERT(googleOption.getCurrentPrice() == 2800.0);
    ASSERT(googleOption.getStrikePrice() == 3000.0);
    ASSERT(googleOption.getExpirationDate() == "2023-12-15");
    ASSERT(googleOption.getAssetType() == "Option Stock");
}

TEST(test_portfolio) {
    Portfolio portfolio;
    
    portfolio.addAsset(std::make_unique<Stock>("AAPL", "Apple Inc.", 150.0));
    portfolio.addAsset(std::make_unique<OptionStock>("GOOGL", "Alphabet Inc.", 2800.0, 3000.0, "2023-12-15"));
    
    double expectedValue = 10000.0 + 150.0 + 2800.0;
    ASSERT(portfolio.getTotalValue() == expectedValue);

    portfolio.removeAsset("AAPL");
    expectedValue -= 150.0;
    ASSERT(portfolio.getTotalValue() == expectedValue);

    portfolio.addAsset(std::make_unique<Stock>("GOOGL", "Alphabet Inc.", 2850.0));
    expectedValue += 2850.0;
    ASSERT(portfolio.getTotalValue() == expectedValue);
}

TEST(test_stock_fetcher) {
    StockFetcher fetcher("YOUR_API_KEY");
    try {
        double price = fetcher.fetchLatestPrice("AAPL");
        ASSERT(price > 0);
    } catch (const std::exception& e) {
        std::cerr << "StockFetcher test failed: " << e.what() << std::endl;
        throw;
    }
}

TEST(test_user_management) {
    std::map<std::string, std::string> users;
    
    registerUser(users);
    ASSERT(users.size() == 1);

    std::string currentUser;
    ASSERT(loginUser(users, currentUser));
    ASSERT(!currentUser.empty());

    users.clear();
    ASSERT(!loginUser(users, currentUser));
}

TEST(test_portfolio_display) {
    Portfolio portfolio;
    portfolio.addAsset(std::make_unique<Stock>("AAPL", "Apple Inc.", 150.0));
    portfolio.addAsset(std::make_unique<OptionStock>("GOOGL", "Alphabet Inc.", 2800.0, 3000.0, "2023-12-15"));

    std::stringstream output;
    std::streambuf* oldCout = std::cout.rdbuf(output.rdbuf());

    portfolio.displayPortfolio();

    std::cout.rdbuf(oldCout);

    std::string displayOutput = output.str();
    ASSERT(displayOutput.find("AAPL") != std::string::npos);
    ASSERT(displayOutput.find("GOOGL") != std::string::npos);
    ASSERT(displayOutput.find("150.0") != std::string::npos);
    ASSERT(displayOutput.find("2800.0") != std::string::npos);
}

TEST(test_portfolio_save_load) {
    Portfolio originalPortfolio;
    originalPortfolio.addAsset(std::make_unique<Stock>("AAPL", "Apple Inc.", 150.0));
    originalPortfolio.addAsset(std::make_unique<OptionStock>("GOOGL", "Alphabet Inc.", 2800.0, 3000.0, "2023-12-15"));

    originalPortfolio.savePortfolio("test_portfolio.dat");

    Portfolio loadedPortfolio;
    loadedPortfolio.loadPortfolio("test_portfolio.dat");

    ASSERT(loadedPortfolio.getTotalValue() == originalPortfolio.getTotalValue());
}

TEST(test_market_data) {
    MarketData marketData("3af9ca15a9f431b11e79114e5fb172e9f00f716d");
    std::vector<StockData> stockData = marketData.fetchStockData("AAPL");
    
    ASSERT(!stockData.empty());
    ASSERT(stockData[0].open > 0);
    ASSERT(stockData[0].close > 0);
    ASSERT(stockData[0].high > 0);
    ASSERT(stockData[0].low > 0);
    ASSERT(!stockData[0].date.empty());
}

int main() {
    std::cout << "Running unit tests...\n";
    
    try {
        test_financial_assets();
        std::cout << "Financial assets test passed.\n";
        
        test_portfolio();
        std::cout << "Portfolio test passed.\n";
        
        test_stock_fetcher();
        std::cout << "Stock fetcher test passed.\n";
        
        test_user_management();
        std::cout << "User management test passed.\n";
        
        test_portfolio_display();
        std::cout << "Portfolio display test passed.\n";
        
        test_portfolio_save_load();
        std::cout << "Portfolio save/load test passed.\n";
        
        test_market_data();
        std::cout << "Market data test passed.\n";
        
        std::cout << "All tests passed successfully!\n";
    } catch (const std::exception& e) {
        std::cerr << "Test failed: " << e.what() << "\n";
        return 1;
    }
    
    return 0;
}