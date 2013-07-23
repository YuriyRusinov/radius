#include <QImage>
#include <QtDebug>

#include <radDataWidget.h>
#include <radTableWidget.h>
#include <rggImageWidget.h>
#include <ffttimewidget.h>
#include <convdistancewidget.h>
#include <convazimuthwidget.h>

#include "ConvAzimuthPhys.h"
#include "ConvDistPhys.h"
#include "ConvDistThread.h"
#include "ConvAzimuthThread.h"
#include "radarConvIm.h"

RadarImageProc * RadarImageProc::instance=0;

RadarImageProc * RadarImageProc::getRadarImage (QObject * parent)
{
    if (!instance)
        instance = new RadarImageProc (parent);
    return instance;
}

RadarImageProc::RadarImageProc (QObject * parent)
    : QObject (parent)
{
    instance = this;
}

RadarImageProc::RadarImageProc (const RadarImageProc&)
    : QObject ()
{
}

RadarImageProc::~RadarImageProc (void)
{
    if (instance)
    {
        delete instance;
        instance = 0;
    }
}

RadarImageProc& RadarImageProc::operator= (const RadarImageProc&)
{
    return *this;
}

ConvDistanceWidget * RadarImageProc::getCDistWidget (QWidget * parent, Qt::WindowFlags flags)
{
    ConvDistanceWidget * cW = new ConvDistanceWidget (parent, flags);

    qDebug ()  << __PRETTY_FUNCTION__;
    connect (cW, SIGNAL (setParams (ConvDistPhysParameters *)), this, SLOT (procConvDist (ConvDistPhysParameters *)) );

    return cW;
}

void RadarImageProc::procConvDist (ConvDistPhysParameters * cParams)
{
    qDebug () << __PRETTY_FUNCTION__;
    if (!cParams)
        return;
    ConvDistThread * cdThread = new ConvDistThread (cParams);
    connect (cdThread, SIGNAL (sendData (complex<double> *, int)), this, SLOT (receiveData (complex<double> *, int)) );
    connect (cdThread, SIGNAL (sendImage (QImage *)), this, SLOT (receiveImage (QImage *)) );
    connect (cdThread, SIGNAL (finished()), this, SLOT (convFinished()) );
    cdThread->start();
    qDebug () << __PRETTY_FUNCTION__;
    //delete cdThread;
}

void RadarImageProc::receiveData (complex<double> * cData, int n)
{
    qDebug () << __PRETTY_FUNCTION__;
    radDataWidget * w = new radDataWidget (cData, n);
    w->setParent (0);
    emit sendWidget (w);
}

void RadarImageProc::receiveImage (QImage * im)
{
    rggImageWidget * imW = new rggImageWidget;
    imW->setImage (*im);
    emit sendWidget (imW);
}

void RadarImageProc::convFinished (void)
{
    qDebug () << __PRETTY_FUNCTION__;
    QObject * thr = sender();
    thr->deleteLater();
}

ConvAzimuthWidget * RadarImageProc::getCAzWidget (QWidget * parent, Qt::WindowFlags flags)
{
    ConvAzimuthWidget * cW = new ConvAzimuthWidget (parent, flags);

    connect (cW, SIGNAL (sendParameters (ConvAzimuthPhysParameters *)), this, SLOT (procConvAzimuth (ConvAzimuthPhysParameters *)) );

    return cW;
}

void RadarImageProc::procConvAzimuth (ConvAzimuthPhysParameters * cParams)
{
    qDebug () << __PRETTY_FUNCTION__;
    ConvAzimuthThread * caThread = new ConvAzimuthThread (cParams);
    connect (caThread, SIGNAL (sendImage (QImage *)), this, SLOT (receiveImage (QImage *)) );
    connect (caThread, SIGNAL (finished()), this, SLOT (convFinished()) );
    caThread->start();
    qDebug () << __PRETTY_FUNCTION__;
}
