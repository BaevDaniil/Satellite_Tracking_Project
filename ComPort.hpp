#pragma once
#include <iostream>
#include <windows.h>

using namespace std;

class ComPort {
public:
	ComPort() { portName = L"COM3"; }; // default
	ComPort(wstring portName) : portName(portName) {};


	/**
	* \brief Connect to the COM port
	*/
	void getConnection();

	/**
	* \brief Close the port
	*/
	void closePort();

	/**
	* \brief Send command to the port
	* \param com String command for the port 
	*/
	void giveCommand(string const& com);

	/**
	* \brief Print data from COM port
	*/
	void readCOM();

	/**
	* \brief Give command to the port to turn at the specified angles
	* \param azimuth Azimuth angle
	* \param elevation Elevation angle
	*/
	void turnOnAngles(double azimuth, double elevation);

	/**
	* \brief Send command and print angles in form as they are given by port
	*/
	void printCurrentAngles();

	/**
	* \brief Send the command and get current azimuth
	* \return Integer azimuth angle
	*/
	int getAzimuth();

	/**
	* \brief Send the command and get current elevation
	* \return Integer elevation angle
	*/
	int getElevation();

	~ComPort() { closePort(); };

private:
	HANDLE hSerial{};
	wstring portName;

	/**
	* \brief Set parametrs for connection (by default)
	*/
	void setConnectionParams();  // default

	/**
	* \brief Get one angle from COM port
	* \return Integer angle from the port (3 digits)
	*/
	int readOneAngle();

	/**
	* \brief Convert data from port to ineger angle
	* \param data Data from port in form of char of array
	* \return Integer angle
	*/
	int toAngle(char* data);

	/**
	* \brief Make string for the port command to get both angles
	* \param az azimuth
	* \param el elevation
	* \return Command in the form of string for the port
	*/
	string makeTurnCommand(int const& az, int const& el);

	/**
	* \brief Convert integer angle to string for the port command
	* \param x integer angle
	* \return string angle
	*/
	string makeAngle(int x);
};