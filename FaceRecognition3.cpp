// FaceRecognition3.cpp : �R���\�[�� �A�v���P�[�V�����̃G���g�� �|�C���g���`���܂��B
//

#include "stdafx.h"
#include "stdio.h"
#include "ctype.h"
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
 ���̃O���[�X�P�[���摜�̊e�s�N�Z���f�[�^��txt�ɂ��ďo�͂��܂��B

 DATA_DIR �ŉ摜������f�B���N�g�����w�肵(����faces\\�`�@�̂��Ƃ̃A�h���X�ɒ���)�AOUTPUT_FILE�Ŏw�肳�ꂽ
 ���O�̃t�@�C��(�\�ߗp�ӂ��Ă�������)�ɏ㏑�����܂��B(DATA_DIR�ɓ����Ă���摜�̖�����SAMPLE�̒l�ƈقȂ�Ɠ����܂���
 �̂Œ���)

\**********************************************************************************************/


#define FACE_SIZE				 64
#define SAMPLE				     51
#define DATA_DIR        "C:\\Users\\t2ladmin\\Documents\\Visual Studio 2010\\Projects\\FaceRecognition3\\FaceRecognition3\\faces\\ushi50\\ushi"
#define OUTPUT_FILE		"ushi.txt"


//�B�e�����摜��C�ӂ̃T�C�Y�Ƀ��T�C�Y���郁�\�b�h
//��{�I�ɂ� FACE_SIZE�~FACE_SIZE ��z��
IplImage* resize(IplImage* img,int height,int width){
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


int _tmain(int argc, _TCHAR* argv[])
{
	const CvArr* imgs[SAMPLE];
	
	int file_num = 0; 
	char dir[] = DATA_DIR;
	char extension[] = ".bmp";
	char file_dir[256];

	for (int i = 0 ; i < SAMPLE ; i++){
		sprintf(file_dir,"%s%04d%s",dir,file_num,extension);
		puts(file_dir);
		imgs[i] = cvLoadImage(file_dir,CV_LOAD_IMAGE_GRAYSCALE);
		file_num++;
	}

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
		getImgValue((IplImage*)imgs[i],p[i]);
	}

	uchar data[SAMPLE][FACE_SIZE*FACE_SIZE];

	for (int i = 0 ; i < SAMPLE ; i++){
		for (int j = 0 ; j < FACE_SIZE ; j++){
			for (int k = 0 ; k < FACE_SIZE ; k++){
				data[i][j*FACE_SIZE + k] = p[i][j][k] ;
			}
		}
	}

	//cvCalcCovarMatrix(vects,SAMPLE,cov_mat,avg,CV_COVAR_NORMAL|CV_COVAR_SCALE);
	
	/*
	for (int i=0; i < FACE_SIZE; i++){
		for (int j=0; j < FACE_SIZE; j++){
			  printf("p[0][%d][%d]= %d ",i,j,p[0][i][j]);
		}
	}
	*/
	/*
	for (int i = 0; i < FACE_SIZE*FACE_SIZE ; i++ ){
		printf("data[0][%d]= %d ",i,data[0][i]);
	}
	*/

	//filename�ɋL���ꂽ�l�Ɋւ���SAMPLE���̃f�[�^�T���v���̍s��data[][]��txt�t�@�C���ɏ����o��
	FILE *fp;
    char dataname[] = OUTPUT_FILE;
    int count;

    /* �t�@�C���I�[�v�� */
    if ((fp = fopen(dataname, "w")) == NULL) {
        fprintf(stderr, "�t�@�C���̃I�[�v���Ɏ��s���܂����D\n");
        return EXIT_FAILURE;
    }

    /* �������� */
    for ( int i = 0 ; i < SAMPLE ; i ++ ){
		for ( int j = 0 ; j < FACE_SIZE*FACE_SIZE ; j++){
			count = fprintf(fp, "%d ",data[i][j]);
		}
		count = fprintf(fp, "\n");
	}

    if ( count < 0 ) {
        fprintf(stderr, "�t�@�C���̏����݂Ɏ��s���܂���.\n");
        fclose(fp);
        return EXIT_FAILURE;
    }

    fprintf(stdout, "%s��%d�����o�͂��܂����D\n", dataname, count);

    /* �t�@�C���N���[�Y */
    fclose(fp);

    return EXIT_SUCCESS;


	//cvNamedWindow("gray", CV_WINDOW_AUTOSIZE);
	//cvShowImage("gray", imgs[0]);
	//cvWaitKey(0);
	//cvReleaseImage(&imgs[0]);
	//cvDestroyWindow("gray");

	return 0;
}

