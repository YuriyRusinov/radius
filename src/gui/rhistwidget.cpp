#include <QGridLayout>
#include <QtDebug>

#include <qwt_plot_histogram.h>
#include <qwt_series_data.h>
#include <qwt_plot.h>
#include "rhistwidget.h"
#include "rhistogram.h"

HistWidget :: HistWidget (QWidget * parent, Qt::WindowFlags flags)
    : QWidget (parent, flags),
    m_plot (new QwtPlot(this))
{
    setWindowTitle (tr("Histogram"));
    QGridLayout * gLay = new QGridLayout (this);
    gLay->addWidget (m_plot, 0, 0, 1, 1);

    m_plot->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    this->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    QwtPlotHistogram * plotHist = new QwtPlotHistogram;
    QVector<QwtIntervalSample> samples(3);
    samples[0].interval = QwtInterval(0, 20);
    samples[0].value = 40;
    samples[1].interval = QwtInterval(20.01, 50);
    samples[1].interval.setBorderFlags(QwtInterval::ExcludeBorders);
    samples[1].value = 80;
    samples[2].interval = QwtInterval(50, 80);
    samples[2].value = 30;
    plotHist->setSamples (samples);//Data (QwtSeriesData<QwtIntervalSample>(&samples));//setSamples (hData);
    qDebug () << __PRETTY_FUNCTION__ << plotHist->boundingRect().size();
    plotHist->attach (m_plot);
    m_plot->setAxisScale(QwtPlot::yLeft, 0.0, 100.0);//yMax);
    m_plot->setAxisScale(QwtPlot::xBottom, 0.0, 100);//m_histogramParams->getXMin(), m_histogramParams->getXMax());

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
