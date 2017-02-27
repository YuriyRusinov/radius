#ifndef _QImageToCVMat_H
#define _QImageToCVMat_H

#include <opencv2/highgui/highgui.hpp>

#include <opencv2/imgproc/imgproc.hpp>

#include "src_config.h"

class QImage;
using namespace cv;

cv::Mat _FFTMATR_EXPORT_ QImageToCvMat( const QImage &inImage, bool inCloneImageData =true);

#endif
