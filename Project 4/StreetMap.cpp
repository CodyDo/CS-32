#include "provided.h"
#include <string>
#include <vector>
#include <set>
#include <functional>
#include <iostream>
#include <fstream>
#include <sstream>
#include "ExpandableHashMap.h"
using namespace std;

unsigned int hasher(const GeoCoord& g)
{
    return std::hash<string>()(g.latitudeText + g.longitudeText);
}

class StreetMapImpl
{
public:
    StreetMapImpl();
    ~StreetMapImpl();
    bool load(string mapFile);
    bool getSegmentsThatStartWith(const GeoCoord& gc, vector<StreetSegment>& segs) const;
private:
    ExpandableHashMap<GeoCoord, vector<StreetSegment>> hashMap;
};

StreetMapImpl::StreetMapImpl()
{
}

StreetMapImpl::~StreetMapImpl()
{
}

bool StreetMapImpl::load(string mapFile)
{
    ifstream infile(mapFile);    // infile is a name of our choosing
    if ( ! infile )                // Did opening the file fail?
    {
        cerr << "Error: Cannot open data!" << endl;
        return false;
    }
    
    string name;
    while (getline(infile, name)) {
        int numLines;
        infile >> numLines;
        infile.ignore(10000, '\n');
        
        vector<StreetSegment> vecStreet;
        vector<StreetSegment> vecStreetRev;
        set<GeoCoord> setGeo;
        
        for (int k = 0; k < numLines; k++) {
            string coordLine;
            getline(infile, coordLine);
            istringstream iss(coordLine);
            string lat1, lon1, lat2, lon2;
            if(!(iss >> lat1 >> lon1 >> lat2 >> lon2)) {
                    continue; //Ignores bad line
            }
            
            GeoCoord geoOne(lat1,lon1);
            GeoCoord geoTwo(lat2,lon2);
            setGeo.insert(geoOne);
            setGeo.insert(geoTwo);
            
            vecStreet.push_back(StreetSegment(geoOne, geoTwo, name));
            vecStreetRev.push_back(StreetSegment(geoTwo, geoOne, name));
        }
        
    for(set<GeoCoord>::iterator it = setGeo.begin(); it != setGeo.end(); it++ ) {
        GeoCoord a = *it;
        vector<StreetSegment> vecSeg;
        
        //Fill vecSeg with any StreetSegments that GeoCoord a already has
        vector<StreetSegment>* x = hashMap.find(a);
        if (x != nullptr)
            vecSeg = *x;
         
         //Checks StreetSeg for matching start GeoCoords
        vector<StreetSegment>::iterator it2 = vecStreet.begin();
        while (it2 != vecStreet.end()) {
            if (a == (*it2).start)
                 vecSeg.push_back(*it2);
             it2++;
         }
         
          //Checks StreetSegRev for matching start GeoCoords
         vector<StreetSegment>::iterator it3 = vecStreetRev.begin();
         while (it3 != vecStreetRev.end()) {
             if (a == (*it3).start)
                 vecSeg.push_back(*it3);
             it3++;
         }
         hashMap.associate(a, vecSeg);
    }
    
}

return true;
}

bool StreetMapImpl::getSegmentsThatStartWith(const GeoCoord& gc, vector<StreetSegment>& segs) const
{
const vector<StreetSegment>* pToVect = hashMap.find(gc);
if (pToVect == nullptr)
    return false;

vector<StreetSegment>::const_iterator it = (*pToVect).begin();
while (it != (*pToVect).end()) {
    segs.push_back(*it);
    it++;
}
return true;
}

//******************** StreetMap functions ************************************

// These functions simply delegate to StreetMapImpl's functions.
// You probably don't want to change any of this code.

StreetMap::StreetMap()
{
m_impl = new StreetMapImpl;
}

StreetMap::~StreetMap()
{
delete m_impl;
}

bool StreetMap::load(string mapFile)
{
return m_impl->load(mapFile);
}

bool StreetMap::getSegmentsThatStartWith(const GeoCoord& gc, vector<StreetSegment>& segs) const
{
return m_impl->getSegmentsThatStartWith(gc, segs);
}
