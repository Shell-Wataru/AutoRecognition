// FaceRecognition2.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
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
#define SCALE 1.3

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
 FaceRecognition2.cpp について

 検出された顔の部分
 【以下の2点を含む長方形
 左上の座標:(faceRect->x * SCALE, faceRect->y * SCALE)
 右下の座標:((faceRect->x + faceRect->width) * SCALE,(faceRect->y + faceRect->height) * SCALE)】
 をbmpとしてプロジェクトがあるフォルダ保存します
 
 キャプチャ表示時に
 CTRL + ALT + 左クリック でファイル名を
 "face%04d.bmp",counter
 として、保存します。
 counterの値は起動するたびに0に初期化されます。

 たまに原因不明のエラーによりx_1,x_2,x_3,x_4にとんでもない値が入りますが、その場合は
 on_face(faceRect, face_info);
 の行をコメントアウトして再度ビルドしたのち、コメントアウトを外してもう一度ビルドすると治ると思います。
 (保存がしたいだけなら、そもそも上記の行は必要ありませんので、コメントアウトしたままでもかまいません)

\**********************************************************************************************/

/* グローバル変数 */
CvFont font;
int counter = 0; //セーブしたキャプチャの枚数
IplImage *mouse_info = 0; //取得されたマウスイベントを表示
IplImage *face_info = 0; //検出された顔に関する座標情報を表示

// ビデオキャプチャ構造体
CvCapture* capture = 0;
// フレーム単位データ
IplImage* frame = 0;
// フレーム単位データコピー用
IplImage *frame_copy = 0;
IplImage *frame_for_save = 0; //セーブ用フレーム(矩形描画なし)

/* プロトタイプ宣言 */
void on_mouse (int event, int x, int y, int flags, void *param);

