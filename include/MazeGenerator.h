#ifndef MAZE_GENERATOR_H
#define MAZE_GENERATOR_H

#include <vector>
#include <utility>
using namespace std;

class MazeGenerator {
private:
    int n;
    vector<vector<int>> maze;
    pair<int, int> start;
    pair<int, int> goal;

    bool isValid(int x, int y);
    bool generateUniquePath();                   
    void fillDeadCells();                         
    bool verifyUniqueness();                     
    vector<pair<int,int>> canonicalPath;

public:
    MazeGenerator(int n);
    void generateMaze();
    void displayMaze() const;
    vector<vector<int>> getMaze() const;
    pair<int,int> getStart() const;
    pair<int,int> getGoal() const;
};

#endif 
