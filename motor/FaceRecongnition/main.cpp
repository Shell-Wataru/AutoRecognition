
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <ctype.h>
#include "FaceRecongnition.h"
#include "..\..\testIP\testIP\SerialLib.h"

#define SCALE0 1.3
#define SCALE SCALE0
int
main (int argc, char **argv)
{
  CvCapture *capture = 0;
  IplImage *frame = 0;
  double w = 320, h = 240;
  int c;

  capture = cvCreateCameraCapture (0);

  /* この設定は，利用するカメラに依存する */
  // (2)キャプチャサイズを設定する．
  cvSetCaptureProperty (capture, CV_CAP_PROP_FRAME_WIDTH, w);
  cvSetCaptureProperty (capture, CV_CAP_PROP_FRAME_HEIGHT, h);

  cvNamedWindow ("Capture", CV_WINDOW_AUTOSIZE);
  FaceRecongnition fr(SCALE0);
  // (3)カメラから画像をキャプチャする
  SerialLib::APSerial sl=SerialLib::create();

  while (1) {
    frame = cvQueryFrame (capture);
    cvShowImage ("Capture", frame);
	if(fr.detect(frame)){
		CvPoint dire=cvPoint(frame->width/2,frame->height/2);
		Dorder order=fr.getDirection(frame,frame->width/16,frame->height/8);
		if(order.goLeft){
			dire.x-=200;
			sl->left();
		}
		else if(order.goRight){
			dire.x+=200;
			sl->right();
		}else{
			sl->stop();
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
	}else{
		sl->udstop();
		sl->stop();
	}
	cvShowImage ("Capture", frame);
    c = cvWaitKey (2);
    if (c == '\x1b')
      break;
  }

  cvReleaseCapture (&capture);
  cvDestroyWindow ("Capture");

  return 0;
}
