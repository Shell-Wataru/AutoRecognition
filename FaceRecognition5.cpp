// FaceRecognition5.cpp : �R���\�[�� �A�v���P�[�V�����̃G���g�� �|�C���g���`���܂��B
//

#include "stdafx.h"
#include "stdafx.h"
#include "stdio.h"
#include "ctype.h"
#include "math.h"
#include "opencv\cv.h"
#include "opencv\cvaux.h"
#include "opencv\cvwimage.h"
#include "opencv\cxcore.h"
#include "opencv\cxmisc.h"
#include "opencv\highgui.h"
#include "opencv\ml.h"

#define snprintf _snprintf

//�v���W�F�N�g�̃v���p�e�B��C/C++�ˑS�ʁ@�̒ǉ��̃C���N���[�h�f�B���N�g����
// opencv2�̂���t�H���_�wC:\OpenCV\include�x�Ȃǂ�ǉ��̂���
#include "opencv2\opencv.hpp"
 
#ifdef _DEBUG
    //Debug���[�h�̏ꍇ
    #pragma comment(lib,"C:\\opencv\\build\\x86\\vc10\\lib\\opencv_core231d.lib")
    #pragma comment(lib,"C:\\opencv\\build\\x86\\vc10\\lib\\opencv_imgproc231d.lib")
    #pragma comment(lib,"C:\\opencv\\build\\x86\\vc10\\lib\\opencv_highgui231d.lib")
    #pragma comment(lib,"C:\\opencv\\build\\x86\\vc10\\lib\\opencv_objdetect231d.lib")
    #pragma comment(lib,"C:\\opencv\\build\\x86\\vc10\\lib\\opencv_contrib231d.lib")
    #pragma comment(lib,"C:\\opencv\\build\\x86\\vc10\\lib\\opencv_features2d231d.lib")
    #pragma comment(lib,"C:\\opencv\\build\\x86\\vc10\\lib\\opencv_flann231d.lib")
    #pragma comment(lib,"C:\\opencv\\build\\x86\\vc10\\lib\\opencv_gpu231d.lib")
    #pragma comment(lib,"C:\\opencv\\build\\x86\\vc10\\lib\\opencv_haartraining_engined.lib")
    #pragma comment(lib,"C:\\opencv\\build\\x86\\vc10\\lib\\opencv_legacy231d.lib")
    #pragma comment(lib,"C:\\opencv\\build\\x86\\vc10\\lib\\opencv_ts231d.lib")
    #pragma comment(lib,"C:\\opencv\\build\\x86\\vc10\\lib\\opencv_video231d.lib")
#else
    //Release���[�h�̏ꍇ
    #pragma comment(lib,"C:\\opencv\\build\\x86\\vc10\\lib\\opencv_core231.lib")
    #pragma comment(lib,"C:\\opencv\\build\\x86\\vc10\\lib\\opencv_imgproc231.lib")
    #pragma comment(lib,"C:\\opencv\\build\\x86\\vc10\\lib\\opencv_highgui231.lib")
    #pragma comment(lib,"C:\\opencv\\build\\x86\\vc10\\lib\\opencv_objdetect231.lib")
    #pragma comment(lib,"C:\\opencv\\build\\x86\\vc10\\lib\\opencv_contrib231.lib")
    #pragma comment(lib,"C:\\opencv\\build\\x86\\vc10\\lib\\opencv_features2d231.lib")
    #pragma comment(lib,"C:\\opencv\\build\\x86\\vc10\\lib\\opencv_flann231.lib")
    #pragma comment(lib,"C:\\opencv\\build\\x86\\vc10\\lib\\opencv_gpu231.lib")
    #pragma comment(lib,"C:\\opencv\\build\\x86\\vc10\\lib\\opencv_haartraining_engined.lib")
    #pragma comment(lib,"C:\\opencv\\build\\x86\\vc10\\lib\\opencv_legacy231.lib")
    #pragma comment(lib,"C:\\opencv\\build\\x86\\vc10\\lib\\opencv_ts231.lib")
    #pragma comment(lib,"C:\\opencv\\build\\x86\\vc10\\lib\\opencv_video231.lib")
