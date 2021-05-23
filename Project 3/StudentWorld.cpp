#include "StudentWorld.h"
#include "GameConstants.h"
#include <string>
#include <cmath>

#include "Actor.h"
using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath)
{
}

StudentWorld::~StudentWorld() {
    //Empty as GameController calls on the cleanup() function
}

int StudentWorld::init()
{
    //Step 1, Creates Socrates
    m_Socrates = new Socrates(IID_PLAYER, 0, VIEW_HEIGHT/2, 0, this);
    
    //Step 2, Creates Pits
    for (int k = 0; k < getLevel(); k++) {
        int x,y;
        newCoords(x, y);
        Pit* p = new Pit(IID_PIT, x, y, 1, this);
        addActor(p);
    }
    
    //Step 3, Creates Food
    int numFood = min((5*getLevel()),25);
    for (int k = 0; k < numFood; k++) {
        int x,y;
        newCoords(x, y);
        Food* p = new Food(IID_FOOD, x, y, 1, this);
        addActor(p);
    }
    
    //Step 4, Creates Dirt
    int numDirt = max(180 - (20 * getLevel()), 20);
    size_t startSize = m_actors.size();
    
    for (int k = 0; k < numDirt; k++) {
        int centerX = VIEW_WIDTH/2;
        int centerY = VIEW_HEIGHT/2;
        int x = 0;
        int y = 0;
        
        for (;;) {
            //Makes sure distance is within 120 pixels from center
            int r = randInt(0, 120);
            double theta = randInt(0, 360);
            x = r*cos(theta) + centerX;
            y = r*sin(theta) + centerY;
            
            //Makes sure x and y values don't overlap with anything else
            bool coordsWork = true;
            list<Actor*>::iterator it = m_actors.begin();
            int currSize = 0;
            while (it != m_actors.end()) {
                if (currSize >= startSize)
                    break;
                
                int x1 = (*it)->getX();
                int y1 = (*it)->getY();
                int distX = abs(x-x1);
                int distY = abs(y-y1);
                it++;
                currSize++;
                
                if (distX <= SPRITE_WIDTH && distY <= SPRITE_WIDTH) {
                    coordsWork = false;
                    break;
                }
            }
            if (coordsWork == false)
                continue;
            else
                break;
        }
        
        Dirt* p = new Dirt(IID_DIRT, x, y, 1, this);
        addActor(p);
    }
    
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    //Step 1, Socrates & Actors Do Something
    m_Socrates->doSomething();
    list<Actor*>::iterator it = m_actors.begin();
    while (it != m_actors.end()) {
        (*it)->doSomething();
        it++;
    }
    
    //Step 1a, Check if Socrates is dead
    if (m_Socrates->getHealth() <= 0){
        decLives();
        return GWSTATUS_PLAYER_DIED;
    }
    
    //Step 1b, Check to see if Socrates has cleared the Petri dish of Bacteria and pits
    it = m_actors.begin();
    bool finishedLevel = true;
    while (it != m_actors.end()) {
        if ((*it)->preventsLevelCompleting())
            finishedLevel = false;
        it++;
    }
    if (finishedLevel) {
        playSound(SOUND_FINISHED_LEVEL);
        return GWSTATUS_FINISHED_LEVEL;
    }
    
    //Step 2, Delete dead actors
    it = m_actors.begin();
    while (it != m_actors.end()) {
        if (!(*it)->isAlive()) {
            delete (*it);
            m_actors.erase(it);
        }
        it++;
    }
    
    //Step 3, Add new Objects to the game (Goodies)
    //Chance for Fungus to form
    int chanceFungus = max(510-getLevel()*10, 200);
    if (randInt(0, chanceFungus) == 0) {
        int angle = randInt(0, 359);
        int x = VIEW_WIDTH/2;
        int y = VIEW_HEIGHT/2;
        Fungus* p = new Fungus(IID_FUNGUS, x, y, 1, this);
        p->moveAngle(angle,VIEW_RADIUS);
        addActor(p);
    }
    
    //Chance for Goodie
    int chanceGoodie = max(510-getLevel()*10, 250);
    if (randInt(0, chanceGoodie) == 0) {
        int angle = randInt(0, 359);
        int x = VIEW_WIDTH/2;
        int y = VIEW_HEIGHT/2;
        int goodieType = randInt(0, 9);
        Goodie* p = nullptr;
        switch (goodieType) {
            case 0:
            case 1:
            case 2:
            case 3:
            case 4:
            case 5: {
                p = new RestoreHealthGoodie(IID_RESTORE_HEALTH_GOODIE, x, y, 1, this);
                break;
            }
            case 6:
            case 7:
            case 8: {
                p = new FlamethrowerGoodie(IID_FLAME_THROWER_GOODIE, x, y, 1, this);
                break;
            }
            case 9: {
                p = new ExtraLifeGoodie(IID_EXTRA_LIFE_GOODIE, x, y, 1, this);
                break;
            }
        }
        p->moveAngle(angle,VIEW_RADIUS);
        addActor(p);
    }

    //Step 4, Update Game Status
    int disScore = getScore();
    int disHealth = m_Socrates->getHealth();
    int disSpray = m_Socrates->getSprays();
    int disFlames = m_Socrates->getFlames();
    
    string newStatus = "Score:  " + to_string(disScore) + "  Level:  " + to_string(getLevel()) + "  Lives:  " + to_string(getLives()) + "  Health:  " + to_string(disHealth) + "  Sprays:  " + to_string(disSpray) + "  Flames:  " + to_string(disFlames);
    
    setGameStatText(newStatus);
    
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
    //Deletes Socrates
    delete m_Socrates;
    
    //Deletes all actors
     list<Actor*>::iterator it = m_actors.begin();
     while (it != m_actors.end()) {
         delete (*it);
         m_actors.erase(it);
         it++;
     }
}

