#include <QImage>
#include <QThread>
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
    qDebug () << __PRETTY_FUNCTION__ << QThread::idealThreadCount();
}

RadarImageProc::RadarImageProc (const RadarImageProc&)
    : QObject ()
{
    qDebug () << __PRETTY_FUNCTION__;
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
    connect (cdThread, SIGNAL (sendTime(int)), this, SLOT (receiveElapsedTime(int)) );
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
    imW->setWindowTitle(tr("Result image"));
    emit sendWidget (imW);
}

void RadarImageProc::convFinished (void)
{
    QThread * thr = qobject_cast<QThread*>(sender());
    qDebug () << __PRETTY_FUNCTION__ << thr->isFinished ();
    thr->quit ();
    delete thr;//thr->deleteLater();
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
    connect (caThread, SIGNAL (sendTime(int)), this, SLOT (receiveElapsedTime(int)) );
    caThread->start();
    qDebug () << __PRETTY_FUNCTION__;
}

void RadarImageProc::receiveElapsedTime (int msec)
{
    FFTTimeWidget * fftWidget = new FFTTimeWidget;
    fftWidget->setTimeElapsed (msec);
    emit sendWidget (fftWidget);
}
