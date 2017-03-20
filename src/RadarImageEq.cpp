#include <QtDebug>

#include <vector>
#include "xform.h"
#include "rhistwidget.h"
#include "RadarImageEq.h"

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "qimage_to_cvmat.h"
#include "cvmat_to_qimage.h"

#include "histogramCalc.h"

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

void RadiusImageEqualizer :: calcHistogram (const QImage& wImage, double * rHist, double * gHist, double * bHist, const int& nColors, const cv::Mat& wMatr, bool accum)
{
    if (wImage.isNull() || !rHist || !gHist || !bHist)
        return;
    qDebug () << __PRETTY_FUNCTION__ << wImage.format();

    QImage wImC (wImage);
    if (wImage.format() == QImage::Format_RGB32)
        wImC = wImage.convertToFormat( QImage::Format_RGB888 );
    cv::Mat wMat = QImageToCvMat (wImC);
    Q_UNUSED (wMatr);
    std::vector<cv::Mat> rgbPlanes;// (3);
    split (wMat, rgbPlanes);
    //qDebug () << __PRETTY_FUNCTION__ << rgbPlanes.size();
    float range[] = {0.0, (float)nColors};
    const float * histRange = { range };
/*    bool uniform = true; 
    bool accumulate = false;
    int histSize = nColors;
*/
    cv::Mat b_hist, g_hist, r_hist;
    Mat histImage;
    int imW = wImage.width();
    int imH = wImage.height();
    int hist_w = imW;//512;
    int hist_h = imH;//400;

    //
    // Compute the histograms:
    //
    histogramCalc (wMat, r_hist, g_hist, b_hist, nColors, histRange, histImage, hist_w, hist_h, accum);
    for (int i=0; i<nColors; i++)
    {
        rHist [i] = (double) (r_hist.at<float> (i));
        gHist [i] = (double) (g_hist.at<float> (i));
        bHist [i] = (double) (b_hist.at<float> (i));
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
    QImage wImC (wImage);
    if (wImage.format() == QImage::Format_RGB32)
        wImC = wImage.convertToFormat( QImage::Format_RGB888 );
    cv::Mat wMat = QImageToCvMat (wImC);
    cvtColor( wMat, wMat, CV_BGR2GRAY );
    cv::Mat wMatR;// (wMat.clone());
    cv::equalizeHist (wMat, wMatR);
    calcHistogram (wImage, rHist, gHist, bHist, nCol, wMat, true);
    cv::Mat wMatRes = wMat.clone();//cv::Mat::zeros (wMat.size(), wMat.type());
    this->equalizeHist (wMat, rHist, gHist, bHist, nCol, wMatRes);
/*    for (int i=0; i<nCol; i++)
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
*/
    QImage res = cvMatToQImage (wMatR);
    QImage res1 = cvMatToQImage (wMatRes);
    if (wImage.format() == QImage::Format_RGB32)
    {
        res.convertToFormat (wImage.format());
        res1.convertToFormat (wImage.format());
    }
    //    if (buffer)
//    {
//        QImage tImage ((unsigned char*)buffer, wImage.width(), wImage.height(), QImage::Format_RGB32);
//        res = tImage;
//        res.detach ();
//        delete [] buffer;
    XFormWidget * xfw = new XFormWidget (0);
    xfw->setImage (res);
    connect (xfw, SIGNAL (pHistogram (QPixmap, const cv::Mat&)), this, SLOT (viewHistogram (QPixmap, const cv::Mat&)) );
    emit histView (xfw);
    emit viewEqImage (res);
    XFormWidget * xfw1 = new XFormWidget (0);
    xfw1->setImage (res1);
    connect (xfw1, SIGNAL (pHistogram (QPixmap, const cv::Mat&)), this, SLOT (viewHistogram (QPixmap, const cv::Mat&)) );
    emit histView (xfw1);
    emit viewEqImage (res1);

//    }
    delete [] bHist;
    delete [] gHist;
    delete [] rHist;
}

void RadiusImageEqualizer :: equalizeHist (const cv::Mat& wMatr, double * rHist, double * gHist, double * bHist, int nCol, cv::Mat& wMatRes)
{
    wMatRes = wMatr.clone();//Mat::zeros (wMatr.cols, wMatr.rows, wMatr.type());
//    double * histogram = new double [nCol];
    int nr = wMatr.rows;
    int nc = wMatr.cols;
    for (int i=0; i<nr; i++)
        for (int j=0; j<nc; j++)
        {
            unsigned int sum = 0;
            int kMax = qMin (wMatr.at<float> (i, j), (float)nCol);
            for (int k=0; k<kMax; k++)
                sum += qRgb(rHist[k], gHist[k], bHist[k]);
            qDebug () << __PRETTY_FUNCTION__ << i << j << sum << wMatr.at<float> (i, j);
            wMatRes.at<float> (i, j) = sum;
            wMatRes.at<double> (i, j) = sum;
        }

//    delete [] histogram;
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
