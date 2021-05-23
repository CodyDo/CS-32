#include "Actor.h"
#include "StudentWorld.h"
#include <list>

using namespace std;
// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp


// // // // // // // // // // // // // // // // // // // // // // //
//                          Actor                                 //
// // // // // // // // // // // // // // // // // // // // // // //

Actor::Actor(int imageID, int startX, int startY, int depth, StudentWorld* studWorld)
: GraphObject(imageID, startX, startY, depth), m_studWorld(studWorld), m_alive(true) {
}

Actor::~Actor() {
}

bool Actor::isAlive() const {
    return m_alive;
}

StudentWorld* Actor::getWorld() const {
    return m_studWorld;
}

void Actor::setAlive(bool x) {
    m_alive = x;
}

bool Actor::preventsLevelCompleting() const {
    return false;
}

bool Actor::canDamage() const {
    return true;
}

bool Actor::blocksBacteriumMovement() const {
    return false;
}

bool Actor::isEdible() const {
    return false;
}

// // // // // // // // // // // // // // // // // // // // // // //
//                          Agent                                 //
// // // // // // // // // // // // // // // // // // // // // // //

Agent::Agent(int imageID, int startX, int startY, int depth, StudentWorld* studWorld, int health) : Actor(imageID, startX, startY, depth, studWorld), m_health(health) {
}

Agent::~Agent() {
}

int Agent::getHealth() const {
    return m_health;
}

void Agent::restoreHealth() {
    m_health = 100;
}

void Agent::takeDamage(int x) {
    m_health -= x;
}

// // // // // // // // // // // // // // // // // // // // // // //
//                          Socrates                              //
// // // // // // // // // // // // // // // // // // // // // // //

Socrates::Socrates(int imageID, int startX, int startY, int depth, StudentWorld* studWorld) : Agent(imageID, startX, startY, depth, studWorld, 100), m_nSprays(20), m_nFlames(5), rechargeSpray(true)
{
    setDirection(0);
    moveAngle(180);
}

Socrates::~Socrates() {
}

int Socrates::getSprays() const {
    return m_nSprays;
}

int Socrates::getFlames() const {
    return m_nFlames;
}

void Socrates::doSomething() {
    int ch;
    if (getWorld()->getKey(ch)) { //2c
        double newx, newy;
        switch (ch) {
            case KEY_PRESS_LEFT: {
                setDirection(getDirection()-5); //ci
            
                moveTo(VIEW_WIDTH/2, VIEW_HEIGHT/2); //cii and ciii
                getPositionInThisDirection(getDirection()+180, VIEW_RADIUS, newx, newy);
                moveTo(newx, newy);
        
                break;
            }
                
            case KEY_PRESS_RIGHT: {
                setDirection(getDirection()+5); //ci
                    
                moveTo(VIEW_WIDTH/2, VIEW_HEIGHT/2); //cii and ciii
                getPositionInThisDirection(getDirection()+180, VIEW_RADIUS, newx, newy);
                moveTo(newx, newy);
        
                break;
            }
                
            case KEY_PRESS_SPACE: {
                //2a Check spray count
                if(m_nSprays <= 0)
                    return;
                double startX = 0;
                double startY = 0;
                getPositionInThisDirection(getDirection(), SPRITE_WIDTH, startX, startY);

                //Allocate new spray object + sound
                Spray* p = new Spray(IID_SPRAY, startX, startY, 1, getWorld(), 112, getDirection());
                getWorld()->addActor(p);
                m_nSprays--;
                getWorld()->playSound(SOUND_PLAYER_SPRAY);
                
                break;
            }
            
            case KEY_PRESS_ENTER: {
                if (m_nFlames <= 0)
                    return;
                double degrees = 0;
                while (degrees < 360) {
                    double startX = 0;
                    double startY = 0;
                    getPositionInThisDirection(degrees, SPRITE_WIDTH, startX, startY);
                    
                    Flame* p = new Flame(IID_FLAME, startX, startY, 1, getWorld(), 32, degrees);
                    getWorld()->addActor(p);
                    
                    degrees += 22; //16 flames in a circle, changes by 22 degrees
                }
                m_nFlames--;
                getWorld()->playSound(SOUND_PLAYER_FIRE);
            }
            
            default:
                break;
        }
    }
    else {
        if (m_nSprays < 20) { //Only recharges every other tick
            if (rechargeSpray)
                m_nSprays++;
            else
                rechargeSpray = true;
        }
    }
    return;
}

