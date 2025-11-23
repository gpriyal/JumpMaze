#ifndef GAME_ENGINE_H
#define GAME_ENGINE_H

#include <vector>
#include <string>
#include <utility>
using namespace std;

class GameEngine {
private:
    vector<vector<int>> maze;
    pair<int,int> pos;
    pair<int,int> goal;
    int n;
    bool lost = false;

public:
    GameEngine(const vector<vector<int>>& maze, pair<int,int> start, pair<int,int> goal);
    void reset();
    pair<int,int> getPosition() const;
    bool isGoalReached() const;
    bool isLost() const;

    bool attemptMove(const string& direction, int steps);
    bool moveUp();
    bool moveDown();
    bool moveLeft();
    bool moveRight();

    int getCurrentJump() const;
    bool hasAnyValidMove() const;
};

#endif 
