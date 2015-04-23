#ifndef _ImageGolographicThread_H
#define _ImageGolographicThread_H

#include <QThread>

#include "src_config.h"

class _FFTMATR_EXPORT_ ImageGolographicThread : public QThread
{
public:
    ImageGolographicThread (QObject * parent=0);
    virtual ~ImageGolographicThread (void);

protected:
    void run (void);

private:
    Q_OBJECT
};

#endif
