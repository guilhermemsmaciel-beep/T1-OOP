CXX      := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -Iinclude
SRC      := $(wildcard src/*.cpp)
OBJ      := $(patsubst src/%.cpp,build/%.o,$(SRC))
BIN      := fishz

.PHONY: all run test clean

all: $(BIN)

$(BIN): $(OBJ)
	$(CXX) $(CXXFLAGS) $^ -o $@

build/%.o: src/%.cpp | build
	$(CXX) $(CXXFLAGS) -c $< -o $@

build:
	mkdir -p build

run: $(BIN)
	./$(BIN)

# Testes: compila tudo menos o main.cpp junto com tests/testes.cpp
test: $(filter-out build/main.o,$(OBJ)) tests/testes.cpp
	$(CXX) $(CXXFLAGS) $^ -o build/testes
	./build/testes

clean:
	rm -rf build $(BIN)
