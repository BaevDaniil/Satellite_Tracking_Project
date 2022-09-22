#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <stdarg.h>
#include "ComPort.hpp"
#include "TleData.hpp"
#include "Satellite.hpp"
#include "Antenna.hpp"

using namespace std;
using std::vector;

class SatTrackInterface {
public:
	shared_ptr<Antenna> antenna;

	SatTrackInterface(LPCTSTR portName);
	SatTrackInterface(vector<string> const& names, double siteLat, double siteLong, int timeSpan);

	/**
	* \brief Get vector of satellites
	*/
	vector<shared_ptr <Satellite>> getSatellites() const { return satellites; }
	
	/**
	* \brief If there is only one satellite on the vector - return it
	* \return Smart pointer to the satellite
	*/
	shared_ptr <Satellite> getSatellite() const;

	/**
	* \brief Get satellite from vector by its name
	* \param name name of the satellite
	* \return Smart pointer to the satellite
	*/
	shared_ptr <Satellite> getSatelliteByName(string const& name) const;

	/**
	* \brief Get amount of satellites in vector
	* \return Integer amount of satellites
	*/
	int getAmount() const { return satellites.size(); };

	/**
	* \brief Get connection to the antenna
	*/
	void connectAntena();

private:
	shared_ptr<TleData> tle;
	shared_ptr<ComPort> port;
	vector<shared_ptr <Satellite>> satellites;
};