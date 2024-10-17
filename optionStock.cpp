#include "optionStock.h"

// Constructor for OptionStock class
OptionStock::OptionStock(const std::string& ticker, const std::string& name, double price, double strikePrice, const std::string& expirationDate)
    : Stock(ticker, name, price), strikePrice(strikePrice), expirationDate(expirationDate) {}

// Getter for the strike price
double OptionStock::getStrikePrice() const {
    return strikePrice;
}

// Getter for the expiration date
std::string OptionStock::getExpirationDate() const {
    return expirationDate;
}

// Overriding getAssetType() to return "Option"
std::string OptionStock::getAssetType() const {
    return "Option";
}