#endif
/**********************************************************************************************\
 FaceRecognition5.cpp �ɂ���

 �J�������猟�o��������A�@�𒆐S�Ƃ��ēK���Ɂi�w�K�f�[�^���쐬�������Ɠ��l�Ɂj�g���~���O���s���A��������T�C�Y����
 �摜�̊e�s�N�Z���l����іځA�@�A���̋�������A���t�f�[�^�̂ǂ�ƍł��������邩���ʂ��s���B

 ����ɁA�F�����ꂽ�炪�������͂��ꂽ�^�[�Q�b�g�Ɠ���ł���΁A�����ǂ�������B�i��̒��S����ʂ̒��S�ɗ���悤��
 ��]���āA�O�i����j
 
\**********************************************************************************************/

#define FACE_SIZE				 32		
#define MEN						  5
#define MARGIN_X                 1/10	//�摜�����菜������
#define MARGIN_Y                 1/10	//�摜�����菜���c��
#define SCALE0                   1.3
#define SCALE                     5     //�튯���o�̂��߂̃T���v���摜�g��{��
#define FRONTALFACE     "C:\\opencv\\data\\haarcascades\\haarcascade_frontalface_default.xml"
#define EYE             "C:\\opencv\\data\\haarcascades\\haarcascade_eye.xml"
#define NOSE            "C:\\opencv\\data\\haarcascades\\haarcascade_mcs_nose.xml"
#define MOUTH           "C:\\opencv\\data\\haarcascades\\haarcascade_mcs_mouth.xml"		//�����Ȃ�F����������
//#define DIS             //���o���A�g���~���O�������\�����郂�[�h

CvFont font;
FILE *datafile;
//�r�f�I�L���v�`���\����
CvCapture* capture = 0;
// �t���[���P�ʃf�[�^
IplImage* frame = 0;
// �F���p�t���[��(��`�`��Ȃ�)
IplImage* frame_copy = 0;
IplImage* recog = 0;

int height = 240; //�L���v�`���̏c�T�C�Y
int width = 320; //�L���v�`���̉��T�C�Y
int c; //���̓L�[�󂯕t���p
int num = 0; //�t���[����
uchar vec[FACE_SIZE*FACE_SIZE];
double vv[MEN][FACE_SIZE*FACE_SIZE];
double obj[MEN];

CvPoint eye_leftup = cvPoint(0,0);
CvPoint eye_rightdown = cvPoint(0,0);
CvPoint nose_leftup = cvPoint(0,0);	
CvPoint nose_rightdown = cvPoint(0,0);	
CvPoint mouth_leftup = cvPoint(0,0);
CvPoint mouth_rightdown = cvPoint(0,0);	
CvPoint face_leftup = cvPoint(0,0);
CvPoint face_rightdown = cvPoint(0,0);
CvPoint reye_center = cvPoint(0,0);	
CvPoint leye_center = cvPoint(0,0);	
CvPoint nose_center = cvPoint(0,0);	
CvPoint mouth_center = cvPoint(0,0);

CvRect FACE;

//�O���[�X�P�[���̉摜����l�����o�����\�b�h
void getData(IplImage* img,uchar vec[FACE_SIZE*FACE_SIZE]){
	for (int i=0; i < img->height; i++){
		for (int j=0; j < img->width; j++){
			vec[FACE_SIZE*i+j] = img->imageData[img->widthStep*i+j];
		}
	}
}

void loadLearnData(double data[MEN][FACE_SIZE*FACE_SIZE]){
	datafile = fopen("learn_vv.txt","r");
	if (datafile == NULL) {
		printf("cannot open\n");
		exit(1);
	}
	for (int i = 0 ; i < MEN ; i++){
		for (int  j= 0 ; j < FACE_SIZE*FACE_SIZE ; j++){
			fscanf(datafile,"%d",&data[i][j]);
		}
	}
	fclose(datafile);
}

int specifyOne(){
	if(vec[0] = 0) return 0;
	
	for(int i = 0; i < MEN ; i++){
		for(int j = 0; j < FACE_SIZE*FACE_SIZE ; j++ ){
			obj[i] += vv[i][j]*vec[j]; 
		}
	}

	double tempmax = obj[0];
	int maxindex = 0;

	for(int i = 0; i < MEN ; i++) {
		if(obj[i]>tempmax) {
			tempmax = obj[i];
			maxindex = i;
		}
	}

	return maxindex+1;
}

