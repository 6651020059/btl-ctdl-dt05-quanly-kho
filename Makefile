CXX      = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Iinclude
SRC      = $(wildcard src/*.cpp)
OUT      = build/quanly

all: $(OUT)

$(OUT): $(SRC) $(wildcard include/*.h)
	mkdir -p build
	$(CXX) $(CXXFLAGS) $(SRC) -o $(OUT)

run: all
	./$(OUT)

clean:
	rm -rf build

.PHONY: all run clean
