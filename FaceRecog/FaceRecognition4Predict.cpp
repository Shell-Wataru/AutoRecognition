// FaceRecognition4.cpp : コンソール アプリケーションのエントリ ポイントを定義します。

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

//プロジェクトのプロパティ⇒C/C++⇒全般　の追加のインクルードディレクトリに
// opencv2のあるフォルダ『C:\OpenCV\include』などを追加のこと
#include "opencv2\opencv.hpp"
 

/**********************************************************************************************\
 FaceRecognition4.cpp について
 Matlab により生成した data.txt (各画像(MEN×SAMPLE枚)の n 次元の主成分とそのラベル)に対して
 SVM より超平面を計算する


\**********************************************************************************************/

#define MEN      5 //判別する人数
#define SAMPLE  51 //各人に対するサンプルの数
#define SIZE    32 //元画像のサイズ
#define DIM      20 //主成分の次元

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
  // (1)画像領域の確保と初期化

  // (2)学習データの生成
  
  
  // (3)学習データの表示
  */

  // (4)学習パラメータの生成
  readData("data_dim20.txt",data,res,size);
  
  //float norm=1;
  float norm=size*DIM;
  for (int i = 0 ; i < size*DIM ; i++) data[i] = data[i]/(norm);


  cvInitMatHeader (&data_mat, size, DIM, CV_32FC1, data);
  cvInitMatHeader (&res_mat, size, 1, CV_32SC1, res);
  criteria = cvTermCriteria (CV_TERMCRIT_EPS, 1000, FLT_EPSILON);
  param = CvSVMParams (CvSVM::C_SVC, CvSVM::RBF, 10.0, 8.0, 1.0, 10.0, 0.5, 0.1, NULL, criteria);

  // (5)SVMの学習
  svm.train (&data_mat, &res_mat, NULL, NULL, param);

  
  // (7)トレーニングデータの再描画
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
  // (8)サポートベクターの描画
  FILE* fp;
  char dataname[] = "support.txt";
  int count;
  if ((fp = fopen(dataname, "w")) == NULL) {
        fprintf(stderr, "ファイルのオープンに失敗しました．\n");
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
        fprintf(stderr, "ファイルの書込みに失敗しました.\n");
        fclose(fp);
        return EXIT_FAILURE;
  }
  fprintf(stdout, "%sへ%d文字出力しました．\n", dataname, count);
  fclose(fp);

  // (6)学習結果の描画
  
  //教師データでテスト
  cout<<"test to teacher data"<<endl;
  for (i = 0; i < size; i++) {
      CvMat m;
	  
      float ret = 0.0;
      cvInitMatHeader (&m, 1, DIM, CV_32FC1, data+i*DIM);
      ret = svm.predict (&m);
	  std::cout<<"idx "<<i<<"'s data is "<<ret<<endl;
  }
  //テストデータでのテスト
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

