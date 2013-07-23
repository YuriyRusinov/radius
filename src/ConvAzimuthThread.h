#ifndef _ConvAzimuthThread_H
#define _ConvAzimuthThread_H

#include <QThread>
#include <QMutex>

#include <complex>
#include "matrix.h"
#include "src_config.h"

class QImage;
class ConvAzimuthPhysParameters;

class _FFTMATR_EXPORT_ ConvAzimuthThread : public QThread
{
public:
    ConvAzimuthThread (ConvAzimuthPhysParameters * cAParams, QObject * parent=0);
    virtual ~ConvAzimuthThread (void);

protected:
    void run (void);

signals:
    void sendData (complex<double> * cData, int n);
    void sendImage (QImage * im);
    void sendTime (int mSeconds);

private:
    //
    // Variables
    //
    ConvAzimuthPhysParameters * convAzParameters;
    QMutex mFile;

private:
    Q_OBJECT
};

#endif