//���ڂ�T���܂��A���ڂ̈ʒu�����ɕ@���T�m����܂�
void searchEYE(CvSeq* eyeSeq,CvRect* faceRect,IplImage* tarImg,
			   double param1,double param2,double param3,double param4,
			   double param5,double param6,double param7,double param8,
			   bool flag
			   ){
				   int px = faceRect->x;
				   int py = faceRect->y;
				   int pw = faceRect->width;
				   int ph = faceRect->height;

				   for (int j = 0; j < eyeSeq->total; j++){
						CvRect* eyeRect = (CvRect*)cvGetSeqElem(eyeSeq, j);
						int cx;
						int cy;
						eye_leftup.x = eyeRect->x;
						eye_leftup.y = eyeRect->y;
						eye_rightdown.x = eyeRect->x + eyeRect->width;
						eye_rightdown.y = eyeRect->y + eyeRect->height;
						cx = (int)(eye_leftup.x + eye_rightdown.x)/2-px;
						cy = (int)(eye_leftup.y + eye_rightdown.y)/2-py;
						if(((int)(pw*param1) < cx)&(cx < (int)(pw*param2))
							&((int)(ph*param3) < cy)&(cy < (int)(ph*param4))
							){
							reye_center.x = cx+px;
							reye_center.y = cy+py;
						}
						if(((int)(pw*param5) < cx)&(cx < (int)(pw*param6))
							&((int)(ph*param7) < cy)&(cy < (int)(ph*param8))
							){
							leye_center.x = cx+px;
							leye_center.y = cy+py;
						}
						if(flag){
							cvLine(tarImg,
								cvPoint((int)(reye_center.x-2)*SCALE0,(int)(reye_center.y-2)*SCALE0),
								cvPoint((int)(reye_center.x+2)*SCALE0,(int)(reye_center.y+2)*SCALE0),
								CV_RGB( 0 , 255 , 0));
							cvLine(tarImg,
								cvPoint((int)(leye_center.x-2)*SCALE0,(int)(leye_center.y+2)*SCALE0),
								cvPoint((int)(leye_center.x+2)*SCALE0,(int)(leye_center.y-2)*SCALE0),
								CV_RGB( 0 , 255 , 0));
						}
					}
}

//faceRecog�̒��Ŏg���܂�
//�@���T�m���ꂽ��ɁA�V���Ɋ�̗̈�(����p)���擾���Avec�Ɋi�[���܂�
void getfaceROI(CvRect* faceRect,bool flag){

		int px = faceRect->x;
		int py = faceRect->y;
		int pw = faceRect->width;
		int ph = faceRect->height;

		if (0 < nose_center.x && nose_center.x < width 
			&& 0 < nose_center.y && nose_center.y < height){
			face_leftup.x = nose_center.x-1.05*(nose_rightdown.x-nose_leftup.x);
			if (face_leftup.x < px) face_leftup.x = px+2;
			face_leftup.y = nose_center.y-1.4*(nose_rightdown.y-nose_leftup.y);
			if (face_leftup.y < py) face_leftup.y = py+2;
			face_rightdown.x = nose_center.x+1.05*(nose_rightdown.x-nose_leftup.x);
			if (face_rightdown.x > px+pw) face_rightdown.x = px+pw-2;
			face_rightdown.y = nose_center.y+1.1*(nose_rightdown.y-nose_leftup.y);
			if (face_rightdown.y > px+pw) face_rightdown.y = py+ph-2;

			FACE = cvRect(face_leftup.x*SCALE0,face_leftup.y*SCALE0,
					(face_rightdown.x-face_leftup.x)*SCALE0,
					(face_rightdown.y-face_leftup.y)*SCALE0);
			cvSetImageROI(frame_copy,FACE);
			cvResize(frame_copy,recog,CV_INTER_NN);
			cvResetImageROI(frame_copy);
			//cvShowImage("RecogData",recog);
			getData(recog,vec);
		}
		if(flag){
			cvRectangle(frame,
				cvPoint((int)face_leftup.x*SCALE0,(int)face_leftup.y*SCALE0),
				cvPoint((int)face_rightdown.x*SCALE0,(int)face_rightdown.y*SCALE0),
				CV_RGB(255, 0 ,255), 2, CV_AA);
		}
}

