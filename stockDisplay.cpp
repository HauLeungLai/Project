#include "stockDisplay.h"
#include <SFML/Graphics.hpp>
#include <algorithm>
#include <sstream>
#include <iomanip>
#include <iostream>

const int WINDOW_WIDTH = 1200;
const int WINDOW_HEIGHT = 800;
const int CHART_WIDTH = 1000;
const int CHART_HEIGHT = 600;
const int MARGIN_LEFT = 100;
const int MARGIN_TOP = 50;

void drawGrid(sf::RenderWindow& window, double minPrice, double maxPrice, int numHorizontalLines) {
    double priceStep = (maxPrice - minPrice) / (numHorizontalLines - 1);
    
    for (int i = 0; i < numHorizontalLines; ++i) {
        double y = MARGIN_TOP + CHART_HEIGHT - (i * CHART_HEIGHT / (numHorizontalLines - 1));
        
        sf::Vertex line[] = {
            sf::Vertex(sf::Vector2f(MARGIN_LEFT, y), sf::Color(200, 200, 200)),
            sf::Vertex(sf::Vector2f(MARGIN_LEFT + CHART_WIDTH, y), sf::Color(200, 200, 200))
        };
        window.draw(line, 2, sf::Lines);
        
        double price = minPrice + i * priceStep;
        sf::Text priceLabel;
        sf::Font font;
        font.loadFromFile("arial.ttf");  // Make sure to have this font file in your project directory
        priceLabel.setFont(font);
        priceLabel.setCharacterSize(12);
        priceLabel.setFillColor(sf::Color::Black);
        std::ostringstream ss;
        ss << std::fixed << std::setprecision(2) << price;
        priceLabel.setString(ss.str());
        priceLabel.setPosition(MARGIN_LEFT - 70, y - 10);
        window.draw(priceLabel);
    }
}

void drawXAxisLabels(sf::RenderWindow& window, const std::vector<StockData>& stockDataList, int numLabels) {
    sf::Font font;
    font.loadFromFile("arial.ttf");  // Make sure to have this font file in your project directory

    float labelStep = static_cast<float>(stockDataList.size()) / numLabels;
    float x = MARGIN_LEFT;

    for (int i = 0; i < numLabels; ++i) {
        int dataIndex = static_cast<int>(i * labelStep);
        const auto& stock = stockDataList[dataIndex];

        sf::Text timeLabel;
        timeLabel.setFont(font);
        timeLabel.setCharacterSize(10);
        timeLabel.setFillColor(sf::Color::Black);
        
        // Convert timestamp to hours:minutes format
        std::string timeString = stock.date.substr(11, 5);  // Extract time (HH:MM) from timestamp
        timeLabel.setString(timeString);
        timeLabel.setRotation(45);
        timeLabel.setPosition(x, MARGIN_TOP + CHART_HEIGHT + 10);
        window.draw(timeLabel);

        x += CHART_WIDTH / numLabels;
    }
}

void displayStockChartSFML(const std::vector<StockData>& stockDataList) {
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Stock Candlestick Chart");

    // Find min/max prices for scaling
    double minPrice = stockDataList[0].low;
    double maxPrice = stockDataList[0].high;
    for (const auto& stock : stockDataList) {
        minPrice = std::min(minPrice, stock.low);
        maxPrice = std::max(maxPrice, stock.high);
    }

    // Add extra padding to min and max price to prevent the chart from being too zoomed in
    double priceRange = maxPrice - minPrice;
    minPrice -= priceRange * 0.05;  // Adding 5% padding to the bottom
    maxPrice += priceRange * 0.05;  // Adding 5% padding to the top

    priceRange = maxPrice - minPrice;
    double scale = CHART_HEIGHT / priceRange;

    sf::Font font;
    font.loadFromFile("arial.ttf");  // Ensure the font file is present in your project

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear(sf::Color::White);

        // Draw grid and Y-axis price labels
        drawGrid(window, minPrice, maxPrice, 10);

        // Draw candlesticks
        float x = MARGIN_LEFT;
        float width = (CHART_WIDTH / stockDataList.size()) - 4;  // Adjust width for spacing between candlesticks
        for (size_t i = 0; i < stockDataList.size(); ++i) {
            const auto& stock = stockDataList[i];

            // Body (open-close)
            sf::RectangleShape candleBody;
            double openY = MARGIN_TOP + CHART_HEIGHT - (stock.open - minPrice) * scale;
            double closeY = MARGIN_TOP + CHART_HEIGHT - (stock.close - minPrice) * scale;

            candleBody.setPosition(x, std::min(openY, closeY));
            candleBody.setSize(sf::Vector2f(width, std::abs(openY - closeY)));
            candleBody.setFillColor(stock.close >= stock.open ? sf::Color::Green : sf::Color::Red);

            // High-low line
            sf::RectangleShape highLowLine;
            double highY = MARGIN_TOP + CHART_HEIGHT - (stock.high - minPrice) * scale;
            double lowY = MARGIN_TOP + CHART_HEIGHT - (stock.low - minPrice) * scale;

            highLowLine.setPosition(x + width / 2, highY);
            highLowLine.setSize(sf::Vector2f(1, lowY - highY));
            highLowLine.setFillColor(sf::Color::Black);

            window.draw(highLowLine);
            window.draw(candleBody);

            x += width + 4;  // Spacing between candlesticks
        }

        // Draw X-axis time labels
        drawXAxisLabels(window, stockDataList, 6);  // Display 6 labels evenly spaced along the X-axis

        // Draw title
        sf::Text title;
        title.setFont(font);
        title.setString("Stock Candlestick Chart");
        title.setCharacterSize(24);
        title.setFillColor(sf::Color::Black);
        title.setPosition(WINDOW_WIDTH / 2 - title.getLocalBounds().width / 2, 10);
        window.draw(title);

        window.display();
    }
}
