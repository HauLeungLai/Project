#include "financialAsset.h"

// Constructor for FinancialAsset
FinancialAsset::FinancialAsset(const std::string& symbol, double initialPrice)
    : symbol(symbol), price(initialPrice) {}

// Method to get the symbol of the asset
std::string FinancialAsset::getSymbol() const {
    return symbol;
}

// Method to get the price of the asset
double FinancialAsset::getPrice() const {
    return price;
}

// Method to update the price of the asset
void FinancialAsset::updatePrice(double newPrice) {
    price = newPrice;
}

// Overloaded operator<< to print the FinancialAsset details
std::ostream& operator<<(std::ostream& os, const FinancialAsset& asset) {
    os << "Symbol: " << asset.getSymbol() << ", Price: $" << asset.getPrice();
    return os;
}