#ifndef _ImageGolographicThread_H
#define _ImageGolographicThread_H

#include <QThread>
#include <QVector>

#include "src_config.h"
#include <gologramma.h>

class _FFTMATR_EXPORT_ ImageGolographicThread : public QThread
{
public:
    ImageGolographicThread (QObject * parent=0);
    virtual ~ImageGolographicThread (void);

    void setData (const generatingDataPlus& data);
    int getImageNum (void);
    const VGolographic& getResults (void) const;

private slots:
    void setPVal (int val);
    void setAllVals (int v);

signals:
    void setValue (int value);
    void getImagesData (const VGolographic&);

protected:
    void run (void);

private:
    //
    // Variables
    //
    //ImageGenerator* generator;
    int numb;
    generatingDataPlus imageData;
    VGolographic resD;

private:
    Q_OBJECT
};

#endif
