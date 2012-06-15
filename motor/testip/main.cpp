#include<stdio.h>
#include<iostream>
#include "SerialLib.h"
using namespace std;

int main(){
	SerialLib::APSerial sl=SerialLib::create();
	string s=sl->read();
	cout<<s<<endl;
	char c;
	while(cin>>c){
		if(c=='g'){
			sl->go();
		}
		if(c=='b'){
			sl->back();
		}
		if(c=='s'){
			sl->stop();
		}
		if(c=='r'){
			sl->right();
		}
		if(c=='l'){
			sl->left();
		}
		if(c=='q')break;
	}
	return 0;
}