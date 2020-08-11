//
//  Game.hpp
//  Project 1
//
//  Created by Cody Do on 1/8/20.
//  Copyright © 2020 Cody Do. All rights reserved.
//

#ifndef Game_h
#define Game_h
#include <string>

class Arena; //Needed as Arena is used in Game's class declaration

class Game
{
  public:
      // Constructor/destructor
    Game(int rows, int cols, int nVampires);
    ~Game();

      // Mutators
    void play();

  private:
    Arena* m_arena;

      // Helper functions
    std::string takePlayerTurn();
};

#endif /* Game.h */
