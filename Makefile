CXX      ?= g++
CXXFLAGS ?= -std=c++17 -Wall -Wextra -O2
SRC       = $(wildcard src/*.cpp)

wish: $(SRC) $(wildcard src/*.h)
	$(CXX) $(CXXFLAGS) -o wish $(SRC)

clean:
	rm -f wish
	rm -rf build tests-out

.PHONY: clean
