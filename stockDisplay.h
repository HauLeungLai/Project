#ifndef STOCKDISPLAY_H
#define STOCKDISPLAY_H

#include <vector>
#include "stockFetcher.h"

// Function to display the stock candlestick chart using SFML
void displayStockChartSFML(const std::vector<StockData>& stockDataList, double currentPrice, const std::string& ticker);
#endif