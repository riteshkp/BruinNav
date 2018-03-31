#include "provided.h"
#include "MyMap.h"
#include <vector>
using namespace std;

class SegmentMapperImpl
{
public:
	SegmentMapperImpl();
	~SegmentMapperImpl();
	void init(const MapLoader& ml);
	vector<StreetSegment> getSegments(const GeoCoord& gc) const;
private:
    MyMap<string, vector<StreetSegment>> SegmentMapperMap;
};

SegmentMapperImpl::SegmentMapperImpl()
{
}

SegmentMapperImpl::~SegmentMapperImpl()
{
}

void SegmentMapperImpl::init(const MapLoader& ml)
{
    for(int i = 0; i < ml.getNumSegments(); i++)
    {
        StreetSegment s;
        if(ml.getSegment(i, s))
        {
            GeoCoord g1;
            GeoCoord g2;
            
            g1 = s.segment.start;   // get the start coordinate of the segment.
            g2 = s.segment.end;     // get the end coordinate of the segment.
            
            string sla1 = g1.latitudeText; // convert to string so we don't have to redefine operators.
            string slo1 = g1.longitudeText;
            
            string geoString1 = sla1 + ", " + slo1;
            
            string sla2 = g2.latitudeText;
            string slo2 = g2.longitudeText;
            
            string geoString2 = sla2 + ", " + slo2;
            
            vector<StreetSegment>* vp = SegmentMapperMap.find(geoString1);    // find first geocoord.
            
            if(vp == nullptr)
            {
                vector<StreetSegment> ss;
                ss.push_back(s);
                SegmentMapperMap.associate(geoString1, ss);
            }
            else
                vp->push_back(s);   // if there is a geocoord already, we just push back the streetsegment to the vector.
            
            vp = SegmentMapperMap.find(geoString2);     // find second geocoord
            
            if(vp == nullptr)
            {
                vector<StreetSegment> ss;
                ss.push_back(s);
                SegmentMapperMap.associate(geoString2, ss);
            }
            else
                vp->push_back(s);   // if there is a geocoord already, we just push back the streetsegment to the vector.
            
            
            for(int j = 0; j < s.attractions.size(); j++) //traverse through the vector of attractions.
            {
                GeoCoord g = s.attractions[j].geocoordinates;
                string sla = g.latitudeText;
                string slo = g.longitudeText;
                string sg = sla + ", " + slo;
                vp = SegmentMapperMap.find(sg);
                if(vp == nullptr)
                {
                    vector<StreetSegment> ss;
                    ss.push_back(s);
                    SegmentMapperMap.associate(sg, ss);
                }
                else
                    vp->push_back(s);
            }
        }
    }
}

vector<StreetSegment> SegmentMapperImpl::getSegments(const GeoCoord& gc) const
{
    vector<StreetSegment> ssvector;
    
    string sg1 = gc.latitudeText;
    string sg2 = gc.longitudeText;
    
    string geoString = sg1 + ", " + sg2;
    
    const vector<StreetSegment>* vp = SegmentMapperMap.find(geoString);
    
    if(vp == nullptr)       // if its null (no segment found), we return an empty streetsegment vector. 
        return ssvector;
    
    return *vp;
}

//******************** SegmentMapper functions ********************************

// These functions simply delegate to SegmentMapperImpl's functions.
// You probably don't want to change any of this code.

SegmentMapper::SegmentMapper()
{
	m_impl = new SegmentMapperImpl;
}

SegmentMapper::~SegmentMapper()
{
	delete m_impl;
}

void SegmentMapper::init(const MapLoader& ml)
{
	m_impl->init(ml);
}

vector<StreetSegment> SegmentMapper::getSegments(const GeoCoord& gc) const
{
	return m_impl->getSegments(gc);
}
