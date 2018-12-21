#include <iostream>
#include <math.h>
#include "imgprocess.h"

using namespace std;
using namespace cv;

void DisTransform(Mat &srcimg,Mat result)
{

}

void BrightnessHitogram(Mat srcimg,int *hist)
{
	uchar * pt;
	int max=0;
	for (int k = 0; k < 256; k++)
	{
		hist[k]=0;
	}

	for (int i = 0; i < srcimg.rows; i++)
	{
		pt=srcimg.ptr<uchar>(i);
		for (int j = 0; j < srcimg.cols; j++)
		{
			hist[pt[j]]++;
		}
	}

	for (int i = 0; i < 255; i++) {if (hist[i]>max) max=hist[i];}
	for (int i = 0; i < 255; i++) {hist[i]=hist[i]*255/max;}
}

void ShowHistogram(int * hist,Mat result)
{
	for (int j = 0; j < 256; j++)
	{
		rectangle(result,Point(2*j,255),Point(2*j+1,255-hist[j]),Scalar(0));
	}
}

void HistEqualization(Mat srcimg,Mat result)
{
	int hist[256],hist_inter[256];
	float LUT[256];
	int num=0;
	uchar *pt1,*pt2;
	num=srcimg.cols*srcimg.rows;
	for (int i = 0; i < 256; i++) hist[i]=0;
	BrightnessHitogram(srcimg,hist);
	for (int j = 0; j < 256; j++) if (j==0) hist_inter[j]=hist[j]; else hist_inter[j]=hist_inter[j-1]+hist[j];
	for (int k = 0; k < 256; k++) LUT[k]=(float)(hist_inter[k]*255)/num;
	for (int k = 0; k < 256; k++) LUT[k]=LUT[k]/LUT[255]*255;

	for (int l = 0; l < srcimg.rows; l++)
	{
		pt1=srcimg.ptr<uchar>(l);
		pt2=result.ptr<uchar>(l);
		for (int m = 0; m < srcimg.cols; m++)
		{
			pt2[m]=LUT[pt1[m]];
		}
	}
}