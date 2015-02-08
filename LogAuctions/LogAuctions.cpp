#include "stdafx.h"
#include <zmq.hpp>

int _tmain(int argc, _TCHAR* argv[]){
	zmq::context_t context(1);
	zmq::socket_t subscriber(context, ZMQ_SUB);
	return 0;
}