void StudentWorld::newCoords(int &x, int &y) {
    //Arbitrarily large radius that's > 120
    int centerX = VIEW_WIDTH/2;
    int centerY = VIEW_HEIGHT/2;

    for (;;) {
        int r = randInt(0, 120);
        double theta = randInt(0, 360);
        x = r*cos(theta) + centerX;
        y = r*sin(theta) + centerY;
        
        //Makes sure x and y values don't overlap with anything else
        bool coordsWork = true;
        list<Actor*>::iterator it = m_actors.begin();
        while (it != m_actors.end()) {
            int x1 = (*it)->getX();
            int y1 = (*it)->getY();
            int distX = abs(x-x1);
            int distY = abs(y-y1);
            it++;
            
            if (distX <= SPRITE_WIDTH && distY <= SPRITE_WIDTH) {
                coordsWork = false;
                break;
            }
        }
        if (coordsWork == false)
            continue;
        else
            break;
    }
}

void StudentWorld::addActor(Actor* p) {
    m_actors.push_back(p);
}

bool StudentWorld::getOverlappingSocrates(Actor* a) const {
    int socX = m_Socrates->getX();
    int socY = m_Socrates->getY();
    int actorX = a->getX();
    int actorY = a->getY();
    
    int distX = abs(actorX-socX);
    int distY = abs(actorY-socY);
    if(distX <= SPRITE_WIDTH && distY <= SPRITE_WIDTH) {
        return true;
    }
    return false;
}

Socrates* StudentWorld::getSocrates() {
    return m_Socrates;
}

bool StudentWorld::takeDamage(int x, int y, int dmg) {
    list<Actor*>::iterator it = m_actors.begin();
    while (it != m_actors.end()) {
        int x1 = (*it)->getX();
        int y1 = (*it)->getY();
        int distX = abs(x-x1);
        int distY = abs(y-y1);
        
        if (distX <= SPRITE_WIDTH && distY <= SPRITE_WIDTH && ((*it)->canDamage())) {
            (*it)->takeDamage(dmg);
            return true;
        }
        it++;
    }
    return false;
}

bool StudentWorld::isFoodHere(int x, int y) {
    list<Actor*>::iterator it = m_actors.begin();
    while (it != m_actors.end()) {
        if ((*it)->isEdible()) {
            int x1 = (*it)->getX();
            int y1 = (*it)->getY();
            int distX = abs(x-x1);
            int distY = abs(y-y1);
            
            if (distX <= SPRITE_WIDTH && distY <= SPRITE_WIDTH) {
                delete (*it);
                m_actors.erase(it);
                return true;
            }
        }
        it++;
    }
    return false;
}

bool StudentWorld::isBacteriumMovementBlockedAt(Actor* a, int units) const {
    list<Actor*>::const_iterator it = m_actors.begin();
    while (it != m_actors.end()) {
        //x and y if the bacterium were to move 3 pixels in a certain direction
        double newx, newy;
        a->getPositionInThisDirection(a->getDirection(), units, newx, newy);
        
        //Checks if bacterium will move outside petri dish; calculates using radius formula from center of petri dish (VIEW_WIDTH/2, VIEW_HEIGHT/2)
        int centerX = VIEW_WIDTH/2;
        int centerY = VIEW_HEIGHT/2;
        int rSquared = (centerX-newx)*(centerX-newx) + (centerY-newy)*(centerY-newy);
        int r = sqrt(rSquared);
        if (r >= VIEW_RADIUS)
            return true;
        
        //Checks if bacterium runs into dirt
        if ((*it)->blocksBacteriumMovement()) {
            int x1 = (*it)->getX();
            int y1 = (*it)->getY();
            
            double distance = sqrt((newx-x1)*(newx-x1) + (newy-y1)*(newy-y1));
            
            if (distance <= SPRITE_WIDTH/2)
                return true;
        }
        it++;
    }
    return false;
}

bool StudentWorld::getAngleToNearestNearbyEdible(Actor* a, int& angle) const {
    int currX = a->getX();
    int currY = a->getY();
    list<Actor*>::const_iterator it = m_actors.begin();
    while (it != m_actors.end()) {
        if ((*it)->isEdible()) {
            double x1 = (*it)->getX();
            double y1 = (*it)->getY();
            
            double distance = sqrt((currX-x1)*(currX-x1) + (currY-y1)*(currY-y1));
            
            //If the food item is within 128 pixels
            if (abs(distance) <= 128) {
                angle = atan2((currY-y1),(currX-x1)) * 180/M_PI + 180;
                return true;
            }
        }
        it++;
    }
    return false;
}

bool StudentWorld::getAngleToNearbySocrates(Actor* a, int dist, int& angle) const {
    int currX = a->getX();
    int currY = a->getY();
    double socX = m_Socrates->getX();
    double socY = m_Socrates->getY();
            
    double distance = sqrt((currX-socX)*(currX-socX) + (currY-socY)*(currY-socY));
            
    //If the food item is within 128 pixels
    if (abs(distance) <= dist) {
        angle = atan2((currY-socY),(currX-socX)) * 180/M_PI + 180;
        return true;
    }
    
    return false;
}
