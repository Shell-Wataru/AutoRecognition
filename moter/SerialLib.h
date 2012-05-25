#pragma once
#include <string>
#include<windows.h>
#include<memory>

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
	void writeByte(char b);
	std::string read();
	typedef std::auto_ptr<SerialLib> APSerial;
	static APSerial create(){
		return APSerial(new SerialLib());
	}
private:
	HANDLE hCom;
};