bool Socrates::canDamage() const {
    return false;
}

void Socrates::addFlames() {
    m_nFlames += 5;
}

void Socrates::takeDamage(int x) {
    Agent::takeDamage(x);
    if (getHealth() > 0)
        getWorld()->playSound(SOUND_PLAYER_HURT);
    if (getHealth() <= 0)
        getWorld()->playSound(SOUND_PLAYER_DIE);
}

// // // // // // // // // // // // // // // // // // // // // // //
//                             Dirt                               //
// // // // // // // // // // // // // // // // // // // // // // //

Dirt::Dirt(int imageID, int startX, int startY, int depth, StudentWorld* studWorld) : Actor(imageID, startX, startY, depth, studWorld) {
}

Dirt::~Dirt() {
}

void Dirt::takeDamage(int x) {
    setAlive(false);
}

void Dirt::doSomething() {
    return;
}

bool Dirt::blocksBacteriumMovement() const {
    return true;
}

// // // // // // // // // // // // // // // // // // // // // // //
//                             Food                               //
// // // // // // // // // // // // // // // // // // // // // // //
Food::Food(int imageID, int startX, int startY, int depth, StudentWorld* studWorld) : Actor(imageID, startX, startY, depth, studWorld) {
}

Food::~Food() {
}

bool Food::canDamage() const {
    return false;
}

void Food::takeDamage(int x) {
    return;
}

void Food::doSomething() {
    return;
}

bool Food::isEdible() const {
    return true;
}

// // // // // // // // // // // // // // // // // // // // // // //
//                          Projectile                            //
// // // // // // // // // // // // // // // // // // // // // // //
Projectile::Projectile(int imageID, int startX, int startY, int depth, StudentWorld* studWorld, int distance, int direction) : Actor(imageID, startX, startY, depth, studWorld), m_availDist(distance) {
    setDirection(direction);
}

Projectile::~Projectile() {
}

bool Projectile::canDamage() const {
    return false;
}

void Projectile::takeDamage(int x) {
    return;
}

int Projectile::availDist() const {
    return m_availDist;
}

void Projectile::changeAvailDist(int x) {
    m_availDist += x;
}

void Projectile::doSomething() {
    //Step 1, check if alive
    if (!isAlive())
        return;
    
    //Step 2, check if overlaps with damageable object and reacts accordingly
    doSomethingSpecific();
    
    //Step 3, move forward in proper direction; adjust available distance
    moveAngle(getDirection(), SPRITE_WIDTH);
    changeAvailDist(-SPRITE_WIDTH);
    
    //Step 4, check if spray should dissipate
    if (availDist() < 0)
        setAlive(false);
}

// // // // // // // // // // // // // // // // // // // // // // //
//                             Spray                              //
// // // // // // // // // // // // // // // // // // // // // // //
Spray::Spray(int imageID, int startX, int startY, int depth, StudentWorld* studWorld, int distance, int direction) : Projectile(imageID, startX, startY, depth, studWorld, distance, direction) {
}

Spray::~Spray() {
}

void Spray::doSomethingSpecific() {
    if(getWorld()->takeDamage(getX(), getY(), 2))
        setAlive(false);
}

// // // // // // // // // // // // // // // // // // // // // // //
//                             Flame                              //
// // // // // // // // // // // // // // // // // // // // // // //
Flame::Flame(int imageID, int startX, int startY, int depth, StudentWorld* studWorld, int distance, int direction) : Projectile(imageID, startX, startY, depth, studWorld, distance, direction) {
}

