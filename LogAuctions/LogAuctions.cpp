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
	subscriber.connect("tcp://127.0.0.1:1000");
	subscriber.connect("tcp://127.0.0.1:1001");
	subscriber.connect("tcp://127.0.0.1:1010");
	subscriber.connect("tcp://127.0.0.1:1011");
	subscriber.connect("tcp://127.0.0.1:1100");
	subscriber.connect("tcp://127.0.0.1:1101");
	subscriber.connect("tcp://127.0.0.1:1110");
	subscriber.connect("tcp://127.0.0.1:1111");
	subscriber.connect("tcp://127.0.0.1:2000");
	subscriber.connect("tcp://127.0.0.1:2001");
	subscriber.connect("tcp://127.0.0.1:2010");
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
	myfile.open("auction1.log", ios_base::app);
	GetLocalTime(&time);
	myfile << message << " " << time.wHour << ":" << time.wMinute << ":" << time.wSecond << endl;
	myfile.close();
	return 0;
}