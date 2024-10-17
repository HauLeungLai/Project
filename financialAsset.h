#ifndef FINANCIALASSET_H
#define FINANCIALASSET_H

#include <string>
#include <iostream>

class FinancialAsset {
protected:
    std::string symbol;
    double price;

public:
    // Constructor
    FinancialAsset(const std::string& symbol, double initialPrice);

    // Getter for symbol
    std::string getSymbol() const;

    // Getter for price
    virtual double getPrice() const;

    // Setter for price
    virtual void updatePrice(double newPrice);

    // Pure virtual function for asset type (forces derived classes to implement this)
    virtual std::string getAssetType() const = 0;

    // Friend function to display asset information
    friend std::ostream& operator<<(std::ostream& os, const FinancialAsset& asset);

    virtual ~FinancialAsset() = default;
};

#endif