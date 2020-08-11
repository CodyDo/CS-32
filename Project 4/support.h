//
//  support.hpp
//  Project 4
//
//  Created by Cody Do on 3/10/20.
//  Copyright © 2020 Cody Do. All rights reserved.
//

#ifndef support_h
#define support_h

#include "provided.h"

class Node {
public:
    Node(Node* parentNode, const GeoCoord& location, const GeoCoord& destination, StreetSegment& sg, bool isFirstNode = false);
    ~Node();
    
    bool hasCoord(const GeoCoord& other) const;
    bool operator <(const Node& other) const;
    
    Node* parent;
    GeoCoord loc;
    StreetSegment streetSeg; //Holds info on streetSeg (Used to compuete path once found)
    double f, g, h; //Used for A*
};

std::vector<Node> getNeighboringNodes(Node* n, const StreetMap* sMap, const GeoCoord& endCoord);
std::list<StreetSegment> createReturnRoute(Node endingNode);
std::string directionToProceed(const double angle);

#endif /* support_hpp */
