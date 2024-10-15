# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++11 -Wall

# Include paths
INCLUDES = -I/opt/homebrew/Cellar/sfml/2.6.1/include -Iinclude

# Linker flags
LDFLAGS = -L/opt/homebrew/Cellar/sfml/2.6.1/lib -lsfml-graphics -lsfml-window -lsfml-system -lcurl -lncurses

# Target executable
TARGET = stock

# Source files
SRCS = main.cpp portfolio.cpp stockFetcher.cpp user.cpp

# Object files
OBJS = $(SRCS:.cpp=.o)

# Build target
$(TARGET): $(OBJS)
	$(CXX) -o $(TARGET) $(OBJS) $(LDFLAGS)

# Rule for compiling .cpp files to .o files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Clean up
clean:
	rm -f $(OBJS) $(TARGET)
