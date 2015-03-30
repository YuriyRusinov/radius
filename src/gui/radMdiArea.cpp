#include <QPainter>
#include <QResizeEvent>
#include "radMdiArea.h"

RadMdiArea :: RadMdiArea (const QImage& im, QWidget * parent)
    : QMdiArea (parent),
    bkImage (im)
{
}

RadMdiArea :: ~RadMdiArea (void)
{
}

void RadMdiArea :: resizeEvent (QResizeEvent * resizeEvent)
{
    QImage newBackground(resizeEvent->size(), QImage::Format_ARGB32_Premultiplied);
    QPainter p(&newBackground);
    p.fillRect(newBackground.rect(), Qt::gray);

    QImage scaled = bkImage.scaled(resizeEvent->size(), Qt::IgnoreAspectRatio);//Qt::KeepAspectRatio);
    QRect scaledRect = scaled.rect();
    scaledRect.moveCenter(newBackground.rect().center());
    p.drawImage(scaledRect, scaled);
    setBackground(newBackground);
    QMdiArea::resizeEvent(resizeEvent);
}
