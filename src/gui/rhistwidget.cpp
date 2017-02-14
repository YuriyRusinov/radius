#include <QGridLayout>
#include <QPushButton>
#include <QHBoxLayout>
#include <QDoubleSpinBox>
#include <QSpinBox>
#include <QLabel>
#include <QGroupBox>
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
    wNoiseMinSp (new QDoubleSpinBox (this)),
    wNoiseMaxSp (new QDoubleSpinBox (this)),
    redColorSp (new QSpinBox (this)),
    blueColorSp (new QSpinBox (this)),
    pbEqualize (new QPushButton(tr("Equalize"), this)),
    yMax (0.0)
{
    setWindowTitle (tr("Histogram"));
    init ();
    connect (pbEqualize, SIGNAL (clicked()), this, SLOT (imageHistEq()) );
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
    wNoiseMinSp->setRange (0.0, yMax);
    wNoiseMinSp->setValue (0.0);
    wNoiseMaxSp->setRange (0.0, yMax);
    wNoiseMaxSp->setValue (yMax);
    m_plot->setAxisScale(QwtPlot::xBottom, 0.0, (double)nColors);//m_histogramParams->getXMin(), m_histogramParams->getXMax());
    m_plot->replot();
    delete [] bhist;
    delete [] ghist;
    delete [] rhist;
}

void HistWidget :: imageHistEq (void)
{
    qDebug () << __PRETTY_FUNCTION__;
    emit histEq (wImage);
}

void HistWidget :: init (void)
{
    QGridLayout * gLay = new QGridLayout (this);
    gLay->addWidget (m_plot, 0, 0, 1, 1);

    QGroupBox * gbNoise = new QGroupBox (tr("Noise filter parameters"), this);
    QLabel * lNoiseMin = new QLabel (tr("Noise min value"), this);
    wNoiseMinSp->setDecimals (6);
    wNoiseMinSp->setSingleStep (0.1e-5);
    QHBoxLayout * hNoiseMinLay = new QHBoxLayout;
    hNoiseMinLay->addWidget (lNoiseMin);
    hNoiseMinLay->addWidget (wNoiseMinSp);
    QGridLayout * gNoiseLay = new QGridLayout (gbNoise);
    gNoiseLay->addLayout (hNoiseMinLay, 0, 0, 1, 1);
    QLabel * lNoiseMax = new QLabel (tr("Noise max value"), this);
    wNoiseMaxSp->setDecimals (6);
    wNoiseMaxSp->setSingleStep (0.1e-5);
    QHBoxLayout * hNoiseMaxLay = new QHBoxLayout;
    hNoiseMaxLay->addWidget (lNoiseMax);
    hNoiseMaxLay->addWidget (wNoiseMaxSp);
    gNoiseLay->addLayout (hNoiseMaxLay, 1, 0, 1, 1);
    gLay->addWidget (gbNoise, 1, 0, 1, 1);

    QGroupBox * gbColors = new QGroupBox (tr("Color range parameters"), this);
    QGridLayout * gColorsLay = new QGridLayout (gbColors);
    QLabel * lColorRed = new QLabel (tr("Red color border"), this);
    QHBoxLayout * hColorRedLay = new QHBoxLayout;
    hColorRedLay->addWidget (lColorRed);
    hColorRedLay->addWidget (redColorSp);
    redColorSp->setRange (0, 256);
    redColorSp->setValue (0);
    gColorsLay->addLayout (hColorRedLay, 0, 0, 1, 1);
    QLabel * lColorBlue = new QLabel (tr("Blue color border"), this);
    blueColorSp->setRange (0, 256);
    blueColorSp->setValue (256);
    QHBoxLayout * hColorBlueLay = new QHBoxLayout;
    hColorBlueLay->addWidget (lColorBlue);
    hColorBlueLay->addWidget (blueColorSp);
    gColorsLay->addLayout (hColorBlueLay, 1, 0, 1, 1);
    gLay->addWidget (gbColors, 2, 0, 1, 1);
    
    QHBoxLayout * hButtonsLay = new QHBoxLayout;
    hButtonsLay->addStretch();
    hButtonsLay->addWidget (pbEqualize);
    gLay->addLayout (hButtonsLay, 3, 0, 1, 1);

    m_plot->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    this->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
}
