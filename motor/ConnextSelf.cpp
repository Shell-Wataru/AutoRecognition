#include "ConnextSelf.h"

#define SELF_IP "127.0.0.1"

ConnextSelf::ConnextSelf(int port,int bufSize)
{
	_bufSize=bufSize;
	_port=port;
	
	WSADATA data;
	WSAStartup(MAKEWORD(2,0), &data);
	buffer=new char[bufSize];
	memset(&_dstAddr, 0, sizeof(_dstAddr));
	_dstAddr.sin_port = htons(port);
	_dstAddr.sin_family = AF_INET;
	_dstAddr.sin_addr.s_addr = inet_addr(SELF_IP);
	dstSocket = socket(AF_INET, SOCK_STREAM, 0);

}

void ConnextSelf::close(){
	closesocket(dstSocket);
}

int ConnextSelf::start(){
	return connect(dstSocket, (struct sockaddr *) &_dstAddr, sizeof(_dstAddr));
}

char* ConnextSelf::recieve(){
	recv(dstSocket, buffer,_bufSize,0);
	return buffer;
}

ConnextSelf::~ConnextSelf(void)
{
	delete[] buffer;
	WSACleanup();
}

char* JuliusResult::sDirection[]={"STOP","FRONT","RIGHT","LEFT","BACK","KEEP"};

tinyxml2::XMLDocument JuliusResult::doc;

JuliusResult JuliusResult::getResult(const char* buffer){
	using namespace tinyxml2;
	doc.Parse(buffer);
	XMLElement* recogout=doc.FirstChildElement("RECOGOUT");
	if(recogout==0){
		return JuliusResult();
	}
	XMLElement* shypo=recogout->FirstChildElement("SHYPO");
	if(shypo==0){
		return JuliusResult();
	}
	double score=shypo->DoubleAttribute("SCORE");
	XMLElement* whypo=shypo->FirstChildElement("WHYPO");
	Direction dire=Direction::KEEP;
	while(whypo!=0){
		const char* word=whypo->Attribute("WORD");
		whypo=whypo->NextSiblingElement();
		if(strcmp(word,"ëO")==0){
			dire=Direction::FRONT;
			break;
		}else if(strcmp(word,"âE")==0){
			dire=Direction::RIGHT;
			break;
		}else if(strcmp(word,"ç∂")==0){
			dire=Direction::LEFT;
			break;
		}else if(strcmp(word,"å„")==0){
			dire=Direction::BACK;
			break;
		}
	}
	return JuliusResult(dire,score);
}