//�@�̒��S��T���܂��B
//�g���~���O���ꂽ��̈�̒l���w�K�f�[�^�Ɋ|���A�ǂ̋��t�f�[�^�Ɉ�ԋ߂������ʂ��܂�
void faceRecog(CvSeq* noseSeq,CvRect* faceRect,IplImage* tarImg,
			   double param1,double param2,double param3,double param4,
			   bool flag1, bool flag2
			   ){
				   //first search NOSE
				   int px = faceRect->x;
				   int py = faceRect->y;
				   int pw = faceRect->width;
				   int ph = faceRect->height;
#ifdef DIS
				   int number = noseSeq->total;
				   char window[] = "RecogData";
				   char name[256];
#endif	   
				   for (int j = 0; j < noseSeq->total; j++){
						CvRect* noseRect = (CvRect*)cvGetSeqElem(noseSeq, j);
						int cx;
						int cy;
						int eye_cx = (int)(reye_center.x + leye_center.x)/2-px;
						nose_leftup.x = noseRect->x;
						nose_leftup.y = noseRect->y;
						nose_rightdown.x = noseRect->x + noseRect->width;
						nose_rightdown.y = noseRect->y + noseRect->height;
						cx = (int)(nose_leftup.x + nose_rightdown.x)/2-px;
						cy = (int)(nose_leftup.y + nose_rightdown.y)/2-py;
						if(((int)eye_cx-pw*param1<cx)&(cx<(int)eye_cx+pw*param2)
						   &((int)ph*param3<cy)&(cy<(int)ph*param4)
							){
							nose_center.x = cx+px;
							nose_center.y = cy+py;
						}
						if(0 < nose_center.x && nose_center.x < width &&
						   0 < nose_center.y && nose_center.y < height && flag1
						  ){
							cvLine(tarImg,
								cvPoint((int)(nose_center.x-2)*SCALE0,(int)(nose_center.y-2)*SCALE0),
								cvPoint((int)(nose_center.x+2)*SCALE0,(int)(nose_center.y+2)*SCALE0),
								CV_RGB( 255 , 0 , 0));
							cvLine(tarImg,
								cvPoint((int)(nose_center.x-2)*SCALE0,(int)(nose_center.y+2)*SCALE0),
								cvPoint((int)(nose_center.x+2)*SCALE0,(int)(nose_center.y-2)*SCALE0),
								CV_RGB( 255 , 0 , 0));
						}

						getfaceROI(faceRect,flag2);
#ifdef DIS
						sprintf(name,"%s%01d",window,j);
						cvNamedWindow(name, CV_WINDOW_AUTOSIZE);
						cvShowImage(name,recog);
						//cvDestroyWindow(name);
#endif
						char str[64];
						specifyOne();

						switch(specifyOne()){
						case 0 :
							snprintf(str,64,"%s","none");
							cvPutText(frame,str,face_rightdown,
								&font,CV_RGB(255,255,0));
							break;
						case 1 :
							snprintf(str,64,"%s","aoi");
							cvPutText(frame,str,face_rightdown,
								&font,CV_RGB(255,255,0));
							break;
						case 2 :
							snprintf(str,64,"%s","kuro");
							cvPutText(frame,str,face_rightdown,
								&font,CV_RGB(255,255,0));
							break;
						case 3 :
							snprintf(str,64,"%s","liang");
							cvPutText(frame,str,face_rightdown,
								&font,CV_RGB(255,255,0));
							break;
						case 4 :
							snprintf(str,64,"%s","shell");
							cvPutText(frame,str,face_rightdown,
								&font,CV_RGB(255,255,0));
							break;
						case 5 :
							snprintf(str,64,"%s","ushi");
							cvPutText(frame,str,face_rightdown,
								&font,CV_RGB(255,255,0));
							break;
						}
					}
}

//�}�C�R�����ǂ�������ׂ����W��n���܂�
CvPoint getpresentNose(){

	CvPoint preNose;
	if ((nose_center.x!=0)&(nose_center.y!=0)){
		preNose = nose_center;
	}else{
		preNose = cvPoint((int)width/2,(int)height/2);
	} 
	return preNose; 
}

