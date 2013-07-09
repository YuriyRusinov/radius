#ifndef _ConvDistThread_H
#define _ConvDistThread_H

#include <QThread>
#include <QMutex>

#include <complex>
#include "matrix.h"
#include "src_config.h"

class QWidget;

class ConvDistPhysParameters;

using std::complex;

class _FFTMATR_EXPORT_ ConvDistThread : public QThread
{
public:
    ConvDistThread (ConvDistPhysParameters * cParams, QObject * parent=0);
    virtual ~ConvDistThread (void);

protected:
    void run (void);

signals:
    void sendWidget (QWidget * w, QThread * pThread);

private:
    //
    // Variables
    //
    ConvDistPhysParameters * convParameters;
    QMutex mFile;
    double* stc2MatrAbs;

private:
    Q_OBJECT
};

#endif
