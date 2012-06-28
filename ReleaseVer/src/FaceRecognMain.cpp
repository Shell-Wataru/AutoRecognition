#include "Controller.h"
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <ctype.h>
//#include "FaceRecongnition.h"
//#include "SerialLib.h"

#include "LibFaceRecong.h"


#include<iostream>
using namespace std;
#define SCALE0 1.3
#define SCALE SCALE0
int main (int argc, char **argv)
{

	Controller cont;
	/*ConnextSelf cself;
	
	//接続
	if(cself.start()){
		printf("接続できませんでした\n");
		char c;cin>>c;
		return(-1);
	}
	while (1){
		
		//パケットの受信
		char* buffer=cself.recieve();

		JuliusResult jr=JuliusResult::getResult(buffer);
		if(jr.isKai){
			break;
		}
		if(jr.dire!=KEEP){
			cout<<"Recognized!"<<endl;
			cout<<"Score="<<jr.score<<endl;
			cout<<"CM="<<jr.cm<<endl;
			cout<<"Direction="<<JuliusResult::sDirection[jr.dire]<<endl;
			if(jr.cm>0.8){
				cont.sl->move(jr.dire);
			}
		}else{
			cout<<"Not Recoginized..."<<endl;
		}
		printf("\n+++++++++++++\n");
	}*/
  capture = 0;
  frame = 0;
  int c;
  cvInitFont (&font, CV_FONT_HERSHEY_DUPLEX, 2, 2,0,4);
  for (int i = 0 ; i < FACE_SIZE*FACE_SIZE ; i++) vec[i] = 0;
  capture = cvCreateCameraCapture (0);

  /* この設定は，利用するカメラに依存する */
  // (2)キャプチャサイズを設定する．
  cvSetCaptureProperty (capture, CV_CAP_PROP_FRAME_WIDTH, width);
  cvSetCaptureProperty (capture, CV_CAP_PROP_FRAME_HEIGHT, height);
  cvNamedWindow("RecogFace", CV_WINDOW_AUTOSIZE);
  //FaceRecongnition fr(SCALE0);
  // (3)カメラから画像をキャプチャする
  //SerialLib::APSerial sl=SerialLib::create();

  	CvHaarClassifierCascade* cvHCC = (CvHaarClassifierCascade*)cvLoad(FRONTALFACE);
	CvHaarClassifierCascade* cvHCC_EYE = (CvHaarClassifierCascade*)cvLoad(EYE);
	CvHaarClassifierCascade* cvHCC_NOSE = (CvHaarClassifierCascade*)cvLoad(NOSE);

	CvMemStorage* cvMStr = cvCreateMemStorage(0);
	CvMemStorage* cvMStr_EYE = cvCreateMemStorage(0);
	CvMemStorage* cvMStr_NOSE = cvCreateMemStorage(0);
	loadLearnData();

		CvSeq* face;
#ifdef EYE_NOSE
	CvSeq* eye;
#endif EYE_NOSE
	CvSeq* nose;
	
	frame = cvQueryFrame (capture);
	IplImage* gray = cvCreateImage(cvSize(frame->width, frame->height), IPL_DEPTH_8U, 1);
	IplImage* detect_frame = cvCreateImage(cvSize((frame->width / SCALE0), (frame->height / SCALE0)), IPL_DEPTH_8U, 1);
  while (1) {
	  cont.frameStartInit();
		frame = cvQueryFrame (capture);
		//認識用イメージ生成
		frame_copy = cvCreateImage(cvSize(frame->width, frame->height), IPL_DEPTH_8U, 1);
		cvCvtColor(frame, frame_copy, CV_BGR2GRAY);
		recog = cvCreateImage (cvSize(FACE_SIZE,FACE_SIZE),frame_copy->depth,frame_copy->nChannels);
		cvZero(recog);
		if(!frame) break;
		//読み込んだ画像のグレースケール化、及びヒストグラムの均一化を行う

		cvCvtColor(frame, gray, CV_BGR2GRAY);
		cvResize(gray, detect_frame, CV_INTER_LINEAR);
		cvEqualizeHist(detect_frame, detect_frame);
		//画像中から検出対象の情報を取得する
		face = cvHaarDetectObjects(detect_frame,cvHCC,cvMStr,1.1,2,CV_HAAR_DO_CANNY_PRUNING,cvSize(30, 30));
		
		for (int i = 0; i < face->total; i++) {
			//検出情報から位置情報を取得
			CvRect* faceRect = (CvRect*)cvGetSeqElem(face, i);
			IplImage* gray_face = cvCreateImage(cvSize(faceRect->width, faceRect->height), IPL_DEPTH_8U, 1);
			cvSetImageROI(detect_frame,*faceRect);
			cvResize(detect_frame,gray_face,CV_INTER_LINEAR);
			cvResetImageROI(detect_frame);

#ifdef EYE_NOSE
			eye = cvHaarDetectObjects(gray_face,cvHCC_EYE, cvMStr_EYE);
#endif EYE_NOSE
			nose = cvHaarDetectObjects(gray_face,cvHCC_NOSE,cvMStr_NOSE);
			//mouth = cvHaarDetectObjects(gray_face,cvHCC_MOUTH,cvMStr_MOUTH);
#ifdef EYE_NOSE
			searchEYE(eye,faceRect,frame,0.0,0.425,0.12,0.4,0.575,1.0,0.12,0.4,1);
#endif EYE_NOSE
			faceRecog(nose,faceRect,frame,0.06,0.06,0.34,0.7,1,1);
			//searchMOUTH(mouth,faceRect,frame,0.08,0.7,1.0,0.16,1);
			
			//取得した顔の位置情報に基づき、矩形描画を行う
			if(nose->total!=0){
				cvRectangle(frame,
				cvPoint(faceRect->x * SCALE0, faceRect->y * SCALE0),
				cvPoint((faceRect->x + faceRect->width) * SCALE0, (faceRect->y + faceRect->height) * SCALE0),
				CV_RGB(0, 255 ,0),2,CV_AA);
				cont.update(frame,cvPoint(
					(int)((faceRect->x + (faceRect->width)/2.0) * SCALE0), 
					(int)((faceRect->y + (faceRect->height)/2.0)*SCALE0)));
			}
			//cvReleaseImage(&gray_face);
		}
		//snprintf(str,64,"%03d[frame]",num);
		//cvPutText(frame,str,cvPoint(10,15),&font,CV_RGB(255,0,0));
		cvShowImage ("RecogFace", frame);
		num++;
		c = cvWaitKey (5);
		if (c == 27) {break;}

		}

	/*
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
	}*/
  cvReleaseImage(&gray);
  cvReleaseImage(&detect_frame);
  cvReleaseCapture (&capture);
  cvDestroyWindow ("Capture");
  
	cvReleaseMemStorage(&cvMStr);
#ifdef EYE_NOSE
	cvReleaseMemStorage(&cvMStr_EYE);
#endif EYE_NOSE
	cvReleaseMemStorage(&cvMStr_NOSE);
	//cvReleaseMemStorage(&cvMStr_MOUTH);
	cvReleaseCapture (&capture);
	
	cvDestroyWindow("RecogFace");

	cvReleaseHaarClassifierCascade(&cvHCC);
#ifdef EYE_NOSE
	cvReleaseHaarClassifierCascade(&cvHCC_EYE);
#endif EYE_NOSE
	cvReleaseHaarClassifierCascade(&cvHCC_NOSE);
	//cvReleaseHaarClassifierCascade(&cvHCC_MOUTH);

  return 0;
}
