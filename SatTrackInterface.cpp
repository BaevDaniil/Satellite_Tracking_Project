#include "SatTrackInterface.hpp"

SatTrackInterface::SatTrackInterface(LPCTSTR portName) {
	shared_ptr<Satellite> sat = make_shared<Satellite>();
	satellites.push_back(sat);
}

SatTrackInterface::SatTrackInterface(vector<string> const& names, double siteLat, double siteLong, int timeSpan) {
	tle = make_shared<TleData>();
	for (auto& name : names) {
		string info;
		try {
			info = tle->getSatelliteData(name);
			satellites.emplace_back(make_shared<Satellite>(info, name, siteLat, siteLong, timeSpan));
		}
		catch (const std::exception& ex) {
			cout << ex.what() << " Check the entered data " << endl;
			exit(-1);
		}
	}
}

void SatTrackInterface::connectAntena() {
	port = make_shared<ComPort>(L"COM3");
	try {
		port->getConnection();
	}
	catch (const std::exception& ex) {
		cout << ex.what() << endl;
		exit(-1);
	}
	antenna = make_shared<Antenna>(port);
}

shared_ptr<Satellite> SatTrackInterface::getSatellite() const {
	if (satellites.size() == 1) {
		return satellites.at(0);
	}
	return nullptr;
}

shared_ptr <Satellite> SatTrackInterface::getSatelliteByName(string const& name) const {
	for (auto& sat : satellites) {
		if (sat->getName() == name) {
			return sat;
		}
	}
	return nullptr;
}
