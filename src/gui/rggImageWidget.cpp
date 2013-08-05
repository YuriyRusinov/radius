#include <QLabel>
#include <QScrollArea>
#include <QGridLayout>
#include <QPixmap>

#include "rggImageWidget.h"

rggImageWidget :: rggImageWidget (QWidget * parent, Qt::WindowFlags flags)
    : QWidget (parent, flags),
    lImage (new QLabel),
    lImScroll (new QScrollArea)
{
    QGridLayout * gLay = new QGridLayout (this);
    gLay->addWidget (lImScroll);
    lImage->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    lImScroll->setWidget (lImage);
}

rggImageWidget :: ~rggImageWidget (void)
{
    delete lImage;
    delete lImScroll;
}

void rggImageWidget :: setImage (const QImage& image)
{
    im = image;
    QPixmap pImage = QPixmap::fromImage (image);
    lImage->setPixmap (pImage);
    lImage->adjustSize();
}
