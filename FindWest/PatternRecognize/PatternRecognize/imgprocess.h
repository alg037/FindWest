#ifndef _IMGPROCESS_H_
#define _IMGPROCESS_H_

#include <opencv2/opencv.hpp>

using namespace cv;
//reference book ��ͼ��������������Ӿ��� ������
void DisTransform(Mat &srcimg,Mat result);			//algorithm 2.1 ����任
void BrightnessHitogram(Mat srcimg,int *hist);		//algorithm 2.2 ����ֱ��ͼ done
void ShowHistogram(int * hist,Mat result);
void AddNoiseGaussian(Mat srcimg,float sigma);		//algorithm 2.3 ��˹������
void GLCMCal(Mat srcimg,Mat result);				//algorithm 4.1 �Ҷȹ���������㣬�����������
void InterImage(Mat srcimg,Mat result);				//algorithm 4.2 ����ͼ��
void HistEqualization(Mat srcimg,Mat result);		//algorithm 5.1 ֱ��ͼ���⻯ done
void blurWithRotateMask(Mat srcimg,Mat mask);		//algorithm 5.2 ������ת��Ĥ��ƽ��
void MediaFilter(Mat srcimg,Mat result);			//algorithm 5.3 ��ֵ�˲�
void canny(Mat srcimg,Mat result);					//algorithm 5.4 canny��Ե�������
void harris(Mat srcimg,Mat result);					//algorithm 5.5 harris�ǵ�������
void BestThreshold(Mat srcimg,int threshold);		//algorithm 6.2 ������ֵ���� 
void InnerEdgeTrack(Mat srcimg,Mat result);			//algorithm 6.7 �ڱ߽����
void OuterEdgeTrack(Mat srcimg,Mat result);			//algorithm 6.8 ��߽����

#endif