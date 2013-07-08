#ifndef _ConvDistThread_H
#define _ConvDistThread_H

#include <QThread>
#include <QMutex>

#include "src_config.h"

class ConvDistPhysParameters;

class _FFTMATR_EXPORT_ ConvDistThread : public QThread
{
public:
    ConvDistThread (ConvDistPhysParameters * cParams, QObject * parent=0);
    virtual ~ConvDistThread (void);

protected:
    void run (void);

private:
    //
    // Variables
    //
    ConvDistPhysParameters * convParameters;
    QMutex mFile;

private:
    Q_OBJECT
};

#endif
