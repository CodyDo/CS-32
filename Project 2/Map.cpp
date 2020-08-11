//
//  Map.cpp
//  Project 2
//
//  Created by Cody Do on 1/22/20.
//  Copyright © 2020 Cody Do. All rights reserved.
//

#include "Map.h"
#include <iostream>
using namespace std;

Map::Map() { //Constructor
    m_mapSize = 0;
}

Map::~Map() { //Destructor; Since Map uses the LinkedList private class to handle all the information, no destructor is necessary (the LinkedList destructor, written in the header file, will be called at the end)
}

Map::Map(const Map &src) { // Copy Constructor
    m_mapSize = src.m_mapSize; //Sets map size of new Map = to source map
    for (int k = 0; k < m_mapSize; k++) { //Copies key/value pair of each node and adds it to the new map m_mapSize times
        KeyType key;
        ValueType value;
        src.get(k, key, value);
        m_map.addToRear(key, value);
    }
}

Map& Map::operator=(const Map &src) { // Assignment Operator
    m_map.clearList(); //Essentially a callable destructor -- removes all items in the target map
    
    m_mapSize = src.m_mapSize; //Set target map equal to source map
    for (int k = 0; k < src.m_mapSize; k++) {
        KeyType key;
        ValueType value;
        src.get(k, key, value);
        m_map.addToRear(key, value);
    }
    return *this;
}

bool Map::empty() const { //Done
    if (m_mapSize == 0)
        return true;
    else
        return false;
}

int Map::size() const { //Done
    return m_mapSize;
}

bool Map::insert(const KeyType& key, const ValueType& value) {
    if (m_map.findItem(key))
        return false;
    else {
        m_map.addToRear(key, value);
        m_mapSize++;
        return true;
    }
}

bool Map::update(const KeyType& key, const ValueType& value) {
    return m_map.updateItem(key, value);
}

bool Map::insertOrUpdate(const KeyType& key, const ValueType& value) {
    if (m_map.findItem(key)) { //If map has key, update the value
        m_map.updateItem(key, value);
        return true;
    }
    else { //If map doesn't have key, insert it
        m_map.addToRear(key, value);
        m_mapSize++;
        return true;
    }
}

bool Map::erase(const KeyType& key) {
    if (m_map.findItem(key)) { //If map has key, delete key, return true
        m_map.deleteItem(key);
        m_mapSize--;
        return true;
    }
    return false;
}

bool Map::contains(const KeyType& key) const {
    return m_map.findItem(key);
}

bool Map::get(const KeyType& key, ValueType& value) const {
    if (m_map.findItem(key)) { //If item exists, call getItem1 and return true
        m_map.getItem1(key, value);
        return true;
    }
    return false;
}

bool Map::get(int i, KeyType& key, ValueType& value) const {
    if (i >= 0 && i < m_mapSize) {
        m_map.getItem2(i, key, value);
        return true;
    }
    return false;
}

void Map::swap(Map& other) {
    Map temp = other;
    other = *this;
    *this = temp;
}

void Map::dump() {
    if (m_mapSize == 0)
        cerr << "Map is empty" << endl;
    else
        m_map.printItems();
}

bool combine(const Map& m1, const Map& m2, Map& result) {
    if (m1.empty() == true) { //Checks if either list is empty. If it is, return true and set result to the other
        result = m2;
        return true;
    }
    if (m2.empty() == true) {
        result = m1;
        return true;
    }
    
    Map combinedMap = m2; //Replicates m2. This allows us to ignore size differences.
    bool valMatch = true; //Set standard for matching
    
    for (int k = 0; k < m1.size(); k++) {
        KeyType key;
        ValueType value;
        m1.get(k, key, value);
        if (m2.contains(key)) { //If second list has key
            ValueType valueComp;
            m2.get(key, valueComp);
            if (value != valueComp) { //Check if keys match. If not, remove key from list and change valMatch to false.
                combinedMap.erase(key);
                valMatch = false;
            }
        }
        else //if m2 doesn't have the key from m1, add it to the combined list
            combinedMap.insert(key, value);
    }
    result = combinedMap;
    return valMatch;
}

void reassign(const Map& m, Map& result) {
    if (m.size() <= 1) { //Checks if the map consists of one or no people. Set result to the map and end function.
        result = m;
        return;
    }
    
    //Code to handle swapping the final node's value into the top node + storing top node's value for movement later
    Map reassignedMap = m;
    KeyType topKey;
    ValueType topValue;
    reassignedMap.get(0, topKey, topValue); //Stores value of top item away
    
    KeyType bottomKey;
    ValueType bottomValue;
    reassignedMap.get(reassignedMap.size()-1, bottomKey, bottomValue); //Stores value of bottom item away
    reassignedMap.update(topKey, bottomValue);
    
    //For loop that starts on the second node and starts shifting items down. Stores values from each iteration to be used.
    for (int k = 1; k < reassignedMap.size(); k++) {
        KeyType key;
        ValueType value;
        reassignedMap.get(k, key, value);
        reassignedMap.update(key, topValue);
        topValue = value; //Stores variable for next iteration
    }
    result = reassignedMap;
}

