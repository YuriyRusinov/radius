#ifndef _ConvDistColumnThread_H
#define _ConvDistColumnThread_H

#include <QThread>
#include <QMutex>

#include <complex>
#include "matrix.h"
#include "src_config.h"

class ConvDistPhysParameters;

using std::complex;

class _FFTMATR_EXPORT_ ConvDistColumnThread : public QThread
{
public:
    ConvDistColumnThread (ConvDistPhysParameters * cParams, FILE * fidIn, FILE * fidOut, int iCol, QObject * parent=0);
    virtual ~ConvDistColumnThread (void);

protected:
    void run (void);

private:
    //
    // Variables
    //
    ConvDistPhysParameters * convParameters;
    FILE * fInput;
    FILE * fOutput;
    int iColumn;
private:
    Q_OBJECT
};

#endif
