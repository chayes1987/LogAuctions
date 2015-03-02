#include "stdafx.h"
#include <zmq.hpp>
#include <iostream>
#include <fstream>
#include <Windows.h>
#include <msclr\marshal_cppstd.h>

/*
	Setting up 0mq in VS2013 -> http://lists.zeromq.org/pipermail/zeromq-dev/2013-July/022136.html
	The documentation was referred to for 0mq usage -> http://api.zeromq.org/2-1:zmq-cpp
	Config file -> http://www.codeproject.com/Articles/21036/Working-with-Configuration-Files-app-config-in-C-C
				   http://stackoverflow.com/questions/16732789/system-a-namespace-with-this-name-does-not-exist
				   http://stackoverflow.com/questions/946813/c-cli-converting-from-systemstring-to-stdstring
				   http://www.codeproject.com/Questions/542628/Addingplusapp-configplustoplusaplusC-b-b-fCLIpl
*/

using namespace System::Configuration;
using namespace std;

int recordOutput(string message);

int _tmain(int argc, _TCHAR* argv[]){
	zmq::context_t context(1);
	zmq::socket_t subscriber(context, ZMQ_SUB);

	for each(System::String ^address in ConfigurationManager::AppSettings){
		string addr = msclr::interop::marshal_as<std::string>(ConfigurationManager::AppSettings[address]);
		subscriber.connect(addr.c_str());
	}

	subscriber.setsockopt(ZMQ_SUBSCRIBE, "", 0);
	cout << "SUB: All commands & events..." << endl;

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