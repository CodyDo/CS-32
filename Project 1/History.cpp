//
//  History.cpp
//  Project 1
//
//  Created by Cody Do on 1/8/20.
//  Copyright © 2020 Cody Do. All rights reserved.
//

#include <iostream>
#include <string>
#include "History.h"
#include "globals.h"
using namespace std;

History::History(int nRows, int nCols) {
    m_Rows = nRows;
    m_Cols = nCols;
    for (int k = 0; k < m_Rows; k++)
        for (int j = 0; j < m_Cols; j++)
            counter[k][j] = 0;
}

bool History::record(int r, int c) {
    if (r < 1 || r > m_Rows || c < 1 || c > m_Cols ) {
        return false;
    }
    counter[r-1][c-1] += 1;
    return true;
}

void History::display() const {
    char displayGrid[MAXROWS][MAXCOLS];
    int r, c;
    
      // Fill displayGrid with the appropriate symbol
    string symbolList = ".ABCDEFGHIJKLMNOPQRSTUVWXY"; //Z is excluded as the exception is hardcoded below
    for (r = 1; r <= m_Rows; r++)
        for (c = 1; c <= m_Cols; c++){
            int gridValue = counter[r-1][c-1];
            if (gridValue >= 26)
                displayGrid[r-1][c-1] = 'Z';
            else {
                displayGrid[r-1][c-1] = symbolList[gridValue];
        }
    }
      // Clears the screen and produces the grid with symbols
    clearScreen();
    for (r = 1; r <= m_Rows; r++)
    {
        for (c = 1; c <= m_Cols; c++)
            cout << displayGrid[r-1][c-1];
        cout << endl;
    }
    cout << endl;
    
}
