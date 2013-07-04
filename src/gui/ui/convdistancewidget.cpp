#include <QPushButton>
#include <QToolButton>
#include <QtDebug>
#include "convdistancewidget.h"
#include "ui_conv_distance_widget.h"

ConvDistanceWidget :: ConvDistanceWidget (QWidget * parent, Qt::WindowFlags flags)
    : QWidget (parent, flags),
    UI (new Ui::conv_distance_widget)
{
    UI->setupUi (this);

    connect (UI->tbRggFileName, SIGNAL (clicked()), this, SLOT (loadDataFile()) );
    connect (UI->tbConvDFileName, SIGNAL (clicked()), this, SLOT (setSaveFile()) );
    connect (UI->pbStart, SIGNAL (clicked()), this, SLOT (startConv()) );
    connect (UI->pbClose, SIGNAL (clicked()), this, SLOT (close()) );
}

ConvDistanceWidget :: ~ConvDistanceWidget (void)
{
    delete UI;
}

void ConvDistanceWidget :: loadDataFile (void)
{
}

void ConvDistanceWidget :: setSaveFile (void)
{
}

void ConvDistanceWidget :: startConv (void)
{
}


