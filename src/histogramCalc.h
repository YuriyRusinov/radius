#ifndef _HISTCALC_H
#define _HISTCALC_H

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include "src_config.h"

using cv::Mat;

void _FFTMATR_EXPORT_ histogramCalc (const Mat& src, Mat& r_hist, Mat& g_hist, Mat& b_hist, int histSize, const float* histRange, Mat& histImage, int width, int height);

#endif
