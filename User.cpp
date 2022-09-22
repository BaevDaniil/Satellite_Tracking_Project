#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <memory>

#include <boost/program_options.hpp>
#include "SatTrackInterface.hpp"
#include "ComPort.hpp"
#include "TleData.hpp"
#include "Satellite.hpp"
#include "Antenna.hpp"
#include "Program.hpp"

using std::vector;

using namespace std;

namespace po = boost::program_options;

// file:///C:/Users/EZinyakova/source/repos/html/index.html

int main(int argc, char* argv[]) {
	shared_ptr<Program> program = make_shared<Program>(argc, argv);
	program->run();
	return 0;
}