#include "convazimuthwidget.h"
#include "ui_conv_azimuth_widget.h"

ConvAzimuthWidget :: ConvAzimuthWidget (QWidget * parent, Qt::WindowFlags flags)
    : QWidget (parent, flags),
    UI (new Ui::conv_azimuth_widget)
{
    UI->setupUi (this);
    this->init();
}

ConvAzimuthWidget :: ~ConvAzimuthWidget (void)
{
    delete UI;
}

void ConvAzimuthWidget :: loadDataFile (void)
{
}

void ConvAzimuthWidget :: setSaveFile (void)
{
}

void ConvAzimuthWidget :: startConv (void)
{
}

void ConvAzimuthWidget :: calcFQuant (const QString& text)
{
    Q_UNUSED (text);
}

void ConvAzimuthWidget :: calcNumbImp (const QString& text)
{
    Q_UNUSED (text);
}

void ConvAzimuthWidget :: init (void)
{
}
