//
//  ExpandableHashMap.h
//  Project 4
//
//  Created by Cody Do on 3/4/20.
//  Copyright © 2020 Cody Do. All rights reserved.
//

#ifndef ExpandableHashMap_h
#define ExpandableHashMap_h

// Skeleton for the ExpandableHashMap class template.  You must implement the first six
// member functions.

#include <vector>

template<typename KeyType, typename ValueType>
class ExpandableHashMap
{
public:
    ExpandableHashMap(double maximumLoadFactor = 0.5);
    ~ExpandableHashMap();
    void reset();
    int size() const;
    void associate(const KeyType& key, const ValueType& value);

      // for a map that can't be modified, return a pointer to const ValueType
    const ValueType* find(const KeyType& key) const;

      // for a modifiable map, return a pointer to modifiable ValueType
    ValueType* find(const KeyType& key)
    {
        return const_cast<ValueType*>(const_cast<const ExpandableHashMap*>(this)->find(key));
    }

      // C++11 syntax for preventing copying and assignment
    ExpandableHashMap(const ExpandableHashMap&) = delete;
    ExpandableHashMap& operator=(const ExpandableHashMap&) = delete;

private:
    double getLoad() const {
        return static_cast<double>(m_count)/static_cast<double>(m_numBuckets);
    }
    
    unsigned int getBucketNum(const KeyType& key) const
    {
        unsigned int hasher(const KeyType& k); // prototype
        unsigned int h = hasher(key);
        return h;
    }
    
    //Node & LinkedList Structure (Based on Project 2)
    struct Node {
         KeyType key;
         ValueType value;
         Node *next;
     };
     
     class LinkedList {
       public:
         LinkedList() {
             head = nullptr;
             m_countLL = 0;
         }
         
         ~LinkedList() {
             Node *p;
             p = head;
             while (p != nullptr) { //Runs off the edge
                 Node *n = p->next; //Pointer to hold value of next node
                 delete p;   //Delete node and move on to the next node
                 p = n;
             }
         }
         
         void addToFront (KeyType key, ValueType value) {
             Node *p = new Node; //Set up a new node and fill with info
             p->key = key;
             p->value = value;
             if (head == nullptr) { //If head = nullptr then list is empty
                 p->next = nullptr;
                 head = p;
                 
             }
             else {
                 p->next = head;
                 head = p;
             }
             m_countLL++;
         }
         
         void addToRear (KeyType key, ValueType value) {
             if (head == nullptr)
                 addToFront(key,value);
             else {
                 Node *p;
                 p = head; //start at top node
                 while (p->next != nullptr) //Traverse to the last node
                     p = p->next;
                 Node *n = new Node;     //New last node
                 n->key = key;
                 n->value = value;
                 p->next = n;       //Link the old last node to new last node
                 n->next = nullptr;
                 m_countLL++;
             }

         }
         
         void deleteItem(KeyType key) {
             if (head == nullptr) //Makes sure to not delete an empty list
                 return;
             if (head->key == key) { //Deleting the first node
                 Node* killMePls = head;
                 head = head->next;
                 if (head == nullptr) { //Double checks to make sure nullptr->next isn't accessed (if you're deleting the only thing in list)
                     delete killMePls;
                     m_countLL--;
                 }
                 else {
                     head->prev = nullptr;
                     delete killMePls;
                     m_countLL--;
                 }
             }
             Node* p = head;
             while (p != nullptr) { //Cycle through list and find item to be deleted
                 if (p->next != nullptr && p->next->key == key)
                     break;
                 p = p->next;
             }
             if (p != nullptr) { //If the item was found
                 Node* killMePls = p->next;
                 if (p->next->next == nullptr) { //Checks if last item in list
                     p->next = nullptr;
                     delete killMePls;
                     m_countLL--;
                 }
                 else {
                     p->next = killMePls->next;
                     killMePls->next->prev = p;
                     delete killMePls;
                     m_countLL--;
                 }
             }
         }
         
         bool findItem(KeyType key) const {
             Node *p = head;
             while (p != nullptr) {
                 if (p->key == key)
                     return true;
                 p = p->next;
             }
             return false;
         }
         
         bool updateItem(KeyType key, ValueType value) {
             Node *p = head;
             while (p != nullptr) { //Scan through list
                 if (p->key == key) {
                     p->value = value; //Update value if key exists
                     return true;
                 }
                 p = p->next; //Move forward in list if keys don't match
             }
             return false;
         }
         
         bool getItem1(const KeyType &key, ValueType*& value) const {
             Node *p = head;
             while (p != nullptr) { //Scan through list
                 if (p->key == key) { //If keys match, set parameter value to pair's value
                     value = &p->value;
                     return true;
                 }
                 p = p->next;
             }
             return false;
         }
         
         bool getItem2(int i, KeyType &key, ValueType &value) const {
             Node *p = head;
             int counter = 0; //Creates a local counter to scan through list
             while (p != nullptr) { //Scan through the list
                 if (i == counter) { //If local counter and input int match, copy information over to parameter key and value
                     key = p->key;
                     value = p->value;
                     return true;
                 }
                 counter++;
                 p = p->next;
             }
             return false;
         }
         
