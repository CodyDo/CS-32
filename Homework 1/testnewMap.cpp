//
//  testnewMap.cpp
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
    
    Map a(1000);   // a can hold at most 1000 key/value pairs
    Map b(5);      // b can hold at most 5 key/value pairs
    Map c;         // c can hold at most DEFAULT_MAX_ITEMS key/value pairs
    KeyType k[6] = { "I","Want","Good","Pho","Right","Now"};
    ValueType v  = 14;

      // No failures inserting pairs with 5 distinct keys into b
    for (int n = 0; n < 5; n++)
        assert(b.insert(k[n], v));

      // Failure if we try to insert a pair with a sixth distinct key into b
    assert(!b.insert(k[5], v));

      // When two Maps' contents are swapped, their capacities are swapped
      // as well:
    a.swap(b);
    assert(!a.insert(k[5], v)  &&  b.insert(k[5], v));
    cout << "All assert-provided tests successful." << endl;
    cout << "Test for proper negative handling (If exit, then it worked): " << endl;
    Map(-1);
}

