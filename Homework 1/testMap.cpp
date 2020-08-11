//
//  testMap.cpp
//  Homework 1
//
//  Created by Cody Do on 1/15/20.
//  Copyright © 2020 Cody Do. All rights reserved.
//

#include "newMap.h"
#include <iostream>
#include <cassert>
using namespace std;

int main()
{
    Map m;  // maps strings to doubles
    assert(m.empty());
    ValueType v = -1234.5;
    assert( !m.get("abc", v)  &&  v == -1234.5); // v unchanged by get failure
    m.insert("xyz", 9876.5);
    assert(m.size() == 1);
    KeyType k = "hello";
    assert(m.get(0, k, v)  &&  k == "xyz"  &&  v == 9876.5);
    cout << "Passed all tests" << endl;
    
    Map x,y;
    x.insert("hey",10);
    x.insert("there",100);
    y.insert("what", 1);
    x.insert("buddy",1000);
    y.insert("is",2);
    y.insert("up?",3);
    y.insert(":)",4);
    cout << "x has a size of " << x.size() << " and has this info: " << endl;
    x.dump();
    cout << "y has a size of " << y.size() << " and has this info: " <<  endl;
    y.dump();
    
    Map z;
    if (z.empty())
        cout << "z is empty!" << endl;
    
    x.update("buddy",1111);
    x.erase("there");
    cout << "New x: " << endl;
    x.dump();
}