         int getCount() {
             return m_countLL;
         }
         
     private:
         Node* head;
         int m_countLL;
     };
    
    int m_numBuckets;
    int m_count;
    double m_maxLoadFactor;
    std::vector<LinkedList*> m_listVect;
    
};

template<typename KeyType, typename ValueType>
ExpandableHashMap<KeyType, ValueType>::ExpandableHashMap(double maximumLoadFactor)
{
    m_listVect.resize(8);
    m_numBuckets = 8;
    m_count = 0;
    
    typename std::vector<LinkedList*>::iterator it = m_listVect.begin();
    for (int k = 0;it != m_listVect.end(); k++) {
        m_listVect[k] = nullptr;
        it++;
    }
    
    if (maximumLoadFactor <= 0 || maximumLoadFactor >= 1.0)
        m_maxLoadFactor = 0.5;
    else
        m_maxLoadFactor = maximumLoadFactor;
}

template<typename KeyType, typename ValueType>
ExpandableHashMap<KeyType, ValueType>::~ExpandableHashMap()
{
    typename std::vector<LinkedList*>::iterator it = m_listVect.begin();
    while (it != m_listVect.end()) {//Go through vector of linked lists
        if (*it != nullptr) //If pointer is active (AKA points to LL), clear it (destroy it)
            delete *it;
        it++;
    }
}

template<typename KeyType, typename ValueType>
void ExpandableHashMap<KeyType, ValueType>::reset()
{
    //Destroys all items in the current HashMap
    typename std::vector<LinkedList*>::iterator it = m_listVect.begin();
    while (it != m_listVect.end()) {
        if (*it != nullptr)
            delete *it;
        it++;
    }
    
    //Resets bucket amount and number of items
    m_numBuckets = 8;
    m_count = 0;
    
    //Vector is set the standard/basic array
    m_listVect.resize(8);
}

template<typename KeyType, typename ValueType>
int ExpandableHashMap<KeyType, ValueType>::size() const
{
    return m_count;
}

template<typename KeyType, typename ValueType>
void ExpandableHashMap<KeyType, ValueType>::associate(const KeyType& key, const ValueType& value)
{
    //Gets Bucket Number
    int bucketNum = getBucketNum(key) % m_numBuckets;
    
    //Check to see if the bucket is null in the array, if it is, make a new list and add the value;
    if (m_listVect[bucketNum] == nullptr) {
        m_listVect[bucketNum] = new LinkedList;
        m_listVect[bucketNum]->addToRear(key, value);
        m_count++;
    }
    
    //If it's not null, check to see if it exists
    else {
        //If it exists, update the item and return
        if (m_listVect[bucketNum]->findItem(key)) {
            m_listVect[bucketNum]->updateItem(key,value);
            return; //Only checks loadfactor when adding an item
        }
        //If it doesn't, then add the item.
        else {
            m_listVect[bucketNum]->addToRear(key, value);
            m_count++;
        }
    }
    
    //Checks to see if the Load is greater than the Max Load
    if (getLoad() > m_maxLoadFactor) {
        //Creates a copy of the current vector to work with. Clears original vector and doubles the size. Sets all pointers in vector to nullptr.
        std::vector<LinkedList*> temp = m_listVect;
        m_listVect.clear();
        m_listVect.resize(m_numBuckets*2);
        typename std::vector<LinkedList*>::iterator it = m_listVect.begin();
        while (it != m_listVect.end()) {
            (*it) = nullptr;
            it++;
        }
        
        //Scans through copied vector of linked lists, checks if linked list is active. If it is, goes through the entire linked list and rehashes the keys with respect to the new, larger vector. Adds it to the array.
        for (int k = 0; k < m_numBuckets; k++) {
            if (temp[k] != nullptr) {
                for (int j = 0; j < temp[k]->getCount(); j++) {
                    KeyType key1;
                    ValueType val1;
                    temp[k]->getItem2(j, key1, val1);
                    
                    int bucketNum2 = getBucketNum(key1)%(m_numBuckets*2);
                    
                    if (m_listVect[bucketNum2] == nullptr) {
                        m_listVect[bucketNum2] = new LinkedList;
                        m_listVect[bucketNum2]->addToRear(key1, val1);
                    }
                    else
                        m_listVect[bucketNum2]->addToRear(key1, val1);
                }
                delete temp[k]; //Removes dynamic memory of old list once it's been rehashed over
            }
        }
        m_numBuckets = m_numBuckets*2;
    }
}

template<typename KeyType, typename ValueType>
const ValueType* ExpandableHashMap<KeyType, ValueType>::find(const KeyType& key) const
{
    int bucketNum = getBucketNum(key) % m_numBuckets;
    ValueType* v = nullptr;
    
    //If Linked List doesn't exist, then value doesn't exist. Return nullptr.
    if (m_listVect[bucketNum] == nullptr)
        return nullptr;
    
    //If Linked List exists, then check if value exists.
    if (m_listVect[bucketNum]->getItem1(key, v))
        return v;
    else
        return nullptr;
}

#endif /* ExpandableHashMap_h */
