#include "provided.h"
#include <vector>
using namespace std;

class DeliveryOptimizerImpl
{
public:
    DeliveryOptimizerImpl(const StreetMap* sm);
    ~DeliveryOptimizerImpl();
    void optimizeDeliveryOrder(
        const GeoCoord& depot,
        vector<DeliveryRequest>& deliveries,
        double& oldCrowDistance,
        double& newCrowDistance) const;
private:
    const StreetMap* sMap;
};

DeliveryOptimizerImpl::DeliveryOptimizerImpl(const StreetMap* sm)
{
    sMap = sm;
}

DeliveryOptimizerImpl::~DeliveryOptimizerImpl()
{
}

void DeliveryOptimizerImpl::optimizeDeliveryOrder(
    const GeoCoord& depot,
    vector<DeliveryRequest>& deliveries,
    double& oldCrowDistance,
    double& newCrowDistance) const
{
    //Uses two iterators to run through the list. it will start at the beginning while it2 starts one after the beginning. The for loop will stop computing when it2 points to the end. Computes oldCrowDistance and sets lowestDistance equal to it as a starting comparison. Adds in the depot as starting and ending points.
    vector<DeliveryRequest>::iterator it1 = deliveries.begin();
    double distance = 0;
    vector<DeliveryRequest> originalVect = deliveries;
    DeliveryRequest depotReq("randomItem", depot);
    deliveries.insert(it1, depotReq);
    deliveries.push_back(depotReq);
    it1 = deliveries.begin();
    vector<DeliveryRequest>::iterator it2 = deliveries.begin();
    it2++;
    
    for (; it2 != deliveries.end(); it2++) {
        double pointToPointDist = distanceEarthMiles((*it1).location, (*it2).location);
        distance += pointToPointDist;
        it1++;
    }
    oldCrowDistance = distance;
    double lowestDistance = distance;
    deliveries = originalVect;
    
    //ALGORITHM: Nested while loops with two iterators. it1 will point at the first item in the vector (the item that will be moved). it2 points at the location the item will be moved to. For each item it1 points to, it will be moved throughout the entirety of the vector as it2 moves throughout the vector (starting at position 0).
    
    vector<DeliveryRequest> newVect = originalVect;
    
    //Sets up the iterators
    it1 = deliveries.begin();
    it2 = deliveries.begin();
    
    //Outer while loop to move the first iterator
    while (it1 != deliveries.end()) {
        int index2 = 0;
        
        //Inner while loop to move the second iterator
        while(it2 != deliveries.end()) {
            //Copies what it1 points at and then erases what it1 points at. Makes backup
            vector<DeliveryRequest> backupVect = deliveries;
            DeliveryRequest req = *it1;
            deliveries.erase(it1);
            
            //Insert req where it2 points. Add depot location to start/end of vector
            it2 = deliveries.begin() + index2;
            if (it2 == deliveries.end())
                break;
            deliveries.insert(it2, req);
            vector<DeliveryRequest>::iterator hi = deliveries.begin();
            deliveries.insert(hi, depotReq);
            deliveries.push_back(depotReq);
            
            //Calculate the distance of the new vector arrangement. Make changes as necessary (reset lowestDistance, remove depot from the front/back, hold the vector with the lowest distance)
            hi = deliveries.begin();
            vector<DeliveryRequest>::iterator hi2 = deliveries.begin();
            hi2++;
            double localDist = 0.0;
            for (; hi2 != deliveries.end(); hi2++) {
                double smallerDist = distanceEarthMiles((*hi).location, (*hi2).location);
                localDist += smallerDist;
                hi++;
            }
            if (localDist < lowestDistance) {
                lowestDistance = localDist;
                vector<DeliveryRequest>::iterator deleteMe = deliveries.begin();
                deliveries.erase(deleteMe);
                deleteMe = deliveries.end();
                deleteMe--;
                deliveries.erase(deleteMe);
                newVect = deliveries;
            }
            
            //Resets vector and increments for the next run through
            deliveries = backupVect;
            index2++;
        }
        it1++;
    }
    
    //Changes input variables
    newCrowDistance = lowestDistance;
    deliveries = newVect;
}


//******************** DeliveryOptimizer functions ****************************

// These functions simply delegate to DeliveryOptimizerImpl's functions.
// You probably don't want to change any of this code.

DeliveryOptimizer::DeliveryOptimizer(const StreetMap* sm)
{
    m_impl = new DeliveryOptimizerImpl(sm);
}

DeliveryOptimizer::~DeliveryOptimizer()
{
    delete m_impl;
}

void DeliveryOptimizer::optimizeDeliveryOrder(
        const GeoCoord& depot,
        vector<DeliveryRequest>& deliveries,
        double& oldCrowDistance,
        double& newCrowDistance) const
{
    return m_impl->optimizeDeliveryOrder(depot, deliveries, oldCrowDistance, newCrowDistance);
}