/*
//����T���܂�(�g���܂���)
void searchMOUTH(CvSeq* mouthSeq,CvRect* faceRect,IplImage* tarImg,
			   double param1,double param2,double param3,double param4,bool flag){
				   int px = faceRect->x;
				   int py = faceRect->y;
				   int pw = faceRect->width;
				   int ph = faceRect->height;

				   for (int j = 0; j < mouthSeq->total; j++){
						CvRect* mouthRect = (CvRect*)cvGetSeqElem(mouthSeq, j);
						int cx;
						int cy;
						mouth_leftup.x = mouthRect->x;
						mouth_leftup.y = mouthRect->y;
						mouth_rightdown.x = mouthRect->x + mouthRect->width;
						mouth_rightdown.y = mouthRect->y + mouthRect->height;
						cx = (int)(mouth_leftup.x + mouth_rightdown.x)/2-px;
						cy = (int)(mouth_leftup.y + mouth_rightdown.y)/2-py;
						if(((int)(nose_center.x-pw*param1)<cx)&(cx<(int)(nose_center.x+pw*param1))
						  &((int)(ph*param2)<cy)&(cy<(int)(ph*param3))&((int)(ph*param4)<(cy-nose_center.y))
						 ){
							mouth_center.x = cx+px;
							mouth_center.y = cy+py;
						}
						if(flag){
							cvLine(tarImg,
								cvPoint((int)(mouth_center.x-2)*SCALE0,(int)(mouth_center.y-2)*SCALE0),
								cvPoint((int)(mouth_center.x+2)*SCALE0,(int)(mouth_center.y+2)*SCALE0),
								CV_RGB( 0 , 0 , 255));
							cvLine(tarImg,
								cvPoint((int)(mouth_center.x-2)*SCALE0,(int)(mouth_center.y+2)*SCALE0),
								cvPoint((int)(mouth_center.x+2)*SCALE0,(int)(mouth_center.y-2)*SCALE0),
								CV_RGB( 0 , 0 , 255));
						}
					}
}
*/



