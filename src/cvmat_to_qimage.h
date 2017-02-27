#ifndef _CVMatToQImage_H
#define _CVMatToQImage_H

#include <opencv2/highgui/highgui.hpp>

#include <opencv2/imgproc/imgproc.hpp>

#include "src_config.h"

class QImage;
using namespace cv;

QImage _FFTMATR_EXPORT_ cvMatToQImage( const cv::Mat &inMat );

#endif
