#include "StdAfx.h"
#include "SerialLib.h"

#include<iostream>
using namespace std;

LPCTSTR portName="COM4";

SerialLib::SerialLib(void)
{
	hCom = CreateFile(portName, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
    if (hCom == INVALID_HANDLE_VALUE){ 
		cout<<"failed to connect"<<endl;
        return;
	}
    DCB dcb;
    BuildCommDCB("9600,n,8,1",&dcb);
    SetCommState(hCom, &dcb);
}

string SerialLib::read(){
	string strText="";
	if (hCom != INVALID_HANDLE_VALUE)
	{
		DWORD dwError;
		COMSTAT comStat;

		if (ClearCommError(hCom, &dwError, &comStat))
		{
			if (comStat.cbInQue)
			{
				char* buff = new char [comStat.cbInQue];
				DWORD NumberOfBytesRead = 0;

				if (ReadFile(hCom, buff, comStat.cbInQue, &NumberOfBytesRead, NULL))
				{
					strText=string(buff, NumberOfBytesRead);
					cout<<strText<<endl;
				}
				delete [] buff;
			}
		}
	}
	return strText;
}

void SerialLib::go(){
	writeByte('g');
}

void SerialLib::back(){
	writeByte('b');
}
void SerialLib::left(){
	writeByte('l');
}
void SerialLib::right(){
	writeByte('r');
}
void SerialLib::stop(){
	writeByte('s');
}

void SerialLib::writeByte(char b){
	if (hCom != INVALID_HANDLE_VALUE)
    {
        string strText ="";
		strText+=b;
        DWORD dwBytes = 0;
        WriteFile(hCom, (LPCTSTR)(strText.c_str()), strText.size(), &dwBytes, NULL);
    }
}

SerialLib::~SerialLib(){
	if (hCom != INVALID_HANDLE_VALUE)
    {
        CloseHandle(hCom);
        hCom = INVALID_HANDLE_VALUE;
    }
}
