#include "GameEngine.h"
#include <iostream>
using namespace std;

GameEngine::GameEngine(const vector<vector<int>>& m, pair<int,int> s, pair<int,int> g)
    : maze(m), pos(s), goal(g), n((int)m.size()), lost(false) {}

void GameEngine::reset() {
    pos = {0, 0};
    lost = false;
}

pair<int,int> GameEngine::getPosition() const { return pos; }
bool GameEngine::isGoalReached() const { return pos == goal; }
bool GameEngine::isLost() const { return lost; }

int GameEngine::getCurrentJump() const {
    if (pos.first < 0 || pos.second < 0 || pos.first >= n || pos.second >= n)
        return 0;
    return maze[pos.first][pos.second];
}

bool GameEngine::hasAnyValidMove() const {
    int jump = getCurrentJump();
    if (jump <= 0) return false; // jump==0 or negative means no moves

    int x = pos.first, y = pos.second;
    int dirs[4][2] = {{-jump,0},{jump,0},{0,-jump},{0,jump}}; // up, down, left, right

    for (auto& d : dirs) {
        int nx = x + d[0], ny = y + d[1];
        if (nx >= 0 && ny >= 0 && nx < n && ny < n)
            return true;
    }
    return false;
}

bool GameEngine::attemptMove(const string& direction, int steps) {
    if (lost || isGoalReached()) return false;

    int jump = getCurrentJump();

    if (jump == 0) {  // if current cell is 0: player can't move and loses
        lost = !hasAnyValidMove();
        return false;
    }

    if (steps > jump) {  // Invalid (steps exceed jump)
        return false;
    }

    int nx = pos.first;
    int ny = pos.second;

    if (direction == "up") nx -= steps;
    else if (direction == "down") nx += steps;
    else if (direction == "left") ny -= steps;
    else if (direction == "right") ny += steps;
    else return false;

    if (nx < 0 || ny < 0 || nx >= n || ny >= n) {  // Invalid (out of bounds)
        return false;
    }

    pos = {nx, ny};  // Valid move
    if (pos == goal) {  // Goal reached
        return true;
    }

    if (!hasAnyValidMove()) {
        lost = true;
    }

    return true;
}

bool GameEngine::moveUp()    { return attemptMove("up", getCurrentJump()); }
bool GameEngine::moveDown()  { return attemptMove("down", getCurrentJump()); }
bool GameEngine::moveLeft()  { return attemptMove("left", getCurrentJump()); }
bool GameEngine::moveRight() { return attemptMove("right", getCurrentJump()); }
