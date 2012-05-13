// FaceRecognition2.cpp : �R���\�[�� �A�v���P�[�V�����̃G���g�� �|�C���g���`���܂��B
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
 FaceRecognition2.cpp �ɂ���

 ���o���ꂽ��̕���
 �y�ȉ���2�_���܂ޒ����`
 ����̍��W:(faceRect->x * SCALE, faceRect->y * SCALE)
 �E���̍��W:((faceRect->x + faceRect->width) * SCALE,(faceRect->y + faceRect->height) * SCALE)�z
 ��bmp�Ƃ��ăv���W�F�N�g������t�H���_�ۑ����܂�
 
 �L���v�`���\������
 CTRL + ALT + ���N���b�N �Ńt�@�C������
 "face%04d.bmp",counter
 �Ƃ��āA�ۑ����܂��B
 counter�̒l�͋N�����邽�т�0�ɏ���������܂��B

 ���܂Ɍ����s���̃G���[�ɂ��x_1,x_2,x_3,x_4�ɂƂ�ł��Ȃ��l������܂����A���̏ꍇ��
 on_face(faceRect, face_info);
 �̍s���R�����g�A�E�g���čēx�r���h�����̂��A�R�����g�A�E�g���O���Ă�����x�r���h����Ǝ���Ǝv���܂��B
 (�ۑ��������������Ȃ�A����������L�̍s�͕K�v����܂���̂ŁA�R�����g�A�E�g�����܂܂ł����܂��܂���)

\**********************************************************************************************/

/* �O���[�o���ϐ� */
CvFont font;
int counter = 0; //�Z�[�u�����L���v�`���̖���
IplImage *mouse_info = 0; //�擾���ꂽ�}�E�X�C�x���g��\��
IplImage *face_info = 0; //���o���ꂽ��Ɋւ�����W����\��

// �r�f�I�L���v�`���\����
CvCapture* capture = 0;
// �t���[���P�ʃf�[�^
IplImage* frame = 0;
// �t���[���P�ʃf�[�^�R�s�[�p
IplImage *frame_copy = 0;
IplImage *frame_for_save = 0; //�Z�[�u�p�t���[��(��`�`��Ȃ�)

/* �v���g�^�C�v�錾 */
void on_mouse (int event, int x, int y, int flags, void *param);

void on_face (CvRect* faceRect, IplImage* face_data)
{
  char str[64];
  static int line = 0;
  const int max_line = 15, w = 15, h = 30;

  switch (1){
  case 0:
	  snprintf (str, 64, "%s", "NO FACE"); //�p�r�Ȃ�
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
//CTRL+ALT+�}�E�X���N���b�N�ŋ�`�Ɉ͂܂ꂽ�����̉摜��؂�o���ĕۑ����郁�\�b�h
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
	
	double height = 240; //�L���v�`���̏c�T�C�Y
	double width = 320; //�L���v�`���̉��T�C�Y
	int c; //���̓L�[�󂯕t���p
	int num = 0; //�t���[����
	char str[64];
	//���ʊ猟�o��̓ǂݍ���
	CvHaarClassifierCascade* cvHCC = (CvHaarClassifierCascade*)cvLoad("C:\\opencv\\data\\haarcascades\\haarcascade_frontalface_default.xml");
	//���o�ɕK�v�ȃ������X�g���[�W��p�ӂ���
	CvMemStorage* cvMStr = cvCreateMemStorage(0);
	//���o�����󂯎�邽�߂̃V�[�P���X��p�ӂ���
	CvSeq* face;
	//0�ԍ��̃J�����ɑ΂���L���v�`���\���̂𐶐������
	capture = cvCreateCameraCapture (0);
	//�L���v�`���̃T�C�Y��ݒ肷��B�������A���̐ݒ�̓L���v�`�����s���J�����Ɉˑ�����̂Œ��ӂ�
	cvSetCaptureProperty (capture, CV_CAP_PROP_FRAME_WIDTH, width);
	cvSetCaptureProperty (capture, CV_CAP_PROP_FRAME_HEIGHT, height);
	cvNamedWindow("capture_face_detect", CV_WINDOW_AUTOSIZE);
	cvNamedWindow("face_info", CV_WINDOW_AUTOSIZE);
	cvNamedWindow ("mouse_info", CV_WINDOW_AUTOSIZE);

	//��~�L�[���������܂ŃJ�����L���v�`���𑱂���
	while (1) {
		CvRect* faceRect;
		frame = cvQueryFrame (capture);
		//�t���[���R�s�[�p�C���[�W�A�Z�[�u�p�C���[�W����
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
		//�ǂݍ��񂾉摜�̃O���[�X�P�[�����A�y�уq�X�g�O�����̋ψꉻ���s��
		IplImage* gray = cvCreateImage(cvSize(frame_copy->width, frame_copy->height), IPL_DEPTH_8U, 1);
		IplImage* detect_frame = cvCreateImage(cvSize((frame_copy->width / SCALE), (frame_copy->height / SCALE)), IPL_DEPTH_8U, 1);
		cvCvtColor(frame_copy, gray, CV_BGR2GRAY);
		cvResize(gray, detect_frame, CV_INTER_LINEAR);
		cvEqualizeHist(detect_frame, detect_frame);
		//�摜�����猟�o�Ώۂ̏����擾����
		face = cvHaarDetectObjects(detect_frame, cvHCC, cvMStr, 1.1, 2, CV_HAAR_DO_CANNY_PRUNING, cvSize(30, 30) );
		for (int i = 0; i < face->total; i++) {
			//���o��񂩂��̈ʒu�����擾
			faceRect = (CvRect*)cvGetSeqElem(face, i);
			//�擾������̈ʒu���Ɋ�Â��A��`�`����s��
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

/* �R�[���o�b�N�֐� */
void on_mouse (int event, int x, int y, int flags, void *param = NULL)
{
  char str[64];
  static int line = 0;
  const int max_line = 15, w = 15, h = 30;

  //�}�E�X�C�x���g���擾
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

  //�}�E�X�{�^���C�C���L�[���擾
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

  //�}�E�X���W�C�C�x���g�C�C���L�[�Ȃǂ��摜�ɕ`��C�\��
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