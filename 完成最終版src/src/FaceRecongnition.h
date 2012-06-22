#pragma once
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <ctype.h>


struct Dorder{
	bool goLeft;
	bool goRight;
	bool goDown;
	bool goUp;
};

class FaceRecongnition
{
public:
	CvHaarClassifierCascade* cvHCC;
	CvMemStorage* cvMStr;
	CvSeq* face;
	double scale;
	bool detect(IplImage* frame);
	FaceRecongnition(double s=1.0);
	~FaceRecongnition(void);
	
	Dorder getDirection(IplImage* frame,int epsw=2,int epsh=2){
		CvRect* faceRect=0;
		if(face->total!=0){
			int fidx=0;
			double bufValue=0;
			for(int i=0;i<face->total;i++){
				faceRect = (CvRect*)cvGetSeqElem(face, 0);
				double value=faceRect->height*faceRect->width;
				if(value>bufValue){
					fidx=i;
					bufValue=value;
				}
			}
			faceRect = (CvRect*)cvGetSeqElem(face, fidx);
		}
		Dorder order={false,false,false,false};
		if(faceRect==0)return order;

		cvRectangle(frame,
			cvPoint(faceRect->x * scale, faceRect->y * scale),
			cvPoint((faceRect->x + faceRect->width) * scale, (faceRect->y + faceRect->height) * scale),
			CV_RGB(0, 255 ,0),
			2, CV_AA);

		double wwc=frame->width/(scale*2.0);double whc=frame->height/(scale*2.0);
		double fx=faceRect->x+(faceRect->width)/2.0;double fy=faceRect->y+(faceRect->height)/2.0;
		
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
};
