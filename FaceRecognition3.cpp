// FaceRecognition3.cpp : �R���\�[�� �A�v���P�[�V�����̃G���g�� �|�C���g���`���܂��B
//

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
 FaceRecognition3.cpp �ɂ���

 FaceRecognition2.cpp �ŕۑ����ꂽ��摜���@FACE_SIZE �~ FASE_SIZE �̃T�C�Y�Ƀ��T�C�Y���A
 ���̃O���[�X�P�[���摜�̊e�s�N�Z���f�[�^�y�іځA�@�A���Ԃ̋�����txt�ɂ��ďo�͂��܂��B
 (���̔F�������Ȃ舫���̂Ŏg�����̂ɂȂ�Ȃ���������Ȃ�)

 DATA_DIR �ŉ摜������f�B���N�g�����w�肵(����faces\\�`�@�̂��Ƃ̃A�h���X�ɒ���)�AOUTPUT_FILE�Ŏw�肳�ꂽ
 ���O�̃t�@�C��(�\�ߗp�ӂ��Ă�������)�ɏ㏑�����܂��B(DATA_DIR�ɓ����Ă���摜�̖�����SAMPLE�̒l�ƈقȂ�Ɠ����Ȃ�
 �̂Œ���)

\**********************************************************************************************/


#define FACE_SIZE				 32		
#define SAMPLE				     51		//�T���v����
#define DATA_DIR        "C:\\Users\\t2ladmin\\Documents\\Visual Studio 2010\\Projects\\FaceRecognition3\\FaceRecognition3\\faces\\ushi50\\ushi"
#define OUTPUT_FILE		"analysis.txt"
#define MARGIN_X                 1/10	//�T���v���摜�����菜������
#define MARGIN_Y                 1/10	//�T���v���摜�����菜���c��
#define SCALE                     5     //�튯���o�̂��߂̃T���v���摜�g��{��
#define LEFT_EYE        "C:\\opencv\\data\\haarcascades\\haarcascade_mcs_lefteye.xml"
#define RIGHT_EYE       "C:\\opencv\\data\\haarcascades\\haarcascade_mcs_righteye.xml"
#define EYE             "C:\\opencv\\data\\haarcascades\\haarcascade_eye.xml"
#define EYE_PAIR_B      "C:\\opencv\\data\\haarcascades\\haarcascade_mcs_eyepair_big.xml"
#define EYE_PAIR_S      "C:\\opencv\\data\\haarcascades\\haarcascade_mcs_eyepair_small.xml"
#define NOSE            "C:\\opencv\\data\\haarcascades\\haarcascade_mcs_nose.xml"
#define MOUTH           "C:\\opencv\\data\\haarcascades\\haarcascade_mcs_mouth.xml"		//�����Ȃ�F����������

//�B�e�����摜��C�ӂ̃T�C�Y�Ƀ��T�C�Y���郁�\�b�h
//��{�I�ɂ� FACE_SIZE�~FACE_SIZE ��z��
IplImage* resize(IplImage* img,int height,int width){
	int x = img->height;
	int y = img->width;
	CvRect rect = cvRect((int)(x*MARGIN_X),(int)(y*MARGIN_Y),(int)(x*(1-2*MARGIN_X)),(int)(y*(1-2*MARGIN_Y)));
	cvSetImageROI(img,rect);
	//IplImage* small_img = cvCreateImage(cvGetSize(img), IPL_DEPTH_8U, 3);
	//cvCopy(img,small_img);
	IplImage* resized_img = cvCreateImage (cvSize(height,width),img->depth,img->nChannels);
	cvResize(img,resized_img,CV_INTER_NN);
	return resized_img;
}

//�O���[�X�P�[���̉摜����l�����o�����\�b�h
void getData(IplImage* img,uchar data[FACE_SIZE][FACE_SIZE]){
	for (int i=0; i < img->height; i++){
		for (int j=0; j < img->width; j++){
			//�l�̎��o��
			data[i][j] = img->imageData[img->widthStep*i+j];
			//�l�̏�������
			//img->imageData[img->widthStep * y + x]=�i0�`255�̒l�j;
			//printf("data[%d][%d]= %d ",i,j,data[i][j]);
		}
	}
}

//���T�C�Y�����摜����l�����o��
void getImgValue(IplImage* img,uchar data[FACE_SIZE][FACE_SIZE]){

	img = resize(img,FACE_SIZE,FACE_SIZE);
	getData(img,data);
}

//2�_�Ԃ̃��[�N���b�h�������v�Z���郁�\�b�h
int getDist(CvPoint a, CvPoint b){

	double dist = 0.0;
	int dist2 = (a.x - b.x)*(a.x - b.x)+(a.y - b.y)*(a.y - b.y);
	dist = (int)sqrtf(dist2);
	return dist;
}



int _tmain(int argc, _TCHAR* argv[])
{
	const CvArr* imgs[SAMPLE];			//�\���e�X�g�p(�J���[)
	const CvArr* imgs2[SAMPLE];			//�ǂݎ��p(����)
	
	int file_num = 0; 
	char dir[] = DATA_DIR;
	char extension[] = ".bmp";
	char file_dir[256];
	//CvPoint reye_leftup[SAMPLE];		//�E�ڍ���̍��W
	//CvPoint reye_rightdown[SAMPLE];	//�E�ډE���̍��W
	//CvPoint leye_leftup[SAMPLE];		//���ڍ���̍��W
	//CvPoint leye_rightdown[SAMPLE];	//���ډE���̍��W
	CvPoint eye_leftup[SAMPLE];
	CvPoint eye_rightdown[SAMPLE];
	CvPoint nose_leftup[SAMPLE];		//�@����̍��W
	CvPoint nose_rightdown[SAMPLE];	//�@�E���̍��W
	CvPoint mouth_leftup[SAMPLE];		//������̍��W
	CvPoint mouth_rightdown[SAMPLE];	//���E���̍��W
	//CvPoint reye_center[SAMPLE];		//�E�ڂ̒��S���W	
	//CvPoint leye_center[SAMPLE];		//���ڂ̒��S���W
	CvPoint eye_center[SAMPLE];
	CvPoint nose_center[SAMPLE];		//�@�̒��S���W
	CvPoint mouth_center[SAMPLE];		//���̒��S���W
	int dist_LE_to_RE[SAMPLE];			//���ڂƉE�ڂ̋���
	int dist_LE_to_NO[SAMPLE];			//���ڂƕ@�̋���
	int dist_RE_to_NO[SAMPLE];			//�E�ڂƕ@�̋���
	int dist_RE_to_MO[SAMPLE];			//�E�ڂƌ��̋���
	int dist_LE_to_MO[SAMPLE];			//���ڂƌ��̋���
	int dist_NO_to_MO[SAMPLE];			//�@�ƌ��̋���

	for (int i = 0 ; i < SAMPLE ; i++){
	
		dist_LE_to_RE[i] = -1;
		dist_LE_to_NO[i] = -2;
		dist_RE_to_NO[i] = -3;
		dist_RE_to_MO[i] = -4;
		dist_LE_to_MO[i] = -5;
		dist_NO_to_MO[i] = -6;
	}

	for (int i = 0 ; i < SAMPLE ; i++){
		sprintf(file_dir,"%s%04d%s",dir,file_num,extension);
		puts(file_dir);
		imgs[i] = cvLoadImage(file_dir,CV_LOAD_IMAGE_COLOR);
		imgs2[i] = cvLoadImage(file_dir,CV_LOAD_IMAGE_GRAYSCALE);
		imgs[i] = resize((IplImage*) imgs[i],FACE_SIZE*SCALE,FACE_SIZE*SCALE);
		imgs2[i] = resize((IplImage*) imgs2[i],FACE_SIZE,FACE_SIZE);
		file_num++;
	}
	// ���o��̓ǂݍ���
	CvHaarClassifierCascade* cvHCC_EYE = (CvHaarClassifierCascade*)cvLoad(EYE);
	CvHaarClassifierCascade* cvHCC_NOSE = (CvHaarClassifierCascade*)cvLoad(NOSE);
	CvHaarClassifierCascade* cvHCC_MOUTH = (CvHaarClassifierCascade*)cvLoad(MOUTH);

	// ���o�ɕK�v�ȃ������X�g���[�W��p�ӂ���
	CvMemStorage* cvMStr_EYE = cvCreateMemStorage(0);
	CvMemStorage* cvMStr_NOSE = cvCreateMemStorage(0);
	CvMemStorage* cvMStr_MOUTH = cvCreateMemStorage(0);

	// ���o�����󂯎�邽�߂̃V�[�P���X��p�ӂ���
	CvSeq* eye;
	CvSeq* nose;
	CvSeq* mouth;

	for (int j = 0; j < 8 ; j++){
		// �摜�����猟�o�Ώۂ̏����擾����
		eye = cvHaarDetectObjects((IplImage*)imgs[j], cvHCC_EYE, cvMStr_EYE);
		nose = cvHaarDetectObjects((IplImage*)imgs[j], cvHCC_NOSE, cvMStr_NOSE);
		mouth = cvHaarDetectObjects((IplImage*)imgs[j], cvHCC_MOUTH, cvMStr_MOUTH);



		for (int i = 0; i < eye->total; i++) {
		//for (int i = 0; i < 2; i++) {
			//���o��񂩂�ڂ̈ʒu�����擾
			CvRect* eyeRect = (CvRect*)cvGetSeqElem(eye, i);
			// �擾�����ڂ̈ʒu���Ɋ�Â��A��`�`����s��
			cvRectangle((IplImage*)imgs[j],
				cvPoint(eyeRect->x, eyeRect->y),
				cvPoint(eyeRect->x + eyeRect->width, eyeRect->y + eyeRect->height),
				CV_RGB(0, 255 ,0), 
				1, CV_AA);
			printf("EYE::GREEN\n");
			printf("(%d,%d)\n",eyeRect->x,eyeRect->y);
			printf("width = %d , height = %d \n",eyeRect->width,eyeRect->height);
			
			//////////////////////////////////////////////////////////////
			//���o�����ڂ̍��W��reye_leftup�Ȃǂɓ����
			//SCALE�Ŋ��邱�Ƃ�Y�ꂸ��
			
			int center_cx;
			int center_cy;
			eye_leftup[j].x = eyeRect->x;
			eye_leftup[j].y = eyeRect->y;
			eye_rightdown[j].x = eyeRect->x + eyeRect->width;
			eye_rightdown[j].y = eyeRect->y + eyeRect->height;
			center_cx = (int)(eye_leftup[j].x + eye_rightdown[j].x)/2;
			center_cy = (int)(eye_leftup[j].y + eye_rightdown[j].y)/2;
			if(((int)(FACE_SIZE*SCALE*0/4) < center_cx)&(center_cx < (int)(FACE_SIZE*SCALE*3/4))
				&((int)(FACE_SIZE*SCALE*0/5) < center_cy)&(center_cy < (int)(FACE_SIZE*SCALE*2/5))
				){
				eye_center[j].x = center_cx;
				eye_center[j].y = center_cy;
			}
			//////////////////////////////////////////////////////////////
			//printf("mouth_candidate[%d][%d] = ( %d , %d )\n",j,i,mouth_center[j].x,mouth_center[j].y);
			cvLine((IplImage*)imgs[j],
				cvPoint(eye_center[j].x-2,eye_center[j].y-2),
				cvPoint(eye_center[j].x+2,eye_center[j].y+2),
				CV_RGB( 0 , 255 , 0));
			cvLine((IplImage*)imgs[j],
				cvPoint(eye_center[j].x-2,eye_center[j].y+2),
				cvPoint(eye_center[j].x+2,eye_center[j].y-2),
				CV_RGB( 0 , 255 , 0));

			//////////////////////////////////////////////////////////////
			//printf("leye_candidate[%d][%d] = ( %d , %d )\n",j,i,leye_center[j].x,leye_center[j].y);
			//printf("reye_candidate[%d][%d] = ( %d , %d )\n",j,i,reye_center[j].x,reye_center[j].y);
		}

		for (int i = 0; i < nose->total; i++) {
		//for (int i = 0; i < 1; i++) {
			CvRect* noseRect = (CvRect*)cvGetSeqElem(nose, i);
			cvRectangle((IplImage*)imgs[j],
				cvPoint(noseRect->x, noseRect->y),
				cvPoint(noseRect->x + noseRect->width, noseRect->y + noseRect->height),
				CV_RGB(255, 0 ,0), 
				1, CV_AA);
			printf("NOSE::RED\n");
			printf("(%d,%d)\n",noseRect->x,noseRect->y);
			printf("width = %d , height = %d \n",noseRect->width,noseRect->height);
			
			//////////////////////////////////////////////////////////////
			int center_cx;
			int center_cy;
			nose_leftup[j].x = noseRect->x;
			nose_leftup[j].y = noseRect->y;
			nose_rightdown[j].x = noseRect->x + noseRect->width;
			nose_rightdown[j].y = noseRect->y + noseRect->height;
			center_cx = (int)(nose_leftup[j].x + nose_rightdown[j].x)/2;
			center_cy = (int)(nose_leftup[j].y + nose_rightdown[j].y)/2;
			if(((int)FACE_SIZE*SCALE*(1.7)/5 < center_cx)&(center_cx <  (int)FACE_SIZE*SCALE*(3.3)/5)
				&((int)FACE_SIZE*SCALE*(1.7)/5 < center_cy)&(center_cy < (int)FACE_SIZE*SCALE*(3.3)/5)){
				nose_center[j].x = center_cx;
				nose_center[j].y = center_cy;
			}
			//////////////////////////////////////////////////////////////
			//printf("nose_candidate[%d][%d] = ( %d , %d )\n",j,i,nose_center[j].x,nose_center[j].y);
			cvLine((IplImage*)imgs[j],
				cvPoint(nose_center[j].x-2,nose_center[j].y-2),
				cvPoint(nose_center[j].x+2,nose_center[j].y+2),
				CV_RGB(255, 0 ,0));
			cvLine((IplImage*)imgs[j],
				cvPoint(nose_center[j].x-2,nose_center[j].y+2),
				cvPoint(nose_center[j].x+2,nose_center[j].y-2),
				CV_RGB(255, 0 ,0));
			
		}

		for (int i = 0; i < mouth->total; i++) {
			CvRect* mouthRect = (CvRect*)cvGetSeqElem(mouth, i);
			cvRectangle((IplImage*)imgs[j],
				cvPoint(mouthRect->x, mouthRect->y),
				cvPoint(mouthRect->x + mouthRect->width, mouthRect->y + mouthRect->height),
				CV_RGB(0, 0 ,255), 
				1, CV_AA);
			printf("MOUTH::BLUE\n");
			printf("(%d,%d)\n",mouthRect->x,mouthRect->y);
			printf("width = %d , height = %d \n",mouthRect->width,mouthRect->height);
			//////////////////////////////////////////////////////////////
			int center_cx;
			int center_cy;
			mouth_leftup[j].x = mouthRect->x;
			mouth_leftup[j].y = mouthRect->y;
			mouth_rightdown[j].x = mouthRect->x + mouthRect->width;
			mouth_rightdown[j].y = mouthRect->y + mouthRect->height;
			center_cx = (int)(mouth_leftup[j].x + mouth_rightdown[j].x)/2;
			center_cy = (int)(mouth_leftup[j].y + mouth_rightdown[j].y)/2;
			if(((int)(nose_center[j].x-FACE_SIZE/4) < center_cx)&(center_cx < (int)(nose_center[j].x+FACE_SIZE/4))
				&((int)(FACE_SIZE*SCALE*3/5) < center_cy)&(center_cy < (int)(FACE_SIZE*SCALE*5/5))
				){
				mouth_center[j].x = center_cx;
				mouth_center[j].y = center_cy;
			}
			//////////////////////////////////////////////////////////////
			//printf("mouth_candidate[%d][%d] = ( %d , %d )\n",j,i,mouth_center[j].x,mouth_center[j].y);
			cvLine((IplImage*)imgs[j],
				cvPoint(mouth_center[j].x-2,mouth_center[j].y-2),
				cvPoint(mouth_center[j].x+2,mouth_center[j].y+2),
				CV_RGB( 0 , 0 ,255));
			cvLine((IplImage*)imgs[j],
				cvPoint(mouth_center[j].x-2,mouth_center[j].y+2),
				cvPoint(mouth_center[j].x+2,mouth_center[j].y-2),
				CV_RGB( 0 , 0 ,255));
		}
		//printf("leye_center[%d] = ( %d , %d )\n",j,leye_center[j].x,leye_center[j].y);
		//printf("reye_center[%d] = ( %d , %d )\n",j,reye_center[j].x,reye_center[j].y);
		printf("nose_center[%d] = ( %d , %d )\n",j,nose_center[j].x,nose_center[j].y);
		printf("mouth_center[%d] = ( %d , %d )\n",j,mouth_center[j].x,mouth_center[j].y);
		//getDist(CvPoint a, CvPoint b)
		//dist_LE_to_RE[j] = getDist(leye_center[j],reye_center[j]);
		//dist_LE_to_NO[j] = getDist(leye_center[j],nose_center[j]);
		//dist_RE_to_NO[j] = getDist(reye_center[j],nose_center[j]);
		//dist_RE_to_MO[j] = getDist(reye_center[j],mouth_center[j]);
		//dist_LE_to_MO[j] = getDist(leye_center[j],mouth_center[j]);
		//dist_NO_to_MO[j] = getDist(nose_center[j],mouth_center[j]);

		char window[] = "detect";
		char name[256];
		printf("=============================================\n");
		sprintf(name,"%s%03d",window,j);
		puts(name);
		printf("=============================================\n");
		cvNamedWindow(name);
		cvShowImage(name, (IplImage*)imgs[j]);
		
	}
	cvWaitKey(0);

	


	
	/*
	cvNamedWindow("gray", CV_WINDOW_AUTOSIZE);
	cvShowImage("gray", imgs[0]);
	cvWaitKey(0);
	//cvReleaseImage(&(IplImage*)imgs[0]);
	cvDestroyWindow("gray");
	*/
	
	uchar p[SAMPLE][FACE_SIZE][FACE_SIZE];
	//�s�N�Z���l���i�[����z��̏�����
	for (int i = 0 ; i < FACE_SIZE ; i++){
		for (int j = 0 ; j < FACE_SIZE ; j++){
			for (int k = 0 ; k < SAMPLE ; k++){
				p[k][i][j] = 0;
			}
		}
	}
	//�e�z��ɉ摜�̒l���i�[
	for (int i = 0 ; i < SAMPLE ; i++ ){
		getImgValue((IplImage*)imgs2[i],p[i]);
	}

	uchar data[SAMPLE][FACE_SIZE*FACE_SIZE];
	for (int i = 0 ; i < SAMPLE ; i++){
		for (int j = 0 ; j < FACE_SIZE ; j++){
			for (int k = 0 ; k < FACE_SIZE ; k++){
				data[i][j*FACE_SIZE + k] = p[i][j][k] ;
			}
		}
	}
	
	//filename�ɋL���ꂽ�l�Ɋւ���SAMPLE���̃f�[�^�T���v���̍s��data[][]��txt�t�@�C���ɏ����o��
	FILE *fp;
    char dataname[] = OUTPUT_FILE;
    int count = 0;
    // �t�@�C���I�[�v��
    if ((fp = fopen(dataname, "w")) == NULL) {
        fprintf(stderr, "�t�@�C���̃I�[�v���Ɏ��s���܂����D\n");
        return EXIT_FAILURE;
    }
    // �������� 
	
    for ( int i = 0 ; i < SAMPLE ; i ++ ){
		for ( int j = 0 ; j < FACE_SIZE*FACE_SIZE ; j++){
			count = fprintf(fp, "%d ",data[i][j]);
		}
		count = fprintf(fp, "%d ",dist_LE_to_RE[i]);
		count = fprintf(fp, "%d ",dist_LE_to_NO[i]);
		count = fprintf(fp, "%d ",dist_RE_to_NO[i]);
		count = fprintf(fp, "%d ",dist_RE_to_MO[i]);
		count = fprintf(fp, "%d ",dist_LE_to_MO[i]);
		count = fprintf(fp, "%d ",dist_NO_to_MO[i]);
		count = fprintf(fp, "\n");
	}
	
    if ( count < 0 ) {
        fprintf(stderr, "�t�@�C���̏����݂Ɏ��s���܂���.\n");
        fclose(fp);
        return EXIT_FAILURE;
    }
    fprintf(stdout, "%s��%d�����o�͂��܂����D\n", dataname, count);

    // �t�@�C���N���[�Y 
    fclose(fp);
    return EXIT_SUCCESS;
	
	return 0;
}

