#include "provided.h"
#include "MyMap.h"
#include <string>
#include <cctype>
using namespace std;

class AttractionMapperImpl
{
public:
	AttractionMapperImpl();
	~AttractionMapperImpl();
	void init(const MapLoader& ml);
	bool getGeoCoord(string attraction, GeoCoord& gc) const;
private:
    MyMap<string, GeoCoord> AttractionMap;
};

AttractionMapperImpl::AttractionMapperImpl()
{
}

AttractionMapperImpl::~AttractionMapperImpl()
{
}

void AttractionMapperImpl::init(const MapLoader& ml)
{
    for(int i = 0; i < ml.getNumSegments(); i++) // traverse through each of the street segments.
    {
        StreetSegment s;
        if(ml.getSegment(i, s))
        {
            for(int j = 0; j < s.attractions.size(); j++) //traverse through the vector of attractions.
            {
                string l = s.attractions[j].name;
                
                for(int z = 0; z < l.size(); z++)
                    l[z] = tolower(l[z]);   // converts attraction name to lower case.
                
                AttractionMap.associate(l,s.attractions[j].geocoordinates);
            }
        }
    }
}

bool AttractionMapperImpl::getGeoCoord(string attraction, GeoCoord& gc) const
{
    for(int i = 0; i < attraction.size(); i++)
        attraction[i] = tolower(attraction[i]); // converts attraction name to lower case
    
    const GeoCoord* g = AttractionMap.find(attraction);
    
    if(g != nullptr)
    {
        gc = *(g);
        return true;
    }
    
    return false;
}

//******************** AttractionMapper functions *****************************

// These functions simply delegate to AttractionMapperImpl's functions.
// You probably don't want to change any of this code.

AttractionMapper::AttractionMapper()
{
	m_impl = new AttractionMapperImpl;
}

AttractionMapper::~AttractionMapper()
{
	delete m_impl;
}

void AttractionMapper::init(const MapLoader& ml)
{
	m_impl->init(ml);
}

bool AttractionMapper::getGeoCoord(string attraction, GeoCoord& gc) const
{
	return m_impl->getGeoCoord(attraction, gc);
}
