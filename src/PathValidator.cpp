#include "PathValidator.h"
#include <queue>
#include <vector>
#include <utility>
#include <set>
#include <algorithm>
using namespace std;

static vector<pair<int,int>> bfsExactJump(
    const vector<vector<int>>& maze,
    pair<int,int> start,
    pair<int,int> goal,
    const set<pair<int,int>>& exclude = {}
) {
    int n = (int)maze.size();
    if (n == 0) return {};

    vector<vector<bool>> visited(n, vector<bool>(n, false));
    vector<vector<pair<int,int>>> parent(n, vector<pair<int,int>>(n, {-1,-1}));

    auto inBounds = [&](int x, int y) {
        return x >= 0 && y >= 0 && x < n && y < n;
    };

    queue<pair<int,int>> q;
    if (exclude.find(start) != exclude.end()) return {}; // if start excluded - no path
    q.push(start);
    visited[start.first][start.second] = true;
    parent[start.first][start.second] = {-1,-1};

    int dirs[4][2] = {{1,0},{-1,0},{0,1},{0,-1}};

    while (!q.empty()) {
        auto cur = q.front(); q.pop();
        int x = cur.first, y = cur.second;

        if (cur == goal) {
            // reconstruct path
            vector<pair<int,int>> path;
            pair<int,int> p = goal;
            while (p.first != -1) {
                path.push_back(p);
                p = parent[p.first][p.second];
            }
            reverse(path.begin(), path.end());
            return path;
        }

        int jump = maze[x][y];
        if (jump <= 0) continue;

        for (auto &d : dirs) {
            int nx = x + d[0] * jump;
            int ny = y + d[1] * jump;
            if (!inBounds(nx, ny)) continue;
            if (exclude.find({nx, ny}) != exclude.end()) continue; // forbidden target
            if (!visited[nx][ny]) {
                visited[nx][ny] = true;
                parent[nx][ny] = {x, y};
                q.push({nx, ny});
            }
        }
    }

    return {}; // no path
}


bool PathValidator::isSolvable(const vector<vector<int>>& maze,
                               pair<int,int> start,
                               pair<int,int> goal) {
    auto path = bfsExactJump(maze, start, goal, {});
    return !path.empty();
}

vector<pair<int,int>> PathValidator::getAnyPath(const vector<vector<int>>& maze,
                                                pair<int,int> start,
                                                pair<int,int> goal) {
    return bfsExactJump(maze, start, goal, {});
}

vector<pair<int,int>> PathValidator::getAnotherPath(const vector<vector<int>>& maze,
                                                    pair<int,int> start,
                                                    pair<int,int> goal,
                                                    const vector<pair<int,int>>& excludePath) {
    set<pair<int,int>> excludeSet(excludePath.begin(), excludePath.end());
    return bfsExactJump(maze, start, goal, excludeSet);
}
