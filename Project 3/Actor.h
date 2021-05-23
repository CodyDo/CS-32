#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class StudentWorld;

// // // // // // // // // // // // // // // // // // // // // // //
//                          Actor                                 //
// // // // // // // // // // // // // // // // // // // // // // //

class Actor : public GraphObject {
  public:
    //Constructor & Destructor
    Actor(int imageID, int startX, int startY, int depth, StudentWorld* studWorld);
    virtual ~Actor();
    
    //Accessors
    bool isAlive() const;
    virtual bool preventsLevelCompleting() const;
    StudentWorld* getWorld() const;
    virtual bool canDamage() const;
    virtual bool blocksBacteriumMovement() const;
    virtual bool isEdible() const;
    
    //Mutators
        void setAlive(bool x);
        virtual void takeDamage(int x = 0) = 0;
        virtual void doSomething() = 0;
    
  private:
    StudentWorld* m_studWorld;
    bool m_alive;
};

// // // // // // // // // // // // // // // // // // // // // // //
//                          Agent                                 //
// // // // // // // // // // // // // // // // // // // // // // //
class Agent: public Actor {
  public:
    //Constructor & Destructor
    Agent(int imageID, int startX, int startY, int depth, StudentWorld* studWorld, int health);
    virtual ~Agent();
    
    //Accessors
    int getHealth() const;

    //Mutators
    void restoreHealth();
    void takeDamage(int x = 0);
    
  private:
    int m_health;
};

// // // // // // // // // // // // // // // // // // // // // // //
//                          Socrates                              //
// // // // // // // // // // // // // // // // // // // // // // //

class Socrates : public Agent {
  public:
    //Constructor & Destructor
    Socrates(int imageID, int startX, int startY, int depth, StudentWorld* studWorld);
    virtual ~Socrates();
    
    //Accessors
    int getSprays() const;
    int getFlames() const;
    virtual bool canDamage() const;

    //Mutators
    virtual void doSomething();
    void addFlames();
    void takeDamage(int x = 0);
    
  private:
    int m_nSprays;
    int m_nFlames;
    bool rechargeSpray;
};

// // // // // // // // // // // // // // // // // // // // // // //
//                             Dirt                               //
// // // // // // // // // // // // // // // // // // // // // // //

class Dirt : public Actor {
  public:
    //Constructor & Destructor
    Dirt(int imageID, int startX, int startY, int depth, StudentWorld* studWorld);
    virtual ~Dirt();
    
    //Accessors
    virtual void takeDamage(int x = 0);
    virtual bool blocksBacteriumMovement() const;

    //Mutators
    virtual void doSomething();
    
};

// // // // // // // // // // // // // // // // // // // // // // //
//                             Food                               //
// // // // // // // // // // // // // // // // // // // // // // //

class Food : public Actor {
  public:
    //Constructor & Destructor
    Food(int imageID, int startX, int startY, int depth, StudentWorld* studWorld);
    virtual ~Food();
    
    //Accessors
    virtual bool canDamage() const;
    virtual void takeDamage(int x = 0);
    virtual bool isEdible() const;
    
    //Mutators
    virtual void doSomething();
    
};

// // // // // // // // // // // // // // // // // // // // // // //
//                          Projectile                            //
// // // // // // // // // // // // // // // // // // // // // // //
class Projectile: public Actor {
  public:
    //Constructor & Destructor
    Projectile(int imageID, int startX, int startY, int depth, StudentWorld* studWorld, int distance, int direction);
    virtual ~Projectile();
    
    //Accessors
    virtual bool canDamage() const;
    virtual void takeDamage(int x = 0);
    int availDist() const;
    
    //Mutators
    void changeAvailDist(int x);
    virtual void doSomething();
    virtual void doSomethingSpecific() = 0;
    
  private:
    int m_availDist;
};

// // // // // // // // // // // // // // // // // // // // // // //
//                             Spray                              //
// // // // // // // // // // // // // // // // // // // // // // //
class Spray : public Projectile {
  public:
    //Constructor & Destructor
    Spray(int imageID, int startX, int startY, int depth, StudentWorld* studWorld, int distance, int direction);
    virtual ~Spray();
    
    //Mutators
    virtual void doSomethingSpecific();
    
};

// // // // // // // // // // // // // // // // // // // // // // //
//                             Flame                              //
// // // // // // // // // // // // // // // // // // // // // // //
class Flame : public Projectile {
  public:
    //Constructor & Destructor
    Flame(int imageID, int startX, int startY, int depth, StudentWorld* studWorld, int distance, int direction);
    virtual ~Flame();
    
    //Mutators
    virtual void doSomethingSpecific();
    
};

// // // // // // // // // // // // // // // // // // // // // // //
//                             Goodie                             //
// // // // // // // // // // // // // // // // // // // // // // //
class Goodie : public Actor {
  public:
    //Constructor & Destructor
    Goodie(int imageID, int startX, int startY, int depth, StudentWorld* studWorld);
    virtual ~Goodie();
    
    //Mutators
    virtual void takeDamage(int x = 0);
    virtual void doSomething();
    virtual void doGoodieTask() = 0;
    
