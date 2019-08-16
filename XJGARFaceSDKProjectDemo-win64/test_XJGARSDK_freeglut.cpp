
#include <GL/glew.h>
#define FREEGLUT_LIB_PRAGMAS 1
#include <GL/freeglut.h>
#ifdef __APPLE__
#	include <glut/glut.h>
#else
#	include <GL/glut.h>
#endif

//#include <vld.h>

#pragma comment(lib,"glew32.lib")

#include <vector>
#include <iostream>
#include <fstream>
//#include <thread>
#include "XJGArSdk.h"

#define RENDER_IMG_TO_TEXTURE_TEST 
//#define USE_OPENCV_LIBRARY 


#include <opencv2/opencv.hpp>
#ifdef USE_OPENCV_LIBRARY
//#include "opencv2/opencv.hpp"
cv::VideoCapture mCamera;
#else
#include "libVidCapInterface.h"
//struct SimpleCapParams g_capture;
//int g_iVideoCapDevice = 0;
struct SimpleCapParams capture;
int device;
void InitVideoCapture(int width, int height)
{
	device = setupESCAPI();
	if (device < 1)
	{
		printf("Unable to init ESCAPI\n");
		exit(1);
	}

	// Use the second device if more than one found
	// (makes my personal test setup easier =)
	if (device > 1)
		device = 1;
	else
		device = 0;

	// Setup ESCAPI to capture a 512x512 image.
	capture.mWidth = width;
	capture.mHeight = height;
	capture.mTargetBuf = new int[width * height];
	initCapture(device, &capture);
	doCapture(device);
}

#endif




cv::Mat Image;
int		g_iCamImgWidth = 1280;
int		g_iCamImgHeight = 720;
//int		g_iCamImgWidth = 1024;
//int		g_iCamImgHeight = 768;
//int		g_iCamImgWidth = 980;
//int		g_iCamImgHeight = 540;

int g_iWindowWith = g_iCamImgWidth;
int g_iWindowHeight = g_iCamImgHeight;

int		g_eyeParams_ = 50;
int		g_chinParams_ = 40;
int		g_iRedFaceLevel_ = 80;
int		g_WhiteSkinLevel_ = 40;
int		g_SmoothSkinLevel_ = 100;
int		g_timvInterval = 120;
float	g_fTargetIdleTime = 120.f;

bool	g_bDrawlandMark = true;

std::string g_strSelectedFilterName_ = "无";

unsigned char * g_pImgBufferRGB = NULL;
unsigned char * g_pImgBufferRGBResult = NULL;
unsigned char * g_pImgBufferTargetRGBResult = NULL;
//int		g_iTargetImgWidth	= 480;
//int		g_iTargetImgHeight  = 360;
int		g_iTargetImgWidth	= 368;
int		g_iTargetImgHeight	= 640;

//int		g_iTargetImgWidth = 480;
//int		g_iTargetImgHeight = 360;

//int		g_iTargetImgWidth = 500;
//int		g_iTargetImgHeight = 500;

//int		g_iTargetImgWidth = 1024;
//int		g_iTargetImgHeight = 500;

//char * g_filterList[] = {
//	"无",
//	"冰冷",
//	"健康",
//	"祖母绿",
//	"怀旧",
//	"蜡笔",
//	"常青"
//};
char * g_filterList[] = {
	"filter_none",
	"filter_cool",
	"filter_Healthy",
	"filter_emerald",
	"filter_nostalgia",
	"filter_crayon",
	"filter_evergreen",
};
//char * g_stickPaperList[] = {
//	"stpaper900224",
//	//s	"amaozi",
//	"stpaper900639",
//	"angel",
//	"caishen",
//	"cangou",
//	"daxiongmao",
//	"diving",
//	"flowermustach",
//	"huahuan",
//	"huangyamotuo",
//	"hunli",
//	"leisi",
//	"lufei",
//	"lvhua",
//	"mengtu",
//	"rabbit",
//	"shumeng",
//	"strawberry",
//	"xuezunv"
//};
char * g_stickPaperList[] = {
	"stpaper900224",
	//"caishentest",
	"angel",
	"caishen",
	"cangou",
	"diving",
	"huahuan",
	"leisi",
	"lufei",
	"lvhua",
	"mengtu",
	"xuezunv"
};

void ShowUsage()
{
	std::cout << "keyboard:  s     toggle show sticker papers"	<< std::endl;
	std::cout << "keyboard:  f     toggle filters"				<< std::endl;
	std::cout << "keyboard:  d     toggle show face landmarks"  << std::endl;
	std::cout << "keyboard:  t     toggle show performace statistics" << std::endl;
	std::cout << "keyboard:  c     toggle sticker papers"		<< std::endl;
	std::cout << "keyboard:  l     toggle landmarks" << std::endl;
	std::cout << "keyboard:  Esc   exit"						<< std::endl;
}

