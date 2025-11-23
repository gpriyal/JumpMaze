#ifndef PATH_VALIDATOR_H
#define PATH_VALIDATOR_H

#include <vector>
#include <utility>
using namespace std;

class PathValidator {
public:
    static bool isSolvable(const vector<vector<int>>& maze,
                           pair<int,int> start,
                           pair<int,int> goal);

    // Return one shortest path by BFS from start to goal
    static vector<pair<int,int>> getAnyPath(const vector<vector<int>>& maze,
                                            pair<int,int> start,
                                            pair<int,int> goal);

    // Return a path that avoids using nodes in excludePath (to find an alternate path)
    static vector<pair<int,int>> getAnotherPath(const vector<vector<int>>& maze,
                                                pair<int,int> start,
                                                pair<int,int> goal,
                                                const vector<pair<int,int>>& excludePath);
};

#endif 
