#include <QtDebug>

#include "xform.h"
#include "rhistwidget.h"
#include "RadarImageEq.h"

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
    connect (xformWidget, SIGNAL (pHistogram (QPixmap)), this, SLOT (viewHistogram (QPixmap)) );
    return xformWidget;
}

void RadiusImageEqualizer :: viewHistogram (QPixmap pMap)
{
    qDebug () << __PRETTY_FUNCTION__;
    QImage wImage = pMap.toImage ();
    HistWidget * hw = new HistWidget;
    int nCol = 256;
    double * rHist = new double [nCol];
    double * gHist = new double [nCol];
    double * bHist = new double [nCol];
    calcHist (wImage, rHist, gHist, bHist, nCol);
    hw->setImage (wImage);
    hw->setHistData (rHist, gHist, bHist, nCol);
    delete [] bHist;
    delete [] gHist;
    delete [] rHist;
    QWidget * w = qobject_cast <QWidget *>(hw);
    connect (hw, SIGNAL (histEq (const QImage&)), this, SLOT (histogramEq (const QImage&)) );
    emit histView (w);
}

void RadiusImageEqualizer :: calcHist (const QImage& wImage, double * rHist, double * gHist, double * bHist, const int& nColors)
{
    if (wImage.isNull() || !rHist || !gHist || !bHist)
        return;
    qDebug () << __PRETTY_FUNCTION__;
    for (int i=0; i<nColors; i++)
    {
        rHist [i] = 0.0;
        gHist [i] = 0.0;
        bHist [i] = 0.0;
    }
    int imW = wImage.width();
    int imH = wImage.height();
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
}

void RadiusImageEqualizer :: histogramEq (const QImage& wImage)
{
    qDebug () << __PRETTY_FUNCTION__;
    int imSize = wImage.width()*wImage.height();
    if (wImage.isNull() || imSize == 0)
        return;
    int nCol = 256;
    double * rHist = new double [nCol];
    double * gHist = new double [nCol];
    double * bHist = new double [nCol];
    calcHist (wImage, rHist, gHist, bHist, nCol);
    for (int i=0; i<nCol; i++)
    {
        rHist[i] /= imSize;
        gHist[i] /= imSize;
        bHist[i] /= imSize;
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
    for (int i = 0; i < nCol; i++ )
    {
        racc += rHist[ i ];
        gacc += gHist[ i ];
        bacc += bHist[ i ];
        histogram[ i ] = qRgb( (int)( racc * 255.0 + 0.5 ),
                         (int)( gacc * 255.0 + 0.5 ),
                         (int)( bacc * 255.0 + 0.5 ));
    }
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