static bool g_bShowlandMarkAndResult = false;
void checkKeyInput(int a)
{

	if (a == 's') {
		static bool bDrawStickPapers = true;
		bDrawStickPapers = !bDrawStickPapers;
		XJGARSDKSetShowStickerPapers(bDrawStickPapers);
	}
	else if (a == 'd') {
		g_bDrawlandMark = !g_bDrawlandMark;
	}
	else if (a == 't') {
		static bool bShowPerformanceStatic = false;
		bShowPerformanceStatic = !bShowPerformanceStatic;
		XJGARSDKSetShowPerformanceStatic(bShowPerformanceStatic);
	}
	else if (a == 'c') {
		static int stickPaperIndex = 0;
		int stickPaperLength = 12;
		stickPaperIndex++;
		stickPaperIndex = stickPaperIndex % stickPaperLength;
		if (stickPaperIndex == 11)
			XJGARSDKSetShowStickerPapers(false);
		else
		{
			XJGARSDKSetShowStickerPapers(true);
			XJGARSDKChangeStickpaper(g_stickPaperList[stickPaperIndex]);

		}
	}
	else if (a == 'f') {
		static int iSelectedFilter = 0;
		int iMaxFilterNum = 6;
		iSelectedFilter++;
		iSelectedFilter = iSelectedFilter % iMaxFilterNum;

		XJGARSDKChangeFilter(g_filterList[iSelectedFilter]);
	}
	else if (a == 'l') {
		g_bShowlandMarkAndResult = !g_bShowlandMarkAndResult;
		if (!g_bShowlandMarkAndResult)
		{
			cv::destroyWindow("Caputered Result Image");
		}
	}
	else if (a == 27) {
		XJGARSDKCleanUP();
		exit(1);

	}
}



void onTrackbarSlide(int pos)
{
	XJGARSDKSetBigEyeParam(g_eyeParams_);
	XJGARSDKSetThinChinParam(g_chinParams_);
	XJGARSDKSetRedFaceParam(g_iRedFaceLevel_);
	XJGARSDKSetWhiteSkinParam(g_WhiteSkinLevel_);
	XJGARSDKSetSkinSmoothParam(g_SmoothSkinLevel_);
}

void initCVEnvironment()
{
	
#ifdef USE_OPENCV_LIBRARY
	mCamera.open(0);
	if (!mCamera.isOpened()) {
		std::cout << "Camera opening failed..." << std::endl;
		system("pause");
		exit(1);
	}
	mCamera.set(CV_CAP_PROP_FRAME_WIDTH, g_iCamImgWidth);
	mCamera.set(CV_CAP_PROP_FRAME_HEIGHT, g_iCamImgHeight);
	mCamera >> Image;

	if (Image.empty())
	{
		printf("===========================Nothing readed from camera !!!!!!!!!!!!!!!!\n");
		mCamera >> Image;
	}
	printf("===========================read camera successfully !!\n");

	//Image = g__tmpImg.clone();

	g_iCamImgWidth = Image.cols;
	g_iCamImgHeight = Image.rows;
#else
	InitVideoCapture(g_iCamImgWidth, g_iCamImgHeight);
#endif


	cv::namedWindow("controlwindows", CV_WINDOW_AUTOSIZE);
	cv::Mat emptyImg;
	emptyImg.create(cv::Size(500, 100), CV_8UC3);
	emptyImg = emptyImg.zeros(emptyImg.size(), CV_8UC3);
	cv::imshow("controlwindows", emptyImg);


	cvCreateTrackbar("大眼", "controlwindows", &g_eyeParams_, 100, onTrackbarSlide);
	cvCreateTrackbar("瘦脸", "controlwindows", &g_chinParams_, 100, onTrackbarSlide);
	cvCreateTrackbar("红润", "controlwindows", &g_iRedFaceLevel_, 100, onTrackbarSlide);
	cvCreateTrackbar("美白", "controlwindows", &g_WhiteSkinLevel_, 100, onTrackbarSlide);
	cvCreateTrackbar("磨皮", "controlwindows", &g_SmoothSkinLevel_, 100, onTrackbarSlide);

	printf("3333333333333333333333333333333 !!\n");
}



//bool rgba2rgb(unsigned char *src, unsigned char *dest, int width, int height)
//{
//	int r, g, b, a;
//	for (int i = 0; i<width*height; ++i)
//	{
//		*dest++ = *src++; // load red
//		*dest++ = *src++; // load green
//		*dest++ = *src++; // load blue
//		src++;				//skip alpha value
//	}
//	return true;
//}

bool bgra2rgb(unsigned char *src, unsigned char *dest, int width, int height)
{
	int r, g, b, a;
	for (int i = 0; i < width*height; ++i)
	{
		b = *src++; // load blue
		g = *src++; // load green
		r = *src++; // load red
		a = *src++; // load alpha
		*dest++ = r;
		*dest++ = g;
		*dest++ = b;
	}
	return true;
}


void Draw(void)
{

#ifdef USE_OPENCV_LIBRARY
	mCamera >> Image;
	cv::cvtColor(Image, Image, CV_BGR2RGB);
	g_pImgBufferRGB = Image.data;	
	if (g_pImgBufferRGBResult == NULL)
	{
		g_pImgBufferRGBResult = new unsigned char[g_iCamImgWidth * g_iCamImgHeight * 3];
		g_pImgBufferTargetRGBResult = new unsigned char[g_iTargetImgWidth * g_iTargetImgHeight * 3];
	}
#else
	if (isCaptureDone(device))
	{
		//Image = cv::Mat(g_iCamImgHeight, g_iCamImgWidth, CV_8UC4, (void*)capture.mTargetBuf);
		//cv::cvtColor(Image, Image, CV_BGRA2RGBA);
		//cv::imshow("Image", Image);
		//cv::waitKey(1);

		if (g_pImgBufferRGB == NULL)
		{
			g_pImgBufferRGB				= new unsigned char[g_iCamImgWidth * g_iCamImgHeight * 3];
			g_pImgBufferRGBResult		= new unsigned char[g_iCamImgWidth * g_iCamImgHeight * 3];
			g_pImgBufferTargetRGBResult = new unsigned char[g_iTargetImgWidth * g_iTargetImgHeight * 3];
		}

		bgra2rgb((unsigned char*)capture.mTargetBuf, g_pImgBufferRGB, g_iCamImgWidth, g_iCamImgHeight);
		// ..and ask for more
		doCapture(device);
#endif


		int time1 = glutGet(GLUT_ELAPSED_TIME);
	#ifdef RENDER_IMG_TO_TEXTURE_TEST
		//RenderImageInGLOffScreenBuf(Image);
		


#ifdef RENDER_TEXTURE_TO_TEXTURE_TEST
		static int outPutTex = 0;
		int outPutTex2 = 0;
		XJGARSDKLoadImageToTexture(g_pImgBufferRGB, g_iCamImgWidth, g_iCamImgHeight, &outPutTex);
		XJGARSDKSetInputTextrueFlips(0, 1);
		XJGARSDKRenderGLTexture(outPutTex, g_iCamImgWidth, g_iCamImgHeight, &outPutTex2);
		XJGARSDKDrawAFullViewTexture(outPutTex2, 0, 0, g_iWindowWith, g_iWindowHeight);
#else
		int outPutTex = 0;
		XJGARSDKRenderImage(g_pImgBufferRGB, g_iCamImgWidth, g_iCamImgHeight, &outPutTex);
		XJGARSDKDrawAFullViewTexture(outPutTex, 0, 0, g_iWindowWith, g_iWindowHeight);
#endif


		//XJGARSDKRenderImage(g_pImgBufferRGB, g_iCamImgWidth, g_iCamImgHeight);
	#else
		//RenderImageInGLOffScreenBuf(Image, finalResult);
		XJGARSDKRenderImage(g_pImgBufferRGB, g_iCamImgWidth, g_iCamImgHeight, g_pImgBufferRGBResult);
	#endif
		int time2 = glutGet(GLUT_ELAPSED_TIME);
		printf("render a frame time is : %d ms\n", time2 - time1);
		float currentfps =  1000.0f / float(time2 - time1);
		printf("Current FPS:%4.2f\n", currentfps);
		float tmToSleep = g_fTargetIdleTime - float(time2 - time1);
		if (tmToSleep > 5.f)
			g_timvInterval = int(tmToSleep);
		else 
			g_timvInterval = 1;
		printf("ilde time is : %d ms\n", g_timvInterval);

		
		glutSwapBuffers();


		if (g_bShowlandMarkAndResult)
		{
			float targetScaleFactorX, targetScaleFactorY;
			float faceLandmarks[MAX_FACE_DETECTION_ARRAY_SIZE];

			int faceNum = XJGARSDKGetTargetResultImgAndLandMarks(g_pImgBufferTargetRGBResult, 0,
				faceLandmarks, g_iTargetImgWidth, g_iTargetImgHeight, 2,
				&targetScaleFactorX, &targetScaleFactorY);
			cv::Mat targetResultImg(g_iTargetImgHeight, g_iTargetImgWidth,
				CV_8UC3, (void*)g_pImgBufferTargetRGBResult);
			cv::cvtColor(targetResultImg, targetResultImg, CV_RGB2BGR);

			//int faceNum = XJGARSDKGetFaceLandmarks(faceLandmarks);

			if (g_bDrawlandMark)
			{
				for (int i = 0; i < faceNum; i++)
				{
					for (int j = 0; j < 68; j++)
					{
						float xTarget = faceLandmarks[i * 136 + j * 2 + 0];
						float yTarget = faceLandmarks[i * 136 + j * 2 + 1];

						cv::circle(targetResultImg, cv::Point2i(xTarget, yTarget), 1, cv::Scalar(0, 255, 0), 2);
					}
				}
			}
			cv::flip(targetResultImg, targetResultImg, 0);
			cv::imshow("Caputered Result Image", targetResultImg);
		}


	#ifndef RENDER_IMG_TO_TEXTURE_TEST

		cv::Mat finalResult = cv::Mat(g_iCamImgHeight, g_iCamImgWidth, CV_8UC3, (void*)g_pImgBufferRGBResult);
		cv::cvtColor(finalResult, finalResult, CV_RGB2BGR);
		cv::flip(finalResult, finalResult, 0);

		int intervalVertical = 20;
		int verticalCoord = 50;
		std::stringstream strStream;
		strStream << "keyboard:  s     toggle show sticker papers";
		cv::putText(finalResult, strStream.str(), cvPoint(20, verticalCoord), 0.5, 0.5, cv::Scalar(255, 255, 255));

		verticalCoord = verticalCoord + intervalVertical;
		strStream.str("");
		strStream << "keyboard:  f     toggle filters, currently slected Filter is: " << g_strSelectedFilterName_;
		cv::putText(finalResult, strStream.str(), cvPoint(20, verticalCoord), 0.5, 0.5, cv::Scalar(255, 255, 255));

		verticalCoord = verticalCoord + intervalVertical;
		strStream.str("");
		strStream << "keyboard:  d     toggle show face landmarks";
		cv::putText(finalResult, strStream.str(), cvPoint(20, verticalCoord), 0.5, 0.5, cv::Scalar(255, 255, 255));

		verticalCoord = verticalCoord + intervalVertical;
		strStream.str("");
		strStream << "           c     toggle sticker papers";
		cv::putText(finalResult, strStream.str(), cvPoint(20, verticalCoord), 0.5, 0.5, cv::Scalar(255, 255, 255));

		verticalCoord = verticalCoord + intervalVertical;
		strStream.str("");
		strStream << "           Esc     exit";
		cv::putText(finalResult, strStream.str(), cvPoint(20, verticalCoord), 0.5, 0.5, cv::Scalar(255, 255, 255));

		cv::imshow("XJGARSDKDemo", finalResult);
	#endif


#ifndef USE_OPENCV_LIBRARY
	}
#endif


	extern void onTrackbarSlide(int pos);
	cv::namedWindow("controlwindows");
	cvCreateTrackbar("大眼", "controlwindows", &g_eyeParams_, 100, onTrackbarSlide);
	cvCreateTrackbar("瘦脸", "controlwindows", &g_chinParams_, 100, onTrackbarSlide);
	cvCreateTrackbar("红润", "controlwindows", &g_iRedFaceLevel_, 100, onTrackbarSlide);
	cvCreateTrackbar("美白", "controlwindows", &g_WhiteSkinLevel_, 100, onTrackbarSlide);
	cvCreateTrackbar("磨皮", "controlwindows", &g_SmoothSkinLevel_, 100, onTrackbarSlide);

	static bool bIsFirstTime = true;
	if (bIsFirstTime)
	{
		int xxxx = 50 + g_iCamImgWidth + 20;
		if (xxxx > 1024) xxxx = 1024;
		cv::moveWindow("controlwindows", xxxx, 50);
		ShowUsage();
		bIsFirstTime = false;
	}

	int a = cv::waitKey(1);
	checkKeyInput(a);

}

void ReShapeFunction(int w, int h)
{
	g_iWindowWith = w;
	g_iWindowHeight = h;
}
// 由GLUT函数库调用，计时器函数
void TimerFunction(int value)
{
	// 使用新的坐标重新绘制场景
	glutPostRedisplay();
	// 重新设置计时器函数
	glutTimerFunc(g_timvInterval, TimerFunction, 1);
	//std::chrono::milliseconds dura(g_timvInterval);
	//std::this_thread::sleep_for(dura);
	//glutTimerFunc(1, TimerFunction, 1);
}

void processNormalKeys(unsigned char key, int x, int y)
{
	checkKeyInput(key);
}
int nWindow;
int initAndStartGLEnvironment()
{

	int argc = 1;
	char*argv[] = { "xiaojigou.exe" };
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE /*| GLUT_BORDERLESS*/); // do try as well with GLUT_BORDERLESS and GLUT_CAPTIONLESS
	//glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);

	glutInitWindowPosition(50, 50);
	glutInitWindowSize(g_iCamImgWidth, g_iCamImgHeight);
	printf("window size is width: %d, height: %d\n", g_iCamImgWidth, g_iCamImgHeight);

	//nWindow = glutCreateWindow(g_Windowtitle.c_str());
	nWindow = glutCreateWindow("XJGARSDKDemo");
	glutSetIconTitle("test icon title");
	printf("main window id: %d\n", nWindow);

	GLenum err = glewInit();  // 前面运行了glut*的一系列函数，已经获得了opengl的context，所以这里不会出错
	if (err != GLEW_OK) {
		fprintf(stderr, "%s\n", glewGetErrorString(err));
		return  -1;
	}
	glutDisplayFunc(Draw);

	// 设置计时器函数
	glutTimerFunc(g_timvInterval, TimerFunction, 1);
	glutKeyboardFunc(&processNormalKeys);
	glutReshapeFunc(ReShapeFunction);
	//glutWindowStatusFunc(WindowStatus);

	glutMainLoop();

	return 1;
}


int main(int argc, char *argv[])
{
	
	////const char* licenseText = "Xb2SGQvurZeKn5kjjQr0S5K9skHCzvhIIZs4sVYidel9ZcA/L6SKdN+yrDilBdd2uJMwCiVT+ztQcGajt6jvvO8/P6wM10+UnECq+S+Ke5rtQ/gW59NDwcX1WR0ZD9Po";
	//std::string licenseText = "Xb2SGQvurZeKn5kjjQr0S5K9skHCzvhIIZs4sVYidel9ZcA/L6SKdN+yrDilBdd2uJMwCiVT+ztQcGajt6jvvDNE/pw32pGwbcNRopqFULhKnIogTXCbiyET4zL6LAhsvnYwhs/94hDjmsfrNkurQnf7GPgYpoom6KMXB2S2ARYj0ZrG7pvn1R9sOU9xNW0DxZ74xD3x/xMBklRojzuRDEZm4y/Dwh+ebF5SZJE54dBPPpuBNYg9tS9uvApGwEAYwBTZxhLok67qx5vpkkwHNHY8MgW+uRRnS12d7eGgAsIeEPdRS4eu15Sy7y/HmnbtW+HKnoJktzV3E7ASe6/O1w==";
	//XJGARSDKInitialization(licenseText.c_str(), "testuser", "测试用户公司");

	std::string licenseText = "hMPthC0oBIbtMp515TWb9jZvrLAKWIMvA4Dhf03n51QvnJr7jZowVe86d0WwU0NK9QGRFaXQn628fRu941qyr3FtsI5R7Y6v1XEpL6YvQNWQCkFEt1SAb0hyawimOYf1tfG2lIaNE63c5e+OxXssOVUWvw8tOr2glVwWVzh79NmZMahrnS8l69SoeoXLMKCYlvAt/qJFFk4+6Aq3QvOv3o72fq5p90yty+YWg7o0HirZpMSP9P5/DHYPFqR/ud7twTJ+Yo2+ZzYvodqRQbGG0HseZn8Xpt7fZdFuZbc2HGRMVk56vNDMRlcGZZXAjENk7m2UMhi1ohhuSf4WmIgXCZFiJXvYFByaY625gXKtEI7+b7t81nWQYHP9BEbzURwL";
	XJGARSDKInitialization(licenseText.c_str(), "DoctorLuoInvitedUser:teacherluo", "LuoInvitedCompany:www.xiaojigou.cn");


	//XJGARSDKSetOptimizationMode(2);//异步线程实现人脸对齐检测的视频模式
	XJGARSDKSetOptimizationMode(0);//视频模式,默认W
	//XJGARSDKSetOptimizationMode(1);//图片模式

	initCVEnvironment();
	initAndStartGLEnvironment();

	system("pause");
	return 0;
}


















