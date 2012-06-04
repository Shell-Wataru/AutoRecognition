// FaceRecognition3.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
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
 そのグレースケール画像の各ピクセルデータ及び目、鼻、口間の距離をtxtにして出力します。
 (口の認識がかなり悪いので使いものにならないかもしれない)

 DATA_DIR で画像があるディレクトリを指定し(特にfaces\\～　のあとのアドレスに注意)、OUTPUT_FILEで指定された
 名前のファイル(予め用意してください)に上書きします。(DATA_DIRに入っている画像の枚数がSAMPLEの値と異なると動かない
 ので注意)

\**********************************************************************************************/


#define FACE_SIZE				 32		
#define SAMPLE				     51		//サンプル数
#define DATA_DIR        "C:\\Users\\t2ladmin\\Documents\\Visual Studio 2010\\Projects\\FaceRecognition3\\FaceRecognition3\\faces\\ushi50\\ushi"
#define OUTPUT_FILE		"analysis.txt"
#define MARGIN_X                 1/10	//サンプル画像から取り除く横幅
#define MARGIN_Y                 1/10	//サンプル画像から取り除く縦幅
#define SCALE                     5     //器官検出のためのサンプル画像拡大倍率
#define LEFT_EYE        "C:\\opencv\\data\\haarcascades\\haarcascade_mcs_lefteye.xml"
#define RIGHT_EYE       "C:\\opencv\\data\\haarcascades\\haarcascade_mcs_righteye.xml"
#define EYE             "C:\\opencv\\data\\haarcascades\\haarcascade_eye.xml"
#define EYE_PAIR_B      "C:\\opencv\\data\\haarcascades\\haarcascade_mcs_eyepair_big.xml"
#define EYE_PAIR_S      "C:\\opencv\\data\\haarcascades\\haarcascade_mcs_eyepair_small.xml"
#define NOSE            "C:\\opencv\\data\\haarcascades\\haarcascade_mcs_nose.xml"
#define MOUTH           "C:\\opencv\\data\\haarcascades\\haarcascade_mcs_mouth.xml"		//←かなり認識率が悪い

//撮影した画像を任意のサイズにリサイズするメソッド
//基本的には FACE_SIZE×FACE_SIZE を想定
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

//2点間のユークリッド距離を計算するメソッド
int getDist(CvPoint a, CvPoint b){

	double dist = 0.0;
	int dist2 = (a.x - b.x)*(a.x - b.x)+(a.y - b.y)*(a.y - b.y);
	dist = (int)sqrtf(dist2);
	return dist;
}



int _tmain(int argc, _TCHAR* argv[])
{
	const CvArr* imgs[SAMPLE];			//表示テスト用(カラー)
	const CvArr* imgs2[SAMPLE];			//読み取り用(白黒)
	
	int file_num = 0; 
	char dir[] = DATA_DIR;
	char extension[] = ".bmp";
	char file_dir[256];
	//CvPoint reye_leftup[SAMPLE];		//右目左上の座標
	//CvPoint reye_rightdown[SAMPLE];	//右目右下の座標
	//CvPoint leye_leftup[SAMPLE];		//左目左上の座標
	//CvPoint leye_rightdown[SAMPLE];	//左目右下の座標
	CvPoint eye_leftup[SAMPLE];
	CvPoint eye_rightdown[SAMPLE];
	CvPoint nose_leftup[SAMPLE];		//鼻左上の座標
	CvPoint nose_rightdown[SAMPLE];	//鼻右下の座標
	CvPoint mouth_leftup[SAMPLE];		//口左上の座標
	CvPoint mouth_rightdown[SAMPLE];	//口右下の座標
	//CvPoint reye_center[SAMPLE];		//右目の中心座標	
	//CvPoint leye_center[SAMPLE];		//左目の中心座標
	CvPoint eye_center[SAMPLE];
	CvPoint nose_center[SAMPLE];		//鼻の中心座標
	CvPoint mouth_center[SAMPLE];		//口の中心座標
	int dist_LE_to_RE[SAMPLE];			//左目と右目の距離
	int dist_LE_to_NO[SAMPLE];			//左目と鼻の距離
	int dist_RE_to_NO[SAMPLE];			//右目と鼻の距離
	int dist_RE_to_MO[SAMPLE];			//右目と口の距離
	int dist_LE_to_MO[SAMPLE];			//左目と口の距離
	int dist_NO_to_MO[SAMPLE];			//鼻と口の距離

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
	// 検出器の読み込み
	CvHaarClassifierCascade* cvHCC_EYE = (CvHaarClassifierCascade*)cvLoad(EYE);
	CvHaarClassifierCascade* cvHCC_NOSE = (CvHaarClassifierCascade*)cvLoad(NOSE);
	CvHaarClassifierCascade* cvHCC_MOUTH = (CvHaarClassifierCascade*)cvLoad(MOUTH);

	// 検出に必要なメモリストレージを用意する
	CvMemStorage* cvMStr_EYE = cvCreateMemStorage(0);
	CvMemStorage* cvMStr_NOSE = cvCreateMemStorage(0);
	CvMemStorage* cvMStr_MOUTH = cvCreateMemStorage(0);

	// 検出情報を受け取るためのシーケンスを用意する
	CvSeq* eye;
	CvSeq* nose;
	CvSeq* mouth;

	for (int j = 0; j < 8 ; j++){
		// 画像中から検出対象の情報を取得する
		eye = cvHaarDetectObjects((IplImage*)imgs[j], cvHCC_EYE, cvMStr_EYE);
		nose = cvHaarDetectObjects((IplImage*)imgs[j], cvHCC_NOSE, cvMStr_NOSE);
		mouth = cvHaarDetectObjects((IplImage*)imgs[j], cvHCC_MOUTH, cvMStr_MOUTH);



		for (int i = 0; i < eye->total; i++) {
		//for (int i = 0; i < 2; i++) {
			//検出情報から目の位置情報を取得
			CvRect* eyeRect = (CvRect*)cvGetSeqElem(eye, i);
			// 取得した目の位置情報に基づき、矩形描画を行う
			cvRectangle((IplImage*)imgs[j],
				cvPoint(eyeRect->x, eyeRect->y),
				cvPoint(eyeRect->x + eyeRect->width, eyeRect->y + eyeRect->height),
				CV_RGB(0, 255 ,0), 
				1, CV_AA);
			printf("EYE::GREEN\n");
			printf("(%d,%d)\n",eyeRect->x,eyeRect->y);
			printf("width = %d , height = %d \n",eyeRect->width,eyeRect->height);
			
			//////////////////////////////////////////////////////////////
			//検出した目の座標をreye_leftupなどに入れる
			//SCALEで割ることを忘れずに
			
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
	
	//filenameに記された人に関するSAMPLE枚のデータサンプルの行列data[][]をtxtファイルに書き出す
	FILE *fp;
    char dataname[] = OUTPUT_FILE;
    int count = 0;
    // ファイルオープン
    if ((fp = fopen(dataname, "w")) == NULL) {
        fprintf(stderr, "ファイルのオープンに失敗しました．\n");
        return EXIT_FAILURE;
    }
    // 書き込み 
	
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
        fprintf(stderr, "ファイルの書込みに失敗しました.\n");
        fclose(fp);
        return EXIT_FAILURE;
    }
    fprintf(stdout, "%sへ%d文字出力しました．\n", dataname, count);

    // ファイルクローズ 
    fclose(fp);
    return EXIT_SUCCESS;
	
	return 0;
}