void on_face (CvRect* faceRect, IplImage* face_data)
{
  char str[64];
  static int line = 0;
  const int max_line = 15, w = 15, h = 30;

  switch (1){
  case 0:
	  snprintf (str, 64, "%s", "NO FACE"); //用途なし
    break;
  case 1:
	  int x_1 = (int)faceRect->x * SCALE;
	  int x_2 = (int)faceRect->y * SCALE;
	  int x_3 = (int)(faceRect->x + faceRect->width) * SCALE;
	  int x_4 = (int)(faceRect->y + faceRect->height) * SCALE;
	  snprintf (str, 64, "(%d,%d),(%d,%d)",x_1, x_2,x_3,x_4);
    break;
  }
  if (line > max_line) {
    cvGetRectSubPix (face_info, face_info, cvPoint2D32f (320 - 0.5, 240 - 0.5 + h));
    cvPutText (face_info, str, cvPoint (w, 20 + h * max_line), &font, CV_RGB (200, 100, 0));
  }
  else {
    cvPutText (face_info, str, cvPoint (w, 20 + h * line), &font, CV_RGB (200, 100, 0));
  }
  line++;
  cvShowImage ("face_info", face_info);
}
//CTRL+ALT+マウス左クリックで矩形に囲まれた部分の画像を切り出して保存するメソッド
void mouse_save_image(CvRect* faceRect,IplImage* base_image){
	
	char str[32];
	
	if( GetAsyncKeyState(VK_LBUTTON) & GetAsyncKeyState(VK_CONTROL) & GetAsyncKeyState(VK_MENU)){
		CvRect rect = cvRect( faceRect->x * SCALE, faceRect->y * SCALE, 
			faceRect->width * SCALE, faceRect->height * SCALE );
		cvSetImageROI( base_image, rect );
		sprintf(str,"face%04d.bmp",counter);
		counter++;
		cvSaveImage(str, base_image);
		cvResetImageROI( base_image );
		cvResetImageROI(base_image);
		cvReleaseImage(&base_image);
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	mouse_info = cvCreateImage (cvSize (640, 480), IPL_DEPTH_8U, 3);
	face_info = cvCreateImage (cvSize (640, 480), IPL_DEPTH_8U, 3);
	cvZero (mouse_info);
	cvZero (face_info);
	cvInitFont (&font, CV_FONT_HERSHEY_DUPLEX, 0.4, 0.4);
	
	double height = 240; //キャプチャの縦サイズ
	double width = 320; //キャプチャの横サイズ
	int c; //入力キー受け付け用
	int num = 0; //フレーム数
	char str[64];
	//正面顔検出器の読み込み
	CvHaarClassifierCascade* cvHCC = (CvHaarClassifierCascade*)cvLoad("C:\\opencv\\data\\haarcascades\\haarcascade_frontalface_default.xml");
	//検出に必要なメモリストレージを用意する
	CvMemStorage* cvMStr = cvCreateMemStorage(0);
	//検出情報を受け取るためのシーケンスを用意する
	CvSeq* face;
	//0番号のカメラに対するキャプチャ構造体を生成するる
	capture = cvCreateCameraCapture (0);
	//キャプチャのサイズを設定する。ただし、この設定はキャプチャを行うカメラに依存するので注意る
	cvSetCaptureProperty (capture, CV_CAP_PROP_FRAME_WIDTH, width);
	cvSetCaptureProperty (capture, CV_CAP_PROP_FRAME_HEIGHT, height);
	cvNamedWindow("capture_face_detect", CV_WINDOW_AUTOSIZE);
	cvNamedWindow("face_info", CV_WINDOW_AUTOSIZE);
	cvNamedWindow ("mouse_info", CV_WINDOW_AUTOSIZE);

	//停止キーが押されるまでカメラキャプチャを続ける
	while (1) {
		CvRect* faceRect;
		frame = cvQueryFrame (capture);
		//フレームコピー用イメージ、セーブ用イメージ生成
		frame_copy = cvCreateImage(cvSize(frame->width, frame->height), IPL_DEPTH_8U, frame->nChannels);
		frame_for_save = cvCreateImage(cvSize(frame->width, frame->height), IPL_DEPTH_8U, frame->nChannels);
		if(frame->origin == IPL_ORIGIN_TL) {
			cvCopy(frame, frame_copy);
		} else {
			cvFlip(frame, frame_copy);
		}
		if(frame->origin == IPL_ORIGIN_TL) {
			cvCopy(frame, frame_for_save);
		} else {
			cvFlip(frame, frame_for_save);
		}
		if(!frame) break;
		//読み込んだ画像のグレースケール化、及びヒストグラムの均一化を行う
		IplImage* gray = cvCreateImage(cvSize(frame_copy->width, frame_copy->height), IPL_DEPTH_8U, 1);
		IplImage* detect_frame = cvCreateImage(cvSize((frame_copy->width / SCALE), (frame_copy->height / SCALE)), IPL_DEPTH_8U, 1);
		cvCvtColor(frame_copy, gray, CV_BGR2GRAY);
		cvResize(gray, detect_frame, CV_INTER_LINEAR);
		cvEqualizeHist(detect_frame, detect_frame);
		//画像中から検出対象の情報を取得する
		face = cvHaarDetectObjects(detect_frame, cvHCC, cvMStr, 1.1, 2, CV_HAAR_DO_CANNY_PRUNING, cvSize(30, 30) );
		for (int i = 0; i < face->total; i++) {
			//検出情報から顔の位置情報を取得
			faceRect = (CvRect*)cvGetSeqElem(face, i);
			//取得した顔の位置情報に基づき、矩形描画を行う
			cvRectangle(frame_copy,
				cvPoint(faceRect->x * SCALE, faceRect->y * SCALE),
				cvPoint((faceRect->x + faceRect->width) * SCALE, (faceRect->y + faceRect->height) * SCALE),
				CV_RGB(0, 255 ,0),
				2, CV_AA);
		}
		snprintf(str,64,"%03d[frame],%03d[counter]",num,counter);
		cvPutText(frame_copy,str,cvPoint(10,20),&font,CV_RGB(255,0,0));
		cvSetMouseCallback ("capture_face_detect", on_mouse);
		on_face(faceRect, face_info);
		cvShowImage ("capture_face_detect", frame_copy);
		cvShowImage ("face_info", face_info);
		cvShowImage ("mouse_info", mouse_info);
		mouse_save_image(faceRect,frame_for_save);
		num++;
		c = cvWaitKey (5);
		if (c == 27) {break;}
	}
	cvReleaseMemStorage(&cvMStr);
	cvReleaseCapture (&capture);
	cvDestroyWindow("capture_face_detect");
	cvReleaseImage (&mouse_info);
	cvReleaseImage (&face_info);
	cvReleaseHaarClassifierCascade(&cvHCC);
    return 0;
}

/* コールバック関数 */
void on_mouse (int event, int x, int y, int flags, void *param = NULL)
{
  char str[64];
  static int line = 0;
  const int max_line = 15, w = 15, h = 30;

  //マウスイベントを取得
  switch (event) {
  case CV_EVENT_MOUSEMOVE:
    snprintf (str, 64, "(%d,%d) %s", x, y, "MOUSE_MOVE");
    break;
  case CV_EVENT_LBUTTONDOWN:
    snprintf (str, 64, "(%d,%d) %s", x, y, "LBUTTON_DOWN");
    break;
  case CV_EVENT_RBUTTONDOWN:
    snprintf (str, 64, "(%d,%d) %s", x, y, "RBUTTON_DOWN");
    break;
  case CV_EVENT_MBUTTONDOWN:
    snprintf (str, 64, "(%d,%d) %s", x, y, "MBUTTON_DOWN");
    break;
  case CV_EVENT_LBUTTONUP:
    snprintf (str, 64, "(%d,%d) %s", x, y, "LBUTTON_UP");
    break;
  case CV_EVENT_RBUTTONUP:
    snprintf (str, 64, "(%d,%d) %s", x, y, "RBUTTON_UP");
    break;
  case CV_EVENT_MBUTTONUP:
    snprintf (str, 64, "(%d,%d) %s", x, y, "MBUTTON_UP");
    break;
  case CV_EVENT_LBUTTONDBLCLK:
    snprintf (str, 64, "(%d,%d) %s", x, y, "LBUTTON_DOUBLE_CLICK");
    break;
  case CV_EVENT_RBUTTONDBLCLK:
    snprintf (str, 64, "(%d,%d) %s", x, y, "RBUTTON_DOUBLE_CLICK");
    break;
  case CV_EVENT_MBUTTONDBLCLK:
    snprintf (str, 64, "(%d,%d) %s", x, y, "MBUTTON_DOUBLE_CLICK");
    break;
  }

  //マウスボタン，修飾キーを取得
  if (flags & CV_EVENT_FLAG_LBUTTON)
    strncat (str, " + LBUTTON", 64);
  if (flags & CV_EVENT_FLAG_RBUTTON)
    strncat (str, " + RBUTTON", 64);
  if (flags & CV_EVENT_FLAG_MBUTTON)
    strncat (str, " + MBUTTON", 64);
  if (flags & CV_EVENT_FLAG_CTRLKEY)
    strncat (str, " + CTRL", 64);
  if (flags & CV_EVENT_FLAG_SHIFTKEY)
    strncat (str, " + SHIFT", 64);
  if (flags & CV_EVENT_FLAG_ALTKEY)
    strncat (str, " + ALT", 64);

  //マウス座標，イベント，修飾キーなどを画像に描画，表示
  if (line > max_line) {
    cvGetRectSubPix (mouse_info, mouse_info, cvPoint2D32f (320 - 0.5, 240 - 0.5 + h));
    cvPutText (mouse_info, str, cvPoint (w, 20 + h * max_line), &font, CV_RGB (0, 200, 100));
  }
  else {
    cvPutText (mouse_info, str, cvPoint (w, 20 + h * line), &font, CV_RGB (0, 200, 100));
  }
  line++;
  cvShowImage ("mouse_info", mouse_info);
}