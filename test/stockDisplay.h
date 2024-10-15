#ifndef STOCKDISPLAY_H
#define STOCKDISPLAY_H

#include <SFML/Graphics.hpp>
#include <vector>
#include "stockFetcher.h"  // Include stockFetcher.h to use StockData

// Function to display the candlestick chart
void displayStockChartSFML(const std::vector<StockData>& stockDataList, double currentPrice);

// Function to draw grid lines and Y-axis labels (price)
void drawGrid(sf::RenderWindow& window, double minPrice, double maxPrice, int numHorizontalLines);

// Function to draw X-axis labels (time by hours)
void drawXAxisLabels(sf::RenderWindow& window, int numHours);

// Function to draw the current price line
void drawCurrentPriceLine(sf::RenderWindow& window, double currentPrice, double minPrice, double maxPrice);

#endif
