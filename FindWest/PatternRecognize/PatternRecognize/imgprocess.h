#ifndef _IMGPROCESS_H_
#define _IMGPROCESS_H_

#include <opencv2/opencv.hpp>

using namespace cv;
//reference book 《图像处理、分析与机器视觉》 第三版
void DisTransform(Mat &srcimg,Mat result);			//algorithm 2.1 距离变换
void BrightnessHitogram(Mat srcimg,int *hist);		//algorithm 2.2 亮度直方图 done
void ShowHistogram(int * hist,Mat result);
void AddNoiseGaussian(Mat srcimg,float sigma);		//algorithm 2.3 高斯白噪声
void GLCMCal(Mat srcimg,Mat result);				//algorithm 4.1 灰度共生矩阵计算，用于纹理计算
void InterImage(Mat srcimg,Mat result);				//algorithm 4.2 积分图像
void HistEqualization(Mat srcimg,Mat result);		//algorithm 5.1 直方图均衡化 done
void blurWithRotateMask(Mat srcimg,Mat mask);		//algorithm 5.2 利用旋转掩膜的平滑
void MediaFilter(Mat srcimg,Mat result);			//algorithm 5.3 中值滤波
void canny(Mat srcimg,Mat result);					//algorithm 5.4 canny边缘检测算子
void harris(Mat srcimg,Mat result);					//algorithm 5.5 harris角点检测算子
void BestThreshold(Mat srcimg,int threshold);		//algorithm 6.2 最优阈值计算 
void InnerEdgeTrack(Mat srcimg,Mat result);			//algorithm 6.7 内边界跟踪
void OuterEdgeTrack(Mat srcimg,Mat result);			//algorithm 6.8 外边界跟踪

#endif