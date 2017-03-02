#include "rhistogram.h"

#include <qwt_plot_layout.h>
#include <qwt_plot_canvas.h>
#include <qwt_plot_renderer.h>
#include <qwt_legend.h>
#include <qwt_plot_grid.h>
#include <qwt_column_symbol.h>
#include <qwt_series_data.h>
#include <qpen.h>
#include <stdlib.h>

Histogram::Histogram( const QString &title ):
    QwtPlotCurve( title )
{
    setStyle ( QwtPlotCurve::Lines );
    //QwtPlotHistogram::Columns );

    //setColor( symbolColor );
}

void Histogram::setColor( const QColor &color )
{
    QColor c = color;
    c.setAlpha( 180 );
    setBrush( QBrush( c ) );
}

void Histogram::setValues( uint numValues, const double *values )
{
    QVector<QPointF> samples( numValues );
    for ( uint i = 0; i < numValues; i++ )
    {
        //QwtInterval interval( double( i ), i + 1.0 );
        //interval.setBorderFlags( QwtInterval::ExcludeMaximum );

        samples[i] = QPointF ( (double)i, values[i]);// QwtIntervalSample( values[i], interval );
    }

    setSamples ( samples );//new QwtIntervalSeriesData( samples ) );
}
