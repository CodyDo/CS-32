//
//  History.hpp
//  Project 1
//
//  Created by Cody Do on 1/8/20.
//  Copyright © 2020 Cody Do. All rights reserved.
//

#ifndef History_h
#define History_h
#include "globals.h"

class History
{
  public:
    History(int nRows, int nCols);
    bool record(int r, int c);
    void display() const;
  private:
    int m_Rows;
    int m_Cols;
    int counter[MAXROWS][MAXCOLS];
};

#endif /* History.h */
