#include "histogramCalc.h"

//using namespace cv;
using std::vector;
using cv::Scalar;
using cv::NORM_MINMAX;

void histogramCalc (const Mat& src, Mat& r_hist, Mat& g_hist, Mat& b_hist, int histSize, const float* histRange, Mat& histImage, int width, int height, bool accum)
{
    if (!src.data)
        return;
    vector<Mat> bgr_planes;
    split( src, bgr_planes );

    bool uniform = true;
    bool accumulate = accum;

    calcHist( &bgr_planes[0], 1, 0, Mat(), b_hist, 1, &histSize, &histRange, uniform, accumulate );
    calcHist( &bgr_planes[1], 1, 0, Mat(), g_hist, 1, &histSize, &histRange, uniform, accumulate );
    calcHist( &bgr_planes[2], 1, 0, Mat(), r_hist, 1, &histSize, &histRange, uniform, accumulate );

    histImage = Mat ( height, width, CV_8UC3, Scalar( 0,0,0) );

    /// Normalize the result to [ 0, histImage.rows ]
    normalize(b_hist, b_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
    normalize(g_hist, g_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
    normalize(r_hist, r_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );

}
