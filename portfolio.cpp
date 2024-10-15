#include "portfolio.h"
#include <fstream>
#include <iostream>
#include <ncurses.h>

void Portfolio::addStock(const std::string& ticker, int quantity, double purchasePrice) {
    if (stocks.find(ticker) != stocks.end()) {
        int totalQuantity = stocks[ticker] + quantity;
        purchasePrices[ticker] = ((stocks[ticker] * purchasePrices[ticker]) + (quantity * purchasePrice)) / totalQuantity;
        stocks[ticker] = totalQuantity;
    } else {
        stocks[ticker] = quantity;
        purchasePrices[ticker] = purchasePrice;
    }
}

void Portfolio::sellStock(const std::string& ticker, int quantity) {
    if (stocks.find(ticker) != stocks.end()) {
        if (stocks[ticker] >= quantity) {
            stocks[ticker] -= quantity;
            if (stocks[ticker] == 0) {
                stocks.erase(ticker);  // Remove stock if quantity becomes 0
                purchasePrices.erase(ticker); 
            }
        }
    }
}
void Portfolio::removeStock(const std::string& ticker, int quantity, double price) {
    if (stocks.find(ticker) != stocks.end()) {
        if (stocks[ticker] >= quantity) {
            stocks[ticker] -= quantity;
            // Add money back to the user's available cash
            availableCash += price * quantity;

            // Remove the stock if the quantity becomes 0
            if (stocks[ticker] == 0) {
                stocks.erase(ticker);
            }

            std::cout << "Sold " << quantity << " shares of " << ticker << " for $" << price * quantity << "\n";
        } else {
            std::cout << "You don't have enough shares to sell.\n";
        }
    } else {
        std::cout << "You don't own this stock.\n";
    }
}

bool Portfolio::hasStock(const std::string& ticker) const {
    return stocks.find(ticker) != stocks.end() && stocks.at(ticker) > 0;
}


bool Portfolio::hasStock(const std::string& ticker, int quantity) {
    return stocks.find(ticker) != stocks.end() && stocks[ticker] >= quantity;
}

void Portfolio::displayPortfolio() const {
    clear();
    printw("Your Portfolio:\n");
    printw("Stock\tQuantity\tAvg Purchase Price\n");
    for (const auto& stock : stocks) {
        printw("%s\t%d\t\t$%.2f\n", stock.first.c_str(), stock.second, purchasePrices.at(stock.first));
    }
    printw("\nAvailable Cash: $%.2f\n", availableCash);
    refresh();
    getch();
}

void Portfolio::loadPortfolio(const std::string& filename) {
    std::ifstream infile(filename);
    std::string ticker;
    int quantity;
    double price;
    if (infile.is_open()) {
        infile >> availableCash;  // Load available cash
        while (infile >> ticker >> quantity >> price) {
            stocks[ticker] = quantity;
            purchasePrices[ticker] = price;
        }
    }
}

void Portfolio::savePortfolio(const std::string& filename) const {
    std::ofstream outfile(filename);
    outfile << availableCash << std::endl;  // Save available cash
    for (const auto& stock : stocks) {
        outfile << stock.first << " " << stock.second << " " << purchasePrices.at(stock.first) << std::endl;
    }
}
