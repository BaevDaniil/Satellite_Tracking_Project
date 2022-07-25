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


//������ ��������� ������ +
//����� ����� ��������, �������, ����� +
//std::optional -no need

//��������� ��������� ��������� �����������
//�-�� ������ ��� ����� (���� �������� �� ���������)
//������� �������� - �������� ��� ��������� - ���� ����� ����� ������
//� ����� ��� ���, �������(������ ��������, ���������, �������, ������� ������� ��������� �������) + ������� help
//������������� ������


// ����� ��������, ��� ������� ���������� 0..... ���? �� ���� .... ������������ ������ ���������� �������� ����� ����,




int main(int argc, char* argv[]) {

	shared_ptr<Program> program = make_shared<Program>(argc, argv);
	program->Run();

	//SatTrackInterface* track = new SatTrackInterface(1, (string)"NOAA 15");
	//shared_ptr<Satellite> sat = track->GetSatellite();
	//sat->UpdateData();
	//
	//sat->UpdatePassInfo(DateTime::Now());
	//cout << DateTime::Now() << endl << sat->GetAos() << endl;
	//struct Info {
	//	int val;
	//	string name;
	//};

	//list<Info> _list;
	//for (int i = 0; i < 10; i++) {
	//	Info info = { i, "a" };
	//	_list.push_back(info);
	//}
	//int j = 5;
	//list<Info>::const_iterator it = _list.begin();
	//while (j >= it->val) {
	//	_list.pop_front();
	//	it = _list.begin();
	//}
	//cout << it->val;
	return 0;
}