//
//  mazequeue.cpp
//  Homework 2
//
//  Created by Cody Do on 2/1/20.
//  Copyright © 2020 Cody Do. All rights reserved.
//

#include <iostream>
#include <queue>
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
    queue<Coord> coordQueue; //Declares a Queue of Coords
    coordQueue.push(Coord(sr,sc));
    maze[sr][sc] = '*';
    
    while (!coordQueue.empty()) {
        Coord currCoord = coordQueue.front(); //Holds first value & then removes it
        coordQueue.pop();
        if (currCoord.r() == er && currCoord.c() == ec)
            return true;
        
        if (currCoord.c() != nCols && maze[currCoord.r()][currCoord.c()+1] == '.') { // Check EAST Boundary and if it was unexplored
            coordQueue.push(Coord(currCoord.r(),currCoord.c()+1));
            maze[currCoord.r()][currCoord.c()+1] = '*';
        }
        
        if (currCoord.r() != nRows && maze[currCoord.r()+1][currCoord.c()] == '.') { // Check SOUTH Boundary and if it was unexplored
            coordQueue.push(Coord(currCoord.r()+1,currCoord.c()));
            maze[currCoord.r()+1][currCoord.c()] = '*';
        }
        
        if (currCoord.c() != nCols && maze[currCoord.r()][currCoord.c()-1] == '.') { // Check WEST Boundary and if it was unexplored
            coordQueue.push(Coord(currCoord.r(),currCoord.c()-1));
            maze[currCoord.r()][currCoord.c()-1] = '*';
        }
        
        if (currCoord.r() != nRows && maze[currCoord.r()-1][currCoord.c()] == '.') { // Check NORTH Boundary and if it was unexplored
            coordQueue.push(Coord(currCoord.r()-1,currCoord.c()));
            maze[currCoord.r()-1][currCoord.c()] = '*';
        }
        
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

