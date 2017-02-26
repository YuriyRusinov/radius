#include <QtDebug>

#include <vector>
#include "xform.h"
#include "rhistwidget.h"
#include "RadarImageEq.h"

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "qimage_to_cvmat.h"

using std::vector;
using namespace cv;

RadiusImageEqualizer :: RadiusImageEqualizer (QObject * parent)
    : QObject(parent)
{
}

RadiusImageEqualizer :: ~RadiusImageEqualizer (void)
{
}

RadiusImageEqualizer& RadiusImageEqualizer :: operator= (const RadiusImageEqualizer&)
{
    return *this;
}

XFormWidget * RadiusImageEqualizer :: viewGolographicImage (QString fileName, QWidget *parent)
{
    XFormWidget * xformWidget = new XFormWidget(parent);
    QStyle *arthurStyle = new ArthurStyle();
    xformWidget->setStyle(arthurStyle);
    xformWidget->setFile(fileName);
    connect (xformWidget, SIGNAL (pHistogram (QPixmap, const cv::Mat&)), this, SLOT (viewHistogram (QPixmap, const cv::Mat&)) );
    return xformWidget;
}

void RadiusImageEqualizer :: viewHistogram (QPixmap pMap, const cv::Mat& wMatr)
{
    qDebug () << __PRETTY_FUNCTION__;
    QImage wImage = pMap.toImage ();
    HistWidget * hw = new HistWidget;
    int nCol = 256;
    double * rHist = new double [nCol];
    double * gHist = new double [nCol];
    double * bHist = new double [nCol];
    calcHistogram (wImage, rHist, gHist, bHist, nCol, wMatr);
    hw->setImage (wImage);
    hw->setHistData (rHist, gHist, bHist, nCol);
    delete [] bHist;
    delete [] gHist;
    delete [] rHist;
    QWidget * w = qobject_cast <QWidget *>(hw);
    connect (hw, SIGNAL (histEq (const QImage&, double, double, int, int)), this, SLOT (histogramEq (const QImage&, double, double, int, int)) );
    emit histView (w);
}

