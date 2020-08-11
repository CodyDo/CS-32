//
//  newMap.cpp
//  Homework 1
//
//  Created by Cody Do on 1/15/20.
//  Copyright © 2020 Cody Do. All rights reserved.
//

#include "newMap.h"
#include <iostream>
using namespace std;

Map::Map() {   // Create an empty map (i.e., one with no key/value pairs)
    m_size = DEFAULT_MAX_ITEMS;
    m_numPairs = 0;
    m_pairs = new pair[DEFAULT_MAX_ITEMS];
}

Map::Map(int n) { // Create a map with a specified size
    if (n < 0) {
        cout << "Error! Input number must be non-negative." << endl;
        exit(1);
    }
    m_size = n;
    m_numPairs = 0;
    m_pairs = new pair[n];
}

Map::~Map() {  // Destructs the map object
    delete [] m_pairs;
}

Map::Map(const Map &src) {  // Copy Operator for Map
    m_size = src.m_size;
    m_numPairs = src.m_numPairs;
    m_pairs = new pair[m_size];
    for (int k = 0; k < m_size; k++)
        m_pairs[k] = src.m_pairs[k];
}

Map& Map::operator=(const Map &src) {  // Assignment Operator for Map
    delete [] m_pairs;
    m_size = src.m_size;
    m_numPairs = src.m_numPairs;
    m_pairs = new pair[m_size];
    for (int k = 0; k < m_size; k++)
        m_pairs[k] = src.m_pairs[k];
    return *this;
}

bool Map::empty() const {  // Return true if the map is empty, otherwise false.
    if(m_numPairs == 0)
        return true;
    else
        return false;
}

int Map::size() const {  // Return the number of key/value pairs in the map.

    return m_numPairs;
}

bool Map::insert(const KeyType& key, const ValueType& value) {
  // If key is not equal to any key currently in the map, and if the
  // key/value pair can be added to the map, then do so and return true.
  // Otherwise, make no change to the map and return false (indicating
  // that either the key is already in the map, or the map has a fixed
  // capacity and is full).
    for(int k = 0; k < m_numPairs; k++) { //Checks if key already exists
        if (m_pairs[k].key == key)
            return false;
    }
    if (m_numPairs == m_size) //Checks if max amount is reached
        return false;
    
    m_pairs[m_numPairs].key = key;
    m_pairs[m_numPairs].value = value;
    m_numPairs++;

    return true;
}

bool Map::update(const KeyType& key, const ValueType& value) {
  // If key is equal to a key currently in the map, then make that key no
  // longer map to the value it currently maps to, but instead map to
  // the value of the second parameter; return true in this case.
  // Otherwise, make no change to the map and return false.
    for(int k = 0; k < m_numPairs; k++) { //Checks if key already exists
        if (m_pairs[k].key == key) {
            m_pairs[k].value = value;
            return true;
        }
    }
    return false;
}


bool Map::insertOrUpdate(const KeyType& key, const ValueType& value) {
  // If key is equal to a key currently in the map, then make that key no
  // longer map to the value that it currently maps to, but instead map to
  // the value of the second parameter; return true in this case.
  // If key is not equal to any key currently in the map and if the
  // key/value pair can be added to the map, then do so and return true.
  // Otherwise, make no change to the map and return false (indicating
  // that the key is not already in the map and the map has a fixed
  // capacity and is full).
    for(int k = 0; k < m_numPairs; k++) { //Checks if key already exists
        if (m_pairs[k].key == key) {
            m_pairs[k].value = value;
            return true;
        }
    }
    if (m_numPairs != m_size) { //Checks if max amount is reached. If isn't add new pair.
        m_pairs[m_numPairs].key = key;
        m_pairs[m_numPairs].value = value;
        m_numPairs++;
        return true;
    }
    return false;
}
    
bool Map::erase(const KeyType& key) {
  // If key is equal to a key currently in the map, remove the key/value
  // pair with that key from the map and return true.  Otherwise, make
  // no change to the map and return false.
    for (int k = 0; k < m_numPairs; k++) {
        if (m_pairs[k].key == key) {
            if (m_numPairs == 1) {
                m_numPairs--;
                return true;
            }
            else {
                m_pairs[k] = m_pairs[m_numPairs-1];
                m_numPairs--;
                return true;
            }
        }
    }
    return false;
}
 
bool Map::contains(const KeyType& key) const {
  // Return true if key is equal to a key currently in the map, otherwise
  // false.
    for(int k = 0; k < m_numPairs; k++) {
        if (m_pairs[k].key == key)
            return true;
    }
    return false;
}
 
bool Map::get(const KeyType& key, ValueType& value) const {
  // If key is equal to a key currently in the map, set value to the
  // value in the map which that key maps to, and return true.  Otherwise,
  // make no change to the value parameter of this function and return
  // false.
    for(int k = 0; k < m_numPairs; k++) {
        if (m_pairs[k].key == key) {
            value = m_pairs[k].value;
            return true;
        }
    }
    return 0;
}
 
bool Map::get(int i, KeyType& key, ValueType& value) const {
  // If 0 <= i < size(), copy into the key and value parameters the
  // key and value of one of the key/value pairs in the map and return
  // true.  Otherwise, leave the key and value parameters unchanged and
  // return false.
    if (i >= 0 && i < m_numPairs)
    {
        key = m_pairs[i].key;
        value = m_pairs[i].value;
        return true;
    }
    return false;
}

void Map::swap(Map& other) {
  // Exchange the contents of this map with the other one.
    Map temp = other;
    other = *this;
    *this = temp;
}

void Map::dump() const {
    if (m_numPairs == 0)
        cerr << "The pairs array is empty." << endl;
    int counter = 0;
    for (int k = 0; k < m_numPairs; k++) {
        KeyType key;
        ValueType val;
        get(k, key, val);
        cerr << "Position " << counter << ": The Key: " << key << "  || The Value: " << val << " " << endl;
        counter++;
    }
}

