// The main.cpp you can use for testing will replace this file soon.

#include "provided.h"
#include "MyMap.h"
#include <string>
#include <iostream>
#include <cassert>
using namespace std;

void printDirectionsRaw(string start, string end, const vector<NavSegment>& directions);

int main()
{
    Navigator navigator;
    if (!navigator.loadMapData("/Users/riteshkp/Desktop/mapdata.txt")) {
    	cerr << "Error initializing map data" << endl;
    	return 1;
    }

    vector<NavSegment> directions;
    navigator.navigate("1061 Broxton Avenue", "Headlines!", directions);
    printDirectionsRaw("1061 Broxton Avenue", "Headlines!", directions);

    navigator.navigate("1031 Broxton Avenue", "1061 Broxton Avenue", directions);
    printDirectionsRaw("1031 Broxton Avenue", "1061 Broxton Avenue", directions);

    navigator.navigate("Triangle Fraternity", "Diddy Riese", directions);
    printDirectionsRaw("Triangle Fraternity", "Diddy Riese", directions);

    navigator.navigate("Rieber Terrace", "Fatburger", directions);
    printDirectionsRaw("Rieber Terrace", "Fatburger", directions);

    navigator.navigate("Southern Regional Library Facility", "Diddy Riese", directions);
    printDirectionsRaw("Southern Regional Library Facility", "Diddy Riese", directions);


}

void printDirectionsRaw(string start, string end, const vector<NavSegment>& directions) {
    double distance = 0;
    cerr << "You are starting at: " << start << endl;
    for (auto dir : directions) {
        if (dir.m_command == NavSegment::NavCommand::PROCEED) {
            cerr << "Proceed " << dir.m_distance << " miles " << dir.m_direction << " on " << dir.m_streetName << endl;
            distance += dir.m_distance;
        } else {
            cerr << "Turn " << dir.m_direction << " onto " << dir.m_streetName << endl;
        }
    }
    cerr << "You have reached your destination: " << end << endl;
    cerr << "Total travel distance: " << distance << " miles" << endl;
}
