#include <stdio.h>
#include<iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include "tinyxml2.h"
#include "ConnextSelf.h"
#include "SerialLib.h"
using namespace std;

int main() {
	
	// �e��p�����[�^
	ConnextSelf cself;
	SerialLib::APSerial _serial=SerialLib::create();
	
	//�ڑ�
	if(cself.start()){
		printf("�ڑ��ł��܂���ł���\n");
		char c;cin>>c;
		return(-1);
	}
	if(_serial->isFailed()){
		printf("Serial�ڑ��ł��܂���ł���\n");
		char c;cin>>c;
		return(-1);
	}

  	printf("�ڑ����܂���\n");
	tinyxml2::XMLDocument doc;
	
	while (1){
		
		//�p�P�b�g�̎�M
		char* buffer=cself.recieve();

		JuliusResult jr=JuliusResult::getResult(buffer);

		if(jr.dire!=KEEP){
			cout<<"Recognized!"<<endl;
			cout<<"Score="<<jr.score<<endl;
			cout<<"CM="<<jr.cm<<endl;
			cout<<"Direction="<<JuliusResult::sDirection[jr.dire]<<endl;
			if(jr.cm>0.8){
				_serial->move(jr.dire);
			}
		}else{
			cout<<"Not Recoginized..."<<endl;
		}
		printf("\n+++++++++++++\n");
	}

	
	return(0);
}
