// FaceRecognition.cpp : �R���\�[�� �A�v���P�[�V�����̃G���g�� �|�C���g���`���܂��B
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

#define SCALE 1.3

//track bar
int g_slider_position = 0;
CvCapture* g_capture = NULL;

void onTrackbarSlide(int pos){

	cvSetCaptureProperty(g_capture, CV_CAP_PROP_POS_FRAMES, pos);  //CV_CAP_PROP_POS_AVI_RATIO
}

//smoothing picture
void example2_4(IplImage* image){

	cvNamedWindow("Example4-in");
	cvNamedWindow("Example4-out");
	cvShowImage("Example4-in",image);
	IplImage* out = cvCreateImage(cvGetSize(image),IPL_DEPTH_8U,3);
	
	//���ꂼ��̃s�N�Z���𒆐S��3�~3�̗̈��Gaussian�ŕ�����
	cvSmooth(image,out,CV_GAUSSIAN,3,3);
	cvShowImage("Example4-out",out);
	
	cvReleaseImage(&out);
	cvWaitKey(0);
	cvDestroyWindow("Example4-in");
	cvDestroyWindow("Example4-out");
}

//hsv
void saturate_hsv(IplImage* img){

	for(int y=0;y<img->height;y++){
		uchar* ptr = (uchar*) (img->imageData + y * img->widthStep);
		for(int x=0;x<img->width;x++){
			ptr[3*x]   = 255-ptr[3*x];		//h:�F��
			ptr[3*x+1] = 255-ptr[3*x+1];		//s:�ʓx
			ptr[3*x+2] = 255-ptr[3*x+2];		//v:���x
		}
	}
	cvNamedWindow("saturate_pic");
	cvShowImage("saturate_pic",img);
	cvReleaseImage(&img);
	cvWaitKey(0);
	cvDestroyWindow("saturate_pic");
}

//half size
IplImage* doPyrDown(IplImage* in,int filter = IPL_GAUSSIAN_5x5){

	assert(in->width%2 == 0 && in->height%2 == 0);
	IplImage* out = cvCreateImage(cvSize(in->width/2,in->height/2),in->depth,in->nChannels);
	cvPyrDown(in,out);
	return(out);
}

//canny
IplImage* doCanny(IplImage* in,double lowThresh,double highThresh,double aperture){

	if(in->nChannels != 1){ return(0);} 
	IplImage* out = cvCreateImage(cvGetSize(in),in->depth,1);
	cvCanny(in,out,lowThresh,highThresh,aperture);
	return(out);
}

//�}�E�X�C�x���g�ɑ΂��đg�ݍ��܂��R�[���o�b�N
//���{�^������������ƁA�l�p�������n�߂�
//�{�^���𗣂��ƁA���̎l�p�����݂̉摜�ɒǉ������
//�}�E�X���{�^���������ꂽ�܂܂Ńh���b�O����Ǝl�p�����T�C�Y����
/*void my_mouse_callback(int event,int x,int y,int flags,void* param){
	IplImage* image = (IplImage*) param;
	switch(event){
	
		case CV_EVENT_MOUSEMOVE:{
			if(drawing_box){
				box.width = x-box.x;
				box.height = y-box.y;
			}					
		}
		break;
		case CV_EVENT_LBUTTONDOWN:{
			drawing_box = true;
			box = cvRect(x,y,0,0);
		}
		break;
		case CV_EVENT_LBUTTONUP:{
			drawing_box = false;
			if(box.width<0){
				box.x += box.width;
				box.width *= -1;
			}
			if(box.height<0){
				box.y += box.height;
				box.height *= -1;
			}
			draw_box(image,box);
		}
		break;
	}
}*/

//CvRect box;
//bool drawing_box = false;
//�摜��Ɏl�p��`�悷��T�u���[�`��
/*void draw_box(IplImage* img,CvRect box){
	cvRectangle(
		img,
		cvPoint(box.x,box.y),
		cvPoint(box.x+box.width,box.y+box.height),
		cvScalar(255,0,0)
	);
}*/

