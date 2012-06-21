// FaceRecognition4.cpp : �R���\�[�� �A�v���P�[�V�����̃G���g�� �|�C���g���`���܂��B

#include "opencv\cv.h"
#include "opencv\cvaux.h"
#include "opencv\cvwimage.h"
#include "opencv\cxcore.h"
#include "opencv\cxmisc.h"
#include "opencv\highgui.h"
#include "opencv\ml.h"
#include <time.h>
#include <fstream>
#include <string>
#include <iostream>
#include <vector>

//�v���W�F�N�g�̃v���p�e�B��C/C++�ˑS�ʁ@�̒ǉ��̃C���N���[�h�f�B���N�g����
// opencv2�̂���t�H���_�wC:\OpenCV\include�x�Ȃǂ�ǉ��̂���
#include "opencv2\opencv.hpp"
 

/**********************************************************************************************\
 FaceRecognition4.cpp �ɂ���
 Matlab �ɂ�萶������ data.txt (�e�摜(MEN�~SAMPLE��)�� n �����̎听���Ƃ��̃��x��)�ɑ΂���
 SVM ��蒴���ʂ��v�Z����


\**********************************************************************************************/

#define MEN      5 //���ʂ���l��
#define SAMPLE  51 //�e�l�ɑ΂���T���v���̐�
#define SIZE    32 //���摜�̃T�C�Y
#define DIM      20 //�听���̎���

/*
CvMat* getData(CvMat* data,CvMat* res){

}
*/
int readData(char* fname,float data[],int res[],int dateSize){
	std::ifstream ifs( fname );
	for (int i=0;i<dateSize;i++){
		float a[MEN];
		for(int j=0;j<DIM;j++){
			ifs>> data[DIM*i+j];
		}
		for(int j=0;j<MEN;j++){
			ifs>>a[j];
		}
		for(int j=0;j<MEN;j++){
			if(a[j]>0){
				res[i]=j+1;
				break;
			}
		}
		std::cout<<data[DIM*i]<<" "<<data[DIM*i+1]<<" "<<data[DIM*i+2]<<" "<<res[i]<<std::endl;
	}
	return 0;
}

int readTestData(char* fname,float data[],int dateSize){
	std::ifstream ifs( fname );
	for (int i=0;i<dateSize;i++){
		float a[MEN];
		for(int j=0;j<DIM;j++){
			ifs>> data[DIM*i+j];
		}
	}
	return 0;
}

int main(int argc, char* argv[])
{
  int i, j, sv_num;
  IplImage *img;
  CvSVM svm = CvSVM ();
  CvSVMParams param;
  CvTermCriteria criteria;
  CvRNG rng = cvRNG (time (NULL));
  //CvPoint pts[s];
  //float data[s * 2];
  //int res[s];
  CvMat data_mat, res_mat;
  //CvScalar rcolor;
  const float *support;

  const int size=MEN*SAMPLE;
  float data[size*DIM];
  int res[size];



  /*
  // (1)�摜�̈�̊m�ۂƏ�����

  // (2)�w�K�f�[�^�̐���
  
  
  // (3)�w�K�f�[�^�̕\��
  */

  // (4)�w�K�p�����[�^�̐���
  readData("data_dim20.txt",data,res,size);
  
  //float norm=1;
  float norm=size*DIM;
  for (int i = 0 ; i < size*DIM ; i++) data[i] = data[i]/(norm);


  cvInitMatHeader (&data_mat, size, DIM, CV_32FC1, data);
  cvInitMatHeader (&res_mat, size, 1, CV_32SC1, res);
  criteria = cvTermCriteria (CV_TERMCRIT_EPS, 1000, FLT_EPSILON);
  param = CvSVMParams (CvSVM::C_SVC, CvSVM::RBF, 10.0, 8.0, 1.0, 10.0, 0.5, 0.1, NULL, criteria);

  // (5)SVM�̊w�K
  svm.train (&data_mat, &res_mat, NULL, NULL, param);

  
  // (7)�g���[�j���O�f�[�^�̍ĕ`��
  /*
  for (i = 0; i < s; i++) {
    CvScalar rcolor;
    switch (res[i]) {
    case 1:
      rcolor = CV_RGB (255, 0, 0);
      break;
    case 2:
      rcolor = CV_RGB (0, 255, 0);
      break;
    case 3:
      rcolor = CV_RGB (0, 0, 255);
      break;
    }
    cvLine (img, cvPoint (pts[i].x - 2, pts[i].y - 2), cvPoint (pts[i].x + 2, pts[i].y + 2), rcolor);
    cvLine (img, cvPoint (pts[i].x + 2, pts[i].y - 2), cvPoint (pts[i].x - 2, pts[i].y + 2), rcolor);
  }
  */
  // (8)�T�|�[�g�x�N�^�[�̕`��
  FILE* fp;
  char dataname[] = "support.txt";
  int count;
  if ((fp = fopen(dataname, "w")) == NULL) {
        fprintf(stderr, "�t�@�C���̃I�[�v���Ɏ��s���܂����D\n");
        return EXIT_FAILURE;
  }
  sv_num = svm.get_support_vector_count ();
  printf("sv_num = %d\n",sv_num);
  for (i = 0; i < sv_num; i++) {
    support = svm.get_support_vector (i);
	printf("support[%d]=(%5f,%5f,%5f)\n",i,support[0],support[1],support[2]);
    //cvCircle (img, cvPoint ((int) (support[0] * size), (int) (support[1] * size)), 5, CV_RGB (200, 200, 200));
	for (int j = 0 ; j < DIM ; j++){
		  count = fprintf(fp,"%5f ",support[j]);
	  }
	count = fprintf(fp,"\n");
  }
  if ( count < 0 ) {
        fprintf(stderr, "�t�@�C���̏����݂Ɏ��s���܂���.\n");
        fclose(fp);
        return EXIT_FAILURE;
  }
  fprintf(stdout, "%s��%d�����o�͂��܂����D\n", dataname, count);
  fclose(fp);

  // (6)�w�K���ʂ̕`��
  
  //���t�f�[�^�Ńe�X�g
  cout<<"test to teacher data"<<endl;
  for (i = 0; i < size; i++) {
      CvMat m;
	  
      float ret = 0.0;
      cvInitMatHeader (&m, 1, DIM, CV_32FC1, data+i*DIM);
      ret = svm.predict (&m);
	  std::cout<<"idx "<<i<<"'s data is "<<ret<<endl;
  }
  //�e�X�g�f�[�^�ł̃e�X�g
  cout<<"test to test data"<<endl;
  const int testDataSize=13;
  float testData[testDataSize*DIM];
  readTestData("test_data_20.txt",testData,testDataSize);
  for (i = 0; i < testDataSize; i++) {
      CvMat m;
	  
	  /*for(j=0;j<DIM;j++){
		  testData[i*DIM+j]/=(testDataSize*DIM);
	  }*/
      float ret = 0.0;
      cvInitMatHeader (&m, 1, DIM, CV_32FC1, testData+i*DIM);
      ret = svm.predict (&m);
	  std::cout<<"idx "<<i<<"'s data is "<<ret<<endl;
  }



  int c;
  cin>>c;
  return 0;
}

