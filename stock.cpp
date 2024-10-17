#include "stock.h"

Stock::Stock(const std::string& ticker, const std::string& name, double price)
    : FinancialAsset(ticker, price), companyName(name) {}

std::string Stock::getCompanyName() const {
    return companyName;
}

std::string Stock::getAssetType() const {
    return "Stock";
}