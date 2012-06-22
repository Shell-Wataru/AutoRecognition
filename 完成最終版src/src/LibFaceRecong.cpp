#include "LibFaceRecong.h"
#define snprintf _snprintf
CvFont font;
FILE *datafile;
//ビデオキャプチャ構造体
CvCapture* capture = 0;
// フレーム単位データ
IplImage* frame = 0;
// 認識用フレーム(矩形描画なし)
IplImage* frame_copy = 0;
IplImage* recog = 0;

int height = 240; //キャプチャの縦サイズ
int width = 320; //キャプチャの横サイズ
int c; //入力キー受け付け用
int num = 0; //フレーム数


int specifyFace;
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

//グレースケールの画像から値を取り出すメソッド
void getData(IplImage* img,uchar vec[FACE_SIZE*FACE_SIZE]){
	for (int i=0; i < img->height; i++){
		for (int j=0; j < img->width; j++){
			vec[FACE_SIZE*i+j] = img->imageData[img->widthStep*i+j];
		}
	}
}

void loadLearnData(){
	datafile = fopen(VV,"r");
	if (datafile == NULL) {
		printf("cannot open\n");
		exit(1);
	}

	//for (int i = 0 ; i < MEN ; i++) fscanf(datafile,"%d",&vv[0][i]);
	
	for (int i = 0 ; i < MEN ; i++){
		for (int  j = 0 ; j < FACE_SIZE*FACE_SIZE ; j++){
			fscanf(datafile,"%lf",&vv[i][j]);
		}
	}

	fclose(datafile);
	//printf("vv[%d][%d] = %f\n",0,0,vv[0][0]);
}

int specifyOne(){
	if(vec[0] = 0) return -1;
	for (int i = 0; i < MEN ; i++) obj[i] = 0.0;
	
	for(int i = 0; i < MEN ; i++){
		for(int j = 0; j < FACE_SIZE*FACE_SIZE ; j++ ){
			obj[i] += vv[i][j]*vec[j]; 
		}
	}
	double tempmax = obj[0];
	int maxindex = 1;
	for(int i = 0; i < MEN ; i++) {
		if(obj[i]>tempmax) {
			tempmax = obj[i];
			maxindex = i+1;
		}
	}
	return maxindex;
}
#ifdef EYE_NOSE
//両目を探します、両目の位置を元に鼻が探知されます
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
						cx = (int)(eye_leftup.x + eye_rightdown.x)/2;
						cy = (int)(eye_leftup.y + eye_rightdown.y)/2;
						if(((int)(pw*param1) < cx)&(cx < (int)(pw*param2))
							&((int)(ph*param3) < cy)&(cy < (int)(ph*param4))
							){
							reye_center.x = cx;
							reye_center.y = cy;
						}
						if(((int)(pw*param5) < cx)&(cx < (int)(pw*param6))
							&((int)(ph*param7) < cy)&(cy < (int)(ph*param8))
							){
							leye_center.x = cx;
							leye_center.y = cy;
						}
						if(flag){
							cvLine(tarImg,
								cvPoint((int)(reye_center.x-2+px)*SCALE0,(int)(reye_center.y-2+py)*SCALE0),
								cvPoint((int)(reye_center.x+2+px)*SCALE0,(int)(reye_center.y+2+py)*SCALE0),
								CV_RGB( 0 , 255 , 0));
							cvLine(tarImg,
								cvPoint((int)(leye_center.x-2+px)*SCALE0,(int)(leye_center.y+2+py)*SCALE0),
								cvPoint((int)(leye_center.x+2+px)*SCALE0,(int)(leye_center.y-2+py)*SCALE0),
								CV_RGB( 0 , 255 , 0));
							cvCircle(tarImg,
								cvPoint((int)(reye_center.x+px)*SCALE0,(int)(reye_center.y+py)*SCALE0),
								4,CV_RGB( 0 , 255 , 0),1,8,0);
							cvCircle(tarImg,
								cvPoint((int)(leye_center.x+px)*SCALE0,(int)(leye_center.y+py)*SCALE0),
								4,CV_RGB( 0 , 255 , 0),1,8,0);
						}
					}
}
#endif EYE_NOSE

