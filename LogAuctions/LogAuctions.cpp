#include "stdafx.h"
#include <zmq.hpp>
#include <iostream>
#include <fstream>
#include <Windows.h>

using namespace std;

int recordOutput(string message);

int _tmain(int argc, _TCHAR* argv[]){
	zmq::context_t context(1);
	zmq::socket_t subscriber(context, ZMQ_SUB);
	subscriber.connect("tcp://172.31.32.20:2000");
	subscriber.connect("tcp://172.31.32.21:2100");
	subscriber.connect("tcp://172.31.32.22:2200");
	subscriber.connect("tcp://172.31.32.23:2300");
	subscriber.connect("tcp://172.31.32.23:2350");
	subscriber.connect("tcp://172.31.32.23:2360");
	subscriber.connect("tcp://172.31.32.23:2370");
	subscriber.connect("tcp://172.31.32.24:2400");
	subscriber.connect("tcp://172.31.32.25:2500");
	subscriber.connect("tcp://172.31.32.28:2800");
	subscriber.connect("tcp://172.31.32.29:2900");
	subscriber.setsockopt(ZMQ_SUBSCRIBE, "", 0);

	cout << "Subscribed to all commands & events..." << endl << endl;

	while (true){
		zmq::message_t update;
		subscriber.recv(&update);
		string message = string(static_cast<char*>(update.data()), update.size());
		cout << message << endl;
		recordOutput(message);
	}

	return 0;
}

int recordOutput(string message){
	SYSTEMTIME time;
	ofstream myfile;
	myfile.open("auctions.log", ios_base::app);
	GetLocalTime(&time);
	myfile << message << " " << time.wHour << ":" << time.wMinute << ":" << time.wSecond << endl;
	myfile.close();
	return 0;
}