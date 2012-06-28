#include <stdio.h>
#include<iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include "tinyxml2.h"
#include "ConnextSelf.h"
#include "SerialLib.h"
using namespace std;

int main() {
	
	// 各種パラメータ
	ConnextSelf cself;
	SerialLib::APSerial _serial=SerialLib::create();
	
	//接続
	if(cself.start()){
		printf("接続できませんでした\n");
		char c;cin>>c;
		return(-1);
	}
	if(_serial->isFailed()){
		printf("Serial接続できませんでした\n");
		char c;cin>>c;
		return(-1);
	}

  	printf("接続しました\n");
	tinyxml2::XMLDocument doc;
	
	while (1){
		
		//パケットの受信
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
