CXX      := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -Iinclude
SRC      := $(wildcard src/*.cpp)
OBJ      := $(patsubst src/%.cpp,build/%.o,$(SRC))
BIN      := fishz

.PHONY: all run clean

all: $(BIN)

$(BIN): $(OBJ)
	$(CXX) $(CXXFLAGS) $^ -o $@

build/%.o: src/%.cpp | build
	$(CXX) $(CXXFLAGS) -c $< -o $@

build:
	mkdir -p build

run: $(BIN)
	./$(BIN)

clean:
	rm -rf build $(BIN)
