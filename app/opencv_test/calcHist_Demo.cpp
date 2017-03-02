/**
 * @function calcHist_Demo.cpp
 * @brief Demo code to use the function calcHist
 * @author
 */
#include <QImage>
#include <QtDebug>

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>

#include <qimage_to_cvmat.h>
#include <cvmat_to_qimage.h>

using std::vector;
using cv::Mat;

#include "histogramCalc.h"

/**
 * @function main
 */
int main( int, char** argv )
{
  Mat src, dst;
  QImage imSrc (argv[1]);
//  qDebug () << __FUNCTION__ << imSrc.format();
  QImage imSrcF (imSrc);
  if (imSrc.format() == QImage::Format_RGB32)
  {
      qDebug () << __FUNCTION__ << "Conversion";
      imSrcF = imSrc.convertToFormat( QImage::Format_RGB888 );
  }
  qDebug () << __FUNCTION__ << imSrcF.format();

  /// Load image
  src = imread( argv[1], 1 );

  Mat src1 = QImageToCvMat (imSrcF);
  QImage imTr = cvMatToQImage (src1);
  QImage imTr0 = imTr.convertToFormat (imSrc.format());
//  QString outFile = QString("copy_%1").arg (argv[1]);
//  QString outFile_s = QString("copy_s_%1").arg (argv[1]);
//  bool saved = imTr.save (outFile);//QString("%1_copy").arg (argv[1]));
//  bool saved_s = imTr0.save (outFile_s);

  qDebug () << __FUNCTION__ << (imTr == imSrcF) << (imTr0 == imSrc);
//  qDebug () << __FUNCTION__ << outFile << outFile_s << saved << saved_s;

  if( !src1.data )
    { return -1; }

  /// Separate the image in 3 places ( B, G and R )
//  vector<Mat> bgr_planes;
//  split( src1, bgr_planes );

  /// Establish the number of bins
  int histSize = 256;

  /// Set the ranges ( for B,G,R) )
  float range[] = { 0, 256 } ;
  const float* histRange = { range };

//  bool uniform = true; bool accumulate = false;

  Mat b_hist, g_hist, r_hist;

  /// Compute the histograms:
//  calcHist( &bgr_planes[0], 1, 0, Mat(), b_hist, 1, &histSize, &histRange, uniform, accumulate );
//  calcHist( &bgr_planes[1], 1, 0, Mat(), g_hist, 1, &histSize, &histRange, uniform, accumulate );
//  calcHist( &bgr_planes[2], 1, 0, Mat(), r_hist, 1, &histSize, &histRange, uniform, accumulate );

  // Draw the histograms for B, G and R
  int hist_w = 2*512; int hist_h = 400;
  int bin_w = cvRound( (double) hist_w/histSize );

  Mat histImage;//( hist_h, hist_w, CV_8UC3, Scalar( 0,0,0) );

  /// Normalize the result to [ 0, histImage.rows ]
//  normalize(b_hist, b_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
//  normalize(g_hist, g_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
//  normalize(r_hist, r_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
  histogramCalc (src1, r_hist, g_hist, b_hist, histSize, histRange, histImage, hist_w, hist_h);

  /// Draw for each channel
  for( int i = 1; i < histSize; i++ )
  {
      line( histImage, Point( bin_w*(i-1), (hist_h - cvRound(b_hist.at<float>(i-1))) ) ,
                       Point( bin_w*(i), (hist_h - cvRound(b_hist.at<float>(i))) ),
                       Scalar( 255, 0, 0), 2, 8, 0  );
      line( histImage, Point( bin_w*(i-1), (hist_h - cvRound(g_hist.at<float>(i-1))) ) ,
                       Point( bin_w*(i), (hist_h - cvRound(g_hist.at<float>(i))) ),
                       Scalar( 0, 255, 0), 2, 8, 0  );
      line( histImage, Point( bin_w*(i-1), (hist_h - cvRound(r_hist.at<float>(i-1))) ) ,
                       Point( bin_w*(i), (hist_h - cvRound(r_hist.at<float>(i))) ),
                       Scalar( 0, 0, 255), 2, 8, 0  );
  }

  /// Display
  namedWindow("calcHist Demo", WINDOW_AUTOSIZE );
  imshow("calcHist Demo", histImage );

  waitKey(0);

  return 0;

}
