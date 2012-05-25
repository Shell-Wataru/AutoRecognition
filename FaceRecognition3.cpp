// FaceRecognition3.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
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
 FaceRecognition3.cpp について

 FaceRecognition2.cpp で保存された顔画像を　FACE_SIZE × FASE_SIZE のサイズにリサイズし、
 そのグレースケール画像の各ピクセルデータをtxtにして出力します。

 DATA_DIR で画像があるディレクトリを指定し(特にfaces\\～　のあとのアドレスに注意)、OUTPUT_FILEで指定された
 名前のファイル(予め用意してください)に上書きします。(DATA_DIRに入っている画像の枚数がSAMPLEの値と異なると動きません
 ので注意)

\**********************************************************************************************/


#define FACE_SIZE				 64
#define SAMPLE				     51
#define DATA_DIR        "C:\\Users\\t2ladmin\\Documents\\Visual Studio 2010\\Projects\\FaceRecognition3\\FaceRecognition3\\faces\\ushi50\\ushi"
#define OUTPUT_FILE		"ushi.txt"


//撮影した画像を任意のサイズにリサイズするメソッド
//基本的には FACE_SIZE×FACE_SIZE を想定
IplImage* resize(IplImage* img,int height,int width){
	IplImage* resized_img = cvCreateImage (cvSize(height,width),img->depth,img->nChannels);
	cvResize(img,resized_img,CV_INTER_NN);

	return resized_img;
}

//グレースケールの画像から値を取り出すメソッド
void getData(IplImage* img,uchar data[FACE_SIZE][FACE_SIZE]){
	for (int i=0; i < img->height; i++){
		for (int j=0; j < img->width; j++){
			//値の取り出し
			data[i][j] = img->imageData[img->widthStep*i+j];
			//値の書き込み
			//img->imageData[img->widthStep * y + x]=（0～255の値）;
			//printf("data[%d][%d]= %d ",i,j,data[i][j]);
		}
	}
}

//リサイズした画像から値を取り出す
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

	//ピクセル値を格納する配列の初期化
	for (int i = 0 ; i < FACE_SIZE ; i++){
		for (int j = 0 ; j < FACE_SIZE ; j++){
			for (int k = 0 ; k < SAMPLE ; k++){
				p[k][i][j] = 0;
			}
		}
	}

	//各配列に画像の値を格納
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

	//filenameに記された人に関するSAMPLE枚のデータサンプルの行列data[][]をtxtファイルに書き出す
	FILE *fp;
    char dataname[] = OUTPUT_FILE;
    int count;

    /* ファイルオープン */
    if ((fp = fopen(dataname, "w")) == NULL) {
        fprintf(stderr, "ファイルのオープンに失敗しました．\n");
        return EXIT_FAILURE;
    }

    /* 書き込み */
    for ( int i = 0 ; i < SAMPLE ; i ++ ){
		for ( int j = 0 ; j < FACE_SIZE*FACE_SIZE ; j++){
			count = fprintf(fp, "%d ",data[i][j]);
		}
		count = fprintf(fp, "\n");
	}

    if ( count < 0 ) {
        fprintf(stderr, "ファイルの書込みに失敗しました.\n");
        fclose(fp);
        return EXIT_FAILURE;
    }

    fprintf(stdout, "%sへ%d文字出力しました．\n", dataname, count);

    /* ファイルクローズ */
    fclose(fp);

    return EXIT_SUCCESS;


	//cvNamedWindow("gray", CV_WINDOW_AUTOSIZE);
	//cvShowImage("gray", imgs[0]);
	//cvWaitKey(0);
	//cvReleaseImage(&imgs[0]);
	//cvDestroyWindow("gray");

	return 0;
}

