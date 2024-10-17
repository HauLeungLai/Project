#ifndef PORTFOLIO_H
#define PORTFOLIO_H

#include <string>
#include <map>
#include <memory>
#include "financialAsset.h"

class FinancialAsset;

class Portfolio {
private:
    double availableCash;
    std::map<std::string, std::unique_ptr<FinancialAsset>> assets;
    std::map<std::string, int> shares;  

public:
    Portfolio();

    void loadPortfolio(const std::string& filename);
    void savePortfolio(const std::string& filename) const;

    void addAsset(std::unique_ptr<FinancialAsset> asset, int quantity);
    void sellAsset(const std::string& symbol, int quantity);
    bool hasAsset(const std::string& symbol, int quantity) const;
    
    void displayPortfolio() const;

    double getAvailableCash() const;
    void setAvailableCash(double cash);
};

#endif