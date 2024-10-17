#include "portfolio.h"
#include "financialAsset.h"
#include "optionStock.h"
#include "stock.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <stdexcept>
#include <ncurses.h>

// Constructor
Portfolio::Portfolio() : availableCash(10000.0) {} 

// Load portfolio from file
void Portfolio::loadPortfolio(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Unable to open file for reading: " + filename);
        availableCash = 10000.0;
        std::cout << "Loaded portfolio. Available cash: $" << availableCash << std::endl;
        return;
    }

    file >> availableCash;

    assets.clear();
    shares.clear();

    std::string symbol;
    double price;
    int quantity;
    while (file >> symbol >> price >> quantity) {
        assets[symbol] = std::make_unique<Stock>(symbol, symbol, price);
        shares[symbol] = quantity;
    }
    file.close();
}

// Save portfolio to file
void Portfolio::savePortfolio(const std::string& filename) const {
    std::ofstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Unable to open file for writing: " + filename);
    }
    
    file << availableCash << std::endl;


    for (const auto& [symbol, asset] : assets) {
        file << symbol << " " << asset->getPrice() << " " << shares.at(symbol) << std::endl;
    }
    file.close();
}

// Function to add an asset to the portfolio
void Portfolio::addAsset(std::unique_ptr<FinancialAsset> asset, int quantity) {
    if (!asset) {
        throw std::runtime_error("Attempting to add a null asset to portfolio");
    }
    
    std::string symbol = asset->getSymbol();
    
    // Check if we already have this asset
    auto it = assets.find(symbol);
    if (it != assets.end()) {
        // Update existing asset
        it->second->updatePrice(asset->getPrice());
        shares[symbol] += quantity;
    } else {
        // Add new asset
        assets[symbol] = std::move(asset);
        shares[symbol] = quantity;
    }
}

// Function to sell an asset from the portfolio
void Portfolio::sellAsset(const std::string& symbol, int quantity) {
    if (hasAsset(symbol, quantity)) {
        shares[symbol] -= quantity;
        if (shares[symbol] <= 0) {
            assets.erase(symbol);
            shares.erase(symbol);
        }
    }
}

// Function to check if the portfolio has the required quantity of an asset
bool Portfolio::hasAsset(const std::string& symbol, int quantity) const {
    auto it = shares.find(symbol);
    return it != shares.end() && it->second >= quantity;
}

// Display the portfolio
void Portfolio::displayPortfolio() const {
    clear();
    int row = 0;
    mvprintw(row++, 0, "Your Portfolio:");
    mvprintw(row++, 0, "-----------------------------");
    
    if (assets.empty()) {
        mvprintw(row++, 0, "No assets in portfolio.");
    } else {
        mvprintw(row++, 0, "%-10s %-10s %-10s", "Symbol", "Quantity", "Price");
        for (const auto& [symbol, asset] : assets) {
            mvprintw(row++, 0, "%-10s %-10d $%-9.2f", 
                     symbol.c_str(), 
                     shares.at(symbol), 
                     asset->getPrice());
        }
    }
    
    mvprintw(row++, 0, "-----------------------------");
    mvprintw(row++, 0, "Available Cash: $%.2f", availableCash);
    
    refresh();
    getch();
}

// Get available cash
double Portfolio::getAvailableCash() const {
    return availableCash;
}

// Set available cash
void Portfolio::setAvailableCash(double cash) {
    availableCash = cash;
}