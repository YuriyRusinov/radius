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
    QWidget * w = new HistWidget (pMap.toImage(), 0);
    emit histView (w);
}
