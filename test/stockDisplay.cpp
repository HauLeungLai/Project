#include "stockDisplay.h"
#include <SFML/Graphics.hpp>
#include <algorithm>
#include <sstream>
#include <iomanip>

const int WINDOW_WIDTH = 1200;
const int WINDOW_HEIGHT = 800;
const int CHART_WIDTH = 1000;
const int CHART_HEIGHT = 600;
const int MARGIN_LEFT = 100;
const int MARGIN_TOP = 50;
const int HOURS_IN_DAY = 24;

// Function to draw grid and Y-axis (price labels)
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
        font.loadFromFile("arial.ttf");  // Ensure you have this font
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

// Function to draw X-axis (hours)
void drawXAxisLabels(sf::RenderWindow& window, int numHours) {
    sf::Font font;
    font.loadFromFile("arial.ttf");

    float hourStep = CHART_WIDTH / numHours;
    for (int i = 0; i < numHours; ++i) {
        sf::Text timeLabel;
        timeLabel.setFont(font);
        timeLabel.setCharacterSize(12);
        timeLabel.setFillColor(sf::Color::Black);
        timeLabel.setString(std::to_string(i) + ":00");
        timeLabel.setPosition(MARGIN_LEFT + i * hourStep - 10, MARGIN_TOP + CHART_HEIGHT + 10);
        window.draw(timeLabel);
    }
}

// Function to draw the current price line
void drawCurrentPriceLine(sf::RenderWindow& window, double currentPrice, double minPrice, double maxPrice) {
    double y = MARGIN_TOP + CHART_HEIGHT - ((currentPrice - minPrice) / (maxPrice - minPrice)) * CHART_HEIGHT;
    sf::Vertex line[] = {
        sf::Vertex(sf::Vector2f(MARGIN_LEFT, y), sf::Color::Blue),
        sf::Vertex(sf::Vector2f(MARGIN_LEFT + CHART_WIDTH, y), sf::Color::Blue)
    };
    window.draw(line, 2, sf::Lines);

    sf::Text priceLabel;
    sf::Font font;
    font.loadFromFile("arial.ttf");
    priceLabel.setFont(font);
    priceLabel.setCharacterSize(12);
    priceLabel.setFillColor(sf::Color::Blue);
    std::ostringstream ss;
    ss << std::fixed << std::setprecision(2) << currentPrice;
    priceLabel.setString(ss.str());
    priceLabel.setPosition(MARGIN_LEFT + CHART_WIDTH + 10, y - 10);
    window.draw(priceLabel);
}

// Function to display the stock candlestick chart
void displayStockChartSFML(const std::vector<StockData>& stockDataList, double currentPrice) {
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Stock Candlestick Chart");

    double minPrice = stockDataList[0].low;
    double maxPrice = stockDataList[0].high;
    for (const auto& stock : stockDataList) {
        minPrice = std::min(minPrice, stock.low);
        maxPrice = std::max(maxPrice, stock.high);
    }

    minPrice -= (maxPrice - minPrice) * 0.05;  // Add padding to Y-axis
    maxPrice += (maxPrice - minPrice) * 0.05;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear(sf::Color::White);

        // Draw the grid and axes
        drawGrid(window, minPrice, maxPrice, 10);  // 10 horizontal lines for price
        drawXAxisLabels(window, HOURS_IN_DAY);     // X-axis labeled by hours

        // Draw the candlesticks
        float x = MARGIN_LEFT;
        float width = CHART_WIDTH / stockDataList.size() - 2;
        for (size_t i = 0; i < stockDataList.size(); ++i) {
            const auto& stock = stockDataList[i];

            // Draw the candlestick body (Open and Close prices)
            sf::RectangleShape candleBody;
            double openY = MARGIN_TOP + CHART_HEIGHT - ((stock.open - minPrice) / (maxPrice - minPrice)) * CHART_HEIGHT;
            double closeY = MARGIN_TOP + CHART_HEIGHT - ((stock.close - minPrice) / (maxPrice - minPrice)) * CHART_HEIGHT;
            candleBody.setPosition(x, std::min(openY, closeY));
            candleBody.setSize(sf::Vector2f(width, std::abs(openY - closeY)));
            candleBody.setFillColor(stock.close >= stock.open ? sf::Color::Green : sf::Color::Red);

            // Draw the high-low line
            sf::RectangleShape highLowLine;
            double highY = MARGIN_TOP + CHART_HEIGHT - ((stock.high - minPrice) / (maxPrice - minPrice)) * CHART_HEIGHT;
            double lowY = MARGIN_TOP + CHART_HEIGHT - ((stock.low - minPrice) / (maxPrice - minPrice)) * CHART_HEIGHT;
            highLowLine.setPosition(x + width / 2, highY);
            highLowLine.setSize(sf::Vector2f(1, lowY - highY));
            highLowLine.setFillColor(sf::Color::Black);

            window.draw(highLowLine);
            window.draw(candleBody);

            x += width + 2;
        }

        // Draw the current price line
        drawCurrentPriceLine(window, currentPrice, minPrice, maxPrice);

        window.display();
    }
}
