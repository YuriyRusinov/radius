#ifndef _HISTOGRAM_H
#define _HISTOGRAM_H

#include <qwt_plot_histogram.h>
#include "src_config.h"

class _FFTMATR_EXPORT_ Histogram : public QwtPlotHistogram
{
public:
    Histogram( const QString &, const QColor & );

    void setColor( const QColor & );
    void setValues( uint numValues, const double * );
};

#endif
