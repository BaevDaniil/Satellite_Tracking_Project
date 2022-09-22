#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <boost/program_options.hpp>
#include "SatTrackInterface.hpp"

using std::vector;
namespace po = boost::program_options;

class Program {
public:
	Program() = default;
	Program(int const& argc, char** argv);

	/**
	* \brief run the program with certain user's command (predict, track, autoTrack or help)
	*/
	void run();

	/**
	* \brief Process the parametrs of the command line
	* \param argc 
	* \param argv
	*/
	po::variables_map readCmdLine(int const& argc, char** argv);

	/**
	* \brief Make schedule for the satellite and write it in the file
	* \param track Smart pointer to object of class SatTrackInterface
	* \param days Amount of days for schedule
	*/
	void predict(const shared_ptr<SatTrackInterface> track, int const& days);

	/**
	* \brief Track one satellite. If it is out of view - wait fo it
	* \param track Smart pointer to object of class SatTrackInterface
	*/
	void track(const shared_ptr<SatTrackInterface>& track);

	/**
	* \brief Tracking of several satellites with automatic switching betwen them
	* \details Choose the next satellite by the earliest aos or the max elevation
	* \param track Smart pointer to object of class SatTrackInterface
	*/
	void autoTracking(const shared_ptr<SatTrackInterface>& track);

private:
	int argc;
	char** argv;
	vector<string> params;
	po::options_description desc{ "Options" };

	/**
	* \brief Cjoose the next satellite for tracking
	* \details
	* \param satArray vector of smart pointers to satellites
	* \return next satellite that will be tracked
	*/
	shared_ptr<Satellite> nextSat(const vector<shared_ptr<Satellite>>& satArray);
	
	/**
	* \brief Choose the satellite with the max elevation
	* \param satArray vector of smart pointers to satellites
	* \return  satellite with max elevatoin
	*/
	shared_ptr<Satellite> maxElevationSat(const vector<shared_ptr<Satellite>>& satList);
};