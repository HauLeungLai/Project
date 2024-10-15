#ifndef PORTFOLIO_H
#define PORTFOLIO_H

#include <string>
#include <map>

class Portfolio {
private:
    std::map<std::string, int> stocks;  // Stock ticker and quantity
    std::map<std::string, double> purchasePrices;  // Stock ticker and average purchase price
    double availableCash = 10000.0;  // Default starting cash

public:
    // Add stock to portfolio
    void addStock(const std::string& ticker, int quantity, double purchasePrice);

    // Sell stock from portfolio
    void sellStock(const std::string& ticker, int quantity);

    // Check if user has enough stock to sell
    bool hasStock(const std::string& ticker, int quantity);

    // Display the user's portfolio
    void displayPortfolio() const;

    // Load portfolio from a file
    void loadPortfolio(const std::string& filename);

    // Save portfolio to a file
    void savePortfolio(const std::string& filename) const;

    // Set available cash
    void setAvailableCash(double cash) { availableCash = cash; }

    // Get available cash
    double getAvailableCash() const { return availableCash; }

    void removeStock(const std::string& ticker, int quantity, double price);  
    
    bool hasStock(const std::string& ticker) const;  

};

#endif
