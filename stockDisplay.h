#ifndef STOCKDISPLAY_H
#define STOCKDISPLAY_H

#include <vector>
#include "stockFetcher.h"  // Include stockFetcher to get access to StockData

// Function to display the stock candlestick chart using SFML
void displayStockChartSFML(const std::vector<StockData>& stockDataList);

#endif