Flame::~Flame() {
}

void Flame::doSomethingSpecific() {
    if(getWorld()->takeDamage(getX(), getY(), 5))
        setAlive(false);
}

// // // // // // // // // // // // // // // // // // // // // // //
//                             Goodie                             //
// // // // // // // // // // // // // // // // // // // // // // //
Goodie::Goodie(int imageID, int startX, int startY, int depth, StudentWorld* studWorld) : Actor(imageID, startX, startY, depth, studWorld)
{
    m_lifetime = max(randInt(0, 300 - 10 * studWorld->getLevel() - 1), 50);
    setDirection(0);
}

Goodie::~Goodie() {
}

void Goodie::takeDamage(int x) {
    setAlive(false);
}

void Goodie::doSomething() {
    //Step 1, check if it's alive
    if(!isAlive())
        return;
    
    //Step 2, Check if overlaps with Socrates. If yes, do Goodie's task
    if(getWorld()->getOverlappingSocrates(this)) {
        doGoodieTask();
        return;
    }
    
    //Step 3, check if lifetime has expired. If not, reduce lifetime.
    if (m_lifetime < 0)
        setAlive(false);
    m_lifetime--;
}

// // // // // // // // // // // // // // // // // // // // // // //
//                     Restore Health Goodie                      //
// // // // // // // // // // // // // // // // // // // // // // //
RestoreHealthGoodie::RestoreHealthGoodie(int imageID, int startX, int startY, int depth, StudentWorld* studWorld) : Goodie(imageID, startX, startY, depth, studWorld) {
}

RestoreHealthGoodie::~RestoreHealthGoodie() {
}

void RestoreHealthGoodie::doGoodieTask() {
    //Step 2, Increase score by 250, kill goodie, play sound, restore health
    getWorld()->increaseScore(250);
    setAlive(false);
    getWorld()->playSound(SOUND_GOT_GOODIE);
    getWorld()->getSocrates()->restoreHealth();
}

// // // // // // // // // // // // // // // // // // // // // // //
//                     Flamethrower Goodie                        //
// // // // // // // // // // // // // // // // // // // // // // //
FlamethrowerGoodie::FlamethrowerGoodie(int imageID, int startX, int startY, int depth, StudentWorld* studWorld) : Goodie(imageID, startX, startY, depth, studWorld) {
}

FlamethrowerGoodie::~FlamethrowerGoodie() {
}

void FlamethrowerGoodie::doGoodieTask() {
    //Step 2, Increase score by 300, kill goodie, play sound, add flamethrower charges
    getWorld()->increaseScore(300);
    setAlive(false);
    getWorld()->playSound(SOUND_GOT_GOODIE);
    getWorld()->getSocrates()->addFlames();
}

// // // // // // // // // // // // // // // // // // // // // // //
//                      Extra Life Goodie                         //
// // // // // // // // // // // // // // // // // // // // // // //
ExtraLifeGoodie::ExtraLifeGoodie(int imageID, int startX, int startY, int depth, StudentWorld* studWorld) : Goodie(imageID, startX, startY, depth, studWorld) {
}

ExtraLifeGoodie::~ExtraLifeGoodie() {
}

void ExtraLifeGoodie::doGoodieTask() {
    //Step 2, Increase score by 500, kill goodie, play sound, add extra life
    getWorld()->increaseScore(500);
    setAlive(false);
    getWorld()->playSound(SOUND_GOT_GOODIE);
    getWorld()->incLives();
}

// // // // // // // // // // // // // // // // // // // // // // //
//                            Fungus                              //
// // // // // // // // // // // // // // // // // // // // // // //
Fungus::Fungus(int imageID, int startX, int startY, int depth, StudentWorld* studWorld) : Goodie(imageID, startX, startY, depth, studWorld) {
}

Fungus::~Fungus() {
}

