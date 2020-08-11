//
//  support.cpp
//  Project 4
//
//  Created by Cody Do on 3/10/20.
//  Copyright © 2020 Cody Do. All rights reserved.
//

#include "support.h"
#include <stack>

//  Node  //
Node::Node(Node* parentNode, const GeoCoord& location, const GeoCoord& destination, StreetSegment& sg, bool isFirstNode) {
    parent = parentNode;
    loc = location;
    streetSeg = sg;
    
    //If it's the first node, no way to compare to previous node
    if(isFirstNode) {
        f = h = distanceEarthMiles(location, destination);
        g = 0;
    }
    
    //If not, f = g + h. g = parent's g value + the value moving from the old location to the new location. h = direct (heuristic) path from new location to end location
    else {
        g = parent->g + distanceEarthMiles(parent->loc, location);
        h = distanceEarthMiles(location, destination);
        f = g + h;
    }
}

Node::~Node() {
}

bool Node::hasCoord(const GeoCoord &other) const {
    if (loc == other)
        return true;
    return false;
}

bool Node::operator <(const Node& other) const {
    if (this->f < other.f)
        return true;
    return false;
}


//Helper Functions

//Used in A* in PointToPointRouter; Finds Neighboring Nodes
std::vector<Node> getNeighboringNodes(Node* n, const StreetMap* sMap, const GeoCoord& endCoord) {
    std::vector<Node> neighborNodes;
    
    //Gets nodes on neighboring nodes
    std::vector<StreetSegment> neighboringStreets;
    sMap->getSegmentsThatStartWith(n->loc, neighboringStreets);
    
    
    for(StreetSegment neighboringStreet : neighboringStreets){
        
        //Check if n's location is the same as the start/end of a street segment, DON'T add it (it would lead back to itself).
        if(!n->hasCoord(neighboringStreet.start))
            neighborNodes.push_back(Node(n, neighboringStreet.start, endCoord, neighboringStreet));
        
        if(!n->hasCoord(neighboringStreet.end))
            neighborNodes.push_back(Node(n, neighboringStreet.end, endCoord, neighboringStreet));
        
    }
    
    return neighborNodes;
}

//Used in A* in PointToPointRouter
std::list<StreetSegment> createReturnRoute(Node endingNode) {
    std::stack<StreetSegment> routeStack;
    Node* curr = &endingNode;
    
    while (curr->parent != nullptr) {
        routeStack.push(curr->streetSeg);
        curr = curr->parent;
    }
    
    std::list<StreetSegment> route;
    while (!routeStack.empty()) {
        route.push_back(routeStack.top());
        routeStack.pop();
    }
    
    return route;
}

// Used in the DeliveryPlanner to find the appropriate direction to proceed on a road.
//0 <= angle < 22.5: east
//o 22.5 <= angle < 67.5: northeast
//o 67.5 <= angle < 112.5: north
//o 112.5 <= angle < 157.5: northwest
//o 157.5 <= angle < 202.5: west
//o 202.5 <= angle < 247.5: southwest
//o 247.5 <= angle < 292.5: south
//o 292.5 <= angle < 337.5: southeast
//o angle >= 337.5: east
std::string directionToProceed(const double angle) {
    if(0 <= angle && angle < 22.5)
        return "east";
    if(angle >= 22.5 && angle < 62.5)
        return "northeast";
    if(angle >= 62.5 && angle < 112.5)
        return "north";
    if(angle >= 112.5 && angle < 157.5)
        return "northwest";
    if(angle >= 157.5 && angle < 202.5)
        return "west";
    if(angle >= 202.5 && angle < 247.5)
        return "southwest";
    if(angle >= 247.5 && angle < 292.5)
        return "south";
    if(angle >= 292.5 && angle < 337.5)
        return "southeast";
    if(angle >= 337.5 && angle <= 360)
        return "east";
    
    return "invalid angle";
}

