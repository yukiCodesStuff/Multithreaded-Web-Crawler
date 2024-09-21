# Compiler
CXX = g++

# Flags
CXXFLAGS = -Wall -Wextra -std=c++11

# Targets
TARGET = main

# Object files
OBJECTS = main.o ./objects/WebCrawler.o ./objects/WebClientUrl.o

# Build the target
all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) -o $(TARGET) $(OBJECTS)

main.o: main.cpp
	$(CXX) $(CXXFLAGS) -c main.cpp

./objects/WebCrawler.o: ./objects/WebCrawler.cpp ./header/WebCrawler.h
	$(CXX) $(CXXFLAGS) -c ./objects/WebCrawler.cpp -o ./objects/WebCrawler.o

./objects/WebClientUrl.o: ./objects/WebClientUrl.cpp ./header/WebClientUrl.h
	$(CXX) $(CXXFLAGS) -c ./objects/WebClientUrl.cpp -o ./objects/WebClientUrl.o

# Clean up
clean:
	rm -f *.o ./objects/*.o $(TARGET)

# Test execution
test: $(TARGET)
	./$(TARGET) 5 ./inputFiles/1-Mil.txt

testHun: $(TARGET)
	./$(TARGET) 5 ./inputFiles/100.txt

testUrl: $(TARGET)
	./$(TARGET) 1 ./inputFiles/URLtest.txt
