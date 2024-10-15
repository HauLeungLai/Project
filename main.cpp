#include <ncurses.h>
#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <string>
#include "portfolio.h"
#include "stockFetcher.h"
#include "user.h"
#include "stockDisplay.h"

// Global stock vector
const std::vector<std::string> AVAILABLE_STOCKS = {"AAPL", "TSLA", "GOOG", "MSFT", "AMZN", "NFLX", "NVDA", "INTC", "AMD"};

Portfolio portfolio;
StockFetcher stockFetcher("3af9ca15a9f431b11e79114e5fb172e9f00f716d");  
std::map<std::string, std::string> users;  // Global user storage

// Function to display available stocks with their current prices
void displayAvailableStocks() {
    printw("Available Stocks:\n");
    for (const auto& stock : AVAILABLE_STOCKS) {
        try {
            double price = stockFetcher.fetchLatestPrice(stock);
            printw("%s: $%.2f\n", stock.c_str(), price);
        } catch (const std::exception& e) {
            printw("%s: Error fetching price.\n", stock.c_str());
        }
    }
    printw("\n");
}

// Function to buy stock
void buyStock(const std::string& currentUser) {
    std::string ticker;
    int quantity;
    char choice;

    clear();
    displayAvailableStocks();
    printw("Enter stock ticker to buy: ");
    echo();
    char buffer[10];
    getnstr(buffer, 10);
    ticker = buffer;
    noecho();

    if (std::find(AVAILABLE_STOCKS.begin(), AVAILABLE_STOCKS.end(), ticker) == AVAILABLE_STOCKS.end()) {
        printw("Invalid stock ticker.\n");
        getch();
        return;
    }

    double stockPrice;
    try {
        stockPrice = stockFetcher.fetchLatestPrice(ticker);
        printw("Current price of %s: $%.2f\n", ticker.c_str(), stockPrice);
    } catch (const std::exception& e) {
        printw("Error fetching stock price: %s\n", e.what());
        getch();
        return;
    }

    printw("Buy at market price (m) or limit price (l)? ");
    choice = getch();
    double priceToBuy = stockPrice;
    if (choice == 'l') {
        printw("\nEnter limit price: ");
        scanw("%lf", &priceToBuy);
    }

    if (priceToBuy >= stockPrice) {
        printw("\nLimit price exceeded market price. Buying at market price $%.2f.\n", stockPrice);
        priceToBuy = stockPrice;
    }

    printw("Enter quantity: ");
    scanw("%d", &quantity);

    double totalCost = priceToBuy * quantity;
    if (portfolio.getAvailableCash() >= totalCost) {
        portfolio.addStock(ticker, quantity, priceToBuy);
        portfolio.setAvailableCash(portfolio.getAvailableCash() - totalCost);
        printw("Successfully bought %d shares of %s.\n", quantity, ticker.c_str());
    } else {
        printw("Insufficient funds.s\n");
    }

    portfolio.savePortfolio(currentUser + "_portfolio.dat");
    getch();
}

// Function to sell stock
void sellStock(const std::string& currentUser) {
    std::string ticker;
    int quantity;

    clear();
    displayAvailableStocks();
    printw("Enter stock ticker to sell: ");
    echo();
    char buffer[10];
    getnstr(buffer, 10);
    ticker = buffer;
    noecho();

    if (std::find(AVAILABLE_STOCKS.begin(), AVAILABLE_STOCKS.end(), ticker) == AVAILABLE_STOCKS.end()) {
        printw("Invalid stock ticker.\n");
        getch();
        return;
    }

    printw("Enter quantity: ");
    scanw("%d", &quantity);

    if (!portfolio.hasStock(ticker, quantity)) {
        printw("Insufficient shares to sell.\n");
        getch();
        return;
    }

    double stockPrice;
    try {
        stockPrice = stockFetcher.fetchLatestPrice(ticker);
        printw("Current price of %s: $%.2f\n", ticker.c_str(), stockPrice);
    } catch (const std::exception& e) {
        printw("Error fetching stock price: %s\n", e.what());
        getch();
        return;
    }

    double totalValue = stockPrice * quantity;
    portfolio.sellStock(ticker, quantity);
    portfolio.setAvailableCash(portfolio.getAvailableCash() + totalValue);
    printw("Successfully sold %d shares of %s.\n", quantity, ticker.c_str());

    portfolio.savePortfolio(currentUser + "_portfolio.dat");
    getch();
}


