#include <QLabel>
#include <QScrollArea>
#include <QGridLayout>
#include <QPixmap>
#include <QContextMenuEvent>
#include <QMenu>
#include <QAction>
#include <QFileDialog>
#include <QDir>
#include <QImageWriter>
#include <QtDebug>

#include "rggImageWidget.h"

rggImageWidget :: rggImageWidget (QWidget * parent, Qt::WindowFlags flags)
    : QWidget (parent, flags),
    lImage (new QLabel),
    lImScroll (new QScrollArea),
    pMenu (new QMenu),
    actSaveImage (new QAction (tr("Save image"), this))
{
    QGridLayout * gLay = new QGridLayout (this);
    gLay->addWidget (lImScroll);
    lImage->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    lImScroll->setWidget (lImage);
    pMenu->addAction (actSaveImage);

    connect (actSaveImage, SIGNAL (triggered()), this, SLOT (saveImage()) );
}

rggImageWidget :: ~rggImageWidget (void)
{
    delete lImage;
    delete lImScroll;
    delete pMenu;
}

void rggImageWidget :: setImage (const QImage& image)
{
    qDebug () << __PRETTY_FUNCTION__ << image.colorCount();
    im = image;
    QPixmap pImage = QPixmap::fromImage (image);
    lImage->setPixmap (pImage);
    lImage->adjustSize();
}

void rggImageWidget :: contextMenuEvent ( QContextMenuEvent * event )
{
    //QPoint p = event->pos();
    QPoint gp = event->globalPos();
    //qDebug () << __PRETTY_FUNCTION__ << p << tCh << UI->tvHistory->viewport() ;
    pMenu->popup(gp);
    event->accept();
    //QWidget::contextMenuEvent(event);
}

void rggImageWidget :: saveImage (void)
{
    qDebug () << __PRETTY_FUNCTION__;
    QString fImName = QFileDialog :: getSaveFileName (this, tr("Save image"), QDir::currentPath(), tr("Image files (*.xpm *.png *.ico *.jpg *.jpeg *.bmp *.gif *.pbm *.pgm *.xbm);;All files (*)"));
    if (fImName.isEmpty())
        return;
    //QImageWriter imW (fImName);
    //imW.setQuality (100);
    bool isW = im.save (fImName);//imW.write (im);
    qDebug () << __PRETTY_FUNCTION__ << isW;
}
