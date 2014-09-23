#ifndef _ConvDistColumnThread_H
#define _ConvDistColumnThread_H

#include <QThread>
#include <QMutex>

#include <complex>
#include "matrix.h"
#include "src_config.h"

class _FFTMATR_EXPORT_ ConvDistColumnThread : public QThread
{
public:
    ConvDistColumnThread (QObject * parent=0);
    virtual ~ConvDistColumnThread (void);

protected:
    void run (void);

private:
    Q_OBJECT
};

#endif
