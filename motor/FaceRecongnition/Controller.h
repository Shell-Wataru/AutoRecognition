#pragma once
#include "ConnextSelf.h"
#include "..\..\testIP\testIP\SerialLib.h"
#include <opencv/cv.h>
#include <opencv/highgui.h>

class Controller
{
public:
	SerialLib::APSerial sl;
	Direction dire;
	bool isUpdated;
	struct Dorder{
		bool goLeft;
		bool goRight;
		bool goDown;
		bool goUp;
	};
	void frameStartInit(){
		if(isUpdated==false){
			sl->stop();
			sl->udstop();
		}
		dire=Direction::KEEP;
		isUpdated=false;
	}

	void update(IplImage* frame,CvPoint noseCenter);

	Dorder getDirection(IplImage* frame,CvPoint noseCenter,int epsw=2,int epsh=2){

		Dorder order={false,false,false,false};

		double wwc=frame->width/(2.0);double whc=frame->height/(2.0);
		double fx=noseCenter.x;double fy=noseCenter.y;
		
		if(wwc+epsw<fx){
			order.goLeft=false;
			order.goRight=true;
		}else if(wwc>fx+epsw){
			order.goLeft=true;
			order.goRight=false;
		}
		if(whc+epsh<fy){
			order.goUp=false;
			order.goDown=true;
		}else if(whc>fy+epsh){
			order.goUp=true;
			order.goDown=false;
		}
		return order;
	}
	Controller(void);
	~Controller(void);
};

