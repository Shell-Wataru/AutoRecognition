#pragma once 
#include "stdio.h"
#include "ctype.h"
#include "math.h"
#include "opencv\cv.h"
#include "opencv\cvaux.h"
#include "opencv\cvwimage.h"
#include "opencv\cxcore.h"
#include "opencv\cxmisc.h"
#include "opencv\highgui.h"


#define FACE_SIZE				 32		
#define MEN						  5
#define VV                       "learn_vv6_0.5.txt"
#define MARGIN_X                 1/10	//画像から取り除く横幅
#define MARGIN_Y                 1/10	//画像から取り除く縦幅
#define SCALE0                   1.3
#define SCALE                     5     //器官検出のためのサンプル画像拡大倍率
#define FRONTALFACE     "C:\\OpenCV2.2\\data\\haarcascades\\haarcascade_frontalface_default.xml"
#define EYE             "C:\\OpenCV2.2\\data\\haarcascades\\haarcascade_eye.xml"
#define NOSE            "C:\\OpenCV2.2\\data\\haarcascades\\haarcascade_mcs_nose.xml"
#define MOUTH           "C:\\OpenCV2.2\\data\\haarcascades\\haarcascade_mcs_mouth.xml"		//←かなり認識率が悪い
//#define DIS             //検出し、トリミングした顔を表示するモード
//#define EYE_NOSE          //目の位置から鼻を検出し、顔を切り出すモード
#define NOSE_ONLY         //鼻の位置だけから顔領域を切り出すモード

void loadLearnData();
void faceRecog(CvSeq* noseSeq,CvRect* faceRect,IplImage* tarImg,
			   double param1,double param2,double param3,double param4,
			   bool flag1, bool flag2
			   );
void searchEYE(CvSeq* eyeSeq,CvRect* faceRect,IplImage* tarImg,
			   double param1,double param2,double param3,double param4,
			   double param5,double param6,double param7,double param8,
			   bool flag
			   );
extern IplImage* recog;
extern IplImage* frame_copy;
extern int width;
extern int height;
extern CvFont font;
extern uchar vec[FACE_SIZE*FACE_SIZE];
extern int num;
extern IplImage* frame;
extern CvCapture* capture;
extern CvPoint nose_center;
extern int specifyFace;
class PersonName{
public:
	static const int Liang=1;
	static const int Kai=2;
};