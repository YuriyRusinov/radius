#ifndef _ImageGeneratorObject_H
#define _ImageGeneratorObject_H

#include <QObject>

#include "src_config.h"
#include <gologramma.h>

class QProgressDialog;
class ImageGolographicThread;

class _FFTMATR_EXPORT_ ImageGolographicObject : public QObject
{
public:
    void generateImages (const generatingDataPlus& data);
    const QVector<golographicData>& getImages (void) const;
    QThread * imThread (void) const;

private slots:
    void setProgVal (int value);
    void imageGenFinished (void);
    void imageGenTerminated (void);
    void loadImageData (const QVector<golographicData>& r);

signals:
    void viewImages (const QVector<golographicData>& r);

private:
    //
    // Functions
    //
    ImageGolographicObject (QObject * parent=0);
    virtual ~ImageGolographicObject (void);

    friend class RadarImageProc;
private:
    //
    // Variables
    //
    ImageGolographicThread * m_ImageGol;
    QVector<golographicData> resD;
    QProgressDialog * pDialog;
private:
    Q_OBJECT
};

#endif