//detect_and_draw
/*
void detect_and_draw(IplImage* img, double scale = 1.3){
	static CvScalar colors[] = {
		{{0,0,255}},{{0,128,255}},{{0,255,255}},{{0,255,0}},
		{{255,128,0}},{{255,255,0}},{{255,0,0}},{{255,0,255}}
	};
	//�摜�̏���
	IplImage* gray = cvCreateImage(cvSize(img->width,img->height),8,1);
	IplImage* small_img = cvCreateImage(cvSize(cvRound(img->width/scale),cvRound(img->height/scale)),8,1);
	cvCvtColor(img,gray,CV_BGR2GRAY);
	cvResize(gray,small_img,CV_INTER_LINEAR);
	cvEqualizeHist(small_img,small_img);
	//�I�u�W�F�N�g������Ό��o����
	cvClearMemStorege(storage);
	Cvseq* objects = cvHaarDetectObjects(small_img,cascade,storage,1.1,2,0,cvSize(30,30));
	//���������I�u�W�F�N�g�����[�v�������A����Ɏl�p�`��`�悷��
	for(int i = 0; i < (objects ? objects->total : 0); i++ ){
	
		CvRect* r = (CvRect*)cvGetSeqElem(objects,i);
		cvRectangle(img,cvPoint(r->x*scale,r->y*scale),cvPoint((r->x+r->width)*scale,(r->y+r->
			)*scale),color[i%8])
	}
	cvReleaseImage(&gray);
	cvReleaseImage(&small_img);
}
*/
int main(int argc, char *argv)
{  
	
	IplImage* img = cvLoadImage("C:\\opencv\\samples\\c\\lena.jpg");
	IplImage* imgc = cvLoadImage("C:\\opencv\\samples\\c\\box_in_scene.png");
	//char* lena = argc > 1 ? argv[1] : "C:\\opencv\\samples\\c\\lena.jpg";
	//IplImage* img_g = cvLoadImage(lena, CV_LOAD_IMAGE_GRAYSCALE);
	IplImage* tmp_img = cvCreateImage(cvGetSize(img), IPL_DEPTH_32F, 1);
	IplImage* face_img = cvLoadImage("C:\\Users\\t2ladmin\\Documents\\Visual Studio 2010\\Projects\\FaceRecognition\\Faces\\image_0074.jpg");
	IplImage* neta = cvLoadImage("C:\\Users\\t2ladmin\\Downloads\\pic\\president.jpg");
	IplImage* src = cvLoadImage("C:\\opencv\\samples\\c\\cat.jpg",1);
	IplImage* src_1 = cvLoadImage("C:\\opencv\\samples\\c\\cat.jpg",1);
	IplImage* src_2 = cvLoadImage("C:\\opencv\\samples\\c\\baboon200.jpg",1);
	IplImage* dimg = cvLoadImage("C:\\opencv\\samples\\cpp\\tutorial_code\\HighGUI\\video-input-psnr-ssim\\video\\Megamind.avi.jpg",1);
	CvCapture* capture_1 = cvCreateFileCapture("C:\\opencv\\samples\\cpp\\tutorial_code\\HighGUI\\video-input-psnr-ssim\\video\\Megamind.avi");
	CvHaarClassifierCascade* cascade = (CvHaarClassifierCascade*)cvLoad("C:\\opencv\\data\\haarcascades\\haarcascade_frontalface_default.xml");
	CvMemStorage* storage = cvCreateMemStorage(0);
	
	//display picture-----------------------------------------------------------------------------
	/*
	IplImage* img = cvLoadImage("C:\\opencv\\samples\\c\\lena.jpg");
	cvNamedWindow("Example1", CV_WINDOW_AUTOSIZE); //default size of the picture
	//cvNamedWindow("Example1", 0); //fixed size of the picture
	cvShowImage("Example1", img);
	cvWaitKey(0);
	cvReleaseImage(&img);
	cvDestroyWindow("Example1");
	*/

	//display movie-------------------------------------------------------------------------------
	/*
	cvNamedWindow("Example2", CV_WINDOW_AUTOSIZE);
	CvCapture* capture = cvCreateFileCapture("C:\\opencv\\samples\\c\\tree.avi");
	IplImage* frame = NULL;
	while(1){
	
		frame = cvQueryFrame(capture);
		if(!frame) break;
		cvShowImage("Example2",frame);
		char c = cvWaitKey(100);
		if(c == 27) break; //break with Esc
	}
	cvReleaseCapture(&capture);
	cvDestroyWindow("Example2");
	*/
	/*
	cvNamedWindow("Example3", CV_WINDOW_AUTOSIZE);
	g_capture = cvCreateFileCapture("C:\\opencv\\samples\\c\\tree.avi");
	int frames = (int) cvGetCaptureProperty(g_capture, CV_CAP_PROP_FRAME_COUNT);
	if(frames != 0){
	
		cvCreateTrackbar("Position","Example3",&g_slider_position,frames,onTrackbarSlide);
	}
	IplImage* frame;
	while(1){
	
		frame = cvQueryFrame(g_capture);
		if(!frame) break;
		cvShowImage("Example3",frame);
		char c = cvWaitKey(1000);
		if(c == 27) break; //break with Esc
	}
	cvReleaseCapture(&g_capture);
	cvDestroyWindow("Example3");
	*/

	//smoothing picture---------------------------------------------------------------------------
	
	//example2_4(img);

	//display half size picture-------------------------------------------------------------------
	/*
	cvNamedWindow("Example5", CV_WINDOW_AUTOSIZE); //default size of the picture
	//cvNamedWindow("Example1", 0); //fixed size of the picture
	IplImage* img_half = doPyrDown(img,IPL_GAUSSIAN_5x5);
	cvShowImage("Example5", img_half);
	cvWaitKey(0);
	cvReleaseImage(&img_half);
	cvDestroyWindow("Example5");
	*/

	//Canny edge Dct------------------------------------------------------------------------------
	/*
	cvNamedWindow("Example6", CV_WINDOW_AUTOSIZE); //default size of the picture
	IplImage* img_canny = doCanny(img_g,50.0,200.0,3);
	cvShowImage("Example6", img_canny);
	cvWaitKey(0);
    cvReleaseImage(&img_canny);
	cvDestroyWindow("Example6");
	*/
	//Sobel edge Dct-----------------------------------------------------------------------------
	/*
	IplImage* sobel_img = cvCreateImage(cvGetSize(img), IPL_DEPTH_8U, 1);
	cvNamedWindow("Example7", CV_WINDOW_AUTOSIZE); //default size of the picture
	cvSobel(img_g,tmp_img,1,0,3);
	cvConvertScaleAbs(tmp_img,sobel_img,1,0);
	cvShowImage("Example7", sobel_img);
	cvWaitKey(0);
    cvReleaseImage(&sobel_img);
	cvDestroyWindow("Example7");
	*/
	//Laplace edge Dct--------------------------------------------------------------------------
	/*
	IplImage* laplace_img = cvCreateImage(cvGetSize(img), IPL_DEPTH_8U, 1);
	cvNamedWindow("Example8", CV_WINDOW_AUTOSIZE); //default size of the picture
	cvLaplace(img_g,tmp_img);
	cvConvertScaleAbs(tmp_img,laplace_img);
	cvShowImage("Example8", laplace_img);
	cvWaitKey(0);
    cvReleaseImage(&laplace_img);
	cvDestroyWindow("Example8");
	*/
	//use CV_MAT_ELEM to access Matrix
	/*
	CvMat* mat = cvCreateMat(5,5,CV_32FC1);
	float element_3_2 = CV_MAT_ELEM(*mat,float,3,2);
	*/
    
    //Face Dect----------------------------------------------------------------------------------
	//detect_and_draw(img);
	
	// �猟�o�Ώۂ̉摜�f�[�^�p
	IplImage* tarImg;
	IplImage* targray;
	// ���o�Ώۂ̉摜�t�@�C���p�X
	//char tarFilePath[] = "lena.jpg";
	// �摜�f�[�^�̓ǂݍ���
	tarImg = cvLoadImage("C:\\Users\\t2ladmin\\Documents\\Visual Studio 2010\\Projects\\FaceRecognition3\\FaceRecognition3\\faces\\lena\\monro0000.jpg");
	targray = cvCreateImage(cvSize(tarImg->width, tarImg->height), IPL_DEPTH_8U, 1);
	cvCvtColor(tarImg, targray, CV_BGR2GRAY);
	//tarImg = img;
	//tarImg = face_img;
	// ���ʊ猟�o��̓ǂݍ���
	CvHaarClassifierCascade* cvHCC = (CvHaarClassifierCascade*)cvLoad("C:\\opencv\\data\\haarcascades\\haarcascade_frontalface_default.xml");
	// ���o�ɕK�v�ȃ������X�g���[�W��p�ӂ���
	CvMemStorage* cvMStr = cvCreateMemStorage(0);
	// ���o�����󂯎�邽�߂̃V�[�P���X��p�ӂ���
	CvSeq* face;
	// �摜�����猟�o�Ώۂ̏����擾����
	face = cvHaarDetectObjects(tarImg, cvHCC, cvMStr);
	for (int i = 0; i < face->total; i++) {
		//���o��񂩂��̈ʒu�����擾
		CvRect* faceRect = (CvRect*)cvGetSeqElem(face, i);
		// �擾������̈ʒu���Ɋ�Â��A��`�`����s��
		cvRectangle(tarImg,
			cvPoint(faceRect->x, faceRect->y),
			cvPoint(faceRect->x + faceRect->width, faceRect->y + faceRect->height),
			CV_RGB(0, 255 ,0), 
			2, CV_AA);
		IplImage* tarface = cvCreateImage(cvSize(faceRect->width,faceRect->height),IPL_DEPTH_8U,1);
		cvSetImageROI(targray,*faceRect);
		cvResize(targray,tarface,CV_INTER_LINEAR);
		cvResetImageROI(targray);
		cvSaveImage("monrogray.jpg",tarface);
	}
	cvNamedWindow("face_detect");
	cvShowImage("face_detect", tarImg);
	cvWaitKey(0);
	cvSaveImage("monroDct.jpg",tarImg);
	cvDestroyWindow("face_detect");
	cvReleaseMemStorage(&cvMStr);
	cvReleaseHaarClassifierCascade(&cvHCC);
	cvReleaseImage(&tarImg);
	
	
	//hsv
	//saturate_hsv(img);

	//ROI_add <image><x><y><width><height><add>
	//if(src != NULL){
		//int x = 90;	int y = 180;	int width = 150;	int height = 120;
		//int add_val1 = 100.0;		//blue
		//int add_val2 = 0.0;			//green
		//int add_val3 = 0.0;			//red
		//int add_val4 = 0.0;			//mask default -> NULL
		//�w�肵���͈͓��̃s�N�Z���l�̑���~����1~
		/*
		cvSetImageROI(src,cvRect(x,y,width,height));
		cvAddS(src,cvScalar(add_val1,add_val2,add_val3,add_val4),src);
		cvResetImageROI(src);
		cvNamedWindow("ROI_Add",1);
		cvShowImage("ROI_Add",src);
		cvWaitKey(0);
		*/
		//�w�肵���͈͓��̃s�N�Z���l�̑���~����2~
		/*
		IplImage *interest_img;
		CvRect interest_rect;
		interest_img = src;
		interest_rect = cvRect(x,y,width,height);
		IplImage *sub_img = cvCreateImageHeader(
			cvSize(interest_rect.width,interest_rect.height),
			interest_img->depth,interest_img->nChannels
		);
		sub_img->origin = interest_img->origin;
		sub_img->widthStep = interest_img->widthStep;
		sub_img->imageData = interest_img->imageData + 
			interest_rect.y * interest_img->widthStep + 
			interest_rect.x * interest_img->nChannels;
		cvAddS(sub_img,cvScalar(add_val1,add_val2,add_val3,add_val4),sub_img);
		cvNamedWindow("interest_img",1);
		cvShowImage("interest_img",interest_img);
		cvReleaseImageHeader(&sub_img);
		cvWaitKey(0);
		*/
	//}
	
	//alphablend <img1><img2><x><y><width><height><alpha><beta>
	/*
	if(src_1 != NULL && src_2 != NULL){
		int x = 65;	int y = 140;	int width = 200;	int height = 200;
		double alpha = 0.6;	double beta = 0.3; double gamma = 0.0;
		cvSetImageROI(src_1,cvRect(x,y,width,height));
		cvSetImageROI(src_2,cvRect(0,0,width,height));
		cvAddWeighted(src_1,alpha,src_2,beta,gamma,src_1);
		cvResetImageROI(src_1);
		cvNamedWindow("Alpha_blend",1);
		cvShowImage("Alpha_blend",src_1);
		cvWaitKey(0);
	}
	*/
	//check IPP
	//char* libraries;
	//char* modules;
	//cvGetModuleInfo(0,&libraries,&modules);
	//printf("Libraries: %s\nModules: %s\n",libraries,modules);
	/*
	//Dct Face from Camera
	// �r�f�I�L���v�`���\����
	CvCapture* capture = 0;
	// �t���[���P�ʃf�[�^
	IplImage* frame = 0;
	// �t���[���P�ʃf�[�^�R�s�[�p
	IplImage *frame_copy = 0;
	// �c���T�C�Y
	double height = 240;
	double width = 320;
	// ���̓L�[�󂯕t���p
	int c;
	// ���ʊ猟�o��̓ǂݍ���
	CvHaarClassifierCascade* cvHCC = (CvHaarClassifierCascade*)cvLoad("C:\\opencv\\data\\haarcascades\\haarcascade_frontalface_default.xml");
	// ���o�ɕK�v�ȃ������X�g���[�W��p�ӂ���
	CvMemStorage* cvMStr = cvCreateMemStorage(0);
	// ���o�����󂯎�邽�߂̃V�[�P���X��p�ӂ���
	CvSeq* face;
	// 0�ԍ��̃J�����ɑ΂���L���v�`���\���̂𐶐������
	capture = cvCreateCameraCapture (0);
	// �L���v�`���̃T�C�Y��ݒ肷��B�������A���̐ݒ�̓L���v�`�����s���J�����Ɉˑ�����̂Œ��ӂ�
	cvSetCaptureProperty (capture, CV_CAP_PROP_FRAME_WIDTH, width);
	cvSetCaptureProperty (capture, CV_CAP_PROP_FRAME_HEIGHT, height);
	cvNamedWindow ("capture_face_detect", CV_WINDOW_AUTOSIZE);
	// ��~�L�[���������܂ŃJ�����L���v�`���𑱂���
	
	while (1) {
		frame = cvQueryFrame (capture);
		// �t���[���R�s�[�p�C���[�W����
		frame_copy = cvCreateImage(cvSize(frame->width, frame->height), IPL_DEPTH_8U, frame->nChannels);
		if(frame->origin == IPL_ORIGIN_TL) {
			cvCopy(frame, frame_copy);
		} else {
			cvFlip(frame, frame_copy);
		}
		if(!frame) break;
		// �ǂݍ��񂾉摜�̃O���[�X�P�[�����A�y�уq�X�g�O�����̋ψꉻ���s��
		IplImage* gray = cvCreateImage(cvSize(frame_copy->width, frame_copy->height), IPL_DEPTH_8U, 1);
		IplImage* detect_frame = cvCreateImage(cvSize((frame_copy->width / SCALE), (frame_copy->height / SCALE)), IPL_DEPTH_8U, 1);
		cvCvtColor(frame_copy, gray, CV_BGR2GRAY);
		cvResize(gray, detect_frame, CV_INTER_LINEAR);
		cvEqualizeHist(detect_frame, detect_frame);
		// �摜�����猟�o�Ώۂ̏����擾����
		face = cvHaarDetectObjects(detect_frame, cvHCC, cvMStr, 1.1, 2, CV_HAAR_DO_CANNY_PRUNING, cvSize(30, 30) );
		for (int i = 0; i < face->total; i++) {
			// ���o��񂩂��̈ʒu�����擾
			CvRect* faceRect = (CvRect*)cvGetSeqElem(face, i);
			// �擾������̈ʒu���Ɋ�Â��A��`�`����s��
			cvRectangle(frame_copy,
				cvPoint(faceRect->x * SCALE, faceRect->y * SCALE),
				cvPoint((faceRect->x + faceRect->width) * SCALE, (faceRect->y + faceRect->height) * SCALE),
				CV_RGB(0, 255 ,0),
				2, CV_AA);
		}
		cvShowImage ("capture_face_detect", frame_copy);
		c = cvWaitKey (5);
		if (c == 27) {break;}
	}
	
	cvReleaseMemStorage(&cvMStr);
	cvReleaseCapture (&capture);
	cvDestroyWindow("capture_face_detect");
	cvReleaseHaarClassifierCascade(&cvHCC);
	*/

	//box = cvRect(-1,-1,0,0);
	//IplImage* image = cvCreateImage(cvSize(200,200),IPL_DEPTH_8U,3);
	//IplImage* image = img;
	//cvZero(image);
	//IplImage* temp = cvCloneImage(image);
	//cvNamedWindow("Box Example");
	//�R�[���o�b�N��g�ݍ���
	//cvSetMouseCallback("Box Example",my_mouse_callback,(void*)image);
	//��Ɨp�摜��temp�摜�ɃR�s�[���āA���[�U�[���`�撆�Ȃ�΁A���ݕ`���Ă���l�p��temp�摜��ɒu��
	//temp�摜��\�����A15ms�L�[���͂�҂�
	/*while(1){
		cvCopyImage(image,temp);
		if(drawing_box) draw_box(temp,box);
		cvShowImage("Box Example",temp);
		if(cvWaitKey(15)==27) break;
	}*/
	//cvShowImage("Box Example",img);
	//cvWaitKey(0);
	//cvShowImage("temp",temp);
	//cvWaitKey(0);
	//cvReleaseImage(&img);
	//cvReleaseImage(&temp);
	//cvDestroyWindow("Box Example");

	return 0;
}