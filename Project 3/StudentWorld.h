#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include <string>
#include <list>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class Actor;
class Socrates;

class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetPath);
    ~StudentWorld();
    virtual int init();
    virtual int move();
    virtual void cleanUp();
    
    //Accessors
    bool takeDamage(int x, int y, int dmg);
    bool getOverlappingSocrates(Actor* a) const;
    bool isBacteriumMovementBlockedAt(Actor* a, int units) const;
    bool getAngleToNearestNearbyEdible(Actor* a, int& angle) const;
    bool getAngleToNearbySocrates(Actor* a, int dist, int& angle) const;
    Socrates* getSocrates();
    
    //Mutators
    void addActor(Actor* p);
    bool isFoodHere(int x, int y);

private:
    Socrates* m_Socrates;
    std::list<Actor*> m_actors;
    
    //Helper Function, changes x and y to (x,y) pair that do not overlap with an existing actor
    void newCoords(int &x, int &y);

};

#endif // STUDENTWORLD_H_