int _tmain(int argc, _TCHAR* argv[])
{
	cvInitFont (&font, CV_FONT_HERSHEY_DUPLEX, 0.4, 0.4);
	for (int i = 0 ; i < FACE_SIZE*FACE_SIZE ; i++) vec[i] = 0;
	loadLearnData(vv);

	

	//���o��̓ǂݍ���
	CvHaarClassifierCascade* cvHCC = (CvHaarClassifierCascade*)cvLoad(FRONTALFACE);
	CvHaarClassifierCascade* cvHCC_EYE = (CvHaarClassifierCascade*)cvLoad(EYE);
	CvHaarClassifierCascade* cvHCC_NOSE = (CvHaarClassifierCascade*)cvLoad(NOSE);
	CvHaarClassifierCascade* cvHCC_MOUTH = (CvHaarClassifierCascade*)cvLoad(MOUTH);
	//���o�ɕK�v�ȃ������X�g���[�W��p�ӂ���
	CvMemStorage* cvMStr = cvCreateMemStorage(0);
	CvMemStorage* cvMStr_EYE = cvCreateMemStorage(0);
	CvMemStorage* cvMStr_NOSE = cvCreateMemStorage(0);
	CvMemStorage* cvMStr_MOUTH = cvCreateMemStorage(0);
	//���o�����󂯎�邽�߂̃V�[�P���X��p�ӂ���
	CvSeq* face;
	CvSeq* eye;
	CvSeq* nose;
	CvSeq* mouth;
	//0�ԍ��̃J�����ɑ΂���L���v�`���\���̂𐶐������
	capture = cvCreateCameraCapture (0);
	//�L���v�`���̃T�C�Y��ݒ肷��B�������A���̐ݒ�̓L���v�`�����s���J�����Ɉˑ�����̂Œ��ӂ�
	cvSetCaptureProperty (capture, CV_CAP_PROP_FRAME_WIDTH, width);
	cvSetCaptureProperty (capture, CV_CAP_PROP_FRAME_HEIGHT, height);
	cvNamedWindow("RecogFace", CV_WINDOW_AUTOSIZE);
	

	//��~�L�[���������܂ŃJ�����L���v�`���𑱂���
	while (1) {
		frame = cvQueryFrame (capture);
		//�F���p�C���[�W����
		frame_copy = cvCreateImage(cvSize(frame->width, frame->height), IPL_DEPTH_8U, 1);
		cvCvtColor(frame, frame_copy, CV_BGR2GRAY);
		recog = cvCreateImage (cvSize(FACE_SIZE,FACE_SIZE),frame_copy->depth,frame_copy->nChannels);
		cvZero(recog);
		if(!frame) break;
		//�ǂݍ��񂾉摜�̃O���[�X�P�[�����A�y�уq�X�g�O�����̋ψꉻ���s��
		IplImage* gray = cvCreateImage(cvSize(frame->width, frame->height), IPL_DEPTH_8U, 1);
		IplImage* detect_frame = cvCreateImage(cvSize((frame->width / SCALE0), (frame->height / SCALE0)), IPL_DEPTH_8U, 1);
		cvCvtColor(frame, gray, CV_BGR2GRAY);
		cvResize(gray, detect_frame, CV_INTER_LINEAR);
		cvEqualizeHist(detect_frame, detect_frame);
		//�摜�����猟�o�Ώۂ̏����擾����
		face = cvHaarDetectObjects(detect_frame,cvHCC,cvMStr,1.1,2,CV_HAAR_DO_CANNY_PRUNING,cvSize(30, 30));
		
		for (int i = 0; i < face->total; i++) {
			//���o��񂩂�ʒu�����擾
			CvRect* faceRect = (CvRect*)cvGetSeqElem(face, i);
			
			eye = cvHaarDetectObjects(detect_frame,cvHCC_EYE, cvMStr_EYE);
			nose = cvHaarDetectObjects(detect_frame,cvHCC_NOSE,cvMStr_NOSE);
			mouth = cvHaarDetectObjects(detect_frame,cvHCC_MOUTH,cvMStr_MOUTH);

			searchEYE(eye,faceRect,frame,0.0,0.425,0.12,0.4,0.575,1.0,0.12,0.4,1);
			faceRecog(nose,faceRect,frame,0.06,0.06,0.34,0.7,1,1);

			//searchMOUTH(mouth,faceRect,frame,0.08,0.7,1.0,0.16,1);
			
			
			//�擾������̈ʒu���Ɋ�Â��A��`�`����s��
			cvRectangle(frame,
				cvPoint(faceRect->x * SCALE0, faceRect->y * SCALE0),
				cvPoint((faceRect->x + faceRect->width) * SCALE0, (faceRect->y + faceRect->height) * SCALE0),
				CV_RGB(0, 255 ,0),2,CV_AA);

		}
		//snprintf(str,64,"%03d[frame]",num);
		//cvPutText(frame,str,cvPoint(10,15),&font,CV_RGB(255,0,0));
		/*
		snprintf(str,64,"L-EYE=(%d,%d)",leye_center.x,leye_center.y);
		cvPutText(frame,str,cvPoint(10,30),&font,CV_RGB(0,255,0));
		snprintf(str,64,"R-EYE=(%d,%d)",reye_center.x,reye_center.y);
		cvPutText(frame,str,cvPoint(10,40),&font,CV_RGB(0,255,0));
		snprintf(str,64,"NOSE=(%d,%d)",nose_center.x,nose_center.y);
		cvPutText(frame,str,cvPoint(10,50),&font,CV_RGB(0,255,0));
		snprintf(str,64,"MOUTH=(%d,%d)",mouth_center.x,mouth_center.y);
		cvPutText(frame,str,cvPoint(10,60),&font,CV_RGB(0,255,0));
		*/
		printf("%03d[frame]\n",num);
		printf("L-EYE=(%d,%d)\n",leye_center.x,leye_center.y);
		printf("R-EYE=(%d,%d)\n",reye_center.x,reye_center.y);
		printf("NOSE=(%d,%d)\n",nose_center.x,nose_center.y);
		//printf("vec[%d]= %d\n",FACE_SIZE*FACE_SIZE-1,vec[FACE_SIZE*FACE_SIZE-1]);
		//printf("MOUTH=(%d,%d)\n",mouth_center.x,mouth_center.y);
		printf("====================================\n");

		cvShowImage ("RecogFace", frame);
		

		num++;
		c = cvWaitKey (5);
		if (c == 27) {break;}
	}

	cvReleaseMemStorage(&cvMStr);
	cvReleaseMemStorage(&cvMStr_EYE);
	cvReleaseMemStorage(&cvMStr_NOSE);
	cvReleaseMemStorage(&cvMStr_MOUTH);
	cvReleaseCapture (&capture);
	
	cvDestroyWindow("RecogFace");

	cvReleaseHaarClassifierCascade(&cvHCC);
	cvReleaseHaarClassifierCascade(&cvHCC_EYE);
	cvReleaseHaarClassifierCascade(&cvHCC_NOSE);
	cvReleaseHaarClassifierCascade(&cvHCC_MOUTH);

	return 0;
}

