#pragma once
#include <iostream>
#include <string>
#include <list>
#include<vector>
#include <cmath>
#include "libsgp4/SGP4.h"
#include "libsgp4/CoordTopocentric.h"
#include "libsgp4/CoordGeodetic.h"
#include "libsgp4/Observer.h"
#include "Handle.hpp"



using namespace std;
class Handle;
using std::vector;

// Polytech
const double site_latitude = 60.00750854195759;
const double site_longitude = 30.374288671966475;
const double site_altitude = 0;


class TrackInfo {
public:
	DateTime time;
	DateTime localTime;
	double azimuth = 0;
	double elevation = 0;
	double longitude = 0.0;
	double latitude = 0.0;
	double altitude= 0.0;
};

class PassDetails {
public:
	DateTime aos;
	DateTime los;
	double max_elevation = 0.0;
};

enum class Direction { 
	east, 
	west,
	unknown,
};

class Satellite {
public:

	/**
	* \brief Default constructor for the satellite
	*/
	Satellite() = default;

	/**
	* \brief Constructor for the satellite
	* \param info tle information about the satellite
	* \param name name of the satellite
	* \param siteLat observer's latitude
	* \param siteLong observer's longitude
	* \param timeSpan time zone to convert time to local
	*/
	Satellite( string const& info, string const& name, double siteLat, double siteLong, int timeSpan);

	/**
	* \brief Get tle data of the satellite
	* \return Tle data about the satellite
	*/
	const Tle& getTle() const { return *tle; };

	/**
	* \brief Get name of the satellite
	* \return string name of the satellite
	*/
	const string getName() const { return name; }

	/**
	* \brief Update current satellite data about its geo posotion and look angle
	*/
	void updateData();

	/**
	* \brief Make satellite's schedule 
	* \param numOfDays amount of days for schedule
	*/
	void createSchedule(int const& numOfDays);

	/** 
	* \brief Write satellite's schedule in file
	*/
	void writeScheduleIFile();

	/**
	* \brief Write satellite's schedule in file
	* \param filename The path to the file
	*/
	void writeScheduleIFile(string const& filename);

	/**
	* \brief Get current azimuth 
	* \return azimuth angle
	*/
	double getAzimuth() const { return info.azimuth; };

	/**
	* \brief Get current elevation
	* \return elevation angle
	*/
	double getElevation() const { return info.elevation; }

	/**
	* \brief Get current altitude
	* \return altitude 
	*/
	double getAltitude() const { return info.altitude; }

	/**
	* \brief  Get current longitude
	* \return longitude
	*/
	double getlongitude() const { return info.longitude; }

	/**
	* \brief Get current latitude
	* \return latitude
	*/
	double getLatitude() const { return info.latitude; }

	/**
	* \brief Get current time
	* \return time
	*/
	DateTime getTime() const { return info.time; };

	/**
	* \brief Get current local time
	* \return local time 
	*/
	DateTime getLocalTime() const { return info.localTime; };

	/**
	* \brief Check if the satellite is visible now
	* \return true if the satellite is visible now
	*/
	bool isVisible();

	/**
	* \brief Get earliest satellite's aos
	* \return aos
	*/
	DateTime getAos() const { return passInfo.aos; };

	/**
	* \brief Get earliest satellite's los
	* \return los
	*/
	DateTime getLos() const { return passInfo.los; };

	/**
	* \brief  Get satellite's max elevation at the earliest appearace
	* \return max elevation
	*/
	double getMaxElevation() const { return passInfo.max_elevation; };

	/**
	* \brief Update information about earliest appearance of the satellite (aos, los, max elevation)
	* \param current time 
	*/
	void updatePassInfo(DateTime const& t1);

	/**
	* \brief Get the middle time of the earliest appearance
	* \return middle time
	*/
	DateTime GetHalfTime();


	/**
	* \brief Get satellite's azimuth at the specified time
	* \param time time when you need to know azimuth
	* \return azimuth angle 
	*/
	double getAzimuthByTime(DateTime const& time);

	/**
	* \brief Get the direction of the satellite
	* \return direction (west or east)
	*/
	Direction getDirection() { return dir; };

	/**
	* \brief Get satellite's longitude at the specified time
	* \param time time when you need to know longitude
	* \return longitude at the specified time
	*/
	double getLongitudeByTime(DateTime const& time);

	/**
	* \brief Get information about observer's coordinates (latitude, longitude, altitude)
	* \return site information
	*/
	Observer getSiteInfo() { return site; };

	/**
	* \brief Convert radians to degrees
	* \return angle in degrees
	*/
	double radiansToDegrees(double x);

	/** 
	* \brief Convert UTC to local time
	* \param time time which need to convert
	* \return DateTime time
	*/
	DateTime toLocalTime(DateTime time) { return time.AddHours((double)timeSpan); }

private:
	const string name;
	shared_ptr<Tle> tle;
	TrackInfo info;
	CoordGeodetic site{ 0, 0, 0 };
	Observer obs{0, 0, 0};
	int timeSpan;

	list<PassDetails> passList;

	PassDetails passInfo{0, 0, 0};

	Direction dir = Direction::unknown;



	void updatePassDetails(const CoordGeodetic& user_geo,
		SGP4 const& sgp4,
		const DateTime& start_time,
		const DateTime& end_time,
		const int time_step);

	void createPassList(
		CoordGeodetic const& user_geo,
		SGP4 const& sgp4,
		DateTime const& start_time,
		DateTime const& end_time,
		const int time_step);

	double findMaxElevation(
		CoordGeodetic const& user_geo,
		SGP4 const& sgp4,
		DateTime const& aos,
		DateTime const& los);


	DateTime findCrossingPoint(
		const CoordGeodetic& user_geo,
		SGP4 const& sgp4,
		DateTime const& initial_time1,
		DateTime const& initial_time2,
		bool finding_aos);

	void defineDirection();
};