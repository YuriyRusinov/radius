#include <QProgressDialog>
#include <QtDebug>

#include "ImageGeneratorObject.h"
#include "ImageGolographicThread.h"

#include "radarConvIm.h"

ImageGolographicObject :: ImageGolographicObject (QObject * parent)
    : QObject (parent),
    m_ImageGol (new ImageGolographicThread (this)),
    pDialog (new QProgressDialog(tr("Creating images..."), tr("&Cancel"), 0, 0))
{
    connect (m_ImageGol, SIGNAL (setValue (int)), this, SLOT (setProgVal(int)) );
    connect (m_ImageGol, SIGNAL (getImagesData (const QVector<golographicData>&)), this, SLOT (loadImageData (const QVector<golographicData>&)) );
    connect (m_ImageGol, SIGNAL (finished()), this, SLOT (imageGenFinished()) );
    connect (m_ImageGol, SIGNAL (terminated()), this, SLOT (imageGenTerminated()) );
}

ImageGolographicObject :: ~ImageGolographicObject (void)
{
    m_ImageGol->setParent (0);
    delete pDialog;
    delete m_ImageGol;
}

void ImageGolographicObject :: generateImages (const generatingDataPlus& imageData)
{
    m_ImageGol->setData (imageData);
    int n = m_ImageGol->getImageNum();
    qDebug () << __PRETTY_FUNCTION__ << n;
    connect (pDialog, SIGNAL (canceled()), m_ImageGol, SLOT (quit()) );
    pDialog->setRange (0, n);
    pDialog->setValue (0);
    pDialog->show ();
    m_ImageGol->start ();
}

void ImageGolographicObject :: imageGenFinished (void)
{
    resD = m_ImageGol->getResults ();
    qDebug () << __PRETTY_FUNCTION__ << resD.size ();
    pDialog->setValue (resD.size ());
    pDialog->hide ();
    emit viewImages (resD);
    disconnect (pDialog, SIGNAL (canceled()), m_ImageGol, SLOT (quit()) );
}

void ImageGolographicObject :: imageGenTerminated (void)
{
    qDebug () << __PRETTY_FUNCTION__ ;
    pDialog->setValue (0);
    pDialog->hide ();
    disconnect (pDialog, SIGNAL (canceled()), m_ImageGol, SLOT (quit()) );
}

void ImageGolographicObject :: setProgVal (int value)
{
    if (m_ImageGol->isRunning())
        pDialog->setValue (value);
}

const QVector<golographicData>& ImageGolographicObject :: getImages (void) const
{
    return resD;
}

QThread * ImageGolographicObject :: imThread (void) const
{
    return m_ImageGol;
}

void ImageGolographicObject :: loadImageData (const QVector<golographicData>& r)
{
    resD = r;
}