void Fungus::doGoodieTask() {
    //Step 2, Decrease score by 50, kill goodie, play sound, decrease health by 20
    getWorld()->increaseScore(-50);
    setAlive(false);
    getWorld()->playSound(SOUND_PLAYER_HURT);
    getWorld()->getSocrates()->takeDamage(20);
}

// // // // // // // // // // // // // // // // // // // // // // //
//                              Pit                               //
// // // // // // // // // // // // // // // // // // // // // // //
Pit::Pit(int imageID, int startX, int startY, int depth, StudentWorld* studWorld) : Actor(imageID, startX, startY, depth, studWorld), m_nRegSal(5), m_nAggroSal(3), m_nEColi(2)
{
    setDirection(0);
}

Pit::~Pit() {
}

bool Pit::canDamage() const {
    return false;
}

void Pit::takeDamage(int x) {
    return;
}

bool Pit::preventsLevelCompleting() const {
    return true;
}

int Pit::pickBacterium() {
    //If all three are alive
    if (m_nRegSal > 0 && m_nAggroSal > 0 && m_nEColi > 0) {
        return randInt(0, 2);
    }
    
    //If only RegSal left
    if(m_nRegSal > 0 && m_nAggroSal <= 0 && m_nEColi <= 0) {
        return 0;
    }
    
    //If only AggroSal left
    if(m_nRegSal <= 0 && m_nAggroSal > 0 && m_nEColi <= 0) {
        return 1;
    }
    
    //If only EColi left
    if(m_nRegSal <= 0 && m_nAggroSal <= 0 && m_nEColi > 0) {
        return 2;
    }
    
    //If only RegSal and AggroSal Left
    if(m_nRegSal > 0 && m_nAggroSal > 0 && m_nEColi <= 0) {
        return randInt(0, 1);
    }
    
    //If only RegSal and EColi Left
    if(m_nRegSal > 0 && m_nAggroSal <= 0 && m_nEColi > 0) {
        int x = randInt(0, 1);
        if(x == 0)
            return 0;
        else
            return 2;
    }
    
    //If only AggroSal and EColi left
    if(m_nRegSal <= 0 && m_nAggroSal > 0 && m_nEColi > 0) {
        return randInt(1, 2);
    }
    
    return -1;
}

void Pit::doSomething() {
    //Step 1, If all bacteria are gone, remove pit
    if (m_nEColi <= 0 && m_nRegSal <= 0 && m_nAggroSal <= 0)
        setAlive(false);
    
    //Step 2, 1 in 50 chance to spawn bacteria. Emit each bacteria with same probability (depending on what's left)
    if(randInt(0, 49) == 0) {
        // pickBacterium is a helper function implemented in private section
        // 0 = Regular Salmonella    1 = Aggressive Salmonella    2 = EColi
        int bacteriaNum = pickBacterium();
        Actor* p = nullptr;
        switch (bacteriaNum) {
            case 0:
                p = new RegSalmonella(IID_SALMONELLA, getX(), getY(), 1, getWorld(), 4);
                m_nRegSal--;
                break;
            
            case 1:
                p = new AggroSalmonella(IID_SALMONELLA, getX(), getY(), 1, getWorld(), 10);
                m_nAggroSal--;
                break;
                
            case 2:
                p = new EColi(IID_ECOLI, getX(), getY(), 1, getWorld(), 5);
                m_nEColi--;
                break;
                
            default:
                break;
        }
        getWorld()->addActor(p);
    }
    return;
}

// // // // // // // // // // // // // // // // // // // // // // //
//                          Bacterium                             //
// // // // // // // // // // // // // // // // // // // // // // //
Bacterium::Bacterium(int imageID, int startX, int startY, int depth, StudentWorld* studWorld, int health) : Agent(imageID, startX, startY, depth, studWorld, health), m_movementPlanDist(0), m_foodEaten(0)
{
    setDirection(90);
    getWorld()->playSound(SOUND_BACTERIUM_BORN);
}

