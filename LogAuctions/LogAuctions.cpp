#include "stdafx.h"
#include <zmq.hpp>
#include <iostream>
#include <fstream>
#include <ctime>
#include <Windows.h>
#include <msclr\marshal_cppstd.h>

/*
	Setting up 0mq in VS2013 -> http://lists.zeromq.org/pipermail/zeromq-dev/2013-July/022136.html
	The documentation was referred to for 0mq usage -> http://api.zeromq.org/2-1:zmq-cpp
	Config file -> http://www.codeproject.com/Articles/21036/Working-with-Configuration-Files-app-config-in-C-C
				   http://stackoverflow.com/questions/16732789/system-a-namespace-with-this-name-does-not-exist
				   http://stackoverflow.com/questions/946813/c-cli-converting-from-systemstring-to-stdstring
				   http://www.codeproject.com/Questions/542628/Addingplusapp-configplustoplusaplusC-b-b-fCLIpl
	ParseStrinbg -> http://stackoverflow.com/questions/18800796/c-get-string-between-two-delimiter-string
*/

using namespace System::Configuration;
using namespace std;

void recordOutput(string message);
string parseMessage(string message, string startStr, string endStr);

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

void recordOutput(string message){
	char date[10];
	char time[10];
	_strdate_s(date);
	_strtime_s(time);
	ofstream myfile;
	string id = parseMessage(message, "<id>", "</id>");
	myfile.open("auction_" + id + ".log", ios_base::app);
	myfile << date << " " << time << " " << message << endl;
	myfile.close();
}

string parseMessage(string message, string startStr, string endStr){
	unsigned first = message.find(startStr) + startStr.length();
	unsigned last = message.find(endStr);
	return message.substr(first, last - first);
}