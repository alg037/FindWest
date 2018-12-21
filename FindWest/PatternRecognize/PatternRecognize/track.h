#ifndef _TRACK_H_
#define _TRACK_H_
#include <opencv2\opencv.hpp>;
using namespace cv;
void mouseHandler(int event, int x, int y, int flags, void *param);
void tracking(Mat frame, Mat &model, Rect &trackBox);
#endif // !_TRACK_H_

