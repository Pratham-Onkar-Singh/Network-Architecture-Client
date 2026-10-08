CXX = g++
CXXFLAGS = -std=c++17 -Wall -O2

.PHONY: all clean

all: bcurl

bcurl: $(wildcard src/*.cpp) $(wildcard src/*.h)
	$(CXX) $(CXXFLAGS) -o bcurl $(wildcard src/*.cpp)

clean:
	rm -f bcurl
