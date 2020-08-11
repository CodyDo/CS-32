#include "provided.h"
#include <list>
#include "support.h"
using namespace std;

std::vector<Node> getNeighboringNodes(Node* n, const StreetMap* sMap, const GeoCoord& endCoord);

class PointToPointRouterImpl
{
public:
    PointToPointRouterImpl(const StreetMap* sm);
    ~PointToPointRouterImpl();
    DeliveryResult generatePointToPointRoute(
        const GeoCoord& start,
        const GeoCoord& end,
        list<StreetSegment>& route,
        double& totalDistanceTravelled) const;
private:
    const StreetMap* sMap;
};

PointToPointRouterImpl::PointToPointRouterImpl(const StreetMap* sm)
{
    sMap = sm;
}

PointToPointRouterImpl::~PointToPointRouterImpl()
{
}

DeliveryResult PointToPointRouterImpl::generatePointToPointRoute(
        const GeoCoord& start,
        const GeoCoord& end,
        list<StreetSegment>& route,
        double& totalDistanceTravelled) const
{
    //If start or end doesn't exist in the map
    vector<StreetSegment> testExistence;
    vector<StreetSegment> testExistence2;
    sMap->getSegmentsThatStartWith(start, testExistence);
    sMap->getSegmentsThatStartWith(end, testExistence2);
    if (testExistence.empty() || testExistence2.empty())
        return BAD_COORD;
    
    //If destination is where you want to go
    if (start == end) {
        route.clear();
        totalDistanceTravelled = 0;
        return DELIVERY_SUCCESS;
    }
    
    list<Node*> openList; //Used to replicate priority queue --> Didn't use priority queue in this case because couldn't figure out how to implement a comparison operator for Node*
    list<Node*> closedList;
    
    vector<StreetSegment> startingVec;
    sMap->getSegmentsThatStartWith(start, startingVec);
    Node* startNode = new Node(nullptr, start, end, startingVec[0], true);
    openList.push_back(startNode);
    
    while (openList.size() > 0) {
        Node* current = openList.front();
        openList.pop_front();
        
        vector<Node> neighborNodes = getNeighboringNodes(current, sMap, end);
        
        //Scans through vector to see if the ending node is present
        for (Node neighborNode : neighborNodes) {
            if (neighborNode.hasCoord(end)) {
                route = createReturnRoute(neighborNode); //RESULT FOUND!!!!
                totalDistanceTravelled = neighborNode.g;
                
                //Cleans Up
                delete current;
                for (Node* n : openList)
                    delete n;
                
                for (Node* n : closedList)
                    delete n;
                
                return DELIVERY_SUCCESS;
            }
            
            bool validInsert = true;
            
            //Checks if the node already exists within the open or closed list with a lower f-value.
            for(Node* n : openList){
                if(n->hasCoord(neighborNode.loc) && n->f < neighborNode.f){
                    validInsert = false;
                    break;
                }
            }
            
            for(Node* n : closedList){
                if(validInsert && n->hasCoord(neighborNode.loc) && n->f < neighborNode.f){
                    validInsert = false;
                    break;
                }
            }
            
            //If node doesn't exist, add it into the open list and rearrange it so that f is at the top.
            if (validInsert) {
                Node* addMe = new Node(neighborNode);
                
                //If list is empty, OR If addMe's f is lower or equal to the lowest, just add to the back
                if (openList.empty() || (openList.back()->f <= addMe->f))
                    openList.push_back(addMe);
                
                //
                else { //Add it in the appropriate spot in the list
                    for(auto it = openList.begin(); it != openList.end(); it++){
                        if(addMe->f < (*it)->f){
                            openList.insert(it, addMe);
                            break;
                        }
                    }
                }
            }
        }
        closedList.push_back(current);
    }

    //Removes the dynamically allocated objects
    for(Node* n : openList) {
        delete n;
    }
    
    for(Node* n : closedList) {
        delete n;
    }
    
    return NO_ROUTE;
}

//******************** PointToPointRouter functions ***************************

// These functions simply delegate to PointToPointRouterImpl's functions.
// You probably don't want to change any of this code.

PointToPointRouter::PointToPointRouter(const StreetMap* sm)
{
    m_impl = new PointToPointRouterImpl(sm);
}

PointToPointRouter::~PointToPointRouter()
{
    delete m_impl;
}

DeliveryResult PointToPointRouter::generatePointToPointRoute(
        const GeoCoord& start,
        const GeoCoord& end,
        list<StreetSegment>& route,
        double& totalDistanceTravelled) const
{
    return m_impl->generatePointToPointRoute(start, end, route, totalDistanceTravelled);
}
