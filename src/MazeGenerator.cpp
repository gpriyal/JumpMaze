#include "MazeGenerator.h"
#include "PathValidator.h"
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <random>
#include <set>
using namespace std;

MazeGenerator::MazeGenerator(int n)
    : n(n), start({0, 0}), goal({n - 1, n - 1}) {
    maze = vector<vector<int>>(n, vector<int>(n, 0));
}

bool MazeGenerator::isValid(int x, int y) {
    return x >= 0 && y >= 0 && x < n && y < n;
}

// Phase 1: Build one unique path
bool MazeGenerator::generateUniquePath() {
    maze.assign(n, vector<int>(n, 0));
    canonicalPath.clear();
    canonicalPath.push_back(start);

    int x = start.first;
    int y = start.second;

    srand(static_cast<unsigned>(time(nullptr)));
    static int dirs[4][2] = {{1,0},{-1,0},{0,1},{0,-1}};

    while (make_pair(x, y) != goal) {
        vector<int> order = {0, 1, 2, 3};
        shuffle(order.begin(), order.end(), std::mt19937(rand()));

        bool moved = false;
        for (int dir : order) {
            int dx = dirs[dir][0], dy = dirs[dir][1];
            for (int step = 1; step <= 5; step++) {
                int nx = x + dx * step;
                int ny = y + dy * step;
                if (isValid(nx, ny) && maze[nx][ny] == 0) {
                    maze[x][y] = step; // jump length to next valid cell
                    x = nx; y = ny;
                    canonicalPath.push_back({x, y});
                    moved = true;
                    break;
                }
            }
            if (moved) break;
        }

        if (!moved) return false; 
    }

    maze[goal.first][goal.second] = -1;
    return true;
}

// Phase 2: Fill Dead Cells
void MazeGenerator::fillDeadCells() {
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (maze[i][j] == 0) {
                int val = rand() % (n+1); // 0 to n
                maze[i][j] = val;
            }
        }
    }
}

// Phase 3: Verify Uniqueness
bool MazeGenerator::verifyUniqueness() {
    auto canonical = PathValidator::getAnyPath(maze, start, goal);
    if (canonical.empty()) return false; 

    auto alt = PathValidator::getAnotherPath(maze, start, goal, canonical);
    return alt.empty(); 
}


void MazeGenerator::generateMaze() {
    const int MAX_ATTEMPTS = 10;
    int attempt = 0;
    bool success = false;

    while (attempt++ < MAX_ATTEMPTS) {
        if (!generateUniquePath()) continue;
        fillDeadCells();

        if (verifyUniqueness()) {
            success = true;
            break;
        }
    }

    if (!success)
        cerr << "Warning: Maze generation failed to produce a unique path after " << MAX_ATTEMPTS << " attempts.\n";
}

void MazeGenerator::displayMaze() const {
    cout << "\nGenerated Maze:\n";
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (maze[i][j] == -1) cout << "X ";
            else cout << maze[i][j] << " ";
        }
        cout << endl;
    }
}

vector<vector<int>> MazeGenerator::getMaze() const { return maze; }
pair<int,int> MazeGenerator::getStart() const { return start; }
pair<int,int> MazeGenerator::getGoal() const { return goal; }
