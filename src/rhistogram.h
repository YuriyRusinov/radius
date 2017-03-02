#ifndef _HISTOGRAM_H
#define _HISTOGRAM_H

#include <qwt_plot_curve.h>
#include "src_config.h"

class _FFTMATR_EXPORT_ Histogram : public QwtPlotCurve
{
public:
    Histogram( const QString &);

    void setColor( const QColor & );
    void setValues( uint numValues, const double * );
};

#endif
