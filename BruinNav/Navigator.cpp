#include "provided.h"
#include "MyMap.h"
#include <string>
#include <vector>
#include <stack>
#include <queue>
using namespace std;

bool same(const StreetSegment& left, const StreetSegment& right)
{
    if (left.segment.start.latitude == right.segment.start.latitude && left.segment.start.longitude == right.segment.start.longitude && left.segment.end.latitude == right.segment.end.latitude && left.segment.end.longitude == right.segment.end.longitude) {
        return true;
    }
    
    return false;
}

string directionOfLine(const GeoSegment& segment) {
    double degree = angleOfLine(segment);
    
    std::string dir;
    
    if (degree >= 0 && degree <= 22.5)
        dir = "east";
    else if (degree > 22.5 && degree <= 67.5)
        dir = "northeast";
    else if (degree > 67.5 && degree <= 112.5)
        dir = "north";
    else if (degree > 112.5 && degree <= 157.5)
        dir = "northwest";
    else if (degree > 157.5 && degree <= 202.5)
        dir = "west";
    else if (degree > 202.5 && degree <= 247.5)
        dir = "southwest";
    else if (degree > 247.5 && degree <= 292.5)
        dir = "south";
    else if (degree > 292.5 && degree <= 337.5)
        dir = "southeast";
    else if (degree > 337.5 && degree < 360)
        dir = "east";
    
    return dir;
}

struct pathInfo
{
    pathInfo() : m_streetName("") {}
    pathInfo(GeoCoord startCoordinate, string streetName, GeoSegment segment) : m_startCoordinate(startCoordinate), m_streetName(streetName), m_segment(segment) {}
    GeoCoord m_startCoordinate;
    string m_streetName;
    GeoSegment m_segment;
};

class NavigatorImpl
{
public:
    NavigatorImpl();
    ~NavigatorImpl();
    bool loadMapData(string mapFile);
    void explore(queue<pathInfo>&q, const GeoCoord start_geoCoord, const GeoCoord end_geoCoord, const GeoSegment gs, MyMap<string, pathInfo> &m, string street) const;
    NavResult navigate(string start, string end, vector<NavSegment>& directions) const;
private:
    MapLoader m_ml;
    AttractionMapper m_am;
    SegmentMapper m_sm;
};

NavigatorImpl::NavigatorImpl()
{
}

NavigatorImpl::~NavigatorImpl()
{
}

bool NavigatorImpl::loadMapData(string mapFile)
{
    if(m_ml.load(mapFile))      // if the mapFile loads
    {
        m_am.init(m_ml);        // then we initiailize the attractionmapper and the segment mapper.
        m_sm.init(m_ml);
        return true;
    }
	return false;
}

