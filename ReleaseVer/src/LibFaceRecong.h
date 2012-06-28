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
#define MARGIN_X                 1/10	//�摜�����菜������
#define MARGIN_Y                 1/10	//�摜�����菜���c��
#define SCALE0                   1.3
#define SCALE                     5     //�튯���o�̂��߂̃T���v���摜�g��{��
#define FRONTALFACE     "C:\\OpenCV2.2\\data\\haarcascades\\haarcascade_frontalface_default.xml"
#define EYE             "C:\\OpenCV2.2\\data\\haarcascades\\haarcascade_eye.xml"
#define NOSE            "C:\\OpenCV2.2\\data\\haarcascades\\haarcascade_mcs_nose.xml"
#define MOUTH           "C:\\OpenCV2.2\\data\\haarcascades\\haarcascade_mcs_mouth.xml"		//�����Ȃ�F����������
//#define DIS             //���o���A�g���~���O�������\�����郂�[�h
//#define EYE_NOSE          //�ڂ̈ʒu����@�����o���A���؂�o�����[�h
#define NOSE_ONLY         //�@�̈ʒu���������̈��؂�o�����[�h

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