Bacterium::~Bacterium() {
}

bool Bacterium::preventsLevelCompleting() const {
    return true;
}

int Bacterium::getFoodEaten() const {
    return m_foodEaten;
}

int Bacterium::getMovementPlan() const {
    return m_movementPlanDist;
}

void Bacterium::setFoodEaten(int x) {
    m_foodEaten = x;
}

void Bacterium::setMovementPlan(int x) {
    m_movementPlanDist = x;
}

void Bacterium::takeDamage(int x) {
    Agent::takeDamage(x);
    if (getHealth() > 0)
        getWorld()->playSound(soundWhenHurt());
    else {
        setAlive(false);
        getWorld()->playSound(soundWhenDie());
        getWorld()->increaseScore(100);
        if(randInt(0, 1) == 0) {
            Food* p = new Food(IID_FOOD, getX(), getY(), 1, getWorld());
            getWorld()->addActor(p);
        }
    }
}

// // // // // // // // // // // // // // // // // // // // // // //
//                         RegSalmonella                          //
// // // // // // // // // // // // // // // // // // // // // // //
RegSalmonella::RegSalmonella(int imageID, int startX, int startY, int depth, StudentWorld* studWorld, int health) : Bacterium(imageID, startX, startY, depth, studWorld, health) {
}

RegSalmonella::~RegSalmonella() {
}

int RegSalmonella::soundWhenHurt() const {
    return SOUND_SALMONELLA_HURT;
}

int RegSalmonella::soundWhenDie() const {
    return SOUND_SALMONELLA_DIE;
}

void RegSalmonella::doSomething() {

    //Step 1, Check if alive and if not, return
    if(!isAlive())
        return;
    
    //Step 2, Check if overlap with Socrates. If so, damage and go to Step 5
    bool skipToFive = false;
    if(getWorld()->getOverlappingSocrates(this)) {
        getWorld()->getSocrates()->takeDamage(1);
        skipToFive = true;
    }
    
    //If skipToFive is false AKA Socrates didn't get hurt
    if (!skipToFive) {
        //Step 3, Check to see if it's eaten 3 foods. If so, divide + skip to 5
        if (getFoodEaten() == 3) {
            //3a, Create new X coord
            int newx = getX();
            if(newx < VIEW_WIDTH/2)
                newx += SPRITE_WIDTH/2;
            if(newx > VIEW_WIDTH/2)
                newx -= SPRITE_WIDTH/2;
            
            //3b, Create new Y coord
            int newy = getY();
            if(newy < VIEW_HEIGHT/2)
                newy += SPRITE_WIDTH/2;
            if(newy > VIEW_HEIGHT/2)
                newy -= SPRITE_WIDTH/2;
            
            //3c, Create new Salmonella Object
            RegSalmonella* p = new RegSalmonella(IID_SALMONELLA, newx, newy, 0, getWorld(), 4);
            getWorld()->addActor(p);
            
            //3de, Reset Food Items Eaten and then skip to 5
            setFoodEaten(0);
            skipToFive = true;
        }
        
        //Step 4, Check if it's overlapping with one or more food items. Only happens if Step 3 doesn't happen.
        if ((!skipToFive) && (getWorld()->isFoodHere(getX(), getY()))) {
            setFoodEaten(getFoodEaten()+1);
        }
    }
    
    //Step 5, Checks to see if movement plan > 0
    if (getMovementPlan() > 0) {
        //5a, Decrement movement by one
        setMovementPlan(getMovementPlan()-1);
        
        //5bd, Checks if movement is blocked. If it is, change direction/movement plan
        if (getWorld()->isBacteriumMovementBlockedAt(this, 3)) {
            setDirection(randInt(0, 359));
            setMovementPlan(10);
        }
        
        //5c, If bacteria can move then move to the new location
        else
            moveAngle(getDirection(), 3);
        
        //5e, return
        return;
    }
    //Step 6, If movement plan is <= 0, move to closet food
    else {
        //6b, food can be found within 128 pixels
        int angle;
        if(getWorld()->getAngleToNearestNearbyEdible(this, angle)) {
            setDirection(angle);
            setMovementPlan(10);
            if (getWorld()->isBacteriumMovementBlockedAt(this, 3)) {
                setDirection(randInt(0, 359));
                setMovementPlan(10);
                return;
            }
        }
        
        //6a, food can't be found within 128 pixels
        else {
            setDirection(randInt(0, 359));
            setMovementPlan(10);
            return;
        }
    }
}

