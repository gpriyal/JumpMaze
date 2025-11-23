CXX = g++
SRC = src/MazeGenerator.cpp src/PathValidator.cpp src/GameEngine.cpp src/main.cpp
OUT = JumpMaze

# paths to SFML
SFML_INCLUDE = C:/Users/priya/Downloads/SFML/SFML-2.5.1/include 
SFML_LIB = C:/Users/priya/Downloads/SFML/SFML-2.5.1/lib

# compiler flags
CXXFLAGS = -Iinclude -I$(SFML_INCLUDE)
LDFLAGS = -L$(SFML_LIB) -lsfml-graphics -lsfml-window -lsfml-system

all:
	$(CXX) $(SRC) -o $(OUT).exe $(CXXFLAGS) $(LDFLAGS)

run: all
	./$(OUT).exe

clean:
	rm -f $(OUT).exe

