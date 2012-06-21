// FaceRecognition4.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
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
 
#ifdef _DEBUG
    //Debugモードの場合
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
    #pragma comment(lib,"C:\\opencv\\build\\x86\\vc10\\lib\\opencv_ml231d.lib")
#else
    //Releaseモードの場合
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
 FaceRecognition4.cpp について
 Matlab により生成した data.txt (各画像(MEN×SAMPLE枚)の n 次元の主成分とそのラベル)に対して
 SVM より超平面を計算する


\**********************************************************************************************/

#define MEN      5 //判別する人数
#define SAMPLE  51 //各人に対するサンプルの数
#define SIZE    32 //元画像のサイズ
#define DIM      3 //主成分の次元

/*
CvMat* getData(CvMat* data,CvMat* res){

}
*/
int readData(float data[],int res[],int dateSize){
	std::ifstream ifs( "data.txt" );
	for (int i=0;i<dateSize;i++){
		float a[MEN];
		ifs>> data[DIM*i]>>data[DIM*i+1]>>data[DIM*i+2]
		>>a[0]>>a[1]>>a[2]>>a[3]>>a[4];
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



int _tmain(int argc, _TCHAR* argv[])
{
  //const int s = 1000;
  //int size = 400;
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
  readData(data,res,size);
  for (int i = 0 ; i < size*DIM ; i++) data[i] = data[i]/(size*DIM);


  /*
  // (1)画像領域の確保と初期化
  img = cvCreateImage (cvSize (size, size), IPL_DEPTH_8U, 3);
  cvZero (img);

  // (2)学習データの生成
  for (i = 0; i < s; i++) {
    pts[i].x = cvRandInt (&rng) % size;
    pts[i].y = cvRandInt (&rng) % size;
    if (pts[i].y > 50 * cos (pts[i].x * CV_PI / 100) + 200) {
      cvLine (img, cvPoint (pts[i].x - 2, pts[i].y - 2), cvPoint (pts[i].x + 2, pts[i].y + 2), CV_RGB (255, 0, 0));
      cvLine (img, cvPoint (pts[i].x + 2, pts[i].y - 2), cvPoint (pts[i].x - 2, pts[i].y + 2), CV_RGB (255, 0, 0));
      res[i] = 1;
    }
    else {
      if (pts[i].x > 200) {
        cvLine (img, cvPoint (pts[i].x - 2, pts[i].y - 2), cvPoint (pts[i].x + 2, pts[i].y + 2), CV_RGB (0, 255, 0));
        cvLine (img, cvPoint (pts[i].x + 2, pts[i].y - 2), cvPoint (pts[i].x - 2, pts[i].y + 2), CV_RGB (0, 255, 0));
        res[i] = 2;
      }
      else {
        cvLine (img, cvPoint (pts[i].x - 2, pts[i].y - 2), cvPoint (pts[i].x + 2, pts[i].y + 2), CV_RGB (0, 0, 255));
        cvLine (img, cvPoint (pts[i].x + 2, pts[i].y - 2), cvPoint (pts[i].x - 2, pts[i].y + 2), CV_RGB (0, 0, 255));
        res[i] = 3;
      }
    }
  }
  
  // (3)学習データの表示
  cvNamedWindow ("SVM", CV_WINDOW_AUTOSIZE);
  cvShowImage ("SVM", img);
  cvWaitKey (0);
  */

  // (4)学習パラメータの生成
  /*
  for (i = 0; i < size; i++) {
    data[i * 2] = float (pts[i].x) / size;
    data[i * 2 + 1] = float (pts[i].y) / size;
  }
  */
  cvInitMatHeader (&data_mat, size, DIM, CV_32FC1, data);
  cvInitMatHeader (&res_mat, size, 1, CV_32SC1, res);
  criteria = cvTermCriteria (CV_TERMCRIT_EPS, 1000, FLT_EPSILON);
  param = CvSVMParams (CvSVM::C_SVC, CvSVM::RBF, 10.0, 8.0, 1.0, 10.0, 0.5, 0.1, NULL, criteria);

  // (5)SVMの学習
  svm.train (&data_mat, &res_mat, NULL, NULL, param);

  // (6)学習結果の描画
  /*
  for (i = 0; i < size; i++) {
    for (j = 0; j < size; j++) {
      CvMat m;
      float ret = 0.0;
      float a[] = { float (j) / size, float (i) / size };
      cvInitMatHeader (&m, 1, 2, CV_32FC1, a);
      ret = svm.predict (&m);
      switch ((int) ret) {
      case 1:
        rcolor = CV_RGB (100, 0, 0);
        break;
      case 2:
        rcolor = CV_RGB (0, 100, 0);
        break;
      case 3:
        rcolor = CV_RGB (0, 0, 100);
        break;
      }
      cvSet2D (img, i, j, rcolor);
    }
  }
  */
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

  // (9)画像の表示
  img = cvCreateImage (cvSize (size, size), IPL_DEPTH_8U, 3);
  cvZero(img);
  cvNamedWindow ("SVM", CV_WINDOW_AUTOSIZE);
  cvShowImage ("SVM", img);
  cvWaitKey (0);

  cvDestroyWindow ("SVM");
  cvReleaseImage (&img);

  return 0;
}

