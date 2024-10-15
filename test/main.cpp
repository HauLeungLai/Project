#include <ncurses.h>
#include <iostream>
#include <string>
#include <vector>
#include "portfolio.h"
#include "stockFetcher.h"
#include "stockDisplay.h"

std::vector<std::string> availableStocks = {"AAPL", "TSLA", "GOOG", "MSFT"};

StockFetcher stockFetcher;  // StockFetcher instance (using test data for now)


// Function to display stock details (candlestick chart)
void viewStockDetails() {
    std::string ticker;
    clear();
    printw("Available Stocks:\n");
    for (const auto& stock : availableStocks) {
        printw("%s\n", stock.c_str());
    }
    printw("Enter stock ticker to view details: ");
    echo();
    char buffer[10];
    getnstr(buffer, 10);
    ticker = buffer;
    noecho();

    // Fetch stock data (using test data)
    std::vector<StockData> stockDataList = stockFetcher.fetchStockData(ticker, "2024-10-15", "2024-10-15");

    // Fetch the current price (using test data)
    double currentPrice = stockFetcher.fetchLatestPrice(ticker); 

    // Exit ncurses before launching SFML
    endwin();

    // Display the candlestick chart using SFML
    displayStockChartSFML(stockDataList, currentPrice);

    // Reinitialize ncurses after SFML window closes
    initscr();
}

int main() {
    // Initialize ncurses
    initscr();
    noecho();
    cbreak();

    int choice;
    while (true) {
        clear();
        printw("1. Buy Stock\n");
        printw("2. Sell Stock\n");
        printw("3. View Portfolio\n");
        printw("4. View Stock Details (Candlestick Chart)\n");
        printw("5. Exit\n");
        printw("Enter your choice: ");
        echo();
        scanw("%d", &choice);
        noecho();

        switch (choice) {
            case 1:
                // Call buyStock function (to be implemented)
                break;
            case 2:
                // Call sellStock function (to be implemented)
                break;
            case 3:
                // Call displayPortfolio function (to be implemented)
                break;
            case 4:
                viewStockDetails();  // Show candlestick chart
                break;
            case 5:
                endwin();  // Exit ncurses
                return 0;
            default:
                printw("Invalid choice. Try again.\n");
                getch();
        }
    }

    endwin();
    return 0;
}
