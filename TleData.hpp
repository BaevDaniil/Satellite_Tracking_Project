#pragma once
#define CURL_STATICLIB
#include <iostream>
#include <string>
#include <SGP4.h>
#include <list>
#include <CoordTopocentric.h>
#include <CoordGeodetic.h>
#include <Observer.h>
#include <fstream>
#include "curl.h"

using namespace std;

class TleData {
public:

    TleData() { downloadTleData(); }

    /**
    * \brief Get data about satellites from calestrack and write in string
    */
    void downloadTleData();

    /**
    * \brief Get TLE data of the specific satellite by its name
    * \param name name of the satellite
    * \return string with satellite data
    */
    string getSatelliteData(string const& name);

    /**
    * \brief Print all tle data about satellites
    */
    void printTle() { cout << tleData << endl; };

private:
    string tleData;
    static size_t dataToString(void* contents, size_t size, size_t nmemb, void* userp);

};