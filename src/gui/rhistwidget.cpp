#include <QGridLayout>
#include <QPushButton>
#include <QHBoxLayout>
#include <QtDebug>

#include <qwt_plot_histogram.h>
#include <qwt_series_data.h>
#include <qwt_plot.h>
#include "rhistwidget.h"
#include "rhistogram.h"

HistWidget :: HistWidget (const QImage& im, QWidget * parent, Qt::WindowFlags flags)
    : QWidget (parent, flags),
    wImage (im),
    m_plot (new QwtPlot(this)),
    pbEqualize (new QPushButton(tr("Equalize"), this))
{
    setWindowTitle (tr("Histogram"));
    QGridLayout * gLay = new QGridLayout (this);
    gLay->addWidget (m_plot, 0, 0, 1, 1);

    QHBoxLayout * hButtonsLay = new QHBoxLayout;
    hButtonsLay->addStretch();
    hButtonsLay->addWidget (pbEqualize);
    gLay->addLayout (hButtonsLay, 1, 0, 1, 1);

    m_plot->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    this->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
}

HistWidget :: ~HistWidget (void)
{
}

void HistWidget :: setImage (const QImage& wIm)
{
    wImage = wIm;
}

void HistWidget :: setHistData (const double * const rHist, const double * const gHist, const double * const bHist, int nColors)
{
    QwtPlotHistogram * plotRedHist = new Histogram (tr("Model histogram"), QColor(Qt::red));//new QwtPlotHistogram;
    QwtPlotHistogram * plotGreenHist = new Histogram (tr("Model histogram"), QColor(Qt::green));//new QwtPlotHistogram;
    QwtPlotHistogram * plotBlueHist = new Histogram (tr("Model histogram"), QColor(Qt::blue));//new QwtPlotHistogram;
    double *rhist = new double [nColors];
    double *ghist = new double [nColors];
    double *bhist = new double [nColors];
    double yMax = 0.0;
    int imW = wImage.width();
    int imH = wImage.height();
    int imSize = imW*imH;
    for( int i = 0; i < nColors; i++ )
    {
        rhist[ i ] = rHist[i] / imSize;
        yMax = qMax (yMax, rhist[i]);
        ghist[ i ] = gHist[i] / imSize;
        yMax = qMax (yMax, ghist[i]);
        bhist[ i ] = bHist[i] / imSize;
        yMax = qMax (yMax, bhist[i]);
    }
    (dynamic_cast <Histogram *>(plotRedHist))->setValues (nColors, rhist);
    (dynamic_cast <Histogram *>(plotGreenHist))->setValues (nColors, ghist);
    (dynamic_cast <Histogram *>(plotBlueHist))->setValues (nColors, bhist);
    plotRedHist->attach (m_plot);
    plotGreenHist->attach (m_plot);
    plotBlueHist->attach (m_plot);
    m_plot->setAxisScale(QwtPlot::yLeft, 0.0, yMax);
    m_plot->setAxisScale(QwtPlot::xBottom, 0.0, (double)nColors);//m_histogramParams->getXMin(), m_histogramParams->getXMax());
    m_plot->replot();
}
