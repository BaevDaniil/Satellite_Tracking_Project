#pragma once
#define CURL_STATICLIB
#include <iostream>
#include <string>
#include <list>
#include <fstream>
#include "curl/curl.h"
#include "libsgp4/SGP4.h"
#include "libsgp4/CoordTopocentric.h"
#include "libsgp4/CoordGeodetic.h"
#include "libsgp4/Observer.h"

using namespace std;

class Satellite;
class SatellitePredict;
class PassDetails;

class Handle {
public:
    
    /**
	* \brief Get all Tle data about active satellites.
    * \details URL: https://celestrak.org/NORAD/elements/gp.php?GROUP=active&FORMAT=tle
	* \return String with all Tle data
    */
    string getTleData();

    /**
    * \brief Find necessary satellite and get its tle data
    * \param name Name of the satellite
    * \return String with tle data of specific satellite
    */
    string getSatelliteData(string const& name);

    /**
    * \brief Open file. The name is the name of the satellite and its AOS time
    * \param name name of the satellite
    * \param time Aos time of the satellite
    */
    void openNewFile(string const& name, DateTime const& time);

    /**
    * \brief Write data in file
    * \param data Data to write in the form of string
    */
    void writeFile(string const& data);

    /**
    * \brief Write data about the satellite position at the moment: azimuth and elevation
    * \param time current time 
    * \param azimuth current azimuth
    * \param elevaion current azimuth
    */
    void writeFile(DateTime const& time, int const& azimuth, int const& elevation);

    /**
    * \brief Close file
    */
    void closeFile();

private:
    ofstream file;

    static size_t dataToString(void* contents, size_t size, size_t nmemb, void* userp);
};