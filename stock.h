#ifndef STOCK_H
#define STOCK_H

#include <string>

class Stock {
private:
    std::string tickerSymbol;
    std::string companyName;
    double currentPrice;

public:
    Stock(const std::string& ticker, const std::string& name, double price);

    std::string getTicker() const;
    std::string getCompanyName() const;
    double getCurrentPrice() const;

    // Simulate a price update (for the purpose of the simulator)
    void updatePrice(double newPrice);
};

#endif