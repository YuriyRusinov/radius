#include <QtDebug>

#include "gui/ui/convdistancewidget.h"
#include "ConvDistPhys.h"
#include "ConvDistThread.h"
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
    cdThread->start();
    while (!cdThread->isFinished())
        ;
    qDebug () << __PRETTY_FUNCTION__;
}
