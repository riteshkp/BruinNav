#include "provided.h"
#include "MyMap.h"
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <cstring>
using namespace std;

class MapLoaderImpl
{
public:
	MapLoaderImpl();
	~MapLoaderImpl();
	bool load(string mapFile);
	size_t getNumSegments() const;
	bool getSegment(size_t segNum, StreetSegment& seg) const;
private:
    std::vector<StreetSegment> StreetSegmentVector;
};

MapLoaderImpl::MapLoaderImpl()
{
}

MapLoaderImpl::~MapLoaderImpl()
{
}

bool MapLoaderImpl::load(string mapFile)
{
    ifstream infile(mapFile);      //load the mapFile.
    
    if(!infile)
    {
        cerr << "this doesnt load." << endl;
        return false;   //if we cannot load the infile, return false.
    }
    
    string street;
    while(getline(infile, street))
          {
              StreetSegment ss;
              ss.streetName = street;   // stores the street name.
              
              string start1;
              string start2;
              
              
              getline(infile, start1, ',');
 
              char c;
              infile.get(c);
              if(c != ' ')
              {
                  string x = string(&c);
                  getline(infile, start2, ' ');
                  start2 = x + start2;
              }
              else
              {
                  getline(infile, start2, ' ');
              }
              
              GeoCoord g1(start1, start2); // create first geocoordinate
              
              string end1;
              string end2;
              
              getline(infile, end1, ',');
              getline(infile, end2);
              
              GeoCoord g2(end1, end2);  // create second geocoordinate
              
              GeoSegment gs(g1, g2);   // create geosegment.
              
              ss.segment = gs;
              
              int numAttractions;
              infile >> numAttractions;
              infile.ignore(10000, '\n');
              
              std::vector<Attraction> AttractionVector;
              
              for(int i = 0; i < numAttractions; i++)
              {
                  string sName;
                  getline(infile, sName, '|');
                  
                  string coord1;
                  string coord2;
                  
                  getline(infile, coord1, ',');
                  
                  char c;
                  infile.get(c);
                  if(c != ' ')
                  {
                      string x = string(&c);
                      getline(infile, coord2);
                      coord2 = c + coord2;
                  }
                  else
                  {
                      getline(infile, coord2);
                  }
                  
                  GeoCoord aGC(coord1, coord2);
                  
                  Attraction a;
                  a.name = sName;
                  a.geocoordinates = aGC;
                  
                  AttractionVector.push_back(a);
              }
              
              ss.attractions = AttractionVector;
              
              StreetSegmentVector.push_back(ss);
          }
    
    return true;
}

size_t MapLoaderImpl::getNumSegments() const
{
	return StreetSegmentVector.size();
}

bool MapLoaderImpl::getSegment(size_t segNum, StreetSegment &seg) const
{
	if(segNum <= getNumSegments()-1)
    {
        seg = StreetSegmentVector[segNum];  // assign proper segment to seg.
        return true;
    }
    
    return false;
}

//******************** MapLoader functions ************************************

// These functions simply delegate to MapLoaderImpl's functions.
// You probably don't want to change any of this code.

MapLoader::MapLoader()
{
	m_impl = new MapLoaderImpl;
}

MapLoader::~MapLoader()
{
	delete m_impl;
}

bool MapLoader::load(string mapFile)
{
	return m_impl->load(mapFile);
}

size_t MapLoader::getNumSegments() const
{
	return m_impl->getNumSegments();
}

bool MapLoader::getSegment(size_t segNum, StreetSegment& seg) const
{
   return m_impl->getSegment(segNum, seg);
}
