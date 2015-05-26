#include <QGridLayout>
#include <QtDebug>

#include <qwt_plot_histogram.h>
#include <qwt_series_data.h>
#include <qwt_plot.h>
#include "rhistwidget.h"
#include "rhistogram.h"

HistWidget :: HistWidget (const QImage& im, QWidget * parent, Qt::WindowFlags flags)
    : QWidget (parent, flags),
    wImage (im),
    m_plot (new QwtPlot(this))
{
    setWindowTitle (tr("Histogram"));
    QGridLayout * gLay = new QGridLayout (this);
    gLay->addWidget (m_plot, 0, 0, 1, 1);

    m_plot->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    this->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    this->init ();
    m_plot->replot();
}

HistWidget :: ~HistWidget (void)
{
}

void HistWidget :: showEvent ( QShowEvent * event )
{
    //qDebug () << __PRETTY_FUNCTION__ << plot.sizeHint() << sizeHint();
    //plot->replot();
    QWidget::showEvent (event);
}

void HistWidget :: init (void)
{
    QwtPlotHistogram * plotRedHist = new Histogram (tr("Model histogram"), QColor(Qt::red));//new QwtPlotHistogram;
    QwtPlotHistogram * plotGreenHist = new Histogram (tr("Model histogram"), QColor(Qt::green));//new QwtPlotHistogram;
    QwtPlotHistogram * plotBlueHist = new Histogram (tr("Model histogram"), QColor(Qt::blue));//new QwtPlotHistogram;
    const int nColors = 256;
/*    QVector<QwtIntervalSample> samples(3);//nColors);
    samples[0].interval = QwtInterval(0, 20);
    samples[0].value = 40;
    samples[1].interval = QwtInterval(20.01, 50);
    samples[1].interval.setBorderFlags(QwtInterval::ExcludeBorders);
    samples[1].value = 80;
    samples[2].interval = QwtInterval(50, 80);
    samples[2].value = 30;
    plotHist->setSamples (samples);//Data (QwtSeriesData<QwtIntervalSample>(&samples));//setSamples (hData);
    qDebug () << __PRETTY_FUNCTION__ << plotHist->boundingRect().size();
*/
    double *rhist = new double [nColors];
    double *ghist = new double [nColors];
    double *bhist = new double [nColors];
    for (int i=0; i<nColors; i++)
    {
        rhist [i] = 0.0;
        ghist [i] = 0.0;
        bhist [i] = 0.0;
    }
    int imW = wImage.width();
    int imH = wImage.height();
    int imSize = imW*imH;
    for (int i=0; i<imW; i++)
    {
        for (int j=0; j<imH; j++)
        {
            int color = wImage.pixel( i,j );
            rhist[ qRed( color ) ] += 1.0;
            ghist[ qGreen( color ) ] += 1.0;
            bhist[ qBlue( color ) ] += 1.0;
        }
    }
    double yMax = 0.0;
    for( int i = 0; i < nColors; i++ )
    {
        rhist[ i ] /= imSize;
        yMax = qMax (yMax, rhist[i]);
        ghist[ i ] /= imSize;
        yMax = qMax (yMax, ghist[i]);
        bhist[ i ] /= imSize;
        yMax = qMax (yMax, bhist[i]);
    }
    (dynamic_cast <Histogram *>(plotRedHist))->setValues (nColors, rhist);
    (dynamic_cast <Histogram *>(plotGreenHist))->setValues (nColors, rhist);
    (dynamic_cast <Histogram *>(plotBlueHist))->setValues (nColors, rhist);
    plotRedHist->attach (m_plot);
    plotGreenHist->attach (m_plot);
    plotBlueHist->attach (m_plot);
    m_plot->setAxisScale(QwtPlot::yLeft, 0.0, yMax);
    m_plot->setAxisScale(QwtPlot::xBottom, 0.0, 256);//m_histogramParams->getXMin(), m_histogramParams->getXMax());


}
