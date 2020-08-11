//
//  Vampire.hpp
//  Project 1
//
//  Created by Cody Do on 1/8/20.
//  Copyright © 2020 Cody Do. All rights reserved.
//

#ifndef Vampire_h
#define Vampire_h

class Arena; //Needed as Arena is used in Vampire's class declaration

class Vampire
{
  public:
      // Constructor
    Vampire(Arena* ap, int r, int c);

      // Accessors
    int  row() const;
    int  col() const;
    bool isDead() const;

      // Mutators
    void move();

  private:
    Arena* m_arena;
    int    m_row;
    int    m_col;
    int    m_health;
    int    m_idleTurnsRemaining;
};

#endif /* Vampire.h */