  private:
    int m_lifetime;
    
};

// // // // // // // // // // // // // // // // // // // // // // //
//                     Restore Health Goodie                      //
// // // // // // // // // // // // // // // // // // // // // // //
class RestoreHealthGoodie : public Goodie {
  public:
    //Constructor & Destructor
    RestoreHealthGoodie(int imageID, int startX, int startY, int depth, StudentWorld* studWorld);
    virtual ~RestoreHealthGoodie();
    
    //Mutators
    virtual void doGoodieTask();
    
};

// // // // // // // // // // // // // // // // // // // // // // //
//                     Flamethrower Goodie                        //
// // // // // // // // // // // // // // // // // // // // // // //
class FlamethrowerGoodie : public Goodie {
  public:
    //Constructor & Destructor
    FlamethrowerGoodie(int imageID, int startX, int startY, int depth, StudentWorld* studWorld);
    virtual ~FlamethrowerGoodie();
    
    //Mutators
    virtual void doGoodieTask();
    
};

// // // // // // // // // // // // // // // // // // // // // // //
//                      Extra Life Goodie                         //
// // // // // // // // // // // // // // // // // // // // // // //
class ExtraLifeGoodie : public Goodie {
  public:
    //Constructor & Destructor
    ExtraLifeGoodie(int imageID, int startX, int startY, int depth, StudentWorld* studWorld);
    virtual ~ExtraLifeGoodie();
    
    //Mutators
    virtual void doGoodieTask();
    
};

// // // // // // // // // // // // // // // // // // // // // // //
//                            Fungus                              //
// // // // // // // // // // // // // // // // // // // // // // //
class Fungus : public Goodie {
  public:
    //Constructor & Destructor
    Fungus(int imageID, int startX, int startY, int depth, StudentWorld* studWorld);
    virtual ~Fungus();
    
    //Mutators
    virtual void doGoodieTask();
    
};

// // // // // // // // // // // // // // // // // // // // // // //
//                              Pit                               //
// // // // // // // // // // // // // // // // // // // // // // //
class Pit: public Actor {
  public:
    //Constructor & Destructor
    Pit(int imageID, int startX, int startY, int depth, StudentWorld* studWorld);
    virtual ~Pit();
    
    //Accessors
    virtual bool preventsLevelCompleting() const;
    virtual bool canDamage() const;
    virtual void takeDamage(int x = 0);

    //Mutators
    virtual void doSomething();
    
  private:
    int m_nRegSal;
    int m_nAggroSal;
    int m_nEColi;
    
    //Helper function that returns an int based on which bacterium are still available in the pit.
    // 0 = Regular Salmonella    1 = Aggressive Salmonella    2 = EColi
    int pickBacterium();
};

// // // // // // // // // // // // // // // // // // // // // // //
//                         Bacterium                              //
// // // // // // // // // // // // // // // // // // // // // // //
class Bacterium : public Agent {
  public:
    //Constructor & Destructor
    Bacterium(int imageID, int startX, int startY, int depth, StudentWorld* studWorld, int health);
    virtual ~Bacterium();
    
    //Accessors
    virtual bool preventsLevelCompleting() const;
    int getFoodEaten() const;
    int getMovementPlan() const;
    virtual int soundWhenHurt() const = 0;
    virtual int soundWhenDie() const = 0;

    //Mutators
    void setFoodEaten(int x);
    void setMovementPlan(int x);
    virtual void takeDamage(int x = 0);
    
  private:
    int m_movementPlanDist;
    int m_foodEaten;
    
};

// // // // // // // // // // // // // // // // // // // // // // //
//                         RegSalmonella                          //
// // // // // // // // // // // // // // // // // // // // // // //
class RegSalmonella : public Bacterium {
  public:
    //Constructor & Destructor
    RegSalmonella(int imageID, int startX, int startY, int depth, StudentWorld* studWorld, int health);
    virtual ~RegSalmonella();

    //Accessors
    virtual int soundWhenHurt() const;
    virtual int soundWhenDie() const;

    //Mutators
    virtual void doSomething();

};

// // // // // // // // // // // // // // // // // // // // // // //
//                       AggroSalmonella                          //
// // // // // // // // // // // // // // // // // // // // // // //
class AggroSalmonella : public Bacterium {
  public:
    //Constructor & Destructor
    AggroSalmonella(int imageID, int startX, int startY, int depth, StudentWorld* studWorld, int health);
    virtual ~AggroSalmonella();

    //Accessors
    virtual int soundWhenHurt() const;
    virtual int soundWhenDie() const;
    
    //Mutators
    virtual void doSomething();

};

// // // // // // // // // // // // // // // // // // // // // // //
//                             EColi                              //
// // // // // // // // // // // // // // // // // // // // // // //
class EColi: public Bacterium {
  public:
    //Constructor & Destructor
    EColi(int imageID, int startX, int startY, int depth, StudentWorld* studWorld, int health);
    virtual ~EColi();

    //Accessors
    virtual int soundWhenHurt() const;
    virtual int soundWhenDie() const;
    
    //Mutators
    virtual void doSomething();

};


#endif // ACTOR_H_