void RadiusImageEqualizer :: calcHistogram (const QImage& wImage, double * rHist, double * gHist, double * bHist, const int& nColors, const cv::Mat& wMatr)
{
    if (wImage.isNull() || !rHist || !gHist || !bHist)
        return;
    qDebug () << __PRETTY_FUNCTION__;
    cv::Mat wMat (wMatr.clone());//= QImageToCvMat (wImage);
    std::vector<cv::Mat> rgbPlanes;// (3);
    split (wMat, rgbPlanes);
    //qDebug () << __PRETTY_FUNCTION__ << rgbPlanes.size();
    float range[] = {0.0, (float)nColors};
    const float * histRange = { range };
    bool uniform = true; 
    bool accumulate = false;
    int histSize = nColors;

    cv::Mat b_hist, g_hist, r_hist;

    //
    // Compute the histograms:
    //
    calcHist( &rgbPlanes[0], 1, 0, Mat(), b_hist, 1, &histSize, &histRange, uniform, accumulate );
    //qDebug () << __PRETTY_FUNCTION__ << b_hist.size ().width << ' ' << b_hist.size ().height;
    calcHist( &rgbPlanes[1], 1, 0, Mat(), g_hist, 1, &histSize, &histRange, uniform, accumulate );
    //qDebug () << __PRETTY_FUNCTION__ << g_hist.size ().width << ' ' << b_hist.size ().height;
    calcHist( &rgbPlanes[2], 1, 0, Mat(), r_hist, 1, &histSize, &histRange, uniform, accumulate );
    //qDebug () << __PRETTY_FUNCTION__ << r_hist.size ().width << ' ' << b_hist.size ().height;
    int imW = wImage.width();
    int imH = wImage.height();
    int hist_w = imW;//512;
    int hist_h = imH;//400;
    qDebug () << __PRETTY_FUNCTION__ << hist_w << hist_h;
    int bin_w = cvRound( (double) hist_w/histSize );

    Mat histImage( hist_h, hist_w, CV_8UC3, Scalar( 0,0,0) );

    /// Normalize the result to [ 0, histImage.rows ]
    normalize(b_hist, b_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
    normalize(g_hist, g_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
    normalize(r_hist, r_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );

    for (int i=0; i<nColors; i++)
    {
        rHist [i] = 0.0;//cvRound (r_hist.at<float> (i, 0));
        gHist [i] = 0.0;//cvRound (g_hist.at<float> (i, 0));//0.0;
        bHist [i] = 0.0;//cvRound (b_hist.at<float> (i, 0));//0.0;
    }
    double sq = imW * imH;
    Q_UNUSED (sq);
    for (int i=0; i<imW; i++)
    {
        for (int j=0; j<imH; j++)
        {
            int color = wImage.pixel( i,j );
            rHist[ qRed( color ) ] += 1.0;
            gHist[ qGreen( color ) ] += 1.0;
            bHist[ qBlue( color ) ] += 1.0;
        }
    }
    double ar = (rHist[1]-rHist[0])/ (r_hist.at<float>(1)-r_hist.at<float>(0));
    double br = rHist[0] - ar * r_hist.at<float>(0);
    double ag = (gHist[1]-gHist[0])/ (g_hist.at<float>(1)-g_hist.at<float>(0));
    double bg = gHist[0] - ag * g_hist.at<float>(0);
    double ab = (bHist[1]-bHist[0])/ (b_hist.at<float>(1)-b_hist.at<float>(0));
    double bb = bHist[0] - ab * b_hist.at<float>(0);
    qDebug () << __PRETTY_FUNCTION__ << ar << br << ag << bg << ab << bb;
    for (int i=0; i<nColors; i++)
    {
        qDebug () << __PRETTY_FUNCTION__ << rHist[i] - (ar*r_hist.at<float> (i, 0)+br);// << ' ' 
//                                         << gHist[i]/imH << g_hist.at<float> (i, 0) << gHist[i] / g_hist.at<float> (i, 0) << ' '
//                                         << bHist[i]/imH << b_hist.at<float> (i, 0) << bHist[i] / b_hist.at<float> (i, 0);
    }

    for( int i = 1; i < histSize; i++ )
    {
        line( histImage, Point( bin_w*(i-1), hist_h - cvRound(b_hist.at<float>(i-1)) ) ,
                         Point( bin_w*(i), hist_h - cvRound(b_hist.at<float>(i)) ),
                         Scalar( 255, 0, 0), 2, 8, 0  );
        line( histImage, Point( bin_w*(i-1), hist_h - cvRound(g_hist.at<float>(i-1)) ) ,
                         Point( bin_w*(i), hist_h - cvRound(g_hist.at<float>(i)) ),
                         Scalar( 0, 255, 0), 2, 8, 0  );
        line( histImage, Point( bin_w*(i-1), hist_h - cvRound(r_hist.at<float>(i-1)) ) ,
                         Point( bin_w*(i), hist_h - cvRound(r_hist.at<float>(i)) ),
                         Scalar( 0, 0, 255), 2, 8, 0  );
    }
}

void RadiusImageEqualizer :: histogramEq (const QImage& wImage, double wNoiseMin, double wNoiseMax, int vRed, int vBlue)
{
    qDebug () << __PRETTY_FUNCTION__ << wNoiseMin << wNoiseMax << vRed << vBlue;
    int imSize = wImage.width()*wImage.height();
    if (wImage.isNull() || imSize == 0)
        return;
    int nCol = 256;
    double * rHist = new double [nCol];
    double * gHist = new double [nCol];
    double * bHist = new double [nCol];
    cv::Mat wMat = QImageToCvMat (wImage);
    calcHistogram (wImage, rHist, gHist, bHist, nCol, wMat);
    for (int i=0; i<nCol; i++)
    {
        rHist[i] /= imSize;
        rHist[i] -= wNoiseMin;
        rHist[i] = qMax (0.0, rHist[i]);
        gHist[i] /= imSize;
        gHist[i] -= wNoiseMin;
        gHist[i] = qMax (0.0, gHist[i]);
        bHist[i] /= imSize;
        bHist[i] -= wNoiseMin;
        bHist[i] = qMax (0.0, bHist[i]);
    }
    unsigned int * histogram = new unsigned int [nCol];
    if (!histogram)
    {
        delete [] bHist;
        delete [] gHist;
        delete [] rHist;
        return;
    }
    double racc = 0.0;
    double gacc = 0.0;
    double bacc = 0.0;
    for (int i=0; i<vRed; i++)
        histogram[ i ] = qRgb( (int)( 0.5 ),
                         (int)( 0.5 ),
                         (int)( 0.5 ));
    for (int i = vRed; i < vBlue; i++ )
    {
        racc += rHist[ i ];
        gacc += gHist[ i ];
        bacc += bHist[ i ];
        histogram[ i ] = qRgb( (int)( racc * 255.0 + 0.5 ),
                         (int)( gacc * 255.0 + 0.5 ),
                         (int)( bacc * 255.0 + 0.5 ));
    }
    for (int i=vBlue; i<nCol; i++)
        histogram[ i ] = qRgb( (int)( 0.5 ),
                         (int)( 0.5 ),
                         (int)( 0.5 ));
    unsigned int * buffer = applyHistogram (wImage, histogram);
    delete [] histogram;

    QImage res;
    if (buffer)
    {
        QImage tImage ((unsigned char*)buffer, wImage.width(), wImage.height(), QImage::Format_RGB32);
        res = tImage;
        res.detach ();
        delete [] buffer;
        XFormWidget * xfw = new XFormWidget (0);
        xfw->setImage (res);
        connect (xfw, SIGNAL (pHistogram (QPixmap, const cv::Mat&)), this, SLOT (viewHistogram (QPixmap, const cv::Mat&)) );
        emit histView (xfw);
        emit viewEqImage (res);
    }
    delete [] bHist;
    delete [] gHist;
    delete [] rHist;
}

unsigned int * RadiusImageEqualizer :: applyHistogram (const QImage &img, unsigned int *histogram)
{
    unsigned int *buf;
    int i, j, pos = 0;
    int color;

    buf = new unsigned int [img.width() * img.height() ];
    if ( !buf )
        return 0;

    for( j = 0; j < img.height(); j++ )
        for( i = 0; i < img.width(); i++ )
        {
            color = img.pixel( i,j );
            buf[ pos ] = qRgb( qRed( histogram[ qRed( color ) ] ),
                          qGreen( histogram[ qGreen( color ) ] ),
                          qBlue( histogram[ qBlue( color ) ] ));
            pos++;
        }
    return buf;
}
