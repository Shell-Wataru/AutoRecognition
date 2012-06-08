#pragma once
#include <winsock2.h>
#include <ws2tcpip.h>
#include "tinyxml2.h"

class ConnextSelf
{
public:
	ConnextSelf(int port=10500,int bufSize=2048);
	~ConnextSelf(void);
	char* recieve();
	void close();
	int start();
	char* buffer;
	int _bufSize;
	int _port;
	struct sockaddr_in _dstAddr;
	int dstSocket;
};


class JuliusResult{
public:
	enum Direction{
	STOP,
	FRONT,
	RIGHT,
	LEFT,
	BACK,
	KEEP
	};
	static char* sDirection[];
public:
	double score;
	Direction dire;
	JuliusResult(Direction d=Direction::KEEP,double s=0):
	score(s),dire(d){
	}
	static tinyxml2::XMLDocument doc;
	static JuliusResult getResult(const char* buffer);
};
