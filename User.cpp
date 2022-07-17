#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <iostream>
#include <string>
#include <boost/program_options.hpp>
#include "SatTrackInterface.hpp"
#include "ComPort.hpp"
#include "TleData.hpp"
#include "Satellite.hpp"
#include "Antenna.hpp"
#include "Program.hpp"
//
using std::vector;

using namespace std;

namespace po = boost::program_options;


// ������ ��������� ������ +
// ����� ����� ��������, �������, ����� +
// std::optional -no need

// ��������� ��������� ��������� �����������
// �-�� ������ ��� ����� (���� �������� �� ���������)
// ������� �������� - �������� ��� ��������� - ���� ����� ����� ������
// � ����� ��� ���, �������(������ ��������, ���������, �������, ������� ������� ��������� �������) + ������� help
// ������������� ������



int main(int argc, char** argv) {
	const shared_ptr<SatTrackInterface> track = make_shared< SatTrackInterface>(1, (string)"AQUA" );
	shared_ptr<Program> program = make_shared<Program>();
	program->Predict(track, 1);
	//program->Run();
	return 0;
}