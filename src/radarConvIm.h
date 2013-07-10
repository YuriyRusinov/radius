#ifndef _RadarConvImage_H
#define _RadarConvImage_H

#include <QObject>

#include <complex>
#include "src_config.h"

class QThread;
class QImage;

class ConvDistanceWidget;
class ConvDistPhysParameters;

using std::complex;

class _FFTMATR_EXPORT_ RadarImageProc : public QObject
{
public:
    static RadarImageProc * getRadarImage (QObject * parent=0);
    ConvDistanceWidget * getCDistWidget (QWidget * parent=0, Qt::WindowFlags flags=0);

public slots:
    void procConvDist (ConvDistPhysParameters * cParams);

private slots:
    void receiveData (complex<double> * cData, int n);
    void receiveImage (QImage * im);

    void convFinished (void);

signals:
    void sendWidget (QWidget * w);

private:
    //
    // Functions
    //
    RadarImageProc (QObject * parent=0);
    RadarImageProc (const RadarImageProc&);
    virtual ~RadarImageProc (void);
    RadarImageProc& operator= (const RadarImageProc&);

private:
    //
    // Variables
    //
    static RadarImageProc * instance;

private:
    Q_OBJECT
};

#endif
