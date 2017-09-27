// measureRadius.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "measureRadius.h"
#include "cv.h"
#include "cxcore.h"
#include "highgui.h"
#include "iostream"
#include "cmath"

using namespace cv;
using namespace std;

/*
// 这是导出变量的一个示例
MEASURERADIUS_API int nmeasureRadius=0;

// 这是导出函数的一个示例。
MEASURERADIUS_API int fnmeasureRadius(void)
{
	return 42;
}

// 这是已导出类的构造函数。
// 有关类定义的信息，请参阅 measureRadius.h
CmeasureRadius::CmeasureRadius()
{
	return;
}*/

const char filename[10] = "aa.bmp";
float r[2];

void removeSmallBlobs(cv::Mat& im, double size) {
	// Only accept CV_8UC1
	if (im.channels() != 1 || im.type() != CV_8U)
		return;

	// Find all contours
	std::vector<std::vector<cv::Point> > contours;
	Mat img = im.clone();
	cv::findContours(img, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

	for (int i = 0; i < contours.size(); i++)
	{
		// Calculate contour area
		double area = cv::contourArea(contours[i]);

		// Remove small objects by drawing the contour with black color
		if (area <= size)
			cv::drawContours(im, contours, i, Scalar::all(0), -1);
	}
}

Mat imfill(Mat src) {
	// detect external contours
	//
	std::vector<std::vector<cv::Point> > contours;
	findContours(src, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
	//
	// fill external contours
	//
	if (!contours.empty()) {
		for (int idx = 0; idx < contours.size(); idx++) {
			drawContours(src, contours, idx, Scalar::all(255), -1);
		}
	}
	return src;
}

void drawCricle(Mat src, IplImage* image, int canshu1, int canshu2, int yuan) {
	CvMemStorage* storage = cvCreateMemStorage(0);
	IplImage* dst = cvCreateImage(cvSize(src.cols, src.rows), IPL_DEPTH_8U, 1);
	dst->imageData = (char*)src.data;
	cvSmooth(dst, dst, CV_GAUSSIAN, canshu1, canshu1);
	CvSeq* circles = cvHoughCircles(dst, storage, CV_HOUGH_GRADIENT, canshu2,
		dst->height / 4, 300, 100, 250, 850);
	for (int i = 0; i < 1; i++) {
		float* p = (float*)cvGetSeqElem(circles, i);
		CvPoint pt = cvPoint (cvRound(p[0]), cvRound(p[1]));
		cvCircle (image, pt, cvRound(p[2]), CV_RGB(255, 0, 0), 3, 8, 0);
		r[yuan] = 2.5 * cvRound(p[2]) / 97.7294 / 2;
		//cout << r[yuan] << endl;
	}
}

void minECircle(Mat src, IplImage* image, int yuan) {
	// detect external contours
	//
	std::vector<std::vector<cv::Point> > contours;
	findContours(src, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
	//
	// fill external contours
	//
	if (!contours.empty()) {
		for (int idx = 0; idx < contours.size(); idx++) {
			float radius;
			Point2f center;
			minEnclosingCircle(Mat(contours[idx]), center, radius);
			r[yuan] = 2.5 * radius / 97.7294 / 2;
			cvCircle(image, Point(center), radius, CV_RGB(0, 0, 255), 2);
		}
	}
}

void measureRadius() {
	IplImage* pImg;
	IplImage* pImg2;
	IplImage* image;
	IplImage* temp;

	//载入图片
	pImg = cvLoadImage(filename, 0);
	cvSetImageROI(pImg, cvRect(930, 0, 1716, 1710));
	pImg2 = cvLoadImage(filename, 1);
	cvSetImageROI(pImg2, cvRect(930, 0, 1716, 1710));
	image = cvCreateImage(cvGetSize(pImg), IPL_DEPTH_8U, 1);
	cvThreshold(pImg, image, 230, 255, CV_THRESH_OTSU | CV_THRESH_BINARY);
	Mat BW(image, 0);
	Mat BWN = 255 - BW;
	removeSmallBlobs(BWN, 50000);

	Mat BW1 = imfill(BWN);
	removeSmallBlobs(BW1, 50000);
	Mat BWtemp = 255 - BW1;
	BWtemp = 255 - BWtemp;
	//imwrite("ab.png", BWtemp);
	minECircle(BWtemp, pImg2, 0);
	//drawCricle(BWtemp, pImg2, 7, 2, 0);

	Mat BW2 = BW + BW1 - 255;
	removeSmallBlobs(BW2, 50000);
	BWtemp = 255 - BW2;
	BWtemp = 255 - BWtemp;
	//imwrite("ac.png",BWtemp);
	minECircle(BWtemp, pImg2, 1);
	//drawCricle(BWtemp, pImg2, 5, 5, 1);
	BWtemp = 255 - ( BW1 - BW2 );

	temp = pImg2;
	imwrite("abcd.png", BWtemp);
	cvSaveImage("abc.png", pImg2);
	getchar();

	//释放图像
	cvReleaseImage(&pImg);
	cvReleaseImage(&pImg2);
	cvReleaseImage(&image);

}

extern "C" _declspec(dllexport) float* getWJ() {
	measureRadius();
	return r;
}
extern "C" _declspec(dllexport) float getNJ() {
	measureRadius();
	return r[1];
}
