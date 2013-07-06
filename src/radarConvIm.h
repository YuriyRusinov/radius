#ifndef _RadarConvImage_H
#define _RadarConvImage_H

#include <QObject>

#include "src_config.h"

class ConvDistanceWidget;

class _FFTMATR_EXPORT_ RadarImageProc : public QObject
{
public:
    static RadarImageProc * getRadarImage (QObject * parent=0);
    ConvDistanceWidget * getCDistWidget (QWidget * parent=0, Qt::WindowFlags flags=0);

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
};

#endif
