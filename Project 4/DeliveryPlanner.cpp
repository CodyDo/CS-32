#include "provided.h"
#include "support.h"
#include <vector>
using namespace std;

class DeliveryPlannerImpl
{
public:
    DeliveryPlannerImpl(const StreetMap* sm);
    ~DeliveryPlannerImpl();
    DeliveryResult generateDeliveryPlan(
        const GeoCoord& depot,
        const vector<DeliveryRequest>& deliveries,
        vector<DeliveryCommand>& commands,
        double& totalDistanceTravelled) const;
private:
    const StreetMap* sMap;
    PointToPointRouter* router;
};

DeliveryPlannerImpl::DeliveryPlannerImpl(const StreetMap* sm)
{
    sMap = sm;
    router = new PointToPointRouter(sm);
}

DeliveryPlannerImpl::~DeliveryPlannerImpl()
{
    delete router;
}


DeliveryResult DeliveryPlannerImpl::generateDeliveryPlan(
    const GeoCoord& depot,
    const vector<DeliveryRequest>& deliveries,
    vector<DeliveryCommand>& commands,
    double& totalDistanceTravelled) const
{
    //Tests if Depot exists
    vector<StreetSegment> testExistenceOfDepot;
    sMap->getSegmentsThatStartWith(depot, testExistenceOfDepot);
    if (testExistenceOfDepot.empty())
        return BAD_COORD;
    
    //Optimizes the deliveries
    DeliveryOptimizer devOp(sMap);
    double oldDist;
    double newDist;
    vector<DeliveryRequest> delivs = deliveries;
    devOp.optimizeDeliveryOrder(depot, delivs, oldDist, newDist);
    
    double totalDist = 0.0; //Holds the distance
    GeoCoord currGeoCoord = depot; //Initially starts at depot
    
    //Goes through all the deliveries and processes them
    vector<DeliveryRequest>::const_iterator testDeliveries = delivs.begin();
    for (; testDeliveries != delivs.end(); testDeliveries++) {
        //Makes sure all deliveries have existing locations. If not, return BAD_COORD
        vector<StreetSegment> vectDeliv;
        sMap->getSegmentsThatStartWith(testDeliveries->location, vectDeliv);
        if (vectDeliv.empty())
            return BAD_COORD;
        
        //Gets the list of routes getting from currGeoCoord to destination's geoCoord. Add distance to total distance. Set currGeoCoord as the location of the item.
        double routeDist = 0.0;
        list<StreetSegment> route;
        router->generatePointToPointRoute(currGeoCoord, testDeliveries->location, route, routeDist);
        totalDist += routeDist;
        currGeoCoord = testDeliveries->location;
        
        //Goes through the entire list. If one's name node matches the name of the next node, then add distance to that node. it starts at position 1 NOT 0. This allows us to compare the values of two consecutive street segments. If it reaches the end, the loop ends. This prevents accessing past the end of the list.
        list<StreetSegment>::iterator it = route.begin();
        list<StreetSegment>::iterator it2 = route.begin();
        it++;
        double segDist = 0;
        bool isFirst = true; //Flag for if it's the first item --> generates angle  for proceed command
        double angle = 0.0;
        
        while (it != route.end()) {
            //If there's only one segment inside
            if(route.size() == 1) {
                angle=angleOfLine(*it2);
                segDist = distanceEarthMiles(it2->start, it2->end);
                DeliveryCommand path;
                path.initAsProceedCommand(directionToProceed(angle), it2->name, segDist);
                commands.push_back(path);
                //Check to see if the end of this one segment is the destination. If it is, add as delivery and return.
                if (it2->end == testDeliveries->location) {
                    DeliveryCommand done;
                    done.initAsDeliverCommand(testDeliveries->item);
                    commands.push_back(done);
                }
                break;
            }
            
            //Check if the destination has been reached. If it does, add a proceed then add a delivery
            if (it->end == testDeliveries->location) {
                if (isFirst) {
                    angle=angleOfLine(*it2);
                    isFirst = false;
                }
                
                //Only occurs if one needs to move
                if (segDist != 0) {
                    DeliveryCommand path;
                    path.initAsProceedCommand(directionToProceed(angle), it2->name, segDist);
                    commands.push_back(path);
                    segDist = 0;
                }
                
                DeliveryCommand done;
                done.initAsDeliverCommand(testDeliveries->item);
                commands.push_back(done);
                break;
            }
            
            //If same name, means same street. Add distance to segDist from it2's start to it's end. If it's the first of a chain of names, generate the angle for a proceed command.
            if (it->name == it2->name) {
                double anotherDist = distanceEarthMiles(it2->start, it->end);
                segDist += anotherDist;
                if (isFirst) {
                    angle=angleOfLine(*it2);
                    isFirst = false;
                }
            }
            
            //If different name then a turn has happened. Add the segment (pre-turn) as a command to the vector of commands. Then add a turn command.
            else {
               if (segDist != 0 ) {
                    DeliveryCommand path;
                    path.initAsProceedCommand(directionToProceed(angle), it2->name, segDist);
                    commands.push_back(path);
                    segDist = 0; //Resets distance amount for next proceed
                }
                double turnAngle = angleBetween2Lines(*it2, *it);
                //If angle is < 1 or > 359, no real turn, instead do a proceed statement. Continue in order to begin the next iteration.
                if (turnAngle < 1 || turnAngle > 359) {
                    it++;
                    it2++;
                    continue;
                }
                //Left Turn
                if (turnAngle >= 1 && turnAngle < 180) {
                    DeliveryCommand turn;
                    turn.initAsTurnCommand("left", it->name);
                    commands.push_back(turn);
                }
                //Right Turn
                if (turnAngle >= 180 && turnAngle <= 359) {
                    DeliveryCommand turn;
                    turn.initAsTurnCommand("right", it->name);
                    commands.push_back(turn);
                }
                
                //Reset the angle to the street you turned on
                angle = angleOfLine(*it);
            }
            
            it++;
            it2++;
        }
        
    }
    
    //All the items should be delivered at this point. Now the robot must go back to the depot
    list<StreetSegment> backHome;
    double routeDist;
    router->generatePointToPointRoute(currGeoCoord, depot, backHome, routeDist);
    totalDist += routeDist;
    list<StreetSegment>::iterator it = backHome.begin();
    list<StreetSegment>::iterator it2 = backHome.begin();
    it++;
    double segDist = 0.0;
    bool isFirst = true; //Flag for if it's the first item --> generates angle  for proceed command
    double angle = 0.0;
    
    while (it != backHome.end()) {
        //If there's only one segment inside
        if(backHome.size() == 1) {
            angle=angleOfLine(*it2);
            segDist = distanceEarthMiles(it2->start, it2->end);
            DeliveryCommand path;
            path.initAsProceedCommand(directionToProceed(angle), it2->name, segDist);
            commands.push_back(path);
            //Check to see if the end of this one segment is the depot.
            if (it2->end == depot) {
                totalDistanceTravelled = totalDist;
                return DELIVERY_SUCCESS;
            }
            break;
        }
        
        //Check if the depot has been reached. If it has, add a proceed then return.
        if (it->end == depot) {
            if (isFirst) {
                angle=angleOfLine(*it2);
                isFirst = false;
            }
            
            //Only happens if you need to move.
            if (segDist != 0) {
                DeliveryCommand path;
                path.initAsProceedCommand(directionToProceed(angle), it2->name, segDist);
                commands.push_back(path);
                segDist = 0;
            }
            
            totalDistanceTravelled = totalDist;
            return DELIVERY_SUCCESS;
        }
        
        //If same name, means same street. Add distance to segDist from it2's start to it's end. If it's the first of a chain of names, generate the angle for a proceed command.
        if (it->name == it2->name) {
            double anotherDist = distanceEarthMiles(it2->start, it->end);
            segDist += anotherDist;
            if (isFirst) {
                angle=angleOfLine(*it2);
                isFirst = false;
            }
        }
        
        //If different name than a turn has happened. Add the segment (pre-turn) as a command to the vector of commands. Then add a turn command.
        else {
            if (segDist != 0 ) {
                 DeliveryCommand path;
                 path.initAsProceedCommand(directionToProceed(angle), it2->name, segDist);
                 commands.push_back(path);
                 segDist = 0; //Resets distance amount for next proceed
             }
            double turnAngle = angleBetween2Lines(*it2, *it);
            //If angle is < 1 or > 359, no real turn, instead do a proceed statement. Continue in order to begin the next iteration.
            if (turnAngle < 1 || turnAngle > 359) {
                it++;
                it2++;
                continue;
            }
            //Left Turn
            if (turnAngle >= 1 && turnAngle < 180) {
                DeliveryCommand turn;
                turn.initAsTurnCommand("left", it->name);
                commands.push_back(turn);
            }
            //Right Turn
            if (turnAngle >= 180 && turnAngle <= 359) {
                DeliveryCommand turn;
                turn.initAsTurnCommand("right", it->name);
                commands.push_back(turn);
            }
            
            //Reset the angle to the street you turned on
            angle = angleOfLine(*it);
        }
        
        it++;
        it2++;
    }

    totalDistanceTravelled = totalDist;
    return NO_ROUTE;
}

//******************** DeliveryPlanner functions ******************************

// These functions simply delegate to DeliveryPlannerImpl's functions.
// You probably don't want to change any of this code.

DeliveryPlanner::DeliveryPlanner(const StreetMap* sm)
{
    m_impl = new DeliveryPlannerImpl(sm);
}

DeliveryPlanner::~DeliveryPlanner()
{
    delete m_impl;
}

DeliveryResult DeliveryPlanner::generateDeliveryPlan(
    const GeoCoord& depot,
    const vector<DeliveryRequest>& deliveries,
    vector<DeliveryCommand>& commands,
    double& totalDistanceTravelled) const
{
    return m_impl->generateDeliveryPlan(depot, deliveries, commands, totalDistanceTravelled);
}
