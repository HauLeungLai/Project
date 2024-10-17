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
        font.loadFromFile("arial.ttf"); 
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
    font.loadFromFile("arial.ttf");  
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
        std::string timeString = stock.date.substr(11, 5);  
        timeLabel.setString(timeString);
        timeLabel.setRotation(45);
        timeLabel.setPosition(x, MARGIN_TOP + CHART_HEIGHT + 10);
        window.draw(timeLabel);

        x += CHART_WIDTH / numLabels;
    }
}


void displayStockChartSFML(const std::vector<StockData>& stockDataList, double currentPrice, const std::string& ticker) {
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Stock Candlestick Chart: " + ticker);

    // Find min or max prices for scaling
    double minPrice = stockDataList[0].low;
    double maxPrice = stockDataList[0].high;
    for (const auto& stock : stockDataList) {
        minPrice = std::min(minPrice, stock.low);
        maxPrice = std::max(maxPrice, stock.high);
    }

    // Add padding to price range
    double priceRange = maxPrice - minPrice;
    minPrice -= priceRange * 0.05;
    maxPrice += priceRange * 0.05;
    priceRange = maxPrice - minPrice;

    double scale = CHART_HEIGHT / priceRange;

    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) {
        std::cerr << "Error loading font" << std::endl;
        return;
    }

    sf::RenderTexture chartTexture;
    if (!chartTexture.create(WINDOW_WIDTH, WINDOW_HEIGHT)) {
        std::cerr << "Error creating render texture" << std::endl;
        return;
    }

    chartTexture.clear(sf::Color::White);

    // Draw axes
    sf::Vertex xAxis[] = {
        sf::Vertex(sf::Vector2f(MARGIN_LEFT, MARGIN_TOP + CHART_HEIGHT), sf::Color::Black),
        sf::Vertex(sf::Vector2f(MARGIN_LEFT + CHART_WIDTH, MARGIN_TOP + CHART_HEIGHT), sf::Color::Black)
    };
    sf::Vertex yAxis[] = {
        sf::Vertex(sf::Vector2f(MARGIN_LEFT, MARGIN_TOP), sf::Color::Black),
        sf::Vertex(sf::Vector2f(MARGIN_LEFT, MARGIN_TOP + CHART_HEIGHT), sf::Color::Black)
    };
    chartTexture.draw(xAxis, 2, sf::Lines);
    chartTexture.draw(yAxis, 2, sf::Lines);

    // Draw price labels and  grid lines
    for (int i = 0; i <= 10; i++) {
        double price = minPrice + (priceRange * i / 10);
        double y = MARGIN_TOP + CHART_HEIGHT - (price - minPrice) * scale;
        
        sf::Text priceLabel;
        priceLabel.setFont(font);
        priceLabel.setString("$" + std::to_string(int(price)));
        priceLabel.setCharacterSize(12);
        priceLabel.setFillColor(sf::Color::Black);
        priceLabel.setPosition(MARGIN_LEFT - 60, y - 10);
        chartTexture.draw(priceLabel);

        sf::Vertex gridLine[] = {
            sf::Vertex(sf::Vector2f(MARGIN_LEFT, y), sf::Color(200, 200, 200)),
            sf::Vertex(sf::Vector2f(MARGIN_LEFT + CHART_WIDTH, y), sf::Color(200, 200, 200))
        };
        chartTexture.draw(gridLine, 2, sf::Lines);
    }

    // Draw candlesticks and X-axis labels
    float candleWidth = (CHART_WIDTH / stockDataList.size()) - 2;
    int labelInterval = std::max(1, static_cast<int>(stockDataList.size() / 10));
    for (size_t i = 0; i < stockDataList.size(); ++i) {
        const auto& stock = stockDataList[i];
        float x = MARGIN_LEFT + i * (candleWidth + 2);

        // Candlestick body
        double openY = MARGIN_TOP + CHART_HEIGHT - (stock.open - minPrice) * scale;
        double closeY = MARGIN_TOP + CHART_HEIGHT - (stock.close - minPrice) * scale;
        sf::RectangleShape body(sf::Vector2f(candleWidth, std::abs(closeY - openY)));
        body.setPosition(x, std::min(openY, closeY));
        body.setFillColor(stock.close >= stock.open ? sf::Color::Green : sf::Color::Red);
        chartTexture.draw(body);

        // Candlestick wick
        double highY = MARGIN_TOP + CHART_HEIGHT - (stock.high - minPrice) * scale;
        double lowY = MARGIN_TOP + CHART_HEIGHT - (stock.low - minPrice) * scale;
        sf::Vertex wick[] = {
            sf::Vertex(sf::Vector2f(x + candleWidth / 2, highY), sf::Color::Black),
            sf::Vertex(sf::Vector2f(x + candleWidth / 2, lowY), sf::Color::Black)
        };
        chartTexture.draw(wick, 2, sf::Lines);

        // X-axis labels
        if (i % labelInterval == 0) {
            sf::Text dateLabel;
            dateLabel.setFont(font);
            dateLabel.setString(stock.date.substr(0, 10)); 
            dateLabel.setCharacterSize(12);
            dateLabel.setFillColor(sf::Color::Black);
            dateLabel.setPosition(x, MARGIN_TOP + CHART_HEIGHT + 10);
            dateLabel.setRotation(45);
            chartTexture.draw(dateLabel);

            // Vertical grid line
            sf::Vertex gridLine[] = {
                sf::Vertex(sf::Vector2f(x, MARGIN_TOP), sf::Color(220, 220, 220)),
                sf::Vertex(sf::Vector2f(x, MARGIN_TOP + CHART_HEIGHT), sf::Color(220, 220, 220))
            };
            chartTexture.draw(gridLine, 2, sf::Lines);
        }
    }

    // Draw title
    sf::Text title;
    title.setFont(font);
    title.setString("Stock Candlestick Chart: " + ticker);
    title.setCharacterSize(24);
    title.setFillColor(sf::Color::Black);
    title.setPosition(WINDOW_WIDTH / 2 - title.getLocalBounds().width / 2, 10);
    chartTexture.draw(title);

    chartTexture.display();

    // Create a sprite from the render texture
    sf::Sprite chartSprite(chartTexture.getTexture());

    // Main loop
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear(sf::Color::White);

        // Draw the chart sprite
        window.draw(chartSprite);

        // Draw current price line (this needs to be drawn every frame as it may change)
        double currentPriceY = MARGIN_TOP + CHART_HEIGHT - (currentPrice - minPrice) * scale;
        sf::Vertex currentPriceLine[] = {
            sf::Vertex(sf::Vector2f(MARGIN_LEFT, currentPriceY), sf::Color::Blue),
            sf::Vertex(sf::Vector2f(MARGIN_LEFT + CHART_WIDTH, currentPriceY), sf::Color::Blue)
        };
        window.draw(currentPriceLine, 2, sf::Lines);

        // Draw current price label
        sf::Text currentPriceLabel;
        currentPriceLabel.setFont(font);
        std::ostringstream ss;
        ss << std::fixed << std::setprecision(2) << "Current: $" << currentPrice;
        currentPriceLabel.setString(ss.str());
        currentPriceLabel.setCharacterSize(14);
        currentPriceLabel.setFillColor(sf::Color::Blue);
        currentPriceLabel.setPosition(MARGIN_LEFT + CHART_WIDTH + 10, currentPriceY - 10);
        window.draw(currentPriceLabel);

        window.display();
    }
}