NavResult NavigatorImpl::navigate(string start, string end, vector<NavSegment> &directions) const
{
    if(!directions.empty())
        directions.clear();     // clear the vector if it is not cleared already.
    
    MyMap<string, pathInfo> locationofPreviousWayPoint; // create a map string.
   
    GeoCoord gcStart;
    GeoCoord gcEnd;
    
    if (!m_am.getGeoCoord(start, gcStart))
        return NAV_BAD_SOURCE; // gets the GeoCoordinates of the start, return bad access if we cant.
    
    if(!m_am.getGeoCoord(end, gcEnd))
        return NAV_BAD_DESTINATION; // gets the GeoCoordinates of the end, return bad access if we cant.
    
    StreetSegment startStreet = m_sm.getSegments(gcStart)[0];   // get the street segments
    StreetSegment endStreet = m_sm.getSegments(gcEnd)[0];
    
    queue <pathInfo> q;       // queue of geocoordinates.
    GeoCoord startCoord = startStreet.segment.start;
    GeoCoord endCoord = startStreet.segment.end;
   
    explore(q, gcStart, endCoord, GeoSegment(gcStart, endCoord), locationofPreviousWayPoint, startStreet.streetName); //explore both paths.
    explore(q, gcStart, startCoord, GeoSegment(gcStart, startCoord), locationofPreviousWayPoint, startStreet.streetName); //explore both paths.
    
    while(! q.empty())
    {
        pathInfo curr = q.front();
        q.pop();
        
        vector <StreetSegment> curSeg = m_sm.getSegments(curr.m_segment.end); // get the vector of streetsegments
        
        for(int i = 0; i < curSeg.size(); i++)
        {
            if(same(curSeg[i], endStreet))  //if we found the location.
            {
                pathInfo pI(curr.m_segment.end, curSeg[i].streetName, GeoSegment(curr.m_segment.end, gcEnd));
                
                locationofPreviousWayPoint.associate((gcEnd.latitudeText  + "," + gcEnd.longitudeText), pI);
                
                pathInfo* p = locationofPreviousWayPoint.find(gcEnd.latitudeText + "," + gcEnd.longitudeText);
                
                stack<pathInfo> s;
                
                while(p != nullptr)
                {
                    pathInfo* t = locationofPreviousWayPoint.find(((*p).m_startCoordinate.latitudeText + "," + (*p).m_startCoordinate.longitudeText));    //find previous pathInfo to get to location
                    s.push(*p); // push current path info into stack
                    p = t;
                }
                
                while(!s.empty()) // store everything as navsegments
                {
                    pathInfo x = s.top();
                    s.pop();
                    
                    string direction = directionOfLine(x.m_segment);
                    string street = x.m_streetName;
                    double distance = distanceEarthKM(x.m_segment.start, x.m_segment.end);
                    GeoSegment gs = x.m_segment;
                    
                    NavSegment nS(direction, street, distance, gs);
                    directions.push_back(nS);       // we put the current navsegment into directions.
                    
                    if(!s.empty())
                    {
                        pathInfo y = s.top();
                        if(x.m_streetName != y.m_streetName)    // if the next navsegment is on a dif street.
                        {
                            string direction;
                            double angleBetween = angleBetween2Lines(x.m_segment, y.m_segment);
                            if(angleBetween < 180)
                                direction = "left";
                            else
                                direction = "right";
                            
                            NavSegment nS(direction, y.m_streetName);
                            directions.push_back(nS);
                        }
                    }
                    
                   
                }
                
                return NAV_SUCCESS;
            }
            else {
                if (curr.m_segment.end.latitude == curSeg[i].segment.start.latitude && curr.m_segment.end.longitude == curSeg[i].segment.start.longitude)
                    explore(q, curr.m_segment.end, curSeg[i].segment.end, GeoSegment(curr.m_segment.end, curSeg[i].segment.end), locationofPreviousWayPoint, curr.m_streetName);
                else
                    explore(q, curr.m_segment.end, curSeg[i].segment.start, GeoSegment(curr.m_segment.end, curSeg[i].segment.start), locationofPreviousWayPoint, curr.m_streetName);
            }
        }
    }
	return NAV_NO_ROUTE;  // if we can't find anything, return this.
}
void NavigatorImpl::explore(queue<pathInfo> &q, const GeoCoord start_geoCoord, const GeoCoord end_geoCoord, const GeoSegment gs, MyMap<string, pathInfo> &m, string street) const
{
    string str_end_geoCoord = end_geoCoord.latitudeText + "," + end_geoCoord.longitudeText;
    if(m.find(str_end_geoCoord) == nullptr)
    {
        const pathInfo pI(start_geoCoord, street , gs);
        q.push(pI);
        m.associate(str_end_geoCoord, pI);  // to make sure we don't view it again and stores it in the map
    }
}

//******************** Navigator functions ************************************

// These functions simply delegate to NavigatorImpl's functions.
// You probably don't want to change any of this code.

Navigator::Navigator()
{
    m_impl = new NavigatorImpl;
}

Navigator::~Navigator()
{
    delete m_impl;
}

bool Navigator::loadMapData(string mapFile)
{
    return m_impl->loadMapData(mapFile);
}

NavResult Navigator::navigate(string start, string end, vector<NavSegment>& directions) const
{
    return m_impl->navigate(start, end, directions);
}
