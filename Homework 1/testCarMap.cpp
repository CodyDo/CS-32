//
//  testCarMap.cpp
//  Homework 1
//
//  Created by Cody Do on 1/15/20.
//  Copyright © 2020 Cody Do. All rights reserved.
//

#include "CarMap.h"
#include <iostream>
using namespace std;

int main() {
    CarMap x;
    if (x.fleetSize() == 0)
        cout << "fleetSize works for = 0" << endl;
    x.addCar("Paul");
    x.addCar("Chelsea");
    x.addCar("Tam");
    cout << "There are now " << x.fleetSize() << " cars." << endl;
    cout << "Test for proper 0 miles start: " << endl;
    x.print();
    x.drive("Paul", 20);
    x.drive("Chelsea", 25);
    x.drive("Tam", 30);
    cout << "Tam has driven " << x.miles("Tam") << " miles" << endl;
    cout << "Everyone: " << endl;
    x.print();
    if (x.miles("Paul") == 20 && x.miles("Chelsea") == 25)
        cout << "Yep, still working" << endl;
}


