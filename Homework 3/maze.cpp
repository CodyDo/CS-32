//
//  maze.cpp
//  Homework 3
//
//  Created by Cody Do on 2/8/20.
//  Copyright © 2020 Cody Do. All rights reserved.
//

#include <iostream>
#include <string>
using namespace std;

class Coord
{
  public:
    Coord(int rr, int cc) : m_r(rr), m_c(cc) {}
    int r() const { return m_r; }
    int c() const { return m_c; }
  private:
    int m_r;
    int m_c;
};

bool pathExists(string maze[], int nRows, int nCols, int sr, int sc, int er, int ec) {
    if (sr == er && sc == ec) //If start/end position is the same, return true
        return true;
    
    maze[sr][sc] = '*'; // drop breadcrumb on position
    
    if (sc != nCols && maze[sr][sc+1] == '.') {
        // Check EAST Boundary and if it was unexplored
        if (pathExists(maze, nRows, nCols, sr, sc+1, er, ec))
            return true;
    }
    
    if (sr != nRows && maze[sr+1][sc] == '.') {
        // Check SOUTH Boundary and if it was unexplored
        if (pathExists(maze, nRows, nCols, sr+1, sc, er, ec))
            return true;
    }
    
    if (sc != 0 && maze[sr][sc-1] == '.') {
        // Check WEST Boundary and if it was unexplored
        if (pathExists(maze, nRows, nCols, sr, sc-1, er, ec))
          return true;
    }
    
    if (sr != 0 && maze[sr-1][sc] == '.') {
        // Check NORTH Boundary and if it was unexplored
        if (pathExists(maze, nRows, nCols, sr-1, sc, er, ec))
           return true;
    }
    
    return false;

}

/*int main()
{
    string maze[10] = {
        "XXXXXXXXXX",
        "X...X..X.X",
        "X.XXX....X",
        "X.X.XXXX.X",
        "XXX......X",
        "X...X.XX.X",
        "X.X.X..X.X",
        "X.XXXX.X.X",
        "X..X...X.X",
        "XXXXXXXXXX"
    };

    if (pathExists(maze, 10,10, 4,3, 1,8))
        cout << "Solvable!" << endl;
    else
        cout << "Out of luck!" << endl;
    
    for (int k = 0; k < 10; k++)
        cout << maze[k] << endl;
} */