// Function to display stock chart
void displayStockChart() {
    clear();
    printw("Enter stock ticker to display chart: ");
    char buffer[10];
    echo();
    getnstr(buffer, 10);
    noecho();
    std::string ticker = buffer;

    printw("Select timeframe (D)aily, (W)eekly, or (M)onthly: ");
    char timeframeChoice = getch();
    std::string startDate, endDate;
    std::string timeframe;

    switch (toupper(timeframeChoice)) {
        case 'D':
            startDate = "2023-10-01";
            endDate = "2023-10-30";
            timeframe = "daily";
            break;
        case 'W':
            startDate = "2023-05-01";
            endDate = "2023-10-30";
            timeframe = "weekly";
            break;
        case 'M':
            startDate = "2022-10-01";
            endDate = "2023-10-30";
            timeframe = "monthly";
            break;
        default:
            printw("Invalid choice. Defaulting to daily view.\n");
            startDate = "2023-10-01";
            endDate = "2023-10-30";
            timeframe = "daily";
    }

    try {
        std::vector<StockData> stockData = stockFetcher.fetchStockData(ticker, startDate, endDate, timeframe);        double currentPrice = stockFetcher.fetchLatestPrice(ticker);
        displayStockChartSFML(stockData, currentPrice, ticker);
    } catch (const std::exception& e) {
        printw("Error fetching stock data: %s\n", e.what());
        getch();
    }
}

// Main menu after login
void dashboard(const std::string& currentUser) {
    int choice;
    do {
        clear();
        printw("1. View Portfolio\n");
        printw("2. Buy Stock\n");
        printw("3. Sell Stock\n");
        printw("4. View Available Stocks\n");
        printw("5. Display Stock Chart\n");
        printw("6. Logout\n");
        printw("Enter your choice: ");
        echo();  
        scanw("%d", &choice);
        noecho();  
        switch (choice) {
            case 1:
                portfolio.displayPortfolio();
                break;
            case 2:
                buyStock(currentUser);
                break;
            case 3:
                sellStock(currentUser);
                break;
            case 4:
                displayAvailableStocks();
                getch();
                break;
            case 5:
                displayStockChart();
                break;
            case 6:
                portfolio.savePortfolio(currentUser + "_portfolio.dat");
                return;
            default:
                printw("Invalid option. Try again.\n");
                getch();
        }
    } while (choice != 6);
}

// Entry point of the program
int main() {
    initscr();
    cbreak();  
    keypad(stdscr, TRUE);  

    // Load user data from file
    loadUsersFromFile(users);

    // Login or Register loop
    std::string currentUser;
    int choice;
    do {
        clear();
        printw("Welcome to Stock Market Simulator\n");
        printw("1. Login\n");
        printw("2. Register\n");
        printw("3. Exit\n");
        printw("Enter your choice: ");
        echo();  
        scanw("%d", &choice);
        noecho();  
        switch (choice) {
            case 1:
                if (loginUser(users, currentUser)) {
                    dashboard(currentUser);  // Show dashboard after successful login
                }
                break;
            case 2:
                registerUser(users);
                break;
            case 3:
                endwin();  // Cleanup ncurses
                return 0;  // Exit the program
            default:
                printw("Invalid choice. Try again.\n");
                getch();
        }
    } while (choice != 3);

    endwin();  // Cleanup ncurses
    return 0;
}