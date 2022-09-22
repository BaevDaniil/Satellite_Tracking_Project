#pragma once
#include <iostream>
#include <string>
#include <memory>
#include <windows.h>
#include "ComPort.hpp"
#include "Satellite.hpp"
#include "Handle.hpp"

class Antenna {
public:

	/**
	* \brief Default constructor for Antenna
	*/
	Antenna() = default;

	/**
	* \brief Constructor for Antenna
	* \param Smart pointer to the com port 
	*/
	Antenna(shared_ptr<ComPort> port) : port(port) {};

	/**
	* \brief Track satellite or park the antenna to the azimuth of the next satellite appearance. Write satellite's path (azimuth ans elevation) in file each second
	* \details Function choose the optimal antenna parking angle to avoid unnecessary turn around the axis. (Antena reserve angke is 90 degrees after 360)
	* \param sat Smart pointer to the tracking satellite
	*/
	void trackSatellite(shared_ptr<Satellite> sat);

	/**
	*\brief Get current antenna azimuth angle
	* \return current azimuth
	*/
	int getAzimuth() { return azimuth; }

	/**
	*\brief Get current antenna elevation angle
	* \return current elevation
	*/
	int getElevation() { return elevation; };

	/**
	* \brief Update current antenna angles (azimuth and elevation)
	*/
	void updateCurrentAngles();

	/**
	* \brief Turn the antennna to the zero position in azimuth and elevation
	*/
	void park() { port->turnOnAngles(0, 0); }

	/**
	* \brief Turn the antenna to the specified angles  
	* \param az Azimuth angle
	* \param el Elevation angle
	*/
	void move(int const& az, int const& el) { port->turnOnAngles(az, el); }

private:
	int azimuth = 0;
	int elevation = 0;
	bool waitingState = true;
	shared_ptr<ComPort> port;
	shared_ptr<Satellite> currentSat = nullptr;


	/**
	* \brief Calculate the optimal antenna angle for waiting the satellite, moving in the west direction
	* \param azimuth at the moment of acquisition of signal
	* \param azimuth at the moment of loss of signal
	* \return the optimal azimuth for waiting
	*/
	int parkAzimuthToWest(int const& aosAz, int const& losAz);


	/**
	* \brief Calculate the optimal antenna angle for waiting the satellite, moving in the east direction
	* \param aosAz azimuth at the moment of the acquisition of signal
	* \param losAz azimuth at the moment of the loss of signal
	* \return the optimal azimuth for waiting
	*/
	int parkAzimuthToEast(int const& aosAz, int const& losAz);

 	
	/**
	* \brief Decide if antenna needs to wait for satellite at the moment of acquisition of signal
	* \details It's necessary for antenna tracking optimization. The antenna is waiting in the end position and track satellite without extra turnings around the axis
	*/
	bool isWaiting() const;

	/**
	* \brief Calculate the optimal waiting azimuth angle for waiting the next satellite to avoid extra turning around the axis. 
	* \details Take into account the direction of movement (to the west or to the east). If the direction is unknown - exception 
	* \return The optimal angle gor waiting
	*/
	int antennaParkAzimuth();

	/**
	* \brief  Check if we need to wait while satellite is moving in the west direction
	* \param aosAz azimuth at the moment of the acquisition of signal
	* \param losAz azimuth at the moment of the loss of signal
	* \return true if we need to make a delay
	*/
	bool delayToWest(int const& aosAz, int const& losAz) const;

	/**
	* \brief  Check if we need to wait while satellite is moving in the east direction
	* \param aosAz azimuth at the moment of the acquisition of signal
	* \param losAz azimuth at the moment of the loss of signal
	* \return true if we need to make a delay
	*/
	bool delayToEast(int const& aosAz, int const& losAz) const;

	/**
	* \brief Check if azimuth is increasing
	* \return true if azimuth is increasing
	*/
	bool azimuthIsInreasing() const;

	/**
	* \brief Check if satellite crosses observer's longitude
	* \return true if the satellite crosses observer's longitude
	*/
	bool crossSitelongitude();

	/**
	* \brief Check if antenna needs to cross zero point while tracking the satellite
	* \ return true if it does
	*/
	bool crossZero();

	/**
	* \brief Decide if it's necessary to convert angles from 0-90 to 360-450
	* \ return true if we need
	*/
	bool needToConvertAngle();

	/**
	* \brief Convert angles by making one rotation around the axis
	* \param angle previous angle
	* \return new angle
	*/
	int convertAngle(int const& angle) const;
};