//faceRecogの中で使います
//鼻が探知された後に、新たに顔の領域(判定用)を取得し、vecに格納します
void getfaceROI(CvRect* faceRect,double param1,double param2,double param3,bool flag){

		int px = faceRect->x;
		int py = faceRect->y;
		int pw = faceRect->width;
		int ph = faceRect->height;

		//double param1 = 1.05;
		//double param2 = 1.4;
		//double param3 = 1.1;

		if (0 < nose_center.x && nose_center.x < px 
			&& 0 < nose_center.y && nose_center.y < py){
			face_leftup.x = nose_center.x-param1*(nose_rightdown.x-nose_leftup.x);
			if (face_leftup.x < 0) face_leftup.x = 2;
			face_leftup.y = nose_center.y-param2*(nose_rightdown.y-nose_leftup.y);
			if (face_leftup.y < 0) face_leftup.y = 2;
			face_rightdown.x = nose_center.x+param1*(nose_rightdown.x-nose_leftup.x);
			if (face_rightdown.x > pw) face_rightdown.x = pw-2;
			face_rightdown.y = nose_center.y+param3*(nose_rightdown.y-nose_leftup.y);
			if (face_rightdown.y > ph) face_rightdown.y = ph-2;

			FACE = cvRect((face_leftup.x+px)*SCALE0,(face_leftup.y+py)*SCALE0,
					(face_rightdown.x-face_leftup.x)*SCALE0,
					(face_rightdown.y-face_leftup.y)*SCALE0);
			cvSetImageROI(frame_copy,FACE);
			cvResize(frame_copy,recog,CV_INTER_LINEAR);
			cvResetImageROI(frame_copy);
			//cvShowImage("RecogData",recog);
			getData(recog,vec);
		}
		if(flag){
			cvRectangle(frame,
				cvPoint((int)(face_leftup.x+px)*SCALE0,(int)(face_leftup.y+py)*SCALE0),
				cvPoint((int)(face_rightdown.x+px)*SCALE0,(int)(face_rightdown.y+py)*SCALE0),
				CV_RGB(255, 0 ,255), 2, CV_AA);
		}
}

void printResult(){

	printf("%03d[frame]\n",num);
#ifdef EYE_NOSE
	printf("L-EYE=(%d,%d)\n",leye_center.x,leye_center.y);
	printf("R-EYE=(%d,%d)\n",reye_center.x,reye_center.y);
#endif EYE_NOSE
	printf("NOSE=(%d,%d)\n",nose_center.x,nose_center.y);
	printf("obj=[%04f,%04f,%04f,%04f,%04f]\n",obj[0],obj[1],obj[2],obj[3],obj[4]);
	//printf("MOUTH=(%d,%d)\n",mouth_center.x,mouth_center.y);
	printf("====================================\n");
}

//鼻の中心を探します。
//トリミングされた顔領域の値を学習データに掛け、どの教師データに一番近いか判別します
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
#ifdef EYE_NOSE
						int eye_cx = (int)(reye_center.x + leye_center.x)/2;
#endif EYE_NOSE
						nose_leftup.x = noseRect->x;
						nose_leftup.y = noseRect->y;
						nose_rightdown.x = noseRect->x + noseRect->width;
						nose_rightdown.y = noseRect->y + noseRect->height;
						cx = (int)(nose_leftup.x + nose_rightdown.x)/2;
						cy = (int)(nose_leftup.y + nose_rightdown.y)/2;
#ifdef EYE_NOSE
						if(((int)eye_cx-pw*param1<cx)&(cx<(int)eye_cx+pw*param2)
						   &((int)ph*param3<cy)&(cy<(int)ph*param4)
							){
							nose_center.x = cx;
							nose_center.y = cy;
						}
#endif EYE_NOSE

#ifdef NOSE_ONLY
						if(((int)pw*0.40<cx)&(cx<(int)pw*0.60)
						   &((int)ph*0.40<cy)&(cy<(int)ph*0.60)
							){
							nose_center.x = cx;
							nose_center.y = cy;
						}
#endif NOSE_ONLY
						if(0 < nose_center.x && nose_center.x < width &&
						   0 < nose_center.y && nose_center.y < height && flag1
						  ){
							cvLine(tarImg,
								cvPoint((int)(nose_center.x-2+px)*SCALE0,(int)(nose_center.y-2+py)*SCALE0),
								cvPoint((int)(nose_center.x+2+px)*SCALE0,(int)(nose_center.y+2+py)*SCALE0),
								CV_RGB( 255 , 0 , 0));
							cvLine(tarImg,
								cvPoint((int)(nose_center.x-2+px)*SCALE0,(int)(nose_center.y+2+py)*SCALE0),
								cvPoint((int)(nose_center.x+2+px)*SCALE0,(int)(nose_center.y-2+py)*SCALE0),
								CV_RGB( 255 , 0 , 0));
							cvCircle(tarImg,
								cvPoint((int)(nose_center.x+px)*SCALE0,(int)(nose_center.y+py)*SCALE0),
								4,CV_RGB( 255 , 0 , 0),1,8,0);
						}

						getfaceROI(faceRect,1.05,1.4,1.1,flag2);
#ifdef DIS
						sprintf(name,"%s%01d",window,j);
						cvNamedWindow(name, CV_WINDOW_AUTOSIZE);
						cvShowImage(name,recog);
						//cvDestroyWindow(name);
#endif
						char str[64];

						//loadLearnData();
						specifyFace=specifyOne();
						CvScalar color=CV_RGB(255,0,0);
						switch(specifyFace){
						case -1 :
							snprintf(str,64,"%s","none");
							break;
						case 1 :
							snprintf(str,64,"%s","Liang");
							break;
						case 2 :
							snprintf(str,64,"%s","Shell");
							
							break;
						case 3 :
							snprintf(str,64,"%s","NONE");
							break;
						case 4 :
							snprintf(str,64,"%s","NONE");
							
							break;
						case 5 :
							snprintf(str,64,"%s","NONE");
							break;
						}
						cvPutText(frame,str,face_rightdown,
								&font,color);
						printResult();
					}
}

//マイコンが追いかけるべき座標を渡します
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
//口を探します(使いません)
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
