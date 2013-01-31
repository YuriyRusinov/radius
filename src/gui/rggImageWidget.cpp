#include <QLabel>
#include <QImage>
#include <QGridLayout>
#include <QPixmap>

#include "rggImageWidget.h"

rggImageWidget :: rggImageWidget (QWidget * parent, Qt::WindowFlags flags)
    : QWidget (parent, flags),
    lImage (new QLabel (this))
{
    QGridLayout * gLay = new QGridLayout (this);
    gLay->addWidget (lImage);
}

rggImageWidget :: ~rggImageWidget (void)
{
}

void rggImageWidget :: setImage (const QImage& image)
{
    QPixmap pImage = QPixmap::fromImage (image);
    lImage->setPixmap (pImage);
}
