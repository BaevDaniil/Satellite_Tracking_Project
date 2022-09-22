#include "Program.hpp"


Program::Program(int const& argc, char** argv) : argc(argc), argv(argv) {
	desc.add_options()
		("satName", po::value<vector<string>>(&params), "Name of satellite")
		("days", po::value<int>()->default_value(3), "Amount of days for prediction")
		("predict", "Create schedule for satellite")
		("track", "track one satellite")
		("autoTrack", "track several satellites in turn")
		("longitude", po::value<double>()->default_value(30.37428867), "longitude of the view point")
		("latitude", po::value<double>()->default_value(60.0075085), "latitude of the view point")
		("timeZone", po::value<int>()->default_value(3), "time zone")
		("help", "Show options");
}


void Program::run() {

	po::variables_map vm = readCmdLine(argc, argv);

	if (vm.count("help")) {
		cout << desc << endl;
		return;
	}

	shared_ptr<SatTrackInterface> trackSat = make_shared< SatTrackInterface>(params, vm["latitude"].as<double>(), vm["longitude"].as<double>(), vm["timeZone"].as<int>());

	if (vm.count("predict")) {
		int days = vm["days"].as<int>();
		predict(trackSat, days);
	}
	if (vm.count("track")) {
		trackSat->connectAntena();
		track(trackSat);
	}
	if (vm.count("autoTrack")) {
		trackSat->connectAntena();
		autoTracking(trackSat);
	}
}


po::variables_map Program::readCmdLine(int const& argc, char** argv) {
	po::variables_map vm;

	// parse arguments
	po::store(po::parse_command_line(argc, argv, desc), vm);
	// check arguments

	try {
		po::notify(vm);
	}
	catch (std::exception& e) {
		std::cout << "Error: " << e.what() << std::endl;
		std::cout << desc << std::endl;
		return nullptr;
	}

	return vm;
}


void Program::autoTracking(const shared_ptr<SatTrackInterface>& track) {
	DateTime currentTime = DateTime::Now();
	shared_ptr<Satellite> currentSat = make_shared<Satellite>();

	vector<shared_ptr<Satellite>> satList = track->getSatellites();
	vector<shared_ptr<Satellite>> visibleSatList;
	currentSat = nullptr;
	while (true) {
		currentTime = DateTime::Now();
		if (!currentSat) {
			for (auto& s : satList) {
				s->updateData();
				s->updatePassInfo(currentTime);
			}

			for (auto& sat : satList) {
				if (sat->isVisible()) {
					visibleSatList.push_back(sat);
				}
			}
			currentSat = (visibleSatList.size() == 0 ? nextSat(satList) : maxElevationSat(visibleSatList));
			
		}
		
		// set antenna in necessary position for waiting next satellite
		track->antenna->trackSatellite(currentSat);
		track->antenna->updateCurrentAngles();
	
		currentTime = DateTime::Now();

		if (currentTime >= currentSat->getLos()) {
			currentSat = nullptr;		
			visibleSatList.clear();
		}
	}
}

void Program::track(const shared_ptr<SatTrackInterface>& track) {
	vector<shared_ptr<Satellite>> satList = track->getSatellites();
	shared_ptr<Satellite> satTrack = track->getSatellite();
	while (true) {
		track->antenna->trackSatellite(satTrack);
	}
}

void Program::predict(const shared_ptr<SatTrackInterface> track, int const& days) {
	vector<shared_ptr<Satellite>> satList = track->getSatellites();
	for (auto& sat : satList) {
		try {
			sat->createSchedule(days);
		}
		catch (const std::exception& ex) {
			cout <<  ex.what() << endl;
			exit(-1);
		}
		
		sat->writeScheduleIFile();
	}
}

shared_ptr<Satellite> Program::maxElevationSat(const vector<shared_ptr<Satellite>>& satList) {
	auto maxElevationSat = std::max_element(satList.begin(), satList.end(), 
		[] (shared_ptr<Satellite> const &sat_a, 
			shared_ptr<Satellite> const &sat_b) -> bool
		{
			return sat_a->getElevation() < sat_b->getElevation();
		});
	return *maxElevationSat;
}

shared_ptr<Satellite> Program::nextSat(const vector<shared_ptr<Satellite>>& satList) {
	auto nextSat = min_element(satList.begin(), satList.end(),
		[](shared_ptr<Satellite> const& sat_first, shared_ptr<Satellite> const& sat_second) 
		{
			if (sat_first->getAos() == sat_second->getAos()) {
				return sat_first->getElevation() < sat_second->getElevation();
			}
			return sat_first->getAos() < sat_second->getAos();
		});
	return *nextSat;
}

