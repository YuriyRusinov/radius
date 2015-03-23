#include "RadSettings.h"
#include "radapplication.h"

_FFTMATR_EXPORT_ RadApplication * RadApplication::selfCore = 0;

RadApplication :: RadApplication (int & argc, char ** argv)
    : QApplication (argc, argv),
    rSettings (0)
{
    if (selfCore)
        qFatal("There should be only one RadApplication object");

    selfCore = this;
}

RadApplication :: ~RadApplication (void)
{
    selfCore = 0;
}

RadiusSettings * RadApplication :: getRadSettings (void)
{
    if (!rSettings)
    {
        rSettings = new RadiusSettings (QString("Radius soft"), QString("Radius calculator"));
    }

    rSettings->readSettings ();
    return rSettings;
}
