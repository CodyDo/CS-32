//
//  Map.hpp
//  Project 2
//
//  Created by Cody Do on 1/22/20.
//  Copyright © 2020 Cody Do. All rights reserved.
//

#ifndef Map_h
#define Map_h

#include <string>
#include <iostream>
using KeyType = std::string;
using ValueType = double;

class Map  {
  public:
    Map();
    ~Map();        // Destructor
    Map(const Map &src);   // Copy Constructor
    Map &operator= (const Map &src); // Assignment Operator
    bool empty() const;
    int size() const;
    bool insert(const KeyType& key, const ValueType& value);
    bool update(const KeyType& key, const ValueType& value);
    bool insertOrUpdate(const KeyType& key, const ValueType& value);
    bool erase(const KeyType& key);
    bool contains(const KeyType& key) const;
    bool get(const KeyType& key, ValueType& value) const;
    bool get(int i, KeyType& key, ValueType& value) const;
    void swap(Map& other);
    void dump();
    
  private:
    struct Node {
        KeyType key;
        ValueType value;
        Node *next;
        Node *prev;
    };
    
    class LinkedList {
      public:
        LinkedList() {
            head = nullptr;
            tail = nullptr;
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
                p->prev = nullptr;
                head = p;
                tail = p;
                
            }
            else {
                p->next = head;
                p->prev = nullptr;
                head = p;
                Node *n = p->next;
                n->prev = p;
            }
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
                n -> prev = p;
                n-> next = nullptr; //Links last node to nullptr
                tail = n; //Resets tail
            }
        }
        
        void deleteItem(KeyType key) {
            if (head == nullptr) //Makes sure to not delete an empty list
                return;
            if (head->key == key) { //Deleting the first node
                Node* killMePls = head;
                head = head->next;
                if (head == nullptr) { //Double checks to make sure nullptr->next isn't accessed (if you're deleting the only thing in list)
                    tail = nullptr;
                    delete killMePls;
                }
                else {
                    head->prev = nullptr;
                    delete killMePls;
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
                    tail = p;
                    delete killMePls;
                }
                else {
                    p->next = killMePls->next;
                    killMePls->next->prev = p;
                    delete killMePls;
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
        
        void printItems() const {
            Node *p = head;
            while (p != nullptr) {
                std::cerr << "Key: "<< p->key << ".....Value: " << p->value << std::endl;
                p = p->next;
            }
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
        
        void getItem1(const KeyType &key, ValueType &value) const {
            Node *p = head;
            while (p != nullptr) { //Scan through list
                if (p->key == key) { //If keys match, set parameter value to pair's value
                    value = p->value;
                    return;
                }
                p = p->next;
            }
            return;
        }
        
        void getItem2(int i, KeyType &key, ValueType &value) const {
            Node *p = head;
            int counter = 0; //Creates a local counter to scan through list
            while (p != nullptr) { //Scan through the list
                if (i == counter) { //If local counter and input int match, copy information over to parameter key and value
                    key = p->key;
                    value = p->value;
                    return;
                }
                counter++;
                p = p->next;
            }
            return;
        }
        
        void clearList() { //Callable Destructor
            Node *p;
            p = head;
            while (p != nullptr) { //Cycles through the list
                Node *n = p->next; //Pointer to hold value of next node
                delete p; //Deletes node and moves on to the next one
                p = n;
            }
            head = nullptr; //Sets the head/tail to nullptr to make an empty list
            tail = nullptr;
        }
        
    private:
        Node* head;
        Node* tail;
    };
    int m_mapSize;
    LinkedList m_map;
};



bool combine(const Map& m1, const Map& m2, Map& result);
void reassign(const Map& m, Map& result);

#endif /* Map_h */

