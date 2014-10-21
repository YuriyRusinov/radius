#ifndef _ConvDistColumnThread_H
#define _ConvDistColumnThread_H

#include <QThread>
#include <QMutex>
#include <QSemaphore>

#include <complex>
#include "matrix.h"
#include "src_config.h"

class ConvDistPhysParameters;

using std::complex;

class _FFTMATR_EXPORT_ ConvDistColumnThread : public QThread
{
public:
    ConvDistColumnThread (ConvDistPhysParameters * cParams, FILE * fidIn, FILE * fidOut, int iCol, complex<double> * _opor, int nd, QObject * parent=0);
    virtual ~ConvDistColumnThread (void);

protected:
    void run (void);

private:
    //
    // Variables
    //
    ConvDistPhysParameters * convParameters;
    QMutex fftMutex;
    QSemaphore fileSem;
    FILE * fInput;
    FILE * fOutput;
    int iColumn;
    complex<double> * m_opor;
    int nDim;
private:
    Q_OBJECT
};

#endif
