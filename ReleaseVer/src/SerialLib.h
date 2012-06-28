#pragma once
#include <string>
#include<windows.h>
#include<memory>

#include "general.h"

class SerialLib
{
public:
	SerialLib(void);
	~SerialLib();
	void go();
	void back();
	void left();
	void right();
	void stop();
	
	void up();
	void down();
	void udstop();

	void move(Direction d){
		if(d==Direction::STOP){
			stop();
		}
		else if(d==Direction::FRONT){
			go();
		}
		else if(d==Direction::LEFT){
			left();
		}else if(d==Direction::RIGHT){
			right();
		}else if(d==Direction::BACK){
			back();
		}
	}
	void writeByte(char b);
	std::string read();
	bool isFailed(){
		return hCom == INVALID_HANDLE_VALUE;
	}
	typedef std::auto_ptr<SerialLib> APSerial;
	static APSerial create(){
		return APSerial(new SerialLib());
	}
private:
	HANDLE hCom;
};

