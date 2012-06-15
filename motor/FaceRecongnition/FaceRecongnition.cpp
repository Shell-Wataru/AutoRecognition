#include "FaceRecongnition.h"

const char* cascadeFile="C:\\OpenCV2.2\\data\\haarcascades\\haarcascade_frontalface_default.xml";

FaceRecongnition::FaceRecongnition(double s):
scale(s)
{
	cvHCC = (CvHaarClassifierCascade*)cvLoad(cascadeFile);
	cvMStr = cvCreateMemStorage(0);
}


FaceRecongnition::~FaceRecongnition(void)
{
	cvReleaseMemStorage(&cvMStr);
	cvReleaseHaarClassifierCascade(&cvHCC);
}


bool FaceRecongnition::detect(IplImage* frame){
	if(!frame)return false;
	IplImage* gray = cvCreateImage(cvSize(frame->width, frame->height), IPL_DEPTH_8U, 1);
	IplImage* detect_frame = cvCreateImage(cvSize((frame->width / scale), (frame->height / scale)), IPL_DEPTH_8U, 1);
	cvCvtColor(frame, gray, CV_BGR2GRAY);
	cvResize(gray, detect_frame, CV_INTER_LINEAR);
	cvEqualizeHist(detect_frame, detect_frame);
	//‰æ‘œ’†‚©‚çŒŸo‘ÎÛ‚Ìî•ñ‚ğæ“¾‚·‚é
	face = cvHaarDetectObjects(detect_frame, cvHCC, cvMStr, 1.1, 2, CV_HAAR_DO_CANNY_PRUNING, cvSize(30, 30) );
	if(face->total==0)return false;
	return true;
}