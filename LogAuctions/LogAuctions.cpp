#include "stdafx.h"
#include <zmq.hpp>
#include <iostream>
#include <fstream>
#include <ctime>
#include <Windows.h>
#include <msclr\marshal_cppstd.h>

/*! Author - Conor Hayes */

/*
	Setting up 0mq in VS2013 -> http://lists.zeromq.org/pipermail/zeromq-dev/2013-July/022136.html
	The documentation was referred to for 0mq usage -> http://api.zeromq.org/2-1:zmq-cpp
	Config file -> http://www.codeproject.com/Articles/21036/Working-with-Configuration-Files-app-config-in-C-C
				   http://stackoverflow.com/questions/16732789/system-a-namespace-with-this-name-does-not-exist
				   http://stackoverflow.com/questions/946813/c-cli-converting-from-systemstring-to-stdstring
				   http://www.codeproject.com/Questions/542628/Addingplusapp-configplustoplusaplusC-b-b-fCLIpl
	ParseString -> http://stackoverflow.com/questions/18800796/c-get-string-between-two-delimiter-string
	Coding Standards -> http://www.stack.nl/~dimitri/doxygen/manual/docblocks.html#cppblock
*/

using namespace System::Configuration;
using namespace std;

// Function declarations
void writeToLogFile(string message);
string parseMessage(string message, string startStr, string endStr);

//! Main Function
int _tmain(int argc, _TCHAR* argv[]){
	zmq::context_t context(1);
	zmq::socket_t subscriber(context, ZMQ_SUB);

	// Connect to each address from the configuration file
	for each(System::String ^address in ConfigurationManager::AppSettings){
		string addr = msclr::interop::marshal_as<std::string>(
			ConfigurationManager::AppSettings[address]);
		subscriber.connect(addr.c_str());
	}

	// Set the topic to empty - all topics
	subscriber.setsockopt(ZMQ_SUBSCRIBE, "", 0);
	cout << "SUB: All commands & events..." << endl;

	while (true){
		// Receive the message and write it to the log file
		zmq::message_t update;
		subscriber.recv(&update);
		string message = string(static_cast<char*>(update.data()), update.size());
		cout << message << endl;
		writeToLogFile(message);
	}

	return 0;
}

//! WriteToLogFile Function
/*!
	Writes the message parameter to a log file along with a timestamp
*/
void writeToLogFile(string message){
	char date[10];
	char time[10];
	// Get the date and time
	_strdate_s(date);
	_strtime_s(time);
	ofstream myfile;
	// Extract the ID for the file, each auction has separate log file
	string id = parseMessage(message, "<id>", "</id>");
	// Open and append to the file
	try{
		myfile.open("auction_" + id + ".log", ios_base::app);
		myfile << date << " " << time << " " << message << endl;
	}
	catch (const exception& ex){
		cout << ex.~exception;
	}
	finally{
		myfile.close();
	}
}

//! ParseMessage Function
/*!
	Parses message given start and end delimiters and returns the result
*/
string parseMessage(string message, string startStr, string endStr){
	unsigned first = message.find(startStr) + startStr.length();
	unsigned last = message.find(endStr);
	return message.substr(first, last - first);
}