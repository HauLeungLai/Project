#include "stock.h"

Stock::Stock(const std::string& ticker, const std::string& name, double price)
    : tickerSymbol(ticker), companyName(name), currentPrice(price) {}

std::string Stock::getTicker() const {
    return tickerSymbol;
}

std::string Stock::getCompanyName() const {
    return companyName;
}

double Stock::getCurrentPrice() const {
    return currentPrice;
}

void Stock::updatePrice(double newPrice) {
    currentPrice = newPrice;
}