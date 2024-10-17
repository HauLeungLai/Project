CXX = g++
CXXFLAGS = -std=c++17 -I/opt/homebrew/Cellar/sfml/2.6.1/include
LDFLAGS = -L/opt/homebrew/Cellar/sfml/2.6.1/lib
LDLIBS = -lsfml-graphics -lsfml-window -lsfml-system -lcurl -lncurses

SOURCES = main.cpp marketData.cpp portfolio.cpp stock.cpp stockDisplay.cpp stockFetcher.cpp user.cpp financialAsset.cpp optionStock.cpp
OBJECTS = $(SOURCES:.cpp=.o)
TARGET = stock

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(LDFLAGS) -o $@ $^ $(LDLIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET)

.PHONY: all clean