// // // // // // // // // // // // // // // // // // // // // // //
//                       AggroSalmonella                          //
// // // // // // // // // // // // // // // // // // // // // // //
AggroSalmonella::AggroSalmonella(int imageID, int startX, int startY, int depth, StudentWorld* studWorld, int health) : Bacterium(imageID, startX, startY, depth, studWorld, health) {
}

AggroSalmonella::~AggroSalmonella() {
}

int AggroSalmonella::soundWhenHurt() const {
    return SOUND_SALMONELLA_HURT;
}

int AggroSalmonella::soundWhenDie() const {
    return SOUND_SALMONELLA_DIE;
}

void AggroSalmonella::doSomething() {
    //Step 1, Check if alive and if not, return
    if(!isAlive())
        return;
    
    //Step 2, Check if Aggressive Salmonella is <= 72 pixels from Socrates
    int socAngle;
    bool isStepTwoDone = false;
    if (getWorld()->getAngleToNearbySocrates(this, 72, socAngle)) {
        setDirection(socAngle);
        moveAngle(getDirection(), 3);
        isStepTwoDone = true;
    }
    
    //Step 3, Check if overlap with Socrates. If so, damage and go to Step 6 (unless step 2 was done. In which case, return.
    bool skipToSix = false;
    if(getWorld()->getOverlappingSocrates(this)) {
        getWorld()->getSocrates()->takeDamage(2);
        skipToSix = true;
        if(isStepTwoDone)
            return;
    }
    
    //If skipToSix is false AKA Socrates didn't get hurt
    if (!skipToSix) {
        //Step 4, Check to see if it's eaten 3 foods. If so, divide + skip to 6
        if (getFoodEaten() == 3) {
            //4a, Create new X coord
            int newx = getX();
            if(newx < VIEW_WIDTH/2)
                newx += SPRITE_WIDTH/2;
            if(newx > VIEW_WIDTH/2)
                newx -= SPRITE_WIDTH/2;
            
            //4b, Create new Y coord
            int newy = getY();
            if(newy < VIEW_HEIGHT/2)
                newy += SPRITE_WIDTH/2;
            if(newy > VIEW_HEIGHT/2)
                newy -= SPRITE_WIDTH/2;
            
            //4c, Create new Salmonella Object
            AggroSalmonella* p = new AggroSalmonella(IID_SALMONELLA, newx, newy, 0, getWorld(), 10);
            getWorld()->addActor(p);
            
            //4de, Reset Food Items Eaten and then skip to 6 unless step 2 was done, in which case, return.
            setFoodEaten(0);
            skipToSix = true;
            if (isStepTwoDone)
                return;
        }
    
        //Step 5, Check if it's overlapping with one or more food items. Only happens if Step 4 doesn't happen. If step 2 was done, then return.
        if ((!skipToSix) && (getWorld()->isFoodHere(getX(), getY()))) {
            //p->setAlive(false);
            setFoodEaten(getFoodEaten()+1);
        }
    }
    
    //If got to step 5 but step 2 was done, then don't pass into step 6
    if (isStepTwoDone)
         return;
    
    //Step 6, Checks to see if movement plan > 0
    if (getMovementPlan() > 0) {
        //6a, Decrement movement by one
        setMovementPlan(getMovementPlan()-1);
        
        //6bd, Checks if movement is blocked. If it is, change direction/movement plan
        if (getWorld()->isBacteriumMovementBlockedAt(this, 3)) {
            setDirection(randInt(0, 359));
            setMovementPlan(10);
        }
        
        //6c, If bacteria can move then move to the new location
        else
            moveAngle(getDirection(), 3);
        
        //6e, return
        return;
    }
    
    //Step 7, If movement plan is <= 0, move to closet food
    else {
        //7b, food can be found within 128 pixels
        int angle;
        if(getWorld()->getAngleToNearestNearbyEdible(this, angle)) {
            setDirection(angle);
            setMovementPlan(10);
            if (getWorld()->isBacteriumMovementBlockedAt(this, 3)) {
                setDirection(randInt(0, 359));
                setMovementPlan(10);
                return;
            }
        }
        
        //7a, food can't be found within 128 pixels
        else {
            setDirection(randInt(0, 359));
            setMovementPlan(10);
            return;
        }
    }
}

