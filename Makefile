CXX = g++
CXXFLAGS = -std=c++17 -Iinclude -Wall

SRC = src/main.cpp src/arquivo.cpp src/utils.cpp src/formaPadrao.cpp src/simplex.cpp
OUT = simplex_app

all: $(OUT)

$(OUT): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(OUT)

run: $(OUT)
	./$(OUT)

clean:
	rm -f $(OUT)
