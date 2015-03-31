#include <QPainter>
#include <QResizeEvent>
#include <QPaintEvent>
#include <QFont>
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
    //qDebug () << __PRETTY_FUNCTION__ << rTitle;
    //pEvent->accept();
    QPainter painter (this->viewport());
    painter.setPen (Qt::yellow);
    painter.setFont(QFont("Arial", 40));
    //painter.drawRect(QRect(0, 0, width() - 1, height() - 1));
    //painter.fillRect(0, 0, 500, 500, QColor(0,220,0));
    painter.drawText (pEvent->rect(), Qt::AlignCenter, rTitle);
}