// // // // // // // // // // // // // // // // // // // // // // //
//                             EColi                              //
// // // // // // // // // // // // // // // // // // // // // // //
EColi::EColi(int imageID, int startX, int startY, int depth, StudentWorld* studWorld, int health) : Bacterium(imageID, startX, startY, depth, studWorld, health) {
}

EColi::~EColi() {
}

int EColi::soundWhenHurt() const {
    return SOUND_ECOLI_HURT;
}

int EColi::soundWhenDie() const {
    return SOUND_ECOLI_DIE;
}

void EColi::doSomething() {

    //Step 1, Check if alive and if not, return
    if(!isAlive())
        return;
    
    //Step 2, Check if overlap with Socrates. If so, damage and go to Step 5
    bool skipToFive = false;
    if(getWorld()->getOverlappingSocrates(this)) {
        getWorld()->getSocrates()->takeDamage(4);
        skipToFive = true;
    }
    
    //If skipToFive is false AKA Socrates didn't get hurt
    if (!skipToFive) {
        //Step 3, Check to see if it's eaten 3 foods. If so, divide + skip to 5
        if (getFoodEaten() == 3) {
            //3a, Create new X coord
            int newx = getX();
            if(newx < VIEW_WIDTH/2)
                newx += SPRITE_WIDTH/2;
            if(newx > VIEW_WIDTH/2)
                newx -= SPRITE_WIDTH/2;
            
            //3b, Create new Y coord
            int newy = getY();
            if(newy < VIEW_HEIGHT/2)
                newy += SPRITE_WIDTH/2;
            if(newy > VIEW_HEIGHT/2)
                newy -= SPRITE_WIDTH/2;
            
            //3c, Create new Salmonella Object
            EColi* p = new EColi(IID_ECOLI, newx, newy, 0, getWorld(), 5);
            getWorld()->addActor(p);
            
            //3de, Reset Food Items Eaten and then skip to 5
            setFoodEaten(0);
            skipToFive = true;
        }
        
        //Step 4, Check if it's overlapping with one or more food items. Only happens if Step 3 doesn't happen.
        if ((!skipToFive) && (getWorld()->isFoodHere(getX(), getY()))) {
            //p->setAlive(false);
            setFoodEaten(getFoodEaten()+1);
        }
    }
    
    //Step 5, Checks to see if movement plan > 0
    
    //Step 5a, gets the theta towards Socrates
    int socAngle;
    if(getWorld()->getAngleToNearbySocrates(this, 256, socAngle)) {
        int count = 0;
        setDirection(socAngle);
        
        //5b, Moves towards Socrates, repeat up to 10 times
        while (count <= 10) {
            if (getWorld()->isBacteriumMovementBlockedAt(this, 2)) {
                setDirection(getDirection()+10);
                count++;
            }
            else {
                moveAngle(getDirection(), 2);
                return;
            }
        }
        
        //5c, If more than 10 tries, do nothing
        if(count >= 10)
            return;
    }
}
