#include <QPainter>
#include <QResizeEvent>
#include <QPaintEvent>
#include <QFont>
#include <QWidget>
#include <QtDebug>
#include "radMdiArea.h"

RadMdiArea :: RadMdiArea (const QImage& im, QString title, QWidget * parent)
    : QMdiArea (parent),
    bkImage (im),
    rTitle (title)
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

void RadMdiArea :: paintEvent (QPaintEvent * pEvent)
{
//    Q_UNUSED (pEvent);
    QMdiArea::paintEvent (pEvent);
    QWidget * vPort = this->viewport();
    QPainter painter (vPort);
    painter.setPen (Qt::yellow);
    painter.setFont(QFont("Arial", 60));
    //painter.drawRect(QRect(0, 0, width() - 1, height() - 1));
    //painter.fillRect(0, 0, 500, 500, QColor(0,220,0));
    painter.drawText (vPort->rect(), Qt::AlignCenter, rTitle);
}
