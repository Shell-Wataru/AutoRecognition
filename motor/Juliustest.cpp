#include <stdio.h>
#include<iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include "tinyxml2.h"
#include "ConnextSelf.h"
using namespace std;

enum Direction{
	STOP,
	FRONT,
	RIGHT,
	LEFT,
	BACK,
	KEEP
};

int main() {
	
	// 各種パラメータ
	ConnextSelf cself;

	//接続
	if(cself.start()){
		printf("接続できませんでした\n");
		char c;cin>>c;
		return(-1);
	}
  	printf("接続しました\n");
	tinyxml2::XMLDocument doc;
	while (1){
		
		//パケットの受信
		char* buffer=cself.recieve();
		
	/*	doc.Parse(buffer);
		doc.Print();
		using namespace tinyxml2;
		
		XMLElement* recogout=doc.FirstChildElement("RECOGOUT");
		if(recogout!=0){
			cout<<"reconginited!!"<<endl;
			XMLElement* shypo=recogout->FirstChildElement("SHYPO");
			double score=shypo->DoubleAttribute("SCORE");
			cout<<"score="<<score<<endl;
			XMLElement* whypo=shypo->FirstChildElement("WHYPO");
			//XMLNode* nwhypo=whypo;
			Direction dire=Direction::KEEP;
			while(whypo!=0){
				const char* word=whypo->Attribute("WORD");
				whypo=whypo->NextSiblingElement();
				cout<<"word="<<word<<endl;
				if(strcmp(word,"前")==0){
					dire=Direction::FRONT;
					break;
				}else if(strcmp(word,"右")==0){
					dire=Direction::RIGHT;
					break;
				}else if(strcmp(word,"左")==0){
					dire=Direction::LEFT;
					break;
				}else if(strcmp(word,"後")==0){
					dire=Direction::BACK;
					break;
				}
			}
			char* sDirection[]={"STOP","FRONT","RIGHT","LEFT","BACK","KEEP"};
			cout<<sDirection[dire]<<endl;
		}*/
		JuliusResult jr=JuliusResult::getResult(buffer);
		if(jr.dire!=JuliusResult::KEEP){
			cout<<"Recognized!"<<endl;
			cout<<"Score="<<jr.score<<endl;
			cout<<"Direction="<<JuliusResult::sDirection[jr.dire]<<endl;
		}else{
			cout<<"Not Recoginized..."<<endl;
		}
		printf("\n+++++++++++++\n");
	}

	
	return(0);
}
