//
//  event.cpp
//  Homework 3
//
//  Created by Cody Do on 2/8/20.
//  Copyright © 2020 Cody Do. All rights reserved.
//

#include <iostream>
#include <string>
using namespace std;

class Event {
 public:
    Event(string n) {
        m_name = n;
    }
    
    virtual ~Event() {
    }
    
    string name() const {
        return m_name;
    }
    
    virtual bool isSport() const = 0;
    virtual string need() const = 0;
    
 private:
    string m_name;
};
// // // // // // // // // // // // // //
class BasketballGame : public Event {
 public:
    BasketballGame(string n) : Event(n) {
        m_needs = "hoops";
    }
    
    virtual ~BasketballGame() {
        cout << "Destroying the " << name() << " basketball game" << endl;
    }
    
    virtual bool isSport() const {
        return true;
    }
    
    virtual string need() const {
        return m_needs;
    }
    
 private:
    string m_needs;
};
// // // // // // // // // // // // // //
class Concert : public Event {
 public:
    Concert(string n, string genre) : Event(n) {
        m_genre = genre;
        m_needs = "a stage";
    }
    
    virtual ~Concert() {
        cout << "Destroying the " << name() << " " << m_genre << " concert" << endl;
    }
    
    virtual bool isSport() const {
        return false;
    }
    
    virtual string need() const {
        return m_needs;
    }
    
 private:
    string m_genre;
    string m_needs;
};
// // // // // // // // // // // // // //
class HockeyGame : public Event {
 public:
    HockeyGame(string n) : Event(n) {
        m_needs = "ice";
    }
    
    virtual ~HockeyGame() {
        cout << "Destroying the " << name() << " hockey game" << endl;
    }
    
    virtual bool isSport() const {
        return true;
    }
    
    virtual string need() const {
        return m_needs;
    }
    
 private:
    string m_needs;
};
// // // // // // // // // // // // // //
void display(const Event* e)
{
    cout << e->name() << ": ";
    if (e->isSport())
    cout << "(sport) ";
    cout << "needs " << e->need() << endl;
}

/*int main()
{
    Event* events[4];
    events[0] = new BasketballGame("Lakers vs. Suns");
      // Concerts have a name and a genre.
    events[1] = new Concert("Banda MS", "banda");
    events[2] = new Concert("KISS", "hard rock");
    events[3] = new HockeyGame("Kings vs. Flames");

    cout << "Here are the events." << endl;
    for (int k = 0; k < 4; k++)
    display(events[k]);

      // Clean up the events before exiting
    cout << "Cleaning up." << endl;
    for (int k = 0; k < 4; k++)
    delete events[k];
} */


