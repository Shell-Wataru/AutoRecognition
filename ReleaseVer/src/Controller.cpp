
#include "Controller.h"
#include "LibFaceRecong.h"



Controller::Controller(void)
{
	sl=SerialLib::create();
}


Controller::~Controller(void)
{
}


void Controller::update(IplImage* frame,CvPoint noseCenter){
		if(isUpdated)return;
		
		isUpdated=true;

		CvPoint dire=cvPoint(frame->width/2,frame->height/2);
		Dorder order=getDirection(frame,noseCenter,frame->width/16,frame->height/8);
		if(order.goLeft){
			dire.x-=200;
			sl->left();
		}
		else if(order.goRight){
			dire.x+=200;
			sl->right();
		}else{
			if(specifyFace==PersonName::Kai){
				sl->go();
			}else{
				sl->stop();
			}
		}

		if(order.goUp){
			dire.y-=100;
			sl->up();
		}
		else if(order.goDown){
			dire.y+=100;
			sl->down();
		}else {
			sl->udstop();
		}
		cvCircle(frame,dire,10,CV_RGB(0,0,255),5);
	}