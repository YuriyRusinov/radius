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
    qDebug () << __PRETTY_FUNCTION__ << wImage.isNull();;
    int nCol = 256;
    double * rHist = new double [nCol];
    double * gHist = new double [nCol];
    double * bHist = new double [nCol];
    calcHist (wImage, rHist, gHist, bHist, nCol);
    delete [] bHist;
    delete [] gHist;
    delete [] rHist;
}
