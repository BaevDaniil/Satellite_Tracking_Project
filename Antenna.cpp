#include "Antenna.hpp"
using namespace std;

void Antenna::TrackSatellite(shared_ptr<Satellite> sat) {
	//Handle handle;
	string path =   sat->GetName() + ".txt";
	ofstream file;
	// string path = name + "txt";
	file.open(path);
	//handle.OpenNewFile(sat->GetName());
	int curAzimuth = 0, curElevation = 0;
	currentSat = sat;
	bool wait = false;

	currentSat->UpdateData();
	currentSat->UpdatePassInfo(DateTime::Now());
	int second = currentSat->GetTime().Second();
	while (1) {
		currentSat->UpdateData();
		if (!currentSat->IsVisible()) {
			if (!parked) {
				port->TurnOnAngles(AntennaParkAzimuth(), 0);
				cout << AntennaParkAzimuth();
				parked = true;
			}
			//handle.WriteFile(sat->GetAos(), AntennaParkAzimuth(), 0);
			file << "Sat " << currentSat->GetAzimuthByTime(currentSat->GetAos()) << " Ant: " << AntennaParkAzimuth() << endl;
			return;
		}

		if (IsWaiting() && sat->GetAzimuth() > AntennaParkAzimuth()) {
			cout << "Need to wait " << endl;
			file << "Need to wait" << endl;
			return;
		}

		if (NeedToConvertAngle()) {
			if (sat->GetAzimuth() > 0 && sat->GetAzimuth() < 90) {
				curAzimuth = ConvertAngle(sat->GetAzimuth());
				file << "Convert angles " << endl;
				file << sat->GetAzimuth() << " " << curAzimuth;
				cout << "Convert angles " << endl;
				cout  << sat->GetAzimuth() << " " << curAzimuth;
			}
		}
		else {
			curAzimuth = sat->GetAzimuth();
		}
		curElevation = sat->GetElevation();
		port->TurnOnAngles(curAzimuth, curElevation);
		UpdateCurrentAngles();
		//handle.WriteFile(sat->GetLocalTime(), sat->GetAzimuth(), sat->GetElevation(), GetAzimuth(), GetElevation());
		file  << time << endl << " Az: " << sat->GetAzimuth() << " El: " << sat->GetElevation() << endl << " AAz: " << GetAzimuth() << " AEl " << GetElevation() << endl;;
		// for check
		cout << sat->ToLocalTime(sat->GetTime()) << endl;
		cout << sat->GetName() << endl;
		cout << "Sat: Az: " << sat->GetAzimuth() << " El: " << sat->GetElevation() << endl;
		cout << "Ant: Az: " << azimuth << " El: " << elevation << endl;
		Sleep(1000);
	}
	//handle.CloseFile();
}

void Antenna::UpdateCurrentAngles() {
	elevation = port->GetElevation();
	azimuth = port->GetAzimuth();
}
int Antenna::ParkAzimuthToWest(int const& aosAz, int const& losAz) {

	// Satellite doesn't cross 0 azimuth
	// Antena can track it all the way
	if (!CrossZero()) {
		cout << "doesn't cross zero point" << endl;
		return aosAz;
	}

	if (aosAz < 90) {
		return 360 + aosAz;   // no delay, need to convert angles
	}
	if (aosAz - 90 < 360 - losAz) {
		return 450; //delay
	}
	if (aosAz - 90 >= 360 - losAz) {
		return aosAz; // no delay
	}
	return 0;
}

// �� ������
int Antenna::ParkAzimuthToEast(int const& aosAz, int const& losAz) {
	// Satellite doesn't cross 0 azimuth
	// Antena can track it all the way
	if (!CrossZero()) {
		cout << "doesn't cross zero point" << endl;
		return aosAz;
	}
	cout << "cross zero" << endl;
	if (losAz < 90) {
		return aosAz;   /// no delay, but convert angles
	}
	if (360 - aosAz >= losAz - 90) {
		return aosAz;
	}
	if (360 - aosAz < losAz - 90) {
		return 0; // delay
	}
	return 0;
}



bool Antenna::IsWaiting() {
	int aosAz = currentSat->GetAzimuthByTime(currentSat->GetAos());
	int losAz = currentSat->GetAzimuthByTime(currentSat->GetLos());
	if (currentSat->GetDirection() == Direction::west) {
		return DelayToWest(aosAz, losAz);
	}
	if (currentSat->GetDirection() == Direction::east) {
		return DelayToEast(aosAz, losAz);
	}
	else {
		throw exception("Something went wrong. Unknown direction");
		exit(-1);
	}
}

int Antenna::AntennaParkAzimuth() {
	int aosAz = currentSat->GetAzimuthByTime(currentSat->GetAos());
	int losAz = currentSat->GetAzimuthByTime(currentSat->GetLos());
	cout <<"Aos Az : " <<  aosAz << " Los Az : " << losAz << endl;
	if (currentSat->GetDirection() == Direction::west) { return ParkAzimuthToWest(aosAz, losAz); }
	if (currentSat->GetDirection() == Direction::east) { return ParkAzimuthToEast(aosAz, losAz); }
	else {
		throw exception("Something went wrong. Unknown direction");
		exit(-1);
	}
}

// ������� �������, ��� west ��� �����....
bool Antenna::DelayToWest(int const& aosAz, int const& losAz) {
	return aosAz > 90 && aosAz - 90 < 360 - losAz;
}

bool Antenna::DelayToEast(int const& aosAz, int const& losAz) {
	return losAz > 90 && 360 - aosAz < losAz - 90;
}

bool Antenna::AzimuthIsInreasing() {
	double azFirst = currentSat->GetAzimuthByTime(currentSat->GetAos());
	double azSecond = currentSat->GetAzimuthByTime(currentSat->GetAos().AddMinutes(1.0));
	return azFirst < azSecond;
}

bool Antenna::CrossSiteLongtitude() {
	double aosLong = currentSat->GetLongitudeByTime(currentSat->GetAos());
	double losLong = currentSat->GetLongitudeByTime(currentSat->GetLos());
	double siteLong = currentSat->radiansToDegrees(currentSat->GetSiteInfo().GetLocation().longitude);
	return (siteLong > losLong && siteLong < aosLong) || (siteLong < losLong&& siteLong > losLong);
}

bool Antenna::CrossZero() {
	//�� �����
	if (currentSat->GetDirection() == Direction::west) {
		return (currentSat->GetLos() > currentSat->GetAos()) && !AzimuthIsInreasing() && CrossSiteLongtitude();
	}
	//�� ������
	if (currentSat->GetDirection() == Direction::east) {
		return (currentSat->GetAos() > currentSat->GetLos()) && AzimuthIsInreasing() && CrossSiteLongtitude();
	}
}

bool Antenna::NeedToConvertAngle() {
	int aosAz = currentSat->GetAzimuthByTime(currentSat->GetAos());
	int losAz = currentSat->GetAzimuthByTime(currentSat->GetLos());

	if (currentSat->GetDirection() == Direction::west) {
		return CrossZero() && aosAz < 90;

	}
	if (currentSat->GetDirection() == Direction::east) {
		return CrossZero() && losAz < 90;
	}
}

int Antenna::ConvertAngle(int angle) {
	return angle + 360;
}