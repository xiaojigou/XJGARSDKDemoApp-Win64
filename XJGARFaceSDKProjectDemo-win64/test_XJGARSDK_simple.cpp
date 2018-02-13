

#include <vector>
#include <iostream>
#include <fstream>
#include "XJGArSdk.h"

#include <opencv2/opencv.hpp>
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



//int		g_iCamImgWidth = 1280;
//int		g_iCamImgHeight = 720;
//int		g_iCamImgWidth = 1024;
//int		g_iCamImgHeight = 768;
int		g_iCamImgWidth = 980;
int		g_iCamImgHeight = 540;


unsigned char * g_pImgBufferRGB = NULL;
unsigned char * g_pImgBufferRGBResult = NULL;




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


#include <opencv2\opencv.hpp> 

int main(int argc, char *argv[])
{
	const char* licenseText = "Xb2SGQvurZeKn5kjjQr0S5K9skHCzvhIIZs4sVYidel9ZcA/L6SKdN+yrDilBdd2uJMwCiVT+ztQcGajt6jvvAp2pa8BaF5N1gY9guz1/SftQ/gW59NDwcX1WR0ZD9Po";
	XJGARSDKInitialization(licenseText, "testuser", "测试用户公司");
	InitVideoCapture(g_iCamImgWidth, g_iCamImgHeight);
	XJGARSDKInitOpenglEnvironment(g_iCamImgWidth, g_iCamImgHeight);
	XJGARSDKSetOptimizationMode(0);
	//XJGARSDKSetOptimizationMode(1 );


	g_pImgBufferRGB = new unsigned char[g_iCamImgWidth * g_iCamImgHeight * 3];
	g_pImgBufferRGBResult = new unsigned char[g_iCamImgWidth * g_iCamImgHeight * 3];

	while (1)
	{
		if (isCaptureDone(device))
		{
			//cv::Mat img(g_iCamImgHeight, g_iCamImgWidth, CV_8UC4, (unsigned char*)capture.mTargetBuf);
			//cv::imshow("img", img);

			bgra2rgb((unsigned char*)capture.mTargetBuf, g_pImgBufferRGB, g_iCamImgWidth, g_iCamImgHeight);
			// ..and ask for more
			doCapture(device);

			XJGARSDKRenderImage(g_pImgBufferRGB, g_iCamImgWidth, g_iCamImgHeight, g_pImgBufferRGBResult);


			cv::Mat finalResult = cv::Mat(g_iCamImgHeight, g_iCamImgWidth, CV_8UC3, (void*)g_pImgBufferRGBResult);
			cv::cvtColor(finalResult, finalResult, CV_RGB2BGR);
			cv::flip(finalResult, finalResult, 0);
			cv::imshow("test", finalResult);
			int a =cv::waitKey(1);
			if (a == 27) {
				XJGARSDKCleanUP();
				delete[] g_pImgBufferRGB;
				delete[] g_pImgBufferRGBResult;
				exit(1);
			}
		}
	}

}


















