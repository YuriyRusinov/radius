#include "radar_conv_image.h"

RadarImageProc * RadarImageProc::instance=0;

RadarImageProc * RadarImageProc::getRadarImage (QObject * parent)
{
    if (!instance)
    {
        instance = new RadarImageProc (parent);
